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
//
//
//		3D Demo Version 2
//

#include "stm8s.h"
#include "glcdlib.h"

// -----------------------------------------------------------------------------

typedef struct {
    double x, y, z;
} Vector3D ;

typedef struct {
	int x;
	int y;
	int z;
} ScreenPoint;

#define	xAxis 64
#define yAxis 32
#define zAxis -550
#define viewFov 356
#define rotSpeed 3

double thetaX = 0;

#define FACENUMBER 72

#define	xCenter 120
#define yCenter 80

const u16 pontos[] = {	
	  0,  0, 25,
	 60,  0, 25,
	 80, 80, 25,
	160, 80, 25,
	180,  0, 25,
	240,  0, 25,
	160,220, 25,
	 80,220, 25,

	 80,100, 25,
	160,100, 25,
	140,180, 25,
	100,180, 25,

	  0,  0,-25,
	 60,  0,-25,
	 80, 80,-25,
	160, 80,-25,
	180,  0,-25,
	240,  0,-25,
	160,220,-25,
	 80,220,-25,

	 80,100,-25,
	160,100,-25,
	140,180,-25,
	100,180,-25

};

const u8 face[ FACENUMBER ] = {
	0,1, 1,2, 2,3, 3,4, 4,5, 5,6, 6,7, 7,0,
	8,9, 9,10, 10,11, 11,8,

	12,13, 13,14, 14,15, 15,16, 16,17, 17,18, 18,19, 19,12,
	20,21, 21,22, 22,23, 23,20,

	0,12, 1,13, 2,14, 3,15, 4,16, 5,17, 6,18, 7,19,
	8,20, 9,21, 10,22, 11,23

};


const double _sin_tab[] = {
 0.000000,0.017452,0.034899,0.052335,0.069756,0.087155,0.104528,0.121869,0.139173
,0.156434,0.173648,0.190808,0.207911,0.224951,0.241921,0.258819,0.275637,0.292371
,0.309016,0.325568,0.342020,0.358367,0.374606,0.390731,0.406736,0.422618,0.438371
,0.453990,0.469471,0.484809,0.499999,0.515038,0.529919,0.544639,0.559192,0.573576
,0.587785,0.601815,0.615661,0.629320,0.642787,0.656059,0.669130,0.681998,0.694658
,0.707106,0.719339,0.731353,0.743144,0.754709,0.766044,0.777145,0.788010,0.798635
,0.809016,0.819152,0.829037,0.838670,0.848048,0.857167,0.866025,0.874619,0.882947
,0.891006,0.898794,0.906307,0.913545,0.920504,0.927183,0.933580,0.939692,0.945518
,0.951056,0.956304,0.961261,0.965925,0.970295,0.974370,0.978147,0.981627,0.984807
,0.987688,0.990268,0.992546,0.994521,0.996194,0.997564,0.998629,0.999390,0.999847
,1.000000
};

double _sin( int angle ) {
	
	if ( angle >= 0 && angle <= 90 ) {
		return _sin_tab[angle];
	} else if ( angle >= 91 && angle <= 180 ) {
		return _sin_tab[90 - (angle - 90)];
	} else if ( angle >= 181 && angle <= 270 ) {
		return - _sin_tab[angle - 180];
	} else if ( angle >= 271 && angle <= 360 ) {
		return - _sin_tab[90 - (angle - 270)];
	}

    return 0;
}

double _cos( int angle ) {
	return _sin((angle+90)%360);
}


void Delay( int ms) {
	int i;
	while(ms--){ 
		i=2600;       //empirically determined fudge factor 16mhz
		while(i--); 
	} 
}


/**
  * [(1      ,0      ,0),
  *  (0      ,cos(a) ,-sin(a)),
  *  (0      ,sin(a) ,cos(a))]
  * @param Vector3D
  * @param theta
  */
Vector3D rotateX(int x, int y, int z, int theta) {
    Vector3D newv;
    newv.x = x;
    newv.y = y * _cos(theta) + z * -_sin(theta);
    newv.z = y * _sin(theta) + z * _cos(theta);
    return newv;
}

/**
  * [(cos(a) ,0      ,sin(a)),
  *  (0      ,1      ,0),
  *  (-sin(a),0      ,cos(a))]
  * @param Vector3D
  * @param theta
  */
Vector3D rotateY(Vector3D vector, int theta) {
    Vector3D newv;
    newv.x = vector.x * _cos(theta) + vector.z * _sin(theta);
    newv.y = vector.y;
    newv.z = vector.x * -_sin(theta) + vector.z * _cos(theta);
    return newv;
}

/**
  * [(cos(a),-sin(a),0),
  *  (sin(a),cos(a) ,0),
  *  (0     ,0      ,1)]
  * @param Vector3D
  * @param theta
  */
Vector3D rotateZ(Vector3D vector, int theta) {
    Vector3D newv;
    newv.x = vector.x * _cos(theta) + vector.y * -_sin(theta);
    newv.y = vector.x * _sin(theta) + vector.y * _cos(theta);
    newv.z = vector.z;
    return newv;
}
// -----------------------------------------------------------------------------


ScreenPoint RotateFace( int faceNum, int angleX, int angleY, int angleZ ) {
	ScreenPoint screenPoint;
	Vector3D v3D;

	v3D = rotateX(pontos[(face[faceNum] * 3)] - xCenter, pontos[(face[faceNum] * 3) + 1] - yCenter, pontos[(face[faceNum] * 3) + 2], angleX);
	v3D = rotateY(v3D, angleY);
	v3D = rotateZ(v3D, angleZ);

	screenPoint.x =  ((viewFov * v3D.x) / (v3D.z - zAxis)) + xAxis;
	screenPoint.y =  ((viewFov * v3D.y) / (v3D.z - zAxis)) + yAxis;
	screenPoint.z = (int)v3D.z;
	return screenPoint;
}


void DrawScreen( void ) {
	ClearScr();
}

void DrawDemo( void ) {
	u8 x = 0;

	ScreenPoint screenPointA, screenPointB;

	FillScr( 0x00 );	
	
	thetaX = thetaX + rotSpeed;
	
	if (thetaX > 360) thetaX = 0;
	
	do {
		
		screenPointA = RotateFace( x, thetaX, thetaX, thetaX ); 
		x++;
		
		screenPointB = RotateFace( x, thetaX, thetaX, thetaX );
		x++;
		
		DrawLine(screenPointA.x, screenPointA.y, screenPointB.x, screenPointB.y, LCD_PIXEL_ON);
		
	} while ( x < FACENUMBER );
	DrawFillRectangle( 0,57, 128, 64, LCD_PIXEL_ON);
	DrawText(1,58, "STM8S 3D - joaquim.org", LCD_CHAR_INV);
	
	UpdateLCD();
	//Delay(20);
}

int main(void) {

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
	
	LCDInit();
	
	DrawScreen();
	
	// Infinite loop.
	for(;;) {
		// Blink Debug LED
		GPIO_WriteReverse(GPIOD, GPIO_PIN_0);						
		DrawDemo();	
	}
}