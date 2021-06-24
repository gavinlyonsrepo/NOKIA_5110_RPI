Table of contents
---------------------------

  * [Overview](#overview)
  * [Installation](#installation)
  * [Hardware](#hardware)
  * [Features](#features)
  * [Output](#output)

Overview
--------------------------------------------
* Name: NOKIA_5110_RPI
* Description:

0. C Library for Nokia 5110 LCD, PCD8544 Driver. 84 x 48 pixels.
1. Dynamic install-able Raspberry Pi C library.
2. Inverse colour, rotate, sleep, contrast methods supported.
3. Five fonts
4. Graphics class included.
5. Bitmaps supported.
6. Hardware and Software SPI
7. Dependency: bcm2835 Library

* Author: Gavin Lyons
* Port of my PIC library at [github link.](https://github.com/gavinlyonsrepo/pic_18F47K42_projects)
* Created: June 2021
* Developed on
    1. Raspberry PI 3 model b,
    2. C complier gcc (Raspbian 6.3.0-18)
    3. Raspbian 9.13 stretch OS
    4. bcm2835 Library 1.68

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
curl -sL https://github.com/gavinlyonsrepo/NOKIA_5110_RPI/archive/1.0.tar.gz | tar xz
```

4. Run "make" to run the makefile to install library, it will be
    installed to usr/lib and usr/include

```sh
cd NOKIA_5110_RPI-1.0
sudo make
```

5. Next step is to test LCD and installed library with the main.c test file.
Wire up your LCD. Next enter the example folder and run the makefile in THAT folder,
This second makefile builds the examples file using the just installed library.
and creates a test exe file in "bin". 
Be sure to use "sudo" as the bcm2835 
requires root permissions by default [see here for more details on that](http://www.airspayce.com/mikem/bcm2835/)
you should now see the test routines in that file running on LCD.

```sh
cd example/
make
sudo bin/test
```

Features
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

The main.c contains the demo functions in a test loop showing features of library. 

[Nokia 5110 LCD dataSheet ](https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf)

![PICTURE](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/NOKIA2.jpg)

**SPI**

This library supports both Hardware SPI and software SPI.
Change the define a top of header file
to switch between the two(before install). USER OPTION SPI TYPE.
Uses bcm2835 library.
Tested at bcm2835 SPI_CLOCK_DIVIDER_32 = 7.8125MHz on Rpi2, 12.5MHz on RPI3
SPI settings can be viewed/changed in LCD_SPI_Initialize function.

**Files**

There is a LCD library (NOKIA_5110_RPI.c and NOKIA_5110_RPI.h),
and a fonts file that contains 5 ASCII pixel fonts.

In example folder:
The Main.c file contains tests showing library functions.
A bitmap data file contains data for  bitmaps  tests.

There are two makefiles

    1. Makefile at root directory builds and installs library at a system level.
    2. Makefile in example directory build example file to an executable.


**Fonts**

Five fonts available :

| Font num | Font name | Pixel size |  Note |
| ------ | ------ | ------ | ------ |
| 1 | Default | 5x8 | Full extended ASCII |
| 2 | Thick   | 7x8 |  no lowercase letters  |
| 3 | Seven segment | 4x8 | ------ |
| 4 | Wide | 8x8 | no lowercase letters |
| 5 | tiny | 3x8 |  ----- |

