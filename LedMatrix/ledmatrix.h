/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LEDMATRIX_H
#define __LEDMATRIX_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/



/* Exported constants --------------------------------------------------------*/
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3

#define DISPLAY_WIDTH 8
#define DISPLAY_HEIGHT 8


/* Exported functions ------------------------------------------------------- */

void InitMatrix(void);
void UpdateMatrix(void);
void FillScr( u8 v );
void GotoXY( int x, int y );
void PutText(char str[], u8 v);
void nextcolum( void );
void DrawPoint( int x, int y, u8 v );
u8 CharSize(char character);

#endif /* __LEDMATRIX_H */