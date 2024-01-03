 /*!
	@file  NOKIA_5110_graphics.hpp
	@brief header file for the graphics functions
	@details Project Name: NOKIA_5110_RPI
		URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
	@author  Gavin Lyons
*/

#pragma once

// Section: Includes
#include <bcm2835.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <iostream>

#include "NOKIA_5110_Print.hpp"
#include "NOKIA_5110_RPI_Font.hpp"

// Section defines TODO
#define LCD_min(a,b) (((a) < (b)) ? (a) : (b))
#define LCD_swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }

// Section: Classes

/*! @brief Graphics class to hold graphic related functions */
class NOKIA_5110_graphics : public Print
{

public:

	NOKIA_5110_graphics(); // Constructor
	~NOKIA_5110_graphics(){};

	//Pixels + lines
	// This(LCDDrawPixel) MUST be defined by the subclass:
	virtual void LCDDrawPixel(uint8_t x, uint8_t y, bool color) = 0;
	void drawHLine(uint8_t x, uint8_t y, uint8_t w, bool color);
	void drawVLine(uint8_t x, uint8_t y, uint8_t w, bool color);
	void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);

	// Shapes
	void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
	void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color);
	void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color);
	void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color);
	void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
	void fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
	void drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint8_t color);
	void fillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint8_t color);

	// Text
	void SetFontNum(LCDFontType_e FontNumber);
	bool drawChar(uint16_t x, uint16_t y, uint8_t c, uint8_t color, uint8_t bg, uint8_t size);
	bool drawText(uint8_t x, uint8_t y, char *pText, uint8_t color, uint8_t bg, uint8_t size);
	bool drawCharBigFont(uint8_t x, uint8_t y, uint8_t c, uint8_t color , uint8_t bg);
	bool drawTextBigFont(uint8_t x, uint8_t y, char *pText, uint8_t color, uint8_t bg);
	virtual size_t write(uint8_t);

	void setTextColor(uint8_t c, uint8_t bg);
	void setTextSize(uint8_t s);
	void setTextWrap(bool w);

	// Screen related
	void setCursor(uint16_t x, uint16_t y);
	uint8_t  getRotation();
	uint16_t getCursorX(void);
	uint16_t getCursorY(void);
	uint16_t getWidth();
	uint16_t getHeight();

	// bitmap + custom characters
	bool drawBitmap(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint8_t color, uint16_t size);
	bool customChar(const uint8_t *c, uint16_t s);

protected:
	uint8_t _width = 0; /**< Display w as modified by current _rotation*/
	uint8_t _height = 0; /**< Display h as modified by current _rotation*/
	uint8_t  _rotation = 0;  /**< Current rotation 0-3 */
	int16_t _cursorX = 0; /**< Current X co-ord cursor position */
	int16_t _cursorY = 0; /**< Current Y co-ord cursor position */

	uint8_t _FontNumber = 1; /**< Holds current font number */
	uint8_t _CurrentFontWidth = 5;   /**<Holds current font width in bits */
	uint8_t _CurrentFontoffset = 0x00;  /**<Holds current font ASCII table offset */
	uint8_t _CurrentFontheight = 8;  /**<Holds current font height in bits */
	uint8_t _CurrentFontLength = 255;/**<Holds current font number of characters */

	uint8_t _textColor = 0xFF;   /**< Text foreground color */
	uint8_t _textBgColor = 0x00; /**< Text background color */
	uint8_t _textSize = 1;  /**< Size of text ,fonts 1-6 */
	bool _textWrap = true;    /**< If set, '_textwrap' text at right edge of display*/

private:
	void drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint8_t color);
	void fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t delta, uint8_t color);
	void drawLine_2(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool color);
	void writeLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);
};


// ********************** EOF *********************
