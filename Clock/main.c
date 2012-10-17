//	
//		main.c
//      
//      Copyright 2011 joaquim <mail@joaquim.org>
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
//
//
//		Ping Pong Clock for 640x240 LCD
//

/*

	SED1335 Connector
	
	1	- D0	- B0
	2	- D1	- B1
	3	- D2	- B2
	4	- D3	- B3
	5	- D4	- B4
	6	- D5	- B5
	7	- D6	- B6
	8	- D7	- B7
	9	- A0	- D5
	10	- ~RD	- D6
	11	- ~WR	- D7
	12	- ~CS	- D4
	13 	- GND	- GND

*/

#include "stm8s.h"

#define LCD_DATA_PORT (GPIOB)
#define LCD_CMD_PORT (GPIOD)

#define LCD_A0_PIN  (GPIO_PIN_5)
#define LCD_RD_PIN  (GPIO_PIN_6)
#define LCD_WR_PIN  (GPIO_PIN_7)
#define LCD_CS_PIN  (GPIO_PIN_4)


#define LCD_CS_DISABLE()	GPIO_WriteHigh(LCD_CMD_PORT, LCD_CS_PIN)
#define LCD_CS_ENABLE()		GPIO_WriteLow(LCD_CMD_PORT, LCD_CS_PIN)

#define LCD_WR_DISABLE()	GPIO_WriteHigh(LCD_CMD_PORT, LCD_WR_PIN)
#define LCD_WR_ENABLE()		GPIO_WriteLow(LCD_CMD_PORT, LCD_WR_PIN)

#define LCD_RD_DISABLE()	GPIO_WriteHigh(LCD_CMD_PORT, LCD_RD_PIN)
#define LCD_RD_ENABLE()		GPIO_WriteLow(LCD_CMD_PORT, LCD_RD_PIN)

#define LCD_A0_DATA()		GPIO_WriteLow(LCD_CMD_PORT, LCD_A0_PIN)
#define LCD_A0_CMD()		GPIO_WriteHigh(LCD_CMD_PORT, LCD_A0_PIN)

// -----------------------------------------------------------------------------
//
// 640 / 8 = 80
// 240 / 8 = 30
// 80 * 240 = 0x4b00 = 19200
// 80 * 30  = 0x0960 = 2400


#define GLCD_WIDTH         640
#define GLCD_HEIGHT        240
#define GLCD_CARLINE       80

#define         SED1335_SYSTEM_SET      		0x40
#define         SED1335_SLEEP_IN                0x53
#define         SED1335_DISP_OFF                0x58
#define         SED1335_DISP_ON                 0x59
#define         SED1335_SCROLL                  0x44
#define         SED1335_CSRFORM                 0x5d
#define         SED1335_CGRAM_ADR      		 	0x5c
#define         SED1335_CSRDIR_U                0x4e
#define         SED1335_CSRDIR_D                0x4f
#define         SED1335_CSRDIR_L                0x4d
#define         SED1335_CSRDIR_R                0x4c
#define         SED1335_HDOT_SCR                0x5a
#define         SED1335_OVLAY                   0x5b
#define         SED1335_CSRW                    0x46
#define         SED1335_CSRR                    0x47
#define         SED1335_MWRITE                  0x42
#define         SED1335_MREAD                   0x43

#define         SED1335_SCR_WIDTH       		GLCD_WIDTH - 1
#define         SED1335_M0                      0
#define         SED1335_M1                      0
#define         SED1335_M2                      0
#define         SED1335_WS                      0
#define         SED1335_IV                      0
#define         SED1335_DR                      0
#define         SED1335_FX                      7
#define         SED1335_FY                      7
#define         SED1335_WF                      1
#define         SED1335_CR                      GLCD_CARLINE - 1
#define         SED1335_TCR                     GLCD_CARLINE + 3
#define         SED1335_LF                      GLCD_HEIGHT - 1
#define         SED1335_APL                     GLCD_CARLINE
#define         SED1335_APH                     0

