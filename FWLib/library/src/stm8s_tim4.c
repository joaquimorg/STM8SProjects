/**
  ******************************************************************************
  * @file stm8s_tim4.c
  * @brief This file contains all the functions for the TIM4 peripheral.
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
#include "stm8s_tim4.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/**
  * @addtogroup TIM4_Public_Functions
  * @{
  */

/**
  * @brief Deinitializes the TIM4 peripheral registers to their default reset values.
  * @param[in] :
  * None
  * @retval None
  */
void TIM4_DeInit(void)
{
    TIM4->CR1 = TIM4_CR1_RESET_VALUE;
    TIM4->IER = TIM4_IER_RESET_VALUE;
    TIM4->CNTR = TIM4_CNTR_RESET_VALUE;
    TIM4->PSCR = TIM4_PSCR_RESET_VALUE;
    TIM4->ARR = TIM4_ARR_RESET_VALUE;
    TIM4->SR1 = TIM4_SR1_RESET_VALUE;
}

/**
  * @brief Initializes the TIM4 Time Base Unit according to the specified parameters.
  * @param[in]  TIM4_Prescaler specifies the Prescaler from TIM4_Prescaler_TypeDef.
  * @param[in]  TIM4_Period specifies the Period value.
  * @retval None
  */
void TIM4_TimeBaseInit(TIM4_Prescaler_TypeDef TIM4_Prescaler, u8 TIM4_Period)
{
    /* Check TIM4 prescaler value */
    assert_param(IS_TIM4_PRESCALER_OK(TIM4_Prescaler));
    /* Set the Prescaler value */
    TIM4->PSCR = (u8)(TIM4_Prescaler);
    /* Set the Autoreload value */
    TIM4->ARR = (u8)(TIM4_Period);
}



/**
  * @brief Enables or disables the TIM4 peripheral.
  * @param[in] NewState new state of the TIM4 peripheral. This parameter can
  * be ENABLE or DISABLE.
  * @retval None
  */
void TIM4_Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* set or Reset the CEN Bit */
    if (NewState != DISABLE)
    {
        TIM4->CR1 |= TIM4_CR1_CEN;
    }
    else
    {
        TIM4->CR1 &= (u8)(~TIM4_CR1_CEN);
    }
}

/**
  * @brief Enables or disables the specified TIM4 interrupts.
  * @param[in] NewState new state of the TIM4 peripheral.
  * This parameter can be: ENABLE or DISABLE.
  * @param[in] TIM4_IT specifies the TIM4 interrupts sources to be enabled or disabled.
  * This parameter can be any combination of the following values:
  * - TIM4_IT_UPDATE: TIM4 update Interrupt source
  * @param[in] NewState new state of the TIM4 peripheral.
  * @retval None
  */
void TIM4_ITConfig(TIM4_IT_TypeDef TIM4_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM4_IT_OK(TIM4_IT));
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the Interrupt sources */
        TIM4->IER |= (u8)TIM4_IT;
    }
    else
    {
        /* Disable the Interrupt sources */
        TIM4->IER &= (u8)(~TIM4_IT);
    }
}

/**
  * @brief Enables or Disables the TIM4 Update event.
  * @param[in] NewState new state of the TIM4 peripheral Preload register. This parameter can
  * be ENABLE or DISABLE.
  * @retval None
  */
void TIM4_UpdateDisableConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* Set or Reset the UDIS Bit */
    if (NewState != DISABLE)
    {
        TIM4->CR1 |= TIM4_CR1_UDIS;
    }
    else
    {
        TIM4->CR1 &= (u8)(~TIM4_CR1_UDIS);
    }
}

/**
  * @brief Selects the TIM4 Update Request Interrupt source.
  * @param[in] TIM4_UpdateSource specifies the Update source.
  * This parameter can be one of the following values
  *                       - TIM4_UPDATESOURCE_REGULAR
  *                       - TIM4_UPDATESOURCE_GLOBAL
  * @retval None
  */
