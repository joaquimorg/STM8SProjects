/**
  ********************************************************************************
  * @file stm8s_wwdg.c
  * @brief This file contains all the functions for the WWDG peripheral.
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
#include "stm8s_wwdg.h"


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/** @addtogroup WWDG_Public_Functions
  * @{
  */

/**
  * @brief Initialization of the WWDG peripheral.
  *  This function set Window Register = WindowValue,
  *  Control Register according to ControlValue and \b ENABLE \b WDG
  * @par Full description:
  * @param[in] Counter WWDG specifies the counter value
  * @param[in] WindowValue specifies the WWDG Window Register, range is 0x00 to 0x7F.
  */
void WWDG_Init(u8 Counter, u8 WindowValue)
{
    /* Check the parameters */
    assert_param(IS_WWDG_WINDOWLIMITVALUE_OK(WindowValue));
    WWDG->WR = WWDG_WR_RESET_VALUE;
    WWDG->CR = (u8)(WWDG_CR_WDGA | WWDG_CR_T6 | Counter);
    WWDG->WR = (u8)((u8)(~WWDG_CR_WDGA) & (u8)(WWDG_CR_T6 | WindowValue));
}

/**
  * @brief Refresh of the WWDG peripheral.
  * This function has no effect out of Refresh Window
  * @par Full description:
  * @param[in] Counter WWDG Counter Value
  * @par Required preconditions:
  * Required preconditions: \b Counter < \b Window \b Limit \n
  */
void WWDG_SetCounter(u8 Counter)
{

    /* Check the parameters */
    assert_param(IS_WWDG_COUNTERVALUE_OK(Counter));

    if ((WWDG->CR & (u8)(~WWDG_CR_WDGA)) < (WWDG->WR))
    {
        WWDG->CR = (u8)(WWDG_CR_WDGA | WWDG_CR_T6 | Counter);
    }

}

/**
  * @brief Read of the WWDG Counter Value.
  * This value could be used to check if WWDG is in the window, where refresh is allowed.
  * @par Full description:
  * None
  * @retval
  * u8 Control Register
  */
u8 WWDG_GetCounter(void)
{
    return(WWDG->CR);
}

/**
  * @brief Generates immediate SW RESET, using the WWDG peripheral.
  * @par Full description:
  * None
  */
void WWDG_SWReset(void)
{
    WWDG->CR = WWDG_CR_WDGA; /* Activate WWDG, with clearing T6 */
}

/**
  * @brief Sets the WWDG window value.
  * @par Full description:
  * @param[in] WindowValue: specifies the window value to be compared to the downcounter.
  * @par Required preconditions:
  * Required preconditions: This parameter value must be \b lower than \b 0x80.\n
  */
void WWDG_SetWindowValue(u8 WindowValue)
{
    /* Check the parameters */
    assert_param(IS_WWDG_WINDOWLIMITVALUE_OK(WindowValue));
    WWDG->WR = (u8)((u8)(~WWDG_CR_WDGA) & (u8)(WWDG_CR_T6 | WindowValue));
}
/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
