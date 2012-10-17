/**
  ******************************************************************************
  * @file stm8s_adc1.c
  * @brief This file contains all the functions/macros for the ADC1 peripheral.
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
#include "stm8s_adc1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/**
  * @addtogroup ADC1_Public_Functions
  * @{
  */

/**
  * @brief Deinitializes the ADC1 peripheral registers to their default reset
  * values.
  * @par Parameters:
  * None
  * @retval None
  */
void ADC1_DeInit(void)
{
    ADC1->CSR  = ADC1_CSR_RESET_VALUE;
    ADC1->CR1  = ADC1_CR1_RESET_VALUE;
    ADC1->CR2  = ADC1_CR2_RESET_VALUE;
    ADC1->CR3  = ADC1_CR3_RESET_VALUE;
    ADC1->TDRH = ADC1_TDRH_RESET_VALUE;
    ADC1->TDRL = ADC1_TDRL_RESET_VALUE;
    ADC1->HTRH = ADC1_HTRH_RESET_VALUE;
    ADC1->HTRL = ADC1_HTRL_RESET_VALUE;
    ADC1->LTRH = ADC1_LTRH_RESET_VALUE;
    ADC1->LTRL = ADC1_LTRL_RESET_VALUE;
    ADC1->AWCRH = ADC1_AWCRH_RESET_VALUE;
    ADC1->AWCRL = ADC1_AWCRL_RESET_VALUE;
}


/**
  * @brief Initializes the ADC1 peripheral according to the specified parameters
  * @param[in] ADC1_ConversionMode: specifies the conversion mode
  * can be one of the values of @ref ADC1_ConvMode_TypeDef.
  * @param[in] ADC1_Channel: specifies the channel to convert
  * can be one of the values of @ref ADC1_Channel_TypeDef.
  * @param[in] ADC1_PrescalerSelection: specifies the ADC1 prescaler
  * can be one of the values of @ref ADC1_PresSel_TypeDef.
  * @param[in] ADC1_ExtTrigger: specifies the external trigger
  * can be one of the values of @ref ADC1_ExtTrig_TypeDef.
  * @param[in] ADC1_ExtTriggerState: specifies the external trigger new state
  * can be one of the values of @ref FunctionalState.
  * @param[in] ADC1_Align: specifies the converted data alignement
  * can be one of the values of @ref ADC1_Align_TypeDef.
  * @param[in] ADC1_SchmittTriggerChannel: specifies the schmitt trigger channel
  * can be one of the values of @ref ADC1_SchmittTrigg_TypeDef.
  * @param[in] ADC1_SchmittTriggerState: specifies the schmitt trigger state
  * can be one of the values of @ref FunctionalState.
  * @retval None
  */
void ADC1_Init(ADC1_ConvMode_TypeDef ADC1_ConversionMode, ADC1_Channel_TypeDef ADC1_Channel, ADC1_PresSel_TypeDef ADC1_PrescalerSelection, ADC1_ExtTrig_TypeDef ADC1_ExtTrigger, FunctionalState ADC1_ExtTriggerState, ADC1_Align_TypeDef ADC1_Align, ADC1_SchmittTrigg_TypeDef ADC1_SchmittTriggerChannel, FunctionalState ADC1_SchmittTriggerState)
{

    /* Check the parameters */
    assert_param(IS_ADC1_CONVERSIONMODE_OK(ADC1_ConversionMode));
    assert_param(IS_ADC1_CHANNEL_OK(ADC1_Channel));
    assert_param(IS_ADC1_PRESSEL_OK(ADC1_PrescalerSelection));
    assert_param(IS_ADC1_EXTTRIG_OK(ADC1_ExtTrigger));
    assert_param(IS_FUNCTIONALSTATE_OK(((ADC1_ExtTriggerState))));
    assert_param(IS_ADC1_ALIGN_OK(ADC1_Align));
    assert_param(IS_ADC1_SCHMITTTRIG_OK(ADC1_SchmittTriggerChannel));
    assert_param(IS_FUNCTIONALSTATE_OK(ADC1_SchmittTriggerState));

    /*-----------------CR1 & CSR configuration --------------------*/
    /* Configure the conversion mode and the channel to convert
    respectively according to ADC1_ConversionMode & ADC1_Channel values  &  ADC1_Align values */
    ADC1_ConversionConfig(ADC1_ConversionMode, ADC1_Channel, ADC1_Align);
    /* Select the prescaler division factor according to ADC1_PrescalerSelection values */
    ADC1_PrescalerConfig(ADC1_PrescalerSelection);

    /*-----------------CR2 configuration --------------------*/
    /* Configure the external trigger state and event respectively
    according to NewState, ADC1_ExtTrigger */
    ADC1_ExternalTriggerConfig(ADC1_ExtTrigger, ADC1_ExtTriggerState);

    /*------------------TDR configuration ---------------------------*/
    /* Configure the schmitt trigger channel and state respectively
    according to ADC1_SchmittTriggerChannel & ADC1_SchmittTriggerNewState  values */
    ADC1_SchmittTriggerConfig(ADC1_SchmittTriggerChannel, ADC1_SchmittTriggerState);

    /* Enable the ADC1 peripheral */
    ADC1->CR1 |= ADC1_CR1_ADON;

}


