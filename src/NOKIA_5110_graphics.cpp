


#include "NOKIA_5110_RPI.h"
#include "NOKIA_5110_RPI_Font.h"
#include "NOKIA_5110_graphics.h"

NOKIA_5110_graphics::NOKIA_5110_graphics()
{
}

size_t NOKIA_5110_graphics::write(uint8_t c)
{
	display_putc(c);
	return 1;
}

/* FunctionName: display_drawLine */

void NOKIA_5110_graphics::display_drawLine_2(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool color) {

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

/* FunctionName: drawHLine */
void NOKIA_5110_graphics::display_drawHLine(uint8_t x, uint8_t y, uint8_t w, bool color) {
   display_drawLine_2(x, y, x + w - 1, y, color);
}

/* FunctionName: drawVLine */
void NOKIA_5110_graphics::display_drawVLine(uint8_t x, uint8_t y, uint8_t h, bool color) {
  display_drawLine_2(x, y, x, y + h - 1, color);
}

void NOKIA_5110_graphics::display_fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color) {
  uint8_t i;
  for (i = x; i < x + w; i++)
	display_drawVLine(i, y, h, color);
}

/*
	Name:         display_writeLine
	Desc:   Write a line.  Bresenham's algorithm
	Param1:    x0  Start point x coordinate
	Param2:    y0  Start point y coordinate
	Param3:    x1  End point x coordinate
	Param4:    y1  End point y coordinate
	Param5:    color
*/
void NOKIA_5110_graphics::display_writeLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color) {
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


/*
   Desc:   Draw a line
	Param1:    x0  Start point x coordinate
	Param2:    y0  Start point y coordinate
	Param3:    x1  End point x coordinate
	Param4:    y1  End point y coordinate
	Param5:    color
*/
void NOKIA_5110_graphics::display_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color) {
	// Update in subclasses if desired!
	if(x0 == x1){
		if(y0 > y1) LCD_swap_int16_t(y0, y1);
		display_drawVLine(x0, y0, y1 - y0 + 1, color);
	} else if(y0 == y1){
		if(x0 > x1) LCD_swap_int16_t(x0, x1);
		display_drawVLine(x0, y0, x1 - x0 + 1, color);
	} else {
			display_writeLine(x0, y0, x1, y1, color);
	}
}


/*
   Desc:   Draw a circle outline
	Param1:    x0   Center-point x coordinate
	Param2:    y0   Center-point y coordinate
	Param3:    r   Radius of circle
	Param4:    color
*/
void NOKIA_5110_graphics::display_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color) {
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

/*
	Desc:   Quarter-circle drawer, used to do circles and roundrects
	Param1:    x0   Center-point x coordinate
	Param2:    y0   Center-point y coordinate
	Param3:    r   Radius of circle
	Param4:    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing
	Param5:    color
*/

void NOKIA_5110_graphics::display_drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint8_t color) {
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


/*
   Desc:   Draw a circle with filled color
	Param1:    x0   Center-point x coordinate
	Param2:    y0   Center-point y coordinate
	Param3:    r   Radius of circle
	Param4:    color
*/
void NOKIA_5110_graphics::display_fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color) {
	display_drawVLine(x0, y0-r, 2*r+1, color);
	display_fillCircleHelper(x0, y0, r, 3, 0, color);
}


/*
	Desc: Quarter-circle drawer with fill, used for circles and roundrects
	Param1:  x0       Center-point x coordinate
	Param2:  y0       Center-point y coordinate
	Param3:  r        Radius of circle
	Param4:  corners  Mask bits indicating which quarters we're doing
	Param5:  delta    Offset from center-point, used for round-rects
	Param6:  color
*/
void NOKIA_5110_graphics::display_fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t corners, uint16_t delta, uint8_t color) {
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
			if(corners & 1) display_drawVLine(x0+x, y0-y, 2*y+delta, color);
			if(corners & 2) display_drawVLine(x0-x, y0-y, 2*y+delta, color);
		}
		if(y != py) {
			if(corners & 1) display_drawVLine(x0+py, y0-px, 2*px+delta, color);
			if(corners & 2) display_drawVLine(x0-py, y0-px, 2*px+delta, color);
			py = y;
		}
		px = x;
	}
}


