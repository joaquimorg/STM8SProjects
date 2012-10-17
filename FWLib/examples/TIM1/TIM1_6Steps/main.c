/**
  ******************************************************************************
  * @file TIM1_6Steps\main.c
  * @brief This file contains the main function for TIM1 6 Steps example.
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
  * @addtogroup TIM1_6Steps
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
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
  /* TIM1 Peripheral Configuration */ 
  TIM1_DeInit();

  /* Time Base configuration */
  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 4095, 0);

  /* Channel 1, 2 and 3 Configuration in TIMING mode */  
  
	/* TIM1_Pulse = 2047 */
	TIM1_OC1Init(TIM1_OCMODE_TIMING, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, 2047, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_SET);  

  /* TIM1_Pulse = 1023 */
  TIM1_OC2Init(TIM1_OCMODE_TIMING, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, 1023, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_SET); 

  /* TIM1_Pulse = 511 */
  TIM1_OC3Init(TIM1_OCMODE_TIMING, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, 511, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_SET); 

  /* Automatic Output enable, Break, dead time and lock configuration*/
  
	TIM1_BDTRConfig( TIM1_OSSISTATE_ENABLE,  TIM1_LOCKLEVEL_OFF, 1,  TIM1_BREAK_DISABLE,  TIM1_BREAKPOLARITY_LOW,  TIM1_AUTOMATICOUTPUT_ENABLE);

  TIM1_CCPreloadControl(ENABLE);

  TIM1_ITConfig(TIM1_IT_COM, ENABLE);

  
  
  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);
  
  /* TIM4 Peripheral Configuration */ 
  /* Time Base configuration */
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 0xFF);
  
  /*TIM4 counter enable */
  TIM4_Cmd(ENABLE);
  
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  /* Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
  enableInterrupts();
  
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
