/******************************************************************************
 * File Name:	Main.c
 * Program:		Bonus Project for Fundamentals of Embedded Systems class
 * Author:		Robert Weber + Tom Lill
 * Purpose:		Main file. Contains the Main function and other executive
 *				functions. It's the "master" of the program.
 *
 *  Date	 By:		Changes:
 * --------- ----------	-------------------------------------------------------
 * 24 Apr 02 R Weber	Original file.
 * 18 Feb 04 T Lill		modified for Winter 04 session
 * 09 May 05 T Lill		Removed deprecated functions.
 * 02 Apr 07 T Lill		Updated heartbeat function.
 * 09 Nov 10 T Lill		Modified for ATmega2560
 * 13 Mar 11 T Lill		Use port D for LEDs
 ******************************************************************************/
#include "lib.h"
#include "interrpt.h"
#include "heartbeat.h"
#include "serial.h"
#include "my_eeprom.h"

/************************* Function Prototypes ******************************/
int main(void);

/*********************** Function Implementations ******************************/

/*
 * Since all we really want to do is toggle an LED on and off, there's no
 * point in keeping track of its state.  Just initialize the heartbeat LED,
 * then write 1 to PINB which toggles the bit.
 */
void heartbeat(void)
{
	asm volatile (" sbi	0x09, 0 ");	// see warning in lib.h about SET_BIT
}

/*****************************************************************************
 * Main function.
 * 
 * Initializes hardware and software. Then enters the endless foreground loop.
 * 
 *****************************************************************************/
int main(void) 
{
	// Disable interrupts
	cli();
	
	// Set port D as an output and turn off all LEDs
	DDRD = 0xFF;
	PORTD = 0xFF;

	/* Initialize the Timer 0 */
	ISR_InitTimer0();

	// Initialize serial I/O
	SCIInitialize();
	
	/*
	 * Enable interrupts. Do as last initialization, so interrupts are
	 * not initiated until all of initialization is complete.
	 */
	sei();

	for ( ; ;)		/* Foreground loops forever */
	{   
		// Do slow tasks here
	}   /* end of endless loop */

	return 0;
}
