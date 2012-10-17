/**
  ********************************************************************************
  * @file stm8s_uart1.c
  * @brief This file contains all the functions for the UART1 peripheral.
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
#include "stm8s_uart1.h"
#include "stm8s_clk.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/** @}
  * @addtogroup UART1_Public_Functions
  * @{
  */

/**
  * @brief Deinitializes the UART1 peripheral.
  * @par Full description:
  * Set the UART1 peripheral registers to their default reset values.
  * @retval None
	*/
void UART1_DeInit(void)
{
    u8 dummy = 0;

    /*< Clear the Idle Line Detected bit in the status rerister by a read
       to the UART1_SR register followed by a Read to the UART1_DR register */
    dummy = UART1->SR;
    dummy = UART1->DR;

    UART1->BRR2 = UART1_BRR2_RESET_VALUE;  /*< Set UART1_BRR2 to reset value 0x00 */
    UART1->BRR1 = UART1_BRR1_RESET_VALUE;  /*< Set UART1_BRR1 to reset value 0x00 */

    UART1->CR1 = UART1_CR1_RESET_VALUE; /*< Set UART1_CR1 to reset value 0x00  */
    UART1->CR2 = UART1_CR2_RESET_VALUE; /*< Set UART1_CR2 to reset value 0x00  */
    UART1->CR3 = UART1_CR3_RESET_VALUE;  /*< Set UART1_CR3 to reset value 0x00  */
    UART1->CR4 = UART1_CR4_RESET_VALUE;  /*< Set UART1_CR4 to reset value 0x00  */
    UART1->CR5 = UART1_CR5_RESET_VALUE; /*< Set UART1_CR5 to reset value 0x00  */

    UART1->GTR = UART1_GTR_RESET_VALUE;
    UART1->PSCR = UART1_PSCR_RESET_VALUE;
}

/**
  * @brief Initializes the UART1 according to the specified parameters.
  * @param[in] BaudRate: The baudrate.
  * @param[in] WordLength : This parameter can be any of the @ref UART1_WordLength_TypeDef enumeration.
  * @param[in] StopBits: This parameter can be any of the @ref UART1_StopBits_TypeDef enumeration.
  * @param[in] Parity: This parameter can be any of the @ref UART1_Parity_TypeDef enumeration.
  * @param[in] SyncMode: This parameter can be any of the @ref UART1_SyncMode_TypeDef values.
  * @param[in] Mode: This parameter can be any of the @ref UART1_Mode_TypeDef values
  * @retval
  * None
  */
