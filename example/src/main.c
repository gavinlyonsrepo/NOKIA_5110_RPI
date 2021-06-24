/*
 * Project Name:  PCD8544 Nokia 5110 SPI LCD display Library
 * File: main.c
 * Description: library test file
 * Author: Gavin Lyons.
 * Created June 2021
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
 */

// ************ libraries **************
#include <bcm2835.h> // for SPI GPIO and delays.
#include <stdio.h>  // for printf

#include "NOKIA_5110_RPI.h" // PCD8544 controller driver
#include "Bitmap_data.h" // for bitmap tests

// *********** Defines **************
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

// ************ Function Headers ****************
void Setup(void);
void screenreset(void);
void EndTests(void);
void textdrawpixel(void);
void testfillscreen(void);
void testsleepmode(void);
void testdrawcircle(void);
void testfillcircle(void);
void testdrawline(void);
void testdrawchar(void);
void testfillrect(void);
void testdrawtriangle(void);
void testfilltriangle(void);
void testdrawroundrect(void);
void testfillroundrect(void);
void testdrawrect(void);
void testRotate(void);
void testTextModes(void);
void testFonts(void);


// ************  MAIN ***************

int main(void)
{
	if(!bcm2835_init())
	{
		printf("Error 1201 : Problem with init bcm2835 library\r\n");
		return -1;
	}
	Setup(); // Includes "splash screen" full size bitmap test.

	while (1)
	{
		textdrawpixel(); //    1. Draw Pixels
		testfillscreen(); //   1b. Fill screen
		testsleepmode(); //    1c. sleep mode
		testdrawline(); //     2. Draw lines
		testdrawrect(); //     3. Draw rectangles
		testfillrect(); //     4. Draw filled rectangles
		testdrawcircle(); //   5.  Draw circles
		testfillcircle(); //   6. Draw a filled circle
		testdrawroundrect(); // 7. Draw a rounded rectangle
		testfillroundrect(); // 8. Fill a rounded rectangle
		testdrawtriangle(); //  9. Draw a triangle
		testfilltriangle(); // 10. Fill a triangle
		testdrawchar(); //     11. draw the  ASCII font default 0-255
		testFonts();    //     11b. test different fonts 1-5
		testTextModes(); //     12. Text display tests
		testRotate(); //        13. Rotation example

		//  14. Miniature bitmap display
		display_clear();
		display_drawBitmapV2(30, 16, smallImage, 20, 20, BLACK);
		display();

		//  15. Invert the display
		display_invert(true);
		bcm2835_delay(2000);
		display_invert(false);
		bcm2835_delay(2000);

		break;
	}

	EndTests();
	return 0;
}
// ********* End OF MAIN ********


// ******** Function Space *************

// Initialize the device

void Setup(void)
{
	bcm2835_delay(TEST_DELAY1);
	printf("LCD Start\r\n");
#ifndef LCD_SPI_HARDWARE
	LCD_begin(RST_LCD, DC_LCD ,CS_LCD , SCLK_LCD, SDIN_LCD);
#else
	LCD_begin(RST_LCD, DC_LCD,-1 , -1, -1);
#endif
	display_setContrast(CONTRAST);
	display_clear();
	display();
	bcm2835_delay(TEST_DELAY1);
	display_drawBitmapV2(0,0, customBitmap, 84, 48, BLACK); // Splashscreen
	display();
	bcm2835_delay(TEST_DELAY5);
	screenreset();
}


