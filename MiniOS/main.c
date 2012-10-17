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
#include "glib.h"
#include <string.h>
#include <stdio.h>
#include "nmea.h"
#include "s65.h"

u16 Conversion_Value = 0;

gps_data_t local_gps_data;
bool valid_sentence = FALSE;
u8 tmp_sat_inview[12];


@far @interrupt void Tim2Update_isr(void) {
		
	TIM2_ClearFlag(TIM2_FLAG_UPDATE);

	ADC1_StartConversion();
	Conversion_Value = ADC1_GetConversionValue();

}

/**
  * @brief Count average of samples stored in the u16 field
  * @par Parameters:
  *  *p: pointer to the begin of the field
  * smp: number of samples in the field
  * @retval Average u16 value
  */
u16 u16_average(u16 *p, u8 smp) { 
	u8 i;
	u16 sum;
	
	for(i=0, sum= 0; i < smp; ++i)
		sum+= *p++;		
	return sum / smp;
}

void ADC_Init(void) {
	TIM2_DeInit ();
	TIM2_TimeBaseInit (TIM2_PRESCALER_32 ,32000); // = 10Hz 16000000/512/3125 - 31250
	TIM2_ITConfig (TIM2_IT_UPDATE, ENABLE);
	TIM2_Cmd (ENABLE);
  
    ADC1_DeInit();
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_6, ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_LEFT, ADC1_SCHMITTTRIG_ALL, DISABLE);
	//ADC1_DataBufferCmd(ENABLE);
	//ADC1_ScanModeCmd(ENABLE);
}

/**
  * @brief Delay.
  * @param[in] ms
  * @retval 
  * None
  */
void Delay( int ms) {
	int i;
	while(ms--){ 
		i=2600;       //empirically determined fudge factor 16mhz
		while(i--); 
	} 
}

void CharDecode(char c);

@far @interrupt void UART2_RX_IRQHandler(void) {
	u8 c;
	if (UART2_GetFlagStatus(UART2_FLAG_RXNE) != RESET) {
		/* Read one byte from the receive data register */
		c = UART2_ReceiveData8();
		valid_sentence = EncodeNMEA( c );
	}
}

void Demo(void) {	
	
	u8 str_buffer[50];
	float valor;
	u16 tmp,barrapos,sinal;

	if ( valid_sentence ) {
		// Blink Debug LED
		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
		
		get_gps_data(&local_gps_data);
		
		valor = local_gps_data.pos.lat / 100.0;
		sprintf(str_buffer, "Lat %03.4f \0", valor);
		Puts(5, 15, str_buffer, FONT5x7, RGB(255,0,0), WHITE);
	
		valor = local_gps_data.pos.lon / 100.0;
		sprintf(str_buffer, "Lon %03.4f \0", valor);	
		Puts(5, 25, str_buffer, FONT5x7, RGB(255,0,0), WHITE);
		
		valor = local_gps_data.pos.alt;
		sprintf(str_buffer, "Alt %04.1f m \0", valor);	
		Puts(100, 15, str_buffer, FONT5x7, RGB(255,0,0), WHITE);
		
		valor = local_gps_data.pos.spd;
		sprintf(str_buffer, "Spd %03d Km/h \0", (int)valor);
		Puts(100, 25, str_buffer, FONT5x7, RGB(255,0,0), WHITE);
		
		valor = local_gps_data.status.siu;
		sprintf(str_buffer, "%02d\0", (int)valor);
		Puts(_LCD_WIDTH - 20, 2, str_buffer, FONT5x5, WHITE, BLACK);
	
		Puts(5, _LCD_HEIGHT - 8, local_gps_data.datetime.date, FONT5x7, WHITE, BLACK );
		Puts(_LCD_WIDTH - 55, _LCD_HEIGHT - 8, local_gps_data.datetime.time, FONT5x7, WHITE, BLACK);
		
		barrapos = 42;
		for(tmp = 0; tmp < 12; tmp++) {
			valor = local_gps_data.sat_inview[tmp].snr;
			if ( tmp_sat_inview[tmp] != valor ) {
				sinal = (int)valor / 2;
				if ( sinal > 38) sinal = 38;
				//DrawRect(11, barrapos - 2, 62, barrapos + 2, RGB(128,128,128));
				if ( tmp_sat_inview[tmp] > valor ) {
					FillRect(12 + sinal, barrapos - 1, 50, barrapos + 1, RGB(255,255,255));
				}
				if ( valor > 38 ) {
					FillRect(12, barrapos - 1, 12 + sinal, barrapos + 1, RGB(0,255,0));
				} else {
					FillRect(12, barrapos - 1, 12 + sinal, barrapos + 1, RGB(0,0,255));
				}
				tmp_sat_inview[tmp] = valor;
			}
			sprintf(str_buffer, "%02d", local_gps_data.sat_inview[tmp].prn);
			Puts(1, barrapos - 2, str_buffer, FONT5x5, WHITE, BLACK);
			barrapos += 6;
		}
	}
	
	sprintf(str_buffer, "%03i\0", Conversion_Value%25);
	Puts(13, 2, str_buffer, FONT5x5, WHITE, BLACK);
}


void DrawMain(void) {
	u16 tmp, barrapos;
	
	FillRect(0, 0, _LCD_WIDTH - 1, 10, BLACK);
	DrawIcon(1,0, batt);
	DrawIcon(165,0, wifi);
	FillRect(0, _LCD_HEIGHT - 1, _LCD_WIDTH - 1, _LCD_HEIGHT - 10, BLACK);
	
	barrapos = 42;
	for(tmp = 0; tmp < 12; tmp++) {
		DrawRect(11, barrapos - 2, 51, barrapos + 2, RGB(128,128,128));
		barrapos += 6;
	}
	
	//DrawRect(56, 40, 167, 116, RGB(200,200,200));
	
	//DrawCircle( 35,70,30, RGB(0,0,255) );
	//DrawCircle( 35,70,20, RGB(0,0,255) );
	//DrawCircle( 35,70,10, RGB(0,0,255) );
	//DrawCircle( 35,70,1, RGB(0,0,255) );
}

int main(void) {
	u8 ii = 0;
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
			
			
	// Init Key Input
	// D1
	// D2
	// D3
	
	GPIO_Init(GPIOD, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);
	
	//Delay(100);
	
	GLib_Init();
		
	
	ADC_Init();
	
	DecodeInit(); // Init the GPS decoding
	
	Delay(4000);
	Fill(RGB(255,255,255));
	
	UART2_ITConfig(UART2_IT_RXNE, ENABLE);
	
	enableInterrupts(); 
				
	DrawMain();
	//ShowMsh("System is not completed...\nWork hard !");
	// Infinite loop.
	for(;;) {
		// Blink Debug LED
		// GPIO_WriteReverse(GPIOD, GPIO_PIN_0);	
		Demo();		
		//Delay(500);	
	}
}