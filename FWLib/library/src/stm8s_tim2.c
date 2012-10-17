/**
  ******************************************************************************
  * @file stm8s_tim2.c
  * @brief This file contains all the functions for the TIM2 peripheral.
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
#include "stm8s_tim2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TI1_Config(u8 TIM2_ICPolarity, u8 TIM2_ICSelection, u8 TIM2_ICFilter);
static void TI2_Config(u8 TIM2_ICPolarity, u8 TIM2_ICSelection, u8 TIM2_ICFilter);
static void TI3_Config(u8 TIM2_ICPolarity, u8 TIM2_ICSelection, u8 TIM2_ICFilter);
/**
  * @addtogroup TIM2_Public_Functions
  * @{
  */

/**
  * @brief Deinitializes the TIM2 peripheral registers to their default reset values.
  * @param[in] :
  * None
  * @retval None
  */
void TIM2_DeInit(void)
{

    TIM2->CR1 = (u8)TIM2_CR1_RESET_VALUE;
    TIM2->IER = (u8)TIM2_IER_RESET_VALUE;
    TIM2->SR2 = (u8)TIM2_SR2_RESET_VALUE;

    /* Disable channels */
    TIM2->CCER1 = (u8)TIM2_CCER1_RESET_VALUE;
    TIM2->CCER2 = (u8)TIM2_CCER2_RESET_VALUE;


    /* Then reset channel registers: it also works if lock level is equal to 2 or 3 */
    TIM2->CCER1 = (u8)TIM2_CCER1_RESET_VALUE;
    TIM2->CCER2 = (u8)TIM2_CCER2_RESET_VALUE;
    TIM2->CCMR1 = (u8)TIM2_CCMR1_RESET_VALUE;
    TIM2->CCMR2 = (u8)TIM2_CCMR2_RESET_VALUE;
    TIM2->CCMR3 = (u8)TIM2_CCMR3_RESET_VALUE;
    TIM2->CNTRH = (u8)TIM2_CNTRH_RESET_VALUE;
    TIM2->CNTRL = (u8)TIM2_CNTRL_RESET_VALUE;
    TIM2->PSCR = (u8)TIM2_PSCR_RESET_VALUE;
    TIM2->ARRH  = (u8)TIM2_ARRH_RESET_VALUE;
    TIM2->ARRL  = (u8)TIM2_ARRL_RESET_VALUE;
    TIM2->CCR1H = (u8)TIM2_CCR1H_RESET_VALUE;
    TIM2->CCR1L = (u8)TIM2_CCR1L_RESET_VALUE;
    TIM2->CCR2H = (u8)TIM2_CCR2H_RESET_VALUE;
    TIM2->CCR2L = (u8)TIM2_CCR2L_RESET_VALUE;
    TIM2->CCR3H = (u8)TIM2_CCR3H_RESET_VALUE;
    TIM2->CCR3L = (u8)TIM2_CCR3L_RESET_VALUE;
    TIM2->SR1 = (u8)TIM2_SR1_RESET_VALUE;
}


/**
  * @brief Initializes the TIM2 Time Base Unit according to the specified parameters.
  * @param[in]  TIM2_Prescaler specifies the Prescaler from TIM2_Prescaler_TypeDef.
  * @param[in]  TIM2_Period specifies the Period value.
  * @retval None
  */
void TIM2_TimeBaseInit( TIM2_Prescaler_TypeDef TIM2_Prescaler,
                        u16 TIM2_Period)
{
    /* Set the Prescaler value */
    TIM2->PSCR = (u8)(TIM2_Prescaler);
    /* Set the Autoreload value */
    TIM2->ARRH = (u8)(TIM2_Period >> 8);
    TIM2->ARRL = (u8)(TIM2_Period);
}


/**
  * @brief Initializes the TIM2 Channel1 according to the specified parameters.
  * @param[in] TIM2_OCMode specifies the Output Compare mode  from @ref TIM2_OCMode_TypeDef.
  * @param[in] TIM2_OutputState specifies the Output State  from @ref TIM2_OutputState_TypeDef.
  * @param[in] TIM2_Pulse specifies the Pulse width  value.
  * @param[in] TIM2_OCPolarity specifies the Output Compare Polarity  from @ref TIM2_OCPolarity_TypeDef.
  * @retval None
  */
void TIM2_OC1Init(TIM2_OCMode_TypeDef TIM2_OCMode,
                  TIM2_OutputState_TypeDef TIM2_OutputState,
                  u16 TIM2_Pulse,
                  TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
    /* Check the parameters */
    assert_param(IS_TIM2_OC_MODE_OK(TIM2_OCMode));
    assert_param(IS_TIM2_OUTPUT_STATE_OK(TIM2_OutputState));
    assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));

    /* Disable the Channel 1: Reset the CCE Bit, Set the Output State , the Output Polarity */
    TIM2->CCER1 &= (u8)(~( TIM2_CCER1_CC1E | TIM2_CCER1_CC1P));
    /* Set the Output State &  Set the Output Polarity  */
    TIM2->CCER1 |= (u8)((TIM2_OutputState  & TIM2_CCER1_CC1E   ) | (TIM2_OCPolarity   & TIM2_CCER1_CC1P   ));

    /* Reset the Output Compare Bits  & Set the Ouput Compare Mode */
    TIM2->CCMR1 = (u8)((TIM2->CCMR1 & (u8)(~TIM2_CCMR_OCM)) | (u8)TIM2_OCMode);

    /* Set the Pulse value */
    TIM2->CCR1H = (u8)(TIM2_Pulse >> 8);
    TIM2->CCR1L = (u8)(TIM2_Pulse);
}


/**
  * @brief Initializes the TIM2 Channel2 according to the specified parameters.
  * @param[in] TIM2_OCMode specifies the Output Compare mode  from @ref TIM2_OCMode_TypeDef.
  * @param[in] TIM2_OutputState specifies the Output State  from @ref TIM2_OutputState_TypeDef.
  * @param[in] TIM2_Pulse specifies the Pulse width  value.
  * @param[in] TIM2_OCPolarity specifies the Output Compare Polarity  from @ref TIM2_OCPolarity_TypeDef.
  * @retval None
  */