void UART1_Init(u32 BaudRate, UART1_WordLength_TypeDef WordLength, UART1_StopBits_TypeDef StopBits, UART1_Parity_TypeDef Parity, UART1_SyncMode_TypeDef SyncMode, UART1_Mode_TypeDef Mode)
{
    u32 BaudRate_Mantissa, BaudRate_Mantissa100 = 0;

    /* assert_param: BaudRate value should be <= 625000 bps */
    assert_param(IS_UART1_BAUDRATE_OK(BaudRate));

    assert_param(IS_UART1_WORDLENGTH_OK(WordLength));

    assert_param(IS_UART1_STOPBITS_OK(StopBits));

    assert_param(IS_UART1_PARITY_OK(Parity));

    /* assert_param: UART1_Mode value should exclude values such as  UART1_ModeTx_Enable|UART1_ModeTx_Disable */
    assert_param(IS_UART1_MODE_OK((u8)Mode));

    /* assert_param: UART1_SyncMode value should exclude values such as
       UART1_CLOCK_ENABLE|UART1_CLOCK_DISABLE */
    assert_param(IS_UART1_SYNCMODE_OK((u8)SyncMode));

    UART1->CR1 &= (u8)(~UART1_CR1_M);  /**< Clear the word length bit */
    UART1->CR1 |= (u8)WordLength; /**< Set the word length bit according to UART1_WordLength value */

    UART1->CR3 &= (u8)(~UART1_CR3_STOP);  /**< Clear the STOP bits */
    UART1->CR3 |= (u8)StopBits;  /**< Set the STOP bits number according to UART1_StopBits value  */

    UART1->CR1 &= (u8)(~(UART1_CR1_PCEN | UART1_CR1_PS  ));  /**< Clear the Parity Control bit */
    UART1->CR1 |= (u8)Parity;  /**< Set the Parity Control bit to UART1_Parity value */

    UART1->BRR1 &= (u8)(~UART1_BRR1_DIVM);  /**< Clear the LSB mantissa of UART1DIV  */
    UART1->BRR2 &= (u8)(~UART1_BRR2_DIVM);  /**< Clear the MSB mantissa of UART1DIV  */
    UART1->BRR2 &= (u8)(~UART1_BRR2_DIVF);  /**< Clear the Fraction bits of UART1DIV */

    /**< Set the UART1 BaudRates in BRR1 and BRR2 registers according to UART1_BaudRate value */
    BaudRate_Mantissa    = ((u32)CLK_GetClockFreq() / (BaudRate << 4));
    BaudRate_Mantissa100 = (((u32)CLK_GetClockFreq() * 100) / (BaudRate << 4));
    UART1->BRR2 |= (u8)((u8)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100)) << 4) / 100) & (u8)0x0F); /**< Set the fraction of UART1DIV  */
    UART1->BRR2 |= (u8)((BaudRate_Mantissa >> 4) & (u8)0xF0); /**< Set the MSB mantissa of UART1DIV  */
    UART1->BRR1 |= (u8)BaudRate_Mantissa;           /**< Set the LSB mantissa of UART1DIV  */

    UART1->CR2 &= (u8)~(UART1_CR2_TEN | UART1_CR2_REN); /**< Disable the Transmitter and Receiver before seting the LBCL, CPOL and CPHA bits */
    UART1->CR3 &= (u8)~(UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL); /**< Clear the Clock Polarity, lock Phase, Last Bit Clock pulse */
    UART1->CR3 |= (u8)((u8)SyncMode & (u8)(UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL));  /**< Set the Clock Polarity, lock Phase, Last Bit Clock pulse */

    if ((u8)Mode & (u8)UART1_MODE_TX_ENABLE)
    {
        UART1->CR2 |= (u8)UART1_CR2_TEN;  /**< Set the Transmitter Enable bit */
    }
    else
    {
        UART1->CR2 &= (u8)(~UART1_CR2_TEN);  /**< Clear the Transmitter Disable bit */
    }
    if ((u8)Mode & (u8)UART1_MODE_RX_ENABLE)
    {
        UART1->CR2 |= (u8)UART1_CR2_REN;  /**< Set the Receiver Enable bit */
    }
    else
    {
        UART1->CR2 &= (u8)(~UART1_CR2_REN);  /**< Clear the Receiver Disable bit */
    }
    /**< Set the Clock Enable bit, lock Polarity, lock Phase and Last Bit Clock pulse bits according to UART1_Mode value */
    if ((u8)SyncMode&(u8)UART1_SYNCMODE_CLOCK_DISABLE)
    {
        UART1->CR3 &= (u8)(~UART1_CR3_CKEN); /**< Clear the Clock Enable bit */
        /**< configure in Push Pull or Open Drain mode the Tx I/O line by setting the correct I/O Port register according the product package and line configuration*/
    }
    else
    {
        UART1->CR3 |= (u8)((u8)SyncMode & UART1_CR3_CKEN);
    }
}

/**
  * @brief Enable the UART1 peripheral.
  * @par Full description:
  * Enable the UART1 peripheral.
  * @param[in] NewState new state of the UART1 Communication.
  * This parameter can be:
  * - ENABLE
  * - DISABLE
  * @retval
  * None
  */
void UART1_Cmd(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        UART1->CR1 &= (u8)(~UART1_CR1_UARTD); /**< UART1 Enable */
    }
    else
    {
        UART1->CR1 |= UART1_CR1_UARTD;  /**< UART1 Disable (for low power consumption) */
    }
}

/**
  * @brief Enables or disables the specified UART1 interrupts.
  * @par Full description:
  * Enables or disables the specified UART1 interrupts.
  * @param[in] UART1_IT specifies the UART1 interrupt sources to be enabled or disabled.
  * This parameter can be one of the following values:
  *   - UART1_IT_LBDF:  LIN Break detection interrupt
  *   - UART1_IT_TXE:  Tansmit Data Register empty interrupt
  *   - UART1_IT_TC:   Transmission complete interrupt
  *   - UART1_IT_RXNE_OR: Receive Data register not empty/Over run error interrupt
  *   - UART1_IT_IDLE: Idle line detection interrupt
  *   - UART1_IT_PE:   Parity Error interrupt
  * @param[in] NewState new state of the specified UART1 interrupts.
  * This parameter can be: ENABLE or DISABLE.
  * @retval
  * None
  */
