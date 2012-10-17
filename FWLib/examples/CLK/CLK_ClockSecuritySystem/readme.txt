/** @page CLK_ClockSecuritySystem CSS (Clock security system) capability


  @par Example description

  This example demonstrates the CSS (clock security system) capability. 
  The MCU clock system is configured as follows:
	- HSI Prescaler = CLK_PRESCALER_HSIDIV8;
	- Switch Mode   = CLK_SWITCHMODE_AUTO
	- New Clock     = CLK_SOURCE_HSE;
	- Current Clock State  = CLK_CURRENTCLOCKSTATE_ENABLE;
	- CSS = ENABLE 
  
  - When detecting the change of clock from HSE to HSI/8 (auxilary), 
    the  blinking frequency of the LEDs becomes slower.


  @par Directory contents

  - CLK\CLK_ClockSecuritySystem\main.c                    Main file containing the "main" function
  - CLK\CLK_ClockSecuritySystem\stm8s_conf.h              Library Configuration file
  - CLK\CLK_ClockSecuritySystem\stm8s_it.c                Interrupt routines source
  - CLK\CLK_ClockSecuritySystem\stm8s_it.h                Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
      - Connect PE0 (CCO pin) to an oscilloscope   


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_clk.c
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
  - Observe the initial device frequency setting (HSE) on LED1, 2, 3, 4 
    of the Evaluation board (fast blinking frequency).
  - Disconnect the crystal. The CPU clock will then switch to HSI backup 
    clock as the CSS is enabled.
  - The CPU is still processing operations but the frequency has changed to HSI/8 
  (CSS default clock): LED1, 2, 3, 4 are toggling at another frequency (Slower). 
  - The clock switching can be monitored by putting a scope on PE0 (CCO).

  @par Hint
  
  - Before using the HSE clock ensure that the "Flash_Wait_States" is set to 1. 
   - To do so :
    - with STVD (menu: Debug Instrument -> MCU configuration-> Options)
    - with RIDE (edit the "STM8_OB.asm" file to configure the OPT7 & NOPT7 option bytes)
    
    
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
