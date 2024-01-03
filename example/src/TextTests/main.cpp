/*!
	@file: example/src/TextTests/main.cpp
	@brief Description library test file, carries out series of text tests , HW SPI
			PCD8544 Nokia 5110 SPI LCD display Library
	@author Gavin Lyons.
	@details https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
	@test
		-# Test 111 draw ASCII font , drawchar +  drawtext methods
		-# Test 112 fonts 1-6
		-# Test 113 fonts 7-10
		-# Test 114 Print method
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

void testDrawChar(void);
void testTextModes(void);
void testFonts_1_6(void);
void testFonts_7_8(void);
void testFonts_9_10(void);

// ************  MAIN ***************

int main(int argc, char **argv)
{
	Setup();

	myLCD.LCDfillScreenPattern(0x31); // Splash screen
	screenReset();

	testDrawChar();
	testFonts_1_6();
	testFonts_7_8();
	testFonts_9_10();
	testTextModes();

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


void testFonts_1_6(void)
{
	std::cout <<"Test 112 Test fonts 1-6" << std::endl;

	char testStr1[]= "size1";
	char testStr2[]= "size2";
	char testStr3[]= "s3";

	char testStr5[]= "123";
	char testStr4[]= "THICK 2082";
	char testStr6[]= "Seven Seg 1273";
	char testStr8[]= "WIDE 9337";
	char testStr10[]= "Tiny 1837";
	char testStr12[]= "Homespun";

	// default font 1
	myLCD.SetFontNum(LCDFontType_Default);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.setTextSize(2);
	myLCD.setCursor(0,9);
	myLCD.print(testStr2);
	myLCD.setTextSize(3);
	myLCD.setCursor(0,25);
	myLCD.print(testStr3);
	screenReset();

	// THICK font 2
	myLCD.setCursor(0, 0);
	myLCD.setTextSize(1);
	myLCD.SetFontNum(LCDFontType_Thick);
	myLCD.print(testStr4);
	myLCD.setTextSize(2);
	myLCD.setCursor(10, 20);
	myLCD.print(testStr5);
	screenReset();

	// Seven seg font 3
	myLCD.SetFontNum(LCDFontType_SevenSeg);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 12);
	myLCD.print(testStr6);
	myLCD.setTextSize(2);
	myLCD.setCursor(10, 20);
	myLCD.print(testStr5);
	screenReset();

	// WIDE  font 4
	myLCD.SetFontNum(LCDFontType_Wide);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr8);
	myLCD.setTextSize(2);
	myLCD.setCursor(10, 20);
	myLCD.print(testStr5);
	screenReset();

	//tiny font 5
	myLCD.SetFontNum(LCDFontType_Tiny);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr10);
	myLCD.setTextSize(2);
	myLCD.setCursor(10, 20);
	myLCD.print(testStr5);
	screenReset();

	//homespun font 6
	myLCD.SetFontNum(LCDFontType_Homespun);
	myLCD.setTextSize(1);
	myLCD.setCursor(0, 0);
	myLCD.print(testStr12);
	myLCD.setTextSize(2);
	myLCD.setCursor(10, 23);
	myLCD.print(testStr5);
	screenReset();

	myLCD.setTextSize(1);
	myLCD.SetFontNum(LCDFontType_Default);
	myLCD.setCursor(0, 0);
}


void testTextModes(void) {

	std::cout <<"Test 114 print method" << std::endl;

	char testStr1[]= "Hello World!\r\n";
	char testStr2[]= "0xABCD";
	char testStr3[]= "20:37\r\n";
	char testStr4[]= "TEXT WRAP 123.\r\n";
	
	// text display tests


	myLCD.setTextSize(1);
	myLCD.SetFontNum(LCDFontType_Default);
	myLCD.setTextColor(LCD_WHITE, LCD_BLACK); // 'inverted' text
	myLCD.setCursor(0, 0);
	myLCD.print(testStr1);
	myLCD.print(3.141592, 4); // prints 3.1416
	myLCD.setCursor(0, 24);
	myLCD.setTextSize(2);
	myLCD.setTextColor(LCD_BLACK, LCD_WHITE);
	myLCD.print(testStr2);

	screenReset();
	// Numbers
	myLCD.setCursor(0, 0);
	myLCD.setTextSize(1);
	myLCD.SetFontNum(LCDFontType_Wide);
	myLCD.print(testStr3);
	myLCD.print(-14790);
	myLCD.setCursor(0, 26);
	myLCD.print(83.6);

	screenReset();

	// Base number systems
	myLCD.setCursor(0, 0);
	myLCD.print(47 , DEC); // print 47
	myLCD.setCursor(0, 9);
	myLCD.print(47 , HEX);  // print 2F
	myLCD.setCursor(0, 19);
	myLCD.print(47, BIN); // print 101111
	myLCD.setCursor(0, 30);
	myLCD.print(47 , OCT); // print 57

	screenReset();

	// print & Println with C++ string object
	std::string timeInfo = "12:45";
	myLCD.setCursor(5, 0);
	myLCD.println(timeInfo);
	myLCD.print(timeInfo);

	screenReset();
	
	// text textwrap with print 
	myLCD.setCursor(40, 0);
	myLCD.print(testStr4);
	
	screenReset();
}

void testDrawChar(void) {

	std::cout << "Test 111 draw ASCII font , drawchar, drawtext" << std::endl;
	char i;
	myLCD.setTextSize(1);
	myLCD.setTextColor(LCD_BLACK, LCD_WHITE);
	myLCD.setCursor(0, 0);
	myLCD.setTextWrap(true);

	// Print out the full ASCII font for default font
	for (i = 0; i < 83; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myLCD.print(i);
	}
	screenReset();

	myLCD.setCursor(0, 0);
	for (i = 84; i < 168; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myLCD.print(i);
	}
	screenReset();

	myLCD.setCursor(0, 0);
	for (i = 168; i < 250; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myLCD.print(i);
	}
	screenReset();

	// Test draw character
	myLCD.drawChar(40, 20 ,'A', 1, 0, 1);
	myLCD.drawChar(5, 30 ,'B', 0, 1, 1); // color inverted

	screenReset();

	// Test Draw Text
	char myString1[9] = {'1', '2', ':', '1', '3', ':', '2', '9'};
	myLCD.SetFontNum(LCDFontType_Tiny);
	myLCD.drawText(0,0, myString1, LCD_WHITE, LCD_BLACK,1);
	myLCD.SetFontNum(LCDFontType_Wide);
	myLCD.drawText(0,32, myString1, LCD_BLACK, LCD_WHITE,1);

	screenReset();

	// test textwrap and drawtext
	myLCD.drawText(60,24, myString1, LCD_WHITE, LCD_BLACK,1);
	screenReset();
}

void screenReset(void) {
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDdisplayClear();
}

void testFonts_7_8(void)
{
	std::cout << "Test 113-a test font 7 and 8 " << std::endl;
	char mytest[] = "23";
	myLCD.setTextColor(LCD_BLACK, LCD_WHITE);

	// Font 7
	myLCD.SetFontNum(LCDFontType_Bignum);
	myLCD.drawCharBigFont(0, 0, '7', LCD_WHITE, LCD_BLACK); // print 7 single character
	myLCD.drawTextBigFont(20, 0, mytest, LCD_BLACK, LCD_WHITE); // Print 23 inverted
	screenReset();

	// Font 7 with print method
	myLCD.setCursor(0, 0);
	myLCD.print(-8.3, 1); //print -8.3
	screenReset();

	myLCD.setCursor(0, 0);
	myLCD.print("12:43"); // print 12:43
	screenReset();
	
	// Font 8 
	myLCD.SetFontNum(LCDFontType_Mednum);
	myLCD.drawCharBigFont(0, 0, '8', LCD_WHITE, LCD_BLACK); // single character inverted
	myLCD.drawTextBigFont(20, 0, mytest, LCD_BLACK, LCD_WHITE); // print 23
	screenReset();

	// Font 8 with print method
	myLCD.setCursor(0, 0);
	myLCD.print(3.947, 2); // prints 3.95
	screenReset();

	myLCD.setCursor(0, 0);
	myLCD.print(-39.7,1);
	myLCD.setCursor(0, 16);
	myLCD.print("10/23");
	myLCD.setCursor(0, 32);
	myLCD.print("11:49");
	screenReset();
}

void testFonts_9_10(void)
{
	std::cout << "Test 113-b test font 9 and 10 " << std::endl;
	char mytest2[] = "BC";
	myLCD.setTextColor(LCD_BLACK, LCD_WHITE);

	// Font 9 
	myLCD.SetFontNum(LCDFontType_ArialRound);
	myLCD.drawCharBigFont(0, 0, 'A', LCD_WHITE, LCD_BLACK); // print A single character
	myLCD.drawTextBigFont(20, 0, mytest2, LCD_BLACK, LCD_WHITE); // Print BC inverted
	screenReset();

	// Font 9 with print method
	myLCD.setCursor(0, 0);
	myLCD.print(-8.3, 1); //print -8.3
	screenReset();

	myLCD.setCursor(0, 0);
	myLCD.print("12:43"); // print 12:43
	screenReset();

	// Font 10
	myLCD.SetFontNum(LCDFontType_ArialBold);
	myLCD.drawCharBigFont(0, 0, 'A', LCD_WHITE, LCD_BLACK); // single character A inverted
	myLCD.drawTextBigFont(20, 0, mytest2, LCD_BLACK, LCD_WHITE); // print BC
	screenReset();

	// Font 10 with print method
	myLCD.setCursor(0, 0);
	myLCD.print(3.947, 2); // prints 3.95
	screenReset();

	myLCD.setCursor(0, 0);
	myLCD.print(-39.7,1);
	myLCD.setCursor(0, 16);
	myLCD.print("01/79");
	myLCD.setCursor(0, 32);
	myLCD.print("23:01");
	screenReset();
}

// *************** EOF ****************
