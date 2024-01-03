# NOKIA_5110_RPI

## Table of contents

  * [Overview](#overview)
  * [Installation](#installation)
  * [Test](#test)
  * [Hardware](#hardware)
  * [Software](#software)
 	* [API](#api)
 	* [SPI](#spi)
 	* [File system](#file-system)
 	* [Fonts](#fonts)
	* [Bitmap](#bitmap)

  * [Output](#output)

## Overview

* Name: NOKIA_5110_RPI
* Description:

0. C++ Library for Nokia 5110 LCD, PCD8544 Driver. 84 x 48 pixels.
1. Dynamic install-able Raspberry Pi C++ library.
2. Invert colour, rotate, sleep, contrast bias control supported.
3. 10 fonts.
4. Graphics class included.
5. Polymorphic print class included.
6. Bitmaps supported.
7. Hardware and Software SPI
8. Dependency: bcm2835 Library

* Author: Gavin Lyons
* Port of my PIC library at [github link.](https://github.com/gavinlyonsrepo/pic_18F47K42_projects)
* Development Tool chain. 
	1. Raspberry PI 3 model b
	2. C++, g++ (Debian 12.2.0) 
	3. Raspbian , Debian 12 bookworm OS, , 64 bit.
	4. kernel : aarch64 Linux 6.1.0-rpi7-rpi-v8
	5. [bcm2835 Library 1.73 dependency](http://www.airspayce.com/mikem/bcm2835/). Provides low level SPI bus, delays and GPIO control.

## Installation

1. Install the dependency bcm2835 Library if not installed (at time of writing latest version is 1.73)
	* The bcm2835 library  provides SPI bus, delays and GPIO control.
	* Install the C libraries of bcm2835, [Installation instructions here](http://www.airspayce.com/mikem/bcm2835/)

2. Download the NOKIA_5110_RPI library
	* Open a Terminal in a folder where you want to download,build & test library
	* Run following command to download from github.

```sh
curl -sL https://github.com/gavinlyonsrepo/NOKIA_5110_RPI/archive/1.3.tar.gz | tar xz
```

3. Run "make" to run the makefile to install library, it will be
    installed to usr/lib and usr/include

```sh
cd NOKIA_5110_RPI-1.3
make
sudo make install
```

## Test

1. Next step is to test LCD and installed library with the main.cpp test file.
Wire up your LCD. Next enter the example folder and run the makefile in THAT folder,
This second makefile builds the example file using the just installed library.
and creates a test exe file in "example/bin". Note : make run uses sudo, bcm2835 library requires sudo.


```sh
cd example/
make
make run
```

2. There are 7 examples files to try out.
To decide which one the makefile builds simply edit "SRC" variable at top of the makefile in examples folder.
in the "User SRC directory Option Section". Pick an example "SRC" directory path and ONE ONLY.
Comment out the rest and repeat: make and run bin/test.

| Filepath | File Function | SPI |
| ---- | ---- | ---- |
| src/HelloWorld | Basic use case | HW |
| src/HelloWorld_SWSPI | Basic use case SW SPI | SW |
| src/TextTests | Text & fonts | HW |
| src/GraphicTests | Graphics | HW |
| src/BitmapTests | Bitmaps | HW |
| src/FunctionTests | Function testing | HW |
| src/SpeedTest | Frame rate per second test | HW |

## Hardware

The Nokia 5110 is a basic graphic LCD screen for lots of applications.
GPIO function on RPI, 5 Nokia 5110 LCD lines SPI bus. Use any GPIO user likes for Software SPI.
For Hardware the user can use any GPIO for reset and data/command line

Example file set up ::

| RPI pin(HW SPI) | RPI pin(SW SPI)| Nokia 5110 LCD |
| ------ | ------ | ------ |
| SPI_CLK | GPIO22 | LCD_CLK Pin 5 clock in |
| SPI_MOSI | GPIO27 | LCD_DIN Pin 4 data in |
| GPIO24 | GPIO24 | LCD_DC Pin 3 data/command|
| SPI_CE0 or SPI_CE1 | GPIO 8 | LCD_CE Pin 2 chip enable |
| GPIO25 | GPIO25 | LCD_RST Pin 1 reset|

Connect Nokia 5110 VCC(pin 6)to 3.3V.
The user may have to adjust LCD contrast and bias settings,
to the screen at hand for optimal display.
A resistor or potentiometer can be connected between (Nokia LCD)
GND(pin8) and LIGHT(pin7) to switch on /off backlight and adjust brightness.

[Nokia 5110 LCD dataSheet ](https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf)

![PICTURE](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/NOKIA2.jpg)

## Software

### API

The Software is commented mostly for"doxygen" and if users uses "doxygen" software
an API can be generated. A Doxyfile is in doc sub folder in repository.

### SPI

This library supports both Hardware SPI and software SPI.
The constructor of the main class is overloaded. 
Which SPI is started depends on which constructor called by user.
Hardware SPI uses bcm2835 library spi module.
The SPI settings are in LCDSPIHWSettings function.
If there are multiple SPI devices on Bus(with different settings) the user can call this method 
to refresh settings before a tranche of LCD commands.
Default Speed is BCM2835_SPI_CLOCK_DIVIDER_64,
6.25MHz on RPI3. This can be adjusted in code or you can pass
the divider value in the contructor as an argument. These values are
defined by enum bcm2835SPIClockDivider. For full list see

![bcm2](https://github.com/gavinlyonsrepo/NOKIA_5110_RPI/blob/main/extra/images/bcm2.png)

User can also adjust which HW SPI chip enable pin they use(CE0 or CE1).
in constructor arguments. This device uses bit order MSB First and SPI Mode 0.

Measured SPI speed results Version 1.3 are in the SpeedTest example file header comment block.
The parameter LCDHardwareSPI is a GPIO uS delay that
can be used to slow down or speed up Software SPI.
By default it is set to 2uS. It may have to be adjusted depending on device/CPU used.

### File system

There are two makefiles

    1. Makefile at root directory builds and installs library at a system level.
    2. Makefile in example directory build example file to an executable.


### Fonts

Font data table:

| num | enum name | Char size XbyY | ASCII range | Size bytes | Scale-able |
| ------ | ------ | ------ | ------ |  ------ | ----- |
| 1 | $_Default | 5x8 |0-0xFF, Full Extended|1275 |Y|
| 2 | $_Thick   | 7x8 |0x20-0x5A, no lowercase letters |406|Y|
| 3 | $_SevenSeg  | 4x8 |0x20-0x7A |360|Y|
| 4 | $_Wide | 8x8 |0x20-0x5A, no lowercase letters|464|Y|
| 5 | $_Tiny | 3x8 |0x20-0x7E |285|Y|
| 6 | $_Homespun  | 7x8 |0x20-0x7E |658|Y|
| 7 | $_Bignum | 16x32 |0x2D-0x3A ,0-10 - . / : |896|N|
| 8 | $_Mednum | 16x16 |0x2D-0x3A ,0-10 - . / :|448|N|
| 9 | $_ArialRound| 16x24 | 0x20-0x7E |4608|N|
| 10 | $_ArialBold | 16x16 |0x20-0x7E |3072|N|

1. $ = OLEDFontType
2. A print class is available to print out many data types.
3. Fonts 1-6 are byte high(at text size 1) scale-able fonts, columns of padding added by SW.
4. Font 7-8 are large numerical fonts and cannot be scaled(just one size).
5. Fonts 9-10 large Alphanumeric fonts and cannot be scaled(just one size)

Font Methods:

| Font num | Method | Notes |
| ------ | ------ | ------ |
| 1-6 | drawChar| draws single  character |
| 1-6 | drawText | draws character array |
| 7-10 | drawCharBigFont | draws single  character |
| 7-10 | drawTextBigFont | draws character array |
| 1-10 | print | Polymorphic print class which will print out many data types |

These methods return false in event of an error, such as wrong font chosen , ASCII character outside
chosen fonts range, character out of screen bounds and invalid character array pointer object.

### Bitmap

Two different bitmaps methods can be used.

| num | Method name | data addressing | Notes | 
| ------ | ------ | ------ | ------ |   
| 1 | drawBitmap() |vertical| Draws bitmaps to the buffer, Bitmap's height must be divisible by 8| 
| 2 | customChar()| vertical | Draws a custom character on screen 5by8, PIC legacy function | 

The drawBitmap will return an error if : The Bitmap is completely off screen , 
Invalid Bitmap pointer object, bitmap bigger than screen , bitmap bigger/smaller than provided width and height co-ordinates
( This prevents buffer overflow if user enters wrong data.).
The Bitmap's height must be divisible by 8. I.e for a full screen bitmap with width=84 and height=48.
Bitmap excepted size = 84 * (48/8) = 504 bytes.

Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) 
See example file "BitmapTests" for more details.

## Output

Example output.

![OUTPUT](https://github.com/gavinlyonsrepo/pic_18F47K42_projects/blob/master/images/nokiagraph1.jpg)