/*
   Desc:  Draw a rectangle with no fill color
	Param1:    x   Top left corner x coordinate
	Param2:    y   Top left corner y coordinate
	Param3:    w   Width in pixels
	Param4:    h   Height in pixels
	Param5:    color
*/
void NOKIA_5110_graphics::display_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color) {
	display_drawHLine(x, y, w, color);
	display_drawHLine(x, y+h-1, w, color);
	display_drawVLine(x, y, h, color);
	display_drawVLine(x+w-1, y, h, color);
}


/*
   Desc:  Draw a rounded rectangle with no fill color
	Param1:    x   Top left corner x coordinate
	Param2:    y   Top left corner y coordinate
	Param3:    w   Width in pixels
	Param4:    h   Height in pixels
	Param5:    r   Radius of corner rounding
	Param6:    color
*/
void NOKIA_5110_graphics::display_drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint8_t color) {
	int16_t max_radius = ((w < h) ? w : h) / 2;
	if(r > max_radius) r = max_radius;
	display_drawHLine(x+r  , y    , w-2*r, color); // Top
	display_drawHLine(x+r  , y+h-1, w-2*r, color); // Bottom
	display_drawVLine(x    , y+r  , h-2*r, color); // Left
	display_drawVLine(x+w-1, y+r  , h-2*r, color); // Right

	display_drawCircleHelper(x+r    , y+r    , r, 1, color);
	display_drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
	display_drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
	display_drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}


/*
   Desc:  Draw a rounded rectangle with fill color
	Param1:    x   Top left corner x coordinate
	Param2:    y   Top left corner y coordinate
	Param3:    w   Width in pixels
	Param4:    h   Height in pixels
	Param5:    r   Radius of corner rounding
	Param6:    color
*/
void NOKIA_5110_graphics::display_fillRoundRect(uint16_t x, uint16_t y, uint16_t w,
  uint16_t h, uint16_t r, uint8_t color) {
	int16_t max_radius = ((w < h) ? w : h) / 2;
	if(r > max_radius) r = max_radius;

	display_fillRect(x+r, y, w-2*r, h, color);

	display_fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
	display_fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}


/*
   Desc:  Draw a triangle with no fill color
	Param1:    x0  Vertex #0 x coordinate
	Param2:    y0  Vertex #0 y coordinate
	Param3:    x1  Vertex #1 x coordinate
	Param4:    y1  Vertex #1 y coordinate
	Param5:    x2  Vertex #2 x coordinate
	Param6:    y2  Vertex #2 y coordinate
	Param7:    color
*/
void NOKIA_5110_graphics::display_drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
  uint16_t x2, uint16_t y2, uint8_t color) {
	display_drawLine_2(x0, y0, x1, y1, color);
	display_drawLine_2(x1, y1, x2, y2, color);
	display_drawLine_2(x2, y2, x0, y0, color);
}

/*
   Desc:    Draw a triangle with color-fill
	Param1:    x0  Vertex #0 x coordinate
	Param2:    y0  Vertex #0 y coordinate
	Param3:    x1  Vertex #1 x coordinate
	Param4:    y1  Vertex #1 y coordinate
	Param5:    x2  Vertex #2 x coordinate
	Param6:    y2  Vertex #2 y coordinate
	Param7:    color
*/
void NOKIA_5110_graphics::display_fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
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
		display_drawHLine(a, y0, b-a+1, color);
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
		display_drawHLine(a, y, b-a+1, color);
	}


	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if(a > b) LCD_swap_int16_t(a,b);
		display_drawHLine(a, y, b-a+1, color);
	}
}

