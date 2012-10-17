/* Includes ------------------------------------------------------------------*/
#include "s65.h"
#include "key.h"


// Font 5x5 ( largura da fonte não é fixa )
extern const u8 Font5x5[289] = {
0, 				// Code for char Espaço   
23, 			// Code for char !  
3,0,3, 			// Code for char "  
10,31,10,31,10, // Code for char #  
4,14,27,10, 	// Code for char $  
9,4,2,9, 		// Code for char %  
10,21,17, 		// Code for char &  
3,4, 			// Code for char '  
14,27,17, 		// Code for char (  
17,27,14, 		// Code for char )  
21,14,14,21, 	// Code for char *  
4,14,4, 		// Code for char +  
16,8, 			// Code for char ,  
4,4,4, 			// Code for char -  
16, 			// Code for char .  
16,12,3, 		// Code for char "/"  
31,17,31, 		// Code for char 0  
18,31,16, 		// Code for char 1  
25,21,18, 		// Code for char 2  
17,21,14, 		// Code for char 3  
14,8,30, 		// Code for char 4  
23,21,9, 		// Code for char 5  
31,21,29, 		// Code for char 6  
18,10,6, 		// Code for char 7  
10,21,10, 		// Code for char 8  
23,21,31, 		// Code for char 9  
10, 			// Code for char :  
16,10, 			// Code for char ;  
4,10,17, 		// Code for char <  
10,10,10, 		// Code for char =  
17,10,4, 		// Code for char >  
1,21,2, 		// Code for char ?  
14,19,22, 		// Code for char @  
30,9,30, 		// Code for char A  
31,21,10, 		// Code for char B  
14,17,17, 		// Code for char C  
31,17,14, 		// Code for char D  
31,21,21, 		// Code for char E  
31,5,5, 		// Code for char F  
14,17,29, 		// Code for char G  
31,4,31, 		// Code for char H  
17,31,17, 		// Code for char I  
24,16,31, 		// Code for char J  
31,4,27, 		// Code for char K  
31,16,16, 		// Code for char L  
31,2,4,2,31, 	// Code for char M  
31,1,30, 		// Code for char N  
14,17,14, 		// Code for char O  
31,5,2, 		// Code for char P  
31,17,31,16, 	// Code for char Q  
31,5,26, 		// Code for char R  
18,21,9, 		// Code for char S  
1,31,1, 		// Code for char T  
15,16,15, 		// Code for char U  
31,8,7, 		// Code for char V  
15,16,12,16,15, // Code for char W  
27,4,27, 		// Code for char X  
3,28,3, 		// Code for char Y  
25,21,19, 		// Code for char Z  
31,17,17, 		// Code for char [  
1,6,24, 		// Code for char "\"  
17,17,31, 		// Code for char ]  
2,1,2, 			// Code for char ^  
16,16,16, 		// Code for char _  
1,2, 			// Code for char `  
8,26,28, 		// Code for char a  
31,20,8, 		// Code for char b  
12,18,18, 		// Code for char c  
8,20,31, 		// Code for char d  
12,22,20, 		// Code for char e  
30,5,1, 		// Code for char f  
20,26,12, 		// Code for char g  
31,4,24, 		// Code for char h  
29, 			// Code for char i  
16,16,13, 		// Code for char j  
31,8,20, 		// Code for char k  
17,31,16, 		// Code for char l  
30,2,12,2,28, 	// Code for char m  
30,2,28, 		// Code for char n  
12,18,12, 		// Code for char o  
30,10,4, 		// Code for char p  
4,10,30, 		// Code for char q  
30,4,2, 		// Code for char r  
20,22,10, 		// Code for char s  
4,30,4, 		// Code for char t  
14,16,14, 		// Code for char u  
30,8,6, 		// Code for char v  
14,16,12,16,14, // Code for char w  
18,12,18, 		// Code for char x  
22,20,14, 		// Code for char y  
26,22,22, 		// Code for char z  
4,14,17, 		// Code for char {  
27, 			// Code for char |  
17,14,4, 		// Code for char }  
1,2,1,2, 		// Code for char ~  
14,14,14,14 	// Code for char " "
};

// Char position and char size table
//
// CharSize = Font5x5WidthPos["2"] - Font5x5WidthPos["2" - 1]
// CharPos  = Font5x5WidthPos["2" - 1]
//
extern const u16 Font5x5WidthPos[96] = {
1,2,5,10,14,18,21,23,26,29,33,36,38,41,
42,45,48,51,54,57,60,63,66,69,72,75,76,
78,81,84,87,90,93,96,99,102,105,108,111,
114,117,120,123,126,129,134,137,140,143,
147,150,153,156,159,162,167,170,173,176,
179,182,185,188,191,193,196,199,202,205,
208,211,214,217,218,221,224,227,232,235,
238,241,244,247,250,253,256,259,264,267,
270,273,276,277,280,284,289
};


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
	while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);

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


