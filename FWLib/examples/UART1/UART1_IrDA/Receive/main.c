/**
  ******************************************************************************
  * @file UART1_IrDA\Receive\main.c
  * @brief This file contains the main function for UART1 in IrDA receive mode.
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
  * @addtogroup UART1_IrDA_Receive
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Evalboard I/Os configuration */
#define LEDS_PORT (GPIOH)
#define LED1_PIN  (GPIO_PIN_3)
#define LED2_PIN  (GPIO_PIN_2)
#define LED3_PIN  (GPIO_PIN_1)
#define LED4_PIN  (GPIO_PIN_0)

#define  NOKEY  0x00
#define  SEL    0x01
#define  RIGHT  0x02
#define  LEFT   0x03
#define  UP     0x04
#define  DOWN   0x05
u8 ReceivedData = 0;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Multiplexer_EvalBoard_Config(void);
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
    /* Configures the Multiplexer on the evalboard to select the IrDA*/
    Multiplexer_EvalBoard_Config();

    /* Initialize I/Os in Output Mode */
    GPIO_Init(LEDS_PORT, LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);

    UART1_DeInit();
    /* UART1 configuration ----------------------------------------------------*/
    /* UART1 configured as follow:
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - BaudRate = 9600 baud
          - Tx and Rx enabled
          - UART1 Clock disabled
    */
    UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

		/* Set Prescaler*/
    UART1_SetPrescaler(0x1);

    UART1_IrDAConfig(UART1_IRDAMODE_NORMAL);

    UART1_IrDACmd(ENABLE);
    while (1)
    {
        /* Wait until a byte is received */
        while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
        {
        }
        /* Read the received byte */
        ReceivedData = UART1_ReceiveData8();

        switch (ReceivedData)
        {
            /* Led connected to PH.0 (LED4) toggle */
        case UP:
            GPIO_WriteReverse(GPIOH, GPIO_PIN_0);
            break;
            /* Led connected to PH.1 (LED3) toggle */
        case DOWN:
            GPIO_WriteReverse(GPIOH, GPIO_PIN_1);
            break;
            /* Led connected to PH.2 (LED2) toggle */
        case LEFT:
            GPIO_WriteReverse(GPIOH, GPIO_PIN_2);
            break;
            /* Led connected to PH.3 (LED1) toggle */
        case RIGHT:
            GPIO_WriteReverse(GPIOH, GPIO_PIN_3);
            break;

        case SEL:
            GPIO_WriteReverse(GPIOH, GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1);
            break;
        default:
            break;
        }
    }

}
/**
  * @brief Configures the Multiplexer on the evalboard to select the IrDA.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void Multiplexer_EvalBoard_Config(void)
{
    /* Initialize I/Os in Output PP Mode */
		GPIO_Init(GPIOF, GPIO_PIN_6 ,GPIO_MODE_OUT_PP_LOW_FAST);
		GPIO_Init(GPIOF, GPIO_PIN_5 ,GPIO_MODE_OUT_PP_HIGH_FAST);
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
