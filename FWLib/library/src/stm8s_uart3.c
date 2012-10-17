/**
  ********************************************************************************
  * @file stm8s_uart3.c
  * @brief This file contains all the functions for the uart3 peripheral.
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
#include "stm8s_uart3.h"
#include "stm8s_clk.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/** @}
  * @addtogroup UART3_Public_Functions
  * @{
  */

/**
	* @brief Deinitializes the UART3 peripheral.
  * @par Full description:
  * Set the UART3 peripheral registers to their default reset values.
  * @retval
  * None
  */

void UART3_DeInit(void)
{
    u8 dummy = 0;
    /*< Clear the Idle Line Detected bit in the status rerister by a read
       to the UART3_SR register followed by a Read to the UART3_DR register */
    dummy = UART3->SR;
    dummy = UART3->DR;

    UART3->BRR2 = UART3_BRR2_RESET_VALUE;  /*< Set UART3_BRR2 to reset value 0x00 */
    UART3->BRR1 = UART3_BRR1_RESET_VALUE;  /*< Set UART3_BRR1 to reset value 0x00 */

    UART3->CR1 = UART3_CR1_RESET_VALUE; /*< Set UART3_CR1 to reset value 0x00  */
    UART3->CR2 = UART3_CR2_RESET_VALUE; /*< Set UART3_CR2 to reset value 0x00  */
    UART3->CR3 = UART3_CR3_RESET_VALUE;  /*< Set UART3_CR3 to reset value 0x00  */
    UART3->CR4 = UART3_CR4_RESET_VALUE;  /*< Set UART3_CR4 to reset value 0x00  */
    UART3->CR6 = UART3_CR6_RESET_VALUE; /*< Set UART3_CR6 to reset value 0x00  */

}

/**
  * @brief Initializes the UART3 according to the specified parameters.
  * @param[in] BaudRate: The baudrate.
  * @param[in] WordLength : This parameter can be any of the @ref UART3_WordLength_TypeDef enumeration.
  * @param[in] StopBits: This parameter can be any of the @ref UART3_StopBits_TypeDef enumeration.
  * @param[in] Parity: This parameter can be any of the @ref UART3_Parity_TypeDef enumeration.
  * @param[in] Mode: This parameter can be any of the @ref UART3_Mode_TypeDef values
  * @retval
  * None
  */
void UART3_Init(u32 BaudRate, UART3_WordLength_TypeDef WordLength, UART3_StopBits_TypeDef StopBits, UART3_Parity_TypeDef Parity, UART3_Mode_TypeDef Mode)
{
    u8 BRR2_1, BRR2_2 = 0;
    u32 BaudRate_Mantissa, BaudRate_Mantissa100 = 0;

    assert_param(IS_UART3_WORDLENGTH_OK(WordLength));

    assert_param(IS_UART3_STOPBITS_OK(StopBits));

    assert_param(IS_UART3_PARITY_OK(Parity));

    /* assert_param: BaudRate value should be <= 625000 bps */
    assert_param(IS_UART3_BAUDRATE_OK(BaudRate));

    /* assert_param: UART3_Mode value should exclude values such as  UART3_ModeTx_Enable|UART3_ModeTx_Disable */
    assert_param(IS_UART3_MODE_OK((u8)Mode));

    /* Wait for no Transmition before modifying the M bit */
    /* while(!(UART3->SR&UART3_SR_TC));      */

    UART3->CR1 &= (u8)(~UART3_CR1_M);     /**< Clear the word length bit */
    UART3->CR1 |= (u8)WordLength; /**< Set the word length bit according to UART3_WordLength value */

    UART3->CR3 &= (u8)(~UART3_CR3_STOP);  /**< Clear the STOP bits */
    UART3->CR3 |= (u8)StopBits;  /**< Set the STOP bits number according to UART3_StopBits value  */

    UART3->CR1 &= (u8)(~(UART3_CR1_PCEN | UART3_CR1_PS));  /**< Clear the Parity Control bit */
    UART3->CR1 |= (u8)Parity;     /**< Set the Parity Control bit to UART3_Parity value */

    UART3->BRR1 &= (u8)(~UART3_BRR1_DIVM);  /**< Clear the LSB mantissa of UART3DIV  */
    UART3->BRR2 &= (u8)(~UART3_BRR2_DIVM);  /**< Clear the MSB mantissa of UART3DIV  */
    UART3->BRR2 &= (u8)(~UART3_BRR2_DIVF);  /**< Clear the Fraction bits of UART3DIV */

    /**< Set the UART3 BaudRates in BRR1 and BRR2 registers according to UART3_BaudRate value */
    BaudRate_Mantissa    = ((u32)CLK_GetClockFreq() / (BaudRate << 4));
    BaudRate_Mantissa100 = (((u32)CLK_GetClockFreq() * 100) / (BaudRate << 4));
    /**< The fraction and MSB mantissa should be loaded in one step in the BRR2 register */
    BRR2_1 = (u8)((u8)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100))
                        << 4) / 100) & (u8)0x0F); /**< Set the fraction of UART3DIV  */
    BRR2_2 = (u8)((BaudRate_Mantissa >> 4) & (u8)0xF0);

    UART3->BRR2 = (u8)(BRR2_1 | BRR2_2);
    UART3->BRR1 = (u8)BaudRate_Mantissa;           /**< Set the LSB mantissa of UART3DIV  */

    if ((u8)Mode&(u8)UART3_MODE_TX_ENABLE)
    {
        UART3->CR2 |= UART3_CR2_TEN;  /**< Set the Transmitter Enable bit */
    }
    else
    {
        UART3->CR2 &= (u8)(~UART3_CR2_TEN);  /**< Clear the Transmitter Disable bit */
    }
    if ((u8)Mode & (u8)UART3_MODE_RX_ENABLE)
    {
        UART3->CR2 |= UART3_CR2_REN;  /**< Set the Receiver Enable bit */
    }
    else
    {
        UART3->CR2 &= (u8)(~UART3_CR2_REN);  /**< Clear the Receiver Disable bit */
    }
}
/**
  * @brief Enable the UART3 peripheral.
  * @par Full description:
  * Enable the UART3 peripheral.
  * @param[in] NewState new state of the UART3 Communication.
  * This parameter can be:
  * - ENABLE
  * - DISABLE
  * @retval
  * None
  */
