/**
  ******************************************************************************
  * @file stm8s_i2c.c
  * @brief This file contains all the functions for the I2C peripheral.
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
#include "stm8s_i2c.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/**
  * @addtogroup I2C_Public_Functions
  * @{
  */

/**
  * @brief Deinitializes the I2C peripheral registers to their default reset values.
  * @par Parameters:
  * None
  * @retval None
  */
void I2C_DeInit(void)
{
  I2C->CR1 = I2C_CR1_RESET_VALUE;
  I2C->CR2 = I2C_CR2_RESET_VALUE;
  I2C->FREQR = I2C_FREQR_RESET_VALUE;
  I2C->OARL = I2C_OARL_RESET_VALUE;
  I2C->OARH = I2C_OARH_RESET_VALUE;
  I2C->ITR = I2C_ITR_RESET_VALUE;
  I2C->CCRL = I2C_CCRL_RESET_VALUE;
  I2C->CCRH = I2C_CCRH_RESET_VALUE;
  I2C->TRISER = I2C_TRISER_RESET_VALUE;
}

/**
  * @brief Initializes the I2C according to the specified parameters in standard or fast mode.
  * @param[in] OutputClockFrequencyHz : Specifies the output clock frequency in Hz.
  * @param[in] OwnAddress : Specifies the own address.
  * @param[in] DutyCycle : Specifies the duty cycle to apply.
  * This parameter can be any of the  @ref I2C_DutyCycle_TypeDef enumeration.
  * @param[in] Ack : Specifies the acknowledge mode to apply.
  * This parameter can be any of the  @ref I2C_Ack_TypeDef enumeration.
  * @param[in] AddMode : Specifies the addressing mode to apply.
  * This parameter can be any of the  @ref I2C_AddMode_TypeDef enumeration.
  * @param[in] InputClockFrequencyMHz : Specifies the input clock frequency in MHz.
  * @retval None
  */
void I2C_Init(u32 OutputClockFrequencyHz, u16 OwnAddress, I2C_DutyCycle_TypeDef DutyCycle, I2C_Ack_TypeDef Ack, I2C_AddMode_TypeDef AddMode, u8 InputClockFrequencyMHz )
{
  u16 result = 0x0004;
  u16 tmpval = 0;
  u8 tmpccrh = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ACK_OK(Ack));
  assert_param(IS_I2C_ADDMODE_OK(AddMode));
  assert_param(IS_I2C_OWN_ADDRESS_OK(OwnAddress));
  assert_param(IS_I2C_INPUT_CLOCK_FREQ_OK(InputClockFrequencyMHz));
  assert_param(IS_I2C_OUTPUT_CLOCK_FREQ_OK(OutputClockFrequencyHz));


  /*------------------------- I2C FREQ Configuration ------------------------*/
  /* Clear frequency bits */
  I2C->FREQR &= (u8)(~I2C_FREQR_FREQ);
  /* Write new value */
  I2C->FREQR |= InputClockFrequencyMHz;

  /*--------------------------- I2C CCR Configuration ------------------------*/
  /* Disable I2C to configure TRISER */
  I2C->CR1 &= (u8)(~I2C_CR1_PE);

  /* Clear CCRH & CCRL */
  I2C->CCRH &= (u8)(~(I2C_CCRH_FS | I2C_CCRH_DUTY | I2C_CCRH_CCR));
  I2C->CCRL &= (u8)(~I2C_CCRL_CCR);

  /* Detect Fast or Standard mode depending on the Output clock frequency selected */
  if (OutputClockFrequencyHz > I2C_MAX_STANDARD_FREQ) /* FAST MODE */
  {
    /* Set F/S bit for fast mode */
    tmpccrh = I2C_CCRH_FS;

    if (DutyCycle == I2C_DUTYCYCLE_2)
    {
      /* Fast mode speed calculate: Tlow/Thigh = 2 */
      result = (u16) ((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz * 3));
    }
    else /* I2C_DUTYCYCLE_16_9 */
    {
      /* Fast mode speed calculate: Tlow/Thigh = 16/9 */
      result = (u16) ((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz * 25));
      /* Set DUTY bit */
      tmpccrh |= I2C_CCRH_DUTY;
    }

    /* Verify and correct CCR value if below minimum value */
    if (result < (u16)0x01)
    {
      /* Set the minimum allowed value */
      result = (u16)0x0001;
    }

    /* Set Maximum Rise Time: 300ns max in Fast Mode
    = [300ns/(1/InputClockFrequencyMHz.10e6)]+1
    = [(InputClockFrequencyMHz * 3)/10]+1 */
    tmpval = ((InputClockFrequencyMHz * 3) / 10) + 1;
    I2C->TRISER = (u8)tmpval;

  }
  else /* STANDARD MODE */
  {

    /* Calculate standard mode speed */
    result = (u16)((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz << (u8)1));

    /* Verify and correct CCR value if below minimum value */
    if (result < (u16)0x0004)
    {
      /* Set the minimum allowed value */
      result = (u16)0x0004;
    }

    /* Set Maximum Rise Time: 1000ns max in Standard Mode
    = [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
    = InputClockFrequencyMHz+1 */
    I2C->TRISER = (u8)(InputClockFrequencyMHz + 1);

  }

  /* Write CCR with new calculated value */
  I2C->CCRL = (u8)result;
  I2C->CCRH = (u8)(((u8)(result >> 8) & I2C_CCRH_CCR) | tmpccrh);

  /* Enable I2C */
  I2C->CR1 |= I2C_CR1_PE;

  /* Configure I2C acknowledgement */
  I2C_AcknowledgeConfig(Ack);

  /*--------------------------- I2C OAR Configuration ------------------------*/
  I2C->OARL = (u8)(OwnAddress);
  I2C->OARH = (u8)((u8)AddMode |
                   I2C_OARH_ADDCONF |
                   (u8)((OwnAddress & (u16)0x0300) >> (u8)7));
}

