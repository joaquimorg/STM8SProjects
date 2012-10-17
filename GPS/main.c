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

#include "stm8s.h"
#include "glcdlib.h"
#include "nmea.h"
#include <stdio.h>
#include <math.h>

char *debug;

@far @interrupt void UART2_RX_IRQHandler(void) {
	u8 c;
	if (UART2_GetFlagStatus(UART2_FLAG_RXNE) != RESET) {
		/* Read one byte from the receive data register */
		c = UART2_ReceiveData8();
		if (EncodeNMEA( c )) {
			  // process new gps info here
		}
	}
}

void DrawScreen( void ) {
	u8 fontWidth;
	ClearScr();
	
	DrawFillRectangle( 0,57, 128, 64, LCD_PIXEL_ON);
	DrawText(1,58, "GPS v0.1 - joaquim.org", LCD_CHAR_INV);
}


void DrawGPSData( void ) {
	int tmp,barrapos,sinal,x,y;
	u8 str_buffer[0x10];
	float valor;
	gps_data_t local_gps_data;
	
	
	get_gps_data(&local_gps_data);
	
	valor = local_gps_data.pos.lat;
	sprintf(str_buffer, "%03.6f", valor);
	GotoXY( 0, 0 );
	PutText("LAT : ", LCD_PIXEL_ON);
	PutText(str_buffer, LCD_PIXEL_ON);

	valor = local_gps_data.pos.lon;
	sprintf(str_buffer, "%03.6f", valor);	
	GotoXY( 0, 6 );
	PutText("LON : ", LCD_PIXEL_ON);
	PutText(str_buffer, LCD_PIXEL_ON);
	
	valor = local_gps_data.pos.alt;
	sprintf(str_buffer, "%04.1f", valor);	
	GotoXY( 0, 12 );
	PutText("ALT : ", LCD_PIXEL_ON);
	PutText(str_buffer, LCD_PIXEL_ON);
	/*
	valor = local_gps_data.status.siu;
	sprintf(str_buffer, "%d", (int)valor);	
	GotoXY( 70, 0 );
	PutText("SAT : ", LCD_PIXEL_ON);
	PutText(str_buffer, LCD_PIXEL_ON);
	
	valor = local_gps_data.pos.cor;
	sprintf(str_buffer, "%d", (int)valor);	
	GotoXY( 70, 6 );
	PutText("CUR : ", LCD_PIXEL_ON);
	PutText(str_buffer, LCD_PIXEL_ON);
	
	valor = local_gps_data.pos.spd;
	sprintf(str_buffer, "%d", (int)valor);	
	GotoXY( 70, 12 );
	PutText("SPE : ", LCD_PIXEL_ON);
	PutText(str_buffer, LCD_PIXEL_ON);
	*/
	
	GotoXY( 0, 51 );	
	PutText(local_gps_data.datetime.date, LCD_PIXEL_ON);
	PutText("   ", LCD_PIXEL_ON);
	PutText(local_gps_data.datetime.time, LCD_PIXEL_ON);	
	
	DrawCircle( 95,30,30, LCD_PIXEL_ON );
	DrawCircle( 95,30,20, LCD_PIXEL_ON );
	DrawCircle( 95,30,10, LCD_PIXEL_ON );
	DrawCircle( 95,30,1, LCD_PIXEL_ON );
	barrapos = 2;
	for(tmp = 0; tmp < 12; tmp++) {
		valor = local_gps_data.sat_inview[tmp].snr;		
		sinal = (int)valor / 5;
		DrawRectangle( barrapos - 1,27, barrapos + 1, 49, LCD_PIXEL_ON);
		DrawFillRectangle( barrapos - 1,49 - sinal, barrapos + 1, 49, LCD_PIXEL_ON); 
		barrapos += 5;
		if ( valor > 0) {
			x = 95 + (30 - local_gps_data.sat_inview[tmp].ele / 3 ) * cos(local_gps_data.sat_inview[tmp].azi);
			y = 30 + (30 - local_gps_data.sat_inview[tmp].ele / 3 ) * sin(local_gps_data.sat_inview[tmp].azi);
			DrawCircle( x,y,3, LCD_PIXEL_ON );
			DrawCircle( x,y,2, LCD_PIXEL_ON );
			DrawCircle( x,y,1, LCD_PIXEL_ON );
			DrawPoint( x,y, LCD_PIXEL_ON );
			GotoXY( x - 1, y - 2 );
			sprintf(str_buffer, "%01d", (int)tmp);
			PutText(str_buffer, LCD_PIXEL_OFF);
		}
	}
	
}

void DrawDemo( void ) {	
	/*char *str = "$GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74";
	u8 cnt;
	for(cnt = 0; cnt < 69; cnt++) {
		if (EncodeNMEA( str[cnt] )) {
			  // process new gps info here
		}
	}*/
	//DrawText(0, 50, debug, LCD_PIXEL_ON);
	
	DrawGPSData();
	UpdateLCD();
	DrawScreen();
	
}


int main(void) {

	// Initialise the clock to have a /1 prescaler and use the external crystal clock source for accuracy.
	CLK_DeInit();
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
	
	// Reset ("de-initialise") GPIO port D.
	GPIO_DeInit(GPIOD);
	// Initialise pin 0 of port D by setting it as:
	// - an output pin,
	// - using a push-pull driver,
	// - at a low logic level (0V), and
	// - 10MHz.
	GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
	
	LCDInit(); // Init the LCD
	DecodeInit(); // Init the GPS decoding
	DrawScreen(); // Setup Screen and Buffer
	
	// Infinite loop.
	for(;;) {
		// Blink Debug LED
		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
		
		DrawDemo();	
	}
}