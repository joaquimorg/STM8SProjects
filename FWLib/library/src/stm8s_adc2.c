/**
  ******************************************************************************
  * @file stm8s_adc2.c
  * @brief This file contains all the functions/macros for the ADC2 peripheral.
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
#include "stm8s_adc2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/**
  * @addtogroup ADC2_Public_Functions
  * @{
  */

/**
  * @brief Deinitializes the ADC2 peripheral registers to their default reset
  * values.
  * @par Parameters:
  * None
  * @retval None
  */
void ADC2_DeInit(void)
{
    ADC2->CSR  = ADC2_CSR_RESET_VALUE;
    ADC2->CR1  = ADC2_CR1_RESET_VALUE;
    ADC2->CR2  = ADC2_CR2_RESET_VALUE;
    ADC2->TDRH = ADC2_TDRH_RESET_VALUE;
    ADC2->TDRL = ADC2_TDRL_RESET_VALUE;
}

/**
  * @brief Initializes the ADC2 peripheral according to the specified parameters
  * @param[in] ADC2_ConversionMode: specifies the conversion mode
  * can be one of the values of @ref ADC2_ConvMode_TypeDef.
  * @param[in] ADC2_Channel: specifies the channel to convert
  * can be one of the values of @ref ADC2_Channel_TypeDef.
  * @param[in] ADC2_PrescalerSelection: specifies the ADC2 prescaler
  * can be one of the values of @ref ADC2_PresSel_TypeDef.
  * @param[in] ADC2_ExtTrigger: specifies the external trigger
  * can be one of the values of @ref ADC2_ExtTrig_TypeDef.
  * @param[in] ADC2_ExtTriggerState: specifies the external trigger new state
  * can be one of the values of @ref FunctionalState.
  * @param[in] ADC2_Align: specifies the converted data alignement
  * can be one of the values of @ref ADC2_Align_TypeDef.
  * @param[in] ADC2_SchmittTriggerChannel: specifies the schmitt trigger channel
  * can be one of the values of @ref ADC2_SchmittTrigg_TypeDef.
  * @param[in] ADC2_SchmittTriggerState: specifies the schmitt trigger state
  * can be one of the values of @ref FunctionalState.
  * @retval None
  */
void ADC2_Init(ADC2_ConvMode_TypeDef ADC2_ConversionMode, ADC2_Channel_TypeDef ADC2_Channel, ADC2_PresSel_TypeDef ADC2_PrescalerSelection, ADC2_ExtTrig_TypeDef ADC2_ExtTrigger, FunctionalState ADC2_ExtTriggerState, ADC2_Align_TypeDef ADC2_Align, ADC2_SchmittTrigg_TypeDef ADC2_SchmittTriggerChannel, FunctionalState ADC2_SchmittTriggerState)
{

    /* Check the parameters */
    assert_param(IS_ADC2_CONVERSIONMODE_OK(ADC2_ConversionMode));
    assert_param(IS_ADC2_CHANNEL_OK(ADC2_Channel));
    assert_param(IS_ADC2_PRESSEL_OK(ADC2_PrescalerSelection));
    assert_param(IS_ADC2_EXTTRIG_OK(ADC2_ExtTrigger));
    assert_param(IS_FUNCTIONALSTATE_OK(((ADC2_ExtTriggerState))));
    assert_param(IS_ADC2_ALIGN_OK(ADC2_Align));
    assert_param(IS_ADC2_SCHMITTTRIG_OK(ADC2_SchmittTriggerChannel));
    assert_param(IS_FUNCTIONALSTATE_OK(ADC2_SchmittTriggerState));

    /*-----------------CR1 & CSR configuration --------------------*/
    /* Configure the conversion mode and the channel to convert
    respectively according to ADC2_ConversionMode & ADC2_Channel values  &  ADC2_Align values */
    ADC2_ConversionConfig(ADC2_ConversionMode, ADC2_Channel, ADC2_Align);
    /* Select the prescaler division factor according to ADC2_PrescalerSelection values */
    ADC2_PrescalerConfig(ADC2_PrescalerSelection);

    /*-----------------CR2 configuration --------------------*/
    /* Configure the external trigger state and event respectively
    according to ADC2_ExtTrigStatus, ADC2_ExtTrigger */
    ADC2_ExternalTriggerConfig(ADC2_ExtTrigger, ADC2_ExtTriggerState);

    /*------------------TDR configuration ---------------------------*/
    /* Configure the schmitt trigger channel and state respectively
    according to ADC2_SchmittTriggerChannel & ADC2_SchmittTriggerNewState  values */
    ADC2_SchmittTriggerConfig(ADC2_SchmittTriggerChannel, ADC2_SchmittTriggerState);

    /* Enable the ADC2 peripheral */
    ADC2->CR1 |= ADC2_CR1_ADON;

}


