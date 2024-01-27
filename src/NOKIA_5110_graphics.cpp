/*!
* @file NOKIA_5110_graphics.cpp
* @brief   source file for the custom graphics functions
* @author Gavin Lyons.
* @details <https://github.com/gavinlyonsrepo/NOKIA_5110_RPI>
*/

#include "NOKIA_5110_graphics.hpp"
#include "NOKIA_5110_RPI.hpp"

/*!
	@brief constructor for NOKIA_5110_graphics class
*/
NOKIA_5110_graphics::NOKIA_5110_graphics()
{
}

/*!
	@brief called from the print class to wrap drawText functions
*/
size_t NOKIA_5110_graphics::write(uint8_t character)
{
	int DrawCharReturnCode;
	if (_FontNumber < LCDFont_Bignum)
	{
		switch (character)
		{
		case '\n':
			_cursorY += _textSize*_CurrentFontheight;
			_cursorX  = 0;
		break;
		case'\r':/* skip */ break;
		default:
				DrawCharReturnCode = drawChar(_cursorX, _cursorY, character, _textColor, _textBgColor, _textSize) ;
				if(DrawCharReturnCode  != LCD_Success)
				{
					std::cout<< "Error write_print method 1: Method drawChar failed" << DrawCharReturnCode << std::endl;
					return DrawCharReturnCode;
				}
			_cursorX += _textSize*(_CurrentFontWidth+1);
				if (_textWrap && (_cursorX > (_width - _textSize*(_CurrentFontWidth+1))))
				{
					_cursorY += _textSize*_CurrentFontheight;
					_cursorX = 0;
				}
		break;
		}
	}else // for font numbers 7-10
	{
		switch (character)
		{
			case '\n':
				_cursorY += _CurrentFontheight;
				_cursorX  = 0;
			break;
			case '\r': /* skip */  break;
			default:
				DrawCharReturnCode = drawChar(_cursorX, _cursorY, character, _textColor, _textBgColor) ;
				if(DrawCharReturnCode  != LCD_Success)
				{
					std::cout<< "Error write_print method 2 : Method drawChar failed" << DrawCharReturnCode << std::endl;
					return DrawCharReturnCode;
				}
				_cursorX += (_CurrentFontWidth);
				if (_textWrap && (_cursorX  > (_width - (_CurrentFontWidth+1))))
				{
					_cursorY += _CurrentFontheight;
					_cursorX = 0;
				}
			break;
		} // end of switch
	} // end of else
  return 1;
}

/*!
	@brief drawLine_2
	@param x0 x-co-ord start
	@param y0 y-co-ord start
	@param x1 x-co-ord end
	@param y1 y-co-ord end
	@param color
*/
void NOKIA_5110_graphics::drawLine_2(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool color) {

	bool steep = abs((int16_t)(y1 - y0)) > abs((int16_t)(x1 - x0));
	int8_t ystep;
	uint8_t dx, dy;
	int16_t err;

	if (steep) {
	LCD_swap_uint8_t(x0, y0);
	LCD_swap_uint8_t(x1, y1);
	}
	if (x0 > x1) {
	LCD_swap_uint8_t(x0, x1);
	LCD_swap_uint8_t(y0, y1);
	}
	dx = x1 - x0;
	dy = abs(y1 - y0);

	err = dx / 2;
	if (y0 < y1)
	ystep = 1;
	else
	ystep = -1;

	for (; x0 <= x1; x0++) {
	if (steep)
		LCDDrawPixel(y0, x0, color);
	else
		LCDDrawPixel(x0, y0, color);
	err -= dy;
	if (err < 0) {
		y0  += ystep;
		err += dx;
	}
	}
}

/*!
	@brief drawHLine
	@param x x-co-ord
	@param y y-co-ord
	@param w Length of line
	@param color
*/
void NOKIA_5110_graphics::drawHLine(uint8_t x, uint8_t y, uint8_t w, bool color) {
	 drawLine_2(x, y, x + w - 1, y, color);
}

/*!
	@brief drawVLine
	@param x x-co-ord
	@param y y-co-ord
	@param h Length of line
	@param color
*/
void NOKIA_5110_graphics::drawVLine(uint8_t x, uint8_t y, uint8_t h, bool color) {
	drawLine_2(x, y, x, y + h - 1, color);
}

