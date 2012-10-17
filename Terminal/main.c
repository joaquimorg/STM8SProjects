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
#include <string.h>



void Delay( int ms) {
	int i;
	while(ms--){ 
		i=2600;       //empirically determined fudge factor 16mhz
		while(i--); 
	} 
}

// Current X position
u8 curX = 0; 
// Current Y position
u8 curY = 0;

u16 Blink = 0;

u16 _term_offset = 0;
u8 _term[100];
u8 text[100];

void DrawScreen( void ) {

	ClearScr();
	
	DrawRectangle( 0,8, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1, LCD_PIXEL_ON );
	DrawText(1,1, "STM8S Terminal - joaquim.org", LCD_PIXEL_ON);
	//curX = 2;
	//curY = 10;
}

void CharDecode(char c) {
	switch (c) {
		case '\r':
        case '\n': 
		case '#':
			memset(text, 0, sizeof(text));
			strncpy(text,_term,_term_offset);
			text[_term_offset] = '\n';
			_term_offset = 0;
			memset(_term, 0, sizeof(_term));
			return;
			
		case '$':
			// Blink Debug LED
			GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
			return;
	}
	if (_term_offset < sizeof(_term) - 1)
        _term[_term_offset++] = c;
	else {
		strncmp(text,_term,_term_offset);
		_term_offset = 0;
	}
}


@far @interrupt void UART2_RX_IRQHandler(void) {
	u8 c;
	if (UART2_GetFlagStatus(UART2_FLAG_RXNE) != RESET) {
		/* Read one byte from the receive data register */
		c = UART2_ReceiveData8();
		CharDecode( c );
	}
}

void DrawDemo( void ) {
	char xx;
	int i;
	//char carRX = 0;
	/*u8 fontWidth;
	if ( Blink == 0 ) {
		Blink = 30000;
		fontWidth = DrawChar5x5(curX, curY, '_', LCD_PIXEL_INV);
		
		// Blink Debug LED
		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);	
		
	} else {
		Blink--;
	}
	if (UART2_GetFlagStatus(UART2_FLAG_RXNE) != RESET) {
		carRX = UART2_ReceiveData8();
		
	}
	if ( carRX != 0 ) {
		fontWidth = DrawChar5x5(curX, curY, '_', LCD_PIXEL_OFF);
		if ( carRX == 13 ) {			
			curX = 2;
			curY += FONT_HEIGHT + 1;
			if ( curY > DISPLAY_HEIGHT ) {
				DrawScreen();
			}	
			
		} else if ( carRX == 10 ) {
			
		} else {			
			fontWidth = DrawChar5x5(curX, curY, carRX, LCD_PIXEL_ON);
			curX += fontWidth + 1;			
			if ( curX > DISPLAY_WIDTH ) {		
				curX = 2;
				curY += FONT_HEIGHT + 1;				
				if ( curY + FONT_HEIGHT > DISPLAY_HEIGHT ) {
					DrawScreen();
				}
			}
		}

	}*/

	Blink = 0;
	DrawScreen();
	GotoXY(2,10);
	PutText(text,LCD_PIXEL_ON);
	UpdateLCD();
	if (  strlen(text) > 0 ) {
		xx = text[0];
		for(i=1; i < strlen(text)-1; i++) {
			text[i - 1] = text[i];
		}
		text[strlen(text)-2] = xx;
	}

}

int main(void) {
	
	// Set the internal high-speed oscillator to 1 to run at 16/1=16MHz.
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	
	// Reset ("de-initialise") GPIO port D.
	GPIO_DeInit(GPIOD);
	// Initialise pin 0 of port D by setting it as:
	// - an output pin,
	// - using a push-pull driver,
	// - at a low logic level (0V), and
	// - 10MHz.
	GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
	
	UART2_DeInit();
	
	/* UART2 configuration ------------------------------------------------------*/
	/* UART2 configured as follow:
		- BaudRate = 9600 baud  
		- Word Length = 8 Bits
		- One Stop Bit
		- Even parity
		- Receive and transmit enabled
		- UART2 Clock disabled
	*/
	/* Configure the UART2 */
	UART2_Init((u32)9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, 
		UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
		
	LCDInit();
	
	DrawScreen();
	UpdateLCD();
	
	UART2_ITConfig(UART2_IT_RXNE, ENABLE);
	
	enableInterrupts();
	// Infinite loop.
	for(;;) {
							
		DrawDemo();	
		Delay(80);
	}
}