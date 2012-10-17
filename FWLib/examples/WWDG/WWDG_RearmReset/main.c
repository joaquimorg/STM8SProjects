/**
  ******************************************************************************
  * @file WWDG_RearmReset\main.c
  * @brief This file contains the main function for wwdg Rearm Reset example.
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
  * @addtogroup WWDG_RearmReset
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Evalboard I/Os configuration */
#define LEDS_PORT 				(GPIOH)
#define LED1_PIN  				(GPIO_PIN_3)
#define LED2_PIN  				(GPIO_PIN_2)
#define LED3_PIN  				(GPIO_PIN_1)
#define LED4_PIN  				(GPIO_PIN_0)

#define JOYSTICK_SEL_PORT	(GPIOD)
#define JOYSTICK_SEL_PIN	(GPIO_PIN_7)

#define BUTTON_PORT 			(GPIOC)
#define BUTTON_PIN  			(GPIO_PIN_0)

/* Private macro -------------------------------------------------------------*/
#define Key_is_pressed ((GPIO_ReadInputData(BUTTON_PORT) & BUTTON_PIN) == (u8)0x00)
#define JoySelect_is_pressed ((GPIO_ReadInputData(JOYSTICK_SEL_PORT) & JOYSTICK_SEL_PIN) == (u8)0x00)

/* Private variables ---------------------------------------------------------*/
u8 window = 0x77;
u8 CounterInit = 0x7F;
u16 TIME = 0xFFFF;
u8 CounterValue = 0;
FunctionalState Refresh;
FunctionalState NonAlowedRefresh;
/* Private function prototypes -----------------------------------------------*/
void Delay (u16 nCount);
void GPIO_Configuration(void);
void WWDG_Configuration(void);
void Test_WWDGReset(void);

/**
  * @brief Example firmware main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{

  /* GPIO Configuration */
  GPIO_Configuration();

  /*Test if Reset occured*/
  Test_WWDGReset();

  /* WWDG Configuration */
  WWDG_Configuration();
	  
	
	/* All LEDs ON */
  GPIO_WriteHigh(LEDS_PORT, LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN );
	
	/* Enable WWDG safe Refresh */
  Refresh = ENABLE;
	NonAlowedRefresh = DISABLE;
	
  while (1)
  {
/* Refresh during the Non Alowed Window => WWDG RESET */
		if (Key_is_pressed)
		{
			/* ------------- Button is pressed ------------- */	
			/* Reverse LED1 and LED4 status */
			GPIO_WriteReverse(LEDS_PORT, (LED1_PIN | LED4_PIN));
			Delay(TIME);
			
			/* Authorize WWDG refresh on non alowed window */ 
			NonAlowedRefresh = ENABLE;
	  }
		
/* No Refresh => WWDG RESET */
		if (JoySelect_is_pressed)
		{
			/* ------------- JOYSTICK SELECT button is pressed ------------- */	
      
			/* Disable Refresh */
			Refresh = DISABLE;
			
			/* Reverse LED2 and LED3 status */
			GPIO_WriteReverse(LEDS_PORT, (LED2_PIN | LED3_PIN));
			Delay(TIME);
	  }
	
		if (Refresh != DISABLE)
		{
			CounterValue = (u8)(WWDG_GetCounter() & 0x7F);
			if (NonAlowedRefresh != DISABLE)
			{
				if(CounterValue > window)
				{
					WWDG_SetCounter(CounterInit);
				}
			}
			else
			{
				if(CounterValue < window)
				{
					WWDG_SetCounter(CounterInit);
				}
			}
		}
 }
}
/* Private functions ---------------------------------------------------------*/
void Delay(u16 nCount) 
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}


/**
  * @brief Configures the WWDG to generate a Reset if the WWDG is not refreched during the correct window.
  * @par Parameters:
  * None
  * @retval 
  * None
  */

void WWDG_Configuration(void) 
{
    /* WWDG Configuration */
    /* Watchdog Window= 0x7F step to 0x3F step
    									= (0x7F-0x3F) * 1 step
    									= 64 * 1 step
    									= 64 * (12288/2Mhz)
    									= 393,216ms
    */
    /* Allowed Window = (0x7F-window) * 1 step
		                  = (0x7F-0x77) * 1 step
    									= 7 * 1 step 
    									= 7 * (12288/2Mhz) 
    									= 43.008ms 
    */
    /* So the non allowed window starts from 0.0ms to 43.008ms
    and the alowed window starts from 43.008ms to 393,216ms */
    WWDG_Init(CounterInit,window);
}

/**
  * @brief Configures the Leds and the Buttons IO.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void GPIO_Configuration(void) 
{
  

  /* LEDS: Initialize I/Os in Output Mode */
  GPIO_Init(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN), GPIO_MODE_OUT_PP_LOW_FAST);
	
	/* Button: Initialize I/O in Input Mode with No Interrupt */

  GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_FL_NO_IT);
	
	/* JOYSTICK_SEL: Initialize I/O in Input Mode with No Interrupt */
  GPIO_Init(JOYSTICK_SEL_PORT, JOYSTICK_SEL_PIN, GPIO_MODE_IN_FL_NO_IT);
}

/**
  * @brief Tests if the Microcontroller has occured an WWDG Reset, If Yes, Leds are Toggled n times.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void Test_WWDGReset(void)
{
  FlagStatus WwdgFlag;
	u8 i=0;
	u8 n=2;
  u8 Leds = (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN);
  
  /*Get WWDG Reset Status */ 
	WwdgFlag = RST_GetFlagStatus(RST_FLAG_WWDGF);

  
  /* Test if a WWDG Reset has occured */
	if (WwdgFlag)
  {
		 /* Clear IWDGF Flag */
		RST_ClearFlag(RST_FLAG_WWDGF);
		
		for(i=0;i<n;i++)
		{
			/* ---------- An WWDG Reset has occured, Toggles LD1, LD2, LD3 and LD4 ------- */
			/* LEDs on */
			GPIO_WriteHigh(LEDS_PORT, Leds);
			/* delay */		
			Delay(TIME);
			Delay(TIME);
			/* LEDs off */
			GPIO_WriteLow(LEDS_PORT, Leds); 
			/* Delay */		
			Delay(TIME);
			Delay(TIME);
		}
	}
}

/* Public functions ----------------------------------------------------------*/

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