void EndTests(void)
{
#ifdef LCD_SPI_HARDWARE
	LCD_SPIoff(); //Stop the hardware SPI
#endif
	LCD_Power_Down(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	printf("LCD End\r\n");
}

void testFonts(void)
{
	// default font 1
	LCDFontNum(1);
	display_setTextSize(1);
	display_setCursor(0, 0);
	display_puts("Default");
	display_setTextSize(2);
	display_setCursor(0,9);
	display_puts("size 2");
	display_setTextSize(3);
	display_setCursor(0,25);
	display_puts("size3");
	screenreset();

	// THICK font 2
	display_setCursor(0, 0);
	display_setTextSize(1);
	LCDFontNum(2);
	display_puts("THICK 2082");
	display_setTextSize(2);
	display_setCursor(10, 20);
	display_puts("TH7");
	screenreset();

	// Seven seg font 3
	LCDFontNum(3);
	display_setTextSize(1);
	display_setCursor(0, 12);
	display_puts("Seven Seg 1273");
	display_setTextSize(2);
	display_setCursor(10, 20);
	display_puts("887");
	screenreset();

	// WIDE  font 4
	LCDFontNum(4);
	display_setTextSize(1);
	display_setCursor(0, 0);
	display_puts("WIDE 9337");
	display_setTextSize(2);
	display_setCursor(10, 20);
	display_puts("WIDE");
	screenreset();


	//tiny font 5
	LCDFontNum(5);
	display_setTextSize(1);
	display_setCursor(0, 0);
	display_puts("TINY 1837");
	display_setTextSize(2);
	display_setCursor(10, 20);
	display_puts("tiny font");
	screenreset();

	display_setTextSize(1);
	LCDFontNum(1);
	display_setCursor(0, 0);
}



void testsleepmode(void) {
	display_clear();
	display_setCursor(0, 0);
	display_setTextSize(1);
	display_puts("Sleep Mode!\r\n");
	display();
	bcm2835_delay(TEST_DELAY5);
	LCDenableSleep();
	bcm2835_delay(TEST_DELAY5);
	LCDdisableSleep();
	display_setContrast(CONTRAST); // reset contrast after sleep
	screenreset();
}

void testTextModes(void) {
	// text display tests
	display_setTextSize(1);
	display_setTextColor(BLACK, WHITE);
	display_setCursor(0, 0);
	display_puts("Hello world!\r\n");
	display_setTextColor(WHITE, BLACK); // 'inverted' text
	display_printf("%4.2f\r\n", 3.141592);
	display_setTextSize(2);
	display_setTextColor(BLACK, WHITE);
	display_printf("0x%X\r\n", 0xABCD);
	display();
	bcm2835_delay(TEST_DELAY5);
	display_clear();
	display_setCursor(0, 0);
	display_setTextSize(3);
	display_puts("912\r\n");
	display_setTextSize(1);
	display_printf("%d\r\n", 14790);
	display();
	bcm2835_delay(TEST_DELAY5);
}

void testRotate(void) {
	// rotation example
	display_clear();
	display_setRotation(1); // rotate 90 degrees counter clockwise,
	// can also use values of 2 and 3 to go further.
	display_setTextSize(1);
	display_setTextColor(BLACK, WHITE);
	display_setCursor(0, 0);
	display_puts("Rotation\r\n");
	display_setTextSize(2);
	display_puts("Example!\r\n");
	display();
	bcm2835_delay(TEST_DELAY2);

	// revert back to no rotation
	display_setRotation(0);
}

void textdrawpixel(void) {
	display_drawPixel(2, 5, BLACK);
	display_drawPixel(10, 10, BLACK);
	display_drawPixel(20, 20, BLACK);
	display_drawPixel(30, 30, BLACK);
	display_drawPixel(32, 35, BLACK);
	display();
	bcm2835_delay(TEST_DELAY2);
	display_clear();
}


void testdrawchar(void) {

	char i;
	const uint8_t mycustomchar[] = {0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00};
	display_setTextSize(1);
	display_setTextColor(BLACK, WHITE);
	display_setCursor(0, 0);
	display_setTextWrap(true);
	for (i = 0; i < 83; i++)
	{
		if (i == '\n' || i == '\r') continue;
		display_putc(i);
	}
	screenreset();
	display_setCursor(0, 0);
	for (i = 84; i < 168; i++)
	{
		if (i == '\n' || i == '\r') continue;
		display_putc(i);
	}
	screenreset();
	display_setCursor(0, 0);
	for (i = 168; i < 250; i++)
	{
		if (i == '\n' || i == '\r') continue;
		display_putc(i);
	}
	screenreset();
	bcm2835_delay(TEST_DELAY2);
	display_setCursor(0, 0);
	display_customChar(mycustomchar);
	display_setCursor(40, 20);
	display_drawChar(40,20,'A',1,0,1);
	screenreset();
}

void testdrawcircle(void) {

	display_drawCircle(20, 20, 10, BLACK);
	screenreset();
}

void testfillcircle(void)
{
	display_fillCircle(20, 20, 10, BLACK);
	screenreset();
}

void testfillrect(void) {

	display_fillRect(20, 20, 10, 30, BLACK);
	screenreset();
}

void testdrawtriangle(void) {

	display_drawTriangle(30, 20, 50, 5, 80, 20, BLACK);
	screenreset();
}

void testfilltriangle(void) {

	display_fillTriangle(30, 20, 50, 5, 80, 20, BLACK);
	screenreset();
}

void testdrawroundrect(void) {
	display_drawRoundRect(20, 20, 15 , 15, 5, BLACK);
	screenreset();
}

void testfillroundrect(void) {
	display_fillRoundRect(20, 20, 15, 15, 5, BLACK);
	screenreset();
}

void testdrawrect(void) {
	display_drawRect(20, 15, 20, 15, BLACK);
	screenreset();
}

void testdrawline(void) {

	display_drawLine(0, 0, 40, 20, BLACK);
	screenreset();
}

void testfillscreen(void){
	fillScreen();
	screenreset();
}

void screenreset(void) {
	display();
	bcm2835_delay(TEST_DELAY5);
	display_clear();
}
// *************** EOF ****************
