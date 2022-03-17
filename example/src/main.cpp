/*
 * Project Name:  PCD8544 Nokia 5110 SPI LCD display Library
 * File: main.cpp
 * Description: library test file
 * Author: Gavin Lyons.
 * Created June 2021
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
 */

// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <iostream> // for std::cout

#include "NOKIA_5110_RPI.h" // PCD8544 controller driver
#include "Bitmap_data.h" // Data for bitmap tests

// *********** Test setup Defines **************
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
#define CONTRAST  0x30 // 0x30 to 0x3F possible values

// **************** GPIO ***************
#define  RST_LCD 25
#define DC_LCD 24
#ifndef LCD_SPI_HARDWARE
	#define SCLK_LCD 11
	#define SDIN_LCD 27
	#define CS_LCD 8
#endif

NOKIA_5110_RPI myLCD;

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
	testDrawChar(); //     11.  draw the  ASCII font default 0-255
					//			+ Custom character + drawChar().
	testFonts();    //      11b. test different fonts 1-5
	testTextModes(); //     12. Text display tests, test print functions and puts
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
	bcm2835_delay(TEST_DELAY1);
	std::cout << "LCD Start\r\n" ;
#ifndef LCD_SPI_HARDWARE
	myLCD.LCDBegin(RST_LCD, DC_LCD ,CS_LCD , SCLK_LCD, SDIN_LCD);
#else
	myLCD.LCDBegin(RST_LCD, DC_LCD,-1 , -1, -1);
#endif
	myLCD.LCDSetContrast(CONTRAST);
	myLCD.LCDdisplayClear();
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY1);
	myLCD.display_drawBitmapV2(0,0, customBitmap, 84, 48, LCD_BLACK); // Splashscreen
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY2);
	screenReset();
}


void EndTests(void)
{
#ifdef LCD_SPI_HARDWARE
	myLCD.LCDSPIoff(); //Stop the hardware SPI
#endif
	myLCD.LCDPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "LCD End\r\n";
}

void testFonts(void)
{
	char testStr1[]= "size1";
	char testStr2[]= "size2";
	char testStr3[]= "s3";
	
	char testStr4[]= "THICK 2082";
	char testStr5[]= "TH7";
	
	char testStr6[]= "Seven Seg 1273";
	char testStr7[]= "887";
	
	char testStr8[]= "WIDE 9337";
	char testStr9[]= "WIDE";
	
	char testStr10[]= "TINY 1837";
	char testStr11[]= "tiny font";
	
	// default font 1
	myLCD.display_SetFontNum(LCDFont_Default);
	myLCD.display_setTextSize(1);
	myLCD.display_setCursor(0, 0);
	myLCD.display_puts(testStr1);
	myLCD.display_setTextSize(2);
	myLCD.display_setCursor(0,9);
	myLCD.display_puts(testStr2);
	myLCD.display_setTextSize(3);
	myLCD.display_setCursor(0,25);
	myLCD.display_puts(testStr3);
	screenReset();

	// THICK font 2
	myLCD.display_setCursor(0, 0);
	myLCD.display_setTextSize(1);
	myLCD.display_SetFontNum(LCDFont_Thick);
	myLCD.display_puts(testStr4);
	myLCD.display_setTextSize(2);
	myLCD.display_setCursor(10, 20);
	myLCD.display_puts(testStr5);
	screenReset();

	// Seven seg font 3
	myLCD.display_SetFontNum(LCDFont_Seven_Seg);
	myLCD.display_setTextSize(1);
	myLCD.display_setCursor(0, 12);
	myLCD.display_puts(testStr6);
	myLCD.display_setTextSize(2);
	myLCD.display_setCursor(10, 20);
	myLCD.display_puts(testStr7);
	screenReset();

	// WIDE  font 4
	myLCD.display_SetFontNum(LCDFont_Wide);
	myLCD.display_setTextSize(1);
	myLCD.display_setCursor(0, 0);
	myLCD.display_puts(testStr8);
	myLCD.display_setTextSize(2);
	myLCD.display_setCursor(10, 20);
	myLCD.display_puts(testStr9);
	screenReset();


	//tiny font 5
	myLCD.display_SetFontNum(LCDFont_Tiny);
	myLCD.display_setTextSize(1);
	myLCD.display_setCursor(0, 0);
	myLCD.display_puts(testStr10);
	myLCD.display_setTextSize(2);
	myLCD.display_setCursor(10, 20);
	myLCD.display_puts(testStr11);
	screenReset();

	myLCD.display_setTextSize(1);
	myLCD.display_SetFontNum(LCDFont_Default);
	myLCD.display_setCursor(0, 0);
}



void testSleepMode(void) {
	
	char testStr1[]= "Sleep 5 secs!\r\n";
	char testStr2[]= "Awake!\r\n";
	
	myLCD.LCDdisplayClear();
	myLCD.display_setCursor(0, 0);
	myLCD.display_setTextSize(1);
	myLCD.display_puts(testStr1);
	myLCD.LCDdisplayUpdate();
	
	bcm2835_delay(TEST_DELAY2);
	myLCD.LCDenableSleep();
	bcm2835_delay(TEST_DELAY5);
	myLCD.LCDdisableSleep();
	
	myLCD.display_puts(testStr2);
	myLCD.LCDdisplayUpdate();
	myLCD.LCDSetContrast(CONTRAST); // reset contrast after sleep
	bcm2835_delay(TEST_DELAY2);
	screenReset();
}


