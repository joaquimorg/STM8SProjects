/**
  ******************************************************************************
  * @file stm8s_tim2.h
  * @brief This file contains all functions prototype and macros for the TIM2 peripheral.
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
#ifndef __STM8S_TIM2_H
#define __STM8S_TIM2_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/


/** TIM2 Forced Action */
typedef enum
{
  TIM2_FORCEDACTION_ACTIVE           = ((u8)0x50),
  TIM2_FORCEDACTION_INACTIVE         = ((u8)0x40)
}TIM2_ForcedAction_TypeDef;

#define IS_TIM2_FORCED_ACTION_OK(ACTION) ((ACTION == TIM2_FORCEDACTION_ACTIVE) || \
    (ACTION == TIM2_FORCEDACTION_INACTIVE))

/** TIM2 Prescaler */
typedef enum
{
  TIM2_PRESCALER_1  = ((u8)0x00),
  TIM2_PRESCALER_2    = ((u8)0x01),
  TIM2_PRESCALER_4    = ((u8)0x02),
  TIM2_PRESCALER_8     = ((u8)0x03),
  TIM2_PRESCALER_16   = ((u8)0x04),
  TIM2_PRESCALER_32     = ((u8)0x05),
  TIM2_PRESCALER_64    = ((u8)0x06),
  TIM2_PRESCALER_128   = ((u8)0x07),
  TIM2_PRESCALER_256   = ((u8)0x08),
  TIM2_PRESCALER_512   = ((u8)0x09),
  TIM2_PRESCALER_1024  = ((u8)0x0A),
  TIM2_PRESCALER_2048 = ((u8)0x0B),
  TIM2_PRESCALER_4096   = ((u8)0x0C),
  TIM2_PRESCALER_8192 = ((u8)0x0D),
  TIM2_PRESCALER_16384 = ((u8)0x0E),
  TIM2_PRESCALER_32768 = ((u8)0x0F)
}TIM2_Prescaler_TypeDef;

#define IS_TIM2_PRESCALER_OK(PRESCALER) (((PRESCALER) == TIM2_PRESCALER_1  ) || \
    ((PRESCALER) == TIM2_PRESCALER_2    ) || \
    ((PRESCALER) == TIM2_PRESCALER_4    ) || \
    ((PRESCALER) == TIM2_PRESCALER_8  ) || \
    ((PRESCALER) == TIM2_PRESCALER_16   ) || \
    ((PRESCALER) == TIM2_PRESCALER_32     ) || \
    ((PRESCALER) == TIM2_PRESCALER_64    ) || \
    ((PRESCALER) == TIM2_PRESCALER_128   ) || \
    ((PRESCALER) == TIM2_PRESCALER_256   ) || \
    ((PRESCALER) == TIM2_PRESCALER_512   ) || \
    ((PRESCALER) == TIM2_PRESCALER_1024  ) || \
    ((PRESCALER) == TIM2_PRESCALER_2048 ) || \
    ((PRESCALER) == TIM2_PRESCALER_4096 ) || \
    ((PRESCALER) == TIM2_PRESCALER_8192 ) || \
    ((PRESCALER) == TIM2_PRESCALER_16384 ) || \
    ((PRESCALER) == TIM2_PRESCALER_32768 ))

/** TIM2 Output Compare and PWM modes */
typedef enum
{
  TIM2_OCMODE_TIMING     = ((u8)0x00),
  TIM2_OCMODE_ACTIVE     = ((u8)0x10),
  TIM2_OCMODE_INACTIVE   = ((u8)0x20),
  TIM2_OCMODE_TOGGLE     = ((u8)0x30),
  TIM2_OCMODE_PWM1       = ((u8)0x60),
  TIM2_OCMODE_PWM2       = ((u8)0x70)
}TIM2_OCMode_TypeDef;

#define IS_TIM2_OC_MODE_OK(MODE) (((MODE) ==  TIM2_OCMODE_TIMING) || \
                                  ((MODE) == TIM2_OCMODE_ACTIVE) || \
                                  ((MODE) == TIM2_OCMODE_INACTIVE) || \
                                  ((MODE) == TIM2_OCMODE_TOGGLE)|| \
                                  ((MODE) == TIM2_OCMODE_PWM1) || \
                                  ((MODE) == TIM2_OCMODE_PWM2))

