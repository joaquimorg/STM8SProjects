/**
  ******************************************************************************
  * @file stm8s_flash.c
  * @brief This file contains all the functions for the FLASH peripheral.
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
#include "stm8s_flash.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLASH_CLEAR_BYTE (u8)0x00;
#define FLASH_SET_BYTE  (u8)0xFF;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define  OPERATION_TIMEOUT  ((u16)0x1000)
/* Private function prototypes -----------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/

/** @addtogroup FLASH_Public_functions
  * @{
  */

/**
  * @brief Unlocks the program or data EEPROM memory
  * @param[in] MemType Memory type to unlock
  * This parameter can be any of the @ref FLASH_MemType_TypeDef values.
  * @retval None
  */
void FLASH_Unlock(FLASH_MemType_TypeDef MemType)
{
    /* Check parameter */
    assert_param(IS_MEMORY_TYPE_OK(MemType));

    /* Unlock program memory */
    if (MemType == FLASH_MEMTYPE_PROG)
    {
        FLASH->PUKR = FLASH_RASS_KEY1;
        FLASH->PUKR = FLASH_RASS_KEY2;
    }

    /* Unlock data memory */
    else
    {
        FLASH->DUKR = FLASH_RASS_KEY2; /* Warning: keys are reversed on data memory !!! */
        FLASH->DUKR = FLASH_RASS_KEY1;
    }
}

/**
  * @brief Locks the program or data EEPROM memory
  * @param[in] MemType Memory type
  * @retval
  * None.
  */
void FLASH_Lock(FLASH_MemType_TypeDef MemType)
{
    /* Check parameter */
    assert_param(IS_MEMORY_TYPE_OK(MemType));

    /* Lock program memory */
    if (MemType == FLASH_MEMTYPE_PROG)
    {
        FLASH->IAPSR = (u8)(~FLASH_IAPSR_PUL);
    }

    /* Lock data memory */
    else
    {
        FLASH->IAPSR = (u8)(~FLASH_IAPSR_DUL);
    }
}

/**
  * @brief Deinitializes the FLASH peripheral registers to their default reset values.
  * @par Parameters:
  * None.
  * @retval None
  */
void FLASH_DeInit(void)
{
    u8 temp = 0;
    FLASH->CR1 = FLASH_CR1_RESET_VALUE;
    FLASH->CR2 = FLASH_CR2_RESET_VALUE;
    FLASH->NCR2 = FLASH_NCR2_RESET_VALUE;
    FLASH->IAPSR &= (u8)(~FLASH_IAPSR_DUL);
    FLASH->IAPSR &= (u8)(~FLASH_IAPSR_PUL);
    temp = FLASH->IAPSR; /* Reading of this register causes the clearing of status flags */
}

/**
  * @brief Enables or Disables the Flash interrupt mode
  * @param[in] NewState The new state of the flash interrupt mode
  * @retval None
  */
void FLASH_ITConfig(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        FLASH->CR1 |= FLASH_CR1_IE; /* Enables the interrupt sources */
    }
    else
    {
        FLASH->CR1 &= (u8)(~FLASH_CR1_IE); /* Disables the interrupt sources */
    }
}

/**
  * @brief Erases one byte in the program or data EEPROM memory
  * @param[in] Address Address of the byte to erase
  * @retval
  * None.
  * @par Required preconditions:
	* PointerAttr define is declared in the stm8s.h file to select if pointer will be declared
  *	as near (2 bytes) or far (3 bytes).
*/
void FLASH_EraseByte(u32 Address)
{
    /* Check parameter */
    assert_param(IS_FLASH_ADDRESS_OK(Address));
    *((PointerAttr u8*) Address) = FLASH_CLEAR_BYTE; /* Erase byte */

}

/**
  * @brief Programs one byte in program or data EEPROM memory
  * @param[in] Address Adress where the byte is written
  * @param[in] Data Value to be written
  * @retval
  * None.
  * @par Required preconditions:
	* PointerAttr define is declared in the stm8s.h file to select if pointer will be declared
  *	as near (2 bytes) or far (3 bytes).
  */
