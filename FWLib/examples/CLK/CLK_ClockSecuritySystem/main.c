/**
  ******************************************************************************
  * @file CLK_ClockSecuritySystem\main.c
  * @brief This file contains the main function for the CLK CSS example.
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
  * @addtogroup CLK_ClockSecuritySystem
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(u16 nCount);
/* Private functions ---------------------------------------------------------*/
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

    u16 i = 0;
    ErrorStatus status = ERROR;

    /* Init GPIO for LED  */
    GPIO_Init(LEDS_PORT, (LED1_PIN |LED2_PIN |LED3_PIN |LED4_PIN), GPIO_MODE_OUT_PP_LOW_FAST);


    CLK_DeInit();

    enableInterrupts();

    status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, DISABLE);

    /*Enable CSS interrupt */
		CLK_ITConfig(CLK_IT_CSSD, ENABLE);
		
		/* Enable CCS */
    CLK_ClockSecuritySystemEnable();

    /* Output Fcpu on CLK_CCO pin */
    CLK_CCOConfig(CLK_OUTPUT_MASTER);

    while (1)
    {
        /*  Toggle the LEDs  */
        GPIO_WriteReverse(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN));
        for (i = 0; i < 2; i++)
        {
            Delay((u16)60000);
        }
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
