/** @page ITC_SoftwarePriority Changing software priority on two external interrupt lines


  @par Example description

  This basic firwmare example shows how to use the ITC peripheral.
  It does the following:
  - Configure two I/Os in output push-pull for driving two LEDs
  - Configure two I/Os in input with interrupt for reading two buttons
  - Configure the two External Interrupt lines in Falling edge + Low level (PORTB and PORTC)
  - Change software priority between PORTB and PORTC (PORTC has a higher sw priority)


  @par Directory contents

  - ITC\ITC_SoftwarePriority\main.c                    Main file containing the "main" function
  - ITC\ITC_SoftwarePriority\main.h                    Main functions prototype
  - ITC\ITC_SoftwarePriority\stm8s_conf.h              Library Configuration file
  - ITC\ITC_SoftwarePriority\stm8s_it.c                Interrupt routines source
  - ITC\ITC_SoftwarePriority\stm8s_it.h                Interrupt routines declaration


 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
     - Two leds LED1 to LED2, Key push button and Joystick Down button are used.
     

  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_exti.c
    - stm8s_gpio.c
    - stm8s_itc.c
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
  - The LED1 and LED2 are ON per default
  - When the Joystick Down button is pressed, the LED1 toggles and LED2 is OFF
  - When the Key button is pressed, the LED2 toggles and LED1 is OFF
  - Play with the Button and the Joystick Down in order to see which one has
    the highest priority:
     - press and maintain the Joystick Down Button -> LED1 toggles, LED2 OFF
     - Continue pressing the Joystick Down Button and press the Key button 
     -> LED2 toggles instead, LED1 OFF (Key has the priority)
     If you do the reverse, you can see that the KEY button is not interrupted 
     by the Joystick button as configured by hardware.

  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
