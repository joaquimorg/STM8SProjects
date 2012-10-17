/**
  ********************************************************************************
  * @file stm8s_wwdg.h
  * @brief This file contains all functions prototype and macros for the WWDG peripheral.
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
#ifndef __STM8S_WWDG_H
#define __STM8S_WWDG_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Private macros ------------------------------------------------------------*/

/** @addtogroup WWDG_Private_Macros
  * @{
  */

/**
  * @brief Macro used by the assert function in order to check the
  * values of the window register.
  */
#define IS_WWDG_WINDOWLIMITVALUE_OK(WindowLimitValue) (WindowLimitValue <= 0x7F)

/**
  * @brief Macro used by the assert function in order to check the different
  * values of the counter register.
  */
#define IS_WWDG_COUNTERVALUE_OK(CounterValue) (CounterValue <= 0x7F)

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/** @addtogroup WWDG_Exported_Functions
  * @{
  */

void WWDG_Init(u8 Counter, u8 WindowValue);
void WWDG_SetCounter(u8 Counter);
u8 WWDG_GetCounter(void);
void WWDG_SWReset(void);
void WWDG_SetWindowValue(u8 WindowValue);


/**
  * @}
  */

#endif /* __STM8S_WWDG_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
