/*!
	@file: example/src/BitmapTests/main.cpp
	@brief Description library test file, carries out series of Bitmap tests , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@details https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
	@test
		-# Test 401 Bitmap full screen
		-# Test 402 Small bitmaps 
		-# Test 403 customChar methods

*/

// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <iostream> // for std::cout

#include "NOKIA_5110_RPI.hpp" // PCD8544 controller driver
#include "Bitmap_data.hpp" // bitmap Data for bitmap tests

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

void testBitMap(void);
void testCustomChar(void);
void testSmallBitmap(void);

// ************  MAIN ***************

int main(int argc, char **argv)
{
	Setup();
	myLCD.LCDdisplayClear();
	testBitMap();
	testSmallBitmap();
	testCustomChar();

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

void testBitMap(void)
{
	std::cout <<"Test 401 Bitmap full screen" << std::endl;
	myLCD.drawBitmap(0,0, backUpIcon, LCDWIDTH, LCDHEIGHT, LCD_BLACK, sizeof(backUpIcon)/sizeof(uint8_t)); // Splash screen
	screenReset();
}

void testCustomChar(void) {

	std::cout << "Test 403 custom character 5by8 " << std::endl;
	// Test custom character (the data is in included bitmap data file)
	myLCD.setCursor(0, 0);
	myLCD.customChar(myCustomChar, sizeof(myCustomChar)/sizeof(uint8_t)); // draws '|||'
	screenReset();
}

void screenReset(void) {
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDdisplayClear();
}


void testSmallBitmap(void)
{
	std::cout <<"Test 402 Small bitmaps " << std::endl;

	// Miniature bitmap display
	myLCD.drawBitmap(5, 5, SignalIcon, 16, 8, LCD_BLACK, sizeof(SignalIcon)/sizeof(uint8_t));
	myLCD.drawBitmap(60, 5, BatIcon, 16, 8, LCD_BLACK, sizeof(BatIcon)/sizeof(uint8_t));
	screenReset();
}

// *************** EOF ****************