void FLASH_ProgramByte(u32 Address, u8 Data)
{
    /* Check parameters */
    assert_param(IS_FLASH_ADDRESS_OK(Address));
    *((PointerAttr u8*) Address) = Data;
}

/**
  * @brief Reads any byte from flash memory
  * @param[in] Address Address to read
  * @retval u8 Value read
  * @par Required preconditions:
	* PointerAttr define is declared in the stm8s.h file to select if pointer will be declared
  *	as near (2 bytes) or far (3 bytes).
  */
u8 FLASH_ReadByte(u32 Address)
{
    /* Check parameter */
    assert_param(IS_FLASH_ADDRESS_OK(Address));
    return(*((PointerAttr u8*) Address)); /* Read byte */

}
/**
  * @brief Programs one word (4 bytes) in program or data EEPROM memory
  * @param[in] Address Adress where the byte is written
  * @param[in] Data Value to be written
  * @retval
  * None.
  * @par Required preconditions:
	* PointerAttr define is declared in the stm8s.h file to select if pointer will be declared
  *	as near (2 bytes) or far (3 bytes).
  */
void FLASH_ProgramWord(u32 Address, u32 Data)
{
    /* Check parameters */
    assert_param(IS_FLASH_ADDRESS_OK(Address));

    /* Enable Word Write Once */
    FLASH->CR2 |= FLASH_CR2_WPRG;
    FLASH->NCR2 &= (u8)(~FLASH_NCR2_NWPRG);

    *((PointerAttr u8*)Address)       = *((u8*)(&Data)); /* Write one byte - from lowest address*/
    *(((PointerAttr u8*)Address) + 1) = *((u8*)(&Data)+1); /* Write one byte*/
    *(((PointerAttr u8*)Address) + 2) = *((u8*)(&Data)+2); /* Write one byte*/
    *(((PointerAttr u8*)Address) + 3) = *((u8*)(&Data)+3); /* Write one byte - from higher address*/
}

/**
  * @brief Programs an option byte
  * @param[in] Address  option byte address  to program
  * @param[in] Data Value to write
  * @retval
  * None
  */
void FLASH_ProgramOptionByte(u16 Address, u8 Data)
{
    /* Check parameter */
    assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));

    /* Enable write access to option bytes */
    FLASH->CR2 |= FLASH_CR2_OPT;
    FLASH->NCR2 &= (u8)(~FLASH_NCR2_NOPT);

    /* Program option byte and his complement */
    *((NEAR u8*)Address) = Data;
    *((NEAR u8*)(Address + 1)) = (u8)(~Data);

    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);

    /* Disable write access to option bytes */
    FLASH->CR2 &= (u8)(~FLASH_CR2_OPT);
    FLASH->NCR2 |= FLASH_NCR2_NOPT;
}

/**
  * @brief Erases an option byte
  * @param[in] Address Option byte address to erase
  * @retval
  * None.
  */
void FLASH_EraseOptionByte(u16 Address)
{
    /* Check parameter */
    assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));

    /* Enable write access to option bytes */
    FLASH->CR2 |= FLASH_CR2_OPT;
    FLASH->NCR2 &= (u8)(~FLASH_NCR2_NOPT);

    /* Erase option byte and his complement */
    *((NEAR u8*)Address) = FLASH_CLEAR_BYTE;
    *((NEAR u8*)(Address + 1 )) = FLASH_SET_BYTE;

    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);

    /* Disable write access to option bytes */
    FLASH->CR2 &= (u8)(~FLASH_CR2_OPT);
    FLASH->NCR2 |= FLASH_NCR2_NOPT;
}
/**
  * @brief Reads one option byte
  * @param[in] Address  option byte address to read.
  * @retval u16 res_value (Value read + complement value read.)
  */
