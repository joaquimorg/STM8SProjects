/**
  ******************************************************************************
  * @file I2C_Interrupt\main.c
  * @brief This file contains the main function for I2C Interrupt example.
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

/* Evalboard I/Os configuration */

#define LEDS_PORT (GPIOH)
#define LED1_PIN  (GPIO_PIN_3)
#define LED2_PIN  (GPIO_PIN_2)
#define LED3_PIN  (GPIO_PIN_1)
#define LED4_PIN  (GPIO_PIN_0)

/**
  * @addtogroup I2C_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
bool EndCommunication = FALSE;
/* Private function prototypes -----------------------------------------------*/
void Delay (u16 nCount);

/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief How to use I2C in master transmitter with interrupt.
  * @par Examples description
  * - Send value 0x5A to slave using interrupts.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
  u8 Input_Clock = 0x0;
  
    /* Initialize I/Os in Output Mode */
    GPIO_Init(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN), GPIO_MODE_OUT_PP_HIGH_FAST);

    /* Get system clock frequency */
    Input_Clock = CLK_GetClockFreq()/1000000;
    
    /* Optional: put here as example only */
    I2C_DeInit();

    /* Initialize the I2C */
    I2C_Init(100000, 0xA0, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, Input_Clock);

    /* Configuration of interrupts only */
    I2C_ITConfig((I2C_IT_EVT |I2C_IT_BUF), ENABLE);

    enableInterrupts();

    /*----- Transmission Phase -----*/
    /* Send START condition */
    I2C_GenerateSTART(ENABLE);
  
			/* Wait end of communication or mark communication error */
			while(EndCommunication != TRUE)
			{
        GPIO_WriteReverse(LEDS_PORT, LED1_PIN);
				Delay((u16)60000);
			}
	
    /* All LEDs are ON per default */
    GPIO_WriteHigh(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN));
		
		/* Toggle all LEDs when I2C communication is terminated */
		while(1)
		{
		GPIO_WriteReverse(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN));
		Delay((u16)60000);
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
