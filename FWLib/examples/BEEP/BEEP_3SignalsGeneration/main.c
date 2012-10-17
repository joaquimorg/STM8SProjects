/**
  ******************************************************************************
  * @file BEEP_3SignalsGeneration\main.c
  * @brief This file contains the main function for BEEP 3 Signals Generation example
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
  * @addtogroup BEEP_3SignalsGeneration
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
/* Private function prototypes -----------------------------------------------*/
void Delay (u16 nCount);
u32 LSIMeasurment(void);
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
bool ButtonPressed = FALSE;

/* Public functions ----------------------------------------------------------*/

/**
  * @brief Example main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{

    u8 i = 0;

    /* Initialization of the clock */
    /* Clock divider to HSI/1 */
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    /* Initialization of I/Os in Output Mode */
    GPIO_Init(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN), GPIO_MODE_OUT_PP_LOW_FAST);

    /* Initialization of I/O in Input Mode with Interrupt */
    GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_FL_IT);

    /* Initialization of the Interrupt sensitivity */
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_ONLY);

    /* BEEP calibration */
    
    BEEP_LSICalibrationConfig(LSIMeasurment());
    
    GPIO_Write(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN)); /* All LEDs ON */

    /* Initialize SPI for LCD */
    SPI_DeInit();
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_128, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_HIGH, SPI_CLOCKPHASE_2EDGE, SPI_DATADIRECTION_1LINE_TX, SPI_NSS_SOFT, 0x07);
    SPI_Cmd(ENABLE);

    /* Initialize LCD */
    LCD_Init();

    /* Clear LCD lines */
    LCD_Clear();

    /* Enable general interrupts for Key button reading */
    enableInterrupts();

    LCD_PrintString(LCD_LINE1, ENABLE, DISABLE, "  BEEPER OFF");
    LCD_PrintString(LCD_LINE2, ENABLE, DISABLE, "   Press Key");

    while (1)
    {
        /* Check button status */
        if (ButtonPressed == TRUE) /* Button is pressed */
        {
            ButtonPressed = FALSE;
            /* Change BEEP frequency */
            switch (i)
            {
            case 0:
                BEEP_Cmd(DISABLE);
                Delay(100);
                BEEP_Init(BEEP_FREQUENCY_1KHZ);
                BEEP_Cmd(ENABLE);
                GPIO_Write(LEDS_PORT, LED1_PIN); /* LED1 ON */
                LCD_PrintString(LCD_LINE1, ENABLE, DISABLE, "  BEEPER 1kHz");
                i = 1;
                break;
            case 1:
                BEEP_Cmd(DISABLE);
                Delay(100);
                BEEP_Init(BEEP_FREQUENCY_2KHZ);
                BEEP_Cmd(ENABLE);
                GPIO_Write(LEDS_PORT, LED2_PIN); /* LED2 ON */
                LCD_PrintString(LCD_LINE1, ENABLE, DISABLE, "  BEEPER 2kHz");
                i = 2;
                break;
            case 2:
                BEEP_Cmd(DISABLE);
                Delay(100);
                BEEP_Init(BEEP_FREQUENCY_4KHZ);
                BEEP_Cmd(ENABLE);
                GPIO_Write(LEDS_PORT, LED3_PIN); /* LED3 ON */
                LCD_PrintString(LCD_LINE1, ENABLE, DISABLE, "  BEEPER 4kHz");
                i = 3;
                break;
            case 3:
                BEEP_Cmd(DISABLE);
                GPIO_Write(LEDS_PORT, LED4_PIN); /* LED4 ON */
                LCD_PrintString(LCD_LINE1, ENABLE, DISABLE, "  BEEPER OFF");
                i = 0;
                break;
						default:
						    break;
            }
        }

    }

}
/**
  * @brief Measure the LSI frequency using timer IC1 and update the calibration registers.
  * @par Parameters:
  * None
  * @retval 
	* None
  * @par Required preconditions:
  * It is recommanded to use a timer clock frequency of at least 10MHz in order 
	* to obtain a better in the LSI frequency measurement.
  */
u32 LSIMeasurment(void)
{

  u32 lsi_freq_hz = 0x0;
  u32 fmaster = 0x0;
  u16 ICValue1 = 0x0;
  u16 ICValue2 = 0x0;

  /* Get master frequency */
  fmaster = CLK_GetClockFreq();

  /* Enable the LSI measurement: LSI clock connected to timer Input Capture 1 */
  AWU->CSR |= AWU_CSR_MSR;

#if defined (STM8S903) || defined (STM8S103)
  /* Measure the LSI frequency with TIMER Input Capture 1 */
  
  /* Capture only every 8 events!!! */
  /* Enable capture of TI1 */
	TIM1_ICInit(TIM1_CHANNEL_1, TIM1_ICPOLARITY_RISING, TIM1_ICSELECTION_DIRECTTI, TIM1_ICPSC_DIV8, 0);
	
  /* Enable TIM1 */
  TIM1_Cmd(ENABLE);
  
  /* wait a capture on cc1 */
  while((TIM1->SR1 & TIM1_FLAG_CC1) != TIM1_FLAG_CC1);
  /* Get CCR1 value*/
  ICValue1 = TIM1_GetCapture1();
  TIM1_ClearFlag(TIM1_FLAG_CC1);
  
  /* wait a capture on cc1 */
  while((TIM1->SR1 & TIM1_FLAG_CC1) != TIM1_FLAG_CC1);
  /* Get CCR1 value*/
  ICValue2 = TIM1_GetCapture1();
  TIM1_ClearFlag(TIM1_FLAG_CC1);
  
  /* Disable IC1 input capture */
  TIM1->CCER1 &= (u8)(~TIM1_CCER1_CC1E);
  /* Disable timer2 */
  TIM1_Cmd(DISABLE);
  
#else  
  /* Measure the LSI frequency with TIMER Input Capture 1 */
  
  /* Capture only every 8 events!!! */
  /* Enable capture of TI1 */
  TIM3_ICInit(TIM3_CHANNEL_1, TIM3_ICPOLARITY_RISING, TIM3_ICSELECTION_DIRECTTI, TIM3_ICPSC_DIV8, 0);

  /* Enable TIM3 */
  TIM3_Cmd(ENABLE);

	/* wait a capture on cc1 */
  while ((TIM3->SR1 & TIM3_FLAG_CC1) != TIM3_FLAG_CC1);
	/* Get CCR1 value*/
  ICValue1 = TIM3_GetCapture1();
  TIM3_ClearFlag(TIM3_FLAG_CC1);

  /* wait a capture on cc1 */
  while ((TIM3->SR1 & TIM3_FLAG_CC1) != TIM3_FLAG_CC1);
    /* Get CCR1 value*/
  ICValue2 = TIM3_GetCapture1();
	TIM3_ClearFlag(TIM3_FLAG_CC1);

  /* Disable IC1 input capture */
  TIM3->CCER1 &= (u8)(~TIM3_CCER1_CC1E);
  /* Disable timer3 */
  TIM3_Cmd(DISABLE);
#endif

  /* Compute LSI clock frequency */
  lsi_freq_hz = (8 * fmaster) / (ICValue2 - ICValue1);
  
  /* Disable the LSI measurement: LSI clock disconnected from timer Input Capture 1 */
  AWU->CSR &= (u8)(~AWU_CSR_MSR);

 return (lsi_freq_hz);
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
