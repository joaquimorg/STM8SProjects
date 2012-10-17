/**
  ******************************************************************************
  * @file stm8s_flash.h
  * @brief This file contains all functions prototype and macros for the FLASH peripheral.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_FLASH_H
#define __STM8S_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported constants --------------------------------------------------------*/

/** @addtogroup FLASH_Exported_Constants
  * @{
  */

#define FLASH_PROG_START_PHYSICAL_ADDRESS ((u32)0x008000) /*!< Program memory: start address */

#if defined (STM8S208) || defined(STM8S207)
 #define FLASH_PROG_END_PHYSICAL_ADDRESS   ((u32)0x027FFF) /*!< Program memory: end address */
 #define FLASH_PROG_BLOCKS_NUMBER          ((u16)1024)     /*!< Program memory: total number of blocks */
 #define FLASH_DATA_START_PHYSICAL_ADDRESS ((u32)0x004000) /*!< Data EEPROM memory: start address */
 #define FLASH_DATA_END_PHYSICAL_ADDRESS   ((u32)0x0047FF) /*!< Data EEPROM memory: end address */
 #define FLASH_DATA_BLOCKS_NUMBER          ((u16)16)       /*!< Data EEPROM memory: total number of blocks */
 #define FLASH_BLOCK_SIZE                  ((u8)128)       /*!< Number of bytes in a block (common for Program and Data memories) */
#endif /* STM8S208, STM8S207 */

#ifdef STM8S105
 #define FLASH_PROG_END_PHYSICAL_ADDRESS   ((u32)0xFFFF) /*!< Program memory: end address */
 #define FLASH_PROG_BLOCKS_NUMBER          ((u16)256)     /*!< Program memory: total number of blocks */
 #define FLASH_DATA_START_PHYSICAL_ADDRESS ((u32)0x004000) /*!< Data EEPROM memory: start address */
 #define FLASH_DATA_END_PHYSICAL_ADDRESS   ((u32)0x0043FF) /*!< Data EEPROM memory: end address */
 #define FLASH_DATA_BLOCKS_NUMBER          ((u16)8)       /*!< Data EEPROM memory: total number of blocks */
 #define FLASH_BLOCK_SIZE                  ((u8)128)       /*!< Number of bytes in a block (common for Program and Data memories) */
#endif /* STM8S105 */

#if defined (STM8S103) || defined(STM8S903)
 #define FLASH_PROG_END_PHYSICAL_ADDRESS   ((u32)0x9FFF) /*!< Program memory: end address */
 #define FLASH_PROG_BLOCKS_NUMBER          ((u16)128)     /*!< Program memory: total number of blocks */
 #define FLASH_DATA_START_PHYSICAL_ADDRESS ((u32)0x004000) /*!< Data EEPROM memory: start address */
 #define FLASH_DATA_END_PHYSICAL_ADDRESS   ((u32)0x00427F) /*!< Data EEPROM memory: end address */
 #define FLASH_DATA_BLOCKS_NUMBER          ((u16)10)       /*!< Data EEPROM memory: total number of blocks */
 #define FLASH_BLOCK_SIZE                  ((u8)64)       /*!< Number of bytes in a block (common for Program and Data memories) */
#endif /* STM8S103, STM8S903 */

#define FLASH_RASS_KEY1 ((u8)0x56) /*!< First RASS key */
#define FLASH_RASS_KEY2 ((u8)0xAE) /*!< Second RASS key */

#define OPTION_BYTE_START_PHYSICAL_ADDRESS  ((u16)0x4800)
#define OPTION_BYTE_END_PHYSICAL_ADDRESS ((u16)0x487F)
#define FLASH_OPTIONBYTE_ERROR ((u16)0x5555) /*!< Error code option byte (if value read is not equal to complement value read) */
/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/

/** @addtogroup FLASH_Exported_Types
  * @{
  */

/**
  * @brief FLASH Memory types
  */
typedef enum {
    FLASH_MEMTYPE_PROG      = (u8)0x00, /*!< Program memory */
    FLASH_MEMTYPE_DATA      = (u8)0x01 /*!< Data EEPROM memory */
} FLASH_MemType_TypeDef;

/**
  * @brief FLASH programming modes
  */
typedef enum {
    FLASH_PROGRAMMODE_STANDARD = (u8)0x00, /*!< Standard programming mode */
    FLASH_PROGRAMMODE_FAST     = (u8)0x10  /*!< Fast programming mode */
} FLASH_ProgramMode_TypeDef;

/**
  * @brief FLASH fixed programming time
  */