void testTextModes(void) {

	char testStr1[]= "Hello world!\r\n";
	char testStr2[]= "0xABCD";
	char testStr3[]= "21:34\r\n";
	
	// text display tests
	myLCD.display_setTextSize(1);
	myLCD.display_setTextColor(LCD_WHITE, LCD_BLACK); // 'inverted' text
	myLCD.display_setCursor(0, 0);
	myLCD.display_puts(testStr1);
	myLCD.print(3.141592, 4); // prints 3.1416
	myLCD.display_setCursor(0, 24);
	myLCD.display_setTextSize(2);
	myLCD.display_setTextColor(LCD_BLACK, LCD_WHITE);
	myLCD.print(testStr2);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	
	myLCD.LCDdisplayClear();
	myLCD.display_setCursor(0, 0);
	myLCD.display_setTextSize(1);
	myLCD.display_SetFontNum(LCDFont_Wide);
	myLCD.display_puts(testStr3);
	myLCD.print(-14790);
	myLCD.display_setCursor(0, 26);
	myLCD.print(83.6);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
}


void testRotate(void) {
	
	char testStr1[]= "Rotate 90\r\n";
	char testStr2[]= "Rotate 180\r\n";
	char testStr3[]= "Rotate 270\r\n";
	
	// rotation example 
	myLCD.LCDdisplayClear();
	myLCD.display_setTextSize(1);
	myLCD.display_SetFontNum(LCDFont_Tiny); //tiny
	myLCD.display_setTextColor(LCD_BLACK, LCD_WHITE);
	myLCD.display_setCursor(0, 0);
	
	myLCD.LCDsetRotation(LCD_Degrees_90); // rotate 90 degrees counter clockwise,
	myLCD.LCDdisplayClear();
	myLCD.display_puts(testStr1);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	
	myLCD.LCDsetRotation(LCD_Degrees_180); // rotate 180 degrees counter clockwise,
	myLCD.LCDdisplayClear();
	myLCD.display_puts(testStr2);
	myLCD.LCDdisplayUpdate();
	bcm2835_delay(TEST_DELAY5);
	
	myLCD.LCDsetRotation(LCD_Degrees_270); // rotate 180 degrees counter clockwise,
	myLCD.LCDdisplayClear();
	myLCD.display_puts(testStr3);
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
	myLCD.display_setTextSize(1);
	myLCD.display_setTextColor(LCD_BLACK, LCD_WHITE);
	myLCD.display_setCursor(0, 0);
	myLCD.display_setTextWrap(true);
	for (i = 0; i < 83; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myLCD.display_putc(i);
	}
	screenReset();
	myLCD.display_setCursor(0, 0);
	for (i = 84; i < 168; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myLCD.display_putc(i);
	}
	screenReset();
	myLCD.display_setCursor(0, 0);
	for (i = 168; i < 250; i++)
	{
		if (i == '\n' || i == '\r') continue;
		myLCD.display_putc(i);
	}
	screenReset();
	bcm2835_delay(TEST_DELAY2);
	myLCD.display_setCursor(0, 0);
	myLCD.display_customChar(mycustomchar); // '|||'
	myLCD.display_setCursor(40, 20);
	myLCD.display_drawChar(40, 20 ,'A', 1, 0, 1);
	myLCD.display_drawChar(5, 30 ,'B', 0, 1, 1); // color inverted 
	screenReset();
}

void testdrawcircle(void) {

	myLCD.display_drawCircle(20, 20, 10, LCD_BLACK);
	screenReset();
}

void testfillcircle(void)
{
	myLCD.display_fillCircle(60, 20, 10, LCD_BLACK);
	screenReset();
}

void testFillRect(void) {

	myLCD.display_fillRect(0, 20, 10, 30, LCD_BLACK);
	screenReset();
}

void testDrawTriangle(void) {
	myLCD.display_drawTriangle(30, 20, 50, 5, 80, 20, LCD_BLACK);
	screenReset();
}

void testFillTriangle(void) {

	myLCD.display_fillTriangle(10, 20, 40, 5, 80, 20, LCD_BLACK);
	screenReset();
}

void testDrawRoundRect(void) {
	myLCD.display_drawRoundRect(20, 20, 15 , 15, 5, LCD_BLACK);
	screenReset();
}

void testFillRoundRect(void) {
	myLCD.display_fillRoundRect(60, 20, 15, 15, 5, LCD_BLACK);
	screenReset();
}

void testDrawRect(void) {
	myLCD.display_drawRect(20, 15, 20, 15, LCD_BLACK);
	screenReset();
}

void testDrawLine(void) {

	myLCD.display_drawLine(0, 0, 40, 20, LCD_BLACK);
	myLCD.display_drawHLine(50, 20, 10 , LCD_BLACK); 
	myLCD.display_drawVLine(70, 20 ,10, LCD_BLACK); 
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
	myLCD.display_drawBitmapV2(30, 16, smallImage, 20, 20, LCD_BLACK);
	myLCD.LCDdisplayUpdate();

	// Invert the display
	myLCD.LCDinvertDisplay(true);
	bcm2835_delay(TEST_DELAY2);
	myLCD.LCDinvertDisplay(false);
	bcm2835_delay(TEST_DELAY2);
}
// *************** EOF ****************
