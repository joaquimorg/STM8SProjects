/**
  ******************************************************************************
  * @file TIM1_Sinewave_Generation\main.c
  * @brief This file contains the main function for TIM1 Sinewave_Generation example.
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
#include "main.h"

/**
  * @addtogroup TIM1_Sinewave_Generation
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile MySin_Typedef sin;/* Stores the sin structure in ram */
vu8 counter = 0;	 /* The counter variable for counting the Update cycles in TIM1 */
/* This shows the value of the samples for the different no of samples in half
sine wave. The last value corresponds to the amplitude of the sine wave.*/

#if	nbr_of_samples_half == 16
const u8 sample[9]={0,20,38,56,71,83,92,98,100};

#elif	nbr_of_samples_half == 18
const u8 sample[10]={0,17,34,50,64,76,86,93,97,99};

#elif	nbr_of_samples_half == 22
const u8 sample[12]={0,14,28,42,54,65,76,84,91,96,99,100};

#elif	nbr_of_samples_half == 24
const u8 sample[13]={0,13,25,38,49,60,69,78,85,91,95,97,98};

#elif	nbr_of_samples_half == 26
const u8 sample[14]={0,12,24,35,46,56,66,74,81,88,93,96,98,99};

#elif	nbr_of_samples_half == 28
const u8 sample[15]={0,11,22,33,43,53,62,71,78,85,90,94,97,99,100};

#elif 	nbr_of_samples_half == 30 
const u8 sample[16]={0,10,21,31,41,50,59,67,74,81,87,91,95,98,99,100};

#elif	nbr_of_samples_half == 46
const u8 sample[24]={0,7,13,20,26,33,39,45,51,57,62,67,72,76,80,84,87,90,92, \
								     94,96,97,98,98};
#endif

/* Private function prototypes -----------------------------------------------*/
void LoadValueIntoSinStruct(void);
void TIMER_Init(void);


/* Public functions ----------------------------------------------------------*/

/**
  * @brief Example firmware main entry point.
  * @par Parameters:
  * None
  * @retval 
  * None
  */

void main(void)
{ 
  /* select Clock = 4 MHz */
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV4);
	CLK_HSICmd(ENABLE);
	
  
	/* The counter value is initialized to 0.*/
  counter = 0;
	/* Call LoadValueIntoSinStruct() & TIMER_Init()*/
  LoadValueIntoSinStruct();           

  TIMER_Init();

  /* Enable interrupts*/
  enableInterrupts();  			
  
	/* forever loop*/
  while(1);
}  
/* Private functions ---------------------------------------------------------*/
void LoadValueIntoSinStruct(void)
{
  u8	offset=0,diff=0;
	
  while(offset <= (nbr_of_samples_half/2))
  {
    /*First half of upper half samples of sine wave is loaded into "value" of sin structure.*/
	   sin.Value[offset] = sample[offset]; 
	   offset++;				            	
  }
  diff = (u8)(offset - 1);
  while(offset < nbr_of_samples_half)
  {                                    
	    diff--;	
	    /* Second half of upper half samples of sine wave is loaded into "value" of sin structure.*/
	    sin.Value[offset] = sample[diff];		
	    offset++;				                 
  }  
  /*Index is initialized to 0.Sin wave upper half is generated at start*/
  sin.Index = 0;                      
  sin.Up = TRUE;                      
}

void TIMER_Init(void)
{
	TIM1_DeInit();
	/* Time base configuration */
  /*
	     TIM1_Period = INIT_TIM1_ARR
          TIM1_Prescaler = 0
	     TIM1_RepetitionCounter=0
      */
  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, INIT_TIM1_ARR, 0);
	
	/* PWM configuration */
	/*
	     TIM1_OCMode = TIM1_OCMODE_PWM1
			 TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
	     TIM1_Pulse = INIT_TIM1_CCR1
       TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
	*/
  TIM1_OC1Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, INIT_TIM1_CCR1, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);
	
	/* Update Interrupt Enable */
	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
	
	/* ARRPreload Enable */
	TIM1_ARRPreloadConfig(ENABLE);
	
	/* Enable TIM1 */
	TIM1_Cmd(ENABLE);
	
	/* Enable CC1 output*/
	TIM1_CtrlPWMOutputs(ENABLE);
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