void UART1_ITConfig(UART1_IT_TypeDef UART1_IT, FunctionalState NewState)
{
    u8 uartreg, itpos = 0x00;
    assert_param(IS_UART1_CONFIG_IT_OK(UART1_IT));
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* Get the UART1 register index */
    uartreg = (u8)(UART1_IT >> (u8)0x08);
    /* Get the UART1 IT index */
    itpos = (u8)((u8)1 << (u8)((u8)UART1_IT & (u8)0x0F));

    if (NewState != DISABLE)
    {
        /**< Enable the Interrupt bits according to UART1_IT mask */
        if (uartreg == 0x01)
        {
            UART1->CR1 |= itpos;
        }
        else if (uartreg == 0x02)
        {
            UART1->CR2 |= itpos;
        }
        else
        {
            UART1->CR4 |= itpos;
        }
    }
    else
    {
        /**< Disable the interrupt bits according to UART1_IT mask */
        if (uartreg == 0x01)
        {
            UART1->CR1 &= (u8)(~itpos);
        }
        else if (uartreg == 0x02)
        {
            UART1->CR2 &= (u8)(~itpos);
        }
        else
        {
            UART1->CR4 &= (u8)(~itpos);
        }
    }

}
/**
  * @brief Enables or disables the UART1’s Half Duplex communication.
  * @par Full description:
  * Enables or disables the UART1’s Half Duplex communication.
  * @param[in] NewState new state of the UART1 Communication.
  *                    This parameter can be: ENABLE or DISABLE.
  * @retval
  * None
  */
void UART1_HalfDuplexCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        UART1->CR5 |= UART1_CR5_HDSEL;  /**< UART1 Half Duplex Enable  */
    }
    else
    {
        UART1->CR5 &= (u8)~UART1_CR5_HDSEL; /**< UART1 Half Duplex Disable */
    }
}

/**
  * @brief Configures the UART1’s IrDA interface.
  * @par Full description:
  * Configures the UART1’s IrDA interface.
  * @par This function is valid only for UART1.
  * @param[in] UART1_IrDAMode specifies the IrDA mode.
  * This parameter can be any of the @ref UART1_IrDAMode_TypeDef values.
  * @retval
  * None
  */
void UART1_IrDAConfig(UART1_IrDAMode_TypeDef UART1_IrDAMode)
{
    assert_param(IS_UART1_IRDAMODE_OK(UART1_IrDAMode));

    if (UART1_IrDAMode != UART1_IRDAMODE_NORMAL)
    {
        UART1->CR5 |= UART1_CR5_IRLP;
    }
    else
    {
        UART1->CR5 &= ((u8)~UART1_CR5_IRLP);
    }
}

/**
  * @brief Enables or disables the UART1’s IrDA interface.
  * @par Full description:
  * Enables or disables the UART1’s IrDA interface.
  * @par This function is related to IrDA mode.
  * @param[in] NewState new state of the IrDA mode.
  * This parameter can be: ENABLE or DISABLE.
  * @retval
  * None
  */
void UART1_IrDACmd(FunctionalState NewState)
{

    /* Check parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the IrDA mode by setting the IREN bit in the CR3 register */
        UART1->CR5 |= UART1_CR5_IREN;
    }
    else
    {
        /* Disable the IrDA mode by clearing the IREN bit in the CR3 register */
        UART1->CR5 &= ((u8)~UART1_CR5_IREN);
    }
}

/**
  * @brief Sets the UART1 LIN Break detection length.
  * @par Full description:
  * Sets the UART1 LIN Break detection length.
  * @param[in] UART1_LINBreakDetectionLength specifies the LIN break detection length.
  * This parameter can be any of the @ref UART1_LINBreakDetectionLength_TypeDef values.
  * @retval
  * None
  */
