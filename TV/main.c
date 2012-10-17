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
#include <string.h>

#define LCD_ADDR_1   0x60


#define I2C_Speed              100000
#define I2C1_SLAVE_ADDRESS7    0xA0


	
// ---------------------------------------------------

void lcddrive_init(void) {
	u8 Input_Clock = 0x0;
	
	/* Get system clock frequency */
	Input_Clock = CLK_GetClockFreq()/1000000;
	
	/* I2C Peripheral Enable */
	I2C_Cmd(ENABLE);
  
	/* Initialize the I2C */
	I2C_Init(I2C_Speed, I2C1_SLAVE_ADDRESS7, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, Input_Clock);
	
}


void lcddrive_send(u8 port, u8 data) {
	u8 tout = 0;
	u8 addok = 0;
	
	/* While the bus is busy */
	while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
  
	/* Send START condition */
	I2C_GenerateSTART(ENABLE);
	
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));
	
	/* Send slave Address for write */
	I2C_Send7bitAddress(port, I2C_DIRECTION_TX);
	
	/* Test on EV6 and clear it */
	while(( addok == 0 ) && ( tout < 100 )) {
		if ( I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED) ) {
			addok = 1;			
		}
		tout += 1;		
	};
	
	I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);

	if ( addok == 1 ) {
		
		I2C_SendData(data); 
		/* Test on EV8 and clear it */
		while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
		I2C_SendData(0xff); 
		/* Test on EV8 and clear it */
		while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
		
		
	}
	/* Send STOP Condition */
	I2C_GenerateSTOP(ENABLE);
}

// ---------------------------------------------------

void Delay( int ms) {
	int i;
	while(ms--){ 
		i=2600;       //empirically determined fudge factor 16mhz
		while(i--); 
	} 
}


int main(void) {
	u8 data, port;
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
	
	lcddrive_init();
	data = 0x00;
	port = 0x51;
	for(;;) {
		Delay(10);
		// Blink Debug LED
		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
		lcddrive_send( port, data );
		port +=1;
		if ( port > 0xF0 ) port = 0x03;
		data +=1;
		if ( data == 0xFF ) data = 0x00;
	}
}