/*
* Project Name: Nokia  5110 Graphic library for PIC  micro-controller
* Description: Source file for NOKIA 5110 library to communicate with LCD
* Author: Gavin Lyons.
* Created: July 2021
* Description: See URL for full details.
* URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
*/

#include "NOKIA_5110_RPI.h"

// ***********  Variables ************

uint8_t _width, _height, rotation;
bool	_sleep;
bool _hardwareSPI;
uint8_t _FontNumber = 1;
uint8_t _CurrentFontWidth = 5;


int8_t _LCD_DC;
int8_t _LCD_RST;
int8_t _LCD_CE;    // Software SPI only
int8_t _LCD_CLK; // Software SPI only
int8_t _LCD_DIN;  // Software SPI only

int16_t cursor_x = 0; // x location to start 
int16_t cursor_y = 0; // y location to start 
uint8_t textcolor = 0xFF;  //  background color for print
uint8_t textbgcolor = 0xFF;// text color for print
uint8_t textsize = 1;  //  Desired magnification of text to print
bool wrap = true;   // If set, 'wrap' text at right edge of display
	
uint8_t pcd8544_buffer[LCDWIDTH * (LCDHEIGHT  / 8)+1];


/* 
   Desc:  This sends the  commands to the PCD8544 to  init LCD, Note Software SPI
*/
void LCD_begin(int8_t rst, int8_t dc, int8_t ce, int8_t sclk, int8_t din) 
{
	_LCD_RST= rst;
	_LCD_DC = dc;
	LCD_RST_SetDigitalOutput;
	LCD_DC_SetDigitalOutput;
	bcm2835_delay(100);
	LCD_RST_SetHigh;
	LCD_RST_SetLow;
	bcm2835_delay(50);
	LCD_RST_SetHigh;
#ifndef LCD_SPI_HARDWARE
	_LCD_CE = ce;
	_LCD_DIN = din;  
	_LCD_CLK = sclk;
	LCD_CE_SetDigitalOutput;
	LCD_CE_SetHigh;
	LCD_CLK_SetDigitalOutput;
	LCD_DIN_SetDigitalOutput;
	_hardwareSPI = false;
#else
	LCD_SPI_Initialize();
	_hardwareSPI = true;
#endif

  // get into the EXTENDED mode
  writeCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION );
  writeCommand(LCD_SETBIAS | 0x03);   // set bias voltage
  writeCommand( LCD_SETVOP | 0x32);   // set contrast
  writeCommand(LCD_FUNCTIONSET);   // normal mode
  // set display to normal mode
  writeCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);

  rotation = 0;
  _width  = LCDWIDTH;
  _height = LCDHEIGHT;
}

// Desc : Init Hardware SPI
void LCD_SPI_Initialize(void)
{
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	//SPI_CLOCK_DIVIDER_32 = 7.8125MHz on Rpi2, 12.5MHz on RPI3
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32); 
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

// Desc: Init software Spi
// End SPI operations. SPI0 pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK), 
// P1-24 (CE0) and P1-26 (CE1) 
// are returned to their default INPUT behavior.

void LCD_SPIoff(void)
{
	bcm2835_spi_end();
}


// Desc: Call when powering down LCD
void LCD_Power_Down(void)
{
	bcm2835_gpio_write(_LCD_DC, LOW);
	bcm2835_gpio_write(_LCD_RST, LOW);
	
#ifndef LCD_SPI_HARDWARE
	bcm2835_gpio_write(_LCD_CLK, LOW);
	bcm2835_gpio_write(_LCD_DIN, LOW);
	bcm2835_gpio_write(_LCD_CE, LOW);
#endif

}

/* 
   Desc:  Writes a byte to the PCD8544
   Param: data byte will be sent as command or data depending on status of DC line
*/
void PCD8544_SPI_Write(uint8_t data) {
#ifndef LCD_SPI_HARDWARE
  uint8_t bit_n;
  for (bit_n = 0x80; bit_n; bit_n >>= 1) {
	LCD_CLK_SetLow;
	bcm2835_delayMicroseconds(LCD_HIGHFREQ_DELAY);
	if (data & bit_n) LCD_DIN_SetHigh;
	else           LCD_DIN_SetLow;
	LCD_CLK_SetHigh;
	bcm2835_delayMicroseconds(LCD_HIGHFREQ_DELAY);
  }
#else
	bcm2835_spi_transfer(data);
#endif
}

