Table of contents
---------------------------

  * [Overview](#overview)
  * [Installation](#installation)
  * [Test](#test)
  * [Software](#software)
  * [Hardware](#hardware)
  * [Output](#output)
  
Overview
--------------------------------------------
* Name: NOKIA_5110_RPI
* Description:

0. C++ Library for Nokia 5110 LCD, PCD8544 Driver. 84 x 48 pixels.
1. Dynamic install-able Raspberry Pi C++ library.
2. Invert colour, rotate, sleep, contrast bias control supported.
3. Eight fonts.
4. Graphics + print data class's included.
5. Bitmaps supported.
6. Hardware and Software SPI
7. Dependency: bcm2835 Library

* Author: Gavin Lyons
* Port of my PIC library at [github link.](https://github.com/gavinlyonsrepo/pic_18F47K42_projects)
* Developed on
	1. Raspberry PI 3 model b, 
	2. C++ complier g++ (Raspbian 8.3.0-6)
	3. Raspbian 10 buster  OS
	4. bcm2835 Library 1.68 (Dependency)

Installation
------------------------------

1. Make sure SPI bus is enabled on your raspberry PI

2. Install the dependency bcm2835 Library if not installed (at time of writing latest version is 1.68.)
	* The bcm2835 library  provides SPI bus, delays and GPIO control.
	* Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

3. Download the NOKIA_5110_RPI library
	* Open a Terminal in a folder where you want to download,build & test library
	* Run following command to download from github.

```sh
curl -sL https://github.com/gavinlyonsrepo/NOKIA_5110_RPI/archive/1.2.tar.gz | tar xz
```

4. Run "make" to run the makefile to install library, it will be
    installed to usr/lib and usr/include

```sh
cd NOKIA_5110_RPI-1.2
sudo make
```
Test
-----------------------------

1. Next step is to test LCD and installed library with the main.cpp test file.
Wire up your LCD. Next enter the example folder and run the makefile in THAT folder,
This second makefile builds the example file using the just installed library.
and creates a test exe file in "bin". 


```sh
cd example/
make
make run
```

2. There are 4 examples files to try out. 
To decide which one the makefile builds simply edit "SRC" variable at top of the makefile in examples folder.
in the "User SRC directory Option Section". Pick an example "SRC" directory path and ONE ONLY.
Comment out the rest and repeat: make and run bin/test.

| Filepath | File Function | 
| ---- | ---- | 
| src/HelloWorld | Basic use case | 
| src/HelloWorld_SWSPI  | Basic use case, software SPI | 
| src/TestRun | Test the library , Graphics, bitmap, text, fonts & functions | 
| src/SpeedTest | Frame rate per second test | 

Hardware
----------------------

The Nokia 5110 is a basic graphic LCD screen for lots of applications. 
GPIO function on RPI, 5 Nokia 5110 LCD lines SPI bus. Use any GPIO user likes for SW SPI.

| RPI pin(HW SPI) | Nokia 5110 LCD |
| ------ | ------ |
| SPI_CLK | LCD_CLK Pin 5 clock in |
| SPI_MOSI  | LCD_DIN Pin 4 data in |
| GPIO24 | LCD_DC Pin 3 data/command|
| SPI_CE0  | LCD_CE Pin 2 chip enable |
| GPIO25 | LCD_RST Pin 1 reset|

Connect Nokia 5110 VCC(pin 6)to 3.3V.
The user may have to adjust LCD contrast and bias settings,
to the screen at hand for optimal display.
A resistor or potentiometer can be connected between (Nokia LCD) 
GND(pin8) and LIGHT(pin7) to switch on /off backlight and adjust brightness. 

[Nokia 5110 LCD dataSheet ](https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf)

![PICTURE](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/NOKIA2.jpg)

Software
--------------------------

**SPI**

This library supports both Hardware SPI and software SPI.

The "define" -> LCD_HIGHFREQ_DELAY is a uS delay that 
can be used to slow down or speed up Software SPI.

Hardware SPI uses bcm2835 library.
The SPI settings are in OLEDSPIon function.
Default Speed is BCM2835_SPI_CLOCK_DIVIDER_64, 
6.25MHz on RPI3. This can be adjusted in code or you can pass 
the divider value in the begin method as a parameter. These values are
defined by enum bcm2835SPIClockDivider. For full list see
[link.](http://www.airspayce.com/mikem/bcm2835/group__constants.html#gaf2e0ca069b8caef24602a02e8a00884e)

User can also adjust which HW SPI chip enable pin the use(CE0 or CE1).
in parameters of begin method.

**Files**

In example folder:
The Main.cpp file contains tests showing library functions.
A bitmap data file contains data for bitmaps tests.

There are two makefiles

    1. Makefile at root directory builds and installs library at a system level.
    2. Makefile in example directory build example file to an executable.


**Fonts**

There are 8 fonts.
A print class is available to print out most passed data types.
The fonts 1-6 are byte high(at text size 1) scale-able fonts, columns of padding added by SW.
Font 7 & 8 are special fixed size large font but it is numbers only + ':' &'.' & '-' only


Font data table: 

| num | enum name | size xbyy |  ASCII range | Size in bytes |
| ------ | ------ | ------ | ------ |  ------ | 
| 1 | LCDFontType_Default | 5x8 | ASCII 0 - 0xFF, Full Extended  | 1275 |
| 2 | LCDFontType_Thick   | 7x8 |  ASCII  0x20 - 0x5A, no lowercase letters | 406 | 
| 3 | LCDFontType_SevenSeg  | 4x8 | ASCII  0x20 - 0x7A | 360 |
| 4 | LCDFontType_Wide | 8x8 |  ASCII 0x20 - 0x5A, no lowercase letters| 464 |
| 5 | LCDFontType_Tiny | 3x8 | ASCII  0x20 - 0x7E | 285 |
| 6 | LCDFontType_Homespun  | 7x8 | ASCII  0x20 - 0x7E |  658 |
| 7 | LCDFontType_Bignum | 16x32 | ASCII 0x30-0x3A ,Numbers + : . - only | 704 |
| 8 | LCDFontType_Mednum | 16x16 | ASCII 0x30-0x3A , Numbers + : . - only | 352 |

Output
------------------------

Example output.

![OUTPUT](https://github.com/gavinlyonsrepo/pic_18F47K42_projects/blob/master/images/nokiagraph1.jpg)