/*!
	@brief fillRect , Draw a rectangle and fill it.
	@param x x-co-ord
	@param y y-co-ord
	@param w width of rectangle
	@param h height of rectangle
	@param color
*/
void NOKIA_5110_graphics::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color) {
	uint8_t i;
	for (i = x; i < x + w; i++)
	drawVLine(i, y, h, color);
}

/*!
	Name:         writeLine
	@brief   Write a line.  Bresenham's algorithm
	@param    x0  Start point x coordinate
	@param    y0  Start point y coordinate
	@param    x1  End point x coordinate
	@param    y1  End point y coordinate
	@param    color
*/
void NOKIA_5110_graphics::writeLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color) {
	bool steep = abs((int16_t)(y1 - y0)) > abs((int16_t)(x1 - x0));
	int16_t dx, dy, err, ystep;
	if (steep) {
		LCD_swap_int16_t(x0, y0);
		LCD_swap_int16_t(x1, y1);
	}

	if (x0 > x1) {
		LCD_swap_int16_t(x0, x1);
		LCD_swap_int16_t(y0, y1);
	}

	dx = x1 - x0;
	dy = abs((int16_t)(y1 - y0));

	err = dx / 2;


	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0<=x1; x0++) {
		if (steep) {
			LCDDrawPixel(y0, x0, color);
		} else {
			LCDDrawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}


/*!
	@brief   Draw a line
	@param    x0  Start point x coordinate
	@param    y0  Start point y coordinate
	@param    x1  End point x coordinate
	@param    y1  End point y coordinate
	@param    color
*/
void NOKIA_5110_graphics::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color) {
	if(x0 == x1){
		if(y0 > y1) LCD_swap_int16_t(y0, y1);
		drawVLine(x0, y0, y1 - y0 + 1, color);
	} else if(y0 == y1){
		if(x0 > x1) LCD_swap_int16_t(x0, x1);
		drawVLine(x0, y0, x1 - x0 + 1, color);
	} else {
			writeLine(x0, y0, x1, y1, color);
	}
}


/*!
	@brief   Draw a circle outline
	@param    x0   Center-point x coordinate
	@param    y0   Center-point y coordinate
	@param    r   Radius of circle
	@param    color
*/
void NOKIA_5110_graphics::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	LCDDrawPixel(x0  , y0+r, color);
	LCDDrawPixel(x0  , y0-r, color);
	LCDDrawPixel(x0+r, y0  , color);
	LCDDrawPixel(x0-r, y0  , color);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		LCDDrawPixel(x0 + x, y0 + y, color);
		LCDDrawPixel(x0 - x, y0 + y, color);
		LCDDrawPixel(x0 + x, y0 - y, color);
		LCDDrawPixel(x0 - x, y0 - y, color);
		LCDDrawPixel(x0 + y, y0 + x, color);
		LCDDrawPixel(x0 - y, y0 + x, color);
		LCDDrawPixel(x0 + y, y0 - x, color);
		LCDDrawPixel(x0 - y, y0 - x, color);
	}
}

/*!
	@brief   Quarter-circle drawer, used to do circles and roundrects
	@param    x0   Center-point x coordinate
	@param    y0   Center-point y coordinate
	@param    r   Radius of circle
	@param    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing
	@param    color
*/
void NOKIA_5110_graphics::drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint8_t color) {
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername & 0x4) {
			LCDDrawPixel(x0 + x, y0 + y, color);
			LCDDrawPixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			LCDDrawPixel(x0 + x, y0 - y, color);
			LCDDrawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			LCDDrawPixel(x0 - y, y0 + x, color);
			LCDDrawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			LCDDrawPixel(x0 - y, y0 - x, color);
			LCDDrawPixel(x0 - x, y0 - y, color);
		}
	}
}


/*!
	@brief   Draw a circle with filled color
	@param    x0   Center-point x coordinate
	@param    y0   Center-point y coordinate
	@param    r   Radius of circle
	@param    color
*/
void NOKIA_5110_graphics::fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color) {
	drawVLine(x0, y0-r, 2*r+1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}


/*!
	@brief Quarter-circle drawer with fill, used for circles and roundrects
	@param  x0       Center-point x coordinate
	@param  y0       Center-point y coordinate
	@param  r        Radius of circle
	@param  corners  Mask bits indicating which quarters we're doing
	@param  delta    Offset from center-point, used for round-rects
	@param  color
*/
void NOKIA_5110_graphics::fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t corners, uint16_t delta, uint8_t color) {
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;
	int16_t px    = x;
	int16_t py    = y;

	delta++;

	while(x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if(x < (y + 1)) {
			if(corners & 1) drawVLine(x0+x, y0-y, 2*y+delta, color);
			if(corners & 2) drawVLine(x0-x, y0-y, 2*y+delta, color);
		}
		if(y != py) {
			if(corners & 1) drawVLine(x0+py, y0-px, 2*px+delta, color);
			if(corners & 2) drawVLine(x0-py, y0-px, 2*px+delta, color);
			py = y;
		}
		px = x;
	}
}