void LCD_SetArea(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) {

	LCD_SendCMD(SD_CSET);     //column address set 
	LCD_SendDAT8(0x08+y0); //start column
	LCD_SendDAT8(0x01);    //start column
	LCD_SendDAT8(0x08+y1); //end column
	LCD_SendDAT8(0x01);    //end column
	
	LCD_SendCMD(SD_PSET); //page address set 
	LCD_SendDAT8(x0);  //start page
	LCD_SendDAT8(x1);  //end page
  
}

static unsigned char disctl[9] = {0x4C, 0x01, 0x53, 0x00, 0x02, 0xB4, 0xB0, 0x02, 0x00};   

void LCD_Init(void) {

	int i;	
	
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

	//LCD_SetArea(0, 0, DISP_H-1, DISP_W-1);

	LCD_Fill(RGB(0,0,0));
	LCD_Puts(5, 5, "Ready !", RGB(255,255,255), RGB(0,0,0));
	LCD_SendCMD(DISON);   	
}

void LCD_Fill(u16 color) {
	u16 i;
	
	LCD_SetArea(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
	
	LCD_SendCMD(RAMWR);
	
	// start data transmission  
	for (i=0; i<LCD_WIDTH*LCD_HEIGHT; i++) {
		LCD_SendDAT16(color);
	}

}


void LCD_SetCursor(unsigned int x, unsigned int y) {

	LCD_SendCMD(SD_CSET);    //column address set 
	LCD_SendDAT8(0x08+y); //start column
	LCD_SendDAT8(0x01);   //start column
	LCD_SendDAT8(0x08+y); //end column
	LCD_SendDAT8(0x01);   //end column
	
	LCD_SendCMD(SD_PSET); //page address set 
	LCD_SendDAT8(x);   //start page
	LCD_SendDAT8(x);   //end page

}

void LCD_DrawPixel(unsigned int x, unsigned int y, unsigned int color)
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


/*
*	Draw a Char
*/
u8 DrawChar5x5(u8 x, u8 y, char character, unsigned int color, unsigned int bgcolor) {
	u16 fontIndex, i, j, fontWidth, fontPos = 0;	
	
	fontIndex = character-32;
	if ( character < 32 ) return;
	if ( fontIndex > 96 ) return;
	
	if ( fontIndex > 0 ) {
		fontPos = Font5x5WidthPos[ fontIndex - 1];
	}
	fontWidth = Font5x5WidthPos[ fontIndex ] - fontPos;
	
	LCD_SetArea(x, y, x+fontWidth+1, y+5);
    LCD_SendCMD(RAMWR);
	for (i = 0; i < fontWidth; i++) {
		for (j = 0; j <= 5; j++) {      
			if ( ValBit( Font5x5[ fontPos + i ], j ) ) {				
				LCD_SendDAT16(color);
			} else {
				LCD_SendDAT16(bgcolor);
			}
		}	   
	}
	for (j = 0; j <= 5; j++) { 
		LCD_SendDAT16(bgcolor);
	}
	fontWidth++;
	return x + fontWidth;
}


unsigned int LCD_Puts(unsigned int x, unsigned int y, const char *s, unsigned int color, unsigned int bgcolor) {
	char c;

	while(*s) {
		c = *s++;
		if(c >= 0x20) {
			x = DrawChar5x5(x, y, c, color, bgcolor);
			if(x > LCD_WIDTH) {
				break;
			}
		}
	}
	return x;
}


void LCD_PutLinebr(unsigned int x, unsigned int y, const char *s, unsigned int color, unsigned int bgcolor) {
	unsigned int i, start_x=x, font_height, wlen, llen;
	char c;
	const char *wstart;
	
	font_height = 5;
	
	LCD_FillRect(0, y, x-1, (y+font_height), bgcolor); //clear before text
	
	llen   = (LCD_WIDTH-x)/8;
	wstart = s;
	while(*s) {
		c = *s++;
		if(c == '\n') {//new line
			LCD_FillRect(x, y, (LCD_WIDTH-1), (y+font_height), bgcolor); //clear after text
			x  = start_x;
			y += font_height+1;
			LCD_FillRect(0, y, x-1, (y+font_height), bgcolor); //clear before text
			continue;
		}
	
		if(c == ' ') {//start of a new word
			wstart = s;
		}
	
		if((c == ' ') && (x == start_x)) {
			//do nothing
		} else if(c >= 0x20) {
			i = DrawChar5x5(x, y, c, color, bgcolor);
			if(i > LCD_WIDTH) {//new line
	  
				if(c == ' ') {//do not start with space
	
					LCD_FillRect(x, y, (LCD_WIDTH-1), (y+font_height), bgcolor); //clear after text
					x  = start_x;
					y += font_height+1;
					LCD_FillRect(0, y, x-1, (y+font_height), bgcolor); //clear before text
				} else {
					wlen = (s-wstart);
					if(wlen > llen) {//word too long
	
						LCD_FillRect(x, y, (LCD_WIDTH-1), (y+font_height), bgcolor); //clear after text
						x  = start_x;
						y += font_height+1;
						LCD_FillRect(0, y, x-1, (y+font_height), bgcolor); //clear before text
						x = DrawChar5x5(x, y, c, color, bgcolor);
					} else {
						LCD_FillRect(x-(wlen*8), y, (LCD_WIDTH-1), (y+font_height), bgcolor); //clear after text
						x  = start_x;
						y += font_height+1;
						LCD_FillRect(0, y, x-1, (y+font_height), bgcolor); //clear before text
						s = wstart;
					}
				}
			} else {
				x = i;
			}
		}
	}
	
	LCD_FillRect(x, y, (LCD_WIDTH-1), (y+font_height), bgcolor); //clear after text
	
}

void LCD_FillRect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color) {
	unsigned int wh, tmp;

	if(x0 > x1) {
		tmp = x0;
		x0  = x1;
		x1  = tmp;
	}
  
	if(y0 > y1) {
		tmp = y0;
		y0  = y1;
		y1  = tmp;
	}

	if((x1 >= LCD_WIDTH) ||
		(y1 >= LCD_HEIGHT)) {
		return;
	}

	LCD_SetArea(x0, y0, x1, y1);

	LCD_SendCMD(RAMWR);
	for(wh=((1+(x1-x0))*(1+(y1-y0))); wh!=0; wh--) {
		LCD_SendDAT16(color);
	}

}

