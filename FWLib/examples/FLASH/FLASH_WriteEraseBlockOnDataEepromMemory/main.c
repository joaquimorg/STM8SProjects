/**
  ******************************************************************************
  * @file FLASH_WriteEraseBlockOnDataEepromMemory\main.c
  * @brief This file contains the main function for FLASH write erase block example.
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
#include "stm8s.h"

/**
  * @addtogroup FLASH_WriteEraseBlockOnDataEepromMemory
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
TestStatus OperationStatus;
/* Private function prototypes -----------------------------------------------*/
int _fctcpy(char name);
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief How to Write / Erase a block on Data EEPROM memory.
  * @par Examples description
  * - Program a block at address 0x4000
  * - Check program using Read byte
  * - Erase block
  * - Check erase with Read byte function.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
    u32 add, start_add, stop_add =0;
    u8 GBuffer[FLASH_BLOCK_SIZE];
    u8 new_val2 = 0xAA;
    u8 block, i =0;

    _fctcpy('b');

    /* Define flash programming Time*/
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

    /* Unlock Data memory */
    FLASH_Unlock(FLASH_MEMTYPE_DATA);

    /* Fill the buffer in RAM */
    for (i = 0; i < FLASH_BLOCK_SIZE; i++) GBuffer[i] = new_val2;

    /* Program the block 0*/
    block = 0; /* block 0 is first block of Data memory: address is 0x4000 */
    FLASH_ProgramBlock(block, FLASH_MEMTYPE_DATA, FLASH_PROGRAMMODE_STANDARD, GBuffer);
		FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);

    /* Check the programmed block */
    start_add = FLASH_DATA_START_PHYSICAL_ADDRESS;
    stop_add = FLASH_DATA_START_PHYSICAL_ADDRESS + (u32)FLASH_BLOCK_SIZE;
    for (add = start_add; add < stop_add; add++)
            {
                if (FLASH_ReadByte(add) != new_val2)
                {
                    /* Error */
                    OperationStatus = FAILED;
                    /* OperationStatus = PASSED, if the data written/read to/from DATA EEPROM memory is correct */
                    /* OperationStatus = FAILED, if the data written/read to/from DATA EEPROM memory is corrupted */
                    while (1);
                }
            }
    /* Erase block 0 and verify it */
    FLASH_EraseBlock(block, FLASH_MEMTYPE_DATA);
		FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);

    for (add = start_add; add < stop_add; add++)
            {
                if (FLASH_ReadByte(add) != 0x00)
                {
                    /* Error */
                    OperationStatus = FAILED;
                    /* OperationStatus = PASSED, if the data written/read to/from DATA EEPROM memory is correct */
                    /* OperationStatus = FAILED, if the data written/read to/from DATA EEPROM memory is corrupted */
                    while (1);
                }
            }

    /* Pass */
    OperationStatus = PASSED;
    /* OperationStatus = PASSED, if the data written/read to/from DATA EEPROM memory is correct */
    /* OperationStatus = FAILED, if the data written/read to/from DATA EEPROM memory is corrupted */
    while (1);

}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval 
  * None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
