/*
 * Project Name:  PCD8544 Nokia 5110 SPI LCD display Library
 * File: main.cpp
 * Description: library test file, carries out series of tests , HW SPI
 * Author: Gavin Lyons.
 * Created June 2021
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
 */

// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <iostream> // for std::cout

#include "NOKIA_5110_RPI.hpp" // PCD8544 controller driver
#include "Bitmap_data.hpp" // Data for bitmap tests

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

NOKIA_5110_RPI myLCD(RST_LCD, DC_LCD);

// ************ Function Headers ****************
void Setup(void);
void screenReset(void);

void textDrawPixel(void);
void testFillScreen(void);
void testSleepMode(void);
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
void testRotate(void);
void testTextModes(void);
void testFonts(void);
void testFontsNum(void);
void testBitmapInvert(void);

void EndTests(void);

// ************  MAIN ***************

int main(void)
{
	if(!bcm2835_init())
	{
		std::cout<< "Error 1201 : Problem with init bcm2835 library\r\n";
		return -1;
	}
	
	Setup(); // 0. Includes "splash screen" full size bitmap test, test0 

	textDrawPixel(); //    1. Draw Pixels
	testFillScreen(); //   1b. Fill screen
	testSleepMode(); //    1c. sleep mode
	
	testDrawLine(); //     2. Draw lines
	testDrawRect(); //     3. Draw rectangle
	testFillRect(); //     4. Draw filled rectangle
	testdrawcircle(); //   5.  Draw circles
	testfillcircle(); //   6. Draw a filled circle
	testDrawRoundRect(); // 7. Draw a rounded rectangle
	testFillRoundRect(); // 8. Fill a rounded rectangle
	testDrawTriangle(); //  9. Draw a triangle
	testFillTriangle(); // 10. Fill a triangle
	
	testDrawChar(); //     11.  draw the ASCII font default 0-255
					//		+ Custom character + drawChar() + drawText()
	testFonts();    //      11b. test different fonts 1-6
	testFontsNum(); //      11c. test font 7-8 numerical fonts.
	testTextModes(); //     12. Text display tests, test print functions
	
	testRotate(); //        13. Rotation example
	testBitmapInvert(); // 14. Miniature bitmap display
					   //  15. Invert the display
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
	myLCD.LCDdisplayClear();
	bcm2835_delay(250);
	myLCD.drawBitmap(0,0, customBitmap, 84, 48, LCD_BLACK); // Splashscreen
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY2);
	screenReset();
}


void EndTests(void)
{

	myLCD.LCDSPIoff(); //Stop the hardware SPI
	myLCD.LCDPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "LCD End\r\n";
}

