/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */

#include "main.h"

extern u8 RxBuffer1[];
u8 Estado = 0;

typedef void @far (*interrupt_handler_t)(void);

struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}


/**
  * @brief UART2 TX interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
@far @interrupt void UART2_TX_IRQHandler(void) {

}

/**
  * @brief UART2 RX interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
@far @interrupt void UART2_RX_IRQHandler(void) {
	u8 i;
	if (UART2_GetFlagStatus(UART2_FLAG_RXNE) != RESET) {
		/* Read one byte from the receive data register */
		i = UART2_ReceiveData8();
		if ( i == '$' ) {
			Estado = 1;
			SetVar_RxCounter1( 0 );
		} else if ( i == '\n' ) {
			Estado = 0;			
		} else if ( Estado == 1 ) {
			RxBuffer1[IncrementVar_RxCounter1()] = i;
	
			if (GetVar_RxCounter1() > 80) {
				Estado = 0;
			}
		}
	}
}

extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NonHandledInterrupt}, /* trap  */
	{0x82, NonHandledInterrupt}, /* irq0  */
	{0x82, NonHandledInterrupt}, /* irq1  */
	{0x82, NonHandledInterrupt}, /* irq2  */
	{0x82, NonHandledInterrupt}, /* irq3  */
	{0x82, NonHandledInterrupt}, /* irq4  */
	{0x82, NonHandledInterrupt}, /* irq5  */
	{0x82, NonHandledInterrupt}, /* irq6  */
	{0x82, NonHandledInterrupt}, /* irq7  */
	{0x82, NonHandledInterrupt}, /* irq8  */
	{0x82, NonHandledInterrupt}, /* irq9  */
	{0x82, NonHandledInterrupt}, /* irq10 */
	{0x82, NonHandledInterrupt}, /* irq11 */
	{0x82, NonHandledInterrupt}, /* irq12 */
	{0x82, NonHandledInterrupt}, /* irq13 */
	{0x82, NonHandledInterrupt}, /* irq14 */
	{0x82, NonHandledInterrupt}, /* irq15 */
	{0x82, NonHandledInterrupt}, /* irq16 */
	{0x82, NonHandledInterrupt}, /* irq17 */
	{0x82, NonHandledInterrupt}, /* irq18 */
	{0x82, NonHandledInterrupt}, /* irq19 */
	{0x82, (interrupt_handler_t)UART2_TX_IRQHandler}, /* irq20 */
	{0x82, (interrupt_handler_t)UART2_RX_IRQHandler}, /* irq21 */
	{0x82, NonHandledInterrupt}, /* irq22 */
	{0x82, NonHandledInterrupt}, /* irq23 */
	{0x82, NonHandledInterrupt}, /* irq24 */
	{0x82, NonHandledInterrupt}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};

