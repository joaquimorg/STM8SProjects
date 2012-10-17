/**
  ******************************************************************************
  * @file stm8s_awu.h
  * @brief This file contains all functions prototype and macros for the AWU peripheral.
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
#ifndef __STM8S_AWU_H
#define __STM8S_AWU_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/

/** @addtogroup AWU_Exported_Types
  * @{
  */

/**
  * @brief AWU TimeBase selection
  */

typedef enum
{
  AWU_TIMEBASE_NO_IT  = (u8)0,    /*!< No AWU interrupt selected */
  AWU_TIMEBASE_250US  = (u8)1,    /*!< AWU Timebase equals 0.25 ms */
  AWU_TIMEBASE_500US  = (u8)2,    /*!< AWU Timebase equals 0.5 ms */
  AWU_TIMEBASE_1MS    = (u8)3,    /*!< AWU Timebase equals 1 ms */
  AWU_TIMEBASE_2MS    = (u8)4,    /*!< AWU Timebase equals 2 ms */
  AWU_TIMEBASE_4MS    = (u8)5,    /*!< AWU Timebase equals 4 ms */
  AWU_TIMEBASE_8MS    = (u8)6,    /*!< AWU Timebase equals 8 ms */
  AWU_TIMEBASE_16MS   = (u8)7,    /*!< AWU Timebase equals 16 ms */
  AWU_TIMEBASE_32MS   = (u8)8,    /*!< AWU Timebase equals 32 ms */
  AWU_TIMEBASE_64MS   = (u8)9,    /*!< AWU Timebase equals 64 ms */
  AWU_TIMEBASE_128MS  = (u8)10,   /*!< AWU Timebase equals 128 ms */
  AWU_TIMEBASE_256MS  = (u8)11,   /*!< AWU Timebase equals 256 ms */
  AWU_TIMEBASE_512MS  = (u8)12,   /*!< AWU Timebase equals 512 ms */
  AWU_TIMEBASE_1S     = (u8)13,   /*!< AWU Timebase equals 1 s */
  AWU_TIMEBASE_2S     = (u8)14,   /*!< AWU Timebase equals 2 s */
  AWU_TIMEBASE_12S    = (u8)15,   /*!< AWU Timebase equals 12 s */
  AWU_TIMEBASE_30S    = (u8)16    /*!< AWU Timebase equals 30 s */
} AWU_Timebase_TypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @addtogroup AWU_Exported_Constants
  * @{
  */

#define LSI_FREQUENCY_MIN ((u32)100000) /*!< LSI minimum value in Hertz */
#define LSI_FREQUENCY_MAX ((u32)200000) /*!< LSI maximum value in Hertz */

/**
  * @}
  */

/* Exported macros ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/** @addtogroup AWU_Private_Macros
  * @{
  */

/**
  * @brief Macro used by the assert function to check the different functions parameters.
  */

/**
  * @brief  Macro used by the assert function to check the AWU timebases
  */
#define IS_AWU_TIMEBASE_OK(TB) \
  (((TB) == AWU_TIMEBASE_NO_IT) || \
   ((TB) == AWU_TIMEBASE_250US) || \
   ((TB) == AWU_TIMEBASE_500US) || \
   ((TB) == AWU_TIMEBASE_1MS)   || \
   ((TB) == AWU_TIMEBASE_2MS)   || \
   ((TB) == AWU_TIMEBASE_4MS)   || \
   ((TB) == AWU_TIMEBASE_8MS)   || \
   ((TB) == AWU_TIMEBASE_16MS)  || \
   ((TB) == AWU_TIMEBASE_32MS)  || \
   ((TB) == AWU_TIMEBASE_64MS)  || \
   ((TB) == AWU_TIMEBASE_128MS) || \
   ((TB) == AWU_TIMEBASE_256MS) || \
   ((TB) == AWU_TIMEBASE_512MS) || \
   ((TB) == AWU_TIMEBASE_1S)    || \
   ((TB) == AWU_TIMEBASE_2S)    || \
   ((TB) == AWU_TIMEBASE_12S)   || \
   ((TB) == AWU_TIMEBASE_30S))

/**
  * @brief   Macro used by the assert function to check the LSI frequency (in Hz)
  */
#define IS_LSI_FREQUENCY_OK(FREQ) \
  (((FREQ) >= LSI_FREQUENCY_MIN) && \
   ((FREQ) <= LSI_FREQUENCY_MAX))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup AWU_Exported_Functions
  * @{
  */
void AWU_DeInit(void);
void AWU_Init(AWU_Timebase_TypeDef AWU_TimeBase);
void AWU_Cmd(FunctionalState NewState);
void AWU_LSICalibrationConfig(u32 LSIFreqHz);
void AWU_IdleModeEnable(void);
void AWU_ReInitCounter(void);
FlagStatus AWU_GetFlagStatus(void);

/**
  * @}
  */

#endif /* __STM8S_AWU_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
