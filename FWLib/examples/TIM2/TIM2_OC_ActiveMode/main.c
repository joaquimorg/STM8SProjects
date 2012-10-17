/**
  ******************************************************************************
  * @file TIM2_OC_ActiveMode\main.c
  * @brief This file contains the main function for TIM2 OC ActiveMode Example.
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
  * @addtogroup TIM2_OC_ActiveMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CCR1_Val  ((u16)976)
#define CCR2_Val  ((u16)488)
#define CCR3_Val  ((u16)244)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
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

  /* Time base configuration */      
  TIM2_TimeBaseInit(TIM2_PRESCALER_2048, 65535);

  /* Prescaler configuration */
  TIM2_PrescalerConfig(TIM2_PRESCALER_2048, TIM2_PSCRELOADMODE_IMMEDIATE);

  /* Output Compare Active Mode configuration: Channel1 */
  /*
	  TIM2_OCMode = TIM2_OCMODE_ACTIVE
       TIM2_OutputState = TIM2_OUTPUTSTATE_ENABLE
       TIM2_Pulse = CCR1_Val
	 TIM2_OCPolarity = TIM2_OCPOLARITY_HIGH
	*/
	TIM2_OC1Init(TIM2_OCMODE_ACTIVE, TIM2_OUTPUTSTATE_ENABLE,CCR1_Val, TIM2_OCPOLARITY_HIGH);
	
  TIM2_OC1PreloadConfig(DISABLE);

  /* Output Compare Active Mode configuration: Channel2 */
  
  /*TIM2_Pulse = CCR2_Val  */
	
  TIM2_OC2Init(TIM2_OCMODE_ACTIVE, TIM2_OUTPUTSTATE_ENABLE,CCR2_Val, TIM2_OCPOLARITY_HIGH); 
  TIM2_OC2PreloadConfig(DISABLE);

  /* Output Compare Active Mode configuration: Channel3 */
  /*
	TIM2_Pulse = CCR3_Val
	*/
  TIM2_OC3Init(TIM2_OCMODE_ACTIVE, TIM2_OUTPUTSTATE_ENABLE,CCR3_Val, TIM2_OCPOLARITY_HIGH);
  TIM2_OC3PreloadConfig(DISABLE);
  TIM2_ARRPreloadConfig(ENABLE);

  /* Set PG.6 pin */
  GPIO_Init(GPIOG, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_WriteHigh(GPIOG, GPIO_PIN_6);
  
  /* TIM2 enable counter */
  TIM2_Cmd(ENABLE);

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
