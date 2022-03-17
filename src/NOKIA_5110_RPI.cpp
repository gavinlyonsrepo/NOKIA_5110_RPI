/*
* Project Name: Nokia  5110  library 
* Description: Source file for NOKIA 5110 library to communicate with LCD
* Description: See URL for full details.
* URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
*/

#include "NOKIA_5110_RPI.h"

NOKIA_5110_RPI::NOKIA_5110_RPI()
{
}

/* 
	 Desc:  This sends the  commands to the PCD8544 to  init LCD, 
	 Param1 : reset pin
	 Param2 : Data or command Pin
	 Param3 : Chip enable pin 
	 Param4 : Clock pin
	 Param5 : data in pin 
	 Note if using Hw SPI set Param3-5 to -1.
*/
void NOKIA_5110_RPI::LCDBegin(int8_t rst, int8_t dc, int8_t ce, int8_t sclk, int8_t din) 
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
	LCDSPIInitialize();
	_hardwareSPI = true;
#endif

	// get into the EXTENDED mode
	LCDWriteCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION );
	LCDWriteCommand(LCD_SETBIAS | 0x03);   // set bias voltage
	LCDWriteCommand( LCD_SETVOP | 0x32);   // set contrast
	LCDWriteCommand(LCD_FUNCTIONSET);   // normal mode
	// set LCDdisplayUpdate to normal mode
	LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);

	_rotation = 0;
	_width  = LCDWIDTH;
	_height = LCDHEIGHT;
}

// Desc : Init Hardware SPI
void NOKIA_5110_RPI::LCDSPIInitialize()
{
	bcm2835_spi_begin();
	
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); //MSB
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); // Mode 0
	
	// This Sets the SPI clock divider and therefore the SPI clock speed.
	//SPI_CLOCK_DIVIDER_32 = BCM2835_SPI_CLOCK_DIVIDER_64 	
	//64 = 3.90625MHz on Rpi2, 6.250MHz on RPI3
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); 
	
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

// Desc: Init software Spi
// End SPI operations. SPI0 pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK), 
// P1-24 (CE0) and P1-26 (CE1) 
// are returned to their default INPUT behavior.

void NOKIA_5110_RPI::LCDSPIoff(void)
{
	bcm2835_spi_end();
}


// Desc: Call when powering down LCD
void NOKIA_5110_RPI::LCDPowerDown(void)
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
void NOKIA_5110_RPI::LCDWriteData(uint8_t data) {
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
	 Param1: command 
*/
void NOKIA_5110_RPI::LCDWriteCommand(uint8_t command) {
	LCD_DC_SetLow;
	if (!_hardwareSPI)LCD_CE_SetLow;
	LCDWriteData(command);
	if (!_hardwareSPI)LCD_CE_SetHigh;
}

/* 
 Desc: Function to set contrast passed a byte 
 Param: con, Set LCD VOP Contrast, range = ((0x00-0x7F) |0x80) 0xB5 = (0x35|0x80) try 32 - 3F possible inputs. 
 */
void NOKIA_5110_RPI::LCDSetContrast(uint8_t con) {
	if (con > 0x7f)
	con = 0x7f;

	LCDWriteCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION );
	LCDWriteCommand( LCD_SETVOP | con); 
	LCDWriteCommand(LCD_FUNCTIONSET);
 }

/* 
 Desc: Writes the buffer to the LCD
 */
void NOKIA_5110_RPI::LCDdisplayUpdate(void) {
	uint16_t i;
	LCDWriteCommand(LCD_SETYADDR);  // set y = 0
	LCDWriteCommand(LCD_SETXADDR);  // set x = 0

	LCD_DC_SetHigh; //Data send
	if (!_hardwareSPI)LCD_CE_SetLow;

	for(i = 0; i < 504; i++)  // 504 = LCDWIDTH*LCDHEIGHT / 8
	LCDWriteData( LCDDisplayBuffer[i] );

	if (!_hardwareSPI)LCD_CE_SetHigh;
}


/* 
 Desc: The most basic function, set a single pixel in the buffer
 */
void NOKIA_5110_RPI::LCDDrawPixel(uint8_t x, uint8_t y, bool color) {

	if ( (x >= _width) || (y >= _height) )
	return;

	switch(_rotation) {
	case LCD_Degrees_90:
		LCD_swap_uint8_t(x, y);
		y =  LCDHEIGHT - 1 - y;
		break;
	case LCD_Degrees_180:
		x = LCDWIDTH - 1 - x;
		y = LCDHEIGHT - 1 - y;
		break;
	case LCD_Degrees_270:
		LCD_swap_uint8_t(x, y);
		x = LCDWIDTH - 1 - x;
	}

	if ( (x >= LCDWIDTH) || (y >= LCDHEIGHT) )
	return;

	if (color)
		LCDDisplayBuffer[x + (uint16_t)(y / 8) * LCDWIDTH] |=  (1 << (y & 7));
	else
		LCDDisplayBuffer[x + (uint16_t)(y / 8) * LCDWIDTH] &=  ~(1 << (y & 7));
}


/* 
 Desc: Writes the buffer (full of zeros) to the LCD
 */
void NOKIA_5110_RPI::LCDdisplayClear(void) {
	uint16_t i;
	for (i = 0; i < ((LCDWIDTH*LCDHEIGHT) / 8) ; i++)  // 504 = LCDWIDTH*LCDHEIGHT / 8
		LCDDisplayBuffer[i] = 0;
}

/* 
 Desc: Writes the buffer (full of ones(0xFF)) to the LCD
 */
void NOKIA_5110_RPI::LCDfillScreen() {
	uint16_t i;
	for (i = 0; i < ((LCDWIDTH*LCDHEIGHT) / 8); i++)  // 504 = LCDWIDTH*LCDHEIGHT / 8
		LCDDisplayBuffer[i] = 0xFF;
}

/* 
 Desc:  set rotation setting for LCDdisplayUpdate,
 Param1:  enum LCD_rotate_e 0 thru 3 corresponding to 4 rotations:
 */
void NOKIA_5110_RPI::LCDsetRotation(LCD_rotate_e mode) {
	_rotation = (mode & 3);

	switch(_rotation) {
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
}

void NOKIA_5110_RPI::LCDinvertDisplay(bool inv) {
	if(inv == 1)
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED);
	else
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
}


void NOKIA_5110_RPI::LCDenableSleep(void)
{
	_sleep = true;
	LCDWriteCommand(LCD_FUNCTIONSET |  LCD_POWERDOWN);
}

void NOKIA_5110_RPI::LCDdisableSleep(void)
{
	_sleep = false;
	// get into the EXTENDED mode
	LCDWriteCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION );
	LCDWriteCommand(LCD_SETBIAS | 0x03);   // set bias voltage
	LCDWriteCommand( LCD_SETVOP | 0x32);   // set contrast
	LCDWriteCommand(LCD_FUNCTIONSET);   // normal mode
	// set LCDdisplayUpdate to normal mode
	LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
}

/* ------------- EOF ------------------ */