#define         SED1335_LINES                   GLCD_HEIGHT / 8

#define         SED1335_SAD1L                   0
#define         SED1335_SAD1H                   0
#define         SED1335_SL1                     SED1335_LF + 1
#define         SED1335_SAD2L                   0x00 // Inicio do Segundo layer
#define         SED1335_SAD2H                   0x4b //
#define         SED1335_SL2                     SED1335_LF + 1
#define         SED1335_SAD3L                   0
#define         SED1335_SAD3H                   0
#define         SED1335_SAD4L                   0
#define         SED1335_SAD4H                   0

#define         SED1335_CRX                     8
#define         SED1335_CRY                     8
#define         SED1335_CM                      0

#define         SED1335_MX0                     1
#define         SED1335_MX1                     1
#define         SED1335_DM1                     1
#define         SED1335_DM2                     1
#define         SED1335_OV                      0

#define         SED1335_SAGL                    0
#define         SED1335_SAGH                    112
#define         SED1335_SCRD                    0
#define         SED1335_ON                   	4

#define         SED1335_TEXTSIZE                ((SED1335_SAD2H << 8) + SED1335_SAD2L)

#define         SED1335_GRAPHICSTART    		((SED1335_SAD2H << 8) + SED1335_SAD2L)

#define         SED1335_GRAPHICSIZE     		((SED1335_SL2+1) * (SED1335_SCR_WIDTH+1))>>3

#define         SED1335_SYS_P1                  0x10 | (SED1335_DR << 7) | (SED1335_IV << 5) | (SED1335_WS << 3) | (SED1335_M2 << 2) | (SED1335_M1 << 1) | SED1335_M0
#define         SED1335_SYS_P2                  0x00 | (SED1335_WF << 7) | SED1335_FX
#define         SED1335_CSRF_P2                 0x00 | (SED1335_CM << 7) | SED1335_CRY
#define         SED1335_OVLAY_P1                0x00 | (SED1335_OV << 4) | (SED1335_DM2 << 3) | (SED1335_DM1 << 2) | (SED1335_MX1 << 1) | SED1335_MX0


#define 		LAYER1	0x00
#define			LAYER2	SED1335_GRAPHICSTART

// -----------------------------------------------------------------------------

#define BALLSIZE 8

u16 ballx = 320;
u16 bally = 120;
u16 balloldx = 320;
u16 balloldy = 120;
u8 dirx = 1;
u8 diry = 1;

u8 player1y = 120;
u8 player1oldy = 120;

u8 player2y = 120;
u8 player2oldy = 120;

u8 BallSave[8];

u8 player1Score = 0;
u8 player2Score = 0;


int randseed = 1;


void Delay( int ms) {
	int i;
	while(ms--){ 
		i=2600;       //empirically determined fudge factor 16mhz
		while(i--); 
	} 
}


void setDataPortOUT( void ) {
// Init Pin Port B - LCD Data OUT D0..D7
	GPIO_Init(LCD_DATA_PORT, 
			GPIO_PIN_0 | GPIO_PIN_1 | 
			GPIO_PIN_2 | GPIO_PIN_3 | 
			GPIO_PIN_4 | GPIO_PIN_5 | 
			GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST); // D0..D7
}

void setDataPortIN( void ) {
// Init Pin Port B - LCD Data IN D0..D7
	GPIO_Init(LCD_DATA_PORT, 
			GPIO_PIN_0 | GPIO_PIN_1 | 
			GPIO_PIN_2 | GPIO_PIN_3 | 
			GPIO_PIN_4 | GPIO_PIN_5 | 
			GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT); // D0..D7
}


