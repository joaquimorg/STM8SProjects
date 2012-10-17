/**
  ******************************************************************************
  * @file UART1_Interrupt\main.c
  * @brief This file contains the main function for a basic communication between UART1 and UART3 using interrupts.
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
#include "main.h"

/**
  * @addtogroup UART1_Interrupt
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private variables ---------------------------------------------------------*/
vu8 TxBuffer1[] = "UART1 Interrupt Example: UART1 -> UART3 using Interrupt";
vu8 TxBuffer2[] = "UART1 Interrupt Example: UART3 -> UART1 using Interrupt";  
u8 RxBuffer1[RxBufferSize1];
u8 RxBuffer2[RxBufferSize2];
vu8 TxCounter1 = 0x00;
vu8 TxCounter2 = 0x00;
vu8 RxCounter1 = 0x00;
vu8 RxCounter2 = 0x00;
u8 NbrOfDataToTransfer1 = TxBufferSize1;
u8 NbrOfDataToTransfer2 = TxBufferSize2;
u8 NbrOfDataToRead1 = RxBufferSize1;
u8 NbrOfDataToRead2 = RxBufferSize2;
volatile TestStatus TransferStatus1 = FAILED;
volatile TestStatus TransferStatus2 = FAILED;

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
    /*High speed internal clock prescaler: 1*/
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    /* Enable general interrupts */
    enableInterrupts();

    /* Deinitializes the UART1 and UART3 peripheral */
    UART1_DeInit();
    UART3_DeInit();
    /* UART1 and UART3 configuration -------------------------------------------------*/
    /* UART1 and UART3 configured as follow:
          - BaudRate = 9600 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Receive and transmit enabled
          - UART1 Clock disabled
     */
    /* Configure the UART1 */
    UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

    /* Configure the UART3 */
 		UART3_Init((u32)9600, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO, UART3_MODE_TXRX_ENABLE);

    /* Enable UART3 Receive and UART1 Transmit interrupt */

    UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
    UART1_ITConfig(UART1_IT_TXE, ENABLE);

    /* Wait until end of transmission from UART1 to UART3 */
    while (GetVar_RxCounter2() < GetVar_NbrOfDataToTransfer1())
    {
    }
    /* Enable UART1 Receive and UART3 Transmit interrupt */
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
    UART3_ITConfig(UART3_IT_TXE, ENABLE);
    /* Wait until end of transmission from UART3 to UART1 */
    while (GetVar_RxCounter1() < GetVar_NbrOfDataToTransfer2())
    {
    }

    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(TxBuffer2, RxBuffer1, RxBufferSize1);
    /* TransferStatus1 = PASSED, if the data transmitted from UART3 and
       received by UART1 are the same */
    /* TransferStatus1 = FAILED, if the data transmitted from UART3 and
       received by UART1 are different */
    TransferStatus2 = Buffercmp(TxBuffer1, RxBuffer2, RxBufferSize2);
    /* TransferStatus2 = PASSED, if the data transmitted from UART1 and
       received by UART3 are the same */
    /* TransferStatus2 = FAILED, if the data transmitted from UART1 and
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
/**
  * @brief Returns TxCounter1 value.
  * @par Parameters:
  * None
  * @retval Transmit counter index1
  * - u8: TxCounter1
  * @par Required preconditions:
  * None
  */
u8 GetVar_TxCounter1(void)
{
return TxCounter1;
}
/**
  * @brief Returns TxCounter2 value.
  * @par Parameters:
  * None
  * @retval Transmit counter index2
  * - u8: TxCounter2
  * @par Required preconditions:
  * None
  */
u8 GetVar_TxCounter2(void)
{
return TxCounter2;
}
/**
  * @brief Returns RxCounter1 value.
  * @par Parameters:
  * None
  * @retval Receive counter index1
  * - u8: RxCounter1
  * @par Required preconditions:
  * None
  */
u8 GetVar_RxCounter1(void)
{
return RxCounter1;
}
/**
  * @brief Returns RxCounter2 value.
  * @par Parameters:
  * None
  * @retval Receive counter index2
  * - u8: RxCounter2
  * @par Required preconditions:
  * None
  */
u8 GetVar_RxCounter2(void)
{
return RxCounter2;
}
/**
  * @brief Increments TxCounter1 variable and return its value 
  * @par Parameters:
  * None
  * @retval Transmit counter index1++
  * - u8: TxCounter1++
  * @par Required preconditions:
  * None
  */
u8 IncrementVar_TxCounter1(void)
{
	return TxCounter1++;
}
/**
  * @brief Increments TxCounter2 variable and return its value 
  * @par Parameters:
  * None
  * @retval Transmit counter index2++
  * - u8: TxCounter2++
  * @par Required preconditions:
  * None
  */
u8 IncrementVar_TxCounter2(void)
{
	return TxCounter2++;
}
/**
  * @brief Increments RxCounter1 variable and return its value 
  * @par Parameters:
  * None
  * @retval Receive counter index1++
  * - u8: RxCounter1++
  * @par Required preconditions:
  * None
  */
u8 IncrementVar_RxCounter1(void)
{
	return RxCounter1++;
}
/**
  * @brief Increments RxCounter2 variable and return its value 
  * @par Parameters:
  * None
  * @retval Receive counter index2++
  * - u8: RxCounter2++
  * @par Required preconditions:
  * None
  */
u8 IncrementVar_RxCounter2(void)
{
	return RxCounter2++;
}
/**
  * @brief Returns NbrOfDataToTransfer1 value.
  * @par Parameters:
  * None
  * @retval Tx Buffer Size1
  * - u8: NbrOfDataToTransfer1
  * @par Required preconditions:
  * None
  */
u8 GetVar_NbrOfDataToTransfer1(void)
{
return NbrOfDataToTransfer1;
}
/**
  * @brief Returns NbrOfDataToTransfer2 value.
  * @par Parameters:
  * None
  * @retval Tx Buffer Size2
  * - u8: NbrOfDataToTransfer2
  * @par Required preconditions:
  * None
  */
u8 GetVar_NbrOfDataToTransfer2(void)
{
return NbrOfDataToTransfer2;
}
/**
  * @brief Returns NbrOfDataToRead1 value.
  * @par Parameters:
  * None
  * @retval Rx Buffer Size1
  * - u8: NbrOfDataToRead1
  * @par Required preconditions:
  * None
  */
u8 GetVar_NbrOfDataToRead1(void)
{
return NbrOfDataToRead1;
}
/**
  * @brief Returns NbrOfDataToRead2 value.
  * @par Parameters:
  * None
  * @retval Rx Buffer Size2
  * - u8: NbrOfDataToRead2
  * @par Required preconditions:
  * None
  */
u8 GetVar_NbrOfDataToRead2(void)
{
return NbrOfDataToRead2;
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