/* 
   Desc:  Writes a command byte to the PCD8544
   Param1: command There are two  banks in the LCD, data and commands. This
	function sets the DC low then sends
	the command byte if it(DC) was high it would be a data byte
*/
void writeCommand(uint8_t command) {
  LCD_DC_SetLow;
  if (!_hardwareSPI)LCD_CE_SetLow;
  PCD8544_SPI_Write(command);
  if (!_hardwareSPI)LCD_CE_SetHigh;
}

/* 
 Desc: Function to set contrast passed a byte 
 Param: con, Set LCD VOP Contrast, range = ((0x00-0x7F) |0x80) 0xB5 = (0x35|0x80) try 32 - 3F possible inputs. 
 */
void display_setContrast(uint8_t con) {
  if (con > 0x7f)
	con = 0x7f;

  writeCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION );
  writeCommand( LCD_SETVOP | con); 
  writeCommand(LCD_FUNCTIONSET);
 }

/* 
 Desc: Writes the buffer to the LCD
 */
void display(void) {
  uint16_t i;
  writeCommand(LCD_SETYADDR);  // set y = 0
  writeCommand(LCD_SETXADDR);  // set x = 0

  LCD_DC_SetHigh; //Data send
  if (!_hardwareSPI)LCD_CE_SetLow;

  for(i = 0; i < 504; i++)  // 504 = LCDWIDTH*LCDHEIGHT / 8
	PCD8544_SPI_Write( pcd8544_buffer[i] );

  if (!_hardwareSPI)LCD_CE_SetHigh;
}


/* 
 Desc: The most basic function, set a single pixel
 */
void drawPixel(uint8_t x, uint8_t y, bool color) {

  if ( (x >= _width) || (y >= _height) )
	return;

  switch(rotation) {
	case 1:
	  pcd8544_swap(x, y);
	  y =  LCDHEIGHT - 1 - y;
	  break;
	case 2:
	  x = LCDWIDTH - 1 - x;
	  y = LCDHEIGHT - 1 - y;
	  break;
	case 3:
	  pcd8544_swap(x, y);
	  x = LCDWIDTH - 1 - x;
  }

  if ( (x >= LCDWIDTH) || (y >= LCDHEIGHT) )
	return;

  if (color)
	pcd8544_buffer[x + (uint16_t)(y / 8) * LCDWIDTH] |=  (1 << (y & 7));

  else
	pcd8544_buffer[x + (uint16_t)(y / 8) * LCDWIDTH] &=  ~(1 << (y & 7));
}

/* FunctionName: drawLine */
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, int16_t y1, bool color) {

  bool steep = abs((int16_t)(y1 - y0)) > abs((int16_t)(x1 - x0));
  int8_t ystep;
  uint8_t dx, dy;
  int16_t err;

  if (steep) {
	pcd8544_swap(x0, y0);
	pcd8544_swap(x1, y1);
  }
  if (x0 > x1) {
	pcd8544_swap(x0, x1);
	pcd8544_swap(y0, y1);
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
	  drawPixel(y0, x0, color);
	else
	  drawPixel(x0, y0, color);
	err -= dy;
	if (err < 0) {
	  y0  += ystep;
	  err += dx;
	}
  }
}

/* FunctionName: drawHLine */
void drawHLine(uint8_t x, uint8_t y, uint8_t w, bool color) {
   drawLine(x, y, x + w - 1, y, color);
}

/* FunctionName: drawVLine */
void drawVLine(uint8_t x, uint8_t y, uint8_t h, bool color) {
  drawLine(x, y, x, y + h - 1, color);
}

void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color) {
  uint8_t i;
  for (i = x; i < x + w; i++)
	drawVLine(i, y, h, color);
}

/* 
 Desc: Writes the buffer (full of zeros) to the LCD
 */
void display_clear(void) {
  uint16_t i;
  for (i = 0; i < 504; i++)  // 504 = LCDWIDTH*LCDHEIGHT / 8
	pcd8544_buffer[i] = 0;
}

/* 
 Desc: Writes the buffer (full of ones(0xFF)) to the LCD
 */
void fillScreen() {
  uint16_t i;
  for (i = 0; i < 504; i++)  // 504 = LCDWIDTH*LCDHEIGHT / 8
	pcd8544_buffer[i] = 0xFF;
}

/* 
 Desc:  set rotation setting for display,
 Param1:  0 thru 3 corresponding to 4 cardinal rotations:
 */