/*
*	Get Data from the LCD
*/
u8 getStatus( void ) {
	u8 data;
	setDataPortIN();
	LCD_WR_DISABLE();
	LCD_A0_DATA();	
	LCD_CS_ENABLE();
	LCD_RD_ENABLE();
	
	data = GPIO_ReadInputData(LCD_DATA_PORT);	
	
	//Delay(1);
	LCD_RD_DISABLE();
	LCD_CS_DISABLE();
	return data;
}

/*
*	Send command to LCD
*/
void sendCMD( u8 cmd ) {
	setDataPortOUT();
	LCD_RD_DISABLE();
	LCD_A0_CMD();
	LCD_CS_ENABLE();	
	LCD_WR_ENABLE();
	
	GPIO_Write(LCD_DATA_PORT, cmd);
	
	//Delay(1);
	LCD_WR_DISABLE();
	LCD_CS_DISABLE();
}

/*
*	Send Data to the LCD
*/
void sendData( u8 data ) {
	
	//while ((getStatus() & 0x40));
	setDataPortOUT();
	LCD_RD_DISABLE();
	LCD_A0_DATA();
	LCD_CS_ENABLE();	
	LCD_WR_ENABLE();
	
	GPIO_Write(LCD_DATA_PORT, data);	
	
	//Delay(1);
	LCD_WR_DISABLE();
	LCD_CS_DISABLE();
}

/*
*	Get Data from the LCD
*/
u8 getData( void ) {
	u8 data;
	setDataPortIN();
	LCD_WR_DISABLE();
	LCD_A0_CMD();	
	LCD_CS_ENABLE();
	LCD_RD_ENABLE();
	
	data = GPIO_ReadInputData(LCD_DATA_PORT);	
	
	//Delay(1);
	LCD_RD_DISABLE();
	LCD_CS_DISABLE();
	return data;
}


// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
void lcdPortInit(void) {
	u16 i;
	
	setDataPortOUT();
			
	// Init Pin Port D - LCD Control CS A0 WR RD
	GPIO_Init(LCD_CMD_PORT, LCD_WR_PIN, GPIO_MODE_OUT_PP_LOW_FAST); // WR - Low Active
	GPIO_Init(LCD_CMD_PORT, LCD_RD_PIN, GPIO_MODE_OUT_PP_LOW_FAST); // RD - Low Active
	GPIO_Init(LCD_CMD_PORT, LCD_A0_PIN, GPIO_MODE_OUT_PP_LOW_FAST); // A0 - Low = Data | High = Cmd
	GPIO_Init(LCD_CMD_PORT, LCD_CS_PIN, GPIO_MODE_OUT_PP_LOW_FAST); // CS - High = Enable | Low = Disable
	
	GPIO_Write(LCD_DATA_PORT, 0);
	LCD_CS_DISABLE();
	LCD_RD_DISABLE();
	LCD_WR_DISABLE();
	//LCD_A0_DATA();
	//LCD_CS_ENABLE();

	sendCMD(SED1335_SYSTEM_SET);
	sendData(SED1335_SYS_P1);
	sendData(SED1335_SYS_P2);
	sendData(SED1335_FY);
	sendData(SED1335_CR);
	sendData(SED1335_TCR);
	sendData(SED1335_LF);
	sendData(SED1335_APL);
	sendData(SED1335_APH);
	
	sendCMD(SED1335_SCROLL);
	sendData(SED1335_SAD1L);
	sendData(SED1335_SAD1H);
	sendData(SED1335_SL1);
	sendData(SED1335_SAD2L);
	sendData(SED1335_SAD2H);
	sendData(SED1335_SL2);
	sendData(SED1335_SAD3L);
	sendData(SED1335_SAD3H);
	sendData(SED1335_SAD4L);
	sendData(SED1335_SAD4H);
	
	sendCMD(SED1335_CSRFORM);
	sendData(SED1335_CRX);
	sendData(SED1335_CSRF_P2);
	
	sendCMD(SED1335_CGRAM_ADR);
	sendData(SED1335_SAGL);
	sendData(SED1335_SAGH);
	
	sendCMD(SED1335_CSRDIR_R);
	
	sendCMD(SED1335_HDOT_SCR);
	sendData(SED1335_SCRD);
	
	sendCMD(SED1335_OVLAY);
	sendData(SED1335_OVLAY_P1);
	
	sendCMD(SED1335_DISP_ON);
	sendData(SED1335_ON);

	randseed = getData();
}


