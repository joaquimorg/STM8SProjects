/** @page IWDG_RearmReset IWDG refreshing, IWDG Reset


  @par Example description

  This example provides a short description of how to use the IWDG peripheral:
	When LED1 and LED3 are ON, this indicates the IWDG refresh.
  Pressing the KEY button will stop the IWDG refresh and by the way LED1 and LED3 
  are  OFF.
  LED2 and LED4 are on for a short delay till the IWDG reset occurs so they will 
  blink twice.
	
	

  @par Directory contents

  - IWDG\IWDG_RearmReset\main.c                    Main file containing the "main" function
  - IWDG\IWDG_RearmReset\stm8s_conf.h              Library Configuration file
  - IWDG\IWDG_RearmReset\stm8s_it.c                Interrupt routines source 
  - IWDG\IWDG_RearmReset\stm8s_it.h                Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
     - Four leds LD1 to LD4 and Key push button are used.


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_iwdg.c
    - stm8s_rst.c
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
  - The LED1 and LED3 are ON
  - If you press the button, a Reset occurs and LED2 and LD4 blink 2 times
    
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
