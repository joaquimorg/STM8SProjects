
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H__
#define __UART_H__

#include "stm8s.h"

// -----------------------------------------------------------------------------
//
//

/** Defines the maximum number of elements for the data buffer */
#define UART_BUFFER_MAX		0x80

/** Buffer holding the all received NMEA messages from the GPS module */
u8 uartBuffer[UART_BUFFER_MAX];

/** Pointer to the current buffer position */
u8 uartBufferPos;

/** returns the byte within the ring buffer at the given position with wrap around */
#define UART_GET_BUFFER_VALUE(pos)  (uartBuffer[(pos) % UART_BUFFER_MAX])

//
//
// -----------------------------------------------------------------------------



/* Exported functions ------------------------------------------------------- */
void uartInit(void);
u8 uartGetData(void);
void uartAddData(u8 NewDataByte);



#endif /* __UART_H__ */