void TIM2_OC2Init(TIM2_OCMode_TypeDef TIM2_OCMode,
                  TIM2_OutputState_TypeDef TIM2_OutputState,
                  u16 TIM2_Pulse,
                  TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
    /* Check the parameters */
    assert_param(IS_TIM2_OC_MODE_OK(TIM2_OCMode));
    assert_param(IS_TIM2_OUTPUT_STATE_OK(TIM2_OutputState));
    assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));


    /* Disable the Channel 1: Reset the CCE Bit, Set the Output State, the Output Polarity */
    TIM2->CCER1 &= (u8)(~( TIM2_CCER1_CC2E |  TIM2_CCER1_CC2P ));
    /* Set the Output State & Set the Output Polarity */
    TIM2->CCER1 |= (u8)((TIM2_OutputState  & TIM2_CCER1_CC2E   ) | \
                        (TIM2_OCPolarity   & TIM2_CCER1_CC2P   ));


    /* Reset the Output Compare Bits & Set the Output Compare Mode */
    TIM2->CCMR2 = (u8)((TIM2->CCMR2 & (u8)(~TIM2_CCMR_OCM)) | (u8)TIM2_OCMode);


    /* Set the Pulse value */
    TIM2->CCR2H = (u8)(TIM2_Pulse >> 8);
    TIM2->CCR2L = (u8)(TIM2_Pulse);
}


/**
  * @brief Initializes the TIM2 Channel3 according to the specified parameters.
  * @param[in] TIM2_OCMode specifies the Output Compare mode from @ref TIM2_OCMode_TypeDef.
  * @param[in] TIM2_OutputState specifies the Output State from @ref TIM2_OutputState_TypeDef.
  * @param[in] TIM2_Pulse specifies the Pulse width value.
  * @param[in] TIM2_OCPolarity specifies the Output Compare Polarity  from @ref TIM2_OCPolarity_TypeDef.
  * @retval None
  */
void TIM2_OC3Init(TIM2_OCMode_TypeDef TIM2_OCMode,
                  TIM2_OutputState_TypeDef TIM2_OutputState,
                  u16 TIM2_Pulse,
                  TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
    /* Check the parameters */
    assert_param(IS_TIM2_OC_MODE_OK(TIM2_OCMode));
    assert_param(IS_TIM2_OUTPUT_STATE_OK(TIM2_OutputState));
    assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));
    /* Disable the Channel 1: Reset the CCE Bit, Set the Output State, the Output Polarity */
    TIM2->CCER2 &= (u8)(~( TIM2_CCER2_CC3E  | TIM2_CCER2_CC3P));
    /* Set the Output State & Set the Output Polarity */
    TIM2->CCER2 |= (u8)((TIM2_OutputState  & TIM2_CCER2_CC3E   ) |  (TIM2_OCPolarity   & TIM2_CCER2_CC3P   ));

    /* Reset the Output Compare Bits & Set the Output Compare Mode */
    TIM2->CCMR3 = (u8)((TIM2->CCMR3 & (u8)(~TIM2_CCMR_OCM)) | (u8)TIM2_OCMode);

    /* Set the Pulse value */
    TIM2->CCR3H = (u8)(TIM2_Pulse >> 8);
    TIM2->CCR3L = (u8)(TIM2_Pulse);

}


/**
  * @brief Initializes the TIM2 peripheral according to the specified parameters.
  * @param[in]  TIM2_Channel specifies the Input Capture Channel from @ref TIM2_Channel_TypeDef.
  * @param[in] TIM2_ICPolarity specifies the Input Capture Polarity from @ref TIM2_ICPolarity_TypeDef.
  * @param[in] TIM2_ICSelection specifies the Input Capture Selection from @ref TIM2_ICSelection_TypeDef.
  * @param[in] TIM2_ICPrescaler specifies the Input Capture Prescaler from @ref TIM2_ICPSC_TypeDef.
  * @param[in] TIM2_ICFilter specifies the Input Capture Filter value (value can be an integer from 0x00 to 0x0F).
  * @retval None
  */
void TIM2_ICInit(TIM2_Channel_TypeDef TIM2_Channel,
                 TIM2_ICPolarity_TypeDef TIM2_ICPolarity,
                 TIM2_ICSelection_TypeDef TIM2_ICSelection,
                 TIM2_ICPSC_TypeDef TIM2_ICPrescaler,
                 u8 TIM2_ICFilter)
{
    /* Check the parameters */
    assert_param(IS_TIM2_CHANNEL_OK(TIM2_Channel));
    assert_param(IS_TIM2_IC_POLARITY_OK(TIM2_ICPolarity));
    assert_param(IS_TIM2_IC_SELECTION_OK(TIM2_ICSelection));
    assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_ICPrescaler));
    assert_param(IS_TIM2_IC_FILTER_OK(TIM2_ICFilter));

    if (TIM2_Channel == TIM2_CHANNEL_1)
    {
        /* TI1 Configuration */
        TI1_Config((u8)TIM2_ICPolarity,
                   (u8)TIM2_ICSelection,
                   (u8)TIM2_ICFilter);

        /* Set the Input Capture Prescaler value */
        TIM2_SetIC1Prescaler(TIM2_ICPrescaler);
    }
    else if (TIM2_Channel == TIM2_CHANNEL_2)
    {
        /* TI2 Configuration */
        TI2_Config((u8)TIM2_ICPolarity,
                   (u8)TIM2_ICSelection,
                   (u8)TIM2_ICFilter);

        /* Set the Input Capture Prescaler value */
        TIM2_SetIC2Prescaler(TIM2_ICPrescaler);
    }
    else
    {
        /* TI3 Configuration */
        TI3_Config((u8)TIM2_ICPolarity,
                   (u8)TIM2_ICSelection,
                   (u8)TIM2_ICFilter);

        /* Set the Input Capture Prescaler value */
        TIM2_SetIC3Prescaler(TIM2_ICPrescaler);
    }
}


/**
  * @brief Configures the TIM2 peripheral in PWM Input Mode according to the specified parameters.
    * @param[in]  TIM2_Channel specifies the Input Capture Channel from @ref TIM2_Channel_TypeDef.
  * @param[in] TIM2_ICPolarity specifies the Input Capture Polarity from @ref TIM2_ICPolarity_TypeDef.
  * @param[in] TIM2_ICSelection specifies the Input Capture Selection from @ref TIM2_ICSelection_TypeDef.
  * @param[in] TIM2_ICPrescaler specifies the Input Capture Prescaler from @ref TIM2_ICPSC_TypeDef.
  * @param[in] TIM2_ICFilter specifies the Input Capture Filter value (value can be an integer from 0x00 to 0x0F).
  * @retval None
  */
