#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	//--Initial Setup--


	if(Initial_Boot){//check to see if this is the first time the device has booted up
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
bool Initial_Boot(){

    return true;
}
