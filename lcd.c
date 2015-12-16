/******************************************************************************
 * File Name:	lcd.c
 * Program:		EEPROMAccess for Fundamentals of Embedded Systems class
 * Author:		Robert Weber, Tom Lill	
 * Purpose:		Contains control functions for LCD on STK502
 *
 *  Date		Changed by:	Changes:
 * ---------	-----------	-------------------------------------------------------
 * 18 Feb 04	T Lill		modified for Winter 04 session
 * 16 Aug 04	T Lill		added '?' and '*' to NumSegments array
 * 21 Nov 05	T Lill		expanded LCD character table to include '*' to 'z'
 * 08 May 06	T Lill		replaced signal.h with interrupt.h
 * 14 Nov 06	T Lill		redefined @ as 'hourglass'
 * 15 Mar 07	T Lill		removed LCD message handlers.  Expanded LCD character
 *							table.  '@' 
 * 16 Nov 07	T Lill		simplified extraction of LCD segment data from EERROM.
 *							extended check for allowable LCD characters to include
 *							the entire character table.  removed temporary variable
 *							from initialization of control registers.
 * 04 Dec 07	T Lill		Corrected error in test for valid LCD characters.
 * 09 Nov 10	T Lill		Modified for use with ATmega2560, which does not have
 *							LCD capability.  Just used for data access.
 ******************************************************************************/

/********************************* Includes ***********************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "avr\pgmspace.h"

#include "interrpt.h"
#include "errors.h"
#include "lib.h"
#include "lcd.h"
#include "my_eeprom.h"

/* **************************** Constant definitions *************************/


/******************************** Global variables ***************************/

/* 
 * This array defines what segments need to be turned on for each character.
 * The lowest nibble gets written to the lowest LCDDR register, the highest
 * nibble to the highest LCDDR register.
 * There are 55 entries in the table, so 110 bytes of eeprom are used up.
 * Of course, the ATmega2560 doesn't have an LCD.  But if it did, this
 * would be useful for minimizing RAM usage.
 */
// ASSIGNMENT:  store this array in the EEPROM, not in text space or RAM
const unsigned int LCDSegments[] __attribute__ ((section (".eeprom"))) = 
{
	0xEAA8,		// '*'
	0x2A80,		// '+'
	0x4000,		// ','
	0x0A00,		// '-'
	0x2000,		// '.'
	0x4008,		// '/'
	0x5559,		// '0'
	0x0118,		// '1'
	0x1e11,		// '2'
	0x1b11,		// '3'
	0x0b50,		// '4'
	0x1b41,		// '5'
	0x1f41,		// '6'
	0x0111,		// '7'
	0x1f51,		// '8'
	0x1b51,		// '9'
	0x2080,		// ':'
	0x4080,		// ';'
	0x8008,		// '<'
	0x1001,		// '='
	0x4020,		// '>'
	0x2811,		// '?'
	0xD029,		// '@' (Not defined: display 'hourglass')
	0x0f51,		// 'A'
	0x3991,		// 'B'
	0x1441,		// 'C'
	0x3191,		// 'D'
	0x1e41,		// 'E'
	0x0e41,		// 'F'
	0x1d41,		// 'G'
	0x0f50,		// 'H'
	0x2080,		// 'I'
	0x1510,		// 'J'
	0x8648,		// 'K'
	0x1440,		// 'L'
	0x0578,		// 'M'
	0x8570,		// 'N'
	0x1551,		// 'O'
	0x0e51,		// 'P'
	0x9551,		// 'Q'
	0x8e51,		// 'R'
	0x9021,		// 'S'
	0x2081,		// 'T'
	0x1550,		// 'U'
	0x4448,		// 'V'
	0xc550,		// 'W'
	0xc028,		// 'X'
	0x2028,		// 'Y'
	0x5009,		// 'Z'
	0x1441,		// '['
	0x8020,		// '\'
	0x1111,		// ']'
	0xC000,		// '^'
	0x1000,		// '_'
	0x0008		// '''
};

/**************************** Function prototypes ****************************/

/******************************************************************************
 * Get values for LCDDR registers
 *****************************************************************************/
eErrorType GetLCDDRValues(char LCDChar, 
			   char *LCDDRx, char *LCDDRx5, char *LCDDRx10, char *LCDDRx15)
{
	int index;

	if (LCDChar == ' ')
	{
		*LCDDRx   = 0x00;
		*LCDDRx5  = 0x00;
		*LCDDRx10 = 0x00;
		*LCDDRx15 = 0x00;
		return NO_ERROR;
	}

	else if ((LCDChar >= '*') && (LCDChar <= '`'))
	{
		index = LCDChar - '*';
	}

	else if ((LCDChar >= 'a' ) && (LCDChar <= 'z'))
	{
		// Convert to upper case
		index = LCDChar - '*' - 0x20;
	}

	else 
	{
		return LCD_INVALID_CHAR;
	}
	/*
	 * The character is valid but not a space, so load the registers.
	 *
	 * ASSIGNMENT:  fetch the segment data for the selected character
	 * from EEPROM.  It will be sent to the terminal for display.
	 */

	/* Get the LSB */
	int LCDsegs = EEPROM_read(2 * index);

	/* Get the MSB */
	int LCDsegs2 = EEPROM_read(2 * index + 1);

	/* Get the 4 bytes */
	*LCDDRx = LCDsegs & 0x000F;
	*LCDDRx5 = (LCDsegs & 0x00F0) >> 4;
	*LCDDRx10 = LCDsegs2 & 0x000F;
	*LCDDRx15 = (LCDsegs2 & 0x00F0) >> 4;

	return NO_ERROR;
}

unsigned GetLCDTableSize(void)
{
	return sizeof(LCDSegments);
}
