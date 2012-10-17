/* Includes ------------------------------------------------------------------*/
#include "key.h"

void KEY_Init(void) {
	GPIO_Init(KEYLINE_PORT, KEYLINESELECT_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(KEYSCAN_PORT, KEYLINE1_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(KEYSCAN_PORT, KEYLINE2_PIN, GPIO_MODE_IN_PU_NO_IT);
}

KEYCODE GetKey(void) {
	u8 code;
	KEYCODE key = KEY_NONE;
	SCANLINE1();
	code = GPIO_ReadInputData( KEYSCAN_PORT );
	if ( !ValBit(code, 4) ) {
		key = KEY_UP;
	} else if ( !ValBit(code, 3) ) {
		key = KEY_DOWN;
	} else {
		SCANLINE2();
		code = GPIO_ReadInputData( KEYSCAN_PORT );
		if ( !ValBit(code, 4) ) {
			key = KEY_ESC;
		} else if ( !ValBit(code, 3) ) {
			key = KEY_ENTER;
		}
	}
	return key;
}


void WaitKey(KEYCODE key) {
	KEYCODE keypressed = KEY_NONE;
	do {
		keypressed = GetKey();
	} while ( keypressed != key);
}

bool IsKey(KEYCODE key) {
	return key == GetKey();
}