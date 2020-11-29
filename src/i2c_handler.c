/*
    i2c_handler.c - handels i2c connection with sensor data
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



#include "i2c_handler.h"
#include "interrupt_manager.h"
#include "pin_manager.h"
#include "modbus.h"

typedef enum
{
    I2C_SLAVE_ADDR_MATCH,
    I2C_SLAVE_WAIT_REG_ADDR,
    I2C_SLAVE_WRITE_REG,
    I2C_SLAVE_READ_REG
} I2C_STATUS;


uint8_t EEPROM_Buffer[] =
{
    0x7f,                                    // active Temp/Humidty sensors
    0x0f,                                    // active VOC Sensors
    0x0f,                                    // active C=2 Sensors
    0x00,                                    // cmd byte
    0x32,0x32,0x32,0x32,0x32,0x32,0x32,0x32, // Humidity 1 .. 8 (initial 50%)
    0x64,0x64,0x64,0x64,0x64,0x64,0x64,0x64, // Temperatur 1 ..8 (initial 20.0°C) 
    0x03,0x23, 0x03,0x24, 0x03,0x25, 0x03,0x26,                     // VOC 1..4
    0x03,0x10, 0x03,0x11, 0x03,0x12, 0x03,0x13                      // CO2 1..4
};

uint8_t eepromAddress    = 0;

#define EEPROM_DATASTART (4u)
#define EEPROM_NUM_FTF   (8u)
#define EEPROM_SIZE_FTF  (1u)
#define EEPROM_NUM_VOC   (4u)
#define EEPROM_SIZE_VOC  (2u)
#define EEPROM_NUM_CO2   (4u)
#define EEPROM_SIZE_CO2  (2u)

void I2C1_ISR ( void )
{
    static I2C_STATUS status = I2C_SLAVE_ADDR_MATCH;

#ifdef DEBUG_PIN
    DebugI2C_SetHigh();
#endif
    
    uint8_t  I2C1_data = 0x55;
    if ((I2C1STAT1bits.RXBF)||(PIR2bits.I2C1RXIF))
    {
        PIR2bits.I2C1RXIF=0;
        I2C1_data = I2C1RXB;
        if (status == I2C_SLAVE_WAIT_REG_ADDR)
        {
            status = I2C_SLAVE_WRITE_REG;
            if (I2C1_data < sizeof(EEPROM_Buffer))
            {
                eepromAddress = I2C1_data;
                I2C1CNT = sizeof(EEPROM_Buffer) - eepromAddress;
            }
            else
            {
                I2C1CNT = 0;                
            }
        }
        else if (status == I2C_SLAVE_WRITE_REG)
        {
            EEPROM_Buffer[eepromAddress++] = I2C1_data;
            if (eepromAddress >= sizeof(EEPROM_Buffer))
            {
                eepromAddress = 0;
            }
        }        
    }
    if (I2C1PIRbits.PCIF) //STOP
    {
        I2C1PIRbits.PCIF=0;
        PIR3bits.I2C1IF=0;
        I2C1STAT1bits.CLRBF=1;//clear I2C1TXB and TXBE
        status = I2C_SLAVE_ADDR_MATCH;
    }
    
    if((I2C1PIRbits.ADRIF)) //ADDR MATCH
    {
        I2C1PIRbits.ADRIF=0;
        PIR3bits.I2C1IF=0;
        if(1 == I2C1STAT0bits.R) // master is reading ,
        {
            status = I2C_SLAVE_READ_REG;
        }     
        else
        {
            status = I2C_SLAVE_WAIT_REG_ADDR;
        }
    }
    
    if (I2C1ERRbits.NACKIF)
    {
        I2C1ERRbits.NACKIF=0;
        PIR3bits.I2C1EIF=0;
        I2C1STAT1bits.CLRBF=1;//clear I2C1TXB and TXBE
    }

    if(status == I2C_SLAVE_READ_REG) // master is reading ,address must have been matched allready...
    {
        if((PIR3bits.I2C1TXIF) ||(I2C1STAT1bits.TXBE))
        {
            PIR3bits.I2C1TXIF=0;
            I2C1TXB = EEPROM_Buffer[eepromAddress++];

            if(sizeof(EEPROM_Buffer) <= eepromAddress)
            {
                eepromAddress = 0;    // wrap to start of eeprom page
                
            }
        }
    }

    I2C1CON0bits.CSTR=0; 
#ifdef DEBUG_PIN
    DebugI2C_SetLow();
#endif
} 

void I2C1_waitIdle()
{
//    while(I2C1STAT0bits.BFRE != 1);
}

uint8_t getHumidity(uint8_t index)
{
    if (index < EEPROM_NUM_FTF)
    {
        I2C1_waitIdle();
        INTERRUPT_GlobalInterruptDisable();
        uint8_t ivalue = EEPROM_Buffer[(index * EEPROM_SIZE_FTF) + EEPROM_DATASTART ];
        INTERRUPT_GlobalInterruptEnable();        
        return ivalue;
    }
    else
    {
        return 0x32;
    }
    
}


float getTemperature(uint8_t index)
{
    if (index < EEPROM_NUM_FTF)
    {
        I2C1_waitIdle();
        INTERRUPT_GlobalInterruptDisable();
        uint8_t ivalue = EEPROM_Buffer[(index * EEPROM_SIZE_FTF) + EEPROM_DATASTART + (EEPROM_NUM_FTF * EEPROM_SIZE_FTF)];
        INTERRUPT_GlobalInterruptEnable();        
        float value = (float)(ivalue);
        value = value / 10.0f;
        value = value + 13.0f; //(3° Offset Helios!!)
        return value;
    }
    else
    {
        return 20.0;
    }
}

uint16_t getVOC(uint8_t index)
{
    if (index < EEPROM_NUM_VOC)
    {
        uint8_t pos = (index * EEPROM_SIZE_VOC)+ EEPROM_DATASTART + ((EEPROM_NUM_FTF * EEPROM_SIZE_FTF)*2u);
        I2C1_waitIdle();
        INTERRUPT_GlobalInterruptDisable();
        uint16_t ivalue = (uint16_t)(EEPROM_Buffer[pos] << 8u);
        ivalue |= EEPROM_Buffer[pos +1u];
        INTERRUPT_GlobalInterruptEnable();   
        return ivalue;
    }
    else
    {
        return 800u;
    }
}
uint16_t getCO2(uint8_t index)
{
    if (index < EEPROM_NUM_CO2)
    {
        uint8_t pos = (index * EEPROM_SIZE_CO2)+ EEPROM_DATASTART + ((EEPROM_NUM_FTF * EEPROM_SIZE_FTF)*2u) + (EEPROM_NUM_VOC * EEPROM_SIZE_VOC);
        I2C1_waitIdle();
        INTERRUPT_GlobalInterruptDisable();
        uint16_t ivalue = (uint16_t)(EEPROM_Buffer[pos] << 8u);
        ivalue |= EEPROM_Buffer[pos +1u];
        INTERRUPT_GlobalInterruptEnable();        
        return ivalue;
    }
    else
    {
        return 900u;
    }
}

uint8_t getTempSensorMask()
{
        INTERRUPT_GlobalInterruptDisable();
        uint8_t ivalue = EEPROM_Buffer[0u];
        INTERRUPT_GlobalInterruptEnable();        
        return ivalue & TEMP_MASK;
}


uint8_t getVOCSensorMask()
{
        INTERRUPT_GlobalInterruptDisable();
        uint8_t ivalue = EEPROM_Buffer[1u];
        INTERRUPT_GlobalInterruptEnable();        
        return ivalue & VOC_MASK;
}

uint8_t getCO2SensorMask()
{
        INTERRUPT_GlobalInterruptDisable();
        uint8_t ivalue = EEPROM_Buffer[2u];
        INTERRUPT_GlobalInterruptEnable();        
        return ivalue & CO2_MASK;
}

uint8_t getCmdByte()
{
        INTERRUPT_GlobalInterruptDisable();
        uint8_t ivalue = EEPROM_Buffer[3u];
        if (ivalue)
        {
            EEPROM_Buffer[3u] = 0;
        }
        INTERRUPT_GlobalInterruptEnable();   
        return ivalue;
}

