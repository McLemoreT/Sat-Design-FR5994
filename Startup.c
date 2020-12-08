/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430FR5x9x Demo - Timer0_A3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
//
//  Description: Toggle P1.0 using software and TA_0 ISR. Timer0_A is
//  configured for up mode, thus the timer overflows when TAR counts
//  to CCR0. In this example, CCR0 is loaded with 50000.
//  ACLK = n/a, MCLK = SMCLK = TACLK = default DCO ~1MHz
//
//           MSP430FR5994
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|-->LED
//
//   William Goh
//   Texas Instruments Inc.
//   October 2015
//   Built with IAR Embedded Workbench V6.30 & Code Composer Studio V6.1
//******************************************************************************

/*
 * This code has been modified
 * Somethings to note
 *  SMCLK = Max frequency 50kHz
 *          Avg frequency 32kHz?
 *  ACLK =  Max frequency 16mHz
 *          Avg frequency ???
 *
 *
 *          Despite the information listed above, it appears that the ACLK
 *          Is running at something like 500kHz based on my testing. So
 *          if I divide the clock by 8, and divide the input to the clock by 8
 *          I get a 64 time division. Then I set CCR0 value (The value when the interrupt fire)
 *          To 60,000 then the interrupt should fire every 15 seconds
 */

#include "Startup.h"
#include <msp430.h>
int time = 0; //Keep track of how many times the interrupt fired
void Boot(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    TA0CCTL0 = CCIE;                        // TACCR0 interrupt enabled
    TA0CCR0 = 65000;
    TA0CTL = TASSEL__SMCLK + MC__UP + ID_3;        // SMCLK, UP mode, Clock divider 8
    TA0EX0 = TAIDEX_7;                          //Input divider expansion. These bits along with the ID bits select the divider for the input clock.


    //CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    //CSCTL3 = DIVA__4;                       //This should divide the clock directly, but doesn't seem t do that


    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0 w/ interrupt
    __no_operation();                       // For debugger
}

// Timer0_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if(time > 120){ //If it has been 30 minutes
        if(time > 180){ //If it has been 45 minutes
            /* Leave low power mode and go back to finish boot operations
             * Might be able to do this by calling another method in the Startup
             * file, but then we would have to ALSO call main in order to go back
             * to that. Problem to be solved.
            */

        }
        time = time + 1; //It's been 30 minutes, but not 45 yet
        //Call program function that allows operations after 30 minutes
    }
    time = time + 1; //It hasn't even been 30 minutes yet
}