void UART3_Cmd(FunctionalState NewState)
{

    if (NewState != DISABLE)
    {
        UART3->CR1 &= (u8)(~UART3_CR1_UARTD); /**< UART3 Enable */
    }
    else
    {
        UART3->CR1 |= UART3_CR1_UARTD;  /**< UART3 Disable (for low power consumption) */
    }
}

/**
  * @brief Enables or disables the specified UART3 interrupts.
  * @par Full description:
  * Enables or disables the specified UART3 interrupts.
  * @param[in] UART3_IT specifies the UART3 interrupt sources to be enabled or disabled.
  * This parameter can be one of the following values:
  *   - UART3_IT_LBDF:  LIN Break detection interrupt
  *   - UART3_IT_LHDF:  LIN Break detection interrupt
  *   - UART3_IT_TXE:  Tansmit Data Register empty interrupt
  *   - UART3_IT_TC:   Transmission complete interrupt
  *   - UART3_IT_RXNE_OR: Receive Data register not empty/Over run error interrupt
  *   - UART3_IT_IDLE: Idle line detection interrupt
  *   - UART3_IT_PE:   Parity Error interrupt
  * @param[in] NewState new state of the specified UART3 interrupts.
  * This parameter can be: ENABLE or DISABLE.
  * @retval
  * None
  */
void UART3_ITConfig(UART3_IT_TypeDef UART3_IT, FunctionalState NewState)
{
    u8 uartreg, itpos = 0x00;
    assert_param(IS_UART3_CONFIG_IT_OK(UART3_IT));
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    /* Get the UART3 register index */
    uartreg = (u8)(UART3_IT >> 0x08);

    /* Get the UART3 IT index */
    itpos = (u8)((u8)1 << (u8)((u8)UART3_IT & (u8)0x0F));

    if (NewState != DISABLE)
    {
        /**< Enable the Interrupt bits according to UART3_IT mask */
        if (uartreg == 0x01)
        {
            UART3->CR1 |= itpos;
        }
        else if (uartreg == 0x02)
        {
            UART3->CR2 |= itpos;
        }
        else if (uartreg == 0x03)
        {
            UART3->CR4 |= itpos;
        }
        else
        {
            UART3->CR6 |= itpos;
        }
    }
    else
    {
        /**< Disable the interrupt bits according to UART3_IT mask */
        if (uartreg == 0x01)
        {
            UART3->CR1 &= (u8)(~itpos);
        }
        else if (uartreg == 0x02)
        {
            UART3->CR2 &= (u8)(~itpos);
        }
        else if (uartreg == 0x03)
        {
            UART3->CR4 &= (u8)(~itpos);
        }
        else
        {
            UART3->CR6 &= (u8)(~itpos);
        }
    }
}