/**
  * @brief Enables or Disables the ADC1 peripheral.
  * @param[in] NewState: specifies the peripheral enabled or disabled state.
  * @retval None
  */
void ADC1_Cmd(FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        ADC1->CR1 |= ADC1_CR1_ADON;
    }
    else /* NewState == DISABLE */
    {
        ADC1->CR1 &= (u8)(~ADC1_CR1_ADON);
    }

}

/**
  * @brief Enables or Disables the ADC1 scan mode.
  * @param[in] NewState: specifies the selected mode enabled or disabled state.
  * @retval None
  */
void ADC1_ScanModeCmd(FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        ADC1->CR2 |= ADC1_CR2_SCAN;
    }
    else /* NewState == DISABLE */
    {
        ADC1->CR2 &= (u8)(~ADC1_CR2_SCAN);
    }

}

/**
  * @brief Enables or Disables the ADC1 data store into the Data Buffer registers rather than in the Data Register
  * @param[in] NewState: specifies the selected mode enabled or disabled state.
  * @retval None
  */
void ADC1_DataBufferCmd(FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        ADC1->CR3 |= ADC1_CR3_DBUF;
    }
    else /* NewState == DISABLE */
    {
        ADC1->CR3 &= (u8)(~ADC1_CR3_DBUF);
    }

}

/**
  * @brief Enables or disables the ADC1 interrupt.
  * @param[in] ADC1_IT specifies the name of the interrupt to enable or disable.
  * This parameter can be one of the following values:
  *    - ADC1_IT_AWDITEN : Analog WDG interrupt enable
  *    - ADC1_IT_EOCITEN  : EOC iterrupt enable
  * @param[in] NewState specifies the state of the interrupt to apply.
  * @retval None
  */
void ADC1_ITConfig(ADC1_IT_TypeDef ADC1_IT, FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_ADC1_IT_OK(ADC1_IT));
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the ADC1 interrupts */
        ADC1->CSR |= (u8)ADC1_IT;
    }
    else  /* NewState == DISABLE */
    {
        /* Disable the ADC1 interrupts */
        ADC1->CSR &= (u8)(~ADC1_IT);
    }

}

/**
  * @brief Configure the ADC1 prescaler division factor.
  * @param[in] ADC1_Prescaler: the selected precaler.
  * It can be one of the values of @ref ADC1_PresSel_TypeDef.
  * @retval None
  */
void ADC1_PrescalerConfig(ADC1_PresSel_TypeDef ADC1_Prescaler)
{

    /* Check the parameter */
    assert_param(IS_ADC1_PRESSEL_OK(ADC1_Prescaler));

    /* Clear the SPSEL bits */
    ADC1->CR1 &= (u8)(~ADC1_CR1_SPSEL);
    /* Select the prescaler division factor according to ADC1_PrescalerSelection values */
    ADC1->CR1 |= (u8)(ADC1_Prescaler);

}


/**
  * @brief Enables or disables the ADC1 Schmitt Trigger on a selected channel.
  * @param[in] ADC1_SchmittTriggerChannel specifies the desired Channel.
  * It can be set of the values of @ref ADC1_SchmittTrigg_TypeDef.
  * @param[in] NewState specifies Channel new status.
  * can have one of the values of @ref FunctionalState.
  * @retval None
  */
