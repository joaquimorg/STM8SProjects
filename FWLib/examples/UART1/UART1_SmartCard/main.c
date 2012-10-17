/**
  ******************************************************************************
  * @file UART1_SmartCard\main.c
  * @brief This file contains the main function for UART1 Smartcard example.
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
  * @addtogroup UART1_SmartCard
  * @{
  */

/* Private define ------------------------------------------------------------*/
#define T0_PROTOCOL           0x00 /* T0 PROTOCOL */
#define SETUP_LENGHT          20
#define HIST_LENGHT           20
#define SC_Receive_Timeout    0x4000 /* direction to reader */

/* Private typedef -----------------------------------------------------------*/
/* ATR STRUCTURE - ANSWER TO RESET */
typedef  struct
{
    u8 TS; /* Bit Convention */
    u8 T0; /* high nibble = N. of setup byte; low nibble = N. of historical byte */
    u8 T[SETUP_LENGHT]; /* setup array */
    u8 H[HIST_LENGHT]; /* historical array */
    u8 Tlenght; /* setup array dimension */
    u8 Hlenght; /* historical array dimension */
} SC_ATR;

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

SC_ATR  SC_A2R;
vu32 index = 0, Counter = 0;
volatile TestStatus ATRDecodeStatus = FAILED;
vu32 CardInserted = 0, CardProtocol = 1;
vu8 DST_Buffer[50]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void GPIO_Configuration(void);
void Multiplexer_EvalBoard_Config(void);
u8 SC_decode_Answer2reset(vu8 *card);

/**
  * @brief Example firmware main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void main(void)
{
    u32 i = 0;
    /* Configure the multiplexer on the evalboard to select the smartCard*/
    Multiplexer_EvalBoard_Config();

    /* Configure the GPIO ports */
    GPIO_Configuration();
    
    /*High speed internal clock prescaler: 1*/
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

    /* Enable general interrupts */
    enableInterrupts();

    UART1_DeInit();
    /* UART1 configuration -------------------------------------------------------*/
    /* UART1 configured as follow:
          - Word Length = 9 Bits
          - 1.5 Stop Bit
          - Even parity
          - BaudRate = 10752 baud
          - Receive and transmit enabled
          - UART1 Clock enabled
    */
  UART1_Init((u32)10752, UART1_WORDLENGTH_9D, UART1_STOPBITS_1_5, UART1_PARITY_EVEN, UART1_SYNCMODE_CLOCK_ENABLE, UART1_MODE_TXRX_ENABLE);

    /* UART1 Clock set to 4MHz (frequence master 16 MHZ / 4) */
    UART1_SetPrescaler(0x02);

    /* UART1 Guard Time set to  Bit */
    UART1_SetGuardTime(0x2);

    /* Enable the UART1 Parity Error Interrupt */
    UART1_ITConfig(UART1_IT_PE, ENABLE);

    /* Enable the NACK Transmission */
    UART1_SmartCardNACKCmd(ENABLE);

    /* Enable the Smart Card Interface */
    UART1_SmartCardCmd(ENABLE);

    /* Loop while no smart card is detected */
    while ((GPIO_ReadInputData(GPIOE)& 0x01) == 0x00)
    {
    }
    
		/* PG7 - SmartCard_/CMDVCC: low */
    GPIO_WriteLow(GPIOG, GPIO_PIN_7);	

    /* release SmartCard_RESET signal */
    GPIO_WriteLow(GPIOG, GPIO_PIN_5);	

    for (i = 0; i < 6000; i++)
    {
    }
    /* set SmartCard_RESET signal */
    GPIO_WriteHigh(GPIOG, GPIO_PIN_5);	


    /* Read Smart Card ATR response */
    for (index = 0; index < 40; index++)
    {
        Counter = 0;
        while ((UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET) && (Counter != SC_Receive_Timeout))
        {
            Counter++;
        }

        if (Counter != SC_Receive_Timeout)
        {
            DST_Buffer[index] = UART1_ReceiveData8();
        }
    }

    /* Decode ATR */
    CardProtocol = SC_decode_Answer2reset(DST_Buffer);

    /* Test if the inserted card is ISO7816-3 T=0 compatible */
    if (CardProtocol == 0)
    {
        /* Inserted card is ISO7816-3 T=0 compatible */
        ATRDecodeStatus = PASSED;
    }
    else
    {
        /* Inserted smart card is not ISO7816-3 T=0 compatible */
        ATRDecodeStatus = FAILED;
    }

    while (1);
}

