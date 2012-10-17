/**
  ******************************************************************************
  * @file UART1_Polling\main.c
  * @brief This file contains the main function for UART1 using flags in communication example.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.1
  * @date 06/05/2009
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/**
  * @addtogroup UART1_Polling
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize   (countof(TxBuffer))

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
u8 TxBuffer[] = "Buffer Send from UART1 to UART3 using Flags";
u8 RxBuffer[TxBufferSize];
u8 TxCounter = 0, RxCounter = 0;
TestStatus TransferStatus = FAILED;
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength);
u8 index = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief Example firmware main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
    /*High speed internal clock prescaler: 1*/
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    /* UART1 and UART3 configuration ------------------------------------------------------*/
    /* UART1 and UART3 configured as follow:
          - BaudRate = 230400 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - Even parity
          - Receive and transmit enabled
          - UART1 Clock disabled
    */
		/* Configure UART1 */
    UART1_Init((u32)230400, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_EVEN, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

    /* Configure UART3 */
 		UART3_Init((u32)230400, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_EVEN, UART3_MODE_TXRX_ENABLE);

    while (TxCounter < TxBufferSize)
    {
        /* Send one byte from UART1 to UART3 */
        UART1_SendData8(TxBuffer[TxCounter++]);

        /* Loop until the end of transmit */
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
        {
        }

        /* Loop until the UART3 Receive Data Register is not empty */
        while (UART3_GetFlagStatus(UART3_FLAG_RXNE) == RESET)
        {
        }

        /* Store the received byte in RxBuffer */
        RxBuffer[RxCounter++] = (UART3_ReceiveData8() & 0x7F);

    }
    /* Check the received data with the send ones */
    TransferStatus = Buffercmp(TxBuffer, RxBuffer, TxBufferSize);
    /* TransferStatus = PASSED, if the data transmitted from UART1 and
       received by UART3 are the same */
    /* TransferStatus = FAILED, if the data transmitted from UART1 and
       received by UART3 are different */

    while (1);

}

/**
  * @brief Compares two buffers.
  * @param[in] pBuffer1 First buffer to be compared.
  * @param[in] pBuffer2 Second buffer to be compared.
  * @param[in] BufferLength Buffer's length
  * @retval TestStatus Status of buffer comparison
  * - PASSED: pBuffer1 identical to pBuffer2
  * - FAILED: pBuffer1 differs from pBuffer2
  * @par Required preconditions:
  * None
  */
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval 
  * None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
