/**
  ******************************************************************************
  * @file stm8s_adc1.h
  * @brief This file contains all the prototypes/macros for the ADC1 peripheral.
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
#ifndef __STM8S_ADC1_H
#define __STM8S_ADC1_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/

/** @addtogroup ADC1_Exported_Types
  * @{
  */

/**
  * @brief ADC1 clock prescaler selection
  */

typedef enum {
  ADC1_PRESSEL_FCPU_D2  = (u8)0x00, /**< Prescaler selection fADC1 = fcpu/2 */
  ADC1_PRESSEL_FCPU_D3  = (u8)0x10, /**< Prescaler selection fADC1 = fcpu/3 */
  ADC1_PRESSEL_FCPU_D4  = (u8)0x20, /**< Prescaler selection fADC1 = fcpu/4 */
  ADC1_PRESSEL_FCPU_D6  = (u8)0x30, /**< Prescaler selection fADC1 = fcpu/6 */
  ADC1_PRESSEL_FCPU_D8  = (u8)0x40, /**< Prescaler selection fADC1 = fcpu/8 */
  ADC1_PRESSEL_FCPU_D10 = (u8)0x50, /**< Prescaler selection fADC1 = fcpu/10 */
  ADC1_PRESSEL_FCPU_D12 = (u8)0x60, /**< Prescaler selection fADC1 = fcpu/12 */
  ADC1_PRESSEL_FCPU_D18 = (u8)0x70  /**< Prescaler selection fADC1 = fcpu/18 */
} ADC1_PresSel_TypeDef;

/**
  * @brief  ADC1 External conversion trigger event selection
  */
typedef enum {
  ADC1_EXTTRIG_TIM   = (u8)0x00, /**< Conversion from Internal TIM1 TRGO event */
  ADC1_EXTTRIG_GPIO  = (u8)0x10  /**< Conversion from External interrupt on ADC_ETR pin*/
} ADC1_ExtTrig_TypeDef;

/**
  * @brief ADC1 data alignment
  */
typedef enum {
  ADC1_ALIGN_LEFT  = (u8)0x00, /**< Data alignment left */
  ADC1_ALIGN_RIGHT = (u8)0x08  /**< Data alignment right */
} ADC1_Align_TypeDef;

/**
  * @brief ADC1 Interrupt source
  */
typedef enum {
  ADC1_IT_AWDIE = (u16)0x10, /**< Analog WDG interrupt enable */
  ADC1_IT_EOCIE = (u16)0x20, /**< EOC iterrupt enable */
  ADC1_IT_AWD   = (u16)0x140, /**< Analog WDG status */
  ADC1_IT_AWS0  = (u16)0x110, /**< Analog channel 0 status */
  ADC1_IT_AWS1  = (u16)0x111, /**< Analog channel 1 status */
  ADC1_IT_AWS2  = (u16)0x112, /**< Analog channel 2 status */
  ADC1_IT_AWS3  = (u16)0x113, /**< Analog channel 3 status */
  ADC1_IT_AWS4  = (u16)0x114, /**< Analog channel 4 status */
  ADC1_IT_AWS5  = (u16)0x115, /**< Analog channel 5 status */
  ADC1_IT_AWS6  = (u16)0x116, /**< Analog channel 6 status */
  ADC1_IT_AWS7  = (u16)0x117, /**< Analog channel 7 status */
  ADC1_IT_AWS8  = (u16)0x118, /**< Analog channel 8 status */
  ADC1_IT_AWS9  = (u16)0x119, /**< Analog channel 9 status */
  ADC1_IT_EOC   = (u16)0x80  /**< EOC pending bit */

} ADC1_IT_TypeDef;

/**
  * @brief ADC1 Flags
  */