void TIM4_UpdateRequestConfig(TIM4_UpdateSource_TypeDef TIM4_UpdateSource)
{
    /* Check the parameters */
    assert_param(IS_TIM4_UPDATE_SOURCE_OK(TIM4_UpdateSource));

    /* Set or Reset the URS Bit */
    if (TIM4_UpdateSource != TIM4_UPDATESOURCE_GLOBAL)
    {
        TIM4->CR1 |= TIM4_CR1_URS;
    }
    else
    {
        TIM4->CR1 &= (u8)(~TIM4_CR1_URS);
    }
}

/**
  * @brief Selects the TIM4’s One Pulse Mode.
  * @param[in] TIM4_OPMode specifies the OPM Mode to be used.
  * This parameter can be one of the following values
  *                    - TIM4_OPMODE_SINGLE
  *                    - TIM4_OPMODE_REPETITIVE
  * @retval None
  */
void TIM4_SelectOnePulseMode(TIM4_OPMode_TypeDef TIM4_OPMode)
{
    /* Check the parameters */
    assert_param(IS_TIM4_OPM_MODE_OK(TIM4_OPMode));

    /* Set or Reset the OPM Bit */
    if (TIM4_OPMode != TIM4_OPMODE_REPETITIVE)
    {
        TIM4->CR1 |= TIM4_CR1_OPM;
    }
    else
    {
        TIM4->CR1 &= (u8)(~TIM4_CR1_OPM);
    }

}

/**
  * @brief Configures the TIM4 Prescaler.
  * @param[in] Prescaler specifies the Prescaler Register value
  * This parameter can be one of the following values
  *                       -  TIM4_PRESCALER_1
  *                       -  TIM4_PRESCALER_2
  *                       -  TIM4_PRESCALER_4
  *                       -  TIM4_PRESCALER_8
  *                       -  TIM4_PRESCALER_16
  *                       -  TIM4_PRESCALER_32
  *                       -  TIM4_PRESCALER_64
  *                       -  TIM4_PRESCALER_128
  * @param[in] TIM4_PSCReloadMode specifies the TIM4 Prescaler Reload mode.
  * This parameter can be one of the following values
  *                       - TIM4_PSCRELOADMODE_IMMEDIATE: The Prescaler is loaded
  *                         immediatly.
  *                       - TIM4_PSCRELOADMODE_UPDATE: The Prescaler is loaded at
  *                         the update event.
  * @retval None
  */
void TIM4_PrescalerConfig(TIM4_Prescaler_TypeDef Prescaler, TIM4_PSCReloadMode_TypeDef TIM4_PSCReloadMode)
{
    /* Check the parameters */
    assert_param(IS_TIM4_PRESCALER_RELOAD_OK(TIM4_PSCReloadMode));
    assert_param(IS_TIM4_PRESCALER_OK(Prescaler));

    /* Set the Prescaler value */
    TIM4->PSCR = (u8)Prescaler;

    /* Set or reset the UG Bit */
    TIM4->EGR = (u8)TIM4_PSCReloadMode;
}

/**
  * @brief Enables or disables TIM4 peripheral Preload register on ARR.
  * @param[in] NewState new state of the TIM4 peripheral Preload register.
  * This parameter can be ENABLE or DISABLE.
  * @retval None
  */
void TIM4_ARRPreloadConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* Set or Reset the ARPE Bit */
    if (NewState != DISABLE)
    {
        TIM4->CR1 |= TIM4_CR1_ARPE;
    }
    else
    {
        TIM4->CR1 &= (u8)(~TIM4_CR1_ARPE);
    }
}

/**
  * @brief Configures the TIM4 event to be generated by software.
  * @param[in] TIM4_EventSource specifies the event source.
  * This parameter can be one of the following values:
  *                       - TIM4_EVENTSOURCE_UPDATE: TIM4 update Event source
  * @retval None
  */
void TIM4_GenerateEvent(TIM4_EventSource_TypeDef TIM4_EventSource)
{
    /* Check the parameters */
    assert_param(IS_TIM4_EVENT_SOURCE_OK(TIM4_EventSource));

    /* Set the event sources */
    TIM4->EGR = (u8)(TIM4_EventSource);
}


/**
  * @brief Sets the TIM4 Counter Register value.
  * @param[in] Counter specifies the Counter register new value.
  * This parameter is between 0x00 and 0xFF.
  * @retval None
  */