void TIM2_PWMIConfig(TIM2_Channel_TypeDef TIM2_Channel,
                     TIM2_ICPolarity_TypeDef TIM2_ICPolarity,
                     TIM2_ICSelection_TypeDef TIM2_ICSelection,
                     TIM2_ICPSC_TypeDef TIM2_ICPrescaler,
                     u8 TIM2_ICFilter)
{
    u8 icpolarity = (u8)TIM2_ICPOLARITY_RISING;
    u8 icselection = (u8)TIM2_ICSELECTION_DIRECTTI;

    /* Check the parameters */
    assert_param(IS_TIM2_PWMI_CHANNEL_OK(TIM2_Channel));
    assert_param(IS_TIM2_IC_POLARITY_OK(TIM2_ICPolarity));
    assert_param(IS_TIM2_IC_SELECTION_OK(TIM2_ICSelection));
    assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_ICPrescaler));

    /* Select the Opposite Input Polarity */
    if (TIM2_ICPolarity != TIM2_ICPOLARITY_FALLING)
    {
        icpolarity = (u8)TIM2_ICPOLARITY_FALLING;
    }
    else
    {
        icpolarity = (u8)TIM2_ICPOLARITY_RISING;
    }

    /* Select the Opposite Input */
    if (TIM2_ICSelection == TIM2_ICSELECTION_DIRECTTI)
    {
        icselection = (u8)TIM2_ICSELECTION_INDIRECTTI;
    }
    else
    {
        icselection = (u8)TIM2_ICSELECTION_DIRECTTI;
    }

    if (TIM2_Channel == TIM2_CHANNEL_1)
    {
        /* TI1 Configuration */
        TI1_Config((u8)TIM2_ICPolarity, (u8)TIM2_ICSelection,
                   (u8)TIM2_ICFilter);

        /* Set the Input Capture Prescaler value */
        TIM2_SetIC1Prescaler(TIM2_ICPrescaler);

        /* TI2 Configuration */
        TI2_Config(icpolarity, icselection, TIM2_ICFilter);

        /* Set the Input Capture Prescaler value */
        TIM2_SetIC2Prescaler(TIM2_ICPrescaler);
    }
    else
    {
        /* TI2 Configuration */
        TI2_Config((u8)TIM2_ICPolarity, (u8)TIM2_ICSelection,
                   (u8)TIM2_ICFilter);

        /* Set the Input Capture Prescaler value */
        TIM2_SetIC2Prescaler(TIM2_ICPrescaler);

        /* TI1 Configuration */
        TI1_Config((u8)icpolarity, icselection, (u8)TIM2_ICFilter);

        /* Set the Input Capture Prescaler value */
        TIM2_SetIC1Prescaler(TIM2_ICPrescaler);
    }
}


/**
  * @brief Enables or disables the TIM2 peripheral.
  * @param[in] NewState new state of the TIM2 peripheral. This parameter can
  * be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* set or Reset the CEN Bit */
    if (NewState != DISABLE)
    {
        TIM2->CR1 |= (u8)TIM2_CR1_CEN;
    }
    else
    {
        TIM2->CR1 &= (u8)(~TIM2_CR1_CEN);
    }
}


/**
  * @brief Enables or disables the specified TIM2 interrupts.
  * @param[in] NewState new state of the TIM2 peripheral.
  * This parameter can be: ENABLE or DISABLE.
  * @param[in] TIM2_IT specifies the TIM2 interrupts sources to be enabled or disabled.
  * This parameter can be any combination of the following values:
  *                       - TIM2_IT_UPDATE: TIM2 update Interrupt source
  *                       - TIM2_IT_CC1: TIM2 Capture Compare 1 Interrupt source
  *                       - TIM2_IT_CC2: TIM2 Capture Compare 2 Interrupt source
  *                       - TIM2_IT_CC3: TIM2 Capture Compare 3 Interrupt source
  * @param[in] NewState new state of the TIM2 peripheral.
  * @retval None
  */
void TIM2_ITConfig(TIM2_IT_TypeDef TIM2_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM2_IT_OK(TIM2_IT));
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the Interrupt sources */
        TIM2->IER |= (u8)TIM2_IT;
    }
    else
    {
        /* Disable the Interrupt sources */
        TIM2->IER &= (u8)(~TIM2_IT);
    }
}


/**
  * @brief Enables or Disables the TIM2 Update event.
  * @param[in] NewState new state of the TIM2 peripheral Preload register. This parameter can
  * be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_UpdateDisableConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* Set or Reset the UDIS Bit */
    if (NewState != DISABLE)
    {
        TIM2->CR1 |= (u8)TIM2_CR1_UDIS;
    }
    else
    {
        TIM2->CR1 &= (u8)(~TIM2_CR1_UDIS);
    }
}

/**
  * @brief Selects the TIM2 Update Request Interrupt source.
  * @param[in] TIM2_UpdateSource specifies the Update source.
  * This parameter can be one of the following values
  *                       - TIM2_UPDATESOURCE_REGULAR
  *                       - TIM2_UPDATESOURCE_GLOBAL
  * @retval None
  */
void TIM2_UpdateRequestConfig(TIM2_UpdateSource_TypeDef TIM2_UpdateSource)
{
    /* Check the parameters */
    assert_param(IS_TIM2_UPDATE_SOURCE_OK(TIM2_UpdateSource));

    /* Set or Reset the URS Bit */
    if (TIM2_UpdateSource != TIM2_UPDATESOURCE_GLOBAL)
    {
        TIM2->CR1 |= (u8)TIM2_CR1_URS;
    }
    else
    {
        TIM2->CR1 &= (u8)(~TIM2_CR1_URS);
    }
}


/**
  * @brief Selects the TIM2’s One Pulse Mode.
  * @param[in] TIM2_OPMode specifies the OPM Mode to be used.
  * This parameter can be one of the following values
  *                    - TIM2_OPMODE_SINGLE
  *                    - TIM2_OPMODE_REPETITIVE
  * @retval None
  */
