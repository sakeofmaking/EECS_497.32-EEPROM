/******************************************************************************
 * File Name:	serial.h
 * Program:		Project for Fundamentals of Embedded Systems class
 * Author:		Robert Weber
 * Purpose:		Contains external definitions for serial.c file. 
 *
 *  Date	`Changed by:	Changes:
 * ---------	-----------	-------------------------------------------------------
 * 18 Feb 02	R Weber		Original file.
 *  2 Mar 02	R Weber     Moved error codes to here.
 * 21 Nov 05	T Lill		modified size of input string
 * 22 May 06	T Lill		modified size of output string
 ******************************************************************************/
#if !defined(SERIAL_H)		/* Prevents including this file multiple times */
#define SERIAL_H

#include <avr/pgmspace.h>

/* Maximum size of I/O strings */
#define MAX_OUT_STR_SIZE                250
#define MAX_IN_STR_SIZE                 22		// 1 extra for the null and return

/* Function Prototypes */
void SCIInitialize(void);
int  SCIWriteString(char *);
char SCIReadChar(void);
int  SCIWriteString_P(PGM_P Str_P);

#endif /* SERIAL_H */
