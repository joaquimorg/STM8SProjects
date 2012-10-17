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


typedef struct {
	u8 port;
	
	/* write-only parameters */
	/* B DATA */
	u8  sound_trap;
	u8  auto_mute_fm;
	u8  carrier_mode;
	u8  modulation;
	u8  forced_mute_audio;
	u8  port1;
	u8  port2;
	/* C DATA */
	u8  top_adjustment;
	u8  deemphasis;
	u8  audio_gain;
	/* E DATA */
	u8  standard_sound_carrier;
	u8  standard_video_if;
	u8  minimum_gain;
	u8  gating;
	u8  vif_agc;
	
	/* read-only values */
	
	u8  after_reset;
	u8  afc_status;
	u8  vif_level;
	u8  afc_win;
	u8  fm_carrier;
	
} TDA9885Rec;

#define TDA9885_ADDR_1   0x86
#define TDA9885_ADDR_2   0x84
#define TDA9885_ADDR_3   0x96
#define TDA9885_ADDR_4   0x94

#define I2C_Speed              100000
#define I2C1_SLAVE_ADDRESS7    0xA0

TDA9885Rec t;

u32 freqactual = 64;
u32 step = 1;
	
// ---------------------------------------------------

void tda9885_init(void) {
	u8 Input_Clock = 0x0;
	
	/* Get system clock frequency */
	Input_Clock = CLK_GetClockFreq()/1000000;
	
	/* I2C Peripheral Enable */
	I2C_Cmd(ENABLE);
  
	/* Initialize the I2C */
	I2C_Init(I2C_Speed, I2C1_SLAVE_ADDRESS7, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, Input_Clock);
	
	/* B data */
	t.sound_trap = 1;
	t.auto_mute_fm = 0;
	t.carrier_mode = 1;
	t.modulation = 2;
	t.forced_mute_audio = 0;
	t.port1 = 0;
	t.port2 = 0;
	
	/* C data */
	t.top_adjustment = 32;
	t.deemphasis = 0;
	t.audio_gain = 0;
	
	/* E data */
	t.standard_sound_carrier = 1;
	t.standard_video_if = 3;
	t.minimum_gain = 0;
	t.gating = 0;
	t.vif_agc = 1;
	
	t.port = TDA9885_ADDR_1;
}

void tda9885_getstatus(void) {
	u8 value;

	/* While the bus is busy */
	while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
  
	/* Send START condition */
	I2C_GenerateSTART(ENABLE);
	
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));
	
	/* Send slave Address for write */
	I2C_Send7bitAddress(t.port, I2C_DIRECTION_RX);
	
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
	I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
	
	/* Disable Acknowledgement */
	I2C_AcknowledgeConfig(I2C_ACK_NONE);
      
	/* Send STOP Condition */
	I2C_GenerateSTOP(ENABLE);
	
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED));
	value = I2C_ReceiveData();
	//I2C_WriteRead(&(t->d), NULL, 0, &value, 1); 
	t.after_reset=value & 1;
	t.afc_status=(value >> 1) & 0xf;
	t.fm_carrier=(value>>5)& 1;
	t.vif_level=(value >>6) & 1;
	t.afc_win=(value >> 7)&1;
	
	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C_ACK_CURR);
}


void tda9885_setparameters(void) {
	u8 data[4];
	
	data[0]=0;  /* start with subaddress 0 */
	data[1]=(t.sound_trap & 1) |
		((t.auto_mute_fm &1)<<1) |
		((t.carrier_mode &1)<<2) |
		((t.modulation &3)<<3) |
		((t.forced_mute_audio &1)<<5) |
		((t.port1 & 1)<<6) |
		((t.port2 &1)<<7);         /* B data */
	data[2]=(t.top_adjustment & 0x1f) |
		((t.deemphasis & 0x3)<<5) |
		((t.audio_gain & 1) << 7);  /* C data */
	data[3]=(t.standard_sound_carrier & 0x3) |
		((t.standard_video_if & 0x07)<<2) |
		((t.minimum_gain & 0x01)<<5) |
		((t.gating & 0x01)<<6) |
		((t.vif_agc & 0x01)<<7);   /* E data */
		
	/* While the bus is busy */
	while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
  
	/* Send START condition */
	I2C_GenerateSTART(ENABLE);
	
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));
	
	/* Send slave Address for write */
	I2C_Send7bitAddress(t.port, I2C_DIRECTION_TX);
	
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
	I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);

	I2C_SendData(data[0]); 
	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
	I2C_SendData(data[1]); 
	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
	I2C_SendData(data[2]); 
	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
	I2C_SendData(data[3]); 
	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
	
	/* Send STOP Condition */
	I2C_GenerateSTOP(ENABLE);
}