/**
  * @brief Sets the UART3 LIN Break detection length.
  * @par Full description:
  * Sets the UART3 LIN Break detection length.
  * @param[in] UART3_LINBreakDetectionLength specifies the LIN break detection length.
  * This parameter can be any of the @ref UART3_LINBreakDetectionLength_TypeDef values.
  * @retval
  * None
  */
void UART3_LINBreakDetectionConfig(UART3_LINBreakDetectionLength_TypeDef UART3_LINBreakDetectionLength)
{
    assert_param(IS_UART3_LINBREAKDETECTIONLENGTH_OK(UART3_LINBreakDetectionLength));

    if (UART3_LINBreakDetectionLength != UART3_LINBREAKDETECTIONLENGTH_10BITS)
    {
        UART3->CR4 |= UART3_CR4_LBDL;
    }
    else
    {
        UART3->CR4 &= ((u8)~UART3_CR4_LBDL);
    }
}

/**
  * @brief Configue the UART3 peripheral.
  * @par Full description:
  * Configue the UART3 peripheral.
  * @param[in] UART3_Mode specifies the LIN mode.
  * This parameter can be any of the @ref UART3_LinMode_TypeDef values.
  * @param[in] UART3_Autosync specifies the LIN automatic resynchronization mode.
  * This parameter can be any of the @ref UART3_LinAutosync_TypeDef values.
  * @param[in] UART3_DivUp specifies the LIN divider update method.
  * This parameter can be any of the @ref UART3_LinDivUp_TypeDef values.
  * @retval
  * None
  */
void UART3_LINConfig(UART3_LinMode_TypeDef UART3_Mode, UART3_LinAutosync_TypeDef UART3_Autosync, UART3_LinDivUp_TypeDef UART3_DivUp)
{
    assert_param(IS_UART3_SLAVE_OK(UART3_Mode));

    assert_param(IS_UART3_AUTOSYNC_OK(UART3_Autosync));

    assert_param(IS_UART3_DIVUP_OK(UART3_DivUp));

    if (UART3_Mode != UART3_LIN_MODE_MASTER)
    {
        UART3->CR6 |=  UART3_CR6_LSLV;
    }
    else
    {
        UART3->CR6 &= ((u8)~UART3_CR6_LSLV);
    }

    if (UART3_Autosync != UART3_LIN_AUTOSYNC_DISABLE)
    {
        UART3->CR6 |=  UART3_CR6_LASE ;
    }
    else
    {
        UART3->CR6 &= ((u8)~ UART3_CR6_LASE );
    }

    if (UART3_DivUp != UART3_LIN_DIVUP_LBRR1)
    {
        UART3->CR6 |=  UART3_CR6_LDUM;
    }
    else
    {
        UART3->CR6 &= ((u8)~ UART3_CR6_LDUM);
    }

}

/**
  * @brief Enables or disables the UART3 LIN mode.
  * @par Full description:
  * Enables or disables the UART3’s LIN mode.
  * @param[in] NewState is new state of the UART3 LIN mode.
  * This parameter can be:
  * - ENABLE
  * - DISABLE
  * @retval
  * None
  */
void UART3_LINCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the LIN mode by setting the LINE bit in the CR2 register */
        UART3->CR3 |= UART3_CR3_LINEN;
    }
    else
    {
        /* Disable the LIN mode by clearing the LINE bit in the CR2 register */
        UART3->CR3 &= ((u8)~UART3_CR3_LINEN);
    }
}

/**
  * @brief Selects the UART3 WakeUp method.
  * @par Full description:
  * Selects the UART3 WakeUp method.
  * @param[in] UART3_WakeUp: specifies the UART3 wakeup method.
  * This parameter can be any of the @ref UART3_WakeUp_TypeDef values.
  * @retval
  * None
  */
void UART3_WakeUpConfig(UART3_WakeUp_TypeDef UART3_WakeUp)
{
    assert_param(IS_UART3_WAKEUP_OK(UART3_WakeUp));

    UART3->CR1 &= ((u8)~UART3_CR1_WAKE);
    UART3->CR1 |= (u8)UART3_WakeUp;
}

