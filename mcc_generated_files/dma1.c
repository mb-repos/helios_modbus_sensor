/**
  DMA Generated Driver File
  
  @Company
    Microchip Technology Inc.

  @File Name
    dma1.c

  @Summary
    This is the generated driver implementation file for the DMA driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for DMA CHANNEL1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65
        Device            :  PIC18F25K42
        Driver Version    :  2.0
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "dma1.h"
#include "interrupt_manager.h"

/**
  Section: Global Variables Definitions
*/

/**
  Section: DMA APIs
*/
void DMA1_Initialize(void)
{
    DMA1SSA = 0x000000; //set source start address
    DMA1DSA = 0x0000; //set destination start address 
    DMA1CON1 = 0x03; //set control register1 
    DMA1SSZ = 0x0000; //set source size
    DMA1DSZ = 0x0000; //set destination size
    DMA1SIRQ = 0x1C; //set DMA Transfer Trigger Source
    DMA1AIRQ = 0x10; //set DMA Transfer abort Source
    
    PIR2bits.DMA1DCNTIF =0; // clear Destination Count Interrupt Flag bit
    PIR2bits.DMA1SCNTIF =0; // clear Source Count Interrupt Flag bit
    PIR2bits.DMA1AIF =0; // clear abort Interrupt Flag bit
	PIR2bits.DMA1ORIF =0; // clear overrun Interrupt Flag bit
    
    PIE2bits.DMA1DCNTIE =0; // disable Destination Count 0 Interrupt
    PIE2bits.DMA1SCNTIE =1; // enable Source Count Interrupt
    PIE2bits.DMA1AIE =0; // disable abort Interrupt
    PIE2bits.DMA1ORIE =0; // disable overrun Interrupt 
	
	asm("BCF INTCON0,7");
	
	asm ("BANKSEL PRLOCK");
    asm ("MOVLW 0x55");
    asm ("MOVWF PRLOCK");
    asm ("MOVLW 0xAA");
    asm ("MOVWF PRLOCK");
    asm ("BSF PRLOCK, 0");
	
	asm("BSF INTCON0,7");
        
    DMA1CON0 = 0x00; //set control register0
}

void DMA1_DMASCNT_ISR(void)
{
    PIR2bits.DMA1SCNTIF=0;// clear Source Count Interrupt Flag 
    // add your DMA channel 1 source count 0 interrupt custom code
    DMA1CON0bits.EN = 0;
}

void DMA1_WriteUART1(uint8_t* buffer, uint8_t len)
{
    DMA1SSA = (uint24_t)buffer;
    DMA1DSA = (uint16_t)&U1TXB;
    
    DMA1SSZ = len;
    DMA1DSZ = 1;
    
    DMA1CON0bits.SIRQEN = 1;
    DMA1CON0bits.EN = 1;
}



/**
  End of File
*/