/*
	Desc: Print one byte/character of data
	Param1:  c The  ascii character to write
*/
void NOKIA_5110_graphics::display_putc(char c) {
  uint8_t i, j;
  if (c == ' ' && _cursorX == 0 && _textWrap)
	return;
  if(c == '\r') {
	_cursorX = 0;
	return;
  }
  if(c == '\n') {
	_cursorY += _textSize * 8;
	return;
  }

  for(i = 0; i < _CurrentFontWidth; i++ ) {
    uint8_t line =0;
    switch (_FontNumber)
		{
			case LCDFont_Default:
				line = FontDefault[c * _CurrentFontWidth + i];
			break;
			case LCDFont_Thick:
				line = FontThick[(c - LCD_ASCII_OFFSET) * _CurrentFontWidth + i];
			break;
			case LCDFont_Seven_Seg:
				line = FontSevenSeg[(c - LCD_ASCII_OFFSET) * _CurrentFontWidth + i];
			break;
			case LCDFont_Wide:
				line = FontWide[(c - LCD_ASCII_OFFSET) * _CurrentFontWidth + i];
			break;
			case LCDFont_Tiny:
				line = FontTiny[(c - LCD_ASCII_OFFSET) * _CurrentFontWidth + i];
			break;
		}
	for(j = 0; j < 8; j++, line >>= 1) {
	  if(line & 1) {
		if(_textSize == 1)
		  LCDDrawPixel(_cursorX + i, _cursorY + j, _textColor);
		else
		  display_fillRect(_cursorX + i * _textSize, _cursorY + j * _textSize, _textSize, _textSize, _textColor);
	  }
	  else
		if(_textBgColor != _textColor) {
		  if(_textSize == 1)
			LCDDrawPixel(_cursorX + i, _cursorY + j, _textBgColor);
		  else
			display_fillRect(_cursorX + i * _textSize, _cursorY + j * _textSize, _textSize, _textSize, _textBgColor);
		}
	}
  }

  if(_textBgColor != _textColor) {
	if(_textSize == 1)  display_drawVLine(_cursorX + _CurrentFontWidth, _cursorY, 8, _textBgColor);
	else               display_fillRect(_cursorX + _CurrentFontWidth * _textSize, _cursorY, _textSize, 8 * _textSize, _textBgColor);
  }

  _cursorX += _textSize * (_CurrentFontWidth+1);

  if( _cursorX > ((uint16_t)_width + _textSize * (_CurrentFontWidth+1)) )
	_cursorX = _width;

  if (_textWrap && (_cursorX + (_textSize * _CurrentFontWidth)) > _width)
  {
	_cursorX = 0;
	_cursorY += _textSize * 8;
	if( _cursorY > ((uint16_t)_height + _textSize * 8) )
	  _cursorY = _height;
  }
}

/*
  Desc: prints a string
  Param1: Pointer to a string of ASCII bytes
*/
void NOKIA_5110_graphics::display_puts(char *s) {
  while(*s)
	display_putc(*s++);
}

/*
  Desc: print custom char  (dimension: 5x8 x by y pixels)
*/
void NOKIA_5110_graphics::display_customChar(const uint8_t *c) {
  uint8_t i, j;
  for(i = 0; i < 5; i++ ) {
	uint8_t line = c[i];
	for(j = 0; j < 8; j++, line >>= 1) {
	  if(line & 1) {
		if(_textSize == 1)
		  LCDDrawPixel(_cursorX + i, _cursorY + j, _textColor);
		else
		  display_fillRect(_cursorX + i * _textSize, _cursorY + j * _textSize, _textSize, _textSize, _textColor);
	  }
	  else
		if(_textBgColor != _textColor) {
		  if(_textSize == 1)
			LCDDrawPixel(_cursorX + i, _cursorY + j, _textBgColor);
		  else
			display_fillRect(_cursorX + i * _textSize, _cursorY + j * _textSize, _textSize, _textSize, _textBgColor);
		}
	}
  }

  if(_textBgColor != _textColor) {  // If opaque, draw vertical line for last column
	if(_textSize == 1)  display_drawVLine(_cursorX + 5, _cursorY, 8, _textBgColor);
	else               display_fillRect(_cursorX + 5 * _textSize, _cursorY, _textSize, 8 * _textSize, _textBgColor);
  }

  _cursorX += _textSize * 6;

  if( _cursorX > ((uint16_t)_width + _textSize * 6) )
	_cursorX = _width;

  if (_textWrap && (_cursorX + (_textSize * 5)) > _width)
  {
	_cursorX = 0;
	_cursorY += _textSize * 8;
	if( _cursorY > ((uint16_t)_height + _textSize * 8) )
	  _cursorY = _height;
  }
}


