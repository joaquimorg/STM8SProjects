//	
//		main.c
//      
//      Copyright 2010 joaquim <mail@joaquim.org>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include <stdio.h>
#include "stm8s.h"
#include "pff.h"
#include "s65.h"
#include "key.h"
#include <string.h>

/**
  * @brief Delay.
  * @param[in] nCount
  * @retval 
  * None
  */
void Delay(u16 nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}


void Demo(void) {
	//

}


int main(void) {
	
	FRESULT res;
	FATFS fs;			/* File system object */
	DIR dir;			/* Directory object */
	FILINFO fno;		/* File information */
	char *ptr = "";
	char str[25];
	int a,b,c,x,y;
	unsigned short ss,color;
	KEYCODE key;
	//u8 data[512];		/* Console input buffer */

	// Initialise the clock to have a /1 prescaler and use the external crystal clock source for accuracy.
	CLK_DeInit();
	//CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	//CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
	
	/*Clock configuration fmaster = 16MHz*/
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	
	// Reset ("de-initialise") GPIO port D.
	GPIO_DeInit(GPIOD);
	// Initialise pin 0 of port D by setting it as:
	// - an output pin,
	// - using a push-pull driver,
	// - at a low logic level (0V), and
	// - 10MHz.
	GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
	
	SPI_DeInit();
	
	/* Initialize SPI in Master mode */
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_HIGH, SPI_CLOCKPHASE_2EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
		
    SPI_Cmd(ENABLE);
	
	KEY_Init();
	
	//Delay(100);
	
	LCD_Init();
	//draw();	
	
	res = pf_mount(&fs);
	res = pf_open("INTROS8.BMP");
	if ( res == FR_OK ) {
		res = pf_read_bmp(0,0);
		if ( res != FR_OK ) {
			ShowMsh("Error reading BMP file !");
		}
		
	} else {
		ShowMsh("Error opening file !");
	}
	//WaitKey(KEY_UP);
	
	/*a=40;
	LCD_Puts(5, a, "-------- SDMMC DIR --------", RGB(255,0,0), RGB(0,0,0));
	a += 8;
	pf_mount(NULL);
	res = pf_mount(&fs);
	res = pf_opendir(&dir, ptr);
	if ( res == FR_OK ) {
		for(;;) {
			res = pf_readdir(&dir, &fno);
			if (res != FR_OK) break;
			if (!fno.fname[0]) break;
			if (fno.fattrib & AM_DIR) {
				sprintf(str, "<DIR>  %s\0", fno.fname);
			} else {
				sprintf(str, "%5lu  %s\0", fno.fsize, fno.fname);
			}	
			LCD_Puts(5, a, str, RGB(255,255,255), RGB(0,0,0));
			a += 6;
		}
	}
	*/
	//WaitKey(KEY_DOWN);
	pf_mount(NULL);
	res = pf_mount(&fs);
	res = pf_open("TESTE.BMP");
	if ( res == FR_OK ) {
		res = pf_read_bmp(130,50);
		if ( res != FR_OK ) {
			ShowMsh("Error reading BMP file !");
		}
		
	} else {
		ShowMsh("Error opening file !");
	}
	a = 115;
	// Infinite loop.
	for(;;) {
		// Blink Debug LED
		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
			
		Delay((u16)0xFFFF);
		//b = GPIO_ReadInputData( KEYSCAN_PORT );
		key = GetKey();
		LCD_FillRect(4,a-1,100,a+5, RGB(0,0,0));
		switch( key ) {
			case KEY_UP :
				sprintf(str, "Key UP\0");
				break;
			case KEY_DOWN :
				sprintf(str, "Key DOWN\0");
				break;
			case KEY_ENTER :
				sprintf(str, "Key ENTER\0");
				break;
			case KEY_ESC :
				sprintf(str, "Key ESCAPE\0");
				break;
			case KEY_NONE :
				sprintf(str, "No key pressed.\0");
				break;				
		}
		
		LCD_Puts(5, a, str, RGB(255,255,255), RGB(0,0,0));
	}
}