/**
  * @brief Enables or disables the I2C peripheral.
  * @param[in] NewState : Indicate the new I2C peripheral state.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_Cmd(FunctionalState NewState)
{

  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Enable I2C peripheral */
    I2C->CR1 |= I2C_CR1_PE;
  }
  else /* NewState == DISABLE */
  {
    /* Disable I2C peripheral */
    I2C->CR1 &= (u8)(~I2C_CR1_PE);
  }
}

/**
  * @brief Enables or disables the I2C General Call feature.
  * @param[in] NewState : State of the General Call feature.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_GeneralCallCmd(FunctionalState NewState)
{

  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Enable General Call */
    I2C->CR1 |= I2C_CR1_ENGC;
  }
  else /* NewState == DISABLE */
  {
    /* Disable General Call */
    I2C->CR1 &= (u8)(~I2C_CR1_ENGC);
  }
}

/**
  * @brief Generates I2C communication START condition.
  * @param[in] NewState : Enable or disable the start condition.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  * @par Required preconditions:
  * CCR must be programmed i.e. I2C_Init function must have been called with a valid I2C_ClockSpeed
  */
void I2C_GenerateSTART(FunctionalState NewState)
{

  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Generate a START condition */
    I2C->CR2 |= I2C_CR2_START;
  }
  else /* NewState == DISABLE */
  {
    /* Disable the START condition generation */
    I2C->CR2 &= (u8)(~I2C_CR2_START);
  }
}

/**
  * @brief Generates I2C communication STOP condition.
  * @param[in] NewState : Enable or disable the stop condition.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_GenerateSTOP(FunctionalState NewState)
{

  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Generate a STOP condition */
    I2C->CR2 |= I2C_CR2_STOP;
  }
  else /* NewState == DISABLE */
  {
    /* Disable the STOP condition generation */
    I2C->CR2 &= (u8)(~I2C_CR2_STOP);
  }
}

/**
  * @brief Enables or disables I2C software reset.
  * @param[in] NewState : Specifies the new state of the I2C software reset.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_SoftwareResetCmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Peripheral under reset */
    I2C->CR2 |= I2C_CR2_SWRST;
  }
  else /* NewState == DISABLE */
  {
    /* Peripheral not under reset */
    I2C->CR2 &= (u8)(~I2C_CR2_SWRST);
  }
}

/**
  * @brief Enables or disables the I2C clock stretching.
  * @param[in] NewState : Specifies the new state of the I2C Clock stretching.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */

void I2C_StretchClockCmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Clock Stretching Enable */
    I2C->CR1 &= (u8)(~I2C_CR1_NOSTRETCH);

  }
  else /* NewState == DISABLE */
  {
    /* Clock Stretching Disable (Slave mode) */
    I2C->CR1 |= I2C_CR1_NOSTRETCH;
  }
}

/**
  * @brief Enable or Disable the I2C acknowledge and position acknowledge feature.
  * @param[in] Ack : Specifies the acknowledge mode to apply.
  * This parameter can be any of the  @ref I2C_Ack_TypeDef enumeration.
  * @retval None
  * @par Required preconditions:
  * This function must be called before data reception starts.
  */
