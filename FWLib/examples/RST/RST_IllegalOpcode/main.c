/**
  ******************************************************************************
  * @file RST_IllegalOpcode\main.c
  * @brief This file contains the main function for RST Illegal Opcode example.
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
  * @addtogroup RST_IllegalOpcode
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay (u16 nCount);
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief Example firmware main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
    u8 i =0;
    u8 Leds = (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN); /* Contains which LEDs to operate */

    FlagStatus IllOpFlag = 0;

    enableInterrupts();

    /* GPIO Configuration */

    /* Initialize I/Os in Output Mode */
    GPIO_Init(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN), GPIO_MODE_OUT_PP_LOW_FAST);

    /* Initialize I/O in Input Mode with Interrupt */
    GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_FL_IT);

    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_ONLY);
		
    FLASH_Unlock(FLASH_MEMTYPE_PROG);
		
    IllOpFlag = RST_GetFlagStatus(RST_FLAG_ILLOPF);

    /* Test if a ILLOP Reset has occured */
    if (IllOpFlag)
    {
        for (i=0;i<50;i++)
        {
            /* An ILLOP Reset has occured, Toggles LD1, LD2, LD3 and LD4 */
            /* LEDs on */
            GPIO_WriteHigh(LEDS_PORT, Leds);
            /* delay */
            Delay(0x7FFF);
            /* LEDs off */
            GPIO_WriteLow(LEDS_PORT, Leds);
            /* delay */
            Delay(0x7FFF);
        }
        /* Clear IWDGF Flag */
        RST_ClearFlag(RST_FLAG_ILLOPF);
    }

    /* Write an illigal opcode */
    FLASH_ProgramByte(0x9FFF, 0x75);

    while (1)
    {
        /* LEDs on */
        GPIO_WriteHigh(LEDS_PORT, Leds);
        /* delay */
        Delay(0xFFFF);
        /* LEDs off */
        GPIO_WriteLow(LEDS_PORT, Leds);
        /* delay */
        Delay(0xFFFF);
    }

}
/**
  * @brief Delay.
  * @param[in] nCount
  * @retval 
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
