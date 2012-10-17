/**
  ******************************************************************************
  * @file RST_IWDG\main.c
  * @brief This file contains the main function for RST IWDG example.
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
  * @addtogroup RST_IWDG
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Evalboard I/Os configuration */

#define LEDS_PORT (GPIOH)
#define LED1_PIN  (GPIO_PIN_3)
#define LED2_PIN  (GPIO_PIN_2)
#define LED3_PIN  (GPIO_PIN_1)
#define LED4_PIN  (GPIO_PIN_0)

#define BUTTON_PORT (GPIOC)
#define BUTTON_PIN  (GPIO_PIN_0)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FunctionalState Refresh;
u16 TIME = 0xFFFF;
/* Private function prototypes -----------------------------------------------*/
void Delay (u16 nCount);
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
    FlagStatus IwdgFlag;
    u8 Leds = (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN); /* Contains which LEDs to operate */
    u8 i=0;
    /* GPIO Configuration */
    /* Initialize I/Os in Output Mode */
    GPIO_Init(LEDS_PORT, LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
    /* Initialize I/O in Input Mode with Interrupt */
    GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_FL_NO_IT);

    /*Get IWDG Reset Status */
    IwdgFlag=RST_GetFlagStatus(RST_FLAG_IWDGF);

    /* Test if a IWDG Reset has occured */
    if (IwdgFlag)
    {
        for (i=0;i<5;i++)
        {
            /* ---------- An IWDG Reset has occured, Toggles LD1, LD2, LD3 and LD4 ------- */
            /* LEDs on */
            GPIO_WriteHigh(LEDS_PORT, Leds);
            /* delay */
            Delay(TIME);
            /* LEDs off */
            GPIO_WriteLow(LEDS_PORT, Leds);
            /* delay */
            Delay(TIME);
        }
        /* Clear IWDGF Flag */
        RST_ClearFlag(RST_FLAG_IWDGF);
    }

    /* TIM4 Configuration */
    TIM4_DeInit();
    /* Time base configuration */
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 0xFF);

    /* Enable TIM4 Update Interrupt*/
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    /* Enable TIM4*/
    TIM4_Cmd(ENABLE);

    /* Enable general interrupts for Timer 4 and Push Button on eval board */
    enableInterrupts();

    /* IWDG Configuration */
    /* Enable the IWDG*/
    IWDG_Enable();
    /* Enable the access to the IWDG registers*/
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    /* Fixe IWDG Reset period */
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(0xFF);
    /* Refresh IWDG */
    IWDG_ReloadCounter();

    /* Enable IWDG Refresh */
    Refresh = ENABLE;

    /* LEDs ON */
    GPIO_WriteHigh(LEDS_PORT, Leds);
    while (1)
    {
        if ((GPIO_ReadInputData(BUTTON_PORT) & BUTTON_PIN) == (u8)0x00)
        {
            /* ------ Button is pressed ------ */
            /* Disable IWDG Refresh */
            Refresh = DISABLE;
            /* LEDs OFF */
            GPIO_WriteLow(LEDS_PORT, Leds);
        }
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
