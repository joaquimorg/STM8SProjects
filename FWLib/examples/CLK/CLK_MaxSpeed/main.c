/**
  ******************************************************************************
  * @file CLK_MaxSpeed\main.c
  * @brief This file contains the main function for the CLK Maximum Speed configuration example.
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
  * @addtogroup CLK_MaxSpeed
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
bool ButtonPressed = FALSE;

/**
  * @brief Example firmware main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
    ErrorStatus status = FALSE;

    /* Initialization of I/O in Input Mode with Interrupt */
    GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_FL_IT);

    /* Initialization of the Interrupt sensitivity */
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_ONLY);

    /* Enable general interrupts */
    enableInterrupts();

    CLK_DeInit();

    /* Configure the Fcpu to DIV1*/
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
		
		/* Configure the HSI prescaler to the optimal value */
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);

    /* Output Fcpu on CLK_CCO pin */
    CLK_CCOConfig(CLK_OUTPUT_CPU);
				
    /* Initilize the CLock controller according to CLK_InitStructure */
    status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, DISABLE);
		
    while (ButtonPressed == FALSE);
		
    status = CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, DISABLE);
		
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
