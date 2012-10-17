/**
  ******************************************************************************
  * @file UART1_HalfDuplex\main.c
  * @brief This file contains the main function for UART1 in Half-Duplex mode example.
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
  * @addtogroup UART1_HalfDuplex
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define TxBufferSize2   (countof(TxBuffer2) - 1)

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* To run the sender connect the UART1_TX on the UART3_RX, uncomment the line "#define sender" and comment the line "#define receiver" */
#define sender 

/* To run the sender connect the UART1_TX on the UART3_TX , uncomment the line "#define receiver" and comment the line "#define sender" */
/* #define receiver */


/* Private variables ---------------------------------------------------------*/
u8 TxBuffer1[] = "HalfDuplex Example: UART1 -> UART3 using HalfDuplex mode";
u8 TxBuffer2[] = "HalfDuplex Example: UART3 -> UART1 using HalfDuplex mode";
u8 RxBuffer1[TxBufferSize2]={0};
u8 RxBuffer2[TxBufferSize1]={0};
u8 NbrOfDataToRead1 = TxBufferSize2;
u8 NbrOfDataToRead2 = TxBufferSize1;
u8 TxCounter1 = 0, RxCounter1 = 0;
u8 TxCounter2 = 0, RxCounter2 = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

/* Private function prototypes -----------------------------------------------*/
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength);

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
    /* Set PA5 as Output open-drain high-impedance level (UART1_Tx)*/
    GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);

    /*High speed internal clock prescaler: 1*/
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    /* UART1 and UART3 configuration ----------------------------------------------*/
    /* UART1 and UART3 configured as follow:
          - BaudRate = 230400 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Receive and transmit enabled
    */
    UART1_DeInit();
    UART1_Init((u32)230400, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

    UART3_DeInit();
		UART3_Init((u32)230400, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, UART3_MODE_TXRX_ENABLE);
   
    /* Enable UART1 Half Duplex Mode*/
    UART1_HalfDuplexCmd(ENABLE);

#ifdef sender

    while (NbrOfDataToRead2--)
    {
        /* Wait until end of transmit */
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
        {
        }
        /* Write one byte in the UART1 Transmit Data Register */
        UART1_SendData8(TxBuffer1[TxCounter1++]);

        /* Wait the byte is entirtly received by UART3 */
        while (UART3_GetFlagStatus(UART3_FLAG_RXNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer2 */
        RxBuffer2[RxCounter2++] = UART3_ReceiveData8();
    }
    /* Check the received data with the send ones */

    TransferStatus1 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);
    /* TransferStatus = PASSED, if the data transmitted from UART1 and received by UART3 are the same */
    /* TransferStatus = FAILED, if the data transmitted from UART1 and received by UART3 are different */

#endif
#ifdef receiver
    /* Clear the UART1 Data Register */
    UART1_ReceiveData8();

    while (NbrOfDataToRead1--)
    {
        /* Wait until end of transmit */
        while (UART3_GetFlagStatus(UART3_FLAG_TXE)== RESET)
        {
        }
        /* Write one byte in the UART3 Transmit Data Register */
        UART3_SendData8(TxBuffer2[TxCounter2++]);

        /* Wait the byte is entirtly received by UART1 */
        while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer1 */
        RxBuffer1[RxCounter1++] = UART1_ReceiveData8();
    }

    /* Check the received data with the send ones */
    TransferStatus2 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);
    /* TransferStatus = PASSED, if the data transmitted from UART3 and received by UART1 are the same */
    /* TransferStatus = FAILED, if the data transmitted from UART3 and received by UART1 are different */
#endif
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
