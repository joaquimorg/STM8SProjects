/** @page CLK_ClockSelection How to select the different clock sources


  @par Example description

  This example demonstrates the possibility to select the different clock sources 
  using the joystick available on the evaluation board.
  - To select a clock source, push the joystick in the corresponding direction. 
    UP = HSE, DOWN = LSI & LEFT = HSI
  - The current clock selection will be displayed on the LCD.
  - Current clock source can be visualized on CCO pin. 
  


  @par Directory contents

  - CLK\CLK_ClockSelection\main.c                    Main file containing the "main" function
  - CLK\CLK_ClockSelection\stm8s_conf.h              Library Configuration file
  - CLK\CLK_ClockSelection\stm8s_it.c                Interrupt routines source
  - CLK\CLK_ClockSelection\stm8s_it.h                Interrupt routines declaration
  

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
    - stm8s_spi.c
    - stm8s_exti.c
    - stm8s_clk.c
    - stm8s_gpio.c
    - mono_lcd.c (under FWLib\examples\Common)
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
  - Using the joystick choose the desired clock source to apply:
    UP = HSE, DOWN = LSI & LEFT = HSI
  - A message is displayed on the LCD to indicate the currently used clock.
  - The clock switching can be monitored by putting a scope on PE0 (CCO).


  @par Hint
  
  - Before using the HSE clock ensure that the "Flash_Wait_States" is set to 1.
  - Before using the LSI clock you have to configure the option bytes in order 
    to enable the "LSI_EN".
  - To do so :
    - with STVD (menu: Debug Instrument -> MCU configuration-> Options)
    - with RIDE (edit the "STM8_OB.asm" file to configure the OPT3 & NOPT3, 
    OPT7 & NOPT7 option bytes)
  
 
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