typedef enum {
  ADC1_FLAG_OVR     = (u8)0x41, /**< Overrun status flag */
  ADC1_FLAG_AWD    = (u8)0x40, /**< Analog WDG status */
  ADC1_FLAG_AWS0  = (u8)0x10, /**< Analog channel 0 status */
  ADC1_FLAG_AWS1  = (u8)0x11, /**< Analog channel 1 status */
  ADC1_FLAG_AWS2  = (u8)0x12, /**< Analog channel 2 status */
  ADC1_FLAG_AWS3  = (u8)0x13, /**< Analog channel 3 status */
  ADC1_FLAG_AWS4  = (u8)0x14, /**< Analog channel 4 status */
  ADC1_FLAG_AWS5  = (u8)0x15, /**< Analog channel 5 status */
  ADC1_FLAG_AWS6  = (u8)0x16, /**< Analog channel 6 status */
  ADC1_FLAG_AWS7  = (u8)0x17, /**< Analog channel 7 status */
  ADC1_FLAG_AWS8  = (u8)0x18, /**< Analog channel 8  status*/
  ADC1_FLAG_AWS9  = (u8)0x19, /**< Analog channel 9 status */
  ADC1_FLAG_EOC     = (u8)0x80  /**< EOC falg */
}ADC1_Flag_TypeDef;


/**
  * @brief ADC1 schmitt Trigger
  */
typedef enum {
  ADC1_SCHMITTTRIG_CHANNEL0  = (u8)0x00, /**< Schmitt trigger disable on AIN0 */
  ADC1_SCHMITTTRIG_CHANNEL1  = (u8)0x01, /**< Schmitt trigger disable on AIN1 */
  ADC1_SCHMITTTRIG_CHANNEL2  = (u8)0x02, /**< Schmitt trigger disable on AIN2 */
  ADC1_SCHMITTTRIG_CHANNEL3  = (u8)0x03, /**< Schmitt trigger disable on AIN3 */
  ADC1_SCHMITTTRIG_CHANNEL4  = (u8)0x04, /**< Schmitt trigger disable on AIN4 */
  ADC1_SCHMITTTRIG_CHANNEL5  = (u8)0x05, /**< Schmitt trigger disable on AIN5 */
  ADC1_SCHMITTTRIG_CHANNEL6  = (u8)0x06, /**< Schmitt trigger disable on AIN6 */
  ADC1_SCHMITTTRIG_CHANNEL7  = (u8)0x07, /**< Schmitt trigger disable on AIN7 */
  ADC1_SCHMITTTRIG_CHANNEL8  = (u8)0x08, /**< Schmitt trigger disable on AIN8 */
  ADC1_SCHMITTTRIG_CHANNEL9  = (u8)0x09, /**< Schmitt trigger disable on AIN9 */  
	ADC1_SCHMITTTRIG_ALL			 = (u8)0xFF /**< Schmitt trigger disable on All channels */ 
} ADC1_SchmittTrigg_TypeDef;

/**
  * @brief ADC1 conversion mode selection
  */

typedef enum {
  ADC1_CONVERSIONMODE_SINGLE     = (u8)0x00, /**< Single conversion mode */
  ADC1_CONVERSIONMODE_CONTINUOUS = (u8)0x01  /**< Continuous conversion mode */
} ADC1_ConvMode_TypeDef;

/**
  * @brief ADC1 analog channel selection
  */


typedef enum {
  ADC1_CHANNEL_0  = (u8)0x00, /**< Analog channel 0 */
  ADC1_CHANNEL_1  = (u8)0x01, /**< Analog channel 1 */
  ADC1_CHANNEL_2  = (u8)0x02, /**< Analog channel 2 */
  ADC1_CHANNEL_3  = (u8)0x03, /**< Analog channel 3 */
  ADC1_CHANNEL_4  = (u8)0x04, /**< Analog channel 4 */
  ADC1_CHANNEL_5  = (u8)0x05, /**< Analog channel 5 */
  ADC1_CHANNEL_6  = (u8)0x06, /**< Analog channel 6 */
  ADC1_CHANNEL_7  = (u8)0x07, /**< Analog channel 7 */
  ADC1_CHANNEL_8  = (u8)0x08, /**< Analog channel 8 */
  ADC1_CHANNEL_9  = (u8)0x09 /**< Analog channel 9 */
  } ADC1_Channel_TypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/** @addtogroup ADC1_Private_Macros
  * @brief Macros used by the assert function to check the different functions parameters.
  * @{
  */

/**
  * @brief Macro used by the assert function to check the different prescaler's values.
  */
