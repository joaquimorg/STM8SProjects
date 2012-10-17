/** @page TIM1_Sinewave_Generation Generate a sinewave in the range of 45Hz to 65Hz with the resolution of 0.1Hz

  @par Example description

  This example shows how user can get the variation in sine wave frequency from 65Hz to 45Hz using TIM1. 
    
  TIM1CLK is fixed to 4 MHz, the TIM1 Prescaler is equal to 0 so the TIM1 counter
  clock used is 4 MHz. 
  
  The TIM1 Channel 1 is configured as PWM Output Mode 1.
  
  To generate a sine wave of frequency 65.3595, you have to uncomment "#define FREQ_65_3595"
  (User can define only specific frequencies, which are commented out in "main.h"). 
  According to frequency selected by define directive, five parameters will be defined. 
  These parameters will make the corresponding sine wave frequency.
  
  The TIM1_CC1 must be connected to a "Low Pass" filter (Rfilter =1.8Kohm, Cfilter= 0.47uF)

 @code  
  TIM1_CC1 |-><-----[Rfilter=1.8Kohm]----+-------------> to the ossilloscope
                                         |
                  [Cfilter=0.47uF]
                         |
                      ///// GND
 @endcode 

  @par Directory contents

  - TIM1\TIM1_Sinewave_Generation\main.h          Defines macroes for all the 
  frequencies in the range of 45Hz to 65Hz with the resolution of 0.1Hz. 
  - TIM1\TIM1_Sinewave_Generation\main.c          Main file containing the "main" function
  - TIM1\TIM1_Sinewave_Generation\stm8s_conf.h    Library Configuration file
  - TIM1\TIM1_Sinewave_Generation\stm8s_it.c      Interrupt routines source
  - TIM1\TIM1_Sinewave_Generation\stm8s_it.h      Interrupt routines declaration
  


 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
     - Connect  TIM1_CC1 pin (PC.1) to a "Low Pass" filter (Rfilter =1.8Kohm, Cfilter= 0.47uF).
     - Connect the "Low Pass" filter output to an oscilloscope.


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_clk.c
    - stm8s_gpio.c
    - stm8s_tim1.c
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
  - Connect TIM1_CC1 to a "Low Pass" Filter (Rfilter =1.8Kohm, Cfilter= 0.47uF)
  - Connect the "Low Pass" filter output to an oscilloscope.

 
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