typedef enum {
    FLASH_PROGRAMTIME_STANDARD = (u8)0x00, /*!< Standard programming time fixed at 1/2 tprog */
    FLASH_PROGRAMTIME_TPROG    = (u8)0x01  /*!< Programming time fixed at tprog */
} FLASH_ProgramTime_TypeDef;

/**
  * @brief FLASH Low Power mode select
  */
typedef enum {
    FLASH_LPMODE_POWERDOWN         = (u8)0x04, /*!< HALT: Power-Down / ACTIVE-HALT: Power-Down */
    FLASH_LPMODE_STANDBY           = (u8)0x08, /*!< HALT: Standby    / ACTIVE-HALT: Standby */
    FLASH_LPMODE_POWERDOWN_STANDBY = (u8)0x00, /*!< HALT: Power-Down / ACTIVE-HALT: Standby */
    FLASH_LPMODE_STANDBY_POWERDOWN = (u8)0x0C  /*!< HALT: Standby    / ACTIVE-HALT: Power-Down */
}
FLASH_LPMode_TypeDef;

/**
  * @brief FLASH status of the last operation
  */
typedef enum {
	#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)		
		FLASH_STATUS_END_HIGH_VOLTAGE  = (u8)0x40, /*!< End of high voltage */
	#endif /* STM8S208, STM8S207, STM8S105 */
		FLASH_STATUS_SUCCESSFUL_OPERATION   = (u8)0x04, /*!< End of operation flag */
		FLASH_STATUS_TIMEOUT = (u8)0x02, /*!< Time out error */
    FLASH_STATUS_WRITE_PROTECTION_ERROR = (u8)0x01 /*!< Write attempted to protected page */
} FLASH_Status_TypeDef;

/**
  * @brief FLASH flags definition
 * - Warning : FLAG value = mapping position register
  */
typedef enum {
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)
    FLASH_FLAG_HVOFF     = (u8)0x40,     /*!< End of high voltage flag */
#endif /* STM8S208, STM8S207, STM8S105 */
    FLASH_FLAG_DUL       = (u8)0x08,     /*!< Data EEPROM unlocked flag */
    FLASH_FLAG_EOP       = (u8)0x04,     /*!< End of programming (write or erase operation) flag */
    FLASH_FLAG_PUL       = (u8)0x02,     /*!< Flash Program memory unlocked flag */
    FLASH_FLAG_WR_PG_DIS = (u8)0x01      /*!< Write attempted to protected page flag */
} FLASH_Flag_TypeDef;

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/

/**
  * @brief Macros used by the assert function in order to check the different functions parameters.
  * @addtogroup FLASH_Private_Macros
  * @{
  */

/**
  * @brief Macro used by the assert function in order to check the different sensitivity values for the flash program Address
  */

#define IS_FLASH_PROG_ADDRESS_OK(ADDRESS) (((ADDRESS) >= FLASH_PROG_START_PHYSICAL_ADDRESS) && \
    ((ADDRESS) <= FLASH_PROG_END_PHYSICAL_ADDRESS))

/**
  * @brief Macro used by the assert function in order to check the different sensitivity values for the data eeprom Address
  */

#define IS_FLASH_DATA_ADDRESS_OK(ADDRESS) (((ADDRESS) >= FLASH_DATA_START_PHYSICAL_ADDRESS) && \
    ((ADDRESS) <= FLASH_DATA_END_PHYSICAL_ADDRESS))

/**
  * @brief Macro used by the assert function in order to check the different sensitivity values for the data eeprom and flash program Address
  */
#define IS_FLASH_ADDRESS_OK(ADDRESS)((((ADDRESS) >= FLASH_PROG_START_PHYSICAL_ADDRESS) && ((ADDRESS) <= FLASH_PROG_END_PHYSICAL_ADDRESS)) || \
                                     (((ADDRESS) >= FLASH_DATA_START_PHYSICAL_ADDRESS) && ((ADDRESS) <= FLASH_DATA_END_PHYSICAL_ADDRESS)))

/**
  * @brief Macro used by the assert function in order to check the different sensitivity values for the flash program Block number
  */
#define IS_FLASH_PROG_BLOCK_NUMBER_OK(BLOCKNUM) ((BLOCKNUM) < FLASH_PROG_BLOCKS_NUMBER)

/**
  * @brief Macro used by the assert function in order to check the different sensitivity values for the data eeprom Block number
  */
#define IS_FLASH_DATA_BLOCK_NUMBER_OK(BLOCKNUM) ((BLOCKNUM) < FLASH_DATA_BLOCKS_NUMBER)