void I2C_AcknowledgeConfig(I2C_Ack_TypeDef Ack)
{

  /* Check function parameters */
  assert_param(IS_I2C_ACK_OK(Ack));

  if (Ack == I2C_ACK_NONE)
  {
    /* Disable the acknowledgement */
    I2C->CR2 &= (u8)(~I2C_CR2_ACK);
  }
  else
  {
    /* Enable the acknowledgement */
    I2C->CR2 |= I2C_CR2_ACK;

    if (Ack == I2C_ACK_CURR)
    {
      /* Configure (N)ACK on current byte */
      I2C->CR2 &= (u8)(~I2C_CR2_POS);
    }
    else
    {
      /* Configure (N)ACK on next byte */
      I2C->CR2 |= I2C_CR2_POS;
    }
  }

}

/**
  * @brief Enables or disables the specified I2C interrupt.
  * @param[in] ITName : Name of the interrupt to enable or disable.
  * This parameter can be any of the  @ref I2C_IT_TypeDef enumeration.
  * @param[in] NewState : State of the interrupt to apply.
  * This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_ITConfig(I2C_IT_TypeDef ITName, FunctionalState NewState)
{

  /* Check functions parameters */
  assert_param(IS_I2C_INTERRUPT_OK(ITName));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C interrupts */
    I2C->ITR |= (u8)ITName;
  }
  else /* NewState == DISABLE */
  {
    /* Disable the selected I2C interrupts */
    I2C->ITR &= (u8)(~(u8)ITName);
  }
}

/**
  * @brief Selects the specified I2C fast mode duty cycle.
  * @param[in] DutyCycle : Specifies the duty cycle to apply.
  * This parameter can be any of the @ref I2C_DutyCycle_TypeDef enumeration.
  * @retval None
  */
void I2C_FastModeDutyCycleConfig(I2C_DutyCycle_TypeDef DutyCycle)
{

  /* Check function parameters */
  assert_param(IS_I2C_DUTYCYCLE_OK(DutyCycle));

  if (DutyCycle == I2C_DUTYCYCLE_16_9)
  {
    /* I2C fast mode Tlow/Thigh = 16/9 */
    I2C->CCRH |= I2C_CCRH_DUTY;
  }
  else /* I2C_DUTYCYCLE_2 */
  {
    /* I2C fast mode Tlow/Thigh = 2 */
    I2C->CCRH &= (u8)(~I2C_CCRH_DUTY);
  }
}

/**
  * @brief Checks whether the last I2C event is equal to the one passed as parameter.
  * @param[in] I2C_Event : Specifies the event to be checked.
  * This parameter can be any of the  @ref I2C_Event_TypeDef enumeration.
  * @retval ErrorStatus : Status of the event
  * SUCCESS : last event is equal to the I2C_Event
  * ERROR : last event is different from the I2C_Event
  * @par Required preconditions:
  * This function must be called only once the flags can be reset by reading the registers.
  */
ErrorStatus I2C_CheckEvent(I2C_Event_TypeDef I2C_Event)
{

  u8 flag1 = 0;
  u8 flag2 = 0;
  ErrorStatus status = ERROR;

  /* Check the parameters */
  assert_param(IS_I2C_EVENT_OK(I2C_Event));

  flag1 = I2C->SR1;
  flag2 = I2C->SR2;

  /* Check which SRx register must be read */
  if (((u16)I2C_Event & (u16)0x0F00) == 0x0700)
  {
    /* Check whether the last event is equal to I2C_EVENT */
    if (flag1 & (u8)I2C_Event)
    {
      /* SUCCESS: last event is equal to I2C_EVENT */
      status = SUCCESS;
    }
    else
    {
      /* ERROR: last event is different from I2C_EVENT */
      status = ERROR;
    }
  }
  else /* Returns whether the status register to check is SR2 */
  {
    if (flag2 & (u8)I2C_Event)
    {
      /* SUCCESS: last event is equal to I2C_EVENT */
      status = SUCCESS;
    }
    else
    {
      /* ERROR: last event is different from I2C_EVENT */
      status = ERROR;
    }
  }

  /* Return status */
  return status;

}

/**
  * @brief Returns the most recent received data.
  * @par Parameters:
  * None
  * @retval u8 : The value of the received byte data.
  */
u8 I2C_ReceiveData(void)
{
  /* Return the data present in the DR register */
  return ((u8)I2C->DR);
}