/*!
	@brief  Draw a rectangle with no fill color
	@param    x   Top left corner x coordinate
	@param    y   Top left corner y coordinate
	@param    w   Width in pixels
	@param    h   Height in pixels
	@param    color
*/
void NOKIA_5110_graphics::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color) {
	drawHLine(x, y, w, color);
	drawHLine(x, y+h-1, w, color);
	drawVLine(x, y, h, color);
	drawVLine(x+w-1, y, h, color);
}


/*!
	@brief  Draw a rounded rectangle with no fill color
	@param    x   Top left corner x coordinate
	@param    y   Top left corner y coordinate
	@param    w   Width in pixels
	@param    h   Height in pixels
	@param    r   Radius of corner rounding
	@param    color
*/
void NOKIA_5110_graphics::drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint8_t color) {
	int16_t max_radius = ((w < h) ? w : h) / 2;
	if(r > max_radius) r = max_radius;
	drawHLine(x+r  , y    , w-2*r, color); // Top
	drawHLine(x+r  , y+h-1, w-2*r, color); // Bottom
	drawVLine(x    , y+r  , h-2*r, color); // Left
	drawVLine(x+w-1, y+r  , h-2*r, color); // Right

	drawCircleHelper(x+r    , y+r    , r, 1, color);
	drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
	drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
	drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}


/*!
	@brief  Draw a rounded rectangle with fill color
	@param    x   Top left corner x coordinate
	@param    y   Top left corner y coordinate
	@param    w   Width in pixels
	@param    h   Height in pixels
	@param    r   Radius of corner rounding
	@param    color
*/
void NOKIA_5110_graphics::fillRoundRect(uint16_t x, uint16_t y, uint16_t w,
	uint16_t h, uint16_t r, uint8_t color) {
	int16_t max_radius = ((w < h) ? w : h) / 2;
	if(r > max_radius) r = max_radius;

	fillRect(x+r, y, w-2*r, h, color);

	fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
	fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}


/*!
	@brief  Draw a triangle with no fill color
	@param    x0  Vertex #0 x coordinate
	@param    y0  Vertex #0 y coordinate
	@param    x1  Vertex #1 x coordinate
	@param    y1  Vertex #1 y coordinate
	@param    x2  Vertex #2 x coordinate
	@param    y2  Vertex #2 y coordinate
	@param    color color to draw
*/
void NOKIA_5110_graphics::drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
	uint16_t x2, uint16_t y2, uint8_t color) {
	drawLine_2(x0, y0, x1, y1, color);
	drawLine_2(x1, y1, x2, y2, color);
	drawLine_2(x2, y2, x0, y0, color);
}