u16 FLASH_ReadOptionByte(u16 Address)
{
    u8 value_optbyte, value_optbyte_complement = 0;
    u16 res_value = 0;

    /* Check parameter */
    assert_param(IS_OPTION_BYTE_ADDRESS_OK(Address));


    value_optbyte = *((NEAR u8*)Address); /* Read option byte */
    value_optbyte_complement = *(((NEAR u8*)Address) + 1); /* Read option byte complement*/

    if (value_optbyte == (u8)(~value_optbyte_complement))
    {
        res_value = (u16)((u16)value_optbyte << 8);
        res_value = res_value | (u16)value_optbyte_complement;
    }
    else
    {
        res_value = FLASH_OPTIONBYTE_ERROR;
    }

    return(res_value);
}

/**
  * @brief Select the Flash behaviour in low power mode
  * @param[in] LPMode Low power mode selection
  * This parameter can be any of the @ref FLASH_LPMode_TypeDef values.
  * @retval None
  */
void FLASH_SetLowPowerMode(FLASH_LPMode_TypeDef LPMode)
{
    /* Check parameter */
    assert_param(IS_FLASH_LOW_POWER_MODE_OK(LPMode));

    FLASH->CR1 &= (u8)(~(FLASH_CR1_HALT | FLASH_CR1_AHALT)); /* Clears the two bits */
    FLASH->CR1 |= (u8)LPMode; /* Sets the new mode */
}

/**
  * @brief Sets the fixed programming time
  * @param[in] ProgTime Indicates the programming time to be fixed
  * This parameter can be any of the @ref FLASH_ProgramTime_TypeDef values.
  * @retval None
  */
void FLASH_SetProgrammingTime(FLASH_ProgramTime_TypeDef ProgTime)
{
    /* Check parameter */
    assert_param(IS_FLASH_PROGRAM_TIME_OK(ProgTime));

    FLASH->CR1 &= (u8)(~FLASH_CR1_FIX);
    FLASH->CR1 |= (u8)ProgTime;
}

/**
  * @brief Returns the Flash behaviour type in low power mode
  * @par Parameters:
  * None
  * @retval FLASH_LPMode_TypeDef Flash behaviour type in low power mode
  */
FLASH_LPMode_TypeDef FLASH_GetLowPowerMode(void)
{
    return((FLASH_LPMode_TypeDef)(FLASH->CR1 & (FLASH_CR1_HALT | FLASH_CR1_AHALT)));
}

/**
  * @brief Returns the fixed programming time
  * @par Parameters:
  * None
  * @retval FLASH_ProgramTime_TypeDef Fixed programming time value
  */
FLASH_ProgramTime_TypeDef FLASH_GetProgrammingTime(void)
{
    return((FLASH_ProgramTime_TypeDef)(FLASH->CR1 & FLASH_CR1_FIX));
}

/**
  * @brief Returns the Boot memory size in bytes
  * @par Parameters:
  * None
  * @retval u32 Boot memory size in bytes
  */
u32 FLASH_GetBootSize(void)
{
    u32 temp = 0;

    /* Calculates the number of bytes */
    temp = (u32)((u32)FLASH->FPR * (u32)512);

    /* Correction because size of 127.5 kb doesn't exist */
    if (FLASH->FPR == 0xFF)
    {
        temp += 512;
    }

    /* Return value */
    return(temp);
}

/**
  * @brief Checks whether the specified SPI flag is set or not.
  * @param[in] FLASH_FLAG : Specifies the flag to check.
  * This parameter can be any of the @ref FLASH_Flag_TypeDef enumeration.
  * @retval FlagStatus : Indicates the state of FLASH_FLAG.
  * This parameter can be any of the @ref FlagStatus enumeration.
  * @par Required preconditions:
  * This function clears the EOP, WR_PG_DIS flags in the IAPSR register.
  */
FlagStatus FLASH_GetFlagStatus(FLASH_Flag_TypeDef FLASH_FLAG)
{
    FlagStatus status = RESET;
    /* Check parameters */
    assert_param(IS_FLASH_FLAGS_OK(FLASH_FLAG));

    /* Check the status of the specified FLASH flag */
    if ((FLASH->IAPSR & (u8)FLASH_FLAG) != (u8)RESET)
    {
        status = SET; /* FLASH_FLAG is set */
    }
    else
    {
        status = RESET; /* FLASH_FLAG is reset*/
    }

    /* Return the FLASH_FLAG status */
    return status;
}
/**
  * @brief Wait for a Flash operation to complete.
  * @par Parameters:
  * @param[in] MemType Memory type
  * @retval FLASH_Status_TypeDef State of the last operation
  * @par Required preconditions:
  * The FLASH_WaitForLastOperation function should be executed from RAM.
  * This function return the FLASH status and clears the EOP, WR_PG_DIS flags in the IAPSR register.
  */