void SetCursorAddress(u16 address) {
	sendCMD(SED1335_CSRW);
	sendData((u8)(address & 0x00FF));
	sendData((u8)(address >> 8));
}

void GraphicGoTo(u16 layer, u16 x, u16 y) {
	SetCursorAddress(layer + (y * GLCD_CARLINE) + x);
}


void ClearGraphic(void) {
	unsigned int i;
	SetCursorAddress(LAYER1);
	//sendCMD(SED1335_MWRITE);
	//for(i = 0; i < 2400; i++)
	//	sendData(0x00);
		
	//SetCursorAddress(LAYER2);
	sendCMD(SED1335_MWRITE);
	while (!(getStatus() & 0x40));
	for(i = 0; i < (GLCD_CARLINE * GLCD_HEIGHT); i++)
		sendData(0x00);
}


void SetPixel(u16 layer, u16 x, u16 y, u8 color) {
	u8 tmp = 0;
	u8 bitmask = 0;
	
	GraphicGoTo( layer, x >> 3, y );
	
	sendCMD(SED1335_MREAD);
	tmp = getData();
	
	bitmask = 0x80 >> (x % 8);
	if (color) {
		tmp |= bitmask; /* set it */
	} else {
		tmp &= ~bitmask; /* clear it */
	}
	
	GraphicGoTo( layer, x >> 3, y );
	sendCMD(SED1335_MWRITE);
	sendData(tmp);
}


/*
 * draw_line
 * algorithm by Jack Bresenham
*/

void DrawLine(int layer, int X1, int Y1, int X2, int Y2, u8 v) 
{	int dy;
	int dx;
	int StepX, StepY;
	int Fraction;
	
	dy = Y2 - Y1;
	dx = X2 - X1;

	if (dy < 0)
	{	dy = -dy; 
		StepY = -1;
	}
	else
		StepY = 1;
		
	if (dx < 0) 
	{	dx = -dx; 
		StepX = -1; 
	}
	else
		StepX = 1;

	dy <<= 1; 									// dy is now 2*dy
	dx <<= 1; 									// dx is now 2*dx
	SetPixel( layer, X1, Y1, v );

	if (dx > dy) 
	{	Fraction = dy - (dx >> 1); 				// same as 2*dy - dx
		while (X1 != X2) 
		{	if (Fraction >= 0) 
			{	Y1 += StepY;
				Fraction -= dx; 				// same as fraction -= 2*dx
			}

			X1 += StepX;
			Fraction += dy; 					// same as fraction -= 2*dy
			SetPixel( layer, X1, Y1, v );
		}
	} 
	
	else 
	{	Fraction = dx - (dy >> 1);
		while (Y1 != Y2) 
		{	if (Fraction >= 0) 
			{	X1 += StepX;
				Fraction -= dy;
			}

			Y1 += StepY;
			Fraction += dx;
			SetPixel( layer, X1, Y1, v );
		}
	}
}


const u8 BallIcon[8] = {
0x18,
0x7e,
0x7e,
0xff,
0xff,
0x7e,
0x7e,
0x18
};

void drawBall(u16 x, u16 y, u8 color) {
	u8 i,ii;
	//BallSave
	if (color == 1) {
		for (i=0; i < BALLSIZE; i++) {
			GraphicGoTo( LAYER1, x >> 3, (y + i) - (BALLSIZE / 2) );
			sendCMD(SED1335_MREAD);
			BallSave[i] = getData();		
		}
	}
	for (i=0; i < BALLSIZE; i++) {
		GraphicGoTo( LAYER1, x >> 3, (y + i) - (BALLSIZE / 2) );
		sendCMD(SED1335_MWRITE);
		while (!(getStatus() & 0x40));
		if (color == 0) sendData(BallSave[i]);
		else sendData(BallIcon[i]);
	}

}