void ADC1_SchmittTriggerConfig(ADC1_SchmittTrigg_TypeDef ADC1_SchmittTriggerChannel, FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_ADC1_SCHMITTTRIG_OK(ADC1_SchmittTriggerChannel));
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (ADC1_SchmittTriggerChannel == ADC1_SCHMITTTRIG_ALL)
    {
        if (NewState != DISABLE)
        {
            ADC1->TDRL &= (u8)0x0;
            ADC1->TDRH &= (u8)0x0;
        }
        else /* NewState == DISABLE */
        {
            ADC1->TDRL |= (u8)0xFF;
            ADC1->TDRH |= (u8)0xFF;
        }
    }
    else if (ADC1_SchmittTriggerChannel < ADC1_SCHMITTTRIG_CHANNEL8)
    {
        if (NewState != DISABLE)
        {
            ADC1->TDRL &= (u8)(~(u8)((u8)0x01 << (u8)ADC1_SchmittTriggerChannel));
        }
        else /* NewState == DISABLE */
        {
            ADC1->TDRL |= (u8)((u8)0x01 << (u8)ADC1_SchmittTriggerChannel);
        }
    }
    else /* ADC1_SchmittTriggerChannel >= ADC1_SCHMITTTRIG_CHANNEL8 */
    {
        if (NewState != DISABLE)
        {
            ADC1->TDRH &= (u8)(~(u8)((u8)0x01 << ((u8)ADC1_SchmittTriggerChannel - (u8)8)));
        }
        else /* NewState == DISABLE */
        {
            ADC1->TDRH |= (u8)((u8)0x01 << ((u8)ADC1_SchmittTriggerChannel - (u8)8));
        }
    }

}


/**
  * @brief Configure the ADC1 conversion on selected channel.
  * @param[in] ADC1_ConversionMode Specifies the conversion type.
  * It can be set of the values of @ref ADC1_ConvMode_TypeDef
  * @param[in] ADC1_Channel specifies the ADC1 Channel.
  * It can be set of the values of @ref ADC1_Channel_TypeDef
  * @param[in] ADC1_Align specifies the conerted data alignement.
  * It can be set of the values of @ref ADC1_Align_TypeDef
  * @retval None
  */
void ADC1_ConversionConfig(ADC1_ConvMode_TypeDef ADC1_ConversionMode, ADC1_Channel_TypeDef ADC1_Channel, ADC1_Align_TypeDef ADC1_Align)
{

    /* Check the parameters */
    assert_param(IS_ADC1_CONVERSIONMODE_OK(ADC1_ConversionMode));
    assert_param(IS_ADC1_CHANNEL_OK(ADC1_Channel));
    assert_param(IS_ADC1_ALIGN_OK(ADC1_Align));

    /* Clear the align bit */
    ADC1->CR2 &= (u8)(~ADC1_CR2_ALIGN);
    /* Configure the data alignment */
    ADC1->CR2 |= (u8)(ADC1_Align);

    if (ADC1_ConversionMode == ADC1_CONVERSIONMODE_CONTINUOUS)
    {
        /* Set the continuous coversion mode */
        ADC1->CR1 |= ADC1_CR1_CONT;
    }
    else /* ADC1_ConversionMode == ADC1_CONVERSIONMODE_SINGLE */
    {
        /* Set the single conversion mode */
        ADC1->CR1 &= (u8)(~ADC1_CR1_CONT);
    }

    /* Clear the ADC1 channels */
    ADC1->CSR &= (u8)(~ADC1_CSR_CH);
    /* Select the ADC1 channel */
    ADC1->CSR |= (u8)(ADC1_Channel);

}


/**
  * @brief Configure the ADC1 conversion on external trigger event.
  * @par Full description:
  * The selected external trigger evant can be enabled or disabled.
  * @param[in] ADC1_ExtTrigger to select the External trigger event.
  * can have one of the values of @ref ADC1_ExtTrig_TypeDef.
  * @param[in] NewState to enable/disable the selected external trigger
  * can have one of the values of @ref FunctionalState.
  * @retval None
  */
