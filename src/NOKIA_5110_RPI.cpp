/*
* Project Name: Nokia  5110  library 
* Description: Source file for NOKIA 5110 library to communicate with LCD
* Description: See URL for full details.
* URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
*/

#include "NOKIA_5110_RPI.hpp"

// Software SPI default
// Constructor of the class object from left to right pin 1-5(LCD)
// RST pin 1, DC pin 2,CE pin 3, DIN pin 4, CLK pin 5 
NOKIA_5110_RPI::NOKIA_5110_RPI(uint8_t LCD_RST, uint8_t LCD_DC, uint8_t LCD_CE, int8_t LCD_DIN, int8_t LCD_CLK) {
	
	_LCD_RST = LCD_RST;
	_LCD_CE = LCD_CE;
	_LCD_DC  = LCD_DC;
	_LCD_DIN  = LCD_DIN;
	_LCD_CLK  = LCD_CLK;
}

// HARDWARE SPI 
// Constructor of the class object from left to right pin 1-2(LCD)
// RST pin 1, DC pin 3
NOKIA_5110_RPI::NOKIA_5110_RPI(uint8_t LCD_RST, uint8_t LCD_DC) {
	
	_LCD_RST = LCD_RST;
	_LCD_DC  = LCD_DC;
	_LCD_CE  = -1;   // -1 for Chip enable specify using hardware SPI
	_LCD_DIN  = -1;  // -1 for din  specify using hardware SPI
	_LCD_CLK  = -1; // -1 for  sclk specify using hardware SPI
}


/* 
	 Desc:  This sends the commands to the PCD8544 to  init LCD, 
*/
void NOKIA_5110_RPI::LCDBegin(bool Inverse, uint8_t Contrast,uint8_t Bias, uint32_t spi_divider, uint8_t SPICE_Pin) 
{
	_inverse = Inverse;
	_bias = Bias;
	_contrast = Contrast;
	_LCD_SPICLK_DIVIDER  = spi_divider; // HW SPI only
	_LCD_SPICE_PIN = SPICE_Pin; // HW SPI only
	
	LCD_RST_SetDigitalOutput;
	LCD_DC_SetDigitalOutput;
	bcm2835_delay(100);
	LCD_RST_SetHigh;
	LCD_RST_SetLow;
	bcm2835_delay(50);
	LCD_RST_SetHigh;
if (isHardwareSPI() == false)
{
	LCD_CE_SetDigitalOutput;
	LCD_CE_SetHigh;
	LCD_CLK_SetDigitalOutput;
	LCD_DIN_SetDigitalOutput;
}else{
	LCDSPIInitialize();
}

	// get into the EXTENDED mode
	LCDWriteCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION );
	LCDWriteCommand(_bias); 
	LCDWriteCommand(LCD_SETTEMP); 
	LCDWriteCommand(_contrast); 
	LCDWriteCommand(LCD_FUNCTIONSET); //We must send 0x20 before modifying the display control mode
	// set LCDdisplayUpdate to normal mode or inverted
	if (_inverse  == false)
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL); 
	else
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED); 

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
	
	// SPI bus speed
	if (_LCD_SPICLK_DIVIDER > 0)
		bcm2835_spi_setClockDivider(_LCD_SPICLK_DIVIDER);
	else // default, BCM2835_SPI_CLOCK_DIVIDER_64 3.90MHz Rpi2, 6.250MHz RPI3
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); 
	
	// Chip enable pin select
	if (_LCD_SPICE_PIN == 0)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	}else if (_LCD_SPICE_PIN  == 1)
	{
		bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);
	}
}

// Desc: Init software Spi
// End SPI operations. SPI0 pins P1-19 (MOSI), P1-21 (MISO), P1-23 (CLK), 
// P1-24 (CE0) and P1-26 (CE1) 
// are returned to their default INPUT behavior.
// NOTE :: Hardware SPI only 
void NOKIA_5110_RPI::LCDSPIoff(void)
{
	if(isHardwareSPI() == true){bcm2835_spi_end();}
}


// Desc: Call when powering down LCD
void NOKIA_5110_RPI::LCDPowerDown(void)
{
	bcm2835_gpio_write(_LCD_DC, LOW);
	bcm2835_gpio_write(_LCD_RST, LOW);
	
	if(isHardwareSPI() == false)
	{
		bcm2835_gpio_write(_LCD_CLK, LOW);
		bcm2835_gpio_write(_LCD_DIN, LOW);
		bcm2835_gpio_write(_LCD_CE, LOW);
	}

}

/* 
	 Desc:  Writes a byte to the PCD8544
	 Param: data byte will be sent as command or data depending on status of DC line
*/
void NOKIA_5110_RPI::LCDWriteData(uint8_t data) 
{
	if (isHardwareSPI() == false)
	{
		uint8_t bit_n;
		for (bit_n = 0x80; bit_n; bit_n >>= 1) 
		{
			LCD_CLK_SetLow;
			bcm2835_delayMicroseconds(LCD_HIGHFREQ_DELAY);
			if (data & bit_n) 
				LCD_DIN_SetHigh;
			else           
				LCD_DIN_SetLow;
			LCD_CLK_SetHigh;
			bcm2835_delayMicroseconds(LCD_HIGHFREQ_DELAY);
		}
	}else{
			bcm2835_spi_transfer(data);
		}
}

/* 
	 Desc:  Writes a command byte to the PCD8544
	 Param1: command 
*/
void NOKIA_5110_RPI::LCDWriteCommand(uint8_t command) {
	LCD_DC_SetLow;
	if (isHardwareSPI() == false)LCD_CE_SetLow;
	LCDWriteData(command);
	if (isHardwareSPI() == false)LCD_CE_SetHigh;
}

/* 
 Desc: Function to set contrast passed a byte 
 Param: con, Set LCD VOP Contrast B0 to BF
 */
void NOKIA_5110_RPI::LCDSetContrast(uint8_t contrast) {
	_contrast = contrast;
	LCDWriteCommand(LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION );
	LCDWriteCommand(_contrast); 
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
	if (isHardwareSPI() == false)LCD_CE_SetLow;

	for(i = 0; i < 504; i++)  // 504 = LCDWIDTH*LCDHEIGHT / 8
	LCDWriteData( LCDDisplayBuffer[i] );

	if (isHardwareSPI() == false)LCD_CE_SetHigh;
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

// Desc :: inverts color on display
// Param :: True = Inverted mode , False = Display control normal
void NOKIA_5110_RPI::LCDinvertDisplay(bool invert) {
	if (invert == false){
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL); 
		_inverse = false;
	}
	else{
		_inverse = true;
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED); 
	}
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
	LCDWriteCommand(_bias); 
	LCDWriteCommand(LCD_SETTEMP); 
	LCDWriteCommand(_contrast); 
	LCDWriteCommand(LCD_FUNCTIONSET);   // normal mode
	// set LCDdisplayUpdate to normal mode
	if (_inverse  == false)
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL); 
	else
		LCDWriteCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED); 
}

// Desc : LCDisSleeping
// Returns  value of _sleep, if true LCD is in sleep mode.
bool NOKIA_5110_RPI::LCDIsSleeping() { return _sleep;}

// Desc: Checks if software SPI is on
// Returns: true 1 if hardware SPi on , false 0 for software spi
bool NOKIA_5110_RPI::isHardwareSPI() 
{
  return (_LCD_DIN == -1 && _LCD_CLK == -1 && _LCD_CE == -1);
}

/* ------------- EOF ------------------ */
