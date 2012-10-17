/**
  ******************************************************************************
  * @file FLASH_BlockOperationOnFlashProgramMemory\main.c
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
  * @addtogroup FLASH_BlockOperationOnFlashProgramMemory
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
TestStatus OperationStatus;
FLASH_Status_TypeDef Status;
u16 timeout = 0x1000;
/* Private function prototypes -----------------------------------------------*/
int _fctcpy(char name);
FLASH_Status_TypeDef ProgramOperation(u16 BlockNum, FLASH_MemType_TypeDef MemType, FLASH_ProgramMode_TypeDef ProgMode, u8 *Buffer);
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief How continue execution from RAM while program operation on FLASH Program is on going 
  * @par Examples description
  * - Program the last block in the flash program memory
  * - Check program using Read byte
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
    u32 add, start_add, stop_add =0;
    u8 new_val2 = 0xAA;
    u8 i = 0x00;
		u16 block = 0;
		u8 GBuffer[FLASH_BLOCK_SIZE];

 		_fctcpy('b');
		
    /* Define flash programming Time*/
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

    /* Unlock Program memory */
    FLASH_Unlock(FLASH_MEMTYPE_PROG);

    /* Fill the buffer in RAM */
    for (i = 0; i < FLASH_BLOCK_SIZE; i++) GBuffer[i] = new_val2;
    
		/* Program the block 0*/
    block = FLASH_PROG_BLOCKS_NUMBER -1; /* block FLASH_PROG_BLOCKS_NUMBER is last block of Flash Program memory */
		
	/* The FLASH_ProgramOperation and FLASH_WaitForLastOperation are merged in the ProgramOperation function to avoid the fact that
	   the program goes back to Flash program memory, before executing the second function (to avoid that the CPU is stalled until 
	   the block program operation is complete.) */
		
    Status = ProgramOperation(block, FLASH_MEMTYPE_PROG, FLASH_PROGRAMMODE_STANDARD, GBuffer);

    /* Check the programmed block*/
		stop_add = FLASH_PROG_END_PHYSICAL_ADDRESS;
    start_add = FLASH_PROG_END_PHYSICAL_ADDRESS - (u32)FLASH_BLOCK_SIZE;

    for (add = start_add+1; add <= stop_add; add++)
            {
                if (FLASH_ReadByte(add) != new_val2)
                {
                    /* Error */
                    OperationStatus = FAILED;
                    /* OperationStatus = PASSED, if the data written/read to/from Flash Program memory is correct */
                    /* OperationStatus = FAILED, if the data written/read to/from Flash Program memory is corrupted */
                    while (1);
                }
            }

    /* Pass */
    OperationStatus = PASSED;
    /* OperationStatus = PASSED, if the data written/read to/from Flash Program memory is correct */
    /* OperationStatus = FAILED, if the data written/read to/from Flash Program memory is corrupted */
    while (1);
}

#pragma section (E_W_ROUTs)

FLASH_Status_TypeDef ProgramOperation(u16 BlockNum, FLASH_MemType_TypeDef MemType, FLASH_ProgramMode_TypeDef ProgMode, u8 *Buffer)
{
    u16 Count = 0;
    u32 StartAddress = 0;
    u8 flagstatus = 0x00;
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

#pragma section ()

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
