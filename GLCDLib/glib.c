//	
//		glib.c
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

#include "glib.h"
#include "s65.h"



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

// standard ascii 5x7 font
// defines ascii characters 0x20-0x7F (32-127)
extern const u8 Font5x7[] = {
        0x00, 0x00, 0x00, 0x00, 0x00,// (space)
        0x00, 0x00, 0x5F, 0x00, 0x00,// !
        0x00, 0x07, 0x00, 0x07, 0x00,// "
        0x14, 0x7F, 0x14, 0x7F, 0x14,// #
        0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
        0x23, 0x13, 0x08, 0x64, 0x62,// %
        0x36, 0x49, 0x55, 0x22, 0x50,// &
        0x00, 0x05, 0x03, 0x00, 0x00,// '
        0x00, 0x1C, 0x22, 0x41, 0x00,// (
        0x00, 0x41, 0x22, 0x1C, 0x00,// )
        0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
        0x08, 0x08, 0x3E, 0x08, 0x08,// +
        0x00, 0x50, 0x30, 0x00, 0x00,// ,
        0x08, 0x08, 0x08, 0x08, 0x08,// -
        0x00, 0x60, 0x60, 0x00, 0x00,// .
        0x20, 0x10, 0x08, 0x04, 0x02,// /
        0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
        0x00, 0x42, 0x7F, 0x40, 0x00,// 1
        0x42, 0x61, 0x51, 0x49, 0x46,// 2
        0x21, 0x41, 0x45, 0x4B, 0x31,// 3
        0x18, 0x14, 0x12, 0x7F, 0x10,// 4
        0x27, 0x45, 0x45, 0x45, 0x39,// 5
        0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
        0x01, 0x71, 0x09, 0x05, 0x03,// 7
        0x36, 0x49, 0x49, 0x49, 0x36,// 8
        0x06, 0x49, 0x49, 0x29, 0x1E,// 9
        0x00, 0x36, 0x36, 0x00, 0x00,// :
        0x00, 0x56, 0x36, 0x00, 0x00,// ;
        0x00, 0x08, 0x14, 0x22, 0x41,// <
        0x14, 0x14, 0x14, 0x14, 0x14,// =
        0x41, 0x22, 0x14, 0x08, 0x00,// >
        0x02, 0x01, 0x51, 0x09, 0x06,// ?
        0x32, 0x49, 0x79, 0x41, 0x3E,// @
        0x7E, 0x11, 0x11, 0x11, 0x7E,// A
        0x7F, 0x49, 0x49, 0x49, 0x36,// B
        0x3E, 0x41, 0x41, 0x41, 0x22,// C
        0x7F, 0x41, 0x41, 0x22, 0x1C,// D
        0x7F, 0x49, 0x49, 0x49, 0x41,// E
        0x7F, 0x09, 0x09, 0x01, 0x01,// F
        0x3E, 0x41, 0x41, 0x51, 0x32,// G
        0x7F, 0x08, 0x08, 0x08, 0x7F,// H
        0x00, 0x41, 0x7F, 0x41, 0x00,// I
        0x20, 0x40, 0x41, 0x3F, 0x01,// J
        0x7F, 0x08, 0x14, 0x22, 0x41,// K
        0x7F, 0x40, 0x40, 0x40, 0x40,// L
        0x7F, 0x02, 0x04, 0x02, 0x7F,// M
        0x7F, 0x04, 0x08, 0x10, 0x7F,// N
        0x3E, 0x41, 0x41, 0x41, 0x3E,// O
        0x7F, 0x09, 0x09, 0x09, 0x06,// P
        0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
        0x7F, 0x09, 0x19, 0x29, 0x46,// R
        0x46, 0x49, 0x49, 0x49, 0x31,// S
        0x01, 0x01, 0x7F, 0x01, 0x01,// T
        0x3F, 0x40, 0x40, 0x40, 0x3F,// U
        0x1F, 0x20, 0x40, 0x20, 0x1F,// V
        0x7F, 0x20, 0x18, 0x20, 0x7F,// W
        0x63, 0x14, 0x08, 0x14, 0x63,// X
        0x03, 0x04, 0x78, 0x04, 0x03,// Y
        0x61, 0x51, 0x49, 0x45, 0x43,// Z
        0x00, 0x00, 0x7F, 0x41, 0x41,// [
        0x02, 0x04, 0x08, 0x10, 0x20,// "\"
        0x41, 0x41, 0x7F, 0x00, 0x00,// ]
        0x04, 0x02, 0x01, 0x02, 0x04,// ^
        0x40, 0x40, 0x40, 0x40, 0x40,// _
        0x00, 0x01, 0x02, 0x04, 0x00,// `
        0x20, 0x54, 0x54, 0x54, 0x78,// a
        0x7F, 0x48, 0x44, 0x44, 0x38,// b
        0x38, 0x44, 0x44, 0x44, 0x20,// c
        0x38, 0x44, 0x44, 0x48, 0x7F,// d
        0x38, 0x54, 0x54, 0x54, 0x18,// e
        0x08, 0x7E, 0x09, 0x01, 0x02,// f
        0x08, 0x14, 0x54, 0x54, 0x3C,// g
        0x7F, 0x08, 0x04, 0x04, 0x78,// h
        0x00, 0x44, 0x7D, 0x40, 0x00,// i
        0x20, 0x40, 0x44, 0x3D, 0x00,// j
        0x00, 0x7F, 0x10, 0x28, 0x44,// k
        0x00, 0x41, 0x7F, 0x40, 0x00,// l
        0x7C, 0x04, 0x18, 0x04, 0x78,// m
        0x7C, 0x08, 0x04, 0x04, 0x78,// n
        0x38, 0x44, 0x44, 0x44, 0x38,// o
        0x7C, 0x14, 0x14, 0x14, 0x08,// p
        0x08, 0x14, 0x14, 0x18, 0x7C,// q
        0x7C, 0x08, 0x04, 0x04, 0x08,// r
        0x48, 0x54, 0x54, 0x54, 0x20,// s
        0x04, 0x3F, 0x44, 0x40, 0x20,// t
        0x3C, 0x40, 0x40, 0x20, 0x7C,// u
        0x1C, 0x20, 0x40, 0x20, 0x1C,// v
        0x3C, 0x40, 0x30, 0x40, 0x3C,// w
        0x44, 0x28, 0x10, 0x28, 0x44,// x
        0x0C, 0x50, 0x50, 0x50, 0x3C,// y
        0x44, 0x64, 0x54, 0x4C, 0x44,// z
        0x00, 0x08, 0x36, 0x41, 0x00,// {
        0x00, 0x00, 0x7F, 0x00, 0x00,// |
        0x00, 0x41, 0x36, 0x08, 0x00,// }
        0x08, 0x08, 0x2A, 0x1C, 0x08,// ->
        0x08, 0x1C, 0x2A, 0x08, 0x08 // <-
};