/**
  * @brief Configures the different GPIO ports.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void GPIO_Configuration(void)
{
    /* Set PA5 as Output open-drain high-impedance level - SmartCard_IO(UART11_Tx)*/
    GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);
    /* Set PG5 as Output push-pull low level - SmartCard_RESET*/
    GPIO_Init(GPIOG, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
    /* Set PA6 as Output push-pull low level - SmartCard_CLK*/
    GPIO_Init(GPIOA, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);
    /* Set PG4 as Output push-pull low level - SmartCard_5V/3V*/
    GPIO_Init(GPIOG, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
    /* Set PG7 as Output push-pull high level (inactive state) - SmartCard_/CMDVCC*/
    GPIO_Init(GPIOG, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST);
    /* Set PE0 as Input pull-up, no external interrupt - SmartCard_OFF*/
    GPIO_Init(GPIOE, GPIO_PIN_0, GPIO_MODE_IN_PU_NO_IT);
    /* Set RSTIN HIGH */
    GPIO_WriteHigh(GPIOG, GPIO_PIN_5);

    /* Select Smart Card CMDVCC */
    GPIO_WriteHigh(GPIOG, GPIO_PIN_7);

    /* Select 5 V */
    GPIO_WriteHigh(GPIOG, GPIO_PIN_4);
}
/**
  * @brief Decode the Card ATR Response.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
u8 SC_decode_Answer2reset(vu8 *card)
{
    u32 i = 0, flag = 0, buf = 0, protocol = 0;

    SC_A2R.TS = 0;
    SC_A2R.T0 = 0;
    for (i = 0; i < SETUP_LENGHT; i++)
    {
        SC_A2R.T[i] = 0;
    }
    for (i = 0;i < HIST_LENGHT; i++)
    {
        SC_A2R.H[i] = 0;
    }
    SC_A2R.Tlenght = 0;
    SC_A2R.Hlenght = 0;

    SC_A2R.TS = card[0]; /* INITIAL CHARACTER */
    SC_A2R.T0 = card[1]; /* FORMAT CHARACTER */

    SC_A2R.Hlenght = SC_A2R.T0 & 0x0F;

    if ((SC_A2R.T0 & 0x80) == 0x80) flag = 1;

    for (i = 0; i < 4; i++)
    {
        SC_A2R.Tlenght = SC_A2R.Tlenght + (((SC_A2R.T0 & 0xF0) >> (4 + i)) & 0x1);
    }

    for (i = 0; i < SC_A2R.Tlenght; i++)
    {
        SC_A2R.T[i] = card[i + 2];
    }

    protocol = SC_A2R.T[SC_A2R.Tlenght - 1] & 0x0F;

    while (flag)
    {
        if ((SC_A2R.T[SC_A2R.Tlenght-1] & 0x80)== 0x80)
        {
            flag = 1;
        }
        else
        {
            flag = 0;
        }
        buf = SC_A2R.Tlenght;
        SC_A2R.Tlenght = 0;

        for (i = 0; i < 4; i++)
        {
            SC_A2R.Tlenght = SC_A2R.Tlenght + (((SC_A2R.T[buf - 1] & 0xF0) >> (4 + i)) & 0x1);
        }
        for (i = 0; i < SC_A2R.Tlenght; i++)
        {
            SC_A2R.T[buf + i] = card[i + 2 + buf];
        }
        SC_A2R.Tlenght += buf;
    }

    for (i = 0;i < SC_A2R.Hlenght; i++)
    {
        SC_A2R.H[i] = card[i + 2 + SC_A2R.Tlenght];
    }

    return ((u8)protocol);
}

/**
  * @brief Configures the Multiplexer on the evalboard to select the smard card.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void Multiplexer_EvalBoard_Config(void)
{
    /* Initialize I/Os in Output PP Mode */
		GPIO_Init(GPIOF, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
		GPIO_Init(GPIOF, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_FAST);
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
