/**
  ******************************************************************************
  * @file TIM1_ComplementarySignals_DeadTime_Break_Lock\main.c
  * @brief This file contains the main function for TIM1 Complementary Signals & DeadTime & Break & Lock.
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
  * @addtogroup TIM1_ComplementarySignals_DeadTime_Break_Lock
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define CCR1_Val  ((u16)32767)
#define CCR2_Val  ((u16)16383)
#define CCR3_Val  ((u16)8191)
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
	/*
  TIM1_Prescaler = 0
  TIM1_CounterMode = TIM1_COUNTERMODE_UP
  TIM1_Period = 65535
  TIM1_RepetitionCounter = 0
	*/

  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 65535,0);

  /* Channel 1, 2 and 3 Configuration in PWM mode */
  /*
	TIM1_OCMode = TIM1_OCMODE_PWM2
  TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
  TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
  TIM1_Pulse = CCR1_Val
  TIM1_OCPolarity = TIM1_OCPOLARITY_LOW 
  TIM1_OCNPolarity = TIM1_OCNPOLARITY_LOW        
  TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
  TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
	*/
  TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,  CCR1_Val, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_SET,  TIM1_OCNIDLESTATE_RESET);

  /* TIM1_Pulse = CCR2_Val */
  
	TIM1_OC2Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,  CCR2_Val, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_SET,  TIM1_OCNIDLESTATE_RESET);

  /* TIM1_Pulse = CCR3_Val */
	
  TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,  CCR3_Val, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_SET,  TIM1_OCNIDLESTATE_RESET);

  /* Automatic Output enable, Break, dead time and lock configuration*/
  /*
	TIM1_OSSIState = TIM1_OSSISTATE_ENABLE
  TIM1_LockLevel = TIM1_LOCKLEVEL_1
  TIM1_DeadTime = 117
  TIM1_Break = TIM1_BREAK_ENABLE
  TIM1_BreakPolarity = TIM1_BREAKPOLARITY_HIGH
  TIM1_AutomaticOutput = TIM1_AUTOMATICOUTPUT_ENABLE
	*/
	TIM1_BDTRConfig( TIM1_OSSISTATE_ENABLE,  TIM1_LOCKLEVEL_1, 117, TIM1_BREAK_ENABLE,  TIM1_BREAKPOLARITY_HIGH, TIM1_AUTOMATICOUTPUT_ENABLE);

  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);

  /* Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
  
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
