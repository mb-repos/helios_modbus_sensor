/*
    i2c_handler.h - handels i2c connection with sensor data
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

#ifndef I2C_HANDLER_H
#define	I2C_HANDLER_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#define CMD_RESET (0x01)   
#define CMD_REINIT (0x02)

#ifdef	__cplusplus
extern "C" {
#endif 
    
void I2C1_ISR ( void );
float getTemperature(uint8_t index);
uint8_t getHumidity(uint8_t index);
uint16_t getVOC(uint8_t index);
uint16_t getCO2(uint8_t index);
uint8_t getTempSensorMask();
uint8_t getVOCSensorMask();
uint8_t getCO2SensorMask();
uint8_t getCmdByte();



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_HANDLER_H */

