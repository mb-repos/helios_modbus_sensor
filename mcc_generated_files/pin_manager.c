/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.35.8
        Device            :  PIC18F25K42
        Driver Version    :  1.02
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/

#include <xc.h>
#include "pin_manager.h"
#include "stdbool.h"



void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */   
    LATA = 0x00;    
    LATB = 0x00;    
    LATC = 0x03;    

    /**
    TRISx registers
    */    
    TRISA = 0xFD;
    TRISB = 0xE3;
    TRISC = 0xF4;

    /**
    ANSELx registers
    */   
    ANSELC = 0x00;
    ANSELB = 0x00;
    ANSELA = 0x00;

    /**
    WPUx registers
    */ 
    WPUE = 0x00;
    WPUB = 0xC0;
    WPUA = 0x00;
    WPUC = 0x10;

    /**
    ODx registers
    */   
    ODCONA = 0x00;
    ODCONB = 0x00;
    ODCONC = 0x03;
    


   
    
    
    RC0PPS = 0x21;   //RC0->I2C1:SCL1;
    U1CTSPPSbits.U1CTSPPS = 0x17;   //RC7->UART1:CTS1;
    RB2PPS = 0x13;   //RB2->UART1:TX1;
    RC1PPS = 0x22;   //RC1->I2C1:SDA1;
    U1RXPPSbits.U1RXPPS = 0x0D;   //RB5->UART1:RX1;
    I2C1SDAPPSbits.I2C1SDAPPS = 0x11;   //RC1->I2C1:SDA1;
    RB3PPS = 0x14;   //RB3->UART1:TXDE1;
    RB4PPS = 0x14;   //RB4->UART1:TXDE1;
    I2C1SCLPPSbits.I2C1SCLPPS = 0x10;   //RC0->I2C1:SCL1;

}       

void PIN_MANAGER_IOC(void)
{   

}

/**
 End of File
*/