void drawPlayer(u16 x, u16 y, u8 color) {
	u8 i,ii;
	if (y > 217) y = 217;
	if (y < 24) y = 24;
	/*for (i=0; i < BALLSIZE; i++) {
		for (ii=0; ii < 40; ii++) {
			SetPixel( LAYER1, (x + i) - (BALLSIZE / 2) , (y + ii) - 20, color );
		}
	}*/
	ii = 40;
	for (i=0; i < ii; i++) {
		GraphicGoTo( LAYER1, x >> 3, (y + i) - (ii / 2) );
		sendCMD(SED1335_MWRITE);
		while (!(getStatus() & 0x40));
		sendData(color);
	}
}

const u8 NumberFont2[80] = {
0x70,0x88,0x88,0xA8,0x88,0x88,0x70,0x00, // 0 0x30
0x20,0x60,0x20,0x20,0x20,0x20,0x70,0x00, // 1
0x70,0x88,0x08,0x70,0x80,0x80,0xF8,0x00, // 2
0xF8,0x08,0x10,0x30,0x08,0x88,0x70,0x00, // 3
0x10,0x30,0x50,0x90,0xF8,0x10,0x10,0x00, // 4
0xF8,0x80,0xF0,0x08,0x08,0x88,0x70,0x00, // 5
0x38,0x40,0x80,0xF0,0x88,0x88,0x70,0x00, // 6
0xF8,0x08,0x08,0x10,0x20,0x40,0x80,0x00, // 7
0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00, // 8
0x70,0x88,0x88,0x78,0x08,0x10,0xE0,0x00  // 9
};

void DrawNumber(u16 x, u16 y, u8 color, u8 number) {
	u8 i,ii,iii,bits;
	
	if (number > 9) return;
	for (i = 0; i < 8; i++) {
		
		bits = NumberFont2[i + (number * 8)];		
		for (ii = 0; ii < 8; ii++, bits <<= 1) {
			if (bits & 0x80) {				
				for (iii = 0; iii < 4; iii++) {
					GraphicGoTo( LAYER1, (x + (ii * 8)) >> 3, (y + (i * 4) + iii) );
					sendCMD(SED1335_MWRITE);
					while (!(getStatus() & 0x40));
					if (color == 0) sendData(0);
					else sendData(0xff);
				}
			}
		}
	}
}

int random(void) {
	
        register int x = randseed;
        register int hi, lo, t;

        hi = x / 127773;
        lo = x % 127773;
        t = 16807 * lo - 2836 * hi;
        if (t <= 0)
                t += 0x7fffffff;
        randseed = t;
        return (t);
}


void drawScore(u16 x, u16 y, u8 score, u8 color) {
	DrawNumber(x, y, color, score / 10);
	DrawNumber(x + 48, y, color, score % 10);
}