extern const u8 batt[] = {
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,0,
	1,0,0,0,0,0,0,0,1,0,
	1,0,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,1,0,
	1,1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
	};

extern const u8 wifi[] = {
	0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,1,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,1,1,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,1,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
	};

	
const u16 pot10[4] ={10,100,1000,10000};

void GLib_Init() {	
	LCD_Init();
	FillM();
	Box(30,50,140,65);
	Puts(35, 55, "MiniOS is Ready !", FONT5x7, RGB(255,255,255), RGB(0,160,0));
}


void Fill(u16 color) {
	u16 i;
	
	LCD_SetArea(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
	
	LCD_SendCMD(RAMWR);
	
	// start data transmission  
	for (i=0; i<LCD_WIDTH*LCD_HEIGHT; i++) {
		LCD_SendDAT16(color);
	}
	
}

void FillM() {
	u16 i,ii;
	u16 color1 = RGB(255,255,255), color2 = RGB(100,100,100);
	u8 b = 0;
	LCD_SetArea(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
	
	LCD_SendCMD(RAMWR);
	
	// start data transmission  
	for (i = 0; i < LCD_WIDTH; i++) {
		for (ii = 0; ii < LCD_HEIGHT / 2; ii++) {
			if ( b == 0 ) {
				LCD_SendDAT16(color1);
				LCD_SendDAT16(color2);								
			} else {
				LCD_SendDAT16(color2);
				LCD_SendDAT16(color1);				
			}
		}
		if ( b == 0 ) b = 1;
		else b = 0;
	}

}



/*
*	Draw a Char
*/
u8 DrawChar5x5(u8 x, u8 y, char character, u16 color, u16 bgcolor) {
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


/*
*	Draw a Char
*/
u8 DrawChar5x7(u8 x, u8 y, char character, u16 color, u16 bgcolor) {
	u16 fontHeight, i, j, fontWidth, fontPos = 0;	
		
	if ( character < 32 ) return;
	if ( character > 127 ) return;
	
	fontWidth = 5;
	fontHeight = 7;
	
	fontPos = ( character - 32 ) * fontWidth;
	
	LCD_SetArea(x, y, x + fontWidth + 1, y+fontHeight);
    LCD_SendCMD(RAMWR);
	for (i = 0; i < fontWidth; i++) {
		for (j = 0; j <= fontHeight; j++) {      
			if ( ValBit( Font5x7[ fontPos + i ], j ) ) {				
				LCD_SendDAT16(color);
			} else {
				LCD_SendDAT16(bgcolor);
			}
		}	   
	}
	for (j = 0; j <= fontHeight; j++) { 
		LCD_SendDAT16(bgcolor);
	}
	fontWidth++;
	return x + fontWidth;
}

unsigned int Puts(u8 x, u8 y, const char *s, u8 font, u16 color, u16 bgcolor) {
	char c;

	while(*s) {
		c = *s++;
		if(c >= 0x20) {
			if ( font == FONT5x5 ) {
				x = DrawChar5x5(x, y, c, color, bgcolor);
			} else if ( font == FONT5x7 ) {
				x = DrawChar5x7(x, y, c, color, bgcolor);
			}
			if(x > LCD_WIDTH) {
				break;
			}
		}
	}
	return x;
}


void PutLinebr(u8 x, u8 y, const char *s, u16 color, u16 bgcolor) {
	unsigned int i, start_x=x, font_height, wlen, llen;
	char c;
	const char *wstart;
	
	font_height = 5;
	
	FillRect(0, y, x-1, (y+font_height), bgcolor); //clear before text
	
	llen   = (LCD_WIDTH-x)/8;
	wstart = s;
	while(*s) {
		c = *s++;
		if(c == '\n') {//new line
			FillRect(x, y, (LCD_WIDTH-1), (y+font_height), bgcolor); //clear after text
			x  = start_x;
			y += font_height+1;
			FillRect(0, y, x-1, (y+font_height), bgcolor); //clear before text
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
	
					FillRect(x, y, (LCD_WIDTH-1), (y+font_height), bgcolor); //clear after text
					x  = start_x;
					y += font_height+1;
					FillRect(0, y, x-1, (y+font_height), bgcolor); //clear before text
				} else {
					wlen = (s-wstart);
					if(wlen > llen) {//word too long
	
						FillRect(x, y, (LCD_WIDTH-1), (y+font_height), bgcolor); //clear after text
						x  = start_x;
						y += font_height+1;
						FillRect(0, y, x-1, (y+font_height), bgcolor); //clear before text
						x = DrawChar5x5(x, y, c, color, bgcolor);
					} else {
						FillRect(x-(wlen*8), y, (LCD_WIDTH-1), (y+font_height), bgcolor); //clear after text
						x  = start_x;
						y += font_height+1;
						FillRect(0, y, x-1, (y+font_height), bgcolor); //clear before text
						s = wstart;
					}
				}
			} else {
				x = i;
			}
		}
	}
	
	FillRect(x, y, (LCD_WIDTH-1), (y+font_height), bgcolor); //clear after text
	
}

void FillRect(u8 x0, u8 y0, u8 x1, u8 y1, u16 color) {
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

void DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, u16 color) {
	
	int dx, dy, dx2, dy2, stepx, stepy, err;

	if((x0 == x1) ||
		(y0 == y1)) //horizontal or vertical line
	{
		FillRect(x0, y0, x1, y1, color);
	} else {
		//calculate direction
		dx = x1 - x0;
		dy = y1 - y0;
		if(dx < 0) { dx = -dx; stepx = -1; } else { stepx = +1; }
		if(dy < 0) { dy = -dy; stepy = -1; } else { stepy = +1; }
		dx2 = dx << 1;
		dy2 = dy << 1;
		//draw line
		//LCD_SetArea(0, 0, (LCD_WIDTH-1), (LCD_HEIGHT-1));
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

void DrawRect(u8 x0, u8 y0, u8 x1, u8 y1, u16 color) {
	FillRect(x0, y0, x0, y1, color);
	FillRect(x0, y1, x1, y1, color);
	FillRect(x1, y0, x1, y1, color);
	FillRect(x0, y0, x1, y0, color);
}


void Box( u8 x0, u8 y0, u8 x1, u8 y1 ) {
	u16 linecolor, backcolor;
	linecolor = RGB(255,255,255);
	backcolor = RGB(0,160,0);
	FillRect(x0 + 1, y0 + 1, x1 - 1, y1 - 1, backcolor);
	DrawRect(x0, y0, x1, y1, linecolor);
	
}

/*
* 	draw a circle 
*	algorithm by Jack Bresenham
*
*/
void DrawCircle(u8 X1, u8 Y1, u8 Radius, u16 color) 
{	int f;
	int ddF_x;
	int ddF_y;
	int x;
	int y;
	
	f = 1 - Radius;
	ddF_x = 0;
	ddF_y = -2 * Radius;
	x = 0;
	y = Radius;

	LCD_DrawPixel(X1, Y1 + Radius, color);
	LCD_DrawPixel(X1, Y1 - Radius, color);
	LCD_DrawPixel(X1 + Radius, Y1, color);
	LCD_DrawPixel(X1 - Radius, Y1, color);

	while (x < y) 
	{	if (f >= 0) 
		{	y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x + 1;
		
		LCD_DrawPixel(X1 + x, Y1 + y, color);
		LCD_DrawPixel(X1 - x, Y1 + y, color);
		LCD_DrawPixel(X1 + x, Y1 - y, color);
		LCD_DrawPixel(X1 - x, Y1 - y, color);
		LCD_DrawPixel(X1 + y, Y1 + x, color);
		LCD_DrawPixel(X1 - y, Y1 + x, color);
		LCD_DrawPixel(X1 + y, Y1 - x, color);
		LCD_DrawPixel(X1 - y, Y1 - x, color);
	}
}

void ShowMsh( const char *s ) {
	u16 linecolor, backcolor;
	linecolor = RGB(255,255,255);
	backcolor = RGB(0,160,0);
	Box(30, 20, 140, 60);
	PutLinebr(33, 23, s, linecolor, backcolor);
	//WaitKey(KEY_ENTER);
	Fill(RGB(0,0,0));
}

void Scroll(u8 scroll) {
	LCD_Scroll(scroll);
}




void u16tostr( u16 val,  char *strg, u8 nzero) {
/*
  convert unsigned int16 value to a character string
  stringlength has to be 6 character 65536 + the final 0
  
  if nzero==0, no leading zeros
  if nzero==1, with leading zeros  
*/
	u8 dec,idx=4,odx=0;
	
	do {
		idx--;
		for (dec=0; val>=pot10[idx]; val-=pot10[idx]) {
			dec++;
		}
		nzero+=dec;
		if (nzero) strg[odx++] = '0' + dec;
	} while(idx);
	strg[odx++]='0'+val;
	strg[odx]=0;
}


void DrawIcon(u8 x, u8 y, u8 icon[]) {
	u8 i,ii;
	u16 color1 = WHITE, color2 = BLACK;
	
	LCD_SetArea(x, y, x + 10, y + 10);
	
	LCD_SendCMD(RAMWR);
	
	// start data transmission  
	for (i = 0; i < 10; i++) {
		for (ii = 0; ii <= 10; ii++) {
			if ( icon[(ii*10) + i] == 1 ) {
				LCD_SendDAT16(color1);
			} else {
				LCD_SendDAT16(color2);
			}			
		}
	}

}
