/**
  ******************************************************************************
  * @file SPI_FastCommunicationMicroSD\main.c
  * @brief This file contains the main function for SPI fast communication example.
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
#include "microsd.h"
#include "mono_lcd.h"

/**
  * @addtogroup SPI_FastCommunicationMicroSD
  * @{
  */

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))
/* Private define ------------------------------------------------------------*/
/* Evalboard I/Os configuration */
#define LEDS_PORT (GPIOH)
#define LED1_PIN  (GPIO_PIN_3)
#define LED2_PIN  (GPIO_PIN_2)
#define LED3_PIN  (GPIO_PIN_1)
#define LED4_PIN  (GPIO_PIN_0)
#define BufferSize (countof(Tx_Buffer)-1)

/* Private variables ---------------------------------------------------------*/
u16 Status = 0;
u8 Tx_Buffer[] = "STM8Sxxx SPI Firmware Library Example: communication with a microSD card";
u8 Rx_Buffer[BufferSize]= {0};
ErrorStatus TransferStatus = ERROR;

/* Private function prototypes -----------------------------------------------*/
void Delay (u16 nCount);
ErrorStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength);
void Fill_Buffer(u8 *pBuffer, u16 BufferLength, u8 Offset);

/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @brief Validation firmware main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
    /* Initialize I/Os in Output Mode */
    GPIO_Init(LEDS_PORT, LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
		
    /******************************Disable LCD*********************************/
    /* Initialize SPI in Master mode for LCD */
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_HIGH, SPI_CLOCKPHASE_2EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
		
    SPI_Cmd(ENABLE);

    /* Initialize LCD*/
    LCD_Init();
    /* Clear LCD*/
    LCD_Clear();
    /* Disable Chip select*/
    LCD_ChipSelect(DISABLE);
    /* Read LCD status*/
    LCD_ReadStatus();
    /* Insert delay*/
    Delay(0xFFFF);
		
    /*************************Configue High Speed Clock**********************/
    /*High speed internal clock prescaler: 1*/
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    /***********************SPI and MSD Card initialization******************/

    while (MSD_Detect() == MICROSD_NOT_PRESENT)
    {
        /* Wait MicroSD card insertion */
    }

    Delay(0xFFFF);
    /* Init the flash micro SD*/
    Status = MSD_Init();

    /***************************Block Read/Write******************************/
		/* Write block of 512 bytes on address 0 */
		Status = MSD_WriteBlock(Tx_Buffer, 0, BufferSize);
    /* Read block of 512 bytes from address 0 */
		Status = MSD_ReadBlock(Rx_Buffer, 0, BufferSize);

    /* Check data */
    TransferStatus = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
    if (TransferStatus != SUCCESS)
    {
        while (1) /* Go to infinite loop when there is mismatch in data programming*/
        {
            GPIO_WriteReverse(LEDS_PORT, LED1_PIN);
            Delay((u16)0xFFFF);
            Delay((u16)0xFFFF);
        }
    }

    while (1)
    {
        GPIO_WriteReverse(LEDS_PORT, (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN));
        Delay((u16)0xFFFF);
        Delay((u16)0xFFFF);
    }
}

/**
  * @brief Compares two buffers.
  * @par Parameters:
  * - pBuffer1, pBuffer2: buffers to be compared.
  * - BufferLength: buffer's length
  * @retval
	* - PASSED: pBuffer1 identical to pBuffer2
	* - FAILED: pBuffer1 differs from pBuffer2
  * @par Required preconditions:
  * None
  */
ErrorStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer1 != *pBuffer2)
        {
            return ERROR;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return SUCCESS;
}
/**
  * @brief Delay.
  * @param[in] nCount
  * @retval 
  * None
  */
void Delay(u16 nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
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
