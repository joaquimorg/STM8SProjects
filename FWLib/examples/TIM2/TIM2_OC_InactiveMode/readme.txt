/** @page TIM2_OC_InactiveMode Generate three different signals with three different delays with Inactive Mode

  @par Example description

  This example shows how to configure the TIM peripheral to generate three different 
  signals with three different delays.

  The TIM2CLK frequency is set to 2 MHz, the Prescaler is set to 2048 and used in 
  Output Compare Inctive Mode.
 
  TIM2 counter clock = TIM2CLK / (Prescaler) = 976 Hz 


  The TIM2 CCR1 register value is equal to 976:
  TIM2_CH1 delay = CCR1_Val/TIM2 counter clock  = 1000 ms
  so the TIM2 Channel 1 generates a signal with a delay equal to 1000 ms.

  The TIM2 CCR2 register value is equal to 488:
  TIM2_CH2 delay = CCR2_Val/TIM2 counter clock = 500 ms
  so the TIM2 Channel 2 generates a signal with a delay equal to 500 ms.

  The TIM2 CCR3 register value is equal to 244:
  TIM2_CH3 delay = CCR3_Val/TIM2 counter clock = 250 ms
  so the TIM2 Channel 3 generates a signal with a delay equal to 250 ms.


  @par Directory contents

  - TIM2\TIM2_OC_InactiveMode\main.c                    Main file containing the "main" function
  - TIM2\TIM2_OC_InactiveMode\stm8s_conf.h              Library Configuration file
  - TIM2\TIM2_OC_InactiveMode\stm8s_it.c                Interrupt routines source 
  - TIM2\TIM2_OC_InactiveMode\stm8s_it.h                Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
    - Connect the following pins to an oscilloscope:
	    - Pin PG.5
	    - Pin PG.6
	    - Pin PG.7

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
  - Connect PG.5 , PG.6 and PG.7 pins to an oscilloscope 
  
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
