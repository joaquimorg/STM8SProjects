/**
  ******************************************************************************
  * @file ITC_SoftwarePriority\main.c
  * @brief This file contains the main function for the ITC software priority example.
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
  * @addtogroup ITC_SoftwarePriority
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Delay (u16 nCount);
/* Public functions ----------------------------------------------------------*/

/**
  * @brief Example main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
  /* Initialize I/Os in Output Mode for LEDs */
  GPIO_Init(LEDS_PORT, LED1_PIN | LED2_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	
  /* Initialize I/O in Input Mode with Interrupt for button and joystick */
	/* Button */
  GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_FL_IT);
  /* Joystick */
  GPIO_Init(JOYSTICK_PORT, JOYSTICK_DOWN_PIN, GPIO_MODE_IN_FL_IT);

  /* Initialize the Interrupt sensitivity for button and joystick */
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_FALL_LOW);
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_LOW);

  /*
    Change the software priority between button and joystick:
    - By hardware the PORTB (joystick) has a higher priority than PORTC (button)
    - We configure the PORTC (button) to have a higher sw priority than PORTB (joystick)
  */
  ITC_SetSoftwarePriority(ITC_IRQ_PORTB, ITC_PRIORITYLEVEL_1); /* joystick */
  ITC_SetSoftwarePriority(ITC_IRQ_PORTC, ITC_PRIORITYLEVEL_2); /* button = higher sw priority */

  enableInterrupts();

  /* LEDs are ON together */
  GPIO_WriteHigh(LEDS_PORT, (LED1_PIN | LED2_PIN));
 
  while (1);
}
/**
  * @brief Delay.
  * @param[in] nCount
  * @retval None
  * @par Required preconditions:
  * None
  */
void Delay(u16 nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
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
