/*
 * Project Name:  PCD8544 Nokia 5110 SPI LCD display Library RPI
 * File: main.cpp
 * Description: library test file , "Hello world" ,basic use case, Software SPI 
 * Author: Gavin Lyons.
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
 */

// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <iostream> // for std::cout
#include "NOKIA_5110_RPI.hpp" // PCD8544 controller driver

// **************** GPIO ***************
#define RST_LCD 25
#define DC_LCD 24
#define SCLK_LCD 22
#define SDIN_LCD 27
#define CS_LCD 8

#define inverse  false // set to true to invert display pixel color
#define contrast 0xB2 // default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define bias 0x13 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14

NOKIA_5110_RPI myLCD(RST_LCD, DC_LCD, CS_LCD, SDIN_LCD, SCLK_LCD);

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
	myLCD.LCDBegin(inverse, contrast, bias);
	bcm2835_delay(250);
	myLCD.LCDdisplayClear();
}


void EndTests(void)
{
	myLCD.LCDPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "LCD End\r\n";
}

void Test(void)
{
	char testStr[]= "Hello SWSPI";
	
	myLCD.SetFontNum(LCDFontType_Default);
	myLCD.setTextSize(1);
	myLCD.setCursor(5, 5);
	myLCD.print(testStr);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(5000);
}