FLASH_Status_TypeDef FLASH_WaitForLastOperation(FLASH_MemType_TypeDef MemType)
{
    u8 flagstatus = 0x00;
    u16 timeout = OPERATION_TIMEOUT;
    /* Wait until operation completion or write protected page occured */
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)
    if (MemType == FLASH_MEMTYPE_PROG)
    {
        while ((flagstatus == 0x00) && (timeout != 0x00))
        {
            flagstatus = (u8)(FLASH->IAPSR & (FLASH_IAPSR_EOP |
                                              FLASH_IAPSR_WR_PG_DIS));
            timeout--;
        }
    }
    else
    {
        while ((flagstatus == 0x00) && (timeout != 0x00))
        {
            flagstatus = (u8)(FLASH->IAPSR & (FLASH_IAPSR_HVOFF |
                                              FLASH_IAPSR_WR_PG_DIS));
            timeout--;
        }
    }
#else /*STM8S103, STM8S903*/
    while ((flagstatus == 0x00) && (timeout != 0x00))
    {
        flagstatus = (u8)(FLASH->IAPSR & (FLASH_IAPSR_EOP |                                            FLASH_IAPSR_WR_PG_DIS));
        timeout--;
    }

#endif /* STM8S208, STM8S207, STM8S105 */
    if (timeout == 0x00 )
    {
        flagstatus = FLASH_STATUS_TIMEOUT;
    }

    return((FLASH_Status_TypeDef)flagstatus);
}
/* *****************************************************************************************/
/*                    Uncomment the line below to use these functions 	                   */
/*                                                                                         */
/* *****************************************************************************************/
/* LINKER SECTIONS DEFINITION FOR THIS FILE ONLY */
#ifdef USE_COSMIC_SECTIONS
#pragma section (FLASH_CODE)
#pragma section const {FLASH_CONST}
#pragma section @near [FLASH_URAM]
#pragma section @near {FLASH_IRAM}
#pragma section @tiny [FLASH_UZRAM]
#pragma section @tiny {FLASH_IZRAM}
#endif

/**
  * @brief Erases a block in the program or data memory.
  * @param[in] MemType Memory type
  * @param[in] BlockNum Indicates the block number to erase
  * @retval
  * None.
  * @par Required preconditions:
  * The FLASH_EraseBlock function should be executed from RAM.
	* PointerAttr define is declared in the stm8s.h file to select if pointer will be declared
  *	as near (2 bytes) or far (3 bytes).
  */
