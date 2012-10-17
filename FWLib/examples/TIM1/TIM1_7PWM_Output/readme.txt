/** @page TIM1_7PWM_Output Generate 7 PWM signals with 4 different duty cycles

  @par Example description

  This example shows how to configure the TIM1 peripheral to generate 7 PWM signals
  with 4 different duty cycles.

   TIM1CLK is fixed to 2 MHz, the TIM1 Prescaler is equal to 0 so the TIM1 counter
  clock used is 2 MHz. (By default, fTimer1= fMaster/8 )
  
  TIM1 frequency is defined as follows:
  TIM1 frequency = TIM1 counter clock / (TIM1_Period + 1) = 488.28 Hz.
  
  The TIM1 CCR1 register value is equal to 0x7FF, so the TIM1 Channel 1 and TIM1 
  Channel 1N generate a PWM signal with a frequency equal to 488.28Hz 
  and a duty cycle equal to:
  TIM1 Channel1 duty cycle = TIM1_CCR1 /( TIM1_Period + 1) = 50%.

  The TIM1 CCR2 register value is equal to 0x5FF, so the TIM1 Channel 2 and TIM1
  Channel 2N generate a PWM signal with a frequency equal to 488.28 Hz
  and a duty cycle equal to:
  TIM1 Channel2 duty cycle = TIM1_CCR2 / ( TIM1_Period + 1)= 37.5%.

  The TIM1 CCR3 register value is equal to 0x3FF, so the TIM1 Channel 3 and TIM1 
  Channel 3N generate a PWM signal with a frequency equal to 488.28 Hz 
  and a duty cycle equal to:
  TIM1 Channel3 duty cycle = TIM1_CCR3 / ( TIM1_Period + 1) = 25%.

  The TIM1 CCR4 register value is equal to 0x1FF, so the TIM1 Channel 4 
  generate a PWM signal with a frequency equal to 488.28 Hz 
  and a duty cycle equal to:
  TIM1 Channel4 duty cycle = TIM1_CCR4 / ( TIM1_Period + 1) = 12.5%.

  The TIM1 waveform can be displayed using an oscilloscope.


  @par Directory contents

  - TIM1\TIM1_7PWM_Output\main.c                    Main file containing the "main" function
  - TIM1\TIM1_7PWM_Output\stm8s_conf.h              Library Configuration file
  - TIM1\TIM1_7PWM_Output\stm8s_it.c                Interrupt routines source (not used in this example)
  - TIM1\TIM1_7PWM_Output\stm8s_it.h                Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - Connect TIM1 pins to an oscilloscope as follows:
	   - TIM1_CH1  pin (PC.1)  
	   - TIM1_CH1N pin (PH.7)  
	   - TIM1_CH2  pin (PC.2)  
	   - TIM1_CH2N pin (PH.6)  
	   - TIM1_CH3  pin (PC.3)  
	   - TIM1_CH3N pin (PH.5)  
	   - TIM1_CH4 pin  (PC.4)


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_tim1.c
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
  - Connect the TIM1 pins to an oscilloscope 

  
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
