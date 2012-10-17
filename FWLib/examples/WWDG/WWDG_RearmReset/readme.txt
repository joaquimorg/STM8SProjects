/** @page WWDG_RearmReset WWDG refreshing, WWDG Reset


  @par Example description

  The example shows how to refresh the WWDG counter at regular time intervals,
  within the allowed refresh window.

  The WWDG timeout is set to 393,21ms, the refresh window is set from 43,008 ms 
  to 393,216 ms.

  Pressing the Key button tunrs off LD1 and LD4 and causes a software reset by 
  refreshing the WWDG outside the allowed time window.  
  Pressing the joystick selection button turns off LD2 and LD3 and disables the 
  watchdog refresh. So the counter counts down to 3Fh and a WWDG reset occurs.
  Each time a WWDG reset is generated, LD1, LD2, LD3 and LD4 are toggled twice 
  after resuming from reset.

  

  @par Directory contents

  - WWDG\WWDG_RearmReset\main.c                    Main file containing the "main" function
  - WWDG\WWDG_RearmReset\stm8s_conf.h              Library Configuration file
  - WWDG\WWDG_RearmReset\stm8s_it.c                Interrupt routines source 
  - WWDG\WWDG_RearmReset\stm8s_it.h                Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
     - Four leds LD1 to LD4, Key and JOYSTICK SEL push buttons are used.


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_gpio.c
    - stm8s_wwdg.c
    - stm8s_rst.c
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
  - (*)The LD1, LD2, LD3 and LD4 are ON
  - If you press the button, LD1 and LD4 change status, a non 
    alowed refresh is Enabled, so a Reset occurs.
  - If you press the Joysisck select button, LD2 and LD3 change
    status, the wwdg is not refreshed, so a Reset occurs.
  - After resuming from WWDG Reset LD1, LD2, LD3 and LD4 blink two times
    and the program returns to the (*) step

  
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