void setRotation(uint8_t m) {
  rotation = (m & 3);

  switch(rotation) {
	case 0:
	case 2:
	  _width  = LCDWIDTH;
	  _height = LCDHEIGHT;
	break;
	case 1:
	case 3:
	  _width  = LCDHEIGHT;
	  _height = LCDWIDTH;
	  break;
  }
}

void invertDisplay(bool inv) {
  if(inv == 1)
	writeCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED);
  else
	writeCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
}


void LCDenableSleep(void)
{
	_sleep = true;
	writeCommand(LCD_FUNCTIONSET |  LCD_POWERDOWN);
}

void LCDdisableSleep(void)
{
	_sleep = false;
	  // get into the EXTENDED mode
  writeCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION );
  writeCommand(LCD_SETBIAS | 0x03);   // set bias voltage
  writeCommand( LCD_SETVOP | 0x32);   // set contrast
  writeCommand(LCD_FUNCTIONSET);   // normal mode
  // set display to normal mode
  writeCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
}

/*
	Name:         writeLine
	Desc:   Write a line.  Bresenham's algorithm 
	Param1:    x0  Start point x coordinate
	Param2:    y0  Start point y coordinate
	Param3:    x1  End point x coordinate
	Param4:    y1  End point y coordinate
	Param5:    color 
*/
void writeLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color) {
	bool steep = abs((int16_t)(y1 - y0)) > abs((int16_t)(x1 - x0));
	int16_t dx, dy, err, ystep;
	if (steep) {
		_swap_int16_t(x0, y0);
		_swap_int16_t(x1, y1);
	}

	if (x0 > x1) {
		_swap_int16_t(x0, x1);
		_swap_int16_t(y0, y1);
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
			display_drawPixel(y0, x0, color);
		} else {
			display_drawPixel(x0, y0, color);
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
void display_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color) {
	// Update in subclasses if desired!
	if(x0 == x1){
		if(y0 > y1) _swap_int16_t(y0, y1);
		display_drawVLine(x0, y0, y1 - y0 + 1, color);
	} else if(y0 == y1){
		if(x0 > x1) _swap_int16_t(x0, x1);
		display_drawHLine(x0, y0, x1 - x0 + 1, color);
	} else {
		writeLine(x0, y0, x1, y1, color);
	}
}


/*
   Desc:   Draw a circle outline
	Param1:    x0   Center-point x coordinate
	Param2:    y0   Center-point y coordinate
	Param3:    r   Radius of circle
	Param4:    color
*/
void display_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	display_drawPixel(x0  , y0+r, color);
	display_drawPixel(x0  , y0-r, color);
	display_drawPixel(x0+r, y0  , color);
	display_drawPixel(x0-r, y0  , color);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		display_drawPixel(x0 + x, y0 + y, color);
		display_drawPixel(x0 - x, y0 + y, color);
		display_drawPixel(x0 + x, y0 - y, color);
		display_drawPixel(x0 - x, y0 - y, color);
		display_drawPixel(x0 + y, y0 + x, color);
		display_drawPixel(x0 - y, y0 + x, color);
		display_drawPixel(x0 + y, y0 - x, color);
		display_drawPixel(x0 - y, y0 - x, color);
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

void display_drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint8_t color) {
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
			display_drawPixel(x0 + x, y0 + y, color);
			display_drawPixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			display_drawPixel(x0 + x, y0 - y, color);
			display_drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			display_drawPixel(x0 - y, y0 + x, color);
			display_drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			display_drawPixel(x0 - y, y0 - x, color);
			display_drawPixel(x0 - x, y0 - y, color);
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
void display_fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color) {
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
void display_fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t corners, uint16_t delta, uint8_t color) {
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
void display_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color) {
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
void display_drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint8_t color) {
	int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
	if(r > max_radius) r = max_radius;
	// smarter version
	display_drawHLine(x+r  , y    , w-2*r, color); // Top
	display_drawHLine(x+r  , y+h-1, w-2*r, color); // Bottom
	display_drawVLine(x    , y+r  , h-2*r, color); // Left
	display_drawVLine(x+w-1, y+r  , h-2*r, color); // Right
	// draw four corners
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
void display_fillRoundRect(uint16_t x, uint16_t y, uint16_t w,
  uint16_t h, uint16_t r, uint8_t color) {
	int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
	if(r > max_radius) r = max_radius;
	// smarter version
	display_fillRect(x+r, y, w-2*r, h, color);
	// draw four corners
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
void display_drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
  uint16_t x2, uint16_t y2, uint8_t color) {
	display_drawLine(x0, y0, x1, y1, color);
	display_drawLine(x1, y1, x2, y2, color);
	display_drawLine(x2, y2, x0, y0, color);
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
void display_fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
  uint16_t x2, uint16_t y2, uint8_t color) {
	int16_t a, b, y, last, dx01, dy01, dx02, dy02, dx12, dy12;
	int32_t
	sa   = 0,
	sb   = 0;

	if (y0 > y1) {
		_swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
	}
	if (y1 > y2) {
		_swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
	}
	if (y0 > y1) {
		_swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
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

	if(y1 == y2) last = y1;   // Include y1 scanline
	else         last = y1-1; // Skip it

	for(y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if(a > b) _swap_int16_t(a,b);
		display_drawHLine(a, y, b-a+1, color);
	}


	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if(a > b) _swap_int16_t(a,b);
		display_drawHLine(a, y, b-a+1, color);
	}
}


/*
	Desc: Print one byte/character of data
	Param1:  c  The 8-bit ascii character to write
*/
void display_putc(char c) {
  uint8_t i, j;
  if (c == ' ' && cursor_x == 0 && wrap)
	return;
  if(c == '\r') {
	cursor_x = 0;
	return;
  }
  if(c == '\n') {
	cursor_y += textsize * 8;
	return;
  }

  for(i = 0; i < _CurrentFontWidth; i++ ) {
    uint8_t line =0;
    switch (_FontNumber) 
		{
			case 1:
				line = Font_One[c * _CurrentFontWidth + i];
			break;
			case 2:
				line = Font_Two[(c - LCD_ASCII_OFFSET) * _CurrentFontWidth + i];
			break;
			case 3:
				line = Font_Three[(c - LCD_ASCII_OFFSET) * _CurrentFontWidth + i];
			break;
			case 4:
				line = Font_Four[(c - LCD_ASCII_OFFSET) * _CurrentFontWidth + i];
			break;
			case 5:
				line = Font_Five[(c - LCD_ASCII_OFFSET) * _CurrentFontWidth + i];
			break;
			default:
				printf("Error wrong font number set must be 1-5\n");
				return;
			break;
		}
	for(j = 0; j < 8; j++, line >>= 1) {
	  if(line & 1) {
		if(textsize == 1)
		  display_drawPixel(cursor_x + i, cursor_y + j, textcolor);
		else
		  display_fillRect(cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textcolor);
	  } 
	  else 
		if(textbgcolor != textcolor) {
		  if(textsize == 1)
			display_drawPixel(cursor_x + i, cursor_y + j, textbgcolor);
		  else
			display_fillRect(cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textbgcolor);
		}
	}
  }

  if(textbgcolor != textcolor) { 
	if(textsize == 1)  display_drawVLine(cursor_x + _CurrentFontWidth, cursor_y, 8, textbgcolor);
	else               display_fillRect(cursor_x + _CurrentFontWidth * textsize, cursor_y, textsize, 8 * textsize, textbgcolor);
  }

  cursor_x += textsize * (_CurrentFontWidth+1);

  if( cursor_x > ((uint16_t)_width + textsize * (_CurrentFontWidth+1)) )
	cursor_x = _width;

  if (wrap && (cursor_x + (textsize * _CurrentFontWidth)) > _width)
  {
	cursor_x = 0;
	cursor_y += textsize * 8;
	if( cursor_y > ((uint16_t)_height + textsize * 8) )
	  cursor_y = _height;
  }
}

/* 
  Desc: prints a string 
  Param1: Pointer to a string of ASCII bytes
*/
void display_puts(char *s) {
  while(*s)
	display_putc(*s++);
}

/* 
  Desc: print custom char  (dimension: 5x8 xbyy pixels)
*/
void display_customChar(const uint8_t *c) {
  uint8_t i, j;
  for(i = 0; i < 5; i++ ) {
	uint8_t line = c[i];
	for(j = 0; j < 8; j++, line >>= 1) {
	  if(line & 1) {
		if(textsize == 1)
		  display_drawPixel(cursor_x + i, cursor_y + j, textcolor);
		else
		  display_fillRect(cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textcolor);
	  } 
	  else 
		if(textbgcolor != textcolor) {
		  if(textsize == 1)
			display_drawPixel(cursor_x + i, cursor_y + j, textbgcolor);
		  else
			display_fillRect(cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textbgcolor);
		}
	}
  }

  if(textbgcolor != textcolor) {  // If opaque, draw vertical line for last column
	if(textsize == 1)  display_drawVLine(cursor_x + 5, cursor_y, 8, textbgcolor);
	else               display_fillRect(cursor_x + 5 * textsize, cursor_y, textsize, 8 * textsize, textbgcolor);
  }

  cursor_x += textsize * 6;

  if( cursor_x > ((uint16_t)_width + textsize * 6) )
	cursor_x = _width;

  if (wrap && (cursor_x + (textsize * 5)) > _width)
  {
	cursor_x = 0;
	cursor_y += textsize * 8;
	if( cursor_y > ((uint16_t)_height + textsize * 8) )
	  cursor_y = _height;
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
void display_drawChar(uint16_t x, uint16_t y, uint8_t c, uint8_t color, uint8_t bg,
	 uint8_t size) {
  uint16_t prev_x     = cursor_x;
  uint16_t	prev_y     = cursor_y;
  uint8_t 	prev_color = textcolor;
  uint8_t 	 prev_bg    = textbgcolor;
  uint8_t  prev_size  = textsize;

  display_setCursor(x, y);
  display_setTextSize(size);
  display_setTextColor(color, bg);
  display_putc(c);

  cursor_x    = prev_x;
  cursor_y    = prev_y;
  textcolor   = prev_color;
  textbgcolor = prev_bg;
  textsize    = prev_size;
}

void display_setCursor(uint16_t x, uint16_t y) {
	cursor_x = x;
	cursor_y = y;
}

uint16_t display_getCursorX(void) {
	return cursor_x;
}

uint16_t display_getCursorY(void) {
	return cursor_y;
}

/*
	Desc:  Set text 'magnification' size. Each increase in s makes 1 pixel that much bigger.
	Param1: Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
*/
void display_setTextSize(uint8_t s) {
	textsize = (s > 0) ? s : 1;
}

void display_setTextColor(uint8_t c, uint8_t b) {
	textcolor   = c;
	textbgcolor = b;
}

/*
	Desc: Whether text that is too long should 'wrap' around to the next line.
	Param1:  Set true for wrapping, false for clipping
*/
void display_setTextWrap(bool w) {
	wrap = w;
}

/*
	Desc:     Get rotation setting for display
	Returns:   0 thru 3 corresponding to 4 cardinal rotations
*/
uint8_t display_getRotation(void) {
	return rotation;
}

uint16_t display_getWidth(void) {
	return _width;
}

uint16_t display_getHeight(void) {
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
void display_drawBitmapV2(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h,
  uint8_t color) {

	uint16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t _byte = 0;
	uint16_t i, j;

	for(j = 0; j < h; j++, y++) {
		for( i = 0; i < w; i++ ) {
			if(i & 7) _byte <<= 1;
			else      _byte   = bitmap[j * byteWidth + i / 8];
			if(_byte & 0x80)
			  display_drawPixel(x+i, y, color);
		}
	}
}

// Called internally from display_printf
uint8_t printNumber(uint32_t n, int8_t n_width, uint8_t _flags) {
  uint8_t i=0, j, buff[10];
  do {
	  buff[i] = (uint8_t)( n % (_flags & 0x1F) );
	  if (buff[i] > 9)
		buff[i] += (_flags & 0x80) ? 0x07 : 0x27;
	  buff[i++] += '0';
	  n /= (_flags & 0x1F);
	  } while (n);
  j = i;
  if(_flags & 0x40) {
	n_width--;
	j++;
	if(_flags & 0x20) {    // put '-' before the zeros
	  display_putc('-');
	  _flags &= ~0x40;
	}
  }
  while (i < n_width--) {
	if (_flags & 0x20)  display_putc('0');
	else                display_putc(' ');
  }
  if (_flags & 0x40)
	display_putc('-');
  do {
	  display_putc(buff[--i]);
	  } while(i);
  return j;
}

// Called internally from display_printf
void printFloat(double float_n, int8_t f_width, int8_t decimal, uint8_t _flags) {
  int32_t int_part = float_n;
  double rem_part;
  if(decimal == 0)  decimal = 1;
  if(float_n < 0) {
	_flags |= 0x40;
	rem_part = (double)int_part - float_n;
	int_part = ~int_part + 1;
  }
  else
	rem_part = float_n - (double)int_part;
  _flags |= 10;
  f_width -= printNumber(int_part, f_width - decimal - 1, _flags);
  display_putc('.');
  f_width--;
  if(f_width < 1)  f_width = 1;
  if(decimal > f_width)  decimal = f_width;
  while( decimal > 0 && (rem_part != 0 || decimal > 0) ) {
	decimal--;
	rem_part *= 10;
	display_putc( (uint8_t)rem_part + '0' );
	rem_part -= (uint8_t)rem_part;
  }
}

/*
 Desc : Called from display_printf
 port of printf to display to screen instead of console
 */
void v_printf(const char *fmt, va_list arp) {
  uint8_t _flags, c, d=0, w=0;
  uint32_t nbr;
  while (1) 
  {
	c = *fmt++;

	if (!c)
	  break;

	if (c != '%') {
	  display_putc(c);
	  continue;
	}

	_flags = 0;
	c = *fmt++;

	if (c == '0') {
	  _flags |= 0x20;  // zero flag
	  c = *fmt++;
	}

	for (w = 0; c >= '0' && c <= '9'; c = *fmt++)   // width
	  w = w * 10 + c - '0';

	if (c == '.') {
	  c = *fmt++;
	  for (d = 0; c >= '0' && c <= '9'; c = *fmt++)   // decimals width
		d = d * 10 + c - '0';
	}
	
	if(c == 'f' || c == 'F') {    // if float number
	  printFloat(va_arg(arp, double), w, d, _flags);
	  continue;
	}

	if (c == 'l' || c == 'L') {   // long number (4 bytes)
	  _flags |= 0x40;    // long number flag
	  c = *fmt++;
	}

	if (!c)   // end of format?
	  break;

	if(c == 'X') {
	  _flags |= 0x80;   // upper case hex flag
	}
	
	if (c >= 'a')   // if lower case, switch to upper
	  c -= 0x20;
	switch (c) {
	  case 'C' :        // character
		display_putc( (uint8_t)va_arg(arp, int) ); continue;
	  case 'B' :        // binary
		_flags |= 2;  break;
	  case 'O' :        // octal
		_flags |= 8;  break;
	  case 'D' :        // signed decimal
	  case 'U' :        // unsigned decimal
		_flags |= 10; break;
	  case 'X' :        // hexadecimal
		_flags |= 16; break;
	  default:          // other
		display_putc(c); continue;
	}

	if(_flags & 0x40)  // if long number
	  nbr = (c == 'D') ? va_arg(arp, int32_t) : va_arg(arp, uint32_t);
	else
	  nbr = (c == 'D') ? (int32_t)va_arg(arp, int) : (uint32_t)va_arg(arp, int);
	if ( (c == 'D') &&  (nbr & 0x80000000) ) {
	  _flags |= 0x40;     // negative number flag
	  nbr = ~nbr + 1;     // change to positive form
	}
	else
	  _flags &= ~0x40;    // number is positive
	printNumber(nbr, w, _flags);
  }
}

/*
 * Desc: Printf port for screen
 * Param1: the format string
 * Param2: variable arguments passed
 */
void display_printf(const char *fmt, ...) {
  
  va_list arg;
  va_start(arg, fmt);
  v_printf(fmt, arg);
  va_end(arg);
}

// Desc :  Set the font number
// Param1: fontnumber 1-5
// 1=default 2=thick 3=seven segment 4=wide 5=tiny

void LCDFontNum(uint8_t FontNumber) {

	enum LCD_Font_width_LCD {
		BLANK = 0, FONT_W_FIVE = 5, FONT_W_SEVEN = 7, FONT_W_FOUR = 4, FONT_W_EIGHT = 8, FONT_W_THREE = 3
	};
    
	enum LCD_Font_width_LCD setfontwidth;
	_FontNumber = FontNumber;

	switch (_FontNumber) 
	{
		case 1: _CurrentFontWidth = (setfontwidth = FONT_W_FIVE);
			break; // Norm default 5 by 8
		case 2: _CurrentFontWidth = (setfontwidth = FONT_W_SEVEN);
			break; //Thick 7 by 8 (NO LOWERCASE LETTERS)
		case 3: _CurrentFontWidth = (setfontwidth = FONT_W_FOUR);
			break; //Seven segment 4 by 8
		case 4: _CurrentFontWidth = (setfontwidth = FONT_W_EIGHT);
			break; // Wide  8 by 8 (NO LOWERCASE LETTERS)
		case 5: _CurrentFontWidth = (setfontwidth = FONT_W_THREE);
			break; // tiny 3 by 8
		default:
			printf("Error : Wrong font number Set must be 1-5\n");
		break;
	}
}
/* ------------- EOF ------------------ */