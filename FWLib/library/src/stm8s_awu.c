/**
  ******************************************************************************
  * @file stm8s_awu.c
  * @brief This file contains all the functions for the AWU peripheral.
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
#include "stm8s_awu.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* See also AWU_Timebase_TypeDef structure in stm8s_awu.h file :
                          N   2   5   1   2   4   8   1   3   6   1   2   5   1   2   1   3
                          O   5   0   m   m   m   m   6   2   4   2   5   1   s   s   2   0
                          I   0   0   s   s   s   s   m   m   m   8   6   2           s   s
                          T   u   u                   s   s   s   m   m   m
                              s   s                               s   s   s
*/
/** Contains the different values to write in the APR register (used by AWU_Init function) */
const u8 APR_Array[17] =
    {
        0, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 61, 23, 23, 62
    };

/** Contains the different values to write in the TBR register (used by AWU_Init function) */
const u8 TBR_Array[17] =
    {
        0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 12, 14, 15, 15
    };

/* Public functions ----------------------------------------------------------*/

/**
  * @addtogroup AWU_Public_Functions
  * @{
  */

/**
  * @brief Deinitializes the AWU peripheral registers to their default reset
  * values.
  * @par Parameters:
  * None
  * @retval None
  */
void AWU_DeInit(void)
{
    AWU->CSR = AWU_CSR_RESET_VALUE;
    AWU->APR = AWU_APR_RESET_VALUE;
    AWU->TBR = AWU_TBR_RESET_VALUE;
}

/**
  * @brief Initializes the AWU peripheral according to the specified parameters.
  * @param[in] AWU_TimeBase : Time base selection (interval between AWU interrupts).
  * can be one of the values of @ref AWU_Timebase_TypeDef.
  * @retval None
  * @par Required preconditions:
  * The LS RC calibration must be performed before calling this function.
  */
void AWU_Init(AWU_Timebase_TypeDef AWU_TimeBase)
{

    /* Check parameter */
    assert_param(IS_AWU_TIMEBASE_OK(AWU_TimeBase));

    /* Enable the AWU peripheral */
    AWU->CSR |= AWU_CSR_AWUEN;

    /* Set the TimeBase */
    AWU->TBR &= (u8)(~AWU_TBR_AWUTB);
    AWU->TBR |= TBR_Array[(u8)AWU_TimeBase];

    /* Set the APR divider */
    AWU->APR &= (u8)(~AWU_APR_APR);
    AWU->APR |= APR_Array[(u8)AWU_TimeBase];

}

/**
  * @brief Enable or disable the AWU peripheral.
  * @param[in] NewState Indicates the new state of the AWU peripheral.
  * @retval None
  * @par Required preconditions:
  * Initialisation of AWU and LS RC calibration must be done before.
  */
void AWU_Cmd(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        /* Enable the AWU peripheral */
        AWU->CSR |= AWU_CSR_AWUEN;
    }
    else
    {
        /* Disable the AWU peripheral */
        AWU->CSR &= (u8)(~AWU_CSR_AWUEN);
    }
}

/**
  * @brief Update APR register with the measured LSI frequency.
  * @par Note on the APR calculation:
  * A is the integer part of lsifreqkhz/4 and x the decimal part.
  * x <= A/(1+2A) is equivalent to A >= x(1+2A) and also to 4A >= 4x(1+2A) [F1]
  * but we know that A + x = lsifreqkhz/4 ==> 4x = lsifreqkhz-4A
  * so [F1] can be written :
  * 4A >= (lsifreqkhz-4A)(1+2A)
  * @param[in] LSIFreqHz Low Speed RC frequency measured by timer (in Hz).
  * @retval None
  * @par Required preconditions:
  * - AWU must be disabled to avoid unwanted interrupts.
  */
void AWU_LSICalibrationConfig(u32 LSIFreqHz)
{

    u16 lsifreqkhz = 0x0;
    u16 A = 0x0;

    /* Check parameter */
    assert_param(IS_LSI_FREQUENCY_OK(LSIFreqHz));

    lsifreqkhz = (u16)(LSIFreqHz / 1000); /* Converts value in kHz */

    /* Calculation of AWU calibration value */

    A = (u16)(lsifreqkhz >> 2U); /* Division by 4, keep integer part only */

    if ((4U * A) >= ((lsifreqkhz - (4U * A)) * (1U + (2U * A))))
    {
        AWU->APR = (u8)(A - 2U);
    }
    else
    {
        AWU->APR = (u8)(A - 1U);
    }

    /* Set the MR bit to load the new value to the prescalers */
    AWU->CSR |= AWU_CSR_MR;

}

/**
  * @brief Configures AWU in Idle mode to reduce power consumption.
  * @par Parameters:
  * None
  * @retval None
  */
void AWU_IdleModeEnable(void)
{

    /* Disable AWU peripheral */
    AWU->CSR &= (u8)(~AWU_CSR_AWUEN);

    /* No AWU timebase */
    AWU->TBR = (u8)(~AWU_TBR_AWUTB);

}

/**
  * @brief Reinitializes AWU counter.
  * @par Parameters:
  * None
  * @retval None
  */
void AWU_ReInitCounter(void)
{
    AWU->CSR |= AWU_CSR_MR;
}

/**
  * @brief Returns status of the AWU peripheral flag.
  * @par Parameters:
  * None
  * @retval FlagStatus : Status of the AWU flag.
  * This parameter can be any of the @ref FlagStatus enumeration.
  */
FlagStatus AWU_GetFlagStatus(void)
{
    return((FlagStatus)(((u8)(AWU->CSR & AWU_CSR_AWUF) == (u8)0x00) ? RESET : SET));
}


/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