void FLASH_EraseBlock(u16 BlockNum, FLASH_MemType_TypeDef MemType)
{
    u16 timeout = OPERATION_TIMEOUT;
#ifdef PointerAttr_Far
    u32 StartAddress = 0;
#else /* PointerAttr_Near */
    PointerAttr u32 *pwFlash;
    u32 StartAddress = 0;
#endif /*PointerAttr_Far*/

    /* Check parameters */
    assert_param(IS_MEMORY_TYPE_OK(MemType));
    if (MemType == FLASH_MEMTYPE_PROG)
    {
        assert_param(IS_FLASH_PROG_BLOCK_NUMBER_OK(BlockNum));
        StartAddress = FLASH_PROG_START_PHYSICAL_ADDRESS;
    }
    else
    {
        assert_param(IS_FLASH_DATA_BLOCK_NUMBER_OK(BlockNum));
        StartAddress = FLASH_DATA_START_PHYSICAL_ADDRESS;
    }
#ifdef PointerAttr_Far
    /* Point to the first block address */
    StartAddress = StartAddress + ((u32)BlockNum * FLASH_BLOCK_SIZE);

    /* Enable erase block mode */
    FLASH->CR2 |= FLASH_CR2_ERASE;
    FLASH->NCR2 &= (u8)(~FLASH_NCR2_NERASE);

    *((PointerAttr u8*) StartAddress) = FLASH_CLEAR_BYTE;
    *((PointerAttr u8*) StartAddress + 1) = FLASH_CLEAR_BYTE;
    *((PointerAttr u8*) StartAddress + 2) = FLASH_CLEAR_BYTE;
    *((PointerAttr u8*) StartAddress + 3) = FLASH_CLEAR_BYTE;

#else /*PointerAttr_Near*/

    /* Point to the first block address */
    pwFlash = (PointerAttr u32 *)(StartAddress + ((u32)BlockNum * FLASH_BLOCK_SIZE));
    /* Enable erase block mode */
    FLASH->CR2 |= FLASH_CR2_ERASE;
    FLASH->NCR2 &= (u8)(~FLASH_NCR2_NERASE);

    *pwFlash = (u32)0;
#endif	/*PointerAttr_Far*/

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)
    /* Waiting until High voltage flag is cleared*/
    while ((FLASH->IAPSR & FLASH_IAPSR_HVOFF) != 0x00 || (timeout == 0x00))
    {
        timeout--;
    }
#endif /* STM8S208, STM8S207, STM8S105 */
}

/**
  *@brief Programs a memory block
  * @param[in] MemType The type of memory to program
  * @param[in] BlockNum The block number
  * @param[in] ProgMode The programming mode.
  * @param[in] Buffer The buffer address of source data.
  * @retval
  * None.
  * @par Required preconditions:
  * The FLASH_ProgramBlock function should be executed from RAM.
	* PointerAttr define is declared in the stm8s.h file to select if pointer will be declared
  *	as near (2 bytes) or far (3 bytes).
  */
void FLASH_ProgramBlock(u16 BlockNum, FLASH_MemType_TypeDef MemType, FLASH_ProgramMode_TypeDef ProgMode, u8 *Buffer)
{
    u16 Count = 0;
    u32 StartAddress = 0;
    u16 timeout = OPERATION_TIMEOUT;
    /* Check parameters */
    assert_param(IS_MEMORY_TYPE_OK(MemType));
    assert_param(IS_FLASH_PROGRAM_MODE_OK(ProgMode));
    if (MemType == FLASH_MEMTYPE_PROG)
    {
        assert_param(IS_FLASH_PROG_BLOCK_NUMBER_OK(BlockNum));
        StartAddress = FLASH_PROG_START_PHYSICAL_ADDRESS;
    }
    else
    {
        assert_param(IS_FLASH_DATA_BLOCK_NUMBER_OK(BlockNum));
        StartAddress = FLASH_DATA_START_PHYSICAL_ADDRESS;
    }

    /* Point to the first block address */
    StartAddress = StartAddress + ((u32)BlockNum * FLASH_BLOCK_SIZE);

    /* Selection of Standard or Fast programming mode */
    if (ProgMode == FLASH_PROGRAMMODE_STANDARD)
    {
        /* Standard programming mode */ /*No need in standard mode */
        FLASH->CR2 |= FLASH_CR2_PRG;
        FLASH->NCR2 &= (u8)(~FLASH_NCR2_NPRG);
    }
    else
    {
        /* Fast programming mode */
        FLASH->CR2 |= FLASH_CR2_FPRG;
        FLASH->NCR2 &= (u8)(~FLASH_NCR2_NFPRG);
    }

    /* Copy data bytes from RAM to FLASH memory */
    for (Count = 0; Count < FLASH_BLOCK_SIZE; Count++)
    {
        *((PointerAttr u8*)StartAddress + Count) = ((u8)(Buffer[Count]));
    }

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)
    if (MemType == FLASH_MEMTYPE_DATA)
    {
        /* Waiting until High voltage flag is cleared*/
        while ((FLASH->IAPSR & FLASH_IAPSR_HVOFF) != 0x00 || (timeout == 0x00))
        {
            timeout--;
        }
    }
#endif /* STM8S208, STM8S207, STM8S105 */
}
/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