void TIM2_SelectOnePulseMode(TIM2_OPMode_TypeDef TIM2_OPMode)
{
    /* Check the parameters */
    assert_param(IS_TIM2_OPM_MODE_OK(TIM2_OPMode));

    /* Set or Reset the OPM Bit */
    if (TIM2_OPMode != TIM2_OPMODE_REPETITIVE)
    {
        TIM2->CR1 |= (u8)TIM2_CR1_OPM;
    }
    else
    {
        TIM2->CR1 &= (u8)(~TIM2_CR1_OPM);
    }

}


/**
  * @brief Configures the TIM2 Prescaler.
  * @param[in] Prescaler specifies the Prescaler Register value
  * This parameter can be one of the following values
  *                       -  TIM2_PRESCALER_1
  *                       -  TIM2_PRESCALER_2
  *                       -  TIM2_PRESCALER_4
  *                       -  TIM2_PRESCALER_8
  *                       -  TIM2_PRESCALER_16
  *                       -  TIM2_PRESCALER_32
  *                       -  TIM2_PRESCALER_64
  *                       -  TIM2_PRESCALER_128
  *                       -  TIM2_PRESCALER_256
  *                       -  TIM2_PRESCALER_512
  *                       -  TIM2_PRESCALER_1024
  *                       -  TIM2_PRESCALER_2048
  *                       -  TIM2_PRESCALER_4096
  *                       -  TIM2_PRESCALER_8192
  *                       -  TIM2_PRESCALER_16384
  *                       -  TIM2_PRESCALER_32768
  * @param[in] TIM2_PSCReloadMode specifies the TIM2 Prescaler Reload mode.
  * This parameter can be one of the following values
  *                       - TIM2_PSCRELOADMODE_IMMEDIATE: The Prescaler is loaded
  *                         immediately.
  *                       - TIM2_PSCRELOADMODE_UPDATE: The Prescaler is loaded at
  *                         the update event.
  * @retval None
  */
void TIM2_PrescalerConfig(TIM2_Prescaler_TypeDef Prescaler,
                          TIM2_PSCReloadMode_TypeDef TIM2_PSCReloadMode)
{
    /* Check the parameters */
    assert_param(IS_TIM2_PRESCALER_RELOAD_OK(TIM2_PSCReloadMode));
    assert_param(IS_TIM2_PRESCALER_OK(Prescaler));

    /* Set the Prescaler value */
    TIM2->PSCR = (u8)Prescaler;

    /* Set or reset the UG Bit */
    TIM2->EGR = (u8)TIM2_PSCReloadMode;
}

/**
  * @brief Forces the TIM2 Channel1 output waveform to active or inactive level.
  * @param[in] TIM2_ForcedAction specifies the forced Action to be set to the output waveform.
  * This parameter can be one of the following values:
  *                       - TIM2_FORCEDACTION_ACTIVE: Force active level on OC1REF
  *                       - TIM2_FORCEDACTION_INACTIVE: Force inactive level on
  *                         OC1REF.
  * @retval None
  */
void TIM2_ForcedOC1Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction)
{
    /* Check the parameters */
    assert_param(IS_TIM2_FORCED_ACTION_OK(TIM2_ForcedAction));

    /* Reset the OCM Bits & Configure the Forced output Mode */
    TIM2->CCMR1  =  (u8)((TIM2->CCMR1 & (u8)(~TIM2_CCMR_OCM))  | (u8)TIM2_ForcedAction);
}

/**
  * @brief Forces the TIM2 Channel2 output waveform to active or inactive level.
  * @param[in] TIM2_ForcedAction specifies the forced Action to be set to the output waveform.
  * This parameter can be one of the following values:
  *                       - TIM2_FORCEDACTION_ACTIVE: Force active level on OC2REF
  *                       - TIM2_FORCEDACTION_INACTIVE: Force inactive level on
  *                         OC2REF.
  * @retval None
  */
void TIM2_ForcedOC2Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction)
{
    /* Check the parameters */
    assert_param(IS_TIM2_FORCED_ACTION_OK(TIM2_ForcedAction));

    /* Reset the OCM Bits & Configure the Forced output Mode */
    TIM2->CCMR2 = (u8)((TIM2->CCMR2 & (u8)(~TIM2_CCMR_OCM))  | (u8)TIM2_ForcedAction);
}

/**
  * @brief Forces the TIM2 Channel3 output waveform to active or inactive level.
  * @param[in] TIM2_ForcedAction specifies the forced Action to be set to the output waveform.
  * This parameter can be one of the following values:
  *                       - TIM2_FORCEDACTION_ACTIVE: Force active level on OC3REF
  *                       - TIM2_FORCEDACTION_INACTIVE: Force inactive level on
  *                         OC3REF.
  * @retval None
  */
void TIM2_ForcedOC3Config(TIM2_ForcedAction_TypeDef TIM2_ForcedAction)
{
    /* Check the parameters */
    assert_param(IS_TIM2_FORCED_ACTION_OK(TIM2_ForcedAction));

    /* Reset the OCM Bits & Configure the Forced output Mode */
    TIM2->CCMR3  =  (u8)((TIM2->CCMR3 & (u8)(~TIM2_CCMR_OCM))  | (u8)TIM2_ForcedAction);
}


/**
  * @brief Enables or disables TIM2 peripheral Preload register on ARR.
  * @param[in] NewState new state of the TIM2 peripheral Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_ARRPreloadConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* Set or Reset the ARPE Bit */
    if (NewState != DISABLE)
    {
        TIM2->CR1 |= (u8)TIM2_CR1_ARPE;
    }
    else
    {
        TIM2->CR1 &= (u8)(~TIM2_CR1_ARPE);
    }
}


/**
  * @brief Enables or disables the TIM2 peripheral Preload Register on CCR1.
  * @param[in] NewState new state of the Capture Compare Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_OC1PreloadConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* Set or Reset the OC1PE Bit */
    if (NewState != DISABLE)
    {
        TIM2->CCMR1 |= (u8)TIM2_CCMR_OCxPE;
    }
    else
    {
        TIM2->CCMR1 &= (u8)(~TIM2_CCMR_OCxPE);
    }
}