/**
  * @brief Macro used by the assert function in order to check the different sensitivity values for the flash memory type
  */

#define IS_MEMORY_TYPE_OK(MEMTYPE) (((MEMTYPE) == FLASH_MEMTYPE_PROG) || \
                                    ((MEMTYPE) == FLASH_MEMTYPE_DATA))

/**
  * @brief Macro used by the assert function in order to check the different sensitivity values for the flash program mode
  */

#define IS_FLASH_PROGRAM_MODE_OK(MODE) (((MODE) == FLASH_PROGRAMMODE_STANDARD) || \
                                        ((MODE) == FLASH_PROGRAMMODE_FAST))

/**
  * @brief Macro used by the assert function in order to check the program time mode
  */

#define IS_FLASH_PROGRAM_TIME_OK(TIME) (((TIME) == FLASH_PROGRAMTIME_STANDARD) || \
                                        ((TIME) == FLASH_PROGRAMTIME_TPROG))

/**
  * @brief Macro used by the assert function in order to check the different sensitivity values for the low power mode
  */

#define IS_FLASH_LOW_POWER_MODE_OK(LPMODE) (((LPMODE) == FLASH_LPMODE_POWERDOWN) || \
    ((LPMODE) == FLASH_LPMODE_STANDBY) || \
    ((LPMODE) == FLASH_LPMODE_POWERDOWN_STANDBY) || \
    ((LPMODE) == FLASH_LPMODE_STANDBY_POWERDOWN))

/**
  * @brief Macro used by the assert function in order to check the different sensitivity values for the option bytes Address
  */
#define IS_OPTION_BYTE_ADDRESS_OK(ADDRESS) (((ADDRESS) >= OPTION_BYTE_START_PHYSICAL_ADDRESS) && \
    ((ADDRESS) <= OPTION_BYTE_END_PHYSICAL_ADDRESS))


/**
  * @brief Macro used by the assert function in order to check the different flags values
  */
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)
#define IS_FLASH_FLAGS_OK(FLAG) (((FLAG) == FLASH_FLAG_HVOFF) || \
                                 ((FLAG) == FLASH_FLAG_DUL) || \
                                 ((FLAG) == FLASH_FLAG_EOP) || \
                                 ((FLAG) == FLASH_FLAG_PUL) || \
                                 ((FLAG) == FLASH_FLAG_WR_PG_DIS))
#else /* STM8S103, STM8S903 */
#define IS_FLASH_FLAGS_OK(FLAG) (((FLAG) == FLASH_FLAG_DUL) || \
                                 ((FLAG) == FLASH_FLAG_EOP) || \
                                 ((FLAG) == FLASH_FLAG_PUL) || \
                                 ((FLAG) == FLASH_FLAG_WR_PG_DIS))
#endif /*STM8S208, STM8S207, STM8S105 */
/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

/** @addtogroup FLASH_Exported_Functions
  * @{
  */
void FLASH_Unlock(FLASH_MemType_TypeDef MemType);
void FLASH_Lock(FLASH_MemType_TypeDef MemType);
void FLASH_DeInit(void);
void FLASH_ITConfig(FunctionalState NewState);
void FLASH_EraseByte(u32 Address);
void FLASH_ProgramByte(u32 Address, u8 Data);
u8 FLASH_ReadByte(u32 Address);
void FLASH_ProgramWord(u32 Address, u32 Data);
u16 FLASH_ReadOptionByte(u16 Address);
void FLASH_ProgramOptionByte(u16 Address, u8 Data);
void FLASH_EraseOptionByte(u16 Address);
void FLASH_SetLowPowerMode(FLASH_LPMode_TypeDef LPMode);
void FLASH_SetProgrammingTime(FLASH_ProgramTime_TypeDef ProgTime);
FLASH_LPMode_TypeDef FLASH_GetLowPowerMode(void);
FLASH_ProgramTime_TypeDef FLASH_GetProgrammingTime(void);
u32 FLASH_GetBootSize(void);
FlagStatus FLASH_GetFlagStatus(FLASH_Flag_TypeDef FLASH_FLAG);
/* Function to be executed from RAM ----------------------------------------- */
void FLASH_EraseBlock(u16 BlockNum, FLASH_MemType_TypeDef MemType);
void FLASH_ProgramBlock(u16 BlockNum, FLASH_MemType_TypeDef MemType, FLASH_ProgramMode_TypeDef ProgMode, u8 *Buffer);
FLASH_Status_TypeDef FLASH_WaitForLastOperation(FLASH_MemType_TypeDef MemType);

/**
  * @}
  */

#endif /*__STM8S_FLASH_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