void TIM4_SetCounter(u8 Counter)
{
    /* Set the Counter Register value */
    TIM4->CNTR = (u8)(Counter);
}


/**
  * @brief Sets the TIM4 Autoreload Register value.
  * @param[in] Autoreload specifies the Autoreload register new value.
  * This parameter is between 0x00 and 0xFF.
  * @retval None
  */
void TIM4_SetAutoreload(u8 Autoreload)
{
    /* Set the Autoreload Register value */
    TIM4->ARR = (u8)(Autoreload);
}

/**
  * @brief Gets the TIM4 Counter value.
  * @param[in] :
  * None
  * @retval Counter Register value.
  */
u8 TIM4_GetCounter(void)
{
    /* Get the Counter Register value */
    return (u8)(TIM4->CNTR);
}

/**
  * @brief Gets the TIM4 Prescaler value.
  * @param[in] :
  * None
  * @retval Prescaler Register configuration value.
  */
TIM4_Prescaler_TypeDef TIM4_GetPrescaler(void)
{
    /* Get the Prescaler Register value */
    return (TIM4_Prescaler_TypeDef)(TIM4->PSCR);
}

/**
  * @brief Checks whether the specified TIM4 flag is set or not.
  * @param[in] TIM4_FLAG specifies the flag to check.
  * This parameter can be one of the following values:
  *                       - TIM4_FLAG_UPDATE: TIM4 update Flag
  * @retval FlagStatus The new state of TIM4_FLAG (SET or RESET).
  */
FlagStatus TIM4_GetFlagStatus(TIM4_FLAG_TypeDef TIM4_FLAG)
{
    /* Check the parameters */
    assert_param(IS_TIM4_GET_FLAG_OK(TIM4_FLAG));

    if ((TIM4->SR1 & TIM4_FLAG) != RESET )
    {
        return (FlagStatus)(SET);
    }
    else
    {
        return (FlagStatus)(RESET);
    }
}

/**
  * @brief Clears the TIM4’s pending flags.
  * @param[in] TIM4_FLAG specifies the flag to clear.
  * This parameter can be one of the following values:
  *                       - TIM4_FLAG_UPDATE: TIM4 update Flag
  * @retval None.
  */
void TIM4_ClearFlag(TIM4_FLAG_TypeDef TIM4_FLAG)
{
    /* Check the parameters */
    assert_param(IS_TIM4_GET_FLAG_OK(TIM4_FLAG));

    /* Clear the flags (rc_w0) clear this bit by writing 0. Writing ‘1’ has no effect*/
    TIM4->SR1 = (u8)(~TIM4_FLAG);

}
/**
  * @brief Checks whether the TIM4 interrupt has occurred or not.
  * @param[in] TIM4_IT specifies the TIM4 interrupt source to check.
  * This parameter can be one of the following values:
  *                       - TIM4_IT_UPDATE: TIM4 update Interrupt source
  * @retval ITStatus The new state of the TIM4_IT (SET or RESET).
  */

ITStatus TIM4_GetITStatus(TIM4_IT_TypeDef TIM4_IT)
{
    /* Check the parameters */
    assert_param(IS_TIM4_IT_OK(TIM4_IT));

    if (((TIM4->SR1 & TIM4_IT) != RESET ) && ((TIM4->IER & TIM4_IT) != RESET ))
    {
        return (ITStatus)(SET);
    }
    else
    {
        return (ITStatus)(RESET);
    }
}

/**
  * @brief Clears the TIM4's interrupt pending bits.
  * @param[in] TIM4_IT specifies the pending bit to clear.
  * This parameter can be one of the following values:
  *                       - TIM4_IT_UPDATE: TIM4 update Interrupt source
  * @retval None.
  */
void TIM4_ClearITPendingBit(TIM4_IT_TypeDef TIM4_IT)
{
    /* Check the parameters */
    assert_param(IS_TIM4_IT_OK(TIM4_IT));

    /* Clear the IT pending Bit */
    TIM4->SR1 = (u8)(~TIM4_IT);
}

/**
  * @}
  */
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