void ADC1_ExternalTriggerConfig(ADC1_ExtTrig_TypeDef ADC1_ExtTrigger, FunctionalState NewState)
{

    /* Check the parameters */
    assert_param(IS_ADC1_EXTTRIG_OK(ADC1_ExtTrigger));
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* Clear the external trigger selection bits */
    ADC1->CR2 &= (u8)(~ADC1_CR2_EXTSEL);

    if (NewState != DISABLE)
    {
        /* Enable the selected external Trigger */
        ADC1->CR2 |= (u8)(ADC1_CR2_EXTTRIG);
    }
    else /* NewState == DISABLE */
    {
        /* Disable the selected external trigger */
        ADC1->CR2 &= (u8)(~ADC1_CR2_EXTTRIG);
    }

    /* Set the slected external trigger */
    ADC1->CR2 |= (u8)(ADC1_ExtTrigger);

}


/**
  * @brief Start ADC1 conversion
  * @par Full description:
  * This function  triggers the start of conversion, after ADC1 configuration.
  * @par Parameters:
  * None
  * @retval None
  * @par Required preconditions:
  * Enable the ADC1 peripheral before calling this fuction
  */
void ADC1_StartConversion(void)
{
    ADC1->CR1 |= ADC1_CR1_ADON;
}

/**
  * @brief Get one sample of measured signal.
  * @par Parameters:
  * None
  * @retval ConversionValue:  value of the measured signal.
  * @par Required preconditions:
  * ADC1 conversion finished.
  */
u16 ADC1_GetConversionValue(void)
{

    u16 temph = 0;
    u8 templ = 0;

    if (ADC1->CR2 & ADC1_CR2_ALIGN) /* Right alignment */
    {
        /* Read LSB first */
        templ = ADC1->DRL;
        /* Then read MSB */
        temph = ADC1->DRH;

        temph = (u16)(templ | (u16)(temph << (u8)8));
    }
    else /* Left alignment */
    {
        /* Read MSB firts*/
        temph = ADC1->DRH;
        /* Then read LSB */
        templ = ADC1->DRL;

        temph = (u16)((u16)(templ << (u8)6) | (u16)(temph << (u8)8));
    }

    return ((u16)temph);

}

/**
  * @brief Enables or disables the analog watchdog for the given channel.
  * @param[in] Channel specifies the desired Channel.
  * It can be set of the values of @ref ADC1_Channel_TypeDef.
  * @param[in] NewState specifies the analog watchdog new state.
  * can have one of the values of @ref FunctionalState.
  * @retval None
  */
void ADC1_AWDChannelConfig(ADC1_Channel_TypeDef Channel, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));
    assert_param(IS_ADC1_CHANNEL_OK(Channel));

    if (Channel < (u8)8)
    {
        if (NewState != DISABLE)
        {
            ADC1->AWCRL |= (u8)((u8)1 << Channel);
        }
        else /* NewState == DISABLE */
        {
            ADC1->AWCRL &= (u8)(~((u8)1 << Channel));
        }
    }
    else
    {
        if (NewState != DISABLE)
        {
            ADC1->AWCRH |= (u8)((u8)1 << (Channel - (u8)8));
        }
        else /* NewState == DISABLE */
        {
            ADC1->AWCRH &= (u8)(~((u8)1 << (Channel - (u8)8)));
        }
    }
}

/**
  * @brief Sets the high threshold of the analog watchdog.
  * @param[in] Threshold specifies the high threshold value.
  * this value depends on the reference voltage range.
  * @retval None
  */
void ADC1_SetHighThreshold(u16 Threshold)
{
    ADC1->HTRH = (u8)(Threshold >> (u8)8);
    ADC1->HTRL = (u8)Threshold;
}

/**
  * @brief Sets the low threshold of the analog watchdog.
  * @param[in] Threshold specifies thelow threshold value.
  * this value depends on the reference voltage range.
  * @retval None
  */
void ADC1_SetLowThreshold(u16 Threshold)
{
    ADC1->LTRL = (u8)Threshold;
    ADC1->LTRH = (u8)(Threshold >> (u8)8);
}

