/**
  ******************************************************************************
  * @file ADC2_ContinuousConversion\main.c
  * @brief This file contains the main function for the ADC2 Continuous Conversion example.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.1
  * @date 06/05/2009
  **************************************************************************
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
  * @addtogroup ADC2_ContinuousConversion
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
 u16 Conversion_Value = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**
  * @brief ADC2_ContinuousConversion  main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
    /*  Init GPIO for ADC2 */
    GPIO_Init(GPIOE, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);

    /* Init GPIO for LED  */
    GPIO_Init(LEDS_PORT, (LED1_PIN |LED2_PIN |LED3_PIN |LED4_PIN), GPIO_MODE_OUT_PP_LOW_FAST);


    ADC2_DeInit();

    enableInterrupts();

    /* Init ADC2 peripheral */
    ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS, ADC2_CHANNEL_9, ADC2_PRESSEL_FCPU_D2, ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SCHMITTTRIG_CHANNEL9, DISABLE);

    /* Enable EOC interrupt */
    ADC2_ITConfig(ENABLE);

    /*Start Conversion */
    ADC2_StartConversion();
    
    /* The LEDs state are changed in the interrupt routine */
    while (1);
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