/**
  * @brief Enables or Disables the ADC2 peripheral.
  * @param[in] NewState: specifies the peripheral enabled or disabled state.
  * @retval None
  */
void ADC2_Cmd(FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        ADC2->CR1 |= ADC2_CR1_ADON;
    }
    else /* NewState == DISABLE */
    {
        ADC2->CR1 &= (u8)(~ADC2_CR1_ADON);
    }

}

/**
  * @brief Enables or disables the ADC2 interrupt.
  * @param[in] NewState specifies the state of ADC2 interrupt.
  * @retval None
  */
void ADC2_ITConfig(FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the ADC2 interrupts */
        ADC2->CSR |= (u8)ADC2_CSR_EOCIE;
    }
    else  /*NewState == DISABLE */
    {
        /* Disable the ADC2 interrupts */
        ADC2->CSR &= (u8)(~ADC2_CSR_EOCIE);
    }

}

/**
  * @brief Configure the ADC2 prescaler division factor.
  * @param[in] ADC2_Prescaler: the selected precaler.
  * It can be one of the values of @ref ADC2_PresSel_TypeDef.
  * @retval None
  */
void ADC2_PrescalerConfig(ADC2_PresSel_TypeDef ADC2_Prescaler)
{

    /* Check the parameter */
    assert_param(IS_ADC2_PRESSEL_OK(ADC2_Prescaler));

    /* Clear the SPSEL bits */
    ADC2->CR1 &= (u8)(~ADC2_CR1_SPSEL);
    /* Select the prescaler division factor according to ADC2_PrescalerSelection values */
    ADC2->CR1 |= (u8)(ADC2_Prescaler);

}


/**
  * @brief Enables or disables the ADC2 Schmitt Trigger on a selected channel.
  * @param[in] ADC2_SchmittTriggerChannel specifies the desired Channel.
  * It can be set of the values of @ref ADC2_SchmittTrigg_TypeDef.
  * @param[in] NewState specifies the Channel  ADC2 Schmitt Trigger new status.
  * can have one of the values of @ref FunctionalState.
  * @retval None
  */
void ADC2_SchmittTriggerConfig(ADC2_SchmittTrigg_TypeDef ADC2_SchmittTriggerChannel, FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_ADC2_SCHMITTTRIG_OK(ADC2_SchmittTriggerChannel));
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (ADC2_SchmittTriggerChannel == ADC2_SCHMITTTRIG_ALL)
    {
        if (NewState != DISABLE)
        {
            ADC2->TDRL &= (u8)0x0;
            ADC2->TDRH &= (u8)0x0;
        }
        else /* NewState == DISABLE */
        {
            ADC2->TDRL |= (u8)0xFF;
            ADC2->TDRH |= (u8)0xFF;
        }
    }
    else if (ADC2_SchmittTriggerChannel < ADC2_SCHMITTTRIG_CHANNEL8)
    {
        if (NewState != DISABLE)
        {
            ADC2->TDRL &= (u8)(~(u8)((u8)0x01 << (u8)ADC2_SchmittTriggerChannel));
        }
        else /* NewState == DISABLE */
        {
            ADC2->TDRL |= (u8)((u8)0x01 << (u8)ADC2_SchmittTriggerChannel);
        }
    }
    else /* ADC2_SchmittTriggerChannel >= ADC2_SCHMITTTRIG_CHANNEL8 */
    {
        if (NewState != DISABLE)
        {
            ADC2->TDRH &= (u8)(~(u8)((u8)0x01 << ((u8)ADC2_SchmittTriggerChannel - (u8)8)));
        }
        else /* NewState == DISABLE */
        {
            ADC2->TDRH |= (u8)((u8)0x01 << ((u8)ADC2_SchmittTriggerChannel - (u8)8));
        }
    }

}

/**
  * @brief Configure the ADC2 conversion on selected channel.
  * @param[in] ADC2_ConversionMode Specifies the conversion type.
  * It can be set of the values of @ref ADC2_ConvMode_TypeDef
  * @param[in] ADC2_Channel specifies the ADC2 Channel.
  * It can be set of the values of @ref ADC2_Channel_TypeDef
  * @param[in] ADC2_Align specifies the conerted data alignement.
  * It can be set of the values of @ref ADC2_Align_TypeDef
  * @retval None
  */
