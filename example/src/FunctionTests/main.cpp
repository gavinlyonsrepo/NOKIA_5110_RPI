/*!
	@file: example/src/FunctionTests/main.cpp
	@brief Description library test file, carries out series of Function tests , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@details https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
	@test
		-# Test 301 fill + clear screen
		-# Test 302 Sleep
		-# Test 303 Rotation
		-# Test 304 invert screen command
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

void testFillScreen(void);
void testSleepMode(void);
void testRotate(void);
void testInvert(void);


// ************  MAIN ***************

int main(int argc, char **argv)
{
	if(!Setup()) return -1;

	testFillScreen();
	testSleepMode();
	testRotate();
	testInvert();

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
		bcm2835_close(); // Close the bcm2835 library
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

void testSleepMode(void) {

	std::cout <<"Test 302 Sleep mode" << std::endl;
	char testStr1[]= "Sleep 5 secs!\r\n";
	char testStr2[]= "Awake!\r\n";

	myLCD.LCDdisplayClear();
	myLCD.setCursor(0, 0);
	myLCD.setTextSize(1);
	myLCD.print(testStr1);
	myLCD.LCDdisplayUpdate();

	bcm2835_delay(TEST_DELAY2);
	myLCD.LCDenableSleep();
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDdisableSleep();

	myLCD.print(testStr2);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY2);
	screenReset();
}


void testRotate(void) {

	std::cout <<"Test 303 rotate mode" << std::endl;
	char testStr0[]= "Rotate 0\r\n";
	char testStr1[]= "Rotate 90\r\n";
	char testStr2[]= "Rotate 180\r\n";
	char testStr3[]= "Rotate 270\r\n";

	// rotation example
	myLCD.LCDdisplayClear();
	myLCD.setTextSize(1);
	myLCD.SetFontNum(LCDFont_Tiny); 
	myLCD.setTextColor(LCD_BLACK, LCD_WHITE);
	myLCD.setCursor(0, 0);

	myLCD.setRotation(LCD_Degrees_90); // rotate 90 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	screenReset();

	myLCD.setRotation(LCD_Degrees_180); // rotate 180 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr2);
	screenReset();

	myLCD.setRotation(LCD_Degrees_270); // rotate 270 degrees counter clockwise,
	myLCD.setCursor(0, 0);
	myLCD.print(testStr3);
	screenReset();

	// revert back to no rotation
	myLCD.setRotation(LCD_Degrees_0);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr0);

	screenReset();
}


void testFillScreen(void){
	std::cout <<"Test 301 fill screen"  << std::endl;
	myLCD.LCDfillScreen();
	screenReset();
	myLCD.LCDfillScreenPattern(0x71);
	screenReset();
}

void screenReset(void) {
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDdisplayClear();
}

void testInvert(void)
{
	std::cout <<"Test 304 invert screen command" << std::endl;

	screenReset();
	myLCD.SetFontNum(LCDFont_Default);
	myLCD.setTextSize(2);
	myLCD.setCursor(5, 5);
	myLCD.print("INVERT 123");
	screenReset();

	// Invert the display
	myLCD.LCDinvertDisplay(true);
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDinvertDisplay(false);
	bcm2835_delay(TEST_DELAY5);
}


// *************** EOF ****************
