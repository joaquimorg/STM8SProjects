/**
  ******************************************************************************
  * @file CLK_ClockSelection\main.c
  * @brief This file contains the main function for the CLK Clock Selection example.
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
#include "mono_lcd.h"

/**
  * @addtogroup CLK_ClockSelection
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief Example  main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{

    /* Initialize SPI for LCD */
    SPI_DeInit();
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_128, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_HIGH, SPI_CLOCKPHASE_2EDGE, SPI_DATADIRECTION_1LINE_TX, SPI_NSS_SOFT, 0x07);
    SPI_Cmd(ENABLE);

    
    /* Configure GPIO used to drive the joystick
	      JOYSTICK_UP    --> PB6
	      JOYSTICK_DOWN  --> PB7
	      JOYSTICK_LEFT --> PB4
	*/
  GPIO_Init(GPIOB, GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_MODE_IN_FL_IT);
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_FALL_LOW);
	
	enableInterrupts();
    
    
    /* Initialize LCD */
    LCD_Init();

    /* Clear LCD lines */
    LCD_Clear();

    LCD_Backlight(ENABLE);

    LCD_PrintString(LCD_LINE1, ENABLE, DISABLE, "Clock Selection");
    LCD_PrintString(LCD_LINE2, ENABLE, DISABLE, " Use joystick");

    CLK_DeInit();
		CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    /* Output Fcpu on CLK_CCO pin */
    CLK_CCOConfig(CLK_OUTPUT_MASTER);

    while (1)
    {
  
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