#define IS_TIM2_OCM_OK(MODE)(((MODE) ==  TIM2_OCMODE_TIMING) || \
                             ((MODE) == TIM2_OCMODE_ACTIVE) || \
                             ((MODE) == TIM2_OCMODE_INACTIVE) || \
                             ((MODE) == TIM2_OCMODE_TOGGLE)|| \
                             ((MODE) == TIM2_OCMODE_PWM1) || \
                             ((MODE) == TIM2_OCMODE_PWM2) || \
                             ((MODE) == TIM2_FORCEDACTION_ACTIVE) || \
                             ((MODE) == TIM2_FORCEDACTION_INACTIVE))

/** TIM2 One Pulse Mode */
typedef enum
{
  TIM2_OPMODE_SINGLE                 = ((u8)0x01),
  TIM2_OPMODE_REPETITIVE             = ((u8)0x00)
}TIM2_OPMode_TypeDef;

#define IS_TIM2_OPM_MODE_OK(MODE) (((MODE) == TIM2_OPMODE_SINGLE) || \
                                   ((MODE) == TIM2_OPMODE_REPETITIVE))

/** TIM2 Channel */
typedef enum
{
  TIM2_CHANNEL_1                     = ((u8)0x00),
  TIM2_CHANNEL_2                     = ((u8)0x01),
  TIM2_CHANNEL_3                     = ((u8)0x02)
}TIM2_Channel_TypeDef;

#define IS_TIM2_CHANNEL_OK(CHANNEL) (((CHANNEL) == TIM2_CHANNEL_1) || \
                                     ((CHANNEL) == TIM2_CHANNEL_2) || \
                                     ((CHANNEL) == TIM2_CHANNEL_3))

#define IS_TIM2_PWMI_CHANNEL_OK(CHANNEL) (((CHANNEL) == TIM2_CHANNEL_1) || \
    ((CHANNEL) == TIM2_CHANNEL_2))

/** TIM2 Output Compare Polarity */
typedef enum
{
  TIM2_OCPOLARITY_HIGH               = ((u8)0x00),
  TIM2_OCPOLARITY_LOW                = ((u8)0x22)
}TIM2_OCPolarity_TypeDef;

#define IS_TIM2_OC_POLARITY_OK(POLARITY) (((POLARITY) == TIM2_OCPOLARITY_HIGH) || \
    ((POLARITY) == TIM2_OCPOLARITY_LOW))

/** TIM2 Output Compare states */
typedef enum
{
  TIM2_OUTPUTSTATE_DISABLE           = ((u8)0x00),
  TIM2_OUTPUTSTATE_ENABLE            = ((u8)0x11)
}TIM2_OutputState_TypeDef;

#define IS_TIM2_OUTPUT_STATE_OK(STATE) (((STATE) == TIM2_OUTPUTSTATE_DISABLE) || \
                                        ((STATE) == TIM2_OUTPUTSTATE_ENABLE))

/** TIM2 Input Capture Polarity */
typedef enum
{
  TIM2_ICPOLARITY_RISING            = ((u8)0x00),
  TIM2_ICPOLARITY_FALLING           = ((u8)0x44)
}TIM2_ICPolarity_TypeDef;

#define IS_TIM2_IC_POLARITY_OK(POLARITY) (((POLARITY) == TIM2_ICPOLARITY_RISING) || \
    ((POLARITY) == TIM2_ICPOLARITY_FALLING))

/** TIM2 Input Capture Selection */
typedef enum
{
  TIM2_ICSELECTION_DIRECTTI          = ((u8)0x01),
  TIM2_ICSELECTION_INDIRECTTI        = ((u8)0x02),
  TIM2_ICSELECTION_TRGI              = ((u8)0x03)
}TIM2_ICSelection_TypeDef;

#define IS_TIM2_IC_SELECTION_OK(SELECTION) (((SELECTION) == TIM2_ICSELECTION_DIRECTTI) || \
    ((SELECTION) == TIM2_ICSELECTION_INDIRECTTI) || \
    ((SELECTION) == TIM2_ICSELECTION_TRGI))

#define IS_TIM2_IC_SELECTION1_OK(SELECTION) (((SELECTION) == TIM2_ICSELECTION_DIRECTTI) || \
    ((SELECTION) == TIM2_ICSELECTION_TRGI))

/** TIM2 Input Capture Prescaler */
typedef enum
{
  TIM2_ICPSC_DIV1                    = ((u8)0x00),
  TIM2_ICPSC_DIV2                    = ((u8)0x04),
  TIM2_ICPSC_DIV4                    = ((u8)0x08),
  TIM2_ICPSC_DIV8                    = ((u8)0x0C)
}TIM2_ICPSC_TypeDef;