#define IS_ADC1_PRESSEL_OK(PRESCALER) (((PRESCALER) == ADC1_PRESSEL_FCPU_D2) || \
                                      ((PRESCALER) == ADC1_PRESSEL_FCPU_D3) || \
                                      ((PRESCALER) == ADC1_PRESSEL_FCPU_D4) || \
                                      ((PRESCALER) == ADC1_PRESSEL_FCPU_D6) || \
                                      ((PRESCALER) == ADC1_PRESSEL_FCPU_D8) || \
                                      ((PRESCALER) == ADC1_PRESSEL_FCPU_D10) || \
                                      ((PRESCALER) == ADC1_PRESSEL_FCPU_D12) || \
                                      ((PRESCALER) == ADC1_PRESSEL_FCPU_D18))

/**
  * @brief Macro used by the assert function to check the different external trigger values.
  */
#define IS_ADC1_EXTTRIG_OK(EXTRIG) (((EXTRIG) == ADC1_EXTTRIG_TIM) || \
								                   ((EXTRIG) == ADC1_EXTTRIG_GPIO))

/**
  * @brief Macro used by the assert function to check the different alignement modes.
  */
#define IS_ADC1_ALIGN_OK(ALIGN) (((ALIGN) == ADC1_ALIGN_LEFT) || \
                                ((ALIGN) == ADC1_ALIGN_RIGHT))

/**
  * @brief Macro used by the assert function to check the Interrupt source.
  */
#define IS_ADC1_IT_OK(IT) (((IT) == ADC1_IT_EOCIE) || \
                          ((IT) == ADC1_IT_AWDIE))

/**
  * @brief Macro used by the assert function to check the ADC1 Flag.
  */
#define IS_ADC1_FLAG_OK(FLAG) (((FLAG) == ADC1_FLAG_EOC)|| \
                              ((FLAG) == ADC1_FLAG_OVR) || \
                              ((FLAG) == ADC1_FLAG_AWD) || \
                              ((FLAG) == ADC1_FLAG_AWS0) || \
                              ((FLAG) == ADC1_FLAG_AWS1) || \
                              ((FLAG) == ADC1_FLAG_AWS2) || \
                              ((FLAG) == ADC1_FLAG_AWS3) || \
                              ((FLAG) == ADC1_FLAG_AWS4) || \
                              ((FLAG) == ADC1_FLAG_AWS5) || \
                              ((FLAG) == ADC1_FLAG_AWS6) || \
                              ((FLAG) == ADC1_FLAG_AWS7) || \
                              ((FLAG) == ADC1_FLAG_AWS8) || \
                              ((FLAG) == ADC1_FLAG_AWS9))

/**
  * @brief Macro used by the assert function to check the ADC1 pending bits.
  */
#define IS_ADC1_ITPENDINGBIT_OK(ITPENDINGBIT) (((ITPENDINGBIT) == ADC1_IT_EOC) || \
    ((ITPENDINGBIT) == ADC1_IT_AWD) || \
    ((ITPENDINGBIT) == ADC1_IT_AWS0) || \
    ((ITPENDINGBIT) == ADC1_IT_AWS1) || \
    ((ITPENDINGBIT) == ADC1_IT_AWS2) || \
    ((ITPENDINGBIT) == ADC1_IT_AWS3) || \
    ((ITPENDINGBIT) == ADC1_IT_AWS4) || \
    ((ITPENDINGBIT) == ADC1_IT_AWS5) || \
    ((ITPENDINGBIT) == ADC1_IT_AWS6) || \
    ((ITPENDINGBIT) == ADC1_IT_AWS7) || \
    ((ITPENDINGBIT) == ADC1_IT_AWS8) || \
    ((ITPENDINGBIT) == ADC1_IT_AWS9))

/**
  * @brief Macro used by the assert function to check the different schmitt trigger values.
  */
#define IS_ADC1_SCHMITTTRIG_OK(SCHMITTTRIG) (((SCHMITTTRIG) == ADC1_SCHMITTTRIG_CHANNEL0) || \
    ((SCHMITTTRIG) == ADC1_SCHMITTTRIG_CHANNEL1) || \
    ((SCHMITTTRIG) == ADC1_SCHMITTTRIG_CHANNEL2) || \
    ((SCHMITTTRIG) == ADC1_SCHMITTTRIG_CHANNEL3) || \
    ((SCHMITTTRIG) == ADC1_SCHMITTTRIG_CHANNEL4) || \
    ((SCHMITTTRIG) == ADC1_SCHMITTTRIG_CHANNEL5) || \
    ((SCHMITTTRIG) == ADC1_SCHMITTTRIG_CHANNEL6) || \
    ((SCHMITTTRIG) == ADC1_SCHMITTTRIG_CHANNEL7) || \
    ((SCHMITTTRIG) == ADC1_SCHMITTTRIG_CHANNEL8) || \
		((SCHMITTTRIG) == ADC1_SCHMITTTRIG_ALL) || \
    ((SCHMITTTRIG) == ADC1_SCHMITTTRIG_CHANNEL9))

