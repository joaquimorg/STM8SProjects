//	
//		ledmatrix.c
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

#include "math.h"
#include "stm8s_it.h"
#include "ledmatrix.h"

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

#define FONT_WIDTH      5
#define FONT_HEIGHT     5

int CursorX = 0, CursorY = 0;

u8 MatrixData[8][8];
u8 data[16];
u8 contacoluna = 0;


@far @interrupt void Tim2Update_isr(void) {
    u8 i, ii;	

	//UpdateMatrix();
	
	//for(i = 0; i < 8; i++) {	
	//	contacoluna	= i;
		GPIO_Write(GPIOE, (7 - contacoluna) << 5);
		for(ii = 0; ii < 8; ii++) {
			if ( MatrixData[contacoluna][ii] == RED ) {
				GPIO_WriteHigh(GPIOD, GPIO_PIN_0);  
				GPIO_Write(GPIOB, 1 << ii);
			} else if ( MatrixData[contacoluna][ii] == GREEN ) {
				GPIO_WriteLow(GPIOD, GPIO_PIN_0);  
				GPIO_Write(GPIOB, 1 << ii);
			} else if ( MatrixData[contacoluna][ii] == YELLOW ) {
				GPIO_WriteHigh(GPIOD, GPIO_PIN_0);  
				GPIO_Write(GPIOB, 1 << ii);
				GPIO_WriteLow(GPIOD, GPIO_PIN_0);  
				GPIO_Write(GPIOB, 1 << ii);
			}			
		}				
	//}	
	contacoluna++;
	if ( contacoluna > 7 ) {
		contacoluna = 0;		
		//UpdateMatrix();
		//GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
	}
	GPIO_Write(GPIOB, 0);
	/*for (i = 0; i < 8; i++) {
	
		GPIO_Write(GPIOE, 1 << 5);	
	//if ( contacoluna < 8 ) {
		GPIO_WriteHigh(GPIOD, GPIO_PIN_0);  
		GPIO_Write(GPIOB, 255);
	//} else {
		//GPIO_WriteLow(GPIOD, GPIO_PIN_0); 
		//GPIO_Write(GPIOB, data[i + 7]);
	}*/
	
	TIM2_ClearFlag(TIM2_FLAG_UPDATE);
}


/*
*	Init GPIO Port B for Output
*/
void SetBPortOut( void ) {
	// Init Pin Port B - Matrix Data D0..D7
	GPIO_Init(GPIOB, 
			GPIO_PIN_0 | GPIO_PIN_1 | 
			GPIO_PIN_2 | GPIO_PIN_3 | 
			GPIO_PIN_4 | GPIO_PIN_5 | 
			GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST); // D0..D7
	
}

void InitMatrix(void) {
	
	// Init Pin Port E - Matrix Row
	GPIO_Init(GPIOE, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST); // 
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST); // 
	GPIO_Init(GPIOE, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST); // 
	GPIO_Init(GPIOE, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST); // 
	SetBPortOut();
	
	FillScr( BLACK );
	UpdateMatrix();
}


void DelayLed( int ms) {
	int i;
	while(ms--){ 
		i=1300;       //empirically determined fudge factor 16mhz
		while(i--); 
	} 
}

void FillScr( u8 v ) {   
	u8 i, ii;
	for(i = 0; i < 8; i++) {
		for(ii = 0; ii < 8; ii++) {
			MatrixData[i][ii] = v;
		}
	}
}

void UpdateMatrix(void) {
	s8 i, ii;
	
	for(i = 0; i < 8; i++) {			
		data[i] = 0;
		data[8 + i] = 0;
		for(ii = 0; ii < 8; ii++) {
			if ( MatrixData[i][ii] == RED ) {
				data[i] |= ( 1 << ii );
			} else if ( MatrixData[i][ii] == GREEN ) {
				data[8 + i] |= ( 1 << ii );
			} else if ( MatrixData[i][ii] == YELLOW ) {
				data[i] |= ( 1 << ii );
				data[8 + i] |= ( 1 << ii );
			}
			/*else {
				data[i] &= ~( 1 << ii );
			}*/
		}				
	}
	
	/*for(i = 0; i < 8; i++) {
		
		GPIO_Write(GPIOE, (7 - i) << 5);		
		GPIO_Write(GPIOB, data[i]);		
		
	}*/
}

