/*!
	@file NOKIA_5110_RPI_Font.hpp
	@brief  Nokia 5110  font data file 10 fonts.
	@author Gavin Lyons.
	@note Fonts are 1-6 are Vertically addressed single dimension array(size scale-bale)
			Fonts 7-10 are Horizontally addressed 2 dimension array flipped 90 degrees(fixed size)
	@details 
		-#  Font_One  default  (FUll ASCII with mods)
		-#  Font_Two  thick (NO LOWERCASE)
		-#  Font_Three seven segment 
		-#  Font_Four wide (NO LOWERCASE)
		-#  Font_Five  tiny
		-#  Font_Six HomeSpun
		-#  Font_Seven big numbers (NUMBERS ONLY + : . ,one size )
		-#  Font_Eight Medium numbers (NUMBERS ONLY + : . ,one size)
		-#  Font_Nine Arial Round
		-#  Font_Ten Arial Bold
*/

#pragma once

#include <stdint.h>

/*! Enum to define current font type selected  */
enum LCDFontType_e : uint8_t
{
	LCDFontType_Default = 1,  /**<  (1) default  (FUll ASCII with mods) */
	LCDFontType_Thick = 2,    /**<  (2) thick (NO LOWERCASE) */
	LCDFontType_SevenSeg = 3, /**<  (3) seven segment  */
	LCDFontType_Wide = 4,     /**<  (4) wide (NO LOWERCASE) */
	LCDFontType_Tiny = 5,     /**<  (5) tiny */
	LCDFontType_Homespun = 6, /**<  (6) HomeSpun */
	LCDFontType_Bignum = 7,   /**< (7) big numbers  NUMBERS only + : . ,fixed size */
	LCDFontType_Mednum = 8,   /**< (8) Med numbers NUMBERS + : . , fixed size */
	LCDFontType_ArialRound = 9,   /**< Arial round */
	LCDFontType_ArialBold = 10    /**< Arial bold */
};

/*!  Width of the font in bits(pixels)  * (N bytes cols) */
enum LCDFontWidth_e 
{
	LCDFontWidth_3 = 3,  /**< 3 tiny font */
	LCDFontWidth_4 = 4,  /**< 4 seven segment font */ 
	LCDFontWidth_5 = 5,   /**< 5 default font */
	LCDFontWidth_7 = 7,  /**< 7 homespun & thick font*/
	LCDFontWidth_8 = 8,  /**< 8 wide font */
	LCDFontWidth_16 = 16 /**< 16 font 7-10 */
};

/*! height of the font in bits or pixels */
enum LCDFontHeight_e : uint8_t
{
	LCDFontHeight_8 = 8,    /**< 8 font  1-6 at size 1*/
	LCDFontHeight_16 = 16,  /**< 16 bit high font 8 & 10  */
	LCDFontHeight_24 = 24, /**< 24 bit font 9 */
	LCDFontHeight_32 = 32  /**<  32 bit font 7 */
}; 

/*! font offset in the ASCII table */
enum LCDFontOffset_e : uint8_t
{
	LCDFontOffset_Extend = 0x00,     /**<  Full ASCII */
	LCDFontOffset_Space = 0x20,      /**< Starts at Space  */
	LCDFontOffset_Minus = 0x2D,      /**< Starts at '-' */
	LCDFontOffset_Number = 0x30      /**<  Starts at number '0' */
};


/*! Number of ASCII characters in Font */
enum LCDFontLength_e : uint8_t
{
	LCDFontLenNumeric = 14,         /* Numeric + : - . forward slash 0x2D - 0x3A */
	LCDFontLenAlphaNumNoLCase = 59, /* " " to "Z" 0x20-0x5A*/
	LCDFontLenAlphaNum = 95,        /* " "  to "~" 0x20-0x7E */
	LCDFontLenAll = 255            /**Full Range  0-0xFF */
};

// Font data is in the cpp file accessed thru extern pointers.

extern const unsigned char * pFontDefaultptr; /**< Pointer to default font data  */
extern const unsigned char * pFontThickptr; /**< Pointer to thick font data  */
extern const unsigned char * pFontSevenSegptr; /**< Pointer to seven segment font data  */
extern const unsigned char * pFontWideptr; /**< Pointer to wide font data  */
extern const unsigned char * pFontTinyptr; /**< Pointer to tiny font data  */
extern const unsigned char * pFontHomeSpunptr; /**< Pointer to homespun font data  */
extern const uint8_t (* pFontBigNumptr)[64]; /**< Pointer to big numbers font data  */
extern const uint8_t (* pFontMedNumptr)[32]; /**< Pointer to medium numbers font data  */
extern const uint8_t (* pFontArial16x24ptr)[48]; /**< Pointer to Arial Round font data */
extern const uint8_t (* pFontArial16x16ptr)[32]; /**< Pointer to Arial bold font data */