/**
  * @brief Macro used by the assert function to check the different conversion modes.
  */
#define IS_ADC1_CONVERSIONMODE_OK(MODE) (((MODE) == ADC1_CONVERSIONMODE_SINGLE) || \
                                        ((MODE) == ADC1_CONVERSIONMODE_CONTINUOUS))

/**
  * @brief Macro used by the assert function to check the different channels values.
  */
#define IS_ADC1_CHANNEL_OK(CHANNEL) (((CHANNEL) == ADC1_CHANNEL_0) || \
                                    ((CHANNEL) == ADC1_CHANNEL_1) || \
                                    ((CHANNEL) == ADC1_CHANNEL_2) || \
                                    ((CHANNEL) == ADC1_CHANNEL_3) || \
                                    ((CHANNEL) == ADC1_CHANNEL_4) || \
                                    ((CHANNEL) == ADC1_CHANNEL_5) || \
                                    ((CHANNEL) == ADC1_CHANNEL_6) || \
                                    ((CHANNEL) == ADC1_CHANNEL_7) || \
                                    ((CHANNEL) == ADC1_CHANNEL_8) || \
                                    ((CHANNEL) == ADC1_CHANNEL_9))

/**
  * @brief Macro used by the assert function to check the possible buffer values.
  */
#define IS_ADC1_BUFFER_OK(BUFFER) ((BUFFER) <= (u8)0x09)

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup ADC1_Exported_Functions
  * @{
  */
void ADC1_DeInit(void);
void ADC1_Init(ADC1_ConvMode_TypeDef ADC1_ConversionMode, ADC1_Channel_TypeDef ADC1_Channel, ADC1_PresSel_TypeDef ADC1_PrescalerSelection, ADC1_ExtTrig_TypeDef ADC1_ExtTrigger, FunctionalState ADC1_ExtTriggerState, ADC1_Align_TypeDef ADC1_Align, ADC1_SchmittTrigg_TypeDef ADC1_SchmittTriggerChannel, FunctionalState ADC1_SchmittTriggerState);
void ADC1_Cmd(FunctionalState NewState);
void ADC1_ScanModeCmd(FunctionalState NewState);
void ADC1_DataBufferCmd(FunctionalState NewState);
void ADC1_ITConfig(ADC1_IT_TypeDef ADC1_IT, FunctionalState NewState);
void ADC1_PrescalerConfig(ADC1_PresSel_TypeDef ADC1_Prescaler);
void ADC1_SchmittTriggerConfig(ADC1_SchmittTrigg_TypeDef ADC1_SchmittTriggerChannel, FunctionalState NewState);
void ADC1_ConversionConfig(ADC1_ConvMode_TypeDef ADC1_ConversionMode, ADC1_Channel_TypeDef ADC1_Channel, ADC1_Align_TypeDef ADC1_Align);
void ADC1_ExternalTriggerConfig(ADC1_ExtTrig_TypeDef ADC1_ExtTrigger, FunctionalState NewState);
void ADC1_AWDChannelConfig(ADC1_Channel_TypeDef Channel, FunctionalState NewState);
void ADC1_StartConversion(void);
u16 ADC1_GetConversionValue(void);
void ADC1_SetHighThreshold(u16 Threshold);
void ADC1_SetLowThreshold(u16 Threshold);
u16 ADC1_GetBufferValue(u8 Buffer);
FlagStatus ADC1_GetAWDChannelStatus(ADC1_Channel_TypeDef Channel);
FlagStatus ADC1_GetFlagStatus(ADC1_Flag_TypeDef Flag);
void ADC1_ClearFlag(ADC1_Flag_TypeDef Flag);
ITStatus ADC1_GetITStatus(ADC1_IT_TypeDef ITPendingBit);
void ADC1_ClearITPendingBit(ADC1_IT_TypeDef ITPendingBit);
/**
  * @}
  */

#endif /* __STM8S_ADC1_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
