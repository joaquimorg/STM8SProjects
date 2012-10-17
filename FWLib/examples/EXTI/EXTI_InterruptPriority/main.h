/**
  ******************************************************************************
  * @file EXTI_InterruptPriority\main.h
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

#define JOYSTICK_PORT      (GPIOB)
#define JOYSTICK_LEFT_PIN  (GPIO_PIN_4)
#define JOYSTICK_RIGHT_PIN (GPIO_PIN_5)
#define JOYSTICK_UP_PIN    (GPIO_PIN_6)
#define JOYSTICK_DOWN_PIN  (GPIO_PIN_7)

#define JOYSTICK_SEL_PORT (GPIOD)
#define JOYSTICK_SEL_PIN  (GPIO_PIN_7)
/* Private function prototypes -----------------------------------------------*/
void Delay (u16 nCount);
/* Private functions ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#endif /* __MAIN_H__ */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
