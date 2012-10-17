/* Includes ------------------------------------------------------------------*/
#include "s65.h"


void delay_ms( int ms) {
	int i;
	while(ms--){ 
		i=2600;       //empirically determined fudge factor 16mhz
		while(i--) {
			nop();
		}
	} 
}

/**
  * @brief Send a byte to LCD through the SPI peripheral
  * @param[in] DataToSend Data to be sent
  */
void LCD_SPISendByte(u8 DataToSend) {

	SPI_SendData(DataToSend);
	//while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);

}

void LCD_SendCMD(u8 Data) {
	
	LCD_RS_ENABLE(); //cmd
	LCD_CS_ENABLE();

	LCD_SPISendByte(Data);
	LCD_SPISendByte(0x00);

	LCD_CS_DISABLE();
	
}

void LCD_SendDAT8(u8 Data) {
	
	LCD_RS_DISABLE(); //data
	LCD_CS_ENABLE();
	
	LCD_SPISendByte(Data);
	LCD_SPISendByte(0x00);
	
	LCD_CS_DISABLE();
	
}

void LCD_SendDAT(u8 Data) {
	
	LCD_RS_DISABLE(); //data
	LCD_CS_ENABLE();
	
	LCD_SPISendByte(Data);
	
	LCD_CS_DISABLE();
	
}

void LCD_SendDAT16(u16 Data) {
	
	LCD_RS_DISABLE(); //data
	LCD_CS_ENABLE();
	
	LCD_SPISendByte(Data >> 8);
	LCD_SPISendByte(Data);
	
	LCD_CS_DISABLE();
	
}


void LCD_SetArea(u8 x0, u8 y0, u8 x1, u8 y1) {

	LCD_SendCMD(SD_CSET);   //column address set 
	LCD_SendDAT8(0x08+y0); 	//start column
	LCD_SendDAT8(0x01);    	//start column
	LCD_SendDAT8(0x08+y1); 	//end column
	LCD_SendDAT8(0x01);    	//end column
	
	LCD_SendCMD(SD_PSET); 	//page address set 
	LCD_SendDAT8(x0);  		//start page
	LCD_SendDAT8(x1);  		//end page
  
}

static unsigned char disctl[9] = {0x4C, 0x01, 0x53, 0x00, 0x02, 0xB4, 0xB0, 0x02, 0x00};   

void LCD_Init(void) {

	int i;	
	
	SPI_DeInit();
	/* Initialize SPI in Master mode */
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_HIGH, SPI_CLOCKPHASE_2EDGE, SPI_DATADIRECTION_1LINE_TX, SPI_NSS_SOFT, 0x07);
    SPI_Cmd(ENABLE);

	/* Set S65 ChipSelect RS RST pin in Output push-pull high level */
	GPIO_Init(S65_CS_PORT, S65_CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	
	GPIO_Init(S65_RS_RST_PORT, S65_RS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(S65_RS_RST_PORT, S65_RST_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	
	//reset
	LCD_CS_DISABLE();
	LCD_RS_DISABLE();
	LCD_RST_ENABLE();
	delay_ms(50);
	LCD_RST_DISABLE();
	delay_ms(50);
	
	LCD_SendCMD(DISOFF); //display off
	
	LCD_SendCMD(DATCTL);   
    LCD_SendDAT8(0x2B);  //565 mode, 0x2A=normal, 0x2B=180  

         
    LCD_SendCMD(DISCTL);   
    for (i=0; i<9; i++) {   
       LCD_SendDAT8(disctl[i]);   	  
    }   
	delay_ms(10);
   
    LCD_SendCMD(SLPOUT);   
   
	delay_ms(10);
	
	LCD_SendCMD(DISON);   	
}


void LCD_SetCursor(u8 x, u8 y) {

	LCD_SendCMD(SD_CSET);    //column address set 
	LCD_SendDAT8(0x08+y); //start column
	LCD_SendDAT8(0x01);   //start column
	LCD_SendDAT8(0x08+y); //end column
	LCD_SendDAT8(0x01);   //end column
	
	LCD_SendCMD(SD_PSET); //page address set 
	LCD_SendDAT8(x);   //start page
	LCD_SendDAT8(x);   //end page

}

void LCD_DrawPixel(u8 x, u8 y, u16 color)
{
	if((x >= LCD_WIDTH) ||
		(y >= LCD_HEIGHT))
	{
		return;
	}
	
	LCD_SetCursor(x, y);
	
	LCD_SendCMD(RAMWR);
	LCD_SendDAT16(color);

}

void LCD_Scroll(u8 scroll) {
	LCD_SendCMD(ASCSET);
	LCD_SendDAT8(0x00);
	LCD_SendDAT8(0xAF);
	LCD_SendDAT8(0xAF);
	LCD_SendDAT8(0x03);
  
	LCD_SendCMD(SCSTART);
	LCD_SendDAT8(scroll);
}