int main(void) {
	u8 ang = 1;
	u8 angball = 1;
	u8 marcou = 0;
	u8 i;
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
	Delay(500);
	lcdPortInit();

	ClearGraphic();
		
	/*GraphicGoTo(LAYER1, 0,0);
	SetPixel(LAYER1,10,10,1);
	SetPixel(LAYER1,20,20,1);*/
	
	//DrawLine(LAYER1,1,1,639,1,1);
	//DrawLine(LAYER1,1,239,639,239,1);
	
	// TOP
	DrawLine(LAYER1,1,1,639,1,1);
	DrawLine(LAYER1,1,2,639,2,1);
	DrawLine(LAYER1,1,3,639,3,1);
	
	//Bottom
	DrawLine(LAYER1,1,239,639,239,1);
	DrawLine(LAYER1,1,238,639,238,1);
	DrawLine(LAYER1,1,237,639,237,1);
	
	//Middle
	for (i = 0; i < 24; i++) {
		DrawLine(LAYER1,319,(i * 10) + 4,319,(i * 10) + 14,1);
		DrawLine(LAYER1,320,(i * 10) + 4,320,(i * 10) + 14,1);
		DrawLine(LAYER1,321,(i * 10) + 4,321,(i * 10) + 14,1);
		i++;
	}
	
	drawPlayer( 8, player1y, 0xff );
	drawPlayer( 624, player2y, 0xff );
	
	drawScore( 216, 10, player1Score, 1 );
	
	drawScore( 336, 10, player2Score, 1 );
	
	drawBall( ballx, bally, 1 );
	// Infinite loop.
	for(;;) {
		// Blink Debug LED
		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);						
		//DrawDemo();			
		
		if (dirx == 1) ballx += BALLSIZE;
		if (dirx == 0) ballx -= BALLSIZE;

		if (diry == 1) bally += BALLSIZE / angball;
		if (diry == 0) bally -= BALLSIZE / angball;
		
		if ( ((bally >= player1y - 22) && (bally <= player1y + 22)) && (ballx == (BALLSIZE * 2))) {
			dirx = 1;
			ang = (u8)random();
			if ( ang < 100 ) diry = 0;
			if ( (ang > 100) && (ang < 240) ) diry = 1;
			if ( ang > 240 ) diry = 2;
			
			angball = (u8)random();
			angball = angball % 10;
			angball = angball % 5;
			if (angball == 0) angball = 1;
		} else if (ballx == 0){
			drawScore( 336, 10, player2Score, 0 );
			player2Score++;
			if (player2Score > 59) player2Score = 0;
			drawScore( 336, 10, player2Score, 1 );				
			marcou = 1;
		}

		
		if ( ((bally >= player2y - 22) && (bally <= player2y + 22)) && (ballx == 640 - (BALLSIZE * 3)) ) {
			dirx = 0;
			ang = (u8)random();
			if ( ang < 150 ) diry = 0;
			if ( (ang > 150) && (ang < 240) ) diry = 1;
			if ( ang > 240 ) diry = 2;
			
			angball = (u8)random();
			angball = angball % 10;
			angball = angball % 5;
			if (angball == 0) angball = 1;
		} else if (ballx == 640 - BALLSIZE) {
			drawScore( 216, 10, player1Score, 0 );
			player1Score++;
			if (player1Score > 23) player1Score = 0;
			drawScore( 216, 10, player1Score, 1 );			
			marcou = 1;
		}
		

		if (bally < BALLSIZE + 4) {
			diry = 1;	
		}
		
		if (bally > 240 - BALLSIZE - 4) {
			diry = 0;	
		}
		
		if ( (dirx == 0) && (ballx < 160) ) {
			drawPlayer( 8, player1oldy, 0 );
		
			if ( player1y > bally ) player1y -=BALLSIZE;
			if ( player1y < bally ) player1y +=BALLSIZE;
			
			player1oldy = player1y;
			drawPlayer( 8, player1y, 0xff );
		}
		
		if ( (dirx == 1) && (ballx > 480) ) {
			drawPlayer( 624, player2oldy, 0 );
			
			if ( player2y > bally ) player2y -=BALLSIZE;
			if ( player2y < bally ) player2y +=BALLSIZE;
			
			player2oldy = player2y;
			drawPlayer( 624, player2y, 0xff );
		}
		
		drawBall( balloldx, balloldy, 0 );
		drawBall( ballx, bally, 1 );
		balloldx = ballx;
		balloldy = bally;
		
		if (marcou == 1) {
			Delay(500);
			marcou = 0;
			ballx = 320;
			bally = 120;
			angball = (u8)random();
			angball = angball % 10;
			angball = angball % 5;
			if (angball == 0) angball = 1;
		}
		else Delay(10);
		
	}
}