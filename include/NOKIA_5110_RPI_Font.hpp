// File to store fonts for project
// https://github.com/gavinlyonsrepo/NOKIA_5110_RPI

#ifndef _FONTFILE_NOKIA_H
#define _FONTFILE_NOKIA_H

#include <inttypes.h>

// (1) default (FUll ASCII)
// (2) thick (NO LOWERCASE)
// (3) seven segment
// (4) wide (NO LOWERCASE)
// (5) tiny
// (6) homespun
// (7) big numbers (NUMBERS ONLY )
// (8) med numbers (NUMBERS ONLY )

extern const unsigned char * pFontDefaultptr;
extern const unsigned char * pFontThickptr;
extern const unsigned char * pFontSevenSegptr;
extern const unsigned char * pFontWideptr;
extern const  unsigned char * pFontTinyptr;
extern const unsigned char * pFontHomeSpunptr;
extern const uint8_t (* pFontBigNumptr)[64];
extern const uint8_t (* pFontMedNumptr)[32];

typedef enum
{
	LCDFontType_Default = 1,
	LCDFontType_Thick = 2, // NO LOWERCASE
	LCDFontType_SevenSeg = 3,
	LCDFontType_Wide = 4, // NO LOWERCASE
	LCDFontType_Tiny = 5,
	LCDFontType_Homespun = 6,
	LCDFontType_Bignum = 7, // NUMBERS + : . ,one size
	LCDFontType_Mednum = 8   // NUMBERS + : . ,one size
}LCDFontType_e;

typedef enum
{
	LCDFontWidth_3 = 3,
	LCDFontWidth_5 = 5,
	LCDFontWidth_7 = 7,
	LCDFontWidth_4 = 4,
	LCDFontWidth_8 = 8,
	LCDFontWidth_16 = 16
}LCDFontWidth_e; // width of the font in bytes cols.

typedef enum
{
	LCDFontOffset_Extend = 0x00, //   extends ASCII
	LCDFontOffset_Space = 0x20,  // Starts at Space
	LCDFontOffset_Number = 0x30,  // Starts at number '0'
}LCDFontOffset_e; // font offset in the ASCII table

typedef enum
{
	LCDFontHeight_8 = 8,
	LCDFontHeight_16 = 16,
	LCDFontHeight_32 = 32
}LCDFontHeight_e; // height of the font in bits

#endif // file guard header