void UART1_LINBreakDetectionConfig(UART1_LINBreakDetectionLength_TypeDef UART1_LINBreakDetectionLength)
{
    assert_param(IS_UART1_LINBREAKDETECTIONLENGTH_OK(UART1_LINBreakDetectionLength));

    if (UART1_LINBreakDetectionLength != UART1_LINBREAKDETECTIONLENGTH_10BITS)
    {
        UART1->CR4 |= UART1_CR4_LBDL;
    }
    else
    {
        UART1->CR4 &= ((u8)~UART1_CR4_LBDL);
    }
}

/**
  * @brief Enables or disables the UART1’s LIN mode.
  * @par Full description:
  * Enables or disables the UART1’s LIN mode.
  * @param[in] NewState is new state of the UART1 LIN mode.
  * This parameter can be: ENABLE or DISABLE.
  * @retval
  * None
  */
void UART1_LINCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the LIN mode by setting the LINE bit in the CR2 register */
        UART1->CR3 |= UART1_CR3_LINEN;
    }
    else
    {
        /* Disable the LIN mode by clearing the LINE bit in the CR2 register */
        UART1->CR3 &= ((u8)~UART1_CR3_LINEN);
    }
}
/**
  * @brief Enables or disables the UART1 Smart Card mode.
  * @par Full description:
  * Enables or disables the UART1 Smart Card mode.
  * @par This function is related to SmartCard mode.
  * @param[in] NewState: new state of the Smart Card mode.
  * This parameter can be: ENABLE or DISABLE.
  * @retval
  * None
  */
void UART1_SmartCardCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the SC mode by setting the SCEN bit in the CR5 register */
        UART1->CR5 |= UART1_CR5_SCEN;
    }
    else
    {
        /* Disable the SC mode by clearing the SCEN bit in the CR5 register */
        UART1->CR5 &= ((u8)(~UART1_CR5_SCEN));
    }
}

/**
  * @brief Enables or disables NACK transmission.
  * @par Full description:
  * Enables or disables NACK transmission.
  * @par This function is valid only for UART1 because is related to SmartCard mode.
  * @param[in] NewState: new state of the Smart Card mode.
  * This parameter can be: ENABLE or DISABLE.
  */
void UART1_SmartCardNACKCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the NACK transmission by setting the NACK bit in the CR5 register */
        UART1->CR5 |= UART1_CR5_NACK;
    }
    else
    {
        /* Disable the NACK transmission by clearing the NACK bit in the CR5 register */
        UART1->CR5 &= ((u8)~(UART1_CR5_NACK));
    }
}

/**
  * @brief Selects the UART1 WakeUp method.
  * @par Full description:
  * Selects the UART1 WakeUp method.
  * @param[in] UART1_WakeUp: specifies the UART1 wakeup method.
  * This parameter can be any of the @ref UART1_WakeUp_TypeDef values.
  * @retval
  * None
  */
void UART1_WakeUpConfig(UART1_WakeUp_TypeDef UART1_WakeUp)
{
    assert_param(IS_UART1_WAKEUP_OK(UART1_WakeUp));

    UART1->CR1 &= ((u8)~UART1_CR1_WAKE);
    UART1->CR1 |= (u8)UART1_WakeUp;
}
/**
  * @brief Determines if the UART1 is in mute mode or not.
  * @par Full description:
  * Determines if the UART1 is in mute mode or not.
  * @param[in] NewState: new state of the UART1 mode.
  * This parameter can be: ENABLE or DISABLE.
  * @retval
  * None
  */
void UART1_ReceiverWakeUpCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the mute mode UART1 by setting the RWU bit in the CR2 register */
        UART1->CR2 |= UART1_CR2_RWU;
    }
    else
    {
        /* Disable the mute mode UART1 by clearing the RWU bit in the CR1 register */
        UART1->CR2 &= ((u8)~UART1_CR2_RWU);
    }
}

/**
  * @brief Returns the most recent received data by the UART1 peripheral.
  * @par Full description:
  * Returns the most recent received data by the UART1 peripheral.
  * @retval u8 Received Data
  * @par Required preconditions:
  * UART1_Cmd(ENABLE);
  */
u8 UART1_ReceiveData8(void)
{
    return ((u8)UART1->DR);
}


