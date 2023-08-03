/*
 * oled.h - SSD1306 I2C OLED driver for STM32F030
 * 04-16-2015 E. Brombaugh
 */

#ifndef __oled__
#define __oled__

#include "stm32f0xx.h"

//#define TINY_OLED
#ifdef TINY_OLED
#define OLED_W 64
#define OLED_H 32
#else
#define OLED_W 128
#define OLED_H 64
#endif
#define OLED_BUFSZ ((OLED_W/8)*OLED_H)

void oled_init(void);
uint32_t oled_i2c_command(uint8_t cmd);
uint32_t oled_i2c_data(uint8_t *data, uint8_t sz);
uint8_t *oled_get_fb(void);
void oled_refresh(void);
void oled_clear(uint8_t color);
void oled_drawPixel(uint8_t x, uint8_t y, uint8_t color);
void oled_xorPixel(uint8_t x, uint8_t y);
uint8_t oled_getPixel(uint8_t x, uint8_t y);
void oled_drawline(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void oled_drawchar(uint8_t x, uint8_t y, uint8_t chr, uint8_t color);
void oled_drawmagchar(uint8_t x, uint8_t y, uint8_t chr, uint8_t color, uint8_t sz);
void oled_drawstr(uint8_t x, uint8_t y, char *str, uint8_t color);
void oled_drawrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void oled_xorrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void oled_drawbitfont(uint8_t x, uint8_t y, char *str);
void oled_draw_logo(void);

#endif