/*
   Desc:  Draw a single character
	Param1:    x   Bottom left corner x coordinate
	Param2:    y   Bottom left corner y coordinate
	Param3:    c   The 8-bit font-indexed character (likely ascii)
	Param4:    color
	Param5:   (if same as color, no background)
	Param6:    size  Font magnification level, 1 is 'original' size
*/
void NOKIA_5110_graphics::display_drawChar(uint16_t x, uint16_t y, uint8_t c, uint8_t color, uint8_t bg,
	 uint8_t size) {
  uint16_t prev_x     = _cursorX;
  uint16_t	prev_y     = _cursorY;
  uint8_t 	prev_color = _textColor;
  uint8_t 	 prev_bg    = _textBgColor;
  uint8_t  prev_size  = _textSize;

  display_setCursor(x, y);
  display_setTextSize(size);
  display_setTextColor(color, bg);
  display_putc(c);

  _cursorX    = prev_x;
  _cursorY    = prev_y;
  _textColor   = prev_color;
  _textBgColor = prev_bg;
  _textSize    = prev_size;
}

void NOKIA_5110_graphics::display_setCursor(uint16_t x, uint16_t y) {
	_cursorX = x;
	_cursorY = y;
}

uint16_t NOKIA_5110_graphics::display_getCursorX(void) {
	return _cursorX;
}

uint16_t NOKIA_5110_graphics::display_getCursorY(void) {
	return _cursorY;
}

/*
	Desc:  Set text 'magnification' size. Each increase in s makes 1 pixel that much bigger.
	Param1: Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
*/
void NOKIA_5110_graphics::display_setTextSize(uint8_t s) {
	_textSize = (s > 0) ? s : 1;
}

void NOKIA_5110_graphics::display_setTextColor(uint8_t c, uint8_t b) {
	_textColor   = c;
	_textBgColor = b;
}

/*
	Desc: Whether text that is too long should '_textWrap' around to the next line.
	Param1:  Set true for _textWrapping, false for clipping
*/
void NOKIA_5110_graphics::display_setTextWrap(bool w) {
	_textWrap = w;
}

/*
	Desc:     Get rotation setting for display
	Returns:   0 thru 3 corresponding to 4 cardinal rotations
*/
uint8_t NOKIA_5110_graphics::display_getRotation(void) {
	return _rotation;
}

uint16_t NOKIA_5110_graphics::display_getWidth(void) {
	return _width;
}

uint16_t NOKIA_5110_graphics::display_getHeight(void) {
	return _height;
}

/*
   Desc:    Draw a ROM-resident 1-bit image at the specified (x,y) position,
			  using the specified foreground color (unset bits are transparent).
	Param1:    x   Top left corner x coordinate
	Param2:    y   Top left corner y coordinate
	Param3:    bitmap  byte array with monochrome bitmap
	Param4:    w   Width of bitmap in pixels
	Param5:    h   Height of bitmap in pixels
	Param6:    color
*/
void NOKIA_5110_graphics::display_drawBitmapV2(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h,
  uint8_t color) {

	uint16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t _byte = 0;
	uint16_t i, j;

	for(j = 0; j < h; j++, y++) {
		for( i = 0; i < w; i++ ) {
			if(i & 7) _byte <<= 1;
			else      _byte   = bitmap[j * byteWidth + i / 8];
			if(_byte & 0x80)
			  LCDDrawPixel(x+i, y, color);
		}
	}
}


// Desc :  Set the font number
// Param1: fontnumber 1-5 enum LCD_FONT_TYPE_e
// 1=default 2=thick 3=seven segment 4=wide 5=tiny

void NOKIA_5110_graphics::display_SetFontNum(LCD_FONT_TYPE_e FontNumber) {

	typedef enum  {
		LCDFont_width_3 = 3,
		LCDFont_width_4 = 4,
		LCDFont_width_5 = 5,
		LCDFont_width_7 = 7,
		LCDFont_width_8 = 8,
	}LCD_Font_width_e;

	_FontNumber = FontNumber;

	switch (_FontNumber)
	{
		case LCDFont_Default: _CurrentFontWidth = LCDFont_width_5;
			break; // Norm default 5 by 8
		case LCDFont_Thick: _CurrentFontWidth = LCDFont_width_7;
			break; //Thick 7 by 8 (NO LOWERCASE LETTERS)
		case LCDFont_Seven_Seg: _CurrentFontWidth = LCDFont_width_4;
			break; //Seven segment 4 by 8
		case LCDFont_Wide: _CurrentFontWidth = LCDFont_width_8;
			break; // Wide  8 by 8 (NO LOWERCASE LETTERS)
		case LCDFont_Tiny: _CurrentFontWidth = LCDFont_width_3;
			break; // tiny 3 by 8
	}
}

// ** EOF **
