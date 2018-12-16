/*
    modbus_crc.c - calculation of CRC of modbus frame
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

#include <stdint.h>

const uint16_t POLYNOM = 0xa001;


uint16_t newCRC(void)
{
    return 0xffff;
}
        
uint16_t CRC_nextbyte (uint16_t crc16, uint8_t b)
{
    crc16 = crc16 ^ b;
    for (uint8_t count = 0; count < 8; count++)
    {
        uint8_t lsb = crc16 & 0x01;
        crc16 = crc16 >> 1;
        if (lsb > 0)
            crc16 = crc16 ^ POLYNOM;
    }
    return crc16;
}