/**
  * @brief Returns the most recent received data by the UART1 peripheral.
  * @par Full description:
  * Returns the most recent received data by the UART1 peripheral.
  * @retval u16 Received Data
  * @par Required preconditions:
  * UART1_Cmd(ENABLE);
  */
u16 UART1_ReceiveData9(void)
{
    return (u16)( (((u16) UART1->DR) | ((u16)(((u16)( (u16)UART1->CR1 & (u16)UART1_CR1_R8)) << 1))) & ((u16)0x01FF));
}

/**
  * @brief Transmits 8 bit data through the UART1 peripheral.
  * @par Full description:
  * Transmits 8 bit data through the UART1 peripheral.
  * @param[in] Data: the data to transmit.
  * @retval
  * None
  * @par Required preconditions:
  * UART1_Cmd(ENABLE);
  */
void UART1_SendData8(u8 Data)
{
    /* Transmit Data */
    UART1->DR = Data;
}

/**
  * @brief Transmits 9 bit data through the UART1 peripheral.
  * @par Full description:
  * Transmits 9 bit data through the UART1 peripheral.
  * @param[in] Data: the data to transmit.
  * @retval
  * None
  * @par Required preconditions:
  * UART1_Cmd(ENABLE);
  */
void UART1_SendData9(u16 Data)
{
    /**< Clear the transmit data bit 8 [8]  */
    UART1->CR1 &= ((u8)~UART1_CR1_T8);
    /**< Write the transmit data bit [8]  */
    UART1->CR1 |= (u8)(((u8)(Data >> 2)) & UART1_CR1_T8);
    /**< Write the transmit data bit [0:7] */
    UART1->DR   = (u8)(Data);
}

/**
  * @brief Transmits break characters.
  * @par Full description:
  * Transmits break characters on the UART1 peripheral.
  * @retval
  * None
  */
void UART1_SendBreak(void)
{
    UART1->CR2 |= UART1_CR2_SBK;
}


/**
  * @brief Sets the address of the UART1 node.
  * @par Full description:
  * Sets the address of the UART1 node.
  * @param[in] UART1_Address: Indicates the address of the UART1 node.
  * @retval
  * None
  */
void UART1_SetAddress(u8 UART1_Address)
{
    /*assert_param for UART1_Address*/
    assert_param(IS_UART1_ADDRESS_OK(UART1_Address));

    /* Clear the UART1 address */
    UART1->CR4 &= ((u8)~UART1_CR4_ADD);
    /* Set the UART1 address node */
    UART1->CR4 |= UART1_Address;
}

/**
  * @brief Sets the specified UART1 guard time.
  * @par Full description:
  * Sets the address of the UART1 node.
  * @par This function is related to SmartCard mode.
  * @param[in] UART1_GuardTime: specifies the guard time.
  * @retval
  * None
  * @par Required preconditions:
  * SmartCard Mode Enabled
  */
void UART1_SetGuardTime(u8 UART1_GuardTime)
{
    /* Set the UART1 guard time */
    UART1->GTR = UART1_GuardTime;
}

/**
  * @brief Sets the system clock prescaler.
  * @par Full description:
  * Sets the system clock prescaler.
  * @par This function is related to SmartCard and IrDa mode.
  * @param[in] UART1_Prescaler: specifies the prescaler clock.
  * This parameter can be one of the following values:
  *                       @par IrDA Low Power Mode
  *   The clock source is diveded by the value given in the register (8 bits)
  *                       - 0000 0000 Reserved
  *                       - 0000 0001 divides the clock source by 1
  *                       - 0000 0010 divides the clock source by 2
  *                       - ...........................................................
  *                       @par Smart Card Mode
  *   The clock source is diveded by the value given in the register (5 significant bits) multipied by 2
  *                       - 0 0000 Reserved
  *                       - 0 0001 divides the clock source by 2
  *                       - 0 0010 divides the clock source by 4
  *                       - 0 0011 divides the clock source by 6
  *                       - ...........................................................
  * @retval
  * None
*/
void UART1_SetPrescaler(u8 UART1_Prescaler)
{
    /* Load the UART1 prescaler value*/
    UART1->PSCR = UART1_Prescaler;
}

