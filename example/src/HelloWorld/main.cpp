/*
 * Project Name:  PCD8544 Nokia 5110 SPI LCD display Library RPI
 * File: main.cpp
 * Description: library test file , "Hello world" ,basic use case, HW SPI 
 * Author: Gavin Lyons.
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
 */

// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <iostream> // for std::cout
#include "NOKIA_5110_RPI.hpp" // PCD8544 controller driver

// **************** GLOBALS ***************
#define RST_LCD 25
#define DC_LCD 24

#define inverse  false // set to true to invert display pixel color
#define contrast 0xB2 // default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define bias 0x13 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14

const uint32_t SPICLK_FREQ = 64; // Spi clock divider see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1

NOKIA_5110_RPI myLCD(RST_LCD, DC_LCD);

// ************ Function Headers ********
void Setup(void);
void Test(void);
void EndTests(void);

// ************  MAIN ***************

int main(void)
{
	if(!bcm2835_init())
	{
		std::cout<< "Error 1201 : Problem with init bcm2835 library\r\n";
		return -1;
	}
	
	Setup();
	Test();
	EndTests();
	return 0;
}
// ********* End OF MAIN ********


// ******** Function Space *************

// Initialize the device

void Setup(void)
{
	bcm2835_delay(250);
	std::cout << "LCD Start\r\n" ;
	myLCD.LCDBegin(inverse, contrast, bias, SPICLK_FREQ, SPI_CE_PIN);
	bcm2835_delay(250);
	myLCD.LCDdisplayClear();
}


void EndTests(void)
{
	myLCD.LCDSPIoff(); //Stop the hardware SPI
	myLCD.LCDPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "LCD End\r\n";
}

void Test(void)
{
	char testStr[]= "Hello World";
	
	myLCD.SetFontNum(LCDFontType_Default);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(5000);

}
