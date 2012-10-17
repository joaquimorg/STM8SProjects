/**
  ******************************************************************************
  * @file UART1_MultiProcessor\main.c
  * @brief This file contains the main function for:use the UART1 in multi-processor mode example.
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
  * @addtogroup UART1_MultiProcessor
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

#define BUTTON_PORT (GPIOC)
#define BUTTON_PIN  (GPIO_PIN_0)

#define JOYSTICK_UP_PORT (GPIOB)
#define JOYSTICK_UP_PIN    (GPIO_PIN_6)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(u32 nCount);
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
    /* Initialize I/Os in Output Mode for LEDs */
    GPIO_Init(LEDS_PORT, LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN, GPIO_MODE_OUT_PP_LOW_FAST);

    /* Initialize I/O in Input Mode with Interrupt for Joystick */
    GPIO_Init(JOYSTICK_UP_PORT, JOYSTICK_UP_PIN, GPIO_MODE_IN_FL_IT);

    /* Initialize I/O in Input Mode with Interrupt for Key */
    GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_FL_IT);

    /* Initialize the Interrupt sensitivity */
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_RISE_ONLY);
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_ONLY);

    GPIO_WriteHigh(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN));

    enableInterrupts();

    UART1_DeInit();
    /* UART1 configuration ------------------------------------------------------*/
    /* UART1 configured as follow:
          - BaudRate = 9600 baud  
          - Word Length = 9 Bits
          - One Stop Bit
          - No parity
          - Receive and Transmit enabled
          - UART1 Clock disabled
    */
    /* Configure UART1 */
    UART1_Init((u32)9600, UART1_WORDLENGTH_9D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

    UART3_DeInit();
    /* UART3 configuration -------------------------------------------*/
    /* UART3 configured as follow:
          - BaudRate = 9600 baud  
          - Word Length = 9 Bits
          - One Stop Bit
          - No parity
          - Receive and transmit enabled
    */
    /* Configure UART3 */
    UART3_Init((u32)9600, UART3_WORDLENGTH_9D, UART3_STOPBITS_1, UART3_PARITY_NO, UART3_MODE_TXRX_ENABLE);

    /* Set the UART1 Address */
    UART1_SetAddress(0x1);
    /* Set the UART3 Address */
    UART3_SetAddress(0x2);

    /* Select the UART3 WakeUp Method */
    UART3_WakeUpConfig(UART3_WAKEUP_ADDRESSMARK);


    while (1)
    {
        /* Send on byte from UART1 to UART3 */
        UART1_SendData9(0x33);
        /* Wait while UART1 TXE = 0 */
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
        {
        }
        if (UART3_GetFlagStatus(UART3_FLAG_RXNE) != RESET)
        {
            if (UART3_ReceiveData9() == 0x33)
            {
                GPIO_WriteReverse(LEDS_PORT, LED1_PIN);
                Delay(0x5FF);
                GPIO_WriteReverse(LEDS_PORT, LED2_PIN);
                Delay(0x5FF);
                GPIO_WriteReverse(LEDS_PORT, LED3_PIN);
                Delay(0x5FF);
                GPIO_WriteReverse(LEDS_PORT, LED4_PIN);
                Delay(0x5FF);
            }
        }
    }
}
/**
  * @brief Inserts a delay time.
  * @param[in] nCount specifies the delay time length.
  * @retval None
  * @par Required preconditions:
  * - None
  * @par Library functions called:
  * - None
  */
void Delay(u32 nCount)
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