void tda6500_setparameters( u32 freq ) {
	u8 data[5];
	u32 freq1 = 0;
	u32 freq2 = 0;
	u8 band = 0;
	
	//freq2 = 38000; // picture carrier pal
	//freq2 += 5500;
	//freq1 = freq * 312;
	
	//freq2 /= 10;
	//freq2 += freq1; // KHz
	//freq2 *= 10;
	//freq2 /= step;
  
	//freq2 += freq1;
	//freq2 /= 500;
  
	//if(freq2 < 170000)
		band = 0x1;
	//else if(freq2 > 170000 && freq2 < 450000)
    //    band = 0x2;
	//else
    //    band = 0x4;
		
	data[0]= (freq >> 8) & 0xFF;  
	data[1]= (freq >> 0) & 0xFF;  
	data[2]=0x82; // 0x8E  
	data[3]=band;
	data[4]=0xA0;  	
	
		
	/* While the bus is busy */
	while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY));
  
	/* Send START condition */
	I2C_GenerateSTART(ENABLE);
	
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));
	
	/* Send slave Address for write */
	I2C_Send7bitAddress(0xC0, I2C_DIRECTION_TX);
	
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
	I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);

	I2C_SendData(data[0]); 
	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
	I2C_SendData(data[1]); 
	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
	I2C_SendData(data[2]); 
	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
	I2C_SendData(data[3]); 
	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
	I2C_SendData(data[4]); 
	/* Test on EV8 and clear it */
	while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
	
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


void DrawScreen( void ) {

	ClearScr();
	
	DrawRectangle( 0,8, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1, LCD_PIXEL_ON );
	DrawText(1,1, "STM8S FM Scanner - joaquim.org", LCD_PIXEL_ON);
	//curX = 2;
	//curY = 10;
}



void DrawDemo( void ) {
	u8 curY = 10;
	
	u32 freq1 = 0;
	u32 freq2 = 0;
	u8 band = 0;
	
	freq2 = 38000; // picture carrier pal
	//freq2 += 5500;
	freq1 = freqactual * 31;
	
	//freq2 /= 10;
	freq2 += freq1; // KHz
	
	DrawScreen();

	GotoXY(2,curY);
	PutText("afc_status : ",LCD_PIXEL_ON);
	PrintNumber(t.afc_status,LCD_PIXEL_ON);
	curY += 6;

	GotoXY(2,curY);
	PutText("fm_carrier : ",LCD_PIXEL_ON);
	PrintNumber(t.fm_carrier,LCD_PIXEL_ON);
	curY += 6;
	curY += 6;

	GotoXY(2,curY);
	PutText("Freq : ",LCD_PIXEL_ON);
	PrintNumber(freq2,LCD_PIXEL_ON);
	PutText(" MHz",LCD_PIXEL_ON);
	curY += 6;
	UpdateLCD();
	
	// Blink Debug LED
	GPIO_WriteReverse(GPIOD, GPIO_PIN_0);

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
	
	LCDInit();
	
	DrawScreen();
	UpdateLCD();
	Delay(80);	
	
	tda9885_init();
	tda9885_setparameters();
	tda6500_setparameters(freqactual);
	
	for(;;) {
		tda9885_getstatus();
		DrawDemo();	
		Delay(100);
		//if ( t.fm_carrier == 1 ) {
		//	Delay(4000);
		//}
		tda6500_setparameters(freqactual);
		freqactual += step;
		
		if ( freqactual > 32766 ) freqactual = 64;
		Delay(100);
	}
}