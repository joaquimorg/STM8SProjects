/** @page TIM2_PWM_DutyCycleConfiguration Generate three different signals with three different duty cycles 

  @par Example description

  This example shows how to configure the TIM2 peripheral in PWM (Pulse Width Modulation) 
  mode.
  The TIM2CLK frequency is 2MHz, the Prescaler is 1 so the TIM2 counter clock is 2MHz. 

  The TIM2 is running at 2 KHz: TIM2 Frequency = TIM2 counter clock/(ARR + 1) 
	
	So TIM2 frequency is 2KHz.


  The TIM2 CCR1 register value is equal to 500, so the TIM2 Channel 1 generates a 
  PWM signal with a frequency equal to 2KHz and a duty cycle equal to 50%:
  TIM2 Channel1 duty cycle = [TIM2_CCR1/(TIM2_ARR + 1)] * 100 = 50%

  The TIM2 CCR2 register value is equal to 375, so the TIM2 Channel 2 generates a 
  PWM signal with a frequency equal to 2KHz and a duty cycle equal to 25%:
  TIM2 Channel2 duty cycle = [TIM2_CCR2/ (TIM2_ARR + 1)] * 100 = 25%

  The TIM2 CCR3 register value is equal to 250, so the TIM2 Channel 3 generates a 
  PWM signal with a frequency equal to 2KHz and a duty cycle equal to 12.5%:
  TIM2 Channel3 duty cycle = [TIM2_CCR3/ (TIM2_ARR + 1)] * 100 = 12.5%


  The PWM waveform can be displayed using an oscilloscope.


  @par Directory contents

  - TIM2\TIM2_PWM_DutyCycleConfiguration\main.c                   Main file containing the "main" function
  - TIM2\TIM2_PWM_DutyCycleConfiguration\stm8s_conf.h             Library Configuration file
  - TIM2\TIM2_PWM_DutyCycleConfiguration\stm8s_it.c               Interrupt routines source (not used in this example)
  - TIM2\TIM2_PWM_DutyCycleConfiguration\stm8s_it.h               Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
    - Connect TIM2 pins to an oscilloscope:
	     - TIM2_CH1  pin (PD.4)  
	     - TIM2_CH2  pin (PD.3)  
	     - TIM2_CH3  pin (PA.3)  


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
