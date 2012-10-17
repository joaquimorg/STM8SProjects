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
#include "main.h"

u8 RxBuffer1[81];
vu8 RxCounter1 = 0x00;

void DrawScreen( void ) {
	u8 fontWidth;
	ClearScr();
	
	DrawFillRectangle( 0,57, 128, 64, LCD_PIXEL_ON);
	DrawText(1,58, "MiniBASIC v0.1 - joaquim.org", LCD_CHAR_INV);
}

void DrawDemo( void ) {	
	
	if (GetVar_RxCounter1() > 0) {

		DrawText(0, 0, RxBuffer1, LCD_PIXEL_ON);
	
		/* Enable UART2 Receive */
		//UART2_ITConfig(UART2_IT_RXNE_OR, DISABLE);
		UpdateLCD();
		DrawScreen();
	}
}


/**
  * @brief Increments RxCounter1 variable and return its value 
  * @par Parameters:
  * None
  * @retval Receive counter index1++
  * - u8: RxCounter1++
  * @par Required preconditions:
  * None
  */
u8 IncrementVar_RxCounter1(void)
{
	return RxCounter1++;
}

/**
  * @brief Returns RxCounter1 value.
  * @par Parameters:
  * None
  * @retval Receive counter index1
  * - u8: RxCounter1
  * @par Required preconditions:
  * None
  */
u8 GetVar_RxCounter1(void)
{
	return RxCounter1;
}

void SetVar_RxCounter1( u8 Counter )
{
	RxCounter1 = Counter;
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
	
	UART2_DeInit();
	
	/* UART2 configuration ------------------------------------------------------*/
	/* UART2 configured as follow:
		- BaudRate = 9600 baud  
		- Word Length = 8 Bits
		- One Stop Bit
		- Odd parity
		- Receive and transmit enabled
		- UART2 Clock disabled
	*/
	/* Configure the UART2 */
	UART2_Init((u32)4800, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, 
		UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	
	LCDInit();
	
	DrawScreen();
	
	/* Enable UART2 Receive */
    UART2_ITConfig(UART2_IT_RXNE, ENABLE);
	
	/* Enable general interrupts */
    enableInterrupts();
	
	// Infinite loop.
	for(;;) {
		// Blink Debug LED
		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
		
		DrawDemo();	
	}
}