/**
  * @brief Get one sample of measured signal.
  * @param[in] Buffer specifies the buffer to read.
  * @retval BufferValue:  value read from the given buffer.
  * @par Required preconditions:
  * ADC1 conversion finished.
  */
u16 ADC1_GetBufferValue(u8 Buffer)
{

    u16 temph = 0;
    u8 templ = 0;

    /* Check the parameters */
    assert_param(IS_ADC1_BUFFER_OK(Buffer));

    if (ADC1->CR2 & ADC1_CR2_ALIGN) /* Right alignment */
    {
        /* Read LSB first */
        templ = *(u8*)(ADC1_BaseAddress + (Buffer << 1) + 1);
        /* Then read MSB */
        temph = *(u8*)(ADC1_BaseAddress + (Buffer << 1));

        temph = (u16)(templ | (u16)(temph << (u8)8));
    }
    else /* Left alignment */
    {
        /* Read MSB firts*/
        temph = *(u8*)(ADC1_BaseAddress + (Buffer << 1));
        /* Then read LSB */
        templ = *(u8*)(ADC1_BaseAddress + (Buffer << 1) + 1);

        temph = (u16)((u16)(templ << (u8)6) | (u16)(temph << (u8)8));
    }

    return ((u16)temph);

}

/**
  * @brief Checks the specified analog watchdog channel status.
  * @param[in] Channel: specify the channel of which to check the analog watchdog
  * can be one of the values of @ref ADC1_Channel_TypeDef.
  * @retval FlagStatus Status of the analog watchdog.
  */
FlagStatus ADC1_GetAWDChannelStatus(ADC1_Channel_TypeDef Channel)
{
    u8 status = 0;

    /* Check the parameters */
    assert_param(IS_ADC1_CHANNEL_OK(Channel));

    if (Channel < (u8)8)
    {
        status = (u8)(ADC1->AWSRL & ((u8)1 << Channel));
    }
    else /* Channel = 8 | 9 */
    {
        status = (u8)(ADC1->AWSRH & ((u8)1 << (Channel - (u8)8)));
    }

    return ((FlagStatus)status);
}

/**
  * @brief Checks the specified ADC1 flag status.
  * @param[in] Flag: ADC1 flag.
  * can be one of the values of @ref ADC1_Flag_TypeDef.
  * @retval FlagStatus Status of the ADC1 flag.
  */
FlagStatus ADC1_GetFlagStatus(ADC1_Flag_TypeDef Flag)
{
    u8 flagstatus = 0;
    u8 temp = 0;

    /* Check the parameters */
    assert_param(IS_ADC1_FLAG_OK(Flag));

    if ((Flag & 0x0F) == 0x01)
    {
        /* Get OVR flag status */
        flagstatus = (u8)(ADC1->CR3 & ADC1_CR3_OVR);
    }
    else if ((Flag & 0xF0) == 0x10)
    {
        /* Get analog watchdog channel status */
        temp = (u8)(Flag & 0x0F);
        if (temp < 8)
        {
            flagstatus = (u8)(ADC1->AWSRL & (1 << temp));
        }
        else
        {
            flagstatus = (u8)(ADC1->AWSRH & (1 << (temp - 8)));
        }
    }
    else  /* Get EOC | AWD flag status */
    {
        flagstatus = (u8)(ADC1->CSR & Flag);
    }
    return ((FlagStatus)flagstatus);

}

/**
  * @brief Clear the specified ADC1 Flag.
  * @param[in] Flag: ADC1 flag.
  * can be one of the values of @ref ADC1_Flag_TypeDef.
  * @retval None
  */
void ADC1_ClearFlag(ADC1_Flag_TypeDef Flag)
{
    u8 temp = 0;

    /* Check the parameters */
    assert_param(IS_ADC1_FLAG_OK(Flag));

    if ((Flag & 0x0F) == 0x01)
    {
        /* Clear OVR flag status */
        ADC1->CR3 &= (u8)(~ADC1_CR3_OVR);
    }
    else if ((Flag & 0xF0) == 0x10)
    {
        /* Clear analog watchdog channel status */
        temp = (u8)(Flag & 0x0F);
        if (temp < 8)
        {
            ADC1->AWSRL &= (u8)(~((u8)1 << temp));
        }
        else
        {
            ADC1->AWSRH &= (u8)(~((u8)1 << (temp - 8)));
        }
    }
    else  /* Clear EOC | AWD flag status */
    {
        ADC1->CSR &= (u8) (~Flag);
    }
}

