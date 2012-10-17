/* 
TEA5767
 
///////////////////////////////////////////////////////////////////////////////////////////////// 
TEA5767 FEATURES 
FM band: 
 
US/Europe  (87.5 to 108 MHz) 
Japanese   (76 to 91MHz) 
 
TEA5767 GENERAL DESCRIPTION 
The TEA5767HN is a single-chip electronically tuned FM 
stereo radio for low-voltage application with fully integrated 
IF selectivity and demodulation. The radio is completely 
adjustment-free and only requires a minimum of small and 
low cost external components. The radio can be tuned to 
the European, US and Japanese FM bands. 
 
*/ 
#ifndef _TEA5767_INCLUDED_ 
#define _TEA5767_INCLUDED_  
 
#include <stdio.h>
#include "glcdlib.h"

unsigned char radio_write_data [5]; // The data to be written TEA5767 
unsigned char radio_read_data [5]; // TEA5767 read out the state of 
unsigned int pll = 0; 
unsigned long frequency = 0;

#define max_freq 108000 
#define min_freq 87500 


void init_tea5767( void );
void search (bool mode);
void get_frequency (void);


void radio_write (void) { 
	unsigned char i; 
	/* Send STRAT condition */
	I2C_GenerateSTART(ENABLE);
	
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));  
  
	/* Send address for write */
	I2C_Send7bitAddress(0xC0, I2C_DIRECTION_TX); // TEA5767 Write Address 
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
	
	for (i = 0; i < 6; i++ ) { 
		/* Send the byte to be written */
		I2C_SendData(radio_write_data[i]);
		//while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	}
	
	/* Send STOP condition */
	I2C_GenerateSTOP(ENABLE);
} 


void radio_read (void) { 
	unsigned char NumByteToRead = 5, i = 0; 
	unsigned char temp_l, temp_h; 

	I2C_GenerateSTART(ENABLE);
	/* Test on EV5 and clear it */
	//while (!I2C_CheckEvent(I2C_EVENT_MASTER_START_SENT));
	
	/* Send slave Address in read direction & wait event */
	I2C_Send7bitAddress(0xC1, I2C_DIRECTION_RX);
	/* Test on EV6 and clear it */
	//while (!I2C_CheckEvent(I2C_EVENT_MASTER_ADDRESS_ACKED));
	//I2C_ClearFlag(I2C_FLAG_ADDRESSSENTMATCHED);
	
	/* While there is data to be read */
	 
	while( NumByteToRead ) {
		//if(I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED)) {
			radio_read_data[i] = I2C_ReceiveData();
			NumByteToRead--;
			i++;
		//}	
	}

	/* Send STOP condition */
	I2C_GenerateSTOP(ENABLE);
	
	temp_l = radio_read_data [1]; 
	temp_h = radio_read_data [0]; 
	temp_h &= 0x3f; 
	pll = temp_h * 256 + temp_l; 
	get_frequency ();
} 



void get_pll (void) { 

	unsigned char hlsi; 
	unsigned int twpll = 0; 
	
	hlsi = radio_write_data[2] & 0x10; 
	if (hlsi == 0) {
		pll = (unsigned int) ((float) ((frequency + 225) * 4) / (float) 32.768); // frequency unit: k
	} else  {
		pll = (unsigned int) ((float) ((frequency - 225) * 4) / (float) 32.768); // frequency unit: k
	}
}


void get_frequency (void) {

	unsigned char hlsi; 
	unsigned int npll = 0; 

	npll = pll; 
	hlsi = radio_write_data[2] & 0x10; 
	if (hlsi == 0) { 
		frequency = (unsigned long) ((float) (npll) * (float) 8.192 - 225); // Frequency Unit: KHz 
	} else { 
		frequency = (unsigned long) ((float) (npll) * (float) 8.192 + 225); // Frequency Unit: KHz 
	}
}


// Manually set the frequency, mode = 1, +0.1 MHz; mode = 0:-0.1MHz, without regard to TEA5767 used to search places related to Taiwan: SM, SUD 
void search (bool mode) { 

	radio_read(); 
	if (mode) {
		frequency += 100; 
		if (frequency> max_freq) 
			frequency = min_freq; 
	} else { 
		frequency -= 100; 
		if (frequency <min_freq) 
			frequency = max_freq; 
	}
	
	get_pll (); 
	radio_write_data [0] = pll/256; 
	radio_write_data [1] = pll% 256; 
	radio_write_data [2] = 0x20; 
	radio_write_data [3] = 0x11; 
	radio_write_data [4] = 0x00; 
	radio_write (); 

}


void init_tea5767( void ) {
	
	radio_write_data[0] = 0b01101111;  //load 100MHz pll
	radio_write_data[1] = 0b10010011;  // away's low side injection
	radio_write_data[2] = 0b00100000;
	radio_write_data[3] = 0b10010000;                                                                  
	radio_write_data[4] = 0b00000000;
	
	radio_read_data[0] = radio_write_data[0];
	radio_read_data[1] = radio_write_data[1];
	
	radio_write (); 

}



void show_frequency (void) {

	u8 display_bit [10] = "000.00Mhz\0"; 
	
	display_bit [0] = frequency / 100000; 
	if (display_bit [0] == 0) display_bit [0] = 0x20; 
	else display_bit [0] += 0x30; 

	display_bit [1] = (frequency / 10000)% 10 + 0x30; 
	display_bit [2] = (frequency / 1000)% 10 + 0x30; 
	display_bit [4] = (frequency / 100)% 10 + 0x30; 
	display_bit [5] = (frequency / 10)% 10 + 0x30; 
	
	GotoXY( 0, 0 );
	PutText(display_bit, LCD_PIXEL_ON);


}

#endif
