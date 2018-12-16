/**
  I2C1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    i2c1.c

  @Summary
    This is the generated header file for the I2C1 driver using 
    PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for I2C1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65
        Device            :  PIC18F25K42
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45
        MPLAB 	          :  MPLAB X 4.10	
*/
/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/
/*
 This file provides some basic functions for common operations on the i2c API
 */
#include <xc.h>
#include <stdbool.h>
#include "i2c1.h"
#include "mcc.h"
#include "pin_manager.h"

/**
 Section: Global Variables
*/
   
/**
 Section: Local Functions
*/

/**
  Prototype:        void I2C1_Initialize(void)
  Input:            none
  Output:           none
  Description:      I2C1_Initialize is an
                    initialization routine that takes inputs from the GUI.
  Comment:          
  Usage:            I2C1_Initialize();

*/
void I2C1_Initialize(void)
{
	
		// ADR 9; 
		I2C1ADR0 = 0x09;
		// ADR 0; 
		I2C1ADR1 = 0x00;
		// ADR 0; 
		I2C1ADR2 = 0x00;
		// ADR 0; 
		I2C1ADR3 = 0x00;
		// TXU 0; CSD Clock Stretching enabled; ACKT 0; RXO 0; ACKDT Acknowledge; ACKSTAT ACK received; ACKCNT Acknowledge; 
		I2C1CON1 = 0x00;
		// ABD enabled; GCEN disabled; ACNT disabled; SDAHT 300 ns hold time; BFRET 8 I2C Clock pulses; FME disabled; 
		I2C1CON2 = 0x00;
		// CLK Fosc/4; 
		I2C1CLK = 0x00;
		// CNT 0; 
		I2C1CNT = 0x00;
		// CSTR Enable clocking; S Cleared by hardware after Start; MODE 7-bit address; EN enabled; RSEN disabled; 
		I2C1CON0 = 0x80;

    PIR2bits.I2C1RXIF=0;
    PIR3bits.I2C1TXIF=0;
    PIR3bits.I2C1EIF=0;
    I2C1ERRbits.NACKIF=0;
    PIR3bits.I2C1IF=0;
    I2C1PIRbits.PCIF=0;
    I2C1PIRbits.ADRIF=0;
    
    PIE2bits.I2C1RXIE=1;//enable I2C RX interrupt
    PIE3bits.I2C1TXIE=1;//enable I2C TX interrupt
    PIE3bits.I2C1EIE=1;//enable I2C error interrupt
    I2C1ERRbits.NACKIE=1;//enable I2C error interrupt for NACK
    PIE3bits.I2C1IE=1;//enable I2C  interrupt
    I2C1PIEbits.PCIE=1;//enable I2C interrupt for stop condition
    I2C1PIEbits.ADRIE=1;//enable I2C interrupt for I2C address match condition
	
    
    I2C1PIR = 0;//    ;Clear all the error flags
    I2C1ERR = 0;
 
    I2C1ADR0 = (uint8_t)(I2C1ADR0 << 1);
}

