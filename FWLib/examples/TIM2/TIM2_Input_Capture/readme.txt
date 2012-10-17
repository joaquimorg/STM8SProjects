/** @page TIM2_Input_Capture LSI Capture by TIM2 

  @par Example description

  This example shows how to configure the TIM2 peripheral to capture the LSI.

  
  TIM2CLK is fixed to 2 MHz, the TIM2 Prescaler is equal to 1 so the TIM2 counter
  clock used is 2 MHz. (By default, fTIM1= fMaster/8 )
  
  The TIM2 Channel 1 is configured as Input capture.
  The LSI signal is redirected to the CCO output Pin.
  The TIM2 Channel 1 is connected to the CCO Pin. 
  
  The value of LSI Frequency is stored in the LSIClockFreq variable.


  @par Directory contents

  - TIM2\TIM2_Input_Capture\main.c                    Main file containing the "main" function
  - TIM2\TIM2_Input_Capture\stm8s_conf.h              Library Configuration file
  - TIM2\TIM2_Input_Capture\stm8s_it.c                Interrupt routines source (not used in this example)
  - TIM2\TIM2_Input_Capture\stm8s_it.h                Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
     - Connect the configurable Clock Output CCO pin (PE.0)	to TIM2_CH1  pin (PD.4)  


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_tim2.c
    - stm8s_gpio.c
    - stm8s_clk.c
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
  - Connect the TIM2_CC1 pins to COO pin
  - Read the value of LSIClockFreq variable

  
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
