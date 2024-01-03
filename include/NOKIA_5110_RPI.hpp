 /*!
	@file  NOKIA_5110_RPI.hpp
	@brief Header file for NOKIA 5110 library to communicate with LCD
			Controls SPI and basic functionality
	@details Project Name: NOKIA_5110_RPI
		URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
	@author  Gavin Lyons
*/

#pragma once

// ************ libraries ************
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <bcm2835.h>

#include "NOKIA_5110_graphics.hpp"

//*********** Definitions **************

#define LCD_BLACK 1 /**< LCD color on */
#define LCD_WHITE 0 /**< LCD color off */

#define LCDWIDTH  84 /**< LCD Width in pixels*/
#define LCDHEIGHT 48 /**< LCD Height in pixels */

#define LCD_FUNCTIONSET          0x20 /** LCD function set*/
#define LCD_POWERDOWN            0x04 /**< LCD power off */
#define LCD_ENTRYMODE            0x02 /**< LCD entry mode */
#define LCD_EXTENDEDINSTRUCTION  0x01 /**< LCD get into the EXTENDED mode when combined with Function set*/

#define LCD_DISPLAYCONTROL       0x08 /**< Set display control */
#define LCD_DISPLAYBLANK         0x00 /**< Blank display */
#define LCD_DISPLAYNORMAL        0x04 /**< normal mode display */
#define LCD_DISPLAYALLON         0x01 /**< all pixels on */
#define LCD_DISPLAYINVERTED      0x05 /**< display inverted */

#define LCD_SETYADDR             0x40
#define LCD_SETXADDR             0x80

#define LCD_SETTEMP  0x04   /**< set temperature coefficient */ 
#define LCD_CONTRAST 0xB0  /**< default value set LCD VOP contrast range 0xB1-BF */
#define LCD_BIAS 0x13  /**< LCD Bias mode 1:48 0x12 to 0x14 */

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

/*! Enum to hold current screen rotation in degrees  */
enum LCD_rotate_e : uint8_t
{
	LCD_Degrees_0 =   0,    /**< LCD screen rotated 0 degrees */
	LCD_Degrees_90 =  1,    /**< LCD screen rotated 90 degrees  */
	LCD_Degrees_180 = 2,    /**< LCD screen rotated 180 degrees  */
	LCD_Degrees_270 = 3     /**< LCD screen rotated 270 degrees */
};

// Section :: Classes

/*!
	@brief Class Controls SPI comms and LCD functionality
*/
class NOKIA_5110_RPI : public NOKIA_5110_graphics
{

public:

	NOKIA_5110_RPI(uint8_t LCD_RST, uint8_t LCD_DC, uint8_t LCD_CE, int8_t LCD_DIN, int8_t LCD_CLK);
	NOKIA_5110_RPI(uint8_t LCD_RST, uint8_t LCD_DC, uint32_t LCD_spi_divider, uint8_t SPICE_Pin);
	~NOKIA_5110_RPI(){};

	bool LCDBegin(bool Inverse = false, uint8_t Contrast = LCD_CONTRAST,uint8_t Bias = LCD_BIAS);
	void LCDenableSleep(void);
	void LCDdisableSleep(void);
	bool LCDIsSleeping(void);

	void LCDSPIHWSettings(void);
	void LCDSPIoff(void);

	void LCDPowerDown(void);

	virtual void LCDDrawPixel(uint8_t x, uint8_t y, bool color) override;
	void LCDsetRotation(LCD_rotate_e m);
	void LCDSetContrast(uint8_t con);
	void LCDdisplayUpdate(void);
	void LCDdisplayClear(void);
	void LCDfillScreen(void);
	void LCDfillScreenPattern(uint8_t);
	void LCDinvertDisplay(bool inv);

	uint8_t LCDDisplayBuffer[LCDWIDTH * (LCDHEIGHT  / 8)+1];

	uint16_t LCDLibVerNumGet(void);
	uint16_t LCDHighFreqDelayGet(void);
	void LCDHighFreqDelaySet(uint16_t);

private:

	void LCDWriteData(uint8_t data);
	void LCDWriteCommand(uint8_t command);
	bool isHardwareSPI(void);
	
	bool _LCDHardwareSPI = true;   /**< Hardware SPI true on , false off*/
	uint32_t _LCD_SPICLK_DIVIDER=0 ; /**< SPI clock divider , bcm2835SPIClockDivider , HW SPI only */
	uint8_t _LCD_SPICE_PIN = 0; /**< which SPI_CE pin to use , 0 or 1 , HW SPI only */
	uint16_t _LCDHighFreqDelay = 2; /**< uS GPIO Communications delay ,used in SW SPI ONLY */
	
	int8_t _LCD_DC; /**< Data or command GPIO */
	int8_t _LCD_RST; /**< Reset GPIO */
	int8_t _LCD_CE;    /**< Chip enable,  Software SPI only */
	int8_t _LCD_CLK; /**< Clock GPIO, Software SPI only*/
	int8_t _LCD_DIN; /**< Data GPIO ,Software SPI only */

	uint8_t  _contrast; /**< LCD contrast */
	uint8_t  _bias;     /**< LCD bias*/
	bool	 _inverse = false; /**< LCD inverted , false for off*/
	bool	_sleep; /**< LCD sleep mode*/

	const uint16_t _LibVersionNum = 130; /**< library version number 130 1.3.0*/
}; //end of class


