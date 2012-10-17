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
#include "ledmatrix.h"
#include "string.h"


void Delay( int ms) {
	int i;
	while(ms--){ 
		i=2600;       //empirically determined fudge factor 16mhz
		while(i--); 
	} 
}

u8 _term[100];
u8 texto[100] = "WWW.JOAQUIM.ORG - Led Matrix display !  Ready for serial data.....     \0";
u16 _term_offset = 0;
int ll = 0;

void CharDecode(char c) {
	switch (c) {
		case '\r':
        case '\n': 
			
			return;
			
		case '#':
			// Blink Debug LED
			//GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
			memset(texto, 0, sizeof(texto));
			strncpy(texto,_term,_term_offset);
			texto[_term_offset] = '\0';
			_term_offset = 0;
			memset(_term, 0, sizeof(_term));
			ll = 0;
			return;
	}
	if (_term_offset < sizeof(_term) - 1)
        _term[_term_offset++] = c;
	else {
		//strncmp(text,_term,_term_offset);
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

main()
{
	int cc = 0, xx = 0, i, pp = 0;
	char textoscr[5]="\0\0\0\0\0";
	
	/* Configure HSI div 1 as clock source */ 
	/* The MCU is clocked by 16 MHz */ 
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); 
	
	// Reset ("de-initialise") GPIO port D.
	GPIO_DeInit(GPIOD);
	GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
	
	InitMatrix();
	
	TIM2_DeInit ();
	TIM2_TimeBaseInit (TIM2_PRESCALER_256 ,31); // = 10Hz 16000000/512/3125 - 31250
	TIM2_ITConfig (TIM2_IT_UPDATE, ENABLE);	
	
	//UART2_DeInit();
	
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
	//UART2_Init((u32)9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, 
	//	UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
		
	//UART2_ITConfig(UART2_IT_RXNE, ENABLE);
	
	enableInterrupts(); 
	TIM2_Cmd (ENABLE);
		
	/* Toggles PD.0 in ISR routine*/ 
	while (1) {
		GotoXY(0 - ll,0);
		PutText(texto,RED);
		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
		cc++;
		if ( cc > 2 ) {
			if ( strlen(texto) > 0 ) {
				xx = 5;
				ll++;
				if ( ll > xx ) {
					ll = 0;
					xx = texto[0];
					for(i=0; i < strlen(texto)-1; i++) {
						texto[i] = texto[i + 1];
					}
					texto[strlen(texto)-2] = xx;
					strncpy(textoscr,texto,4);
				}
			}
			cc = 0;
			//pp++;
			//if ( pp > 3 ) pp = 0;
			//DrawPoint(0,7,pp);
		}
	}	
}