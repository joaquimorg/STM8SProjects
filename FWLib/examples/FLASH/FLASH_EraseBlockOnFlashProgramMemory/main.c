/**
  ******************************************************************************
  * @file FLASH_EraseBlockOnFlashProgramMemory\main.c
  * @brief This file contains the main function for FLASH erase block example.
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
  * <h2><center>&copy; COPYRIGHT 2008 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/**
  * @addtogroup FLASH_EraseBlockOnFlashProgramMemory
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
  * @brief How to Write byte / Write word / Erase a block on FLASH memory.
  * @par Example description
  * - Program 32/64 (FLASH_BLOCK_SIZE/2) bytes from flash program memory end physical address minus FLASH_BLOCK_SIZE up to 
	*   flash program memory end physical address minus FLASH_BLOCK_SIZE/2.
  * - Check programmed bytes
  * - Program 8/16 (FLASH_BLOCK_SIZE/8) words from flash program memory end physical address minus FLASH_BLOCK_SIZE/2 up to 
	*   flash program memory end physical address.
  * - Checked programmed words
  * - Erase the last block in flash program memory
  * - Check erase
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
    u32 add, start_add, stop_add, new_val2 =0;
    u8 new_val1, val =0;
    u16 block =0;
    u8 i =0;

    _fctcpy('b');

    /* Define flash programming Time */
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

    /* Unlock Program memory */
    FLASH_Unlock(FLASH_MEMTYPE_PROG);
    /* Program FLASH_BLOCK_SIZE/2 bytes & check */
    start_add = FLASH_PROG_END_PHYSICAL_ADDRESS - FLASH_BLOCK_SIZE +1;
    stop_add = start_add + FLASH_BLOCK_SIZE/2;
    new_val1 = 0x55;
    for (add = start_add; add < stop_add; add++)
            {
                FLASH_ProgramByte(add, new_val1);
                if (FLASH_ReadByte(add) != new_val1)
                {
                    /* Error */
                    OperationStatus = FAILED;
                    /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
                    /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
                    while (1);
                }
            }

    /* Program FLASH_BLOCK_SIZE/8 words & check */
    start_add = stop_add;
    stop_add = FLASH_PROG_END_PHYSICAL_ADDRESS;
    new_val2 = 0x12345678;
    add = start_add;
    for (i = 0; i < FLASH_BLOCK_SIZE/8; i++)
    {
        FLASH_ProgramWord(add, new_val2);
        add = add + 4;
    }

    /* check */
    for (add = start_add; add < stop_add; add++)
            {

                val = FLASH_ReadByte(add);
                if (val != BYTE_3(new_val2))
                {
                    /* Error */
                    OperationStatus = FAILED;
                    /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
                    /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
                    while (1);
                }

                add += 1;
                val = FLASH_ReadByte(add);
                if (val != BYTE_2(new_val2))
                {
                    /* Error */
                    OperationStatus = FAILED;
                    /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
                    /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
                    while (1);
                }

                add += 1;
                val = FLASH_ReadByte(add);
                if (val != BYTE_1(new_val2))
                {
                    /* Error */
                    OperationStatus = FAILED;
                    /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
                    /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
                    while (1);
                }

                add += 1;
                val = FLASH_ReadByte(add);
                if (val != BYTE_0(new_val2))
                {
                    /* Error */
                    OperationStatus = FAILED;
                    /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
                    /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
                    while (1);
                }
            }

    /* Erase the last block & check */
    block = FLASH_PROG_BLOCKS_NUMBER -1; /* Last block of Flash program memory */
    start_add = FLASH_PROG_END_PHYSICAL_ADDRESS - FLASH_BLOCK_SIZE +1;
    stop_add = FLASH_PROG_END_PHYSICAL_ADDRESS;
		/* Without calling the FLASH_WaitForLastOperation function */
		/* When the program goes back to Flash program memory, it is stalled untill the block erase operation is complete.*/
    FLASH_EraseBlock(block, FLASH_MEMTYPE_PROG);

    for (add = start_add; add < stop_add; add++)
            {
                if (FLASH_ReadByte(add) != 0x00)
                {
                    /* Error */
                    OperationStatus = FAILED;
                    /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
                    /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
                    while (1);
                }
            }

    /* Pass */
    OperationStatus = PASSED;
    /* OperationStatus = PASSED, if the data written/read to/from FLASH Program memory is correct */
    /* OperationStatus = FAILED, if the data written/read to/from FLASH Program memory is corrupted */
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