/**
  * @brief Enables or disables the TIM2 peripheral Preload Register on CCR2.
  * @param[in] NewState new state of the Capture Compare Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_OC2PreloadConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* Set or Reset the OC2PE Bit */
    if (NewState != DISABLE)
    {
        TIM2->CCMR2 |= (u8)TIM2_CCMR_OCxPE;
    }
    else
    {
        TIM2->CCMR2 &= (u8)(~TIM2_CCMR_OCxPE);
    }
}


/**
  * @brief Enables or disables the TIM2 peripheral Preload Register on CCR3.
  * @param[in] NewState new state of the Capture Compare Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM2_OC3PreloadConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* Set or Reset the OC3PE Bit */
    if (NewState != DISABLE)
    {
        TIM2->CCMR3 |= (u8)TIM2_CCMR_OCxPE;
    }
    else
    {
        TIM2->CCMR3 &= (u8)(~TIM2_CCMR_OCxPE);
    }
}


/**
  * @brief Configures the TIM2 event to be generated by software.
  * @param[in] TIM2_EventSource specifies the event source.
  * This parameter can be one of the following values:
  *                       - TIM2_EVENTSOURCE_UPDATE: TIM2 update Event source
  *                       - TIM2_EVENTSOURCE_CC1: TIM2 Capture Compare 1 Event source
  *                       - TIM2_EVENTSOURCE_CC2: TIM2 Capture Compare 2 Event source
  *                       - TIM2_EVENTSOURCE_CC3: TIM2 Capture Compare 3 Event source
  * @retval None
  */
void TIM2_GenerateEvent(TIM2_EventSource_TypeDef TIM2_EventSource)
{
    /* Check the parameters */
    assert_param(IS_TIM2_EVENT_SOURCE_OK(TIM2_EventSource));

    /* Set the event sources */
    TIM2->EGR = (u8)TIM2_EventSource;
}


/**
  * @brief Configures the TIM2 Channel 1 polarity.
  * @param[in] TIM2_OCPolarity specifies the OC1 Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_OCPOLARITY_LOW: Output Compare active low
  *                       - TIM2_OCPOLARITY_HIGH: Output Compare active high
  * @retval None
  */
void TIM2_OC1PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
    /* Check the parameters */
    assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));

    /* Set or Reset the CC1P Bit */
    if (TIM2_OCPolarity != TIM2_OCPOLARITY_HIGH)
    {
        TIM2->CCER1 |= (u8)TIM2_CCER1_CC1P;
    }
    else
    {
        TIM2->CCER1 &= (u8)(~TIM2_CCER1_CC1P);
    }
}


/**
  * @brief Configures the TIM2 Channel 2 polarity.
  * @param[in] TIM2_OCPolarity specifies the OC2 Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_OCPOLARITY_LOW: Output Compare active low
  *                       - TIM2_OCPOLARITY_HIGH: Output Compare active high
  * @retval None
  */
void TIM2_OC2PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
    /* Check the parameters */
    assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));

    /* Set or Reset the CC2P Bit */
    if (TIM2_OCPolarity != TIM2_OCPOLARITY_HIGH)
    {
        TIM2->CCER1 |= TIM2_CCER1_CC2P;
    }
    else
    {
        TIM2->CCER1 &= (u8)(~TIM2_CCER1_CC2P);
    }
}


/**
  * @brief Configures the TIM2 Channel 3 polarity.
  * @param[in] TIM2_OCPolarity specifies the OC3 Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_OCPOLARITY_LOW: Output Compare active low
  *                       - TIM2_OCPOLARITY_HIGH: Output Compare active high
  * @retval None
  */
void TIM2_OC3PolarityConfig(TIM2_OCPolarity_TypeDef TIM2_OCPolarity)
{
    /* Check the parameters */
    assert_param(IS_TIM2_OC_POLARITY_OK(TIM2_OCPolarity));

    /* Set or Reset the CC3P Bit */
    if (TIM2_OCPolarity != TIM2_OCPOLARITY_HIGH)
    {
        TIM2->CCER2 |= (u8)TIM2_CCER2_CC3P;
    }
    else
    {
        TIM2->CCER2 &= (u8)(~TIM2_CCER2_CC3P);
    }
}


/**
  * @brief Enables or disables the TIM2 Capture Compare Channel x.
  * @param[in] TIM2_Channel specifies the TIM2 Channel.
  * This parameter can be one of the following values:
  *                       - TIM2_CHANNEL_1: TIM2 Channel1
  *                       - TIM2_CHANNEL_2: TIM2 Channel2
  *                       - TIM2_CHANNEL_3: TIM2 Channel3
  * @param[in] NewState specifies the TIM2 Channel CCxE bit new state.
  * This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM2_CCxCmd(TIM2_Channel_TypeDef TIM2_Channel, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM2_CHANNEL_OK(TIM2_Channel));
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (TIM2_Channel == TIM2_CHANNEL_1)
    {
        /* Set or Reset the CC1E Bit */
        if (NewState != DISABLE)
        {
            TIM2->CCER1 |= (u8)TIM2_CCER1_CC1E;
        }
        else
        {
            TIM2->CCER1 &= (u8)(~TIM2_CCER1_CC1E);
        }

    }
    else if (TIM2_Channel == TIM2_CHANNEL_2)
    {
        /* Set or Reset the CC2E Bit */
        if (NewState != DISABLE)
        {
            TIM2->CCER1 |= (u8)TIM2_CCER1_CC2E;
        }
        else
        {
            TIM2->CCER1 &= (u8)(~TIM2_CCER1_CC2E);
        }
    }
    else
    {
        /* Set or Reset the CC3E Bit */
        if (NewState != DISABLE)
        {
            TIM2->CCER2 |= (u8)TIM2_CCER2_CC3E;
        }
        else
        {
            TIM2->CCER2 &= (u8)(~TIM2_CCER2_CC3E);
        }
    }
}

/**
  * @brief Selects the TIM2 Output Compare Mode. This function disables the
  * selected channel before changing the Output Compare Mode. User has to
  * enable this channel using TIM2_CCxCmd and TIM2_CCxNCmd functions.
  * @param[in] TIM2_Channel specifies the TIM2 Channel.
  * This parameter can be one of the following values:
  *                       - TIM2_CHANNEL_1: TIM2 Channel1
  *                       - TIM2_CHANNEL_2: TIM2 Channel2
  *                       - TIM2_CHANNEL_3: TIM2 Channel3
  * @param[in] TIM2_OCMode specifies the TIM2 Output Compare Mode.
  * This paramter can be one of the following values:
  *                       - TIM2_OCMODE_TIMING
  *                       - TIM2_OCMODE_ACTIVE
  *                       - TIM2_OCMODE_TOGGLE
  *                       - TIM2_OCMODE_PWM1
  *                       - TIM2_OCMODE_PWM2
  *                       - TIM2_FORCEDACTION_ACTIVE
  *                       - TIM2_FORCEDACTION_INACTIVE
  * @retval None
  */