/**
  * @brief Determines if the UART3 is in mute mode or not.
  * @par Full description:
  * Determines if the UART3 is in mute mode or not.
  * @param[in] NewState: new state of the UART3 mode.
  * This parameter can be:
  * - ENABLE
  * - DISABLE
  * @retval
  * None
  */
void UART3_ReceiverWakeUpCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the mute mode UART3 by setting the RWU bit in the CR2 register */
        UART3->CR2 |= UART3_CR2_RWU;
    }
    else
    {
        /* Disable the mute mode UART3 by clearing the RWU bit in the CR1 register */
        UART3->CR2 &= ((u8)~UART3_CR2_RWU);
    }
}


/**
  * @brief Returns the most recent received data by the UART3 peripheral.
  * @par Full description:
  * Returns the most recent received data by the UART3 peripheral.
  * @retval u16 Received Data
  * @par Required preconditions:
  * UART3_Cmd(ENABLE);
  */
u8 UART3_ReceiveData8(void)
{
    return ((u8)UART3->DR);
}

/**
  * @brief Returns the most recent received data by the UART3 peripheral.
  * @par Full description:
  * Returns the most recent received data by the UART3 peripheral.
  * @retval u16 Received Data
  * @par Required preconditions:
  * UART3_Cmd(ENABLE);
  */
u16 UART3_ReceiveData9(void)
{
    return (u16)((((u16)UART3->DR) | ((u16)(((u16)((u16)UART3->CR1 & (u16)UART3_CR1_R8)) << 1))) & ((u16)0x01FF));
}



/**
  * @brief Transmits 8 bit data through the UART3 peripheral.
  * @par Full description:
  * Transmits 8 bit data through the UART3 peripheral.
  * @param[in] Data: the data to transmit.
  * @retval
  * None
  * @par Required preconditions:
  * UART3_Cmd(ENABLE);
  */
void UART3_SendData8(u8 Data)
{
    /* Transmit Data */
    UART3->DR = Data;
}

/**
  * @brief Transmits 9 bit data through the UART3 peripheral.
  * @par Full description:
  * Transmits 9 bit data through the UART3 peripheral.
  * @param[in] Data: the data to transmit.
  * @retval
  * None
  * @par Required preconditions:
  * UART3_Cmd(ENABLE);
  */
void UART3_SendData9(u16 Data)
{
    UART3->CR1 &= ((u8)~UART3_CR1_T8);                  /**< Clear the transmit data bit 8     */
    UART3->CR1 |= (u8)(((u8)(Data >> 2)) & UART3_CR1_T8); /**< Write the transmit data bit [8]   */
    UART3->DR   = (u8)(Data);                    /**< Write the transmit data bit [0:7] */

}
/**
  * @brief Transmits break characters.
  * @par Full description:
  * Transmits break characters on the UART3 peripheral.
  * @retval
  * None
  */
void UART3_SendBreak(void)
{
    UART3->CR2 |= UART3_CR2_SBK;
}
/**
  * @brief Sets the address of the UART3 node.
  * @par Full description:
  * Sets the address of the UART3 node.
  * @param[in] UART3_Address: Indicates the address of the UART3 node.
  * @retval
  * None
  */

void UART3_SetAddress(u8 UART3_Address)
{
    /*assert_param for x UART3_Address*/
    assert_param(IS_UART3_ADDRESS_OK(UART3_Address));

    /* Clear the UART3 address */
    UART3->CR4 &= ((u8)~UART3_CR4_ADD);
    /* Set the UART3 address node */
    UART3->CR4 |= UART3_Address;
}
/**
  * @brief Checks whether the specified UART3 flag is set or not.
  * @par Full description:
  * Checks whether the specified UART3 flag is set or not.
  * @param[in] UART3_FLAG specifies the flag to check.
  * This parameter can be any of the @ref UART3_Flag_TypeDef enumeration.
  * @retval FlagStatus (SET or RESET)
  */

