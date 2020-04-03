/*
    modbus.c - handling content of modbus frames
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
#include <string.h>
#include <stdbool.h>
#include "modbus.h"
#include "modbus_crc.h"
#include "i2c_handler.h"
#include "interrupt_manager.h"
#include "dma1.h"
#include "i2c1.h"
#include "uart1.h"

uint8_t const defaultTempReadFrame[] = {0x31,0x04,0x28,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,
                                0x05,0x31,0x00,0x03,0x00,0x00,0x0c,0x00,0x00,0x69,0x00,
                                0x00,0x00,0xcc,0x00,0x00, 0xcc,0xcd,0x41,0xa0, 0x00,0x3a,
                                0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1b,0x00,0x00,0x11,0xe4};

uint8_t const defaultVOCReadFrame[] = {0x51,0x04,0x28,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,
                                0x07,0x51,0x00,0x03,0x00,0x00,0x0C,0x00,0x00,0x6B,0x00,
                                0x00,0x00,0xC8,0x00,0x00, 0x3C,0x00,0x46,0x1C, 0x01,0xC2,
                                0x00,0x02,0x00,0x08,0x00,0x00,0x00,0x1B,0x00,0x00,0x3D,0x20};

uint8_t const defaultCO2ReadFrame[] = {0x41,0x04,0x28,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,
                                0x06,0x41,0x00,0x03,0x00,0x00,0x0C,0x00,0x00,0x6B,0x00,
                                0x00,0x00,0xC8,0x00,0x00, 0x3C,0x00,0x46,0x1C, 0x27,0x0f,
                                0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x1B,0x00,0x00,0xc1,0x31};

uint8_t ReadAnswer[2][NUM_SENSORS][sizeof(defaultTempReadFrame)];


uint8_t WriteAnswer[8] =  {0x31,0x10,0,0,0,0,0,0};



#define INDEX_ADDR1  (0)
#define INDEX_LEN    (5)
#define INDEX_FOO    (11)
#define INDEX_ADDR2  (12)
#define INDEX_TEMP   (27) //float !!
#define INDEX_HUMID  (32)
#define INDEX_VOC    (31) //uint16
#define INDEX_CO2    (31) //uint16
#define INDEX_ADDR3  (36)
#define INDEX_CRC    (43)

#define INDEX_WCRC   (6)

uint8_t bufferselect;

bool ChangeBuffer;

void modbus_init()
{    
    for (uint8_t i = 0; i<2 ; i++)
    {
        uint8_t j = 0;
        uint8_t addr = 0;
        for (; j<MAX_TEMP_SENSCOUNT ; j++)
        {
            for (uint8_t k = 0; k< sizeof(defaultTempReadFrame) ; k++)
            {
                ReadAnswer[i][j][k] = defaultTempReadFrame[k];
            }
            ReadAnswer[i][j][INDEX_ADDR1] = 0x31u + addr;
            ReadAnswer[i][j][INDEX_ADDR2] = 0x31u + addr;
            ReadAnswer[i][j][INDEX_ADDR3] = addr;
            addr++;
            setReadCRC(j);
        }
        
        addr = 0;
        for (; j< MAX_TEMP_SENSCOUNT + MAX_VOC_SENSCOUNT ; j++)
        {
            for (uint8_t k = 0; k< sizeof(defaultVOCReadFrame) ; k++)
            {
                ReadAnswer[i][j][k] = defaultVOCReadFrame[k];
            }
            ReadAnswer[i][j][INDEX_ADDR1] = 0x51u + addr;
            ReadAnswer[i][j][INDEX_ADDR2] = 0x51u + addr;
            ReadAnswer[i][j][INDEX_ADDR3] = addr;
            addr++;
            setReadCRC(j);
        }
        
        addr = 0;
        for (; j< MAX_TEMP_SENSCOUNT + MAX_VOC_SENSCOUNT + MAX_CO2_SENSCOUNT; j++)
        {
            for (uint8_t k = 0; k< sizeof(defaultCO2ReadFrame) ; k++)
            {
                ReadAnswer[i][j][k] = defaultCO2ReadFrame[k];
            }
            ReadAnswer[i][j][INDEX_ADDR1] = 0x41u + addr;
            ReadAnswer[i][j][INDEX_ADDR2] = 0x41u + addr;
            ReadAnswer[i][j][INDEX_ADDR3] = addr;
            addr++;
            setReadCRC(j);
        }
    }
    ChangeBuffer = false;
    bufferselect = 0;
}

void setWriteCRC()
{
    uint16_t crc = newCRC();
    for (uint8_t i = 0; i< sizeof(WriteAnswer) -2u ; i++)
    {
        crc = CRC_nextbyte(crc, WriteAnswer[i]);
    }
    WriteAnswer[INDEX_WCRC]= crc & 0x00ff;
    WriteAnswer[INDEX_WCRC +1] = (crc & 0xff00) >> 8;
}
       

void setReadCRC(uint8_t index)
{
    if (index < NUM_SENSORS)
    {
        uint16_t crc = newCRC();
        for (uint8_t i = 0; i< sizeof(defaultTempReadFrame) -2u ; i++)
        {
            crc = CRC_nextbyte(crc, ReadAnswer[bufferselect][index][i]);
        }
        ReadAnswer[bufferselect][index][INDEX_CRC]= crc & 0x00ff;
        ReadAnswer[bufferselect][index][INDEX_CRC +1] = (crc & 0xff00) >> 8;
    }
}
        
        
void updateValuesTrigger(void)
{
    ChangeBuffer = true;
}

void updateValues(void)
{
    if (ChangeBuffer)
    {
        ChangeBuffer = false;
        uint8_t index = 0;
        uint8_t new_buffer = 1u - bufferselect;
        for ( uint8_t i = 0 ; i< MAX_TEMP_SENSCOUNT ; i++, index++)
        {
            ReadAnswer[new_buffer][index][INDEX_HUMID] = getHumidity(i);
            union
            {
                float f;
                uint8_t b[4];
            }u;
            u.f = getTemperature(i);
            ReadAnswer[new_buffer][index][INDEX_TEMP] = u.b[1];
            ReadAnswer[new_buffer][index][INDEX_TEMP +1] = u.b[0];
            ReadAnswer[new_buffer][index][INDEX_TEMP +2] = u.b[3];
            ReadAnswer[new_buffer][index][INDEX_TEMP +3] = u.b[2];
            setReadCRC(index);
        }
        for ( uint8_t i = 0 ; i< MAX_VOC_SENSCOUNT ; i++, index++)
        {
            union
            {
                uint16_t v;
                uint8_t b[2];
            }voc;
            voc.v = getVOC(i);
            ReadAnswer[new_buffer][index][INDEX_VOC] = voc.b[1];
            ReadAnswer[new_buffer][index][INDEX_VOC +1] = voc.b[0];
            setReadCRC(index);
        }
        for ( uint8_t i = 0 ; i< MAX_CO2_SENSCOUNT ; i++, index++)
        {
            union
            {
                uint16_t v;
                uint8_t b[2];
            }co2;
            co2.v = getCO2(i);
            ReadAnswer[new_buffer][index][INDEX_CO2] = co2.b[1];
            ReadAnswer[new_buffer][index][INDEX_CO2 +1] = co2.b[0];
            setReadCRC(index);
        }
        while (DMA1CON0bits.EN == 1);
        INTERRUPT_GlobalInterruptDisable();
        bufferselect = new_buffer;
        INTERRUPT_GlobalInterruptEnable();
    }
}

void SendReadAnswer(uint8_t addr)
{
    uint8_t index = addr2index(addr);
    DMA1_WriteUART1(ReadAnswer[bufferselect][index], sizeof(defaultTempReadFrame));
    }

void UpdateReadAnswerFoo(uint8_t addr, uint8_t foo)
{
    uint8_t index = addr2index(addr);
    INTERRUPT_GlobalInterruptDisable();
    ReadAnswer[bufferselect][index][INDEX_FOO] = foo;
    INTERRUPT_GlobalInterruptEnable();
}

void SendWriteAnswer(uint8_t addr, uint8_t len)
{
    WriteAnswer[INDEX_ADDR1] = addr;
    WriteAnswer[INDEX_LEN] = len;
    setWriteCRC();
    
    DMA1_WriteUART1(WriteAnswer, sizeof(WriteAnswer) );
}

uint8_t addr2index(uint8_t addr)
{
    uint8_t index = (addr & 0x0fu) -1u;
    uint8_t type = ((addr & 0xf0u) >> 4) -3u;
    if ((addr > 0x30) && (addr < 0x58))
    {
        switch (type)
        {
            case 0:  // 0x3x
                break;
            case 2: // 0x5x
                index += MAX_TEMP_SENSCOUNT;
                break;
            case 1: // x04x
                index += MAX_TEMP_SENSCOUNT + MAX_VOC_SENSCOUNT;
                break;
        }
    }
    return index;
}