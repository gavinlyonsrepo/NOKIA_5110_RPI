/*!
	@file: example/src/GraphicTests/main.cpp
	@brief Description library test file, carries out series of graphic  tests , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@details https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
	@test
		-# Test 202 draw pixel
		-# Test 203 Draw lines
		-# Test 204 draw shapes
*/


// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <iostream> // for std::cout
#include "NOKIA_5110_RPI.hpp" // PCD8544 controller driver

// *********** Test setup Defines **************
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// **************** GPIO ***************
#define RST_LCD 25
#define DC_LCD 24

#define inverse  false // set to true to invert display pixel color
#define contrast 0xB2 // default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define bias 0x13 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14

const uint32_t SPICLK_FREQ = 64; // Spi clock divider see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1

NOKIA_5110_RPI myLCD(RST_LCD, DC_LCD, SPICLK_FREQ, SPI_CE_PIN);

// ************ Function Headers ****************
bool Setup(void);
void screenReset(void);
void EndTests(void);

void textDrawPixel(void);
void testdrawcircle(void);
void testfillcircle(void);
void testDrawLine(void);
void testDrawChar(void);
void testDrawRect(void);
void testFillRect(void);
void testDrawTriangle(void);
void testFillTriangle(void);
void testDrawRoundRect(void);
void testFillRoundRect(void);

// ************  MAIN ***************

int main(int argc, char **argv)
{
	Setup(); 

	myLCD.LCDfillScreenPattern(0x31); // Splash screen
	screenReset();

	textDrawPixel();
	testDrawLine();

	std::cout <<"Test 204 Draw shapes" << std::endl;
	testDrawRect();
	testFillRect();
	testdrawcircle();
	testfillcircle();
	testDrawRoundRect();
	testFillRoundRect();
	testDrawTriangle();
	testFillTriangle();

	EndTests();
	return 0;
}
// ********* End OF MAIN ********


// ******** Function Space *************

// Initialize the device
bool Setup(void)
{
	std::cout << "LCD Start"  << std::endl;
	if(!bcm2835_init())
	{
		std::cout<< "Error 1201 : Setup :Problem with init bcm2835 library" << std::endl;
		return false;
	}else{
		std::cout<< "bcm2835 library version : " << bcm2835_version() << std::endl;
	}
	bcm2835_delay(250);
	if(!myLCD.LCDBegin(inverse, contrast, bias))
	{
		std::cout<< "Error 1202: Setup : bcm2835_spi_begin :Cannot start spi, Running as root?" << std::endl;
		return false;
	}
	std::cout<< "Nokia 5110 library version : " << myLCD.LCDLibVerNumGet() << std::endl;
	bcm2835_delay(250);
	return true;
}

void EndTests(void)
{
	myLCD.LCDSPIoff(); //Stop the hardware SPI
	myLCD.LCDPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "LCD End" << std::endl;
}

void textDrawPixel(void) {
	std::cout <<"Test 201 Draw pixels" << std::endl;
	myLCD.LCDDrawPixel(2, 5, LCD_BLACK);
	myLCD.LCDDrawPixel(10, 10, LCD_BLACK);
	myLCD.LCDDrawPixel(20, 20, LCD_BLACK);
	myLCD.LCDDrawPixel(30, 30, LCD_BLACK);
	myLCD.LCDDrawPixel(32, 35, LCD_BLACK);
	screenReset();
}

void testdrawcircle(void) {

	myLCD.drawCircle(20, 20, 10, LCD_BLACK);
	screenReset();
}

void testfillcircle(void)
{
	myLCD.fillCircle(60, 20, 10, LCD_BLACK);
	screenReset();
}

void testFillRect(void) {

	myLCD.fillRect(0, 20, 10, 30, LCD_BLACK);
	screenReset();
}

void testDrawTriangle(void) {
	myLCD.drawTriangle(30, 20, 50, 5, 80, 20, LCD_BLACK);
	screenReset();
}

void testFillTriangle(void) {

	myLCD.fillTriangle(10, 20, 40, 5, 80, 20, LCD_BLACK);
	screenReset();
}

void testDrawRoundRect(void) {
	myLCD.drawRoundRect(20, 20, 15 , 15, 5, LCD_BLACK);
	screenReset();
}

void testFillRoundRect(void) {
	myLCD.fillRoundRect(60, 20, 15, 15, 5, LCD_BLACK);
	screenReset();
}

void testDrawRect(void) {
	myLCD.drawRect(20, 15, 20, 15, LCD_BLACK);
	screenReset();
}

void testDrawLine(void) {

	std::cout <<"Test 202 Draw lines" << std::endl;
	myLCD.drawLine(0, 0, 40, 20, LCD_BLACK);
	myLCD.drawHLine(50, 20, 10 , LCD_BLACK); 
	myLCD.drawVLine(70, 20 ,10, LCD_BLACK); 
	screenReset();
}


void screenReset(void) {
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDdisplayClear();
}

// *************** EOF ****************