void ADC2_ConversionConfig(ADC2_ConvMode_TypeDef ADC2_ConversionMode, ADC2_Channel_TypeDef ADC2_Channel, ADC2_Align_TypeDef ADC2_Align)
{

    /* Check the parameters */
    assert_param(IS_ADC2_CONVERSIONMODE_OK(ADC2_ConversionMode));
    assert_param(IS_ADC2_CHANNEL_OK(ADC2_Channel));
    assert_param(IS_ADC2_ALIGN_OK(ADC2_Align));

    /* Clear the align bit */
    ADC2->CR2 &= (u8)(~ADC2_CR2_ALIGN);
    /* Configure the data alignment */
    ADC2->CR2 |= (u8)(ADC2_Align);

    if (ADC2_ConversionMode == ADC2_CONVERSIONMODE_CONTINUOUS)
    {
        /* Set the continuous coversion mode */
        ADC2->CR1 |= ADC2_CR1_CONT;
    }
    else /* ADC2_ConversionMode == ADC2_CONVERSIONMODE_SINGLE */
    {
        /* Set the single conversion mode */
        ADC2->CR1 &= (u8)(~ADC2_CR1_CONT);
    }

    /* Clear the ADC2 channels */
    ADC2->CSR &= (u8)(~ADC2_CSR_CH);
    /* Select the ADC2 channel */
    ADC2->CSR |= (u8)(ADC2_Channel);

}


/**
  * @brief Configure the ADC2 conversion on external trigger event.
  * @par Full description:
  * The selected external trigger evant can be enabled or disabled.
  * @param[in] ADC2_ExtTrigger to select the External trigger event.
  * can have one of the values of @ref ADC2_ExtTrig_TypeDef.
  * @param[in] NewState to enable/disable the selected external trigger
  * can have one of the values of @ref FunctionalState.
  * @retval None
  */
void ADC2_ExternalTriggerConfig(ADC2_ExtTrig_TypeDef ADC2_ExtTrigger, FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_ADC2_EXTTRIG_OK(ADC2_ExtTrigger));
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* Clear the external trigger selection bits */
    ADC2->CR2 &= (u8)(~ADC2_CR2_EXTSEL);

    if (NewState != DISABLE)
    {
        /* Enable the selected external Trigger */
        ADC2->CR2 |= (u8)(ADC2_CR2_EXTTRIG);
    }
    else /* NewState == DISABLE */
    {
        /* Disable the selected external trigger */
        ADC2->CR2 &= (u8)(~ADC2_CR2_EXTTRIG);
    }

    /* Set the slected external trigger */
    ADC2->CR2 |= (u8)(ADC2_ExtTrigger);

}


/**
  * @brief Start ADC2 conversion
  * @par Full description:
  * This function  triggers the start of conversion, after ADC2 configuration.
  * @par Parameters:
  * None
  * @retval None
  * @par Required preconditions:
  * Enable the ADC2 peripheral before calling this fuction
  */
void ADC2_StartConversion(void)
{
    ADC2->CR1 |= ADC2_CR1_ADON;
}

/**
  * @brief Get one sample of measured signal.
  * @par Parameters:
  * None
  * @retval ConversionValue:  value of the measured signal.
  * @par Required preconditions:
  * ADC2 conversion finished.
  */
u16 ADC2_GetConversionValue(void)
{

    u16 temph = 0;
    u8 templ = 0;

    if (ADC2->CR2 & ADC2_CR2_ALIGN) /* Right alignment */
    {
        /* Read LSB first */
        templ = ADC2->DRL;
        /* Then read MSB */
        temph = ADC2->DRH;

        temph = (u16)(templ | (u16)(temph << (u8)8));
    }
    else /* Left alignment */
    {
        /* Read MSB firts*/
        temph = ADC2->DRH;
        /* Then read LSB */
        templ = ADC2->DRL;

        temph = (u16)((u16)(templ << (u8)6) | (u16)(temph << (u8)8));
    }

    return ((u16)temph);

}

/**
  * @brief Checks the ADC2 EOC flag status.
  * @par Parameters:
  * None
  * @retval FlagStatus Status of the ADC2 EOC flag.
  */
FlagStatus ADC2_GetFlagStatus(void)
{
    /* Get EOC  flag status */
    return ((u8)(ADC2->CSR & ADC2_CSR_EOC));

}

/**
  * @brief Clear the ADC2 EOC Flag.
  * @par Parameters:
  * None
  * @retval None
  */
void ADC2_ClearFlag(void)
{
    ADC2->CSR &= (u8)(~ADC2_CSR_EOC);
}

/**
  * @brief Returns the EOC  pending bit status
 * @par Parameters:
  * None
  * @retval FlagStatus: status of the EOC pending bit.
  */
ITStatus ADC2_GetITStatus(void)
{
    return ((u8)(ADC2->CSR & ADC2_CSR_EOC));
}

/**
  * @brief Clear the ADC2 End of Conversion pending bit.
  * @par Parameters:
  * None
  * @retval None
  */
void ADC2_ClearITPendingBit(void)
{
    ADC2->CSR &= (u8)(~ADC2_CSR_EOC);
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
