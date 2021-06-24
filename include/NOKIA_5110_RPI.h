/*
* Project Name: Nokia  5110 Graphic library for PIC  micro-controller
* Description: Header file for NOKIA 5110 library to communicate with LCD
* Author: Gavin Lyons.
* Created: July 2021
* Description: See URL for full details.
* URL: https://github.com/gavinlyonsrepo/NOKIA_5110_RPI
*/

#ifndef _NOKIA_5110_H
#define _NOKIA_5110_H

// ************ libraries ************
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h> 
#include <bcm2835.h>

#include "NOKIA_5110_RPI.h"
#include "NOKIA_5110_RPI_Font.h"

//*********** Definitions **************

// ******** USER OPTION 1 SPI TYPE ***********
// SPI LCD module connections
// *** If software SPI module is used, comment this line OUT ***
#define LCD_SPI_HARDWARE
// *******************************************

#define BLACK 1
#define WHITE 0

#define LCDWIDTH  84
#define LCDHEIGHT 48

#define LCD_POWERDOWN            0x04
#define LCD_ENTRYMODE            0x02
#define LCD_EXTENDEDINSTRUCTION  0x01

#define LCD_DISPLAYBLANK         0x00
#define LCD_DISPLAYNORMAL        0x04
#define LCD_DISPLAYALLON         0x01
#define LCD_DISPLAYINVERTED      0x05

#define LCD_FUNCTIONSET          0x20
#define LCD_DISPLAYCONTROL       0x08
#define LCD_SETYADDR             0x40
#define LCD_SETXADDR             0x80

#define LCD_SETTEMP  0x04
#define LCD_SETBIAS  0x13
#define LCD_SETVOP   0x80

#define LCD_HIGHFREQ_DELAY 2
#define LCD_ASCII_OFFSET 0x20 //ASCII character for Space
#define LCD_ASCII_OFFSET_NUM 0x30 //offset for bignum font

// GPIO 
#define LCD_DC_SetHigh  bcm2835_gpio_write(_LCD_DC, HIGH)
#define LCD_DC_SetLow  bcm2835_gpio_write(_LCD_DC, LOW)
#define LCD_RST_SetHigh  bcm2835_gpio_write(_LCD_RST, HIGH)
#define LCD_RST_SetLow  bcm2835_gpio_write(_LCD_RST, LOW)
#define LCD_CE_SetHigh bcm2835_gpio_write(_LCD_CE, HIGH) // SW SPI last 6 lines
#define LCD_CE_SetLow bcm2835_gpio_write(_LCD_CE, LOW)
#define LCD_CLK_SetHigh bcm2835_gpio_write(_LCD_CLK, HIGH)
#define LCD_CLK_SetLow  bcm2835_gpio_write(_LCD_CLK, LOW)
#define LCD_DIN_SetHigh bcm2835_gpio_write(_LCD_DIN, HIGH)
#define LCD_DIN_SetLow  bcm2835_gpio_write(_LCD_DIN,LOW)


#define LCD_DC_SetDigitalOutput bcm2835_gpio_fsel(_LCD_DC, BCM2835_GPIO_FSEL_OUTP)
#define LCD_RST_SetDigitalOutput bcm2835_gpio_fsel(_LCD_RST, BCM2835_GPIO_FSEL_OUTP)
#define LCD_CLK_SetDigitalOutput bcm2835_gpio_fsel(_LCD_CLK, BCM2835_GPIO_FSEL_OUTP) // SW SPI last 3 lines
#define LCD_DIN_SetDigitalOutput bcm2835_gpio_fsel(_LCD_DIN, BCM2835_GPIO_FSEL_OUTP)
#define LCD_CE_SetDigitalOutput bcm2835_gpio_fsel(_LCD_CE, BCM2835_GPIO_FSEL_OUTP)

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

#define pcd8544_swap(a, b) { uint8_t t = a; a = b; b = t; }
#define display_drawPixel   drawPixel
#define display_drawVLine   drawVLine
#define display_drawHLine   drawHLine
#define display_fillScreen  fillScreen
#define display_fillRect    fillRect
#define display_setRotation setRotation
#define display_invert      invertDisplay



// ************* User Functions ************
#ifdef __cplusplus
extern "C" {
#endif

void LCD_begin(int8_t, int8_t, int8_t, int8_t, int8_t);
void LCDenableSleep(void);
void LCDdisableSleep(void);
void LCD_SPI_Initialize(void);
void LCD_SPIoff(void);
void LCD_Power_Down(void);
void LCDFontNum(uint8_t FontNumber);

void display_drawPixel(uint8_t x, uint8_t y, bool color);
void drawHLine(uint8_t x, uint8_t y, uint8_t w, bool color);
void drawVLine(uint8_t x, uint8_t y, uint8_t w, bool color);
void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color);
void setRotation(uint8_t m);
void display_setContrast(uint8_t con);
void display(void);
void display_clear(void);
void fillScreen(void);
void invertDisplay(bool inv);

void display_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);
void display_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
void display_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color);
void display_drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint8_t color);
void display_fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t color);
void display_fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t delta, uint8_t color);
void display_drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
void display_fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
void display_drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint8_t color);
void display_fillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint8_t color);
void display_setCursor(uint16_t x, uint16_t y);
void display_setTextColor(uint8_t c, uint8_t bg);
void display_setTextSize(uint8_t s);
void display_setTextWrap(bool w);
void display_putc(char c);
void display_puts(char *s);
void display_printf(const char *fmt, ...);
void display_customChar(const uint8_t *c);
void display_drawChar(uint16_t x, uint16_t y, uint8_t c, uint8_t color, uint8_t bg, uint8_t size);

uint8_t  display_getRotation();
uint16_t getCursorX(void);
uint16_t getCursorY(void);
uint16_t display_getWidth();
uint16_t display_getHeight();

void display_drawBitmapV2(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint8_t color);

// ******************** Non User Functions ********************
void writeLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t color);
uint8_t printNumber(uint32_t n, int8_t n_width, uint8_t _flags);
void printFloat(double float_n, int8_t f_width, int8_t decimal, uint8_t _flags);
void v_printf(const char *fmt, va_list arp);
void PCD8544_SPI_Write(uint8_t d);
void writeCommand(uint8_t c);

#ifdef __cplusplus
}
#endif

#endif // .h file Guard header
