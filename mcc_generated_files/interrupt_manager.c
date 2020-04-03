/**
  Generated Interrupt Manager Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    interrupt_manager.c

  @Summary:
    This is the Interrupt Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description:
    This header file provides implementations for global interrupt handling.
    For individual peripheral handlers please see the peripheral driver for
    all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65
        Device            :  PIC18F25K42
        Driver Version    :  1.03
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.45 or later
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

#include "mcc.h"
#include "interrupt_manager.h"
#include "i2c_handler.h"

void  INTERRUPT_Initialize (void)
{
    // Disable Interrupt Priority Vectors (16CXXX Compatibility Mode)
    INTCON0bits.IPEN = 0;
}

void __interrupt() INTERRUPT_InterruptManager (void)
{
    // interrupt handler
    if(PIE5bits.DMA2AIE == 1 && PIR5bits.DMA2AIF == 1)
    {
        DMA2_DMAA_ISR();
    }
    else if(PIE5bits.DMA2DCNTIE == 1 && PIR5bits.DMA2DCNTIF == 1)
    {
        DMA2_DMADCNT_ISR();
    }
    else if(PIE4bits.TMR2IE == 1 && PIR4bits.TMR2IF == 1)
    {
        TMR2_ISR();
    }
    else if(PIE3bits.U1RXIE == 1 && PIR3bits.U1RXIF == 1)
    {
        UART1_Receive_ISR();
    }
    else if(PIE2bits.I2C1RXIE == 1 && PIR2bits.I2C1RXIF == 1)
    {
        I2C1_ISR();
    }
    else if(PIE3bits.I2C1TXIE == 1 && PIR3bits.I2C1TXIF == 1)
    {
        I2C1_ISR();
    }
    else if(PIE3bits.I2C1EIE == 1 && PIR3bits.I2C1EIF == 1)
    {
        I2C1_ISR();
    }
    else if(PIE3bits.I2C1IE == 1 && PIR3bits.I2C1IF == 1)
    {
        I2C1_ISR();
    }
    else if(PIE2bits.DMA1SCNTIE == 1 && PIR2bits.DMA1SCNTIF == 1)
    {
        DMA1_DMASCNT_ISR();
    }
    if(PIE4bits.TMR1IE == 1 && PIR4bits.TMR1IF == 1)
    {
        TMR1_ISR();
    }
    else
    {
        //Unhandled Interrupt
    }
}
/**
 End of File
*/
