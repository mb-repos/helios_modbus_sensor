/**
  DMA Generated Driver File
  
  @Company
    Microchip Technology Inc.

  @File Name
    dma2.c

  @Summary
    This is the generated driver implementation file for the DMA driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for DMA CHANNEL2.
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
#include "dma2.h"
#include "interrupt_manager.h"
#include "pin_manager.h"
/**
  Section: Global Variables Definitions
*/
uint16_t bytesRead = 0;
/**
  Section: DMA APIs
*/
void DMA2_Initialize(void)
{
    DMA2SSA = 0x000000; //set source start address
    DMA2DSA = 0x0000; //set destination start address 
    DMA2CON1 = 0x60; //set control register1 
    DMA2SSZ = 0x0000; //set source size
    DMA2DSZ = 0x0000; //set destination size
    DMA2SIRQ = 0x1B; //set DMA Transfer Trigger Source
    DMA2AIRQ = 0x22; //set DMA Transfer abort Source
    
    PIR5bits.DMA2DCNTIF =0; // clear Destination Count Interrupt Flag bit
    PIR5bits.DMA2SCNTIF =0; // clear Source Count Interrupt Flag bit
    PIR5bits.DMA2AIF =0; // clear abort Interrupt Flag bit
	PIR5bits.DMA2ORIF =0; // clear overrun Interrupt Flag bit
    
    PIE5bits.DMA2DCNTIE =1; // enable Destination Count 0 Interrupt
    PIE5bits.DMA2SCNTIE =0; // disable Source Count Interrupt
    PIE5bits.DMA2AIE =1; // enable abort Interrupt
    PIE5bits.DMA2ORIE =0; // disable overrun Interrupt 
	
	asm("BCF INTCON0,7");
	
	asm ("BANKSEL PRLOCK");
    asm ("MOVLW 0x55");
    asm ("MOVWF PRLOCK");
    asm ("MOVLW 0xAA");
    asm ("MOVWF PRLOCK");
    asm ("BSF PRLOCK, 0");
	
	asm("BSF INTCON0,7");
        
    DMA2CON0 = 0x00; //set control register0
}


void DMA2_DMADCNT_ISR(void)
{
    PIR5bits.DMA2DCNTIF=0; // clear Destination Count Interrupt Flag 
    // add your DMA channel 2 destination count 0 interrupt custom code
    bytesRead = DMA2DSZ - DMA2DCNT;
    DMA2CON0bits.EN = 0;
}

void DMA2_DMAA_ISR(void)
{
    PIR5bits.DMA2AIF=0;// clear abort Interrupt Flag 
    // add your DMA channel 2 abort interrupt custom code
    bytesRead = DMA2DSZ - DMA2DCNT;
    DMA2CON0bits.EN = 0;
}


uint16_t DMA2_getNumBytesRead()
{
    uint16_t r;
    INTERRUPT_GlobalInterruptDisable();
    r = bytesRead;
    bytesRead = 0;
    INTERRUPT_GlobalInterruptEnable();
    return r;
}

void DMA2_ReadUART1(uint8_t* buffer, uint8_t len)
{
    DMA2SSA = (unsigned short long)&U1RXB;
    DMA2DSA = (unsigned short) buffer;
    
    DMA2SSZ = 1;
    DMA2DSZ = len;
    
    DMA2CON0bits.SIRQEN = 1;
    DMA2CON0bits.AIRQEN = 1;
    DMA2CON0bits.EN = 1;
}

/**
  End of File
*/