/**
  * @brief Checks whether the specified UART1 flag is set or not.
  * @par Full description:
  * Checks whether the specified UART1 flag is set or not.
  * @param[in] UART1_FLAG specifies the flag to check.
  * This parameter can be any of the @ref UART1_Flag_TypeDef enumeration.
  * @retval FlagStatus (SET or RESET)
  */
FlagStatus UART1_GetFlagStatus(UART1_Flag_TypeDef UART1_FLAG)
{
    FlagStatus status = RESET;

    /* Check parameters */
    assert_param(IS_UART1_FLAG_OK(UART1_FLAG));


    /* Check the status of the specified UART1 flag*/
    if (UART1_FLAG == UART1_FLAG_LBDF)
    {
        if ((UART1->CR4 & (u8)UART1_FLAG) != (u8)0x00)
        {
            /* UART1_FLAG is set*/
            status = SET;
        }
        else
        {
            /* UART1_FLAG is reset*/
            status = RESET;
        }
    }
    else if (UART1_FLAG == UART1_FLAG_SBK)
    {
        if ((UART1->CR2 & (u8)UART1_FLAG) != (u8)0x00)
        {
            /* UART1_FLAG is set*/
            status = SET;
        }
        else
        {
            /* UART1_FLAG is reset*/
            status = RESET;
        }
    }
    else
    {
        if ((UART1->SR & (u8)UART1_FLAG) != (u8)0x00)
        {
            /* UART1_FLAG is set*/
            status = SET;
        }
        else
        {
            /* UART1_FLAG is reset*/
            status = RESET;
        }
    }
    /* Return the UART1_FLAG status*/
    return status;
}
/**
  * @brief Clears the UART1 flags.
  * @par Full description:
  * Clears the UART1 flags.
  * @param[in] UART1_FLAG specifies the flag to clear
  * This parameter can be any combination of the following values:
  *   - UART1_FLAG_LBDF: LIN Break detection flag.
  *   - UART1_FLAG_RXNE: Receive data register not empty flag.
  * @par Notes:
  *   - PE (Parity error), FE (Framing error), NE (Noise error), OR (OverRun error)
  *     and IDLE (Idle line detected) flags are cleared by software sequence: a read
  *    operation to UART1_SR register (UART1_GetFlagStatus())followed by a read operation
  *     to UART1_DR register(UART1_ReceiveData8() or UART1_ReceiveData9()).
  *   - RXNE flag can be also cleared by a read to the UART1_DR register
  *     (UART1_ReceiveData8()or UART1_ReceiveData9()).
  *   - TC flag can be also cleared by software sequence: a read operation to UART1_SR
  *     register (UART1_GetFlagStatus()) followed by a write operation to UART1_DR register
  *     (UART1_SendData8() or UART1_SendData9()).
  *   - TXE flag is cleared only by a write to the UART1_DR register (UART1_SendData8() or
  *     UART1_SendData9()).
  *   - SBK flag is cleared during the stop bit of break.
  * @retval
  * None
  */

void UART1_ClearFlag(UART1_Flag_TypeDef UART1_FLAG)
{
    assert_param(IS_UART1_CLEAR_FLAG_OK(UART1_FLAG));

    /* Clear the Receive Register Not Empty flag */
    if (UART1_FLAG == UART1_FLAG_RXNE)
    {
        UART1->SR = (u8)~(UART1_SR_RXNE);
    }
    /* Clear the LIN Break Detection flag */
    else
    {
        UART1->CR4 &= (u8)~(UART1_CR4_LBDF);
    }
}

/**
  * @brief Checks whether the specified UART1 interrupt has occurred or not.
  * @par Full description:
  * Checks whether the specified UART1 interrupt has occurred or not.
  * @param[in] UART1_IT: Specifies the UART1 interrupt pending bit to check.
  * This parameter can be one of the following values:
  *   - UART1_IT_LBDF:  LIN Break detection interrupt
  *   - UART1_IT_TXE:  Tansmit Data Register empty interrupt
  *   - UART1_IT_TC:   Transmission complete interrupt
  *   - UART1_IT_RXNE: Receive Data register not empty interrupt
  *   - UART1_IT_IDLE: Idle line detection interrupt
  *   - UART1_IT_OR:  OverRun Error interrupt
  *   - UART1_IT_PE:   Parity Error interrupt
  * @retval
  * ITStatus The new state of UART1_IT (SET or RESET).
  */