void TIM2_SelectOCxM(TIM2_Channel_TypeDef TIM2_Channel, TIM2_OCMode_TypeDef TIM2_OCMode)
{
    /* Check the parameters */
    assert_param(IS_TIM2_CHANNEL_OK(TIM2_Channel));
    assert_param(IS_TIM2_OCM_OK(TIM2_OCMode));

    if (TIM2_Channel == TIM2_CHANNEL_1)
    {
        /* Disable the Channel 1: Reset the CCE Bit */
        TIM2->CCER1 &= (u8)(~TIM2_CCER1_CC1E);

        /* Reset the Output Compare Bits & Set the Output Compare Mode */
        TIM2->CCMR1 = (u8)((TIM2->CCMR1 & (u8)(~TIM2_CCMR_OCM)) | (u8)TIM2_OCMode);
    }
    else if (TIM2_Channel == TIM2_CHANNEL_2)
    {
        /* Disable the Channel 2: Reset the CCE Bit */
        TIM2->CCER1 &= (u8)(~TIM2_CCER1_CC2E);

        /* Reset the Output Compare Bits & Set the Output Compare Mode */
        TIM2->CCMR2 = (u8)((TIM2->CCMR2 & (u8)(~TIM2_CCMR_OCM)) | (u8)TIM2_OCMode);
    }
    else
    {
        /* Disable the Channel 3: Reset the CCE Bit */
        TIM2->CCER2 &= (u8)(~TIM2_CCER2_CC3E);

        /* Reset the Output Compare Bits & Set the Output Compare Mode */
        TIM2->CCMR3 = (u8)((TIM2->CCMR3 & (u8)(~TIM2_CCMR_OCM)) | (u8)TIM2_OCMode);
    }
}


/**
  * @brief Sets the TIM2 Counter Register value.
  * @param[in] Counter specifies the Counter register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM2_SetCounter(u16 Counter)
{
    /* Set the Counter Register value */
    TIM2->CNTRH = (u8)(Counter >> 8);
    TIM2->CNTRL = (u8)(Counter);

}


/**
  * @brief Sets the TIM2 Autoreload Register value.
  * @param[in] Autoreload specifies the Autoreload register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM2_SetAutoreload(u16 Autoreload)
{

    /* Set the Autoreload Register value */
    TIM2->ARRH = (u8)(Autoreload >> 8);
    TIM2->ARRL = (u8)(Autoreload);

}


/**
  * @brief Sets the TIM2 Capture Compare1 Register value.
  * @param[in] Compare1 specifies the Capture Compare1 register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM2_SetCompare1(u16 Compare1)
{
    /* Set the Capture Compare1 Register value */
    TIM2->CCR1H = (u8)(Compare1 >> 8);
    TIM2->CCR1L = (u8)(Compare1);

}


/**
  * @brief Sets the TIM2 Capture Compare2 Register value.
  * @param[in] Compare2 specifies the Capture Compare2 register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM2_SetCompare2(u16 Compare2)
{
    /* Set the Capture Compare2 Register value */
    TIM2->CCR2H = (u8)(Compare2 >> 8);
    TIM2->CCR2L = (u8)(Compare2);

}


/**
  * @brief Sets the TIM2 Capture Compare3 Register value.
  * @param[in] Compare3 specifies the Capture Compare3 register new value.
  * This parameter is between 0x0000 and 0xFFFF.
  * @retval None
  */
void TIM2_SetCompare3(u16 Compare3)
{
    /* Set the Capture Compare3 Register value */
    TIM2->CCR3H = (u8)(Compare3 >> 8);
    TIM2->CCR3L = (u8)(Compare3);

}


/**
  * @brief Sets the TIM2 Input Capture 1 Prescaler.
  * @param[in] TIM2_IC1Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM2_ICPSC_DIV1: no prescaler
  *                       - TIM2_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM2_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM2_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM2_SetIC1Prescaler(TIM2_ICPSC_TypeDef TIM2_IC1Prescaler)
{
    /* Check the parameters */
    assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_IC1Prescaler));

    /* Reset the IC1PSC Bits &Set the IC1PSC value */
    TIM2->CCMR1 = (u8)((TIM2->CCMR1 & (u8)(~TIM2_CCMR_ICxPSC)) | (u8)TIM2_IC1Prescaler);
}

/**
  * @brief Sets the TIM2 Input Capture 2 prescaler.
  * @param[in] TIM2_IC2Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM2_ICPSC_DIV1: no prescaler
  *                       - TIM2_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM2_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM2_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM2_SetIC2Prescaler(TIM2_ICPSC_TypeDef TIM2_IC2Prescaler)
{
    /* Check the parameters */
    assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_IC2Prescaler));

    /* Reset the IC1PSC Bits &Set the IC1PSC value */
    TIM2->CCMR2 = (u8)((TIM2->CCMR2 & (u8)(~TIM2_CCMR_ICxPSC)) | (u8)TIM2_IC2Prescaler);
}

/**
  * @brief Sets the TIM2 Input Capture 3 prescaler.
  * @param[in] TIM2_IC3Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM2_ICPSC_DIV1: no prescaler
  *                       - TIM2_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM2_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM2_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM2_SetIC3Prescaler(TIM2_ICPSC_TypeDef TIM2_IC3Prescaler)
{

    /* Check the parameters */
    assert_param(IS_TIM2_IC_PRESCALER_OK(TIM2_IC3Prescaler));
    /* Reset the IC1PSC Bits &Set the IC1PSC value */
    TIM2->CCMR3 = (u8)((TIM2->CCMR3 & (u8)(~TIM2_CCMR_ICxPSC)) | (u8)TIM2_IC3Prescaler);
}

/**
  * @brief Gets the TIM2 Input Capture 1 value.
  * @param[in] :
  * None
  * @retval Capture Compare 1 Register value.
  */