/**
  * @brief Transmits the 7-bit address (to select the) slave device.
  * @param[in] Address : Specifies the slave address which will be transmitted.
  * @param[in] Direction : Specifies whether the I2C device will be a Transmitter or a Receiver.
  * This parameter can be any of the @ref I2C_Direction_TypeDef enumeration.
  * @retval None
  */
void I2C_Send7bitAddress(u8 Address, I2C_Direction_TypeDef Direction)
{
  /* Check function parameters */
  assert_param(IS_I2C_ADDRESS_OK(Address));
  assert_param(IS_I2C_DIRECTION_OK(Direction));

  /* Clear bit0 (direction) just in case */
  Address &= (u8)0xFE;

  /* Send the Address + Direction */
  I2C->DR = (u8)(Address | (u8)Direction);
}

/**
  * @brief Send a byte by writing in the DR register.
  * @param[in] Data : Byte to be sent.
  * @retval None
  */
void I2C_SendData(u8 Data)
{
  /* Write in the DR register the data to be sent */
  I2C->DR = Data;
}

/**
  * @brief Returns the specified I2C flag status
  * @param[in] Flag : Specifies the flag to read
  * This parameter can be any of the  @ref I2C_Flag_TypeDef enumeration.
  * @retval FlagStatus : State of Flag.
  * This parameter can be any of the @ref FlagStatus enumeration.
  */
FlagStatus I2C_GetFlagStatus(I2C_Flag_TypeDef Flag)
{

  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_I2C_FLAG_OK(Flag));

  /* Check SRx index */
  switch ((u16)Flag & (u16)0xF000)
  {

      /* Returns whether the status register to check is SR1 */
    case 0x1000:
      /* Check the status of the specified I2C flag */
      if ((I2C->SR1 & (u8)Flag) != 0)
      {
        /* Flag is set */
        bitstatus = SET;
      }
      else
      {
        /* Flag is reset */
        bitstatus = RESET;
      }
      break;

      /* Returns whether the status register to check is SR2 */
    case 0x2000:
      /* Check the status of the specified I2C flag */
      if ((I2C->SR2 & (u8)Flag) != 0)
      {
        /* Flag is set */
        bitstatus = SET;
      }
      else
      {
        /* Flag is reset */
        bitstatus = RESET;
      }
      break;

      /* Returns whether the status register to check is SR3 */
    case 0x3000:
      /* Check the status of the specified I2C flag */
      if ((I2C->SR3 & (u8)Flag) != 0)
      {
        /* Flag is set */
        bitstatus = SET;
      }
      else
      {
        /* Flag is reset */
        bitstatus = RESET;
      }
      break;

    default:
      break;

  }
  /* Return the flag status */
  return bitstatus;
}

/**
  * @brief Clear flags
  * @param[in] Flag : Specifies the flag to clear
  * This parameter can be one of the following:
  * I2C_FLAG_STOPDETECTION
  * I2C_FLAG_HEADERSENT
  * I2C_FLAG_TRANSFERFINISHED
  * I2C_FLAG_ADDRESSSENTMATCHED
  * I2C_FLAG_STARTDETECTION
  * I2C_FLAG_WAKEUPFROMHALT
  * I2C_FLAG_OVERRUNUNDERRUN
  * I2C_FLAG_ACKNOWLEDGEFAILURE
  * I2C_FLAG_ARBITRATIONLOSS
  * I2C_FLAG_BUSERROR   
  * @retval None
  */
void I2C_ClearFlag(I2C_Flag_TypeDef Flag)
{
  u8 tmp1 = 0;
  u8 tmp2 = 0;
  u16 tmp3 = 0;

  /* Check the parameters */
  assert_param(IS_I2C_CLEAR_FLAG_OK(Flag));

  /* Check the clear flag methodology index */
  tmp3 = ((u16)Flag & (u16)0x0F00);
  
   /* Clear the flag directly in the SR2 register */
  if(tmp3 == 0x0100)
  {
	/* Clear the selected I2C flag */
      I2C->SR2 = (u8)(~(u8)Flag);
  }
  /* Flags that need a read of SR1 register and a dummy write in CR2 register to be cleared */
  else if(tmp3 == 0x0200)
  {
      /* Read the SR1 register */
      tmp1 = I2C->SR1;
      /* Dummy write in the CR2 register */
      I2C->CR2 = I2C->CR2;
  }
  /* Flags that need a read of SR1 register followed by a read of SR3 register to be cleared */
  else if(tmp3 == 0x0300)
  {
	  /* 2 variables are used to avoid any compiler optimization */
      /* Read the SR1 register */
      tmp1 = I2C->SR1;
      /* Read the SR3 register */
      tmp2 = I2C->SR3;
  }
  /* Flags that need a read of SR1 register followed by a read of DR register to be cleared */
  else if(tmp3 == 0x0400)
  {
      /* 2 variables are used to avoid any compiler optimization */
      /* Read the SR1 register */
      tmp1 = I2C->SR1;
      /* Read the DR register */
      tmp2 = I2C->DR;
  }
}

