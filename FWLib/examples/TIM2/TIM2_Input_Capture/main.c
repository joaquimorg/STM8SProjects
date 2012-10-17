/**
  ******************************************************************************
  * @file TIM2_Input_Capture\main.c
  * @brief This file contains the main function for TIM2 Input Capture example.
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
  * @addtogroup TIM2_Input_Capture
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM2ClockFreq  ((u32)2000000)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32 LSIClockFreq = 0;
u16 ICValue1 =0, ICValue2 =0;
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
 
	/* Capture only every 8 events!!! */
  /* Enable capture of TI1 */
	TIM2_ICInit(TIM2_CHANNEL_1, TIM2_ICPOLARITY_FALLING, TIM2_ICSELECTION_DIRECTTI, TIM2_ICPSC_DIV8, 0x00);
  
	/* Enable CC1 interrupt */
	TIM2_ITConfig(TIM2_IT_CC1, ENABLE); 
	
	/* Enable TIM2 */
  TIM2_Cmd(ENABLE);
		
	/* Clear CC1 Flag*/
  TIM2_ClearFlag(TIM2_FLAG_CC1);
  
	/* Connect LSI to COO pin*/
  GPIO_Init(GPIOE, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
	CLK_CCOConfig(CLK_OUTPUT_LSI);
	CLK_CCOCmd(ENABLE);
	
	
	/* wait a capture on CC1 */
  while((TIM2->SR1 & TIM2_FLAG_CC1) != TIM2_FLAG_CC1);
  /* Get CCR1 value*/
	ICValue1 = TIM2_GetCapture1();
	TIM2_ClearFlag(TIM2_FLAG_CC1);
  
	/* wait a capture on cc1 */
  while((TIM2->SR1 & TIM2_FLAG_CC1) != TIM2_FLAG_CC1);
  /* Get CCR1 value*/
	ICValue2 = TIM2_GetCapture1();
	TIM2_ClearFlag(TIM2_FLAG_CC1);

  /* Compute LSI clock frequency */
  LSIClockFreq = (8 * TIM2ClockFreq) / (ICValue2 - ICValue1);
  
	/* Insert a break point here */
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
