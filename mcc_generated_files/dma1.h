/**
  DMA Generated Driver API Header File
  
  @Company
    Microchip Technology Inc.

  @File Name
    dma1.h
	
  @Summary
    This is the generated header file for the DMA driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for DMA CHANNEL1.
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
#ifndef DMA1_H
#define DMA1_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
        
/**
  Section: Interface Routines
*/

/**
  @Summary
    This function initializes DMA channel 1 instance : 

  @Description
    This routine initializes the DMA channel driver instance for : 
    index, making it ready for clients to open and use it. It also initializes any
    internal data structures.
    This routine must be called before any other DMA routine is called. 

  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Comment
    
 
  @Example
    <code>
        unsigned short int srcArray[100];
        unsigned short int dstArray[100];
        int i;
        int count;
        for (i=0; i<100; i++)
        {
            srcArray[i] = i+1;
            dstArray[i] = 0;
        }
        DMA1_Initialize();
        // lock priority for accessing program flash
        asm ("BANKSEL PRLOCK");
        asm ("MOVLW 0x55");
        asm ("MOVWF PRLOCK");
        asm ("MOVLW 0xAA");
        asm ("MOVWF PRLOCK");
        asm ("BSF PRLOCK, 0");
    //  hardware trigger source set as Timer 1
        TMR1_StartTimer();// start the trigger source 
    </code>

*/
void DMA1_Initialize(void);

/**
  @Summary
    DMA channel  1 source count 0 Interrupt Service Routine

  @Description
    The DMA channel 1 will trigger an interrupt when the DMAxSCNT<11:0> reaches zero and 
 * is reloaded to its starting value.
 * DMA source count 0 Interrupt Service Routine is called by the Interrupt Manager.
    User can write the code in this function.

  @Preconditions
    Initialize  the DMA channel 1 module with source count reaching 0 interrupt before calling this isr.

  @Param
    None

  @Returns
    None

  @Example
    None
*/
void DMA1_DMASCNT_ISR(void);

void DMA1_WriteUART1(uint8_t* buffer, uint8_t len);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
    
#endif  // dma1.h