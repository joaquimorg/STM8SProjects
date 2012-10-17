/**
  ******************************************************************************
  * @file ADC2_TriggerConversion\main.c
  * @brief This file contains the main function for the ADC2 Trigger Conversion example.
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
  * @addtogroup ADC2_TriggerConversion
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * @brief Example main entry point.
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

    /* Configure the conversion mode and the channel to convert  */
    ADC2_ConversionConfig(ADC2_CONVERSIONMODE_CONTINUOUS, ADC2_CHANNEL_9, ADC2_ALIGN_RIGHT);

    /* Configure the schmitt trigger channel and state */
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL9, DISABLE);

    /* Enable the ADC2 peripheral */
    ADC2_Cmd(ENABLE);

    /* Enable EOC interrupt */
    ADC2_ITConfig(ENABLE);

    /* Configure the TIM1 Master/Slave mode */
    TIM1_SelectMasterSlaveMode(ENABLE);

    /*Configure the ADC2 external trigger */
    ADC2_ExternalTriggerConfig(ADC2_EXTTRIG_TIM, ENABLE);

    /* Trigger the conversion */
    TIM1_SelectOutputTrigger(TIM1_TRGOSOURCE_ENABLE);
    TIM1_Cmd(ENABLE);

	  /* The LEDs are changed in the interrupt routine */
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
