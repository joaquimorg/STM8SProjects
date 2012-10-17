/**
  ******************************************************************************
  * @file project\main.c
  * @brief This file contains the example main function.
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
/**
  * @addtogroup CAN_Networking
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"

/* Private defines -----------------------------------------------------------*/
/* Private variables -----------------------------------------------------------*/
u8 Key_Pressed_Number = 0x00;
vu8 Key_status = 0x00;
/* Private function prototypes -----------------------------------------------*/
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
  CAN_TxStatus_TypeDef status =CAN_TxStatus_Failed;
	/* Transmit Parameters */
	CAN_Id_TypeDef Tx_IDE;
  CAN_RTR_TypeDef Tx_RTR;
  u8 Tx_DLC;
	u8 Tx_Data[8];
	u32 Tx_Id;
	
	/* Clock configuration */
  CLK_Config();
	
  /* GPIO configuration */
  GPIO_Config();
	
	/* LCD configuration */
  LCD_Config();

  /* CAN configuration */
  CAN_Config();

  /* Enable Fifo message pending interruption*/
	/* Message reception is done by CAN_RX ISR*/
	CAN_ITConfig(CAN_IT_FMP, ENABLE);
	
	/* Enable Interrupts*/
	enableInterrupts();

  /* Infinite loop*/
  while(1)
  {
    while(Key_status!= Key_NoPressed)
     {
	  
			if(Key_Pressed_Number==0x0) 
			{
	        Key_Pressed_Number = 0x03;
			}
		  else
		  {
			    Key_Pressed_Number--;
		  }
			
			/* Sender Display*/
	    LED_Display(Key_Pressed_Number);
			LCD_Display(Key_Pressed_Number);
			Delay(TIME);
			
			/* Transmit Parameters*/
			Tx_Id = 0x321;
      Tx_IDE = CAN_Id_Standard;
			Tx_RTR = CAN_RTR_Data;
			Tx_DLC = 1;
      Tx_Data[0] = Key_Pressed_Number;
			
			/* Sender send Frame */
		  status = CAN_Transmit(Tx_Id,Tx_IDE,Tx_RTR,Tx_DLC,Tx_Data);
			
			/* while key is not pressed, loop*/
      Key_status= Key_NoPressed;
	 }
  }
}
/**
  * @brief Configures the GPIO of leds and key botton
  * @par Parameters:
  * None
  * @retval None
  * @par Required preconditions:
  * None
  */
void GPIO_Config(void)
{
  u8 GPIO_Pin;
  u8 GPIO_Mode;
  /* (Leds) as output push-pull */
  GPIO_Pin = LED1_PIN|LED2_PIN|LED3_PIN| LED4_PIN;
  GPIO_Mode = GPIO_MODE_OUT_PP_HIGH_FAST;
  GPIO_Init(LEDS_PORT, GPIO_Pin, GPIO_Mode);

  /*  (key button) as input floating */
  GPIO_Pin = BUTTON_PIN ;
  GPIO_Mode = GPIO_MODE_IN_FL_NO_IT;
  GPIO_Init(BUTTON_PORT, GPIO_Pin, GPIO_Mode);
	/* Initialize I/O in Input Mode with Interrupt for button */
  GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_FL_IT);
	/* Initialize the Interrupt sensitivity for button*/
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_LOW);
	/* Set all leds OFF*/
	GPIO_WriteLow(LEDS_PORT, LED1_PIN|LED2_PIN|LED3_PIN|LED4_PIN);
}
/**
  * @brief Configures the CAN mode and filter
  * @par Parameters:
  * None
  * @retval None
  * @par Required preconditions:
  * None
  */
