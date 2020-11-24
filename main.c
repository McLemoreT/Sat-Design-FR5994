#include <msp430.h> 


/**
 * main.c
 */
#include <msp430.h>
#include <Startup.h>
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    // Configure GPIO
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
	//--Initial Setup--


	if(Initial_Boot()){//check to see if this is the first time the device has booted up
	    //Go to Initial boot sequence
	    //Remember, we must wait 15 minutes before broadcasts

	}else{
	    //Find out what went wrong and resume normal operations
	    //Send a message to ground control that the system just rebooted and is back online
	}


	return 0;
}

/*
 * Checks to see if this is the first time the device has booted up
 * Or if this is a reboot from a previous failed state/power loss
 *
 * In the event that the program can not determine if this is
 * an initial boot, it should assume that it is an initial boot
 *
 */
int Initial_Boot(){

    return 1;
}
