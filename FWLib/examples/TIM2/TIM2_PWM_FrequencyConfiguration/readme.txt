/** @page TIM2_PWM_FrequencyConfiguration Generate three different signals with three different frequencies 

  @par Example description

  This example shows how to configure the TIM2 peripheral to generate three
  different signals with three different frequencies. 

  
	TIM2CLK= Fmaster/8 = 2 MHz
  TIM2 counter clock = TIM2CLK /Prescaler = 1 MHz 

	The TIM2 CCR1 register value is equal to 32768: 
	CC1 update rate = TIM2 counter clock / CCR1_Val = 30.51 Hz,
	so the TIM2 Channel 1 generates a periodic signal with a frequency equal to 15.25 Hz.

	The TIM2 CCR2 register is equal to 16384:
	CC2 update rate = TIM2 counter clock / CCR2_Val = 61.03 Hz
	so the TIM2 channel 2 generates a periodic signal with a frequency equal to 30.5 Hz.

	The TIM2 CCR3 register is equal to 8192:
	CC3 update rate = TIM2 counter clock / CCR3_Val = 122.07 Hz
	so the TIM2 channel 3 generates a periodic signal with a frequency equal to 61 Hz.
  
  The TIM2 waveform can be displayed using an oscilloscope. 


  @par Directory contents

  - TIM2\TIM2_PWM_FrequencyConfiguration\main.c                   Main file containing the "main" function
  - TIM2\TIM2_PWM_FrequencyConfiguration\stm8s_conf.h             Library Configuration file
  - TIM2\TIM2_PWM_FrequencyConfiguration\stm8s_it.c               Interrupt routines source 
  - TIM2\TIM2_PWM_FrequencyConfiguration\stm8s_it.h               Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
    - Connect TIM2 pins to an oscilloscope:
	     - TIM2_CH1  pin (PD.4)  15.29
	     - TIM2_CH2  pin (PD.3)  30.58
	     - TIM2_CH3  pin (PA.3)  61.3


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_tim2.c
    - stm8s_gpio.c
  - Edit stm8s.h file to select the device you are working on (#define STM8S208, in this case).

  @b Tip: You can tailor the provided project template to run this example, for more
          details please refer to "stm8s_fwlib_um.chm" user manual; select "Peripheral Examples"
          then follow the instructions provided in "How to proceed" section.  
   
  - With STVD ?
   - Rebuild all files: Build-> Rebuild all.     
   - Load project image: Debug->Start Debugging
   - Run program: Debug->Run (Ctrl+F5)

  - With RIDE ?
   - Open the RIDE workspace
   - Rebuild all files: Project->Build project(Alt+F9)
   - Load project image: Debug->Start (Ctrl+D)
   - Run program: Debug->Run (Ctrl+F9)
  - Connect the TIM2 pins to an oscilloscope 
  
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
