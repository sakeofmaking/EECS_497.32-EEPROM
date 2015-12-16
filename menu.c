/******************************************************************************
 * File Name:	menu.c
 * Program:		Project for Fundamentals of Embedded Systems class
 * Author:		Robert Weber
 * Purpose:		Contains menu functions.
 *
 *  Date	Changed by:		Changes:
 * -------	-----------		----------------------------------------------------
 * 17 Aug 03	R Weber		Copied from 68HC11 code.
 * 01 Sep 04	T Lill		Added * and ? to lcd menu option
 * 22 May 06	T Lill		Modified menu system to display 2 lines per loop. 
 *							Changed 'debug' prompt to 'cmd.'
 * 09 Nov 10	T Lill		Converted program to ATmega2560
 ******************************************************************************/
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "lib.h"
#include "serial.h"
#include "errors.h"
#include "menu.h"
#include "lcd.h"
#include "my_eeprom.h"

#define MAX_MEM_SIZE 0x20

// Enumeration for the menuing system
typedef enum {
    TOP_MENU,
    DISPLAY_HELP_MENU1,
    DISPLAY_HELP_MENU2,
    DISPLAY_HELP_MENU3,
	GET_CHAR_FOR_EEPROM,
	GET_EEPROM_READ_ADDR,
	GET_EEPROM_WRITE_ADDR
} DebugMenuStateType;

/******************************************************************************
 * Processes keypresses received via RS-232. Implements a menuing system.
 ******************************************************************************/
static char zInputStr[MAX_IN_STR_SIZE];
static char *ptrInputStr = zInputStr;
static DebugMenuStateType MenuState = TOP_MENU;
static unsigned EEPROMAddr;
static char LCDChar;