ITStatus UART1_GetITStatus(UART1_IT_TypeDef UART1_IT)
{
    ITStatus pendingbitstatus = RESET;
    u8 itpos = 0;
    u8 itmask1 = 0;
    u8 itmask2 = 0;
    u8 enablestatus = 0;

    /* Check parameters */
    assert_param(IS_UART1_GET_IT_OK(UART1_IT));

    /* Get the UART1 IT index */
    itpos = (u8)((u8)1 << (u8)((u8)UART1_IT & (u8)0x0F));
    /* Get the UART1 IT index */
    itmask1 = (u8)((u8)UART1_IT >> (u8)4);
    /* Set the IT mask*/
    itmask2 = (u8)((u8)1 << itmask1);


    /* Check the status of the specified UART1 pending bit*/
    if (UART1_IT == UART1_IT_PE)
    {
        /* Get the UART1_IT enable bit status*/
        enablestatus = (u8)((u8)UART1->CR1 & itmask2);
        /* Check the status of the specified UART1 interrupt*/

        if (((UART1->SR & itpos) != (u8)0x00) && enablestatus)
        {
            /* Interrupt occurred*/
            pendingbitstatus = SET;
        }
        else
        {
            /* Interrupt not occurred*/
            pendingbitstatus = RESET;
        }
    }

    else if (UART1_IT == UART1_IT_LBDF)
    {
        /* Get the UART1_IT enable bit status*/
        enablestatus = (u8)((u8)UART1->CR4 & itmask2);
        /* Check the status of the specified UART1 interrupt*/
        if (((UART1->CR4 & itpos) != (u8)0x00) && enablestatus)
        {
            /* Interrupt occurred*/
            pendingbitstatus = SET;
        }
        else
        {
            /* Interrupt not occurred*/
            pendingbitstatus = RESET;
        }
    }
    else
    {
        /* Get the UART1_IT enable bit status*/
        enablestatus = (u8)((u8)UART1->CR2 & itmask2);
        /* Check the status of the specified UART1 interrupt*/
        if (((UART1->SR & itpos) != (u8)0x00) && enablestatus)
        {
            /* Interrupt occurred*/
            pendingbitstatus = SET;
        }
        else
        {
            /* Interrupt not occurred*/
            pendingbitstatus = RESET;
        }
    }

    /* Return the UART1_IT status*/
    return  pendingbitstatus;
}

/**
  * @brief Clears the UART1 pending flags.
  * @par Full description:
  * Clears the UART1 pending bit.
  * @param[in] UART1_IT specifies the pending bit to clear
  * This parameter can be one of the following values:
  *   - UART1_IT_LBDF:  LIN Break detection interrupt
  *   - UART1_IT_RXNE: Receive Data register not empty interrupt.
  * @par Notes:
  *   - PE (Parity error), FE (Framing error), NE (Noise error), OR (OverRun error) and
  *     IDLE (Idle line detected) pending bits are cleared by software sequence: a read
  *     operation to UART1_SR register (UART1_GetITStatus()) followed by a read operation
  *     to UART1_DR register (UART1_ReceiveData8() or UART1_ReceiveData9()).
  *   - RXNE pending bit can be also cleared by a read to the UART1_DR register
  *     (UART1_ReceiveData8() or UART1_ReceiveData9()).
  *   - TC (Transmit complet) pending bit can be cleared by software sequence: a read
  *     operation to UART1_SR register (UART1_GetITStatus()) followed by a write operation
  *     to UART1_DR register (UART1_SendData8()or UART1_SendData9()).
  *   - TXE pending bit is cleared only by a write to the UART1_DR register
  *     (UART1_SendData8() or UART1_SendData9()).
  * @retval
  * None
  */
void UART1_ClearITPendingBit(UART1_IT_TypeDef UART1_IT)
{
    assert_param(IS_UART1_CLEAR_FLAG_OK(UART1_IT));

    /*< Clear the Receive Register Not Empty pending bit */
    if (UART1_IT == UART1_IT_RXNE)
    {
        UART1->SR = (u8)~(UART1_SR_RXNE);
    }
    /*< Clear the LIN Break Detection pending bit */
    else
    {
        UART1->CR4 &= (u8)~(UART1_CR4_LBDF);
    }
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
