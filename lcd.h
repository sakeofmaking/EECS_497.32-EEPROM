/******************************************************************************
 * File Name:	lcd.h
 * Program:		EEPROMAccess for Fundamentals of Embedded Systems class
 * Author:		Tom Lill and Robert Weber
 * Purpose:		Header file for lcd.c file.
 *
 *  Date		Changed by:	Changes:
 * -------		-----------	-------------------------------------------------------
 * 18 Feb 04	T Lill		modified for Winter 04 session
 * 21 Nov 05	T Lill		added prototypes for displaying LCD messages
 * 30 May 07	T Lill		removed unneeded prototypes.  Added prototype of "Get"
 *							function for LCD segment table size.
 * 09 Nov 10	T Lill		modified for ATmega2560
 ******************************************************************************/
#if !defined(LCD_H)		/* Prevents including this file multiple times */
#define LCD_H

/* LCD prototypes */
unsigned GetLCDTableSize(void);
eErrorType GetLCDDRValues(char LCDChar, 
			   char *LCDDRx, char *LCDDRx5, char *LCDDRx10, char *LCDDRx15);

#endif /* LCD_H */
