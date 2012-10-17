/**
  ******************************************************************************
  * @file UART1_Synchronous\main.c
  * @brief This file contains the main function for UART1 in synchronous mode example.
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
  * @addtogroup UART1_Synchronous
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define TxBufferSize2   (countof(TxBuffer2) - 1)
#define DYMMY_BYTE      0x00

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/

u8 TxBuffer1[] = "UART1 Example: UART1 -> SPI using TXE and RXNE Flags";
u8 TxBuffer2[] = "UART1 Example: SPI -> UART1 using TXE and RXNE Flags";
u8 RxBuffer1[TxBufferSize2];
u8 RxBuffer2[TxBufferSize1];
u8 NbrOfDataToRead1 = TxBufferSize2;
u8 NbrOfDataToRead2 = TxBufferSize1;
u8 TxCounter1 = 0, RxCounter1 = 0;
u8 TxCounter2 = 0, RxCounter2 = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
ErrorStatus HSEStartUpStatus;

/* Private function prototypes -----------------------------------------------*/
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength);
void Delay (u16 nCount);
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

   UART1_DeInit();
   /* UART1 configuration -------------------------------------------------------*/
   /* UART1 configured as follow:
          - Word Length = 8 Bits
          - 1 Stop Bit
          - No parity
          - BaudRate = 9600 baud
          - UART1 Clock enabled
          - Polarity Low
          - Phase Middle
          - Last Bit enabled
          - Receive and transmit enabled
   */

  UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_ENABLE | UART1_SYNCMODE_CPOL_LOW |UART1_SYNCMODE_CPHA_MIDDLE |UART1_SYNCMODE_LASTBIT_ENABLE, UART1_MODE_TXRX_ENABLE);
  UART1_Cmd(DISABLE);
  /* SPI config */
  SPI_DeInit();
	/* Initialize SPI in Slave mode  */
  SPI_Init(SPI_FIRSTBIT_LSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_SLAVE, SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT,(u8)0x07);

  /* Enable the UART1*/
  UART1_Cmd(ENABLE);
	Delay(300);
  /* Enable the SPI*/
	SPI_Cmd(ENABLE);
  
    while (NbrOfDataToRead2--)
    {
        /* Write one byte in the UART1 Transmit Data Register */
        UART1_SendData8(TxBuffer1[TxCounter1++]);
        /* Wait until end of transmit */
        while (UART1_GetFlagStatus(UART1_FLAG_TC) == RESET)
        {
        }
        /* Wait the byte is entirtly received by SPI */
        while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer2 */
        RxBuffer2[RxCounter2++] = SPI_ReceiveData();
    }

    /* Clear the UART1 Data Register */
    UART1_ReceiveData8();

    while (NbrOfDataToRead1--)
    {
        /* Wait until end of transmit */
        while (SPI_GetFlagStatus(SPI_FLAG_TXE)== RESET)
        {
        }
        /* Write one byte in the SPI Transmit Data Register */
        SPI_SendData(TxBuffer2[TxCounter2++]);

        /* Send a Dummy byte to generate clock to slave */
        UART1_SendData8(DYMMY_BYTE);
        /* Wait until end of transmit */
        while (UART1_GetFlagStatus(UART1_FLAG_TC) == RESET)
        {
        }
        /* Wait the byte is entirtly received by UART1 */
        while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
        {
        }
        /* Store the received byte in the RxBuffer1 */
        RxBuffer1[RxCounter1++] = UART1_ReceiveData8();
    }

    /* Check the received data with the send ones */
    TransferStatus1 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);
    /* TransferStatus = PASSED, if the data transmitted from UART1 and received by SPI are the same */
    /* TransferStatus = FAILED, if the data transmitted from UART1 and received by SPI are different */
    TransferStatus2 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);
    /* TransferStatus = PASSED, if the data transmitted from SPI and received by UART1 are the same */
    /* TransferStatus = FAILED, if the data transmitted from SPI and received by UART11 are different */

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
  * @brief Inssert a delay.
  * @param[in] nCount specifies the counter value.
  * @retval void
  * @par Required preconditions:
  * None
  */
void Delay (u16 nCount)
{
    /* Decrement nCount value */
    for (; nCount != 0; nCount--);
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

