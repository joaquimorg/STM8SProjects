/** @page EXTI_InterruptPriority Reading buttons state using external interrupts


  @par Example description

  This example provides a short description of how to use the EXTI peripheral:
  - Configuration of External Interrupt / Falling edge and TLI
  - LEDs are toggling at each interrupt detected


  @par Directory contents

  - EXTI\EXTI_InterruptPriority\main.c                    Main file containing the "main" function
  - EXTI\EXTI_InterruptPriority\stm8s_conf.h              Library Configuration file
  - EXTI\EXTI_InterruptPriority\stm8s_it.c                Interrupt routines source
  - EXTI\EXTI_InterruptPriority\stm8s_it.h                Interrupt routines declaration
  
  
 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
     - Four leds LD1 to LD4 are used.
     - Joystick LEFT, RIGHT, UP, DOWN and SEL buttons are used.


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_exti.c
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
  - The LD1, LD2, LD3 and LD4 are ON per default
  - Joystick LEFT  pressed: toggle LED1
  - Joystick RIGHT pressed: toggle LED2
  - Joystick UP    pressed: toggle LED3
  - Joystick DOWN  pressed: toggle LED4
  - Joystick SEL   pressed: toggle all LEDs

 
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
