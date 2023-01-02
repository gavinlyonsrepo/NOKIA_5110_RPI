
/*
 * Project Name:  PCD8544 Nokia 5110 SPI LCD display Library
 * File: main.cpp
 * Description: library test file, carries out FPS test , HW SPI
 * Author: Gavin Lyons.
 * Created Dec 2022 v 1.2.0
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
 */

// Test results RPI3, Frame rate per Second, FPS.
// FPS 850, BCM2835_SPI_CLOCK_DIVIDER_64  SPICLK_FREQ = 64, 6.250Mhz
// FPS 500, BCM2835_SPI_CLOCK_DIVIDER_128 SPICLK_FREQ = 128, 3.125MHz
// FPS 150, BCM2835_SPI_CLOCK_DIVIDER_512 SPICLK_FREQ = 512, 781.25kHz 
// FPS 60,  Software SPI.

// ************ libraries **************
#include <bcm2835.h> // for SPI, GPIO and delays. airspayce.com/mikem/bcm2835/index.html
#include <iostream> // for std::cout
#include <time.h>
#include "NOKIA_5110_RPI.hpp" // PCD8544 controller driver
#include "Bitmap_data.hpp" // Data for bitmap tests

// *********** Test setup  **************
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
uint16_t count = 0;
bool colour = 1;
uint64_t  previousCounter = 0;

// **************** GPIO ***************
#define RST_LCD 25
#define DC_LCD 24

#define inverse  false // set to true to invert display pixel color
#define contrast 0xB2 // default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark/dim
#define bias 0x13 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14
const uint32_t SPICLK_FREQ = 64; // Spi clock divider, see bcm2835SPIClockDivider enum bcm2835
const uint8_t SPI_CE_PIN = 0; // which HW SPI chip enable pin to use,  0 or 1

NOKIA_5110_RPI myLCD(RST_LCD, DC_LCD);

// =============== Function prototype ================
uint8_t  setup(void);
void myLoop(void);
void display_buffer(long , int );
void EndTests(void);
static uint64_t counter( void );

// ======================= Main ===================
int main(int argc, char **argv)
{

	if (!setup()){return -1;}
	myLoop();
	EndTests();
	return 0;
}
// ======================= End of main  ===================

void EndTests()
{
	myLCD.LCDSPIoff(); //Stop the hardware SPI
	myLCD.LCDPowerDown(); //Switch off display
	bcm2835_close(); // Close the library
	std::cout << "LCD End\r\n";
}

uint8_t setup()
{
	if(!bcm2835_init())
	{
		std::cout<< "Error 1201 : Problem with init bcm2835 library\r\n";
		return -1;
	}
	bcm2835_delay(50);
	std::cout << "LCD Start\r\n" ;
	myLCD.LCDBegin(inverse, contrast, bias, SPICLK_FREQ, SPI_CE_PIN);
	bcm2835_delay(250);
	myLCD.setTextSize(1);
	return 1;
}

void myLoop() {

		while (count < 10000)
		{
			static long framerate = 0;
			display_buffer(framerate, count);
			framerate++;
			count++;
		}

}

void display_buffer(long currentFramerate, int count)
{
	// Values to count frame rate per second
	static long lastFramerate = 0;
	static uint16_t fps;
	uint64_t currentCounter = counter();
	if(currentCounter - previousCounter >= 1000000000)
	{
		//(Rolls over every 1 second)
		fps = currentFramerate - lastFramerate;
		lastFramerate = currentFramerate ;
		previousCounter = currentCounter;
		colour = !colour;
	}
	
	// ** Code to speed test here **
	myLCD.LCDdisplayClear();
	myLCD.setCursor(0, 0);
	myLCD.print("Nokia");

	myLCD.setCursor(0, 10);
	myLCD.print("G Lyons");

	myLCD.setCursor(0, 20);
	myLCD.print(count);

	myLCD.setCursor(0, 30);
	myLCD.print(fps);
	myLCD.print(" fps");
	myLCD.setCursor(0, 40);
	myLCD.print("V 1.2.0");

	myLCD.fillRect(60, 1, 20, 20, colour);
	myLCD.fillCircle(60, 35, 10, !colour);

	myLCD.LCDdisplayUpdate();
	// ** **
}

// This func returns nano-seconds as a 64-bit unsigned number, 
// monotonically increasing, probably since system boot.
// The actual resolution looks like microseconds. returns nanoseconds
static uint64_t counter( void )
{
  struct timespec now;
  clock_gettime( CLOCK_MONOTONIC, &now );
  return  ((uint64_t)now.tv_sec * 1000000000U) + (uint64_t)now.tv_nsec; 
}
