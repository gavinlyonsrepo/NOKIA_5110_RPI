/*
* Project Name: NOKIA_5110_RPI
* File: NOKIA_5110_graphics.h
* Description: header file for the custom graphics functions
* Author: Gavin Lyons.
* URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
*/

#ifndef _NOKIA_5110_graphics_h
#define _NOKIA_5110_graphics_h


// Section: Includes 
#include <bcm2835.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "NOKIA_5110_Print.h"  //custom

// Section defines 
#define LCD_ASCII_OFFSET 0x20 //ASCII character for Space
#define LCD_min(a,b) (((a) < (b)) ? (a) : (b))
#define LCD_swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }

// Section: Enums 
typedef enum 
{
	LCDFont_Default = 1,
	LCDFont_Thick = 2, // No lowercase
	LCDFont_Seven_Seg = 3,
	LCDFont_Wide = 4, //no lowercase
	LCDFont_Tiny = 5,
}LCD_FONT_TYPE_e; // Font type 1-5

// Section: Classes

// Class to handle fonts and graphics of ST7735 display
class NOKIA_5110_graphics : public Print {

 public:

	NOKIA_5110_graphics(); // Constructor
	~NOKIA_5110_graphics(){};
	
	//Pixels + lines 
	// This MUST be defined by the subclass:
	virtual void LCDDrawPixel(uint8_t x, uint8_t y, bool color) = 0;
	void display_drawHLine(uint8_t x, uint8_t y, uint8_t w, bool color);
	void display_drawVLine(uint8_t x, uint8_t y, uint8_t w, bool color);
	void display_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);
	
	// Shapes
	void display_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
	void display_fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color);
	void display_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color);
	void display_fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color);
	void display_drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
	void display_fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
	void display_drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint8_t color);
	void display_fillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint8_t color);
	
	// Text 
	void display_SetFontNum(LCD_FONT_TYPE_e FontNumber);
	void display_putc(char c);
	void display_puts(char *s);
	void display_drawChar(uint16_t x, uint16_t y, uint8_t c, uint8_t color, uint8_t bg, uint8_t size);
	virtual size_t write(uint8_t);
	void display_setTextColor(uint8_t c, uint8_t bg);
	void display_setTextSize(uint8_t s);
	void display_setTextWrap(bool w);
	
	// Screen related
	void display_setCursor(uint16_t x, uint16_t y);
	uint8_t  display_getRotation();
	uint16_t display_getCursorX(void);
	uint16_t display_getCursorY(void);
	uint16_t display_getWidth();
	uint16_t display_getHeight();
	
	// bitmap + custom characters 
	void display_drawBitmapV2(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint8_t color);
	void display_customChar(const uint8_t *c);
	
protected:
	uint8_t _width, _height, _rotation;
	uint8_t _FontNumber = LCDFont_Default;
	uint8_t _CurrentFontWidth = 5; // 5 width of default font
	
	int16_t _cursorX = 0; // x location to start 
	int16_t _cursorY = 0; // y location to start 
	uint8_t _textColor = 0xFF;  //  background color for print
	uint8_t _textBgColor = 0x00;// text color for print
	uint8_t _textSize = 1;  //   magnification of text to print
	bool _textWrap = true;   // If true _textWrap text at right edge of display
	
private:
	void display_drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint8_t color);
	void display_fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t delta, uint8_t color);
	void display_drawLine_2(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool color);
	void display_writeLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);
};

#endif // file header guard

// ********************** EOF *********************