/*!
	@brief    Draw a triangle with color-fill
	@param    x0  Vertex #0 x coordinate
	@param    y0  Vertex #0 y coordinate
	@param    x1  Vertex #1 x coordinate
	@param    y1  Vertex #1 y coordinate
	@param    x2  Vertex #2 x coordinate
	@param    y2  Vertex #2 y coordinate
	@param    color color to fill
*/
void NOKIA_5110_graphics::fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
	uint16_t x2, uint16_t y2, uint8_t color) {
	int16_t a, b, y, last, dx01, dy01, dx02, dy02, dx12, dy12;
	int32_t
	sa   = 0,
	sb   = 0;

	if (y0 > y1) {
		LCD_swap_int16_t(y0, y1); LCD_swap_int16_t(x0, x1);
	}
	if (y1 > y2) {
		LCD_swap_int16_t(y2, y1); LCD_swap_int16_t(x2, x1);
	}
	if (y0 > y1) {
		LCD_swap_int16_t(y0, y1); LCD_swap_int16_t(x0, x1);
	}

	if(y0 == y2) {
		a = b = x0;
		if(x1 < a)      a = x1;
		else if(x1 > b) b = x1;
		if(x2 < a)      a = x2;
		else if(x2 > b) b = x2;
		drawHLine(a, y0, b-a+1, color);
		return;
	}

	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;

	if(y1 == y2) last = y1;
	else         last = y1-1;

	for(y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if(a > b) LCD_swap_int16_t(a,b);
		drawHLine(a, y, b-a+1, color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if(a > b) LCD_swap_int16_t(a,b);
		drawHLine(a, y, b-a+1, color);
	}
}


/*!
	@brief print custom char  (dimension: 5x8 x by y pixels)
	@param character the character to draw
	@param sizeofArray The size of the character array passed must be 5
	@return LCD_Return_Codes_e
*/
LCD_Return_Codes_e NOKIA_5110_graphics::customChar(const uint8_t* character, uint16_t sizeofArray) {

	// Check for null pointer
	if(character == nullptr)
	{
		std::cout<< "Error customChar 1 :Custom char array is not valid pointer" << std::endl;
		return LCD_BitmapNullptr;
	}

	const uint8_t CustomCharWidth = 5;
	const uint8_t CustomCharHeight = 8;
	uint8_t i = 0;
	uint8_t j = 0;

	// check Array is correct size '5'
	if(sizeofArray != CustomCharWidth )
	{
		std::cout << "Error customChar 2 :: CustomChar array must always be 5 bytes long :" << sizeofArray<<std::endl;
		return LCD_CustomCharLen;
	}

	for(i = 0; i < CustomCharWidth ; i++ )
	{
		uint8_t line = character[i];
		for(j = 0; j < CustomCharHeight ; j++, line >>= 1)
		{
			if(line & 1)
			{
			if(_textSize == 1)
				LCDDrawPixel(_cursorX + i, _cursorY + j, _textColor);
			else
				fillRect(_cursorX + i * _textSize, _cursorY + j * _textSize, _textSize, _textSize, _textColor);
			}
			else
			if(_textBgColor != _textColor)
			{
				if(_textSize == 1)
					LCDDrawPixel(_cursorX + i, _cursorY + j, _textBgColor);
				else
					fillRect(_cursorX + i * _textSize, _cursorY + j * _textSize, _textSize, _textSize, _textBgColor);
			}
		}
	}

	if(_textBgColor != _textColor)
	{
		if(_textSize == 1)
			drawVLine(_cursorX + CustomCharWidth , _cursorY, CustomCharHeight , _textBgColor);
		else
			fillRect(_cursorX + CustomCharWidth  * _textSize, _cursorY, _textSize, CustomCharHeight  * _textSize, _textBgColor);
	}

	_cursorX += _textSize * (CustomCharWidth+1) ;

	if( _cursorX > ((uint16_t)_width + _textSize * (CustomCharWidth +1)) )
		_cursorX = _width;

	if (_textWrap && (_cursorX + (_textSize * CustomCharWidth )) > _width)
	{
		_cursorX = 0;
		_cursorY += _textSize * CustomCharHeight ;
		if( _cursorY > ((uint16_t)_height + _textSize * CustomCharHeight ) )
			_cursorY = _height;
	}
	return LCD_Success;
}


/*!
	@brief  Draw a single character font 1-6
	@param    x   Bottom left corner x coordinate
	@param    y   Bottom left corner y coordinate
	@param    character   The 8-bit font-indexed character (likely ascii)
	@param   color foreground color
	@param   bg   background color
	@param size Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
	@return LCD_Return_Codes_e 
*/
LCD_Return_Codes_e NOKIA_5110_graphics::drawChar(uint16_t x, uint16_t y, uint8_t character, uint8_t color, uint8_t bg,uint8_t size)
{

	// 1. Check for wrong font
	if (_FontNumber >= LCDFont_Bignum)
	{
		std::cout<< "Error drawChar 1: Wrong font selected, must be font 1-6 "<< std::endl;
		return LCD_WrongFont;
	}
	// 2. Check for screen out of  bounds
	if((x >= _width)            || // Clip right
	(y >= _height)           || // Clip bottom
	((x + (_CurrentFontWidth+1) * size - 1) < 0) || // Clip left
	((y + _CurrentFontheight  * size - 1) < 0))   // Clip top
	{
		std::cout<< "Error drawChar 2: Co-ordinates out of bounds " << std::endl;
		return LCD_CharScreenBounds;
	}
	// 3. Check for character out of font range bounds
	if ( character < _CurrentFontoffset || character >= (_CurrentFontLength+ _CurrentFontoffset))
	{
		std::cout<< "Error drawChar 3: Character out of Font bounds " << character << " " <<+_CurrentFontoffset <<  +(_CurrentFontLength + _CurrentFontoffset) << std::endl;
		return LCD_CharFontASCIIRange;
	}

	for (int8_t i=0; i<(_CurrentFontWidth+1); i++ )
	{
	uint8_t line;
	if (i == _CurrentFontWidth)
	{
		line = 0x0;
	}
	else
	{
		switch (_FontNumber)
		{
			case LCDFont_Default:
				line = pFontDefaultptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
			break;
			case LCDFont_Thick:
				line = pFontThickptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
			break;
			case LCDFont_SevenSeg:
				line = pFontSevenSegptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
			break;
			case LCDFont_Wide:
				line = pFontWideptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
			break;
			case LCDFont_Tiny:
				line =pFontTinyptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
			break;
			case LCDFont_Homespun:
				line = pFontHomeSpunptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
			break;
			default:
				std::cout<< "Error drawChar 4: Wrong font number , must be 1-6" << std::endl;
				return LCD_WrongFont;
			break;
		}//switch font linenumber
	}

		for (int8_t j = 0; j<_CurrentFontheight; j++)
		{
			if (line & 0x1) {
			if (size == 1) // default size
				LCDDrawPixel(x+i, y+j, color);
			else {  // big size
				fillRect(x+(i*size), y+(j*size), size, size, color);
			}
			} else if (bg != color) {
			if (size == 1) // default size
				LCDDrawPixel(x+i, y+j, bg);
			else {  // big size
				fillRect(x+i*size, y+j*size, size, size, bg);
			}
			}
			line >>= 1;
		}
	} // for loop
	return LCD_Success;
}

/*!
	@brief setCursor , sets cursor position
	@param x x_co-ord
	@param y y_Co-ord
*/
void NOKIA_5110_graphics::setCursor(uint16_t x, uint16_t y) {
	_cursorX = x;
	_cursorY = y;
}

/*!
	@brief getCursorX , gets cursor X position
	@return X cursor position
*/
uint16_t NOKIA_5110_graphics::getCursorX(void) {
	return _cursorX;
}

/*!
	@brief getCursorY , gets cursor X position
	@return y cursor position
*/
uint16_t NOKIA_5110_graphics::getCursorY(void) {
	return _cursorY;
}

/*!
	@brief  Set text 'magnification' size. Each increase in s makes 1 pixel that much bigger.
	@param s Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
*/
void NOKIA_5110_graphics::setTextSize(uint8_t s) {
	_textSize = (s > 0) ? s : 1;
}

/*!
	@brief    Set the text color fornt and ba=background
	@param c forecolor
	@param b background
*/
void NOKIA_5110_graphics::setTextColor(uint8_t c, uint8_t b) {
	_textColor   = c;
	_textBgColor = b;
}

/*!
	@brief Whether text that is too long should '_textWrap' around to the next line.
	@param w  Set true for _textWrapping, false for clipping
*/
void NOKIA_5110_graphics::setTextWrap(bool w) {_textWrap = w;}

/*!
	@brief     Get current width of display, can vary with rotation
	@returns   _width
*/
uint16_t NOKIA_5110_graphics::getWidth(void) {return _width;}

/*!
	@brief     Get current height of display, can vary with rotation
	@returns   _height
*/
uint16_t NOKIA_5110_graphics::getHeight(void) {return _height;}

/*!
	 @brief    Draw a 1-bit image at the specified (x,y) position,
				using the specified foreground color (unset bits are transparent).
	@param    x   Top left corner x coordinate
	@param    y   Top left corner y coordinate
	@param    bitmap  byte array with monochrome bitmap
	@param    w   Width of bitmap in pixels
	@param    h   Height of bitmap in pixels
	@param    color color of bitmap nlack or white
	@param     sizeOfBitmap Size of the bitmap in bytes
	@return LCD_Return_Codes_e
	@note data must be vertically addressed
*/
LCD_Return_Codes_e NOKIA_5110_graphics::drawBitmap(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h,
	uint8_t color, uint16_t sizeOfBitmap) {

	// User error checks
	// 1. Completely out of bounds?
	if (x > _width || y > _height)
	{
		std::cout<< "Error drawBitmap 1: Bitmap co-ord out of bounds, check x and y" << std::endl;
		return LCD_BitmapScreenBounds ;
	}
	// 2. bitmap weight and height
	if (w > _width || h > _height)
	{
		std::cout<< "Error drawBitmap 2: Bitmap is larger than screen, check w and h" << std::endl;
		return LCD_BitmapLargerThanScreen;
	}
	// 3. bitmap is null
	if(bitmap == nullptr)
	{
		std::cout<< "Error drawBitmap 3: Bitmap is is not valid pointer" << std::endl;
		return LCD_BitmapNullptr;
	}

	// 4.check bitmap size
	if(sizeOfBitmap != (w * (h/8)))
	{
		std::cout<< "Error drawBitmap 4: Bitmap size is incorrect: " <<  sizeOfBitmap << ". Check w & h (w*(h/8):" << w << " " << h << std::endl;
		return LCD_BitmapSize;
	}

	// Vertical byte bitmaps
	uint8_t vline;
	int16_t i, j, r = 0, yin = y;

	for (i=0; i<(w+1); i++ )
	{
		if (r == (h+7)/8 * w) break;
		vline = bitmap [r] ;
		r++;
		if (i == w)
		{
			y = y+8;
			i = 0;
		}

		for (j=0; j<8; j++ )
		{
			if (y+j-yin == h) break;
			if (vline & 0x1)
			{
				LCDDrawPixel(x+i, y+j, color);
			}
			vline >>= 1;
		}
	}

	return LCD_Success;
}

/*!
	@brief  Set the font number
	@param  FontNumber LCDFont_e  enum , fontnumber 1-12
*/
void NOKIA_5110_graphics::SetFontNum(LCDFontType_e FontNumber) {

	_FontNumber = FontNumber;

	switch (_FontNumber) {
	case LCDFont_Default:  // Norm default 5 by 8
		_CurrentFontWidth = LCDFontWidth_5;
		_CurrentFontoffset =  LCDFontOffset_Extend;
		_CurrentFontheight = LCDFontHeight_8;
		_CurrentFontLength = LCDFontLenAll;
	break;
	case LCDFont_Thick: // Thick 7 by 8 (NO LOWERCASE LETTERS)
		_CurrentFontWidth = LCDFontWidth_7;
		_CurrentFontoffset = LCDFontOffset_Space;
		_CurrentFontheight = LCDFontHeight_8;
		_CurrentFontLength = LCDFontLenAlphaNumNoLCase;
	break;
	case LCDFont_SevenSeg:  // Seven segment 4 by 8
		_CurrentFontWidth = LCDFontWidth_4;
		_CurrentFontoffset = LCDFontOffset_Space;
		_CurrentFontheight = LCDFontHeight_8;
		_CurrentFontLength = LCDFontLenAlphaNum;
	break;
	case LCDFont_Wide : // Wide  8 by 8 (NO LOWERCASE LETTERS)
		_CurrentFontWidth = LCDFontWidth_8;
		_CurrentFontoffset = LCDFontOffset_Space;
		_CurrentFontheight = LCDFontHeight_8;
		_CurrentFontLength = LCDFontLenAlphaNumNoLCase;
	break;
	case LCDFont_Tiny:  // tiny 3 by 8
		_CurrentFontWidth = LCDFontWidth_3;
		_CurrentFontoffset =  LCDFontOffset_Space;
		_CurrentFontheight = LCDFontHeight_8;
		_CurrentFontLength = LCDFontLenAlphaNum;
	break;
	case LCDFont_Homespun: // homespun 7 by 8
		_CurrentFontWidth = LCDFontWidth_7;
		_CurrentFontoffset = LCDFontOffset_Space;
		_CurrentFontheight = LCDFontHeight_8;
		_CurrentFontLength = LCDFontLenAlphaNum;
	break;
	case LCDFont_Bignum : // big nums 16 by 32 (NUMBERS)
		_CurrentFontWidth = LCDFontWidth_16;
		_CurrentFontoffset = LCDFontOffset_Minus;
		_CurrentFontheight = LCDFontHeight_32;
		_CurrentFontLength = LCDFontLenNumeric;
	break;
	case LCDFont_Mednum: // med nums 16 by 16 (NUMBERS)
		_CurrentFontWidth = LCDFontWidth_16;
		_CurrentFontoffset =  LCDFontOffset_Minus;
		_CurrentFontheight = LCDFontHeight_16;
		_CurrentFontLength = LCDFontLenNumeric;
	break;
	case LCDFont_ArialRound: // Arial round 16 by 24
		_CurrentFontWidth = LCDFontWidth_16;
		_CurrentFontoffset = LCDFontOffset_Space;
		_CurrentFontheight = LCDFontHeight_24;
		_CurrentFontLength = LCDFontLenAlphaNum;
	break;
	case LCDFont_ArialBold: // Arial bold  16 by 16
		_CurrentFontWidth = LCDFontWidth_16;
		_CurrentFontoffset = LCDFontOffset_Space;
		_CurrentFontheight = LCDFontHeight_16;
		_CurrentFontLength = LCDFontLenAlphaNum;
	break;
	case LCDFont_Mia: // mia  8 by 16
		_CurrentFontWidth = LCDFontWidth_8;
		_CurrentFontoffset = LCDFontOffset_Space;
		_CurrentFontheight = LCDFontHeight_16;
		_CurrentFontLength = LCDFontLenAlphaNum;
	break;
	case LCDFont_Dedica: // dedica  6 by 12
		_CurrentFontWidth = LCDFontWidth_6;
		_CurrentFontoffset = LCDFontOffset_Space;
		_CurrentFontheight = LCDFontHeight_12;
		_CurrentFontLength = LCDFontLenAlphaNum;
	break;
	default: // if wrong font num passed in,  set to default
		_CurrentFontWidth = LCDFontWidth_5;
		_CurrentFontoffset =  LCDFontOffset_Extend;
		_CurrentFontheight = LCDFontHeight_8;
		_FontNumber = LCDFont_Default;
		_CurrentFontLength = LCDFontLenAll;
	break;
	}
}

/*!
	@brief Writes text string (*pText) on the Display
	@param x x-co-ord start
	@param y y-co-ord start
	@param pText pointer to character array
	@param color foreground
	@param bg background color
	@param size Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
	@return LCD_Return_Codes_e
	@note for font 1- 6 only
*/
LCD_Return_Codes_e  NOKIA_5110_graphics::drawText(uint8_t x, uint8_t y, char* pText, uint8_t color, uint8_t bg, uint8_t size) {

	// check Correct font number
	if (_FontNumber >= LCDFont_Bignum)
	{
		std::cout<< "Error drawText 1: Wrong font number , must be 1-6" << std::endl;
		return LCD_WrongFont;
	}
	// Check for null pointer
	if(pText == nullptr)
	{
		std::cout<< "Error drawText 2: String array is not valid pointer" << std::endl;
		return LCD_CharArrayNullptr;
	}
	LCD_Return_Codes_e DrawCharReturnCode;
	uint8_t lcursorX = x;
	uint8_t lcursorY = y;

	while (*pText != '\0')
	{
		if (_textWrap  && ((lcursorX + size * _CurrentFontWidth) > _width))
		{
			lcursorX = 0;
			lcursorY = lcursorY + size * 7 + 3;
			if (lcursorY > _height) lcursorY = _height;
		}
		DrawCharReturnCode = drawChar(lcursorX, lcursorY, *pText, color, bg, size);
		
		if (DrawCharReturnCode != LCD_Success)
		{
			std::cout<< "Error drawText 3: Method drawChar failed" << std::endl;
			return DrawCharReturnCode;
		}
		lcursorX = lcursorX + size * (_CurrentFontWidth + 1);
		if (lcursorX > _width) lcursorX = _width;
		pText++;
	}
	return LCD_Success;
}

/*!
	@brief writes a character on the LCD
	@param x x-co-ord start
	@param y y-co-ord start
	@param character The ASCII character
	@param color foreground color
	@param bg background color
	@return LCD_Return_Codes_e
	@note for font 7-12 only
*/
LCD_Return_Codes_e NOKIA_5110_graphics::drawChar(uint8_t x, uint8_t y, uint8_t character, uint8_t color , uint8_t bg)
{
	uint8_t FontSizeMod = 0;
	// Check user input
	// 1. Check for wrong font
	switch (_FontNumber)
	{
		case LCDFont_Bignum:
		case LCDFont_Mednum:
		case LCDFont_ArialRound:
		case LCDFont_ArialBold:
			FontSizeMod  = 2;
		break;
		case LCDFont_Mia:
		case LCDFont_Dedica:
			FontSizeMod  = 1;
		break;
		default:
			return LCD_WrongFont;
		break;
	}
	// 2. Check for character out of font bounds
	if ( character < _CurrentFontoffset || character >= (_CurrentFontLength+ _CurrentFontoffset))
	{
		std::cout<< "Error drawChar 2: Character out of Font bounds " << character << " " <<+_CurrentFontoffset  <<  +(_CurrentFontLength + _CurrentFontoffset) << std::endl;
		return LCD_CharFontASCIIRange;
	}
	// 3. Check for screen out of  bounds
	if((x >= _width)            || // Clip right
	(y >= _height)           || // Clip bottom
	((x + _CurrentFontWidth+1) < 0) || // Clip left
	((y + _CurrentFontheight) < 0))   // Clip top
	{
		std::cout<< "Error drawChar 3: Co-ordinates out of bounds " << std::endl;
		return LCD_CharScreenBounds;
	}

	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t ctemp = 0;
	uint8_t y0 = y;

	for (i = 0; i < (_CurrentFontheight*FontSizeMod); i++)
	{
		switch (_FontNumber)
		{
			case LCDFont_Bignum: ctemp = pFontBigNumptr[character - _CurrentFontoffset][i]; break;
			case LCDFont_Mednum: ctemp = pFontMedNumptr[character - _CurrentFontoffset][i]; break;
			case LCDFont_ArialRound: ctemp = pFontArial16x24ptr[character - _CurrentFontoffset][i]; break;
			case LCDFont_ArialBold: ctemp = pFontArial16x16ptr[character - _CurrentFontoffset][i]; break;
			case LCDFont_Mia: ctemp = pFontMia8x16ptr[character - _CurrentFontoffset][i]; break;
			case LCDFont_Dedica: ctemp = pFontDedica8x12ptr[character - _CurrentFontoffset][i]; break;
			default :
				std::cout<< "Error drawChar 4: Wrong font selected, must be font 7-12"<< std::endl;
				return LCD_WrongFont;
			break;
		}

		for (j = 0; j < 8; j++)
		{
			if (ctemp & 0x80)
			{
				LCDDrawPixel(x, y, color);
			} else {
				LCDDrawPixel(x, y, bg);
			}
			ctemp <<= 1;
			y++;
			if ((y - y0) == _CurrentFontheight)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
	return LCD_Success;
}

/*!
	@brief Writes text string (*pText) on the Display
	@param  x   Bottom left corner x coordinate
	@param  y   Bottom left corner y coordinate
	@param pText pointer to character array
	@param color foreground color
	@param bg background color
	@return LCD_Return_Codes_e
	@note for font 7 -12  only
*/
LCD_Return_Codes_e NOKIA_5110_graphics::drawText(uint8_t x, uint8_t y, char* pText, uint8_t color, uint8_t bg)
{
	// Check correct font number
	if (_FontNumber < LCDFont_Bignum)
	{
		std::cout<< "Error drawText 1: Wrong font selected, must be > font 7 " << std::endl;
		return LCD_WrongFont;
	}

	// Check for null pointer
	if(pText == nullptr)
	{
		std::cout<< "Error drawText 2 :String array is not valid pointer" << std::endl;
		return LCD_CharArrayNullptr;
	}
	LCD_Return_Codes_e DrawCharReturnCode;
	while (*pText != '\0')
	{
	if (x > (_width - _CurrentFontWidth ))
	{
		x = 0;
		y += _CurrentFontheight ;
		if (y > (_height - _CurrentFontheight))
		{
			y = x = 0;
		}
	}
	DrawCharReturnCode = drawChar(x, y, *pText, color, bg);
	if(DrawCharReturnCode  != LCD_Success)
	{
		std::cout<< "Error drawText 3: Method drawChar failed" << std::endl;
		return DrawCharReturnCode;
	}
	x += _CurrentFontWidth ;
	pText++;
	}
	return LCD_Success;
}

/*!
	@brief  set rotation setting for LCDdisplayUpdate
	@param  mode enum LCD_rotate_e 0 thru 3 corresponding to 4 rotations:
*/
void NOKIA_5110_graphics::setRotation(LCD_rotate_e CurrentRotation) {

	switch(CurrentRotation) {
	case LCD_Degrees_0:
	case LCD_Degrees_180:
		_width  = LCDWIDTH;
		_height = LCDHEIGHT;
	break;
	case LCD_Degrees_90:
	case LCD_Degrees_270:
		_width  = LCDHEIGHT;
		_height = LCDWIDTH;
	break;
	}
	_LCD_rotate = CurrentRotation;
}

/*!
	@brief     Get rotation setting for display
	@returns   0 thru 3 corresponding to 4 cardinal rotations
*/
LCD_rotate_e NOKIA_5110_graphics::getRotation(void) {return _LCD_rotate;}

// ** EOF **