void testFonts(void)
{
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



void testSleepMode(void) {
	
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


void testTextModes(void) {

	char testStr1[]= "Hello World!\r\n";
	char testStr2[]= "0xABCD";
	char testStr3[]= "20:37\r\n";
	
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
	
	myLCD.setCursor(0, 0);
	myLCD.setTextSize(1);
	myLCD.SetFontNum(LCDFontType_Wide);
	myLCD.print(testStr3);
	myLCD.print(-14790);
	myLCD.setCursor(0, 26);
	myLCD.print(83.6);
	
	screenReset();
	
	myLCD.setCursor(0, 0);
	myLCD.print(47 , DEC); // print 47 
	myLCD.setCursor(0, 9);
	myLCD.print(47 , HEX);  // print 2F
	myLCD.setCursor(0, 19);
	myLCD.print(47, BIN); // print 101111
	myLCD.setCursor(0, 30);
	myLCD.print(47 , OCT); // print 57
	
	screenReset();
}


void testRotate(void) {
	
	char testStr1[]= "Rotate 90\r\n";
	char testStr2[]= "Rotate 180\r\n";
	char testStr3[]= "Rotate 270\r\n";
	
	// rotation example 
	myLCD.LCDdisplayClear();
	myLCD.setTextSize(1);
	myLCD.SetFontNum(LCDFontType_Tiny); //tiny
	myLCD.setTextColor(LCD_BLACK, LCD_WHITE);
	myLCD.setCursor(0, 0);
	
	myLCD.LCDsetRotation(LCD_Degrees_90); // rotate 90 degrees counter clockwise,
	myLCD.LCDdisplayClear();
	myLCD.print(testStr1);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	
	myLCD.LCDsetRotation(LCD_Degrees_180); // rotate 180 degrees counter clockwise,
	myLCD.LCDdisplayClear();
	myLCD.print(testStr2);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	
	myLCD.LCDsetRotation(LCD_Degrees_270); // rotate 180 degrees counter clockwise,
	myLCD.LCDdisplayClear();
	myLCD.print(testStr3);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	
	// revert back to no rotation
	myLCD.LCDsetRotation(LCD_Degrees_0);
}

void textDrawPixel(void) {
	myLCD.LCDDrawPixel(2, 5, LCD_BLACK);
	myLCD.LCDDrawPixel(10, 10, LCD_BLACK);
	myLCD.LCDDrawPixel(20, 20, LCD_BLACK);
	myLCD.LCDDrawPixel(30, 30, LCD_BLACK);
	myLCD.LCDDrawPixel(32, 35, LCD_BLACK);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY2);
	myLCD.LCDdisplayClear();
}


void testDrawChar(void) {

	char i;
	const uint8_t mycustomchar[] = {0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00};
	myLCD.setTextSize(1);
	myLCD.setTextColor(LCD_BLACK, LCD_WHITE);
	myLCD.setCursor(0, 0);
	myLCD.setTextWrap(true);
	
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

	myLCD.setCursor(0, 0);
	myLCD.customChar(mycustomchar); // '|||'
	myLCD.setCursor(40, 20);
	myLCD.drawChar(40, 20 ,'A', 1, 0, 1);
	myLCD.drawChar(5, 30 ,'B', 0, 1, 1); // color inverted 
	
	screenReset();
	
	char myString1[9] = {'1', '2', ':', '1', '3', ':', '2', '9'};
	myLCD.SetFontNum(LCDFontType_Tiny);
	myLCD.drawText(0,0, myString1, LCD_WHITE, LCD_BLACK,1);
	myLCD.SetFontNum(LCDFontType_Wide);
	myLCD.drawText(0,32, myString1, LCD_BLACK, LCD_WHITE,1);
	
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

	myLCD.drawLine(0, 0, 40, 20, LCD_BLACK);
	myLCD.drawHLine(50, 20, 10 , LCD_BLACK); 
	myLCD.drawVLine(70, 20 ,10, LCD_BLACK); 
	screenReset();
}

void testFillScreen(void){
	myLCD.LCDfillScreen();
	screenReset();
}

void screenReset(void) {
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDdisplayClear();
}

void testBitmapInvert(void)
{
	// Miniature bitmap display
	myLCD.LCDdisplayClear();
	myLCD.drawBitmap(5, 5, SignalIcon, 16, 8, LCD_BLACK);
	myLCD.drawBitmap(30, 5, MsgIcon, 16, 8, LCD_BLACK);
	myLCD.drawBitmap(60, 5, BatIcon, 16, 8, LCD_BLACK);
	myLCD.LCDdisplayUpdate();

	// Invert the display
	myLCD.LCDinvertDisplay(true);
	bcm2835_delay(TEST_DELAY2);
	myLCD.LCDinvertDisplay(false);
	bcm2835_delay(TEST_DELAY2);
}

void testFontsNum(void)
{
	char mytest[] = "23";
	
	// Font 7 723 
	myLCD.SetFontNum(LCDFontType_Bignum);
	myLCD.drawCharNumFont(0, 0, '7', LCD_WHITE, LCD_BLACK); // single character
	myLCD.drawTextNumFont(20, 0, mytest, LCD_WHITE, LCD_BLACK);
	screenReset();
	
	// Font 8 823
	myLCD.SetFontNum(LCDFontType_Mednum);
	myLCD.drawCharNumFont(0, 0, '8', LCD_WHITE, LCD_BLACK); // single character
	myLCD.drawTextNumFont(20, 0, mytest, LCD_BLACK, LCD_WHITE);
	screenReset();
	
	// Font 7-8 with print
	myLCD.setTextColor(LCD_BLACK, LCD_WHITE);
	myLCD.SetFontNum(LCDFontType_Bignum);
	myLCD.setCursor(0, 0);
	myLCD.print(8.3, 1);
	screenReset();
	
	myLCD.SetFontNum(LCDFontType_Mednum);
	myLCD.setCursor(0, 0);
	myLCD.print(3.947, 2); // prints 3.95
	screenReset();
	
	myLCD.SetFontNum(LCDFontType_Mednum);
	myLCD.setCursor(0, 0);
	myLCD.print(-39); 
	screenReset();
}
// *************** EOF ****************