void CAN_Config(void)
{
	CAN_InitStatus_TypeDef status = CAN_InitStatus_Failed;
	
	/* Filter Parameters */
	CAN_FilterNumber_TypeDef CAN_FilterNumber;
	FunctionalState CAN_FilterActivation;
	CAN_FilterMode_TypeDef CAN_FilterMode;
	CAN_FilterScale_TypeDef CAN_FilterScale;
	u8 CAN_FilterID1;
	u8 CAN_FilterID2;
	u8 CAN_FilterID3;
	u8 CAN_FilterID4;
	u8 CAN_FilterIDMask1;
	u8 CAN_FilterIDMask2;
	u8 CAN_FilterIDMask3;
	u8 CAN_FilterIDMask4; 
	
	/* Init Parameters*/
	CAN_MasterCtrl_TypeDef CAN_MasterCtrl;
  CAN_Mode_TypeDef CAN_Mode;
  CAN_SynJumpWidth_TypeDef CAN_SynJumpWidth;
  CAN_BitSeg1_TypeDef CAN_BitSeg1;
  CAN_BitSeg2_TypeDef CAN_BitSeg2;
  CAN_ClockSource_TypeDef CAN_ClockSource;
  u8 CAN_Prescaler;	
  
	/* CAN register init */
  CAN_DeInit();
  	
	/* CAN  init */
	CAN_MasterCtrl=CAN_MasterCtrl_AllDisabled;
  CAN_Mode = CAN_Mode_Normal;
  CAN_SynJumpWidth = CAN_SynJumpWidth_1TimeQuantum;
  CAN_BitSeg1 = CAN_BitSeg1_8TimeQuantum;
  CAN_BitSeg2 = CAN_BitSeg2_7TimeQuantum;
  CAN_Prescaler = 1;
	CAN_ClockSource = CAN_ClockSource_Internal;
  status = CAN_Init(CAN_MasterCtrl,CAN_Mode,CAN_SynJumpWidth,CAN_BitSeg1,CAN_BitSeg2,CAN_ClockSource,CAN_Prescaler);

	/* CAN filter init */
	CAN_FilterNumber = 0;
	CAN_FilterActivation = ENABLE;
	CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterScale = CAN_FilterScale_32Bit;
	CAN_FilterID1=0;  
	CAN_FilterID2=0;
	CAN_FilterID3=0;
	CAN_FilterID4=0;
	CAN_FilterIDMask1=0;
	CAN_FilterIDMask2=0;
	CAN_FilterIDMask3=0;
	CAN_FilterIDMask4=0;  
	CAN_FilterInit(CAN_FilterNumber, CAN_FilterActivation, CAN_FilterMode, CAN_FilterScale,CAN_FilterID1, CAN_FilterID2, CAN_FilterID3, CAN_FilterID4,CAN_FilterIDMask1, CAN_FilterIDMask2, CAN_FilterIDMask3, CAN_FilterIDMask4);
}

/**
  * @brief Display led status 
  * @param Ledstatus the led to set ON, other leds are set OFF
  * @retval 
  * None
  */
void LED_Display(u8 Ledstatus)
{
	/* Set all leds OFF*/
	GPIO_WriteLow(LEDS_PORT, LED1_PIN|LED2_PIN|LED3_PIN|LED4_PIN);

 /* Set the led ON depending on the value of Ledstatus*/
 switch((u8)Ledstatus)
 {
   case(0): GPIO_WriteHigh(LEDS_PORT, LED4_PIN);
            break;
   
   case(1): GPIO_WriteHigh(LEDS_PORT, LED3_PIN);
            break;
 
   case(2): GPIO_WriteHigh(LEDS_PORT, LED2_PIN);
            break;

   case(3): GPIO_WriteHigh(LEDS_PORT, LED1_PIN);
            break;
   }
}
/**
  * @brief Generates a delay time.
  * @param nCount the value for looping.
  * None
  * @retval 
  * None
  */
void Delay (u16 nCount)
{
  /* Decrement nCount value */
  for (; nCount != 0; nCount--);
}
/**
  * @brief Configures of the LCD 
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void  LCD_Config(void)
{
		/* SPI initialization for LCD */
    SPI_DeInit();
    SPI_Init(SPI_FIRSTBIT_MSB,  SPI_BAUDRATEPRESCALER_64, SPI_MODE_MASTER,  SPI_CLOCKPOLARITY_HIGH, SPI_CLOCKPHASE_2EDGE,  SPI_DATADIRECTION_1LINE_TX, SPI_NSS_SOFT, 0x07);

    SPI_Cmd(ENABLE);

    /* Otherwise the LCD display doesn't work when the board is powered-up ... */
    Delay(0xFFFF);
		
		/* LCD initialization: Init_SPISimplex must be executed before */
    LCD_Init();
    LCD_Backlight(DISABLE);
		
		/* Display the welcome text on first LCD line */
    LCD_PrintMsg("   STM8S CAN      Press Key");
}
/**
  * @brief Display led status and communication direction on the LCD 
  * @param Ledstatus the led status
  * @retval 
  * None
  */
void LCD_Display(u8 Ledstatus)
{
	LCD_ClearLine(LCD_LINE2);
	
	switch((u8)Ledstatus)
 {
   case(0): LCD_PrintMsg("   STM8S CAN   Sender   LD4 ON ");
            break;
   
   case(1): LCD_PrintMsg("   STM8S CAN   Sender   LD3 ON ");
            break;
 
   case(2): LCD_PrintMsg("   STM8S CAN   Sender   LD2 ON ");
            break;

   case(3): LCD_PrintMsg("   STM8S CAN   Sender   LD1 ON ");
            break;
						
   case(4): LCD_PrintMsg("   STM8S CAN   Receiver LD4 ON ");
            break;
   
   case(5): LCD_PrintMsg("   STM8S CAN   Receiver LD3 ON ");
            break;
 
   case(6): LCD_PrintMsg("   STM8S CAN   Receiver LD2 ON ");
            break;

   case(7): LCD_PrintMsg("   STM8S CAN   Receiver LD1 ON ");
            break;
	}
}
/**
  * @brief Configuration of the CLK to provide 16MHz 
  * @par Parameters:
  * None
  * @retval 
  * None
  */
void  CLK_Config(void)
{
		/* fCAN = fCAN = fHSI = 16Mhz */
		CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
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
