/******************************************************************************
 * File Name:	my_eeprom.h
 * Program:		EEPROMAccess for Fundamentals of Embedded Systems class
 * Author:		Tom Lill
 * Purpose:		Header file for my_eeprom.h
 *
 *  Date		Changed by:	Changes:
 * ---------	-----------	-------------------------------------------------------
 * 21 Nov 05	T Lill		original file
 * 14 Nov 06	T Lill		added prototypes for eeprom message write function
 * 14 Mar 07	T Lill		removed unused function prototypes
 ******************************************************************************/

#if !defined(MY_EEPROM_H)		/* Prevents including this file multiple times */
#define MY_EEPROM_H


/******************************** Public constants *****************************/


/****************************** Function prototypes ****************************/
unsigned char	EEPROM_read(unsigned int address);
void			EEPROM_write(unsigned int address, unsigned char data);

#endif
