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

#ifdef USEFONT5x5
#include "font5x5.h"
#endif

#ifdef USEFONT8x8
#include "font8x8.h"
#endif


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

	
extern const u16 pot10[4] ={10,100,1000,10000};

unsigned int CursorX;
unsigned int CursorY;
u8 DisplayActive = 0;


// -------------------------------------------------------------------------
void GLib_Init() {	
	LCD_Init();
	CursorX = 0;
	CursorY = 12;
	DisplayActive = 0;
}

// -------------------------------------------------------------------------
void Fill(u16 color) {
	u16 i;
	
	if (DisplayActive == 1) return;
	DisplayActive = 1;
	
	LCD_SetArea(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
	
	LCD_SendCMD(RAMWR);
	
	// start data transmission  
	for (i=0; i<LCD_WIDTH*LCD_HEIGHT; i++) {
		LCD_SendDAT16(color);
	}
	DisplayActive = 0;
}

// -------------------------------------------------------------------------
void FillM(void) {
	u16 i,ii;
	u16 color1 = RGB(255,255,255), color2 = RGB(100,100,100);
	u8 b = 0;
	
	if (DisplayActive == 1) return;
	DisplayActive = 1;
	
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
	DisplayActive = 0;
}

// -------------------------------------------------------------------------
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

	if (DisplayActive == 1) return;
	DisplayActive = 1;
	
	LCD_SetArea(x0, y0, x1, y1);

	LCD_SendCMD(RAMWR);
	for(wh=((1+(x1-x0))*(1+(y1-y0))); wh!=0; wh--) {
		LCD_SendDAT16(color);
	}

	DisplayActive = 0;
}

// -------------------------------------------------------------------------
void DrawLine(u8 x0, u8 y0, u8 x1, u8 y1, u16 color) {
	
	int dx, dy, dx2, dy2, stepx, stepy, err;

	if (DisplayActive == 1) return;
	DisplayActive = 1;
	
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
	DisplayActive = 0;
}

// -------------------------------------------------------------------------
void DrawIcon(u8 x, u8 y, u8 icon[]) {
	u8 i,ii;
	u16 color1 = WHITE, color2 = BLACK;
	
	if (DisplayActive == 1) return;
	DisplayActive = 1;
	
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
	DisplayActive = 0;
}

// -------------------------------------------------------------------------
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


// -------------------------------------------------------------------------
// Set the position of the cursor to print 
void GotoXY( u16 x, u16 y ) {	
	CursorX = x;
	CursorY = y;
}


// -------------------------------------------------------------------------
void Cls(void) {
	FillRect(0, 11, LCD_WIDTH - 1, LCD_HEIGHT - 1, RGB(255,255,255));
	GotoXY( 0, 12 );
}

// -------------------------------------------------------------------------
u8 GetCharWidth(const char character) {
#ifdef USEFONT5x5
	u8 fontIndex, fontPos = 0;
	
	fontIndex = character - 32;
	if ( character < 32 ) return 0;
	if ( fontIndex > 96 ) return 0;
	
	if ( fontIndex > 0 ) {
		fontPos = Font5x5WidthPos[ fontIndex - 1];
	}
	return (Font5x5WidthPos[ fontIndex ] - fontPos);
#endif
#ifdef USEFONT8x8
	return 5;
#endif
}


// -------------------------------------------------------------------------
#ifdef USEFONT5x5
/*
*	Draw a Char 5x5
*/
u8 DrawChar5x5(u8 x, u8 y, const char character, u16 color, u16 bgcolor) {
	u16 i, j, fontWidth, fontPos = 0, fontIndex;	
	
	fontWidth = GetCharWidth(character);
	
	if (fontWidth == 0) return 0;
	
	fontIndex = character - 32;
	
	if ( fontIndex > 0 ) {
		fontPos = Font5x5WidthPos[ fontIndex - 1];
	}
	
	LCD_SetArea(x, y, x + fontWidth + 1, y + 5);
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
	return fontWidth;
}
#endif

// -------------------------------------------------------------------------
#ifdef USEFONT8x8
/*
*	Draw a Char 8x8
*/
u8 DrawChar8x8(u8 x, u8 y, const char character, u16 color, u16 bgcolor) {
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
	return fontWidth;
}
#endif

// -------------------------------------------------------------------------
unsigned int Puts(u8 x, u8 y, const char *s, u16 color, u16 bgcolor) {
	char c;

	if (DisplayActive == 1) return;
	DisplayActive = 1;
	
	while(*s) {
		c = *s++;
		if(c >= 0x20) {
			#ifdef USEFONT5x5
				x = x + DrawChar5x5(x, y, c, color, bgcolor);
			#endif
			#ifdef USEFONT8x8
				x = x + DrawChar8x8(x, y, c, color, bgcolor);
			#endif
			if(x > LCD_WIDTH) {
				break;
			}
		}
	}
	
	DisplayActive = 0;
	return x;
}

// -------------------------------------------------------------------------
void BackChar(const char c) {
	u8 charw;
	charw = GetCharWidth(c);
	CursorX = CursorX - charw - 1;
	FillRect(CursorX, CursorY, CursorX + charw, CursorY + 5, RGB(255,255,255));
	
}

// -------------------------------------------------------------------------
void DrawChar(char c, u16 color, u16 bgcolor) {
	u8 fontWidth;

	u8 x = CursorX;
	u8 y = CursorY;
	
	/* If the character is a newline, then prepare our x and y
	 * coordinates for the next character on the new line. */
	if (c == '\n') {
		/* Reset x to its origin */
		x = 0;
		/* Move y one character down */	
		if ((y + 8) >= LCD_HEIGHT) {
			y = 12;
			Cls();
		} else {
			y += 8;
		}
	} else { 
		if ((x + 6) > LCD_WIDTH) {
			x = 0;				
			if ((y + 8) >= LCD_HEIGHT) {
				y = 12;
				Cls();
			} else {
				y += 8;
			}
		}
		
		#ifdef USEFONT5x5
			fontWidth = DrawChar5x5(x, y, c, color, bgcolor);
		#endif
		#ifdef USEFONT8x8
			fontWidth = DrawChar8x8(x, y, c, color, bgcolor);
		#endif
		
		x += fontWidth;
	}
	GotoXY( x, y );
}

// -------------------------------------------------------------------------
void Write(const char *s, u16 color, u16 bgcolor) {
	char c;
	if (DisplayActive == 1) return;
	DisplayActive = 1;
	
	while(*s) {
		c = *s++;
		if(c >= 0x20) {
			DrawChar(c, color, bgcolor);
		}
	}
	DisplayActive = 0;
}