/**
  * @brief Returns the specified I2C pending bit status
  * @param[in] ITPendingBit : Specifies the flag to read
  * This parameter can be any of the  @ref I2C_ITPendingBit_TypeDef enumeration.
  * @retval ITStatus : Status of the pending bit.
	* This parameter can be any of the @ref ITStatus enumeration.
  */
ITStatus I2C_GetITStatus(I2C_ITPendingBit_TypeDef ITPendingBit)
{
    ITStatus itstatus = RESET;

    /* Check the parameters */
    assert_param(IS_I2C_ITPENDINGBIT_OK(ITPendingBit));

    /* Check SRx index */
    if (((u16)ITPendingBit & 0xF000) == 0x1000) /* Returns whether the status register to check is SR1 */
    {
        /* Check the status of the specified I2C pending bit */
        if ((I2C->SR1 & (u8)ITPendingBit) != 0)
        {
            /* Flag is set */
            itstatus = SET;
        }
        else
        {
            /* Flag is reset */
            itstatus = RESET;
        }
    }
    else /* Returns whether the status register to check is SR2 */
    {
        /* Check the status of the specified I2C pending bit */
        if ((I2C->SR2 & (u8)ITPendingBit) != 0)
        {
            /* Flag is set */
            itstatus = SET;
        }
        else
        {
            /* Flag is reset */
            itstatus = RESET;
        }
    }

    /* Return the pending bit status */
    return itstatus;

}
/**
  * @brief Clear IT pending bit
  * @param[in] ITPendingBit Specifies the pending bit to clear
  * This parameter can be one of the following:
  * I2C_ITPENDINGBIT_STOPDETECTION
  * I2C_ITPENDINGBIT_HEADERSENT
  * I2C_ITPENDINGBIT_TRANSFERFINISHED 
  * I2C_ITPENDINGBIT_ADDRESSSENTMATCHED
  * I2C_ITPENDINGBIT_STARTDETECTION
  * I2C_ITPENDINGBIT_WAKEUPFROMHALT 
  * I2C_ITPENDINGBIT_OVERRUNUNDERRUN
  * I2C_ITPENDINGBIT_ACKNOWLEDGEFAILURE
  * I2C_ITPENDINGBIT_ARBITRATIONLOSS 
  * I2C_ITPENDINGBIT_BUSERROR  
  * @retval None
  */

void I2C_ClearITPendingBit(I2C_ITPendingBit_TypeDef ITPendingBit)
{
  u8 tmp1 = 0;
  u8 tmp2 = 0;
  u16 tmp3 = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ITPENDINGBIT_OK(ITPendingBit));

  /* Check the clear flag methodology index */
  tmp3 = ((u16)ITPendingBit & (u16)0x0F00);
  
  /* Clear the flag directly in the SR2 register */
  if(tmp3 == 0x0100)
  {
	/* Clear the selected I2C flag */
      I2C->SR2 = (u8)(~(u8)ITPendingBit);
  }
  /* Flags that need a read of SR1 register and a dummy write in CR2 register to be cleared */
  else if(tmp3 == 0x0200)
  {
      /* Read the SR1 register */
      tmp1 = I2C->SR1;
      /* Dummy write in the CR2 register */
      I2C->CR2 = I2C->CR2;
  }
  /* Flags that need a read of SR1 register followed by a read of SR3 register to be cleared */
  else if(tmp3 == 0x0300)
  {
	  /* 2 variables are used to avoid any compiler optimization */
      /* Read the SR1 register */
      tmp1 = I2C->SR1;
      /* Read the SR3 register */
      tmp2 = I2C->SR3;
  }
  /* Flags that need a read of SR1 register followed by a read of DR register to be cleared */
  else if(tmp3 == 0x0400)
  {
      /* 2 variables are used to avoid any compiler optimization */
      /* Read the SR1 register */
      tmp1 = I2C->SR1;
      /* Read the DR register */
      tmp2 = I2C->DR;
  }
}
/**
  * @}
  */


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