#define IS_TIM2_IC_PRESCALER_OK(PRESCALER) (((PRESCALER) == TIM2_ICPSC_DIV1) || \
    ((PRESCALER) == TIM2_ICPSC_DIV2) || \
    ((PRESCALER) == TIM2_ICPSC_DIV4) || \
    ((PRESCALER) == TIM2_ICPSC_DIV8))

/** TIM2 Input Capture Filer Value */
#define IS_TIM2_IC_FILTER_OK(ICFILTER) ((ICFILTER) <= 0x0F)

/** TIM2 interrupt sources */
typedef enum
{
  TIM2_IT_UPDATE                     = ((u8)0x01),
  TIM2_IT_CC1                        = ((u8)0x02),
  TIM2_IT_CC2                        = ((u8)0x04),
  TIM2_IT_CC3                        = ((u8)0x08)
}TIM2_IT_TypeDef;

#define IS_TIM2_IT_OK(IT) (((IT) != 0x00) && ((IT) <= 0x0F))

#define IS_TIM2_GET_IT_OK(IT) (((IT) == TIM2_IT_UPDATE) || \
                               ((IT) == TIM2_IT_CC1) || \
                               ((IT) == TIM2_IT_CC2) || \
                               ((IT) == TIM2_IT_CC3))

/** TIM2 Prescaler Reload Mode */
typedef enum
{
  TIM2_PSCRELOADMODE_UPDATE          = ((u8)0x00),
  TIM2_PSCRELOADMODE_IMMEDIATE       = ((u8)0x01)
}TIM2_PSCReloadMode_TypeDef;

#define IS_TIM2_PRESCALER_RELOAD_OK(RELOAD) (((RELOAD) == TIM2_PSCRELOADMODE_UPDATE) || \
    ((RELOAD) == TIM2_PSCRELOADMODE_IMMEDIATE))

/** TIM2 Event Source */
typedef enum
{
  TIM2_EVENTSOURCE_UPDATE            = ((u8)0x01),
  TIM2_EVENTSOURCE_CC1               = ((u8)0x02),
  TIM2_EVENTSOURCE_CC2               = ((u8)0x04),
  TIM2_EVENTSOURCE_CC3               = ((u8)0x08)
}TIM2_EventSource_TypeDef;

#define IS_TIM2_EVENT_SOURCE_OK(SOURCE) ((SOURCE != 0x00))

/** TIM2 Update Source */
typedef enum
{
  TIM2_UPDATESOURCE_GLOBAL           = ((u8)0x00),
  TIM2_UPDATESOURCE_REGULAR          = ((u8)0x01)
}TIM2_UpdateSource_TypeDef;

#define IS_TIM2_UPDATE_SOURCE_OK(SOURCE) (((SOURCE) == TIM2_UPDATESOURCE_GLOBAL) || \
    ((SOURCE) == TIM2_UPDATESOURCE_REGULAR))

/** TIM2 Flags */
typedef enum
{
  TIM2_FLAG_UPDATE                   = ((u16)0x0001),
  TIM2_FLAG_CC1                      = ((u16)0x0002),
  TIM2_FLAG_CC2                      = ((u16)0x0004),
  TIM2_FLAG_CC3                      = ((u16)0x0008),
  TIM2_FLAG_CC1OF                    = ((u16)0x0200),
  TIM2_FLAG_CC2OF                    = ((u16)0x0400),
  TIM2_FLAG_CC3OF                    = ((u16)0x0800)
}TIM2_FLAG_TypeDef;

#define IS_TIM2_GET_FLAG_OK(FLAG) (((FLAG) == TIM2_FLAG_UPDATE) || \
                                   ((FLAG) == TIM2_FLAG_CC1) || \
                                   ((FLAG) == TIM2_FLAG_CC2) || \
                                   ((FLAG) == TIM2_FLAG_CC3) || \
                                   ((FLAG) == TIM2_FLAG_CC1OF) || \
                                   ((FLAG) == TIM2_FLAG_CC2OF) || \
                                   ((FLAG) == TIM2_FLAG_CC3OF))

#define IS_TIM2_CLEAR_FLAG_OK(FLAG) (((FLAG & (u16)0xF1F0) == (u16)0x0000) && (FLAG != (u16)0x0000))

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/** @addtogroup TIM2_Exported_Functions
  * @{
  */

