/*
* Project Name: Nokia  5110  library 
* Description: Header file for NOKIA 5110 library to communicate with LCD
* Controls SPI and basic functionality 
* Author: Gavin Lyons.
* Created: July 2021
* Description: See URL for full details.
* URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
*/

#ifndef _NOKIA_5110_RPI_H
#define _NOKIA_5110_RPI_H

// ************ libraries ************
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h> 
#include <bcm2835.h>

#include "NOKIA_5110_graphics.h"

//*********** Definitions **************

// ******** USER OPTION 1 SPI TYPE ***********
// SPI LCD module connections
// *** If software SPI module is used, comment this line OUT ***
#define LCD_SPI_HARDWARE
// *******************************************

#define LCD_BLACK 1
#define LCD_WHITE 0

#define LCDWIDTH  84
#define LCDHEIGHT 48

#define LCD_POWERDOWN            0x04
#define LCD_ENTRYMODE            0x02
#define LCD_EXTENDEDINSTRUCTION  0x01

#define LCD_DISPLAYBLANK         0x00
#define LCD_DISPLAYNORMAL        0x04
#define LCD_DISPLAYALLON         0x01
#define LCD_DISPLAYINVERTED      0x05

#define LCD_FUNCTIONSET          0x20
#define LCD_DISPLAYCONTROL       0x08
#define LCD_SETYADDR             0x40
#define LCD_SETXADDR             0x80

#define LCD_SETTEMP  0x04
#define LCD_SETBIAS  0x13
#define LCD_SETVOP   0x80

#define LCD_HIGHFREQ_DELAY 2 // uS used in SW SPI

// GPIO 
#define LCD_DC_SetHigh  bcm2835_gpio_write(_LCD_DC, HIGH)
#define LCD_DC_SetLow  bcm2835_gpio_write(_LCD_DC, LOW)
#define LCD_RST_SetHigh  bcm2835_gpio_write(_LCD_RST, HIGH)
#define LCD_RST_SetLow  bcm2835_gpio_write(_LCD_RST, LOW)
#define LCD_CE_SetHigh bcm2835_gpio_write(_LCD_CE, HIGH) // SW SPI last 6 lines
#define LCD_CE_SetLow bcm2835_gpio_write(_LCD_CE, LOW)
#define LCD_CLK_SetHigh bcm2835_gpio_write(_LCD_CLK, HIGH)
#define LCD_CLK_SetLow  bcm2835_gpio_write(_LCD_CLK, LOW)
#define LCD_DIN_SetHigh bcm2835_gpio_write(_LCD_DIN, HIGH)
#define LCD_DIN_SetLow  bcm2835_gpio_write(_LCD_DIN,LOW)

#define LCD_DC_SetDigitalOutput bcm2835_gpio_fsel(_LCD_DC, BCM2835_GPIO_FSEL_OUTP)
#define LCD_RST_SetDigitalOutput bcm2835_gpio_fsel(_LCD_RST, BCM2835_GPIO_FSEL_OUTP)
#define LCD_CLK_SetDigitalOutput bcm2835_gpio_fsel(_LCD_CLK, BCM2835_GPIO_FSEL_OUTP) // SW SPI last 3 lines
#define LCD_DIN_SetDigitalOutput bcm2835_gpio_fsel(_LCD_DIN, BCM2835_GPIO_FSEL_OUTP)
#define LCD_CE_SetDigitalOutput bcm2835_gpio_fsel(_LCD_CE, BCM2835_GPIO_FSEL_OUTP)

#define LCD_swap_uint8_t(a, b) { uint8_t t = a; a = b; b = t; }

// Section : Enums

typedef enum  {
	LCD_Degrees_0 = 0, LCD_Degrees_90, LCD_Degrees_180, LCD_Degrees_270
}LCD_rotate_e; // LCD rotate modes in degrees 

// Section :: Classes 


class NOKIA_5110_RPI : public NOKIA_5110_graphics
{

public:

	NOKIA_5110_RPI();
	~NOKIA_5110_RPI(){};
	
	void LCDBegin(int8_t, int8_t, int8_t, int8_t, int8_t);
	void LCDenableSleep(void);
	void LCDdisableSleep(void);

	void LCDSPIoff(void);
	void LCDPowerDown(void);
	
	virtual void LCDDrawPixel(uint8_t x, uint8_t y, bool color) override;
	void LCDsetRotation(LCD_rotate_e m);
	void LCDSetContrast(uint8_t con);
	void LCDdisplayUpdate(void);
	void LCDdisplayClear(void);
	void LCDfillScreen(void);
	void LCDinvertDisplay(bool inv);
	
	uint8_t LCDDisplayBuffer[LCDWIDTH * (LCDHEIGHT  / 8)+1];
	
private:

	void LCDSPIInitialize(void);
	void LCDWriteData(uint8_t data);
	void LCDWriteCommand(uint8_t command);
	
	bool	_sleep;
	bool _hardwareSPI;
	int8_t _LCD_DC;
	int8_t _LCD_RST;
	int8_t _LCD_CE;    // Software SPI only
	int8_t _LCD_CLK; // Software SPI only
	int8_t _LCD_DIN;  // Software SPI only
	
}; //end of class


#endif // .h file Guard header