u16 TIM2_GetCapture1(void)
{
    /* Get the Capture 1 Register value */
    u16 tmpccr1 = 0;
    u8 tmpccr1l=0, tmpccr1h=0;

    tmpccr1h = TIM2->CCR1H;
    tmpccr1l = TIM2->CCR1L;

    tmpccr1 = (u16)(tmpccr1l);
    tmpccr1 |= (u16)((u16)tmpccr1h << 8);
    /* Get the Capture 1 Register value */
    return (u16)tmpccr1;
}

/**
  * @brief Gets the TIM2 Input Capture 2 value.
  * @param[in] :
  * None
  * @retval Capture Compare 2 Register value.
  */
u16 TIM2_GetCapture2(void)
{
    /* Get the Capture 2 Register value */
    u16 tmpccr2 = 0;
    u8 tmpccr2l=0, tmpccr2h=0;

    tmpccr2h = TIM2->CCR2H;
    tmpccr2l = TIM2->CCR2L;

    tmpccr2 = (u16)(tmpccr2l);
    tmpccr2 |= (u16)((u16)tmpccr2h << 8);
    /* Get the Capture 2 Register value */
    return (u16)tmpccr2;
}

/**
  * @brief Gets the TIM2 Input Capture 3 value.
  * @param[in] :
  * None
  * @retval Capture Compare 3 Register value.
  */
u16 TIM2_GetCapture3(void)
{
    /* Get the Capture 3 Register value */
    u16 tmpccr3 = 0;
    u8 tmpccr3l=0, tmpccr3h=0;

    tmpccr3h = TIM2->CCR3H;
    tmpccr3l = TIM2->CCR3L;

    tmpccr3 = (u16)(tmpccr3l);
    tmpccr3 |= (u16)((u16)tmpccr3h << 8);
    /* Get the Capture 3 Register value */
    return (u16)tmpccr3;
}

/**
  * @brief Gets the TIM2 Counter value.
  * @param[in] :
  * None
  * @retval Counter Register value.
  */
u16 TIM2_GetCounter(void)
{
    /* Get the Counter Register value */
    return (u16)(((u16)TIM2->CNTRH << 8) | (u16)(TIM2->CNTRL));
}


/**
  * @brief Gets the TIM2 Prescaler value.
  * @param[in] :
  * None
  * @retval Prescaler Register configuration value  @ref TIM2_Prescaler_TypeDef.
  */
TIM2_Prescaler_TypeDef TIM2_GetPrescaler(void)
{
    /* Get the Prescaler Register value */
    return (TIM2_Prescaler_TypeDef)(TIM2->PSCR);
}


/**
  * @brief Checks whether the specified TIM2 flag is set or not.
  * @param[in] TIM2_FLAG specifies the flag to check.
  * This parameter can be one of the following values:
  *                       - TIM2_FLAG_UPDATE: TIM2 update Flag
  *                       - TIM2_FLAG_CC1: TIM2 Capture Compare 1 Flag
  *                       - TIM2_FLAG_CC2: TIM2 Capture Compare 2 Flag
  *                       - TIM2_FLAG_CC3: TIM2 Capture Compare 3 Flag
  *                       - TIM2_FLAG_CC1OF: TIM2 Capture Compare 1 over capture Flag
  *                       - TIM2_FLAG_CC2OF: TIM2 Capture Compare 2 over capture Flag
  *                       - TIM2_FLAG_CC3OF: TIM2 Capture Compare 3 over capture Flag
  * @retval FlagStatus The new state of TIM2_FLAG (SET or RESET).
  */
FlagStatus TIM2_GetFlagStatus(TIM2_FLAG_TypeDef TIM2_FLAG)
{
    volatile FlagStatus bitstatus = RESET;
    vu8 tim2_flag_l, tim2_flag_h;

    /* Check the parameters */
    assert_param(IS_TIM2_GET_FLAG_OK(TIM2_FLAG));

    tim2_flag_l = (u8)(TIM2_FLAG);
    tim2_flag_h = (u8)(TIM2_FLAG >> 8);

    if (((TIM2->SR1 & tim2_flag_l) | (TIM2->SR2 & tim2_flag_h)) != (u8)RESET )
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return (FlagStatus)bitstatus;
}


/**
  * @brief Clears the TIM2’s pending flags.
  * @param[in] TIM2_FLAG specifies the flag to clear.
  * This parameter can be one of the following values:
  *                       - TIM2_FLAG_UPDATE: TIM2 update Flag
  *                       - TIM2_FLAG_CC1: TIM2 Capture Compare 1 Flag
  *                       - TIM2_FLAG_CC2: TIM2 Capture Compare 2 Flag
  *                       - TIM2_FLAG_CC3: TIM2 Capture Compare 3 Flag
  *                       - TIM2_FLAG_CC1OF: TIM2 Capture Compare 1 over capture Flag
  *                       - TIM2_FLAG_CC2OF: TIM2 Capture Compare 2 over capture Flag
  *                       - TIM2_FLAG_CC3OF: TIM2 Capture Compare 3 over capture Flag
  * @retval None.
  */
void TIM2_ClearFlag(TIM2_FLAG_TypeDef TIM2_FLAG)
{
    /* Check the parameters */
    assert_param(IS_TIM2_CLEAR_FLAG_OK(TIM2_FLAG));

    /* Clear the flags (rc_w0) clear this bit by writing 0. Writing ‘1’ has no effect*/
    TIM2->SR1 = (u8)(~((u8)(TIM2_FLAG)));
    TIM2->SR2 = (u8)(~((u8)(TIM2_FLAG >> 8)));
}


/**
  * @brief Checks whether the TIM2 interrupt has occurred or not.
  * @param[in] TIM2_IT specifies the TIM2 interrupt source to check.
  * This parameter can be one of the following values:
  *                       - TIM2_IT_UPDATE: TIM2 update Interrupt source
  *                       - TIM2_IT_CC1: TIM2 Capture Compare 1 Interrupt source
  *                       - TIM2_IT_CC2: TIM2 Capture Compare 2 Interrupt source
  *                       - TIM2_IT_CC3: TIM2 Capture Compare 3 Interrupt source
  * @retval ITStatus The new state of the TIM2_IT(SET or RESET).
  */