void LCD_DrawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color) {
	
	int dx, dy, dx2, dy2, stepx, stepy, err;

	if((x0 == x1) ||
		(y0 == y1)) //horizontal or vertical line
	{
		LCD_FillRect(x0, y0, x1, y1, color);
	} else {
		//calculate direction
		dx = x1 - x0;
		dy = y1 - y0;
		if(dx < 0) { dx = -dx; stepx = -1; } else { stepx = +1; }
		if(dy < 0) { dy = -dy; stepy = -1; } else { stepy = +1; }
		dx2 = dx << 1;
		dy2 = dy << 1;
		//draw line
		LCD_SetArea(0, 0, (LCD_WIDTH-1), (LCD_HEIGHT-1));
		LCD_DrawPixel(x0, y0, color);
		if(dx > dy) {
			err = dy2 - dx;
			while(x0 != x1) {
				if(err >= 0) {
					err -= dx2;
					y0  += stepy;
				}
				err += dy2;
				x0  += stepx;
				LCD_DrawPixel(x0, y0, color);
			}
		} else {
			err = dx2 - dy;
			while(y0 != y1) {
				if(err >= 0) {
					err -= dy2;
					x0  += stepx;
				}
				err += dx2;
				y0  += stepy;
				LCD_DrawPixel(x0, y0, color);
			}
		}
	}
}

void LCD_DrawRect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int color) {
	LCD_FillRect(x0, y0, x0, y1, color);
	LCD_FillRect(x0, y1, x1, y1, color);
	LCD_FillRect(x1, y0, x1, y1, color);
	LCD_FillRect(x0, y0, x1, y0, color);
}


void Box( unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1 ) {
	unsigned int linecolor, backcolor;
	linecolor = RGB(255,255,255);
	backcolor = RGB(200,200,200);
	LCD_FillRect(x0, y0, x1, y1, backcolor);
	LCD_DrawRect(x0, y0, x1, y1, linecolor);
	
}

void ShowMsh( const char *s ) {
	unsigned int linecolor, backcolor;
	linecolor = RGB(255,255,255);
	backcolor = RGB(200,200,200);
	Box(30, 20, 140, 60);
	LCD_Puts(33, 23, s, linecolor, backcolor);
	WaitKey(KEY_ENTER);
	LCD_Fill(RGB(0,0,0));
}