/*
    uart_handler.c - handling content of modbus frames
    Copyright (C) 2018, M. Brandt>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdbool.h>
#include "uart_handler.h"
#include "modbus.h"
#include "modbus_crc.h"
#include "i2c_handler.h"
#include "tmr2.h"
#include "uart1.h"
#include "pin_manager.h"
#include "interrupt_manager.h"
#include "i2c1.h"
#include "dma1.h"
#include "dma2.h"

uint8_t modbus_head;
bool modbus_in_sync;
uint8_t modbus_frame[MODBUS_MAX_FRAMELENGTH];
uint8_t addr;
uint8_t fn_code;

bool frame_ready;

void uart_handler_init()
{
    modbus_head = 0;
    modbus_in_sync = false;
    TMR2_SetInterruptHandler(crcValid);
    TMR2_StartTimer();
}

#if 0
uint8_t CalcParity(uint8_t c)
{
    uint8_t parity = 0;
    uint8_t byte = c;
    
    while (byte > 0)
    {
        parity += (byte & 0x01u);
        byte = (uint8_t)(byte >> 1u);
    }
    parity &= 0x01;
    return parity;
}
#endif
void crcValid(void)
{
#ifdef DEBUG_PIN
    DebugPin_SetHigh();
#endif
    
    modbus_in_sync = true;
    modbus_head += DMA2_getNumBytesRead(); //addr + fc read first per IRQ;
    
    if (modbus_head > 6)
    {
        uint16_t crcRX = newCRC();

        for (uint8_t i = 0; i < modbus_head - 2u ; i++)
            crcRX = CRC_nextbyte(crcRX, modbus_frame[i]);
        
        uint16_t tmpCRC = (((uint16_t)(modbus_frame[modbus_head -1u] << 8)) | modbus_frame[modbus_head -2u ]);    
        modbus_head = 0;

        if (crcRX == tmpCRC )
        {
            INTERRUPT_GlobalInterruptDisable();
            frame_ready = true;
            tmpCRC = crcRX -1;
            INTERRUPT_GlobalInterruptEnable();    
        }
    }
    modbus_head = 0;
    U1FIFObits.RXBE = 1; //clear fifo
    U1ERRIRbits.U1RXFOIF = 0;
    PIR3bits.U1RXIF = 0;
    PIE3bits.U1RXIE = 1;

#ifdef DEBUG_PIN
    DebugPin_SetLow();
#endif
}

void uart_handle_modbus_rx(uint8_t c)
{    
    uint8_t tempSensMask;
    uint8_t vocSensMask;
    uint8_t co2SensMask;

    
    if (! modbus_in_sync)
    {
        return;
    }

#ifdef DEBUG_PIN
    DebugPin_SetHigh();
    DebugPin_SetLow();
    DebugPin_SetHigh();
    DebugPin_SetLow();
    DebugPin_SetHigh();
#endif
    
    modbus_frame[modbus_head++] = c;
    
    switch (modbus_head)
    {
        case 0:
            break; //not possible
        case 1:
            addr = c;
            tempSensMask = getTempSensorMask();            
            tempSensMask &= TEMP_MASK;
            vocSensMask = getVOCSensorMask();            
            vocSensMask &= VOC_MASK;
            co2SensMask = getCO2SensorMask();
            co2SensMask &= CO2_MASK;
            
            if ( (!(((addr & 0xf0u) == 0x30u) && (( 1u << ((addr & 0x0fu) -1u)) & tempSensMask) )) &&
                 (!(((addr & 0xf0u) == 0x50u) && (( 1u << ((addr & 0x0fu) -1u)) & vocSensMask) ))  &&
                 (!(((addr & 0xf0u) == 0x40u) && (( 1u << ((addr & 0x0fu) -1u)) & co2SensMask) ))
               )
            {
                modbus_in_sync = false; //start with next frame
                modbus_head = 0;
                PIE3bits.U1RXIE = 0; // stop reception of current frame 
            }
            break;
        case 2:
            fn_code = c;
            if ((fn_code != 4) && (fn_code != 16))
            {
                modbus_in_sync = false; //start with next frame
                modbus_head = 0;
                PIE3bits.U1RXIE = 0; // stop reception of current frame 
            }
            else
            {
                PIE3bits.U1RXIE = 0; // continue per DMA
                DMA2_ReadUART1(&modbus_frame[2u], MODBUS_MAX_FRAMELENGTH - 2u);
            }
            break;
        default:
            //nothing
            break;
    }
        
    if (modbus_head >= MODBUS_MAX_FRAMELENGTH)
    {
        //oops: should not happen!
        modbus_head = 0;
        modbus_in_sync = false;
    }

#ifdef DEBUG_PIN
    DebugPin_SetLow();
#endif
}


bool modbus_ready()
{
    INTERRUPT_GlobalInterruptDisable();
    bool ready = frame_ready;
    frame_ready = false;
    INTERRUPT_GlobalInterruptEnable();
    return ready;
}

void handleframe(void)
{
    switch (fn_code)
    {
        case 4: //Read Regs
            SendReadAnswer(addr);
            break;
        case 16:  //Write 
        {
            uint8_t len = modbus_frame[5u];
            uint8_t foo = modbus_frame[16u];

            UpdateReadAnswerFoo(addr, foo);
            SendWriteAnswer(addr, len);
            break;
        }
    }
    modbus_head = 0;
}
