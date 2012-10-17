/**
  ******************************************************************************
  * @file UART1_IrDA\Transmit\main.c
  * @brief This file contains the main function for UART1 in IrDA transmit mode.
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
  * @addtogroup UART1_IrDA_Transmit
  * @{
  */
 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Evalboard I/Os configuration */
#define Bit_RESET  0
#define Bit_SET    1

#define  NOKEY  0x00
#define  SEL    0x01
#define  RIGHT  0x02
#define  LEFT   0x03
#define  UP     0x04
#define  DOWN   0x05

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Multiplexer_EvalBoard_Config(void);
u8 GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
u8 ReadJoystick(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Example firmware main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{

    u8 MyKey1= 0;

    /* Configures the Multiplexer on the evalboard to select the IrDA*/
    Multiplexer_EvalBoard_Config();

    UART1_DeInit();
    /* UART1 configuration ----------------------------------------------------*/
    /* UART1 configured as follow:
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - BaudRate = 9600 baud
          - Tx and Rx enabled
          - UART1 Clock disabled
    */
  UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);

		/* Set Prescaler*/
    UART1_SetPrescaler(0x1);

    UART1_IrDAConfig(UART1_IRDAMODE_NORMAL);

    UART1_IrDACmd(ENABLE);

    while (1)
    {
        /* Read Key */
        while (MyKey1 == NOKEY)
        {
            MyKey1 = ReadJoystick();
        }

        switch (MyKey1)
        {
        case UP:
            UART1_SendData8(UP);
            while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
            {
            }
            MyKey1 = NOKEY;
            break;
        case DOWN:
            UART1_SendData8(DOWN);
            while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
            {
            }
            MyKey1 = NOKEY;
            break;
        case LEFT:
            UART1_SendData8(LEFT);
            while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
            {
            }
            MyKey1 = NOKEY;
            break;
        case RIGHT:
            UART1_SendData8(RIGHT);
            while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
            {
            }
            MyKey1 = NOKEY;
            break;
        case SEL:
            UART1_SendData8(SEL);
            while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
            {
            }
            MyKey1 = NOKEY;
            break;
        default:
            break;
        }
    }

}
/**
  * @brief Configures the Multiplexer on the evalboard to select the IrDA.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void Multiplexer_EvalBoard_Config(void)
{
  /* Initialize I/Os in Output PP Mode */
	GPIO_Init(GPIOF, GPIO_PIN_6 ,GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(GPIOF, GPIO_PIN_5 ,GPIO_MODE_OUT_PP_HIGH_FAST);
}

/**
  * @brief Reads joystick position from evalboard.
  * @version V1.1.0
  * @par Parameters:
  * None
  * @retval u8 Returns the Joystick position (RIGHT, LEFT, SEL, UP, DOWN or NOKEY)
  * @par Required preconditions:
  * None
  * @par Called functions:
  * - GPIO_ReadInputDataBit
  */

u8 ReadJoystick(void)
{
    /* "right" key is pressed */
    if (!GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_5))
    {
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_5) == Bit_RESET);
        return RIGHT;
    }
    /* "left" key is pressed */
    if (!GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_4))
    {
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_4) == Bit_RESET);
        return LEFT;
    }
    /* "up" key is pressed */
    if (!GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_6))
    {
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_6) == Bit_RESET);
        return UP;
    }
    /* "down" key is pressed */
    if (!GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_7))
    {
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_7) == Bit_RESET);
        return DOWN;
    }
    /* "sel" key is pressed */
    if (!GPIO_ReadInputDataBit(GPIOD, GPIO_PIN_7))
    {
        while (GPIO_ReadInputDataBit(GPIOD, GPIO_PIN_7) == Bit_RESET);
        return SEL;
    }
    /* No key is pressed */
    else
    {
        return NOKEY;
    }
}

/**
  * @brief Reads the specified input port pin.
  * @param[in] GPIOx GPIO number, where x can be (A..E) to select the GPIO peripheral.
  * @param[in] GPIO_Pin Specifies the port bit to read, it can be GPIO_Pin_x where x can be (0..7).
  * @retval u8 The input port pin value.
  */

u8 GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin)
{
    u8 bitstatus = 0x00;

    if ((GPIOx->IDR & GPIO_Pin) != (u32)Bit_RESET)
    {
        bitstatus = (u8)Bit_SET;
    }
    else
    {
        bitstatus = (u8)Bit_RESET;
    }
    return bitstatus;
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