/**
  * @brief Returns the specified pending bit status
  * @param[in] ITPendingBit : the IT pending bit to check.
  * This parameter can be one of the following values:
  *    - ADC1_IT_AWD   : Analog WDG IT status
  *    - ADC1_IT_AWS0 : Analog channel 0 IT status
  *    - ADC1_IT_AWS1 : Analog channel 1 IT status
  *    - ADC1_IT_AWS2 : Analog channel 2 IT status
  *    - ADC1_IT_AWS3 : Analog channel 3 IT status
  *    - ADC1_IT_AWS4 : Analog channel 4 IT status
  *    - ADC1_IT_AWS5 : Analog channel 5 IT status
  *    - ADC1_IT_AWS6 : Analog channel 6 IT status
  *    - ADC1_IT_AWS7 : Analog channel 7 IT status
  *    - ADC1_IT_AWS8 : Analog channel 8 IT status
  *    - ADC1_IT_AWS9 : Analog channel 9 IT status
  *    - ADC1_IT_EOC    : EOC pending bit
  * @retval ITStatus: status of the specified pending bit.
  */
ITStatus ADC1_GetITStatus(ADC1_IT_TypeDef ITPendingBit)
{
    ITStatus itstatus = RESET;
    u8 temp = 0;

    /* Check the parameters */
    assert_param(IS_ADC1_ITPENDINGBIT_OK(ITPendingBit));

    if ((ITPendingBit & 0xF0) == 0x10)
    {
        /* Get analog watchdog channel status */
        temp = (u8)(ITPendingBit & 0x0F);
        if (temp < 8)
        {
            itstatus = (u8)(ADC1->AWSRL & (u8)((u8)1 << temp));
        }
        else
        {
            itstatus = (u8)(ADC1->AWSRH & (u8)((u8)1 << (temp - 8)));
        }
    }
    else  /* Get EOC | AWD flag status */
    {
        itstatus = (u8)(ADC1->CSR & ITPendingBit);
    }
    return ((ITStatus)itstatus);

}

/**
  * @brief Clear the ADC1 End of Conversion pending bit.
  * @param[in] ITPendingBit : the IT pending bit to clear.
  * This parameter can be one of the following values:
  *    - ADC1_IT_AWD   : Analog WDG IT status
  *    - ADC1_IT_AWS0 : Analog channel 0 IT status
  *    - ADC1_IT_AWS1 : Analog channel 1 IT status
  *    - ADC1_IT_AWS2 : Analog channel 2 IT status
  *    - ADC1_IT_AWS3 : Analog channel 3 IT status
  *    - ADC1_IT_AWS4 : Analog channel 4 IT status
  *    - ADC1_IT_AWS5 : Analog channel 5 IT status
  *    - ADC1_IT_AWS6 : Analog channel 6 IT status
  *    - ADC1_IT_AWS7 : Analog channel 7 IT status
  *    - ADC1_IT_AWS8 : Analog channel 8 IT status
  *    - ADC1_IT_AWS9 : Analog channel 9 IT status
  *    - ADC1_IT_EOC  : EOC pending bit
  * @retval None
  */
void ADC1_ClearITPendingBit(ADC1_IT_TypeDef ITPendingBit)
{
    u8 temp = 0;

    /* Check the parameters */
    assert_param(IS_ADC1_ITPENDINGBIT_OK(ITPendingBit));

    if ((ITPendingBit& 0xF0) == 0x10)
    {
        /* Clear analog watchdog channel status */
        temp = (u8)(ITPendingBit & 0x0F);
        if (temp < 8)
        {
            ADC1->AWSRL &= (u8)(~((u8)1 << temp));
        }
        else
        {
            ADC1->AWSRH &= (u8)(~((u8)1 << (temp - 8)));
        }
    }
    else  /* Clear EOC | AWD flag status */
    {
        ADC1->CSR &= (u8) (~ITPendingBit);
    }
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