void RunMenu(void)
{
    char cTempChar = 1;     // Set to any value other than 0
    char zOutputStr[MAX_MEM_SIZE + 3];  // Add space for newline, return and NULL
    char *ptrOutputStr;
	char LCDDRx, LCDDRx5, LCDDRx10, LCDDRx15;
    eErrorType error;

    // Read input characters until input buffer is empty
    while ((cTempChar = SCIReadChar()) != 0)
    {   // Have another character from input buffer
        if (cTempChar == '\r')
        {   // Enter character. Process input
            *ptrInputStr = '\0';        // append Null 
            ptrInputStr = zInputStr;    // Reset input string
            SCIWriteString_P(PSTR("\n\r"));     // Move cursor to next line

            // Process entry based on debug menu state
            switch(MenuState)
            {
                case TOP_MENU:
                    if (strcmp(zInputStr, "?") == 0)
                    {   // Help screen
						MenuState = DISPLAY_HELP_MENU1;
                    }

                    else if (strcmp(zInputStr, "ge") == 0)
                    {   // Display desired signal parameters
                        // Retrieve signal parameters
                        error = GetError();
                        SCIWriteString_P(PSTR("  Error = "));
                        ptrOutputStr = zOutputStr;
                        _itoa(&ptrOutputStr, error, 10);
                        SCIWriteString(zOutputStr);
                        SCIWriteString_P(PSTR("\n\r"));
                    }

                    else if (strcmp(zInputStr, "ce") == 0)
                    {
                        ClearError();
                    }                   

                    else if (strcmp(zInputStr, "wc") == 0)
                    {
                        SCIWriteString_P(PSTR("  Enter char to store: "));
                        MenuState = GET_CHAR_FOR_EEPROM;
                    }
					
					else if (strcmp(zInputStr, "rc") == 0)
					{	/*
						 * Get address.  If the LCD segment table is not stored
						 * in the low address of the EEPROM, the prompt should
						 * be changed to reflect this.
						 */
						SCIWriteString_P(PSTR("  Enter EEPROM address, 110 - 4095: "));
						MenuState = GET_EEPROM_READ_ADDR;
					}

                    else
                    {   // No entry
                        // Back to top menu
                        MenuState = TOP_MENU;
                    }
                    break;

                case GET_CHAR_FOR_EEPROM:
                    if (zInputStr[0] != '\0')
                    {   // Just skip NULL entries
						LCDChar = zInputStr[0];
						/*
						 * Get address.  If the LCD segment table is not stored
						 * in the low address of the EEPROM, the prompt should
						 * be changed to reflect this.
						 */
						SCIWriteString_P(PSTR("  Enter EEPROM address, 110 - 4095: "));
						MenuState = GET_EEPROM_WRITE_ADDR;
					}
					else MenuState = TOP_MENU;
                    break;

				case GET_EEPROM_READ_ADDR:
					/*
					 * Stay here until the entire message has been
					 * received from the serial port.
					 */
					if (zInputStr[0] != '\0')
					{	// Just skip NULL entries
						EEPROMAddr = _atoi(zInputStr, 10);
						LCDChar = EEPROM_read(EEPROMAddr);
						if (EEPROMAddr >= GetLCDTableSize())
						{
							error = GetLCDDRValues(LCDChar, &LCDDRx, &LCDDRx5, &LCDDRx10, &LCDDRx15);
							if (error == NO_ERROR)
							{
								strcpy_P(zOutputStr, PSTR("Char = "));
								zOutputStr[7] = LCDChar;
								zOutputStr[8] = '\n';
								zOutputStr[9] = '\r';
								zOutputStr[10] = '\0';
								SCIWriteString(zOutputStr);
								sprintf(zOutputStr, "Segment Data = %X%X%X%X\n\r", 
										LCDDRx15, LCDDRx10, LCDDRx5, LCDDRx);
								SCIWriteString(zOutputStr);
							}
							else
							{
								ReportError(LCD_INVALID_CHAR);
							}
						}
						else
						{
							ReportError(PARAMETER_OUT_OF_RANGE);
						}
					}
					MenuState = TOP_MENU;
					break;

				case GET_EEPROM_WRITE_ADDR:
					if (zInputStr[0] != '\0')
					{	// Just skip null entries
						EEPROMAddr = _atoi(zInputStr, 10);
						if (EEPROMAddr >= GetLCDTableSize())
						{
							EEPROM_write(EEPROMAddr, LCDChar);
							strcpy(zOutputStr,zInputStr);
						}
						else
						{
							ReportError(PARAMETER_OUT_OF_RANGE);
						}
					}
					MenuState = TOP_MENU;
					break;
                
                default:
                    // Erroneous state. Reset to none
                    MenuState = TOP_MENU;
                    break;
            }   // end switch

            // Reset string pointer. May have been moved during command processing
            ptrInputStr = zInputStr;

            if (MenuState == TOP_MENU)
            {
                // Display prompt
                SCIWriteString_P(PSTR("cmd> "));
            }
        }
        else
        {   // Save new character to input buffer
            if (ptrInputStr < &zInputStr[MAX_IN_STR_SIZE-2])
            {   // Buffer is not full
                *ptrInputStr = cTempChar;
                *(ptrInputStr+1) = '\0';    // Keep null in string

                // echo character
                SCIWriteString(ptrInputStr);

                ++ptrInputStr;
            }   // else, buffer is full. Ignore characters.
        }
    }   // End while. All characters processed

	if (MenuState == DISPLAY_HELP_MENU1)
	{	// Display 1st part of help menu
		SCIWriteString_P(PSTR("  Commands are:\n\r"));
		SCIWriteString_P(PSTR("  ge  - Display error code\n\r"));
		SCIWriteString_P(PSTR("  ce  - Clear current error\n\r"));
		MenuState = DISPLAY_HELP_MENU2;
	}
	
	
	
	else if (MenuState == DISPLAY_HELP_MENU2)
	{	// Display 3rd part of help menu
		SCIWriteString_P(PSTR("  wc - Write character to EEPROM\n\r"));
		SCIWriteString_P(PSTR("  rc - Read character from EEPROM\n\r"));
		MenuState = DISPLAY_HELP_MENU3;
	}
	else if (MenuState == DISPLAY_HELP_MENU3)
	{	// Display 4th part of help menu
		SCIWriteString_P(PSTR("  ?   - Display this help menu\n\r"));
		MenuState = TOP_MENU;
	}
}
