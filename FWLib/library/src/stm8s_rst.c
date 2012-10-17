/**
  ******************************************************************************
  * @file stm8s_rst.c
  * @brief This file contains all the functions for the RST peripheral.
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

#include "stm8s_rst.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
  * @addtogroup RST_Public_Functions
  * @{
  */


/**
  * @brief Checks whether the specified RST flag is set or not.
  * @param[in] RST_Flag : specify the reset flag to check.
  * can be one of the values of @ref RST_Flag_TypeDef.
  * @retval FlagStatus: status of the given RST flag.
  */
FlagStatus RST_GetFlagStatus(RST_Flag_TypeDef RST_Flag)
{
    /* Check the parameters */
    assert_param(IS_RST_FLAG_OK(RST_Flag));

    /* Get flag status */

    return ((FlagStatus)((u8)RST->SR & (u8)RST_Flag));
}

/**
  * @brief Clears the specified RST flag.
  * @param[in] RST_Flag : specify the reset flag to clear.
  * can be one of the values of @ref RST_Flag_TypeDef.
  * @retval None
  */
void RST_ClearFlag(RST_Flag_TypeDef RST_Flag)
{
    /* Check the parameters */
    assert_param(IS_RST_FLAG_OK(RST_Flag));

    RST->SR = (u8)RST_Flag;
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
