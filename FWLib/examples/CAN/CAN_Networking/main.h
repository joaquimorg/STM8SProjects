	/**
  ******************************************************************************
  * @file Networking\main.h
  * @brief This file contains the external declaration of main.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include  "mono_lcd.h"

/* Private variables ---------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define Key_Pressed 0x01
#define Key_NoPressed 0x00
#define TIME 0x7FFF

/* Evalboard I/Os configuration */
#define LEDS_PORT (GPIOH)
#define LED1_PIN  (GPIO_PIN_3)
#define LED2_PIN  (GPIO_PIN_2)
#define LED3_PIN  (GPIO_PIN_1)
#define LED4_PIN  (GPIO_PIN_0)

#define BUTTON_PORT (GPIOC)
#define BUTTON_PIN  (GPIO_PIN_0)



/* Public function prototypes -----------------------------------------------*/
	void CLK_Config(void);
	void GPIO_Config(void);
  void CAN_Config(void);
	void LCD_Config(void);
  void LED_Display(u8 Ledstatus);
	void LCD_Display(u8 Ledstatus);
	void Delay (u16 nCount);


#endif /* __MAIN_H__ */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/	