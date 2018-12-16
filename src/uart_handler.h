/*
    uart_handler.h - handling content of modbus frames
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

#ifndef UART_HANDLER_H
#define	UART_HANDLER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "stdint.h"
    
void uart_handler_init(void);

void uart_handle_modbus_rx(uint8_t c);
void uart_do_modbus_tx(uint8_t c);

bool modbus_ready(void);
void handleframe(void);
void crcValid(void);

uint8_t CalcParity(uint8_t c);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_HANDLER_H */