void nextcolum( void ) {
	//DelayLed(2);
	GPIO_Write(GPIOE, (7 - contacoluna) << 5);	
	if ( contacoluna < 8 ) {
		GPIO_WriteHigh(GPIOD, GPIO_PIN_0);  
		GPIO_Write(GPIOB, data[contacoluna]);
	} else {
		GPIO_WriteLow(GPIOD, GPIO_PIN_0); 
		GPIO_Write(GPIOB, data[contacoluna]);
	}
	//DelayLed(1);
	contacoluna++;
	if ( contacoluna > 15 ) {
		contacoluna = 0;		
		UpdateMatrix();
		//GPIO_WriteReverse(GPIOD, GPIO_PIN_0);
	}
}

// Set the position of the cursor to print 
void GotoXY( int x, int y ) {	
	CursorX = x;
	CursorY = y;
}


void DrawPoint( int x, int y, u8 v ) {

	if ( ( (y >= 0) && ( y <= DISPLAY_HEIGHT) ) && ( (x >= 0) && ( x <= DISPLAY_WIDTH) ) ) { 

		MatrixData[x][y] = v;
	}
	
}


/*
*	Draw a Char
*/
u8 DrawChar5x5(int x, int y, char character, u8 v) {
	u16 fontIndex, i, j, fontWidth, fontPos = 0;	
	
	fontIndex = character-32;
	if ( character < 32 ) return;
	if ( fontIndex > 96 ) return;
	
	if ( fontIndex > 0 ) {
		fontPos = Font5x5WidthPos[ fontIndex - 1];
	}
	fontWidth = Font5x5WidthPos[ fontIndex ] - fontPos;
	
	for (i = 0; i < fontWidth; i++) {
		for (j = 0; j < FONT_HEIGHT; j++) {      
			
			if ( ValBit( Font5x5[ fontPos + i ], j ) ) {
				DrawPoint( x + i, y + j, v );
			} else {
				DrawPoint( x + i, y + j, BLACK );
			}
			
		}	   
	}

	
	for (j = 0; j < FONT_HEIGHT; j++) {
		DrawPoint( x + fontWidth, y + j, BLACK );
	}
	fontWidth += 1;
	return fontWidth;
}

/*
*	Draw a Char
*/
u8 DrawChar5x7(int x, int y, char character, u8 v) {
	u16 fontIndex, i, j, fontWidth, fontPos = 0;	
	
	fontIndex = character-32;
	if ( character < 32 ) return;
	if ( fontIndex > 96 ) return;
	
	if ( fontIndex > 0 ) {
		fontPos = fontIndex * 5;
	}
	fontWidth = 5;
	
	for (i = 0; i < fontWidth; i++) {
		for (j = 0; j < 7; j++) {      
			
			if ( ValBit( Font5x7[ fontPos + i ], j ) ) {
				DrawPoint( x + i, y + j, v );
			} else {
				DrawPoint( x + i, y + j, BLACK );
			}
			
		}	   
	}

	
	for (j = 0; j < 7; j++) {
		DrawPoint( x + fontWidth, y + j, BLACK );
	}
	fontWidth += 1;
	return fontWidth;
}

u8 CharSize(char character) {
	u16 fontIndex, fontWidth, fontPos = 0;	
	
	fontIndex = character-32;
	if ( character < 32 ) return;
	if ( fontIndex > 96 ) return;
	
	if ( fontIndex > 0 ) {
		fontPos = Font5x5WidthPos[ fontIndex - 1];
	}
	fontWidth = Font5x5WidthPos[ fontIndex ] - fontPos;
	
	return fontWidth;
}

void DrawText(int x, int y, char str[], u8 v) {
	int origin_X, fontWidth;

	/* Preserve the origin X, in case of a new line */
	origin_X = x;

	/* Continue through the string until we encounter a null character */
	while (*str != '\0') {
		/* If the character is a newline, then prepare our x and y
		 * coordinates for the next character on the new line. */
		//if (*str == '\n') {
			/* Reset x to its origin */
		//	x = origin_X;
			/* Move y one character down */
		//	y += FONT_HEIGHT + 1;

		//	str++;
		//	continue;
		//}

		fontWidth = DrawChar5x7(x, y, *str++, v);
		
		/* Add a 1-pixel spacing between the characters */
		x += fontWidth;

		/* In case we are putting this character out of bounds,
		 * move the character to the next line on the display */
		//if ((x+fontWidth) > DISPLAY_WIDTH) {
			/* Reset x to its origin */
		//	x = origin_X;
			/* Move y one character down */
		//	y += FONT_HEIGHT + 1;
		//}
		
	}	
	GotoXY( x, y );
}

void PutText(char str[], u8 v) {
	DrawText(CursorX, CursorY, str, v);
}