/******************************************************************************
 * File Name:	my_eeprom.c
 * Program:		EEPROMAccess for Fundamentals of Embedded Systems class
 * Author:		Tom Lill
 * Purpose:		Contains control functions for accessing data in eeprom
 *
 *  Date		Changed by:	Changes:
 * ---------	-----------	-------------------------------------------------------
 * 21 Nov 05	T Lill		original file
 * 22 May 06	T Lill		added interrupt.h header
 * 14 Nov 06	T Lill		added eeprom message write and read functions
 * 15 Mar 07	T Lill		removed message scrolling functions used in bonus project
 * 09 Nov 10 	T Lill		Modified for ATmega2560
 ******************************************************************************/

/********************************* Includes ***********************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "my_eeprom.h"	// WinAVR has an eeprom.h so we need to avoid confusion

/*
 * EEPROM_write
 *
 * Purpose:	Write a single byte to a specified location in EEPROM
 *
 * Inputs:	address		= The address, relative to the start of the eeprom, 
 *						  where the data will be written.
 *			data		= the data to be written
 * 
 * Returns:	nothing
 */
void EEPROM_write(unsigned int address, unsigned char data)
{
	// ASSIGNMENT:  fill in this function

	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));

	/* Set up address and Data Registers */
	EEAR = address;
	EEDR = data;

	/* Write logical 1 to EEMPE */
	EECR |= (1<<EEMPE);

	/* Start EEPROM write by setting EEPE */
	EECR |= (1<<EEPE);
}

/*
 * EEPROM_read
 *
 * Purpose: Read a single byte from a specified location in EEPROM
 *
 * Input:	address		= The address, relative to the start of the eeprom, 
 *						  from which the data will be read.
 * 
 * Returns:	a byte of data
 */
unsigned char EEPROM_read(unsigned int address)
{
	// ASSIGNMENT:  fill in this function

	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));

	/* Set up address register */
	EEAR = address;

	/* Start EEPROM read by writing EERE */
	EECR |= (1<<EERE);

	/* Return data from Data Register */
	return EEDR;
}