FlagStatus UART3_GetFlagStatus(UART3_Flag_TypeDef UART3_FLAG)
{
    FlagStatus status = RESET;

    /* Check parameters */
    assert_param(IS_UART3_FLAG_OK(UART3_FLAG));

    /* Check the status of the specified UART3 flag*/
    if (UART3_FLAG == UART3_FLAG_LBDF)
    {
        if ((UART3->CR4 & (u8)UART3_FLAG) != (u8)0x00)
        {
            /* UART3_FLAG is set*/
            status = SET;
        }
        else
        {
            /* UART3_FLAG is reset*/
            status = RESET;
        }
    }
    else if (UART3_FLAG == UART3_FLAG_SBK)
    {
        if ((UART3->CR2 & (u8)UART3_FLAG) != (u8)0x00)
        {
            /* UART3_FLAG is set*/
            status = SET;
        }
        else
        {
            /* UART3_FLAG is reset*/
            status = RESET;
        }
    }
    else if ((UART3_FLAG == UART3_FLAG_LHDF) || (UART3_FLAG == UART3_FLAG_LSF))
    {
        if ((UART3->CR6 & (u8)UART3_FLAG) != (u8)0x00)
        {
            /* UART3_FLAG is set*/
            status = SET;
        }
        else
        {
            /* UART3_FLAG is reset*/
            status = RESET;
        }
    }
    else
    {
        if ((UART3->SR & (u8)UART3_FLAG) != (u8)0x00)
        {
            /* UART3_FLAG is set*/
            status = SET;
        }
        else
        {
            /* UART3_FLAG is reset*/
            status = RESET;
        }
    }

    /* Return the UART3_FLAG status*/
    return  status;
}
/**
  * @brief Clears the UART3 flags.
  * @par Full description:
  * Clears the UART3 flags.
  * @param[in] UART3_FLAG specifies the flag to clear
  * This parameter can be any combination of the following values:
  *   - UART3_FLAG_LBDF: LIN Break detection flag.
  *   - UART3_FLAG_LHDF: LIN Header detection flag.
  *   - UART3_FLAG_LSF: LIN synchrone field flag.
  *   - UART3_FLAG_RXNE: Receive data register not empty flag.
  * @par Notes:
  *   - PE (Parity error), FE (Framing error), NF (Noise error), OR (OverRun error)
  *     and IDLE (Idle line detected) flags are cleared by software sequence: a read
  *     operation to UART3_SR register (UART3_GetFlagStatus())followed by a read operation
  *     to UART3_DR register(UART3_ReceiveData8() or UART3_ReceiveData9()).
  *   - RXNE flag can be also cleared by a read to the UART3_DR register
  *     (UART3_ReceiveData8()or UART3_ReceiveData9()).
  *   - TC flag can be also cleared by software sequence: a read operation to UART3_SR
  *     register (UART3_GetFlagStatus()) followed by a write operation to UART3_DR register
  *     (UART3_SendData8() or UART3_SendData9()).
  *   - TXE flag is cleared only by a write to the UART3_DR register (UART3_SendData8() or
  *     UART3_SendData9()).
  *   - SBK flag is cleared during the stop bit of break.
  * @retval
  * None
  */

void UART3_ClearFlag(UART3_Flag_TypeDef UART3_FLAG)
{
    assert_param(IS_UART3_CLEAR_FLAG_OK(UART3_FLAG));

    /*< Clear the Receive Register Not Empty flag */
    if (UART3_FLAG == UART3_FLAG_RXNE)
    {
        UART3->SR = (u8)~(UART3_SR_RXNE);
    }
    /*< Clear the LIN Break Detection flag */
    else if (UART3_FLAG == UART3_FLAG_LBDF)
    {
        UART3->CR4 &= (u8)(~UART3_CR4_LBDF);
    }
    /*< Clear the LIN Header Detection Flag */
    else if (UART3_FLAG == UART3_FLAG_LHDF)
    {
        UART3->CR6 &= (u8)(~UART3_CR6_LHDF);
    }
    /*< Clear the LIN Synch Field flag */
    else
    {
        UART3->CR6 &= (u8)(~UART3_CR6_LSF);
    }

}

/**
  * @brief Checks whether the specified UART3 interrupt has occurred or not.
  * @par Full description:
  * Checks whether the specified UART3 interrupt has occurred or not.
  * @param[in] UART3_IT: Specifies the UART3 interrupt pending bit to check.
  * This parameter can be one of the following values:
  *   - UART3_IT_LBDF:  LIN Break detection interrupt
  *   - UART3_IT_TXE:  Tansmit Data Register empty interrupt
  *   - UART3_IT_TC:   Transmission complete interrupt
  *   - UART3_IT_RXNE: Receive Data register not empty interrupt
  *   - UART3_IT_IDLE: Idle line detection interrupt
  *   - UART3_IT_OR:  OverRun Error interrupt
  *   - UART3_IT_PE:   Parity Error interrupt
  * @retval
  * ITStatus The new state of UART3_IT (SET or RESET).
  */
