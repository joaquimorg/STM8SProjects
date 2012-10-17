/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_H
#define __KEY_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

#define KEYSCAN_PORT (GPIOC)
#define KEYLINE1_PIN  (GPIO_PIN_3)
#define KEYLINE2_PIN  (GPIO_PIN_4)

#define KEYLINE_PORT (GPIOE)
#define KEYLINESELECT_PIN  (GPIO_PIN_5)

#define SCANLINE1()              GPIO_WriteHigh(KEYLINE_PORT, KEYLINESELECT_PIN)
#define SCANLINE2()              GPIO_WriteLow(KEYLINE_PORT, KEYLINESELECT_PIN)


typedef enum {
	KEY_UP = 0,		/* 0 */
	KEY_DOWN,		/* 1 */
	KEY_ENTER,		/* 2 */
	KEY_ESC,		/* 3 */
	KEY_NONE		/* 4 */
} KEYCODE;


void KEY_Init(void);
KEYCODE GetKey(void);
void WaitKey(KEYCODE);
bool IsKey(KEYCODE key);


#endif /* __MSD_H */