void TIM2_DeInit(void);
void TIM2_TimeBaseInit(TIM2_Prescaler_TypeDef TIM2_Prescaler, u16 TIM2_Period);
void TIM2_OC1Init(TIM2_OCMode_TypeDef TIM2_OCMode, TIM2_OutputState_TypeDef TIM2_OutputState, u16 TIM2_Pulse, TIM2_OCPolarity_TypeDef TIM2_OCPolarity);
void TIM2_OC2Init(TIM2_OCMode_TypeDef TIM2_OCMode, TIM2_OutputState_TypeDef TIM2_OutputState, u16 TIM2_Pulse, TIM2_OCPolarity_TypeDef TIM2_OCPolarity);
void TIM2_OC3Init(TIM2_OCMode_TypeDef TIM2_OCMode, TIM2_OutputState_TypeDef TIM2_OutputState, u16 TIM2_Pulse, TIM2_OCPolarity_TypeDef TIM2_OCPolarity);
void TIM2_ICInit(TIM2_Channel_TypeDef TIM2_Channel, TIM2_ICPolarity_TypeDef TIM2_ICPolarity, TIM2_ICSelection_TypeDef TIM2_ICSelection,  TIM2_ICPSC_TypeDef TIM2_ICPrescaler, u8 TIM2_ICFilter);
void TIM2_PWMIConfig(TIM2_Channel_TypeDef TIM2_Channel, TIM2_ICPolarity_TypeDef TIM2_ICPolarity, TIM2_ICSelection_TypeDef TIM2_ICSelection,  TIM2_ICPSC_TypeDef TIM2_ICPrescaler, u8 TIM2_ICFilter);
void TIM2_Cmd(FunctionalState NewState);
void TIM2_ITConfig(TIM2_IT_TypeDef TIM2_IT, FunctionalState NewState);
void TIM2_InternalClockConfig(void);
void TIM2_UpdateDisableConfig(FunctionalState Newstate);
void TIM2_UpdateRequestConfig(TIM2_UpdateSource_TypeDef TIM2_UpdateSource);
void TIM2_SelectOnePulseMode(TIM2_OPMode_TypeDef TIM2_OPMode);
void TIM2_PrescalerConfig(TIM2_Prescaler_TypeDef Prescaler, TIM2_PSCReloadMode_TypeDef TIM2_PSCReloadMode);
void TIM2_ForcedOC1Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction);
void TIM2_ForcedOC2Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction);
void TIM2_ForcedOC3Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction);
void TIM2_ARRPreloadConfig(FunctionalState Newstate);
void TIM2_CCPreloadControl(FunctionalState Newstate);
void TIM2_OC1PreloadConfig(FunctionalState Newstate);
void TIM2_OC2PreloadConfig(FunctionalState Newstate);
void TIM2_OC3PreloadConfig(FunctionalState Newstate);
void TIM2_GenerateEvent(TIM2_EventSource_TypeDef TIM2_EventSource);
void TIM2_OC1PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity);
void TIM2_OC2PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity);
void TIM2_OC3PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity);
void TIM2_CCxCmd(TIM2_Channel_TypeDef TIM2_Channel, FunctionalState Newstate);
void TIM2_SelectOCxM(TIM2_Channel_TypeDef TIM2_Channel, TIM2_OCMode_TypeDef TIM2_OCMode);
void TIM2_SetCounter(u16 Counter);
void TIM2_SetAutoreload(u16 Autoreload);
void TIM2_SetCompare1(u16 Compare1);
void TIM2_SetCompare2(u16 Compare2);
void TIM2_SetCompare3(u16 Compare3);
void TIM2_SetIC1Prescaler(TIM2_ICPSC_TypeDef TIM2_IC1Prescaler);
void TIM2_SetIC2Prescaler(TIM2_ICPSC_TypeDef TIM2_IC2Prescaler);
void TIM2_SetIC3Prescaler(TIM2_ICPSC_TypeDef TIM2_IC3Prescaler);
u16 TIM2_GetCapture1(void);
u16 TIM2_GetCapture2(void);
u16 TIM2_GetCapture3(void);
u16 TIM2_GetCounter(void);
TIM2_Prescaler_TypeDef TIM2_GetPrescaler(void);
FlagStatus TIM2_GetFlagStatus(TIM2_FLAG_TypeDef TIM2_FLAG);
void TIM2_ClearFlag(TIM2_FLAG_TypeDef TIM2_FLAG);
ITStatus TIM2_GetITStatus(TIM2_IT_TypeDef TIM2_IT);
void TIM2_ClearITPendingBit(TIM2_IT_TypeDef TIM2_IT);

/**
  * @}
  */

#endif /* __STM8S_TIM2_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
