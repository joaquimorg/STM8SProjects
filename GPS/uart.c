#include "uart.h"

@far @interrupt void UART2_RX_IRQHandler(void) {
	u8 i;
	if (UART2_GetFlagStatus(UART2_FLAG_RXNE) != RESET) {
		/* Read one byte from the receive data register */
		i = UART2_ReceiveData8();
		uartAddData( i );
	}
}

void uartInit(void) {
	   
	/* clear UART data buffer */
   for( uartBufferPos = 0x00; uartBufferPos < UART_BUFFER_MAX; uartBufferPos++ ) {
      uartBuffer[uartBufferPos] = 0x00;
   }
   uartBufferPos = 0x00;
   
   UART2_DeInit();
	
	/* UART2 configuration ------------------------------------------------------*/
	/* UART2 configured as follow:
		- BaudRate = 9600 baud  
		- Word Length = 8 Bits
		- One Stop Bit
		- Odd parity
		- Receive and transmit enabled
		- UART2 Clock disabled
	*/
	/* Configure the UART2 */
	UART2_Init((u32)4800, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, 
		UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
		
	/* Enable UART2 Receive */
    UART2_ITConfig(UART2_IT_RXNE, ENABLE);
	
	/* Enable general interrupts */
    enableInterrupts();

}


u8 uartGetData(void) {
	
	u8 transmitByte;

	transmitByte = 0x00;
	
	transmitByte = uartBuffer[uartBufferPos];
	uartBufferPos++;
	
	return transmitByte;
}


void uartAddData(u8 NewDataByte) {
	uartBuffer[uartBufferPos] = NewDataByte;
	uartBufferPos = ((uartBufferPos + 1) % UART_BUFFER_MAX);
}

