/** @page TIM5_Synchronisation_with_TIM6 Synchronisation of TIM5 using TIM6


  @par Example description

  This example provides a short description of how to use the TIM5 peripheral
  synchronised by TIM6. TIM5 is configured as a slave of TIM6. 

  TIM5 is configurated to be enabled if it detects an edge on its input trigger (TRGI).
  TIM6 is configurated to generate an edge through its (TRGO) at the update event.
  TIM5 TRGI is configurated to be connected (internal connection) to TIM6 TRGO.

  If line "Inc1:" in main.c is commented, TIM6 is not enabled => no Edge is detected on TIM5 TRGI,
  ==> Result variable value will be equal to 0 (FAILED).
  If line "Inc1" in main.c is uncommented, TIM6 is enabled => TIM6 generate an Edge on its TRGO and 
  an Edge is detected on TIM5 TRGI. ==> Result variable value will be equal to 1 (PASS).


  @par Directory contents

  - TIM5\TIM5_Synchronisation_With_TIM6\main.c                    Main file containing the "main" function
  - TIM5\TIM5_Synchronisation_With_TIM6\stm8s_conf.h              Library Configuration file
  - TIM5\TIM5_Synchronisation_With_TIM6\stm8s_it.c                Interrupt routines source
  - TIM5\TIM5_Synchronisation_With_TIM6\stm8s_it.h                Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example runs only on STM8S903 device.
  
  - This example has been tested with a STMicroelectronics prototyping board 
    (STM8S903 device) and ST Visual Develop (STVD) toolchain (Cosmic and 
    Raisonance compilers). 
    And can be easily tailored to any other development board. 

  
@par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_tim5.c
    - stm8s_tim6.c
  - Edit stm8s.h file to select the device you are working on (#define STM8S903, in this case).

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
  - Example result can be displayed using "Result" variable.
 
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