ITStatus UART3_GetITStatus(UART3_IT_TypeDef UART3_IT)
{
    ITStatus pendingbitstatus = RESET;
    u8 itpos = 0;
    u8 itmask1 = 0;
    u8 itmask2 = 0;
    u8 enablestatus = 0;

    /* Check parameters */
    assert_param(IS_UART3_GET_IT_OK(UART3_IT));

    /* Get the UART3 IT index*/
    itpos = (u8)((u8)1 << (u8)((u8)UART3_IT & (u8)0x0F));
    /* Get the UART3 IT index*/
    itmask1 = (u8)((u8)UART3_IT >> (u8)4);
    /* Set the IT mask*/
    itmask2 = (u8)((u8)1 << itmask1);



    /* Check the status of the specified UART3 pending bit*/
    if (UART3_IT == UART3_IT_PE)
    {
        /* Get the UART3_ITPENDINGBIT enable bit status*/
        enablestatus = (u8)((u8)UART3->CR1 & itmask2);
        /* Check the status of the specified UART3 interrupt*/

        if (((UART3->SR & itpos) != (u8)0x00) && enablestatus)
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

    else if (UART3_IT == UART3_IT_LBDF)
    {
        /* Get the UART3_IT enable bit status*/
        enablestatus = (u8)((u8)UART3->CR4 & itmask2);
        /* Check the status of the specified UART3 interrupt*/
        if (((UART3->CR4 & itpos) != (u8)0x00) && enablestatus)
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
    else if (UART3_IT == UART3_IT_LHDF)
    {
        /* Get the UART3_IT enable bit status*/
        enablestatus = (u8)((u8)UART3->CR6 & itmask2);
        /* Check the status of the specified UART3 interrupt*/
        if (((UART3->CR6 & itpos) != (u8)0x00) && enablestatus)
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
        /* Get the UART3_IT enable bit status*/
        enablestatus = (u8)((u8)UART3->CR2 & itmask2);
        /* Check the status of the specified UART3 interrupt*/
        if (((UART3->SR & itpos) != (u8)0x00) && enablestatus)
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
    /* Return the UART3_IT status*/
    return  pendingbitstatus;
}

/**
  * @brief Clears the UART3 pending flags.
  * @par Full description:
  * Clears the UART3 pending bit.
  * @param[in] UART3_IT specifies the pending bit to clear
  * This parameter can be one of the following values:
  *   - UART3_IT_LBDF:  LIN Break detection interrupt
  *   - UART3_IT_LHDF:  LIN Header detection interrupt
  *   - UART3_IT_RXNE: Receive Data register not empty interrupt.
  *
  * @par Notes:
  *   - PE (Parity error), FE (Framing error), NF (Noise error), OR (OverRun error) and
  *     IDLE (Idle line detected) pending bits are cleared by software sequence: a read
  *     operation to UART3_SR register (UART3_GetITStatus()) followed by a read operation
  *     to UART3_DR register (UART3_ReceiveData8() or UART3_ReceiveData9() ).
  *   - RXNE pending bit can be also cleared by a read to the UART3_DR register
  *     (UART3_ReceiveData8() or UART3_ReceiveData9() ).
  *   - TC (Transmit complet) pending bit can be cleared by software sequence: a read
  *     operation to UART3_SR register (UART3_GetITStatus()) followed by a write operation
  *     to UART3_DR register (UART3_SendData8()or UART3_SendData9()).
  *   - TXE pending bit is cleared only by a write to the UART3_DR register
  *     (UART3_SendData8() or UART3_SendData9()).
  * @retval
  * None
  */
void UART3_ClearITPendingBit(UART3_IT_TypeDef UART3_IT)
{
    assert_param(IS_UART3_CLEAR_IT_OK(UART3_IT));

    /*< Clear the Receive Register Not Empty pending bit */
    if (UART3_IT == UART3_IT_RXNE)
    {
        UART3->SR = (u8)~(UART3_SR_RXNE);
    }
    /*< Clear the LIN Break Detection pending bit */
    else if (UART3_IT == UART3_IT_LBDF)
    {
        UART3->CR4 &= (u8)~(UART3_CR4_LBDF);
    }
    /*< Clear the LIN Header Detection pending bit */
    else
    {
        UART3->CR6 &= (u8)(~UART3_CR6_LHDF);
    }
}
/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
