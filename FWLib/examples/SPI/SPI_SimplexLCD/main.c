/**
  ******************************************************************************
  * @file SPI_SimplexLCD\main.c
  * @brief This file contains the main function for SPI simplex communication with LCD example.
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
#include "main.h"
#include <string.h>
#include <stdio.h>
/**
  * @addtogroup SPI_SimplexLCD
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
bool ButtonPressed = FALSE;
/* Public functions ----------------------------------------------------------*/

/**
  * @brief Validation firmware main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
    int ButtonIndex =0;
    u8 i =0;
    u8 LedState =1;
    u8 pos =0;
    u8 s[16];

    /* Initialize I/Os in Output Mode for LEDs */
    GPIO_Init(LEDS_PORT, LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN,GPIO_MODE_OUT_PP_LOW_FAST);

    /* Initialization of I/O in Input Mode with Interrupt for Key button */
    GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_FL_IT);

    /* Initialization of the Interrupt sensitivity */
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_ONLY);

    /* Enable general interrupts for Key button reading */
    enableInterrupts();

    /* Initialize SPI */
    SPI_DeInit();
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_128, SPI_MODE_MASTER,
             SPI_CLOCKPOLARITY_HIGH, SPI_CLOCKPHASE_2EDGE, SPI_DATADIRECTION_1LINE_TX,
             SPI_NSS_SOFT, 0x07);
    SPI_Cmd(ENABLE);

    /* Initialize LCD */
    LCD_Init();

    /* Clear LCD lines */
    LCD_Clear();

    /* Display "moving" ST logo on first line */
    for (pos = 0x80; pos < 0x88; pos++)
    {
        LCD_Clear();
        LCD_DisplayLogo(pos);
        Delay(20000);
    }

    /* Display "moving" ST logo on second line */
    for (pos = 0x90; pos < 0x98; pos++)
    {
        LCD_Clear();
        LCD_DisplayLogo(pos);
        Delay(20000);
    }

    LCD_Clear();

    /* More than 15 characters: the message is displayed on 2 lines */
    /* LCD_PrintMsg("STM8S - SPI/LCD example"); */

    /* Max 15 characters per line */
    LCD_PrintString(LCD_LINE1, ENABLE, DISABLE, " *** STM8S *** ");
    LCD_PrintString(LCD_LINE2, ENABLE, DISABLE, "SPI-LCD example");

    for (i = 0; i < 10; i++)
    {
        Delay(0xFFFF);
    }

    LCD_Clear();

    LCD_PrintString(LCD_LINE1, ENABLE, DISABLE, "Press KEY...");

    while (1)
    {
        /* Check button status */
        if (GetVar_ButtonPressed() == TRUE) /* Button is pressed */
        {
            ButtonPressed = FALSE;
            LCD_ClearLine(LCD_LINE2);
            LCD_PrintString(LCD_LINE2, DISABLE, DISABLE, "LED #");
						
           /*Use the  sprintf() and LCD_Print() function to print a digit on the LCD */
 					 if (LedState == 0)
            {
                sprintf(s,"%d ", ButtonIndex + 1);
                LCD_Print(s);
								LCD_SetCursorPos(LCD_LINE2, 4); /* 8th position */
                LCD_PrintChar('O');
                LCD_PrintChar('F');
                LCD_PrintChar('F');
            }
						/*Use the LCD_PrintDec1() function to print a digit on the LCD */
            else
            {
                LCD_PrintDec1((u8)(ButtonIndex + 1)); /* Display at current cursor position */
                LCD_SetCursorPos(LCD_LINE2, 4); /* 8th position */ 
								LCD_PrintChar('O');
                LCD_PrintChar('N');
                LCD_PrintChar(' ');
            }
             switch (ButtonIndex)
            {
            case 0:
                GPIO_WriteReverse(LEDS_PORT, LED1_PIN);
                ButtonIndex++;
                break;
            case 1:
                GPIO_WriteReverse(LEDS_PORT, LED2_PIN);
                ButtonIndex++;
                break;
            case 2:
                GPIO_WriteReverse(LEDS_PORT, LED3_PIN);
                ButtonIndex++;
                break;
            default:
            case 3:
                GPIO_WriteReverse(LEDS_PORT, LED4_PIN);
                ButtonIndex = 0;
                LedState ^= 0x01;
                break;
            }
        }
    }

}
/**
  * @brief Returns ButtonPressed value.
  * @par Parameters:
  * None
  * @retval check if button pressed
  * - bool: ButtonPressed
  * @par Required preconditions:
  * None
  */
bool GetVar_ButtonPressed(void)
{
    return ButtonPressed;
}
/**
  * @brief Set the ButtonPressed value.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void SetVar_ButtonPressed(void)
{
    ButtonPressed = TRUE;
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
