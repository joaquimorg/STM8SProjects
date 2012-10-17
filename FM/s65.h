/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __S65_H
#define __S65_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* S65 activation port & pin */
#define S65_CS_PORT (GPIOG)
#define S65_CS_PIN  (GPIO_PIN_1)

#define S65_RS_RST_PORT (GPIOC)
#define S65_RS_PIN  (GPIO_PIN_2)
#define S65_RST_PIN  (GPIO_PIN_1)

//LCD
#define LCD_RST_DISABLE()              GPIO_WriteHigh(S65_RS_RST_PORT, S65_RST_PIN)
#define LCD_RST_ENABLE()               GPIO_WriteLow(S65_RS_RST_PORT, S65_RST_PIN)
#define LCD_CS_DISABLE()               GPIO_WriteHigh(S65_CS_PORT, S65_CS_PIN)
#define LCD_CS_ENABLE()                GPIO_WriteLow(S65_CS_PORT, S65_CS_PIN)
#define LCD_RS_DISABLE()               GPIO_WriteHigh(S65_RS_RST_PORT, S65_RS_PIN)
#define LCD_RS_ENABLE()                GPIO_WriteLow(S65_RS_RST_PORT, S65_RS_PIN)

#define _LCD_WIDTH                     (176)
#define _LCD_HEIGHT                    (132)

#ifdef LCD_ROTATE
#define LCD_WIDTH                     _LCD_HEIGHT
#define LCD_HEIGHT                    _LCD_WIDTH
#else
#define LCD_WIDTH                     _LCD_WIDTH
#define LCD_HEIGHT                    _LCD_HEIGHT
#endif

#define RGB(r,g,b)                     (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue
#define GET_RED(x)                     ((x>>8)&0xF8) //5 red
#define GET_GREEN(x)                   ((x>>3)&0xFC) //6 green
#define GET_BLUE(x)                    ((x<<3)&0xF8) //5 blue


#define DATCTL 0xBC         // Data Control (data handling in RAM)
#define DISCTL 0xCA         // Display Control
#define GCP64 0xCB          // pulse set for 64 gray scale
#define GCP16 0xCC          // pulse set for 16 gray scale
#define OSSEL 0xD0          // Oscillator select
#define GSSET 0xCD          // set for gray scales
#define ASCSET 0xAA         // aerea scroll setting
#define SCSTART 0xAB        // scroll start setting
#define DISON 0xAF          // Display ON (no parameter)
#define DISOFF 0xAE         // Display OFF (no parameter)
#define DISINV 0xA7         // Display Invert (no parameter)
#define DISNOR 0xA6         // Display Normal (no parameter)
#define SLPIN  0x95         // Display Sleep (no parameter)
#define SLPOUT 0x94         // Display out of sleep (no parameter)
#define RAMWR 0x5C          // Display Memory write
#define PTLIN 0xA8          // partial screen write

#define SD_CSET 0x15        // column address setting
#define SD_PSET 0x75        // page address setting

void LCD_Init(void);

void LCD_SendCMD(u8 Data);
void LCD_SendDAT(u8 Data);
void LCD_SendDAT16(u16 Data);
void LCD_SPISendByte(u8 DataToSend);

void LCD_Fill(u16 color);
void LCD_DrawPixel(unsigned int x, unsigned int y, unsigned int color);
void LCD_DrawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color);
void LCD_SetArea(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
void LCD_FillRect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color);

unsigned int LCD_Puts(unsigned int x, unsigned int y, const char *s, unsigned int color, unsigned int bgcolor);
void LCD_PutLinebr(unsigned int x, unsigned int y, const char *s, unsigned int color, unsigned int bgcolor);

void ShowMsh( const char *s );

#endif /* __MSD_H */