ITStatus TIM2_GetITStatus(TIM2_IT_TypeDef TIM2_IT)
{
    volatile ITStatus bitstatus = RESET;
    vu8 TIM2_itStatus = 0, TIM2_itEnable = 0;

    /* Check the parameters */
    assert_param(IS_TIM2_GET_IT_OK(TIM2_IT));

    TIM2_itStatus = (u8)(TIM2->SR1 & TIM2_IT);

    TIM2_itEnable = (u8)(TIM2->IER & TIM2_IT);

    if ((TIM2_itStatus != (u8)RESET ) && (TIM2_itEnable != (u8)RESET ))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return (ITStatus)(bitstatus);
}


/**
  * @brief Clears the TIM2's interrupt pending bits.
  * @param[in] TIM2_IT specifies the pending bit to clear.
  * This parameter can be one of the following values:
  *                       - TIM2_IT_UPDATE: TIM2 update Interrupt source
  *                       - TIM2_IT_CC1: TIM2 Capture Compare 1 Interrupt source
  *                       - TIM2_IT_CC2: TIM2 Capture Compare 2 Interrupt source
  *                       - TIM2_IT_CC3: TIM2 Capture Compare 3 Interrupt source
  * @retval None.
  */
void TIM2_ClearITPendingBit(TIM2_IT_TypeDef TIM2_IT)
{
    /* Check the parameters */
    assert_param(IS_TIM2_IT_OK(TIM2_IT));

    /* Clear the IT pending Bit */
    TIM2->SR1 = (u8)(~TIM2_IT);
}


/**
  * @brief Configure the TI1 as Input.
  * @param[in] TIM2_ICPolarity  The Input Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_ICPOLARITY_FALLING
  *                       - TIM2_ICPOLARITY_RISING
  * @param[in] TIM2_ICSelection specifies the input to be used.
  * This parameter can be one of the following values:
  *                       - TIM2_ICSELECTION_DIRECTTI: TIM2 Input 1 is selected to
  *                         be connected to IC1.
  *                       - TIM2_ICSELECTION_INDIRECTTI: TIM2 Input 1 is selected to
  *                         be connected to IC2.
  * @param[in] TIM2_ICFilter Specifies the Input Capture Filter.
  * This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI1_Config(u8 TIM2_ICPolarity,
                       u8 TIM2_ICSelection,
                       u8 TIM2_ICFilter)
{
    /* Disable the Channel 1: Reset the CCE Bit */
    TIM2->CCER1 &= (u8)(~TIM2_CCER1_CC1E);

    /* Select the Input and set the filter */
    TIM2->CCMR1  = (u8)((TIM2->CCMR1 & (u8)(~( TIM2_CCMR_CCxS     |        TIM2_CCMR_ICxF    ))) | (u8)(( (TIM2_ICSelection)) | ((u8)( TIM2_ICFilter << 4))));

    /* Select the Polarity */
    if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
    {
        TIM2->CCER1 |= TIM2_CCER1_CC1P;
    }
    else
    {
        TIM2->CCER1 &= (u8)(~TIM2_CCER1_CC1P);
    }
    /* Set the CCE Bit */
    TIM2->CCER1 |= TIM2_CCER1_CC1E;
}


/**
  * @brief Configure the TI2 as Input.
  * @param[in] TIM2_ICPolarity  The Input Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_ICPOLARITY_FALLING
  *                       - TIM2_ICPOLARITY_RISING
  * @param[in] TIM2_ICSelection specifies the input to be used.
  * This parameter can be one of the following values:
  *                       - TIM2_ICSELECTION_DIRECTTI: TIM2 Input 2 is selected to
  *                         be connected to IC2.
  *                       - TIM2_ICSELECTION_INDIRECTTI: TIM2 Input 2 is selected to
  *                         be connected to IC1.
  * @param[in] TIM2_ICFilter Specifies the Input Capture Filter.
  * This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI2_Config(u8 TIM2_ICPolarity,
                       u8 TIM2_ICSelection,
                       u8 TIM2_ICFilter)
{
    /* Disable the Channel 2: Reset the CCE Bit */
    TIM2->CCER1 &= (u8)(~TIM2_CCER1_CC2E);

    /* Select the Input and set the filter */
    TIM2->CCMR2 = (u8)((TIM2->CCMR2 & (u8)(~( TIM2_CCMR_CCxS     |        TIM2_CCMR_ICxF    ))) | (u8)(( (TIM2_ICSelection)) | ((u8)( TIM2_ICFilter << 4))));


    /* Select the Polarity */
    if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
    {
        TIM2->CCER1 |= TIM2_CCER1_CC2P;
    }
    else
    {
        TIM2->CCER1 &= (u8)(~TIM2_CCER1_CC2P);
    }

    /* Set the CCE Bit */
    TIM2->CCER1 |= TIM2_CCER1_CC2E;

}

/**
  * @brief Configure the TI3 as Input.
  * @param[in] TIM2_ICPolarity  The Input Polarity.
  * This parameter can be one of the following values:
  *                       - TIM2_ICPOLARITY_FALLING
  *                       - TIM2_ICPOLARITY_RISING
  * @param[in] TIM2_ICSelection specifies the input to be used.
  * This parameter can be one of the following values:
  *                       - TIM2_ICSELECTION_DIRECTTI: TIM2 Input 3 is selected to
  *                         be connected to IC3.
  * @param[in] TIM2_ICFilter Specifies the Input Capture Filter.
  * This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI3_Config(u8 TIM2_ICPolarity, u8 TIM2_ICSelection,
                       u8 TIM2_ICFilter)
{
    /* Disable the Channel 3: Reset the CCE Bit */
    TIM2->CCER2 &=  (u8)(~TIM2_CCER2_CC3E);

    /* Select the Input and set the filter */
    TIM2->CCMR3 = (u8)((TIM2->CCMR3 & (u8)(~( TIM2_CCMR_CCxS     |        TIM2_CCMR_ICxF    ))) | (u8)(( (TIM2_ICSelection)) | ((u8)( TIM2_ICFilter << 4))));


    /* Select the Polarity */
    if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
    {
        TIM2->CCER2 |= TIM2_CCER2_CC3P;
    }
    else
    {
        TIM2->CCER2 &= (u8)(~TIM2_CCER2_CC3P);
    }
    /* Set the CCE Bit */
    TIM2->CCER2 |= TIM2_CCER2_CC3E;
}
/**
  * @}
  */
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
