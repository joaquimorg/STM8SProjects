/** @page TIM1_ComplementarySignals_DeadTime_Break_Lock Generating 6 complementary PWM signals with dead time insertion, break capability and lock option using TIM1

  @par Example description

  This example shows how to configure the TIM1 peripheral to generate three
  complementary TIM1 signals, to insert a defined dead time value, to use the
  break feature and to lock the desired parameters.

  
  TIM1CLK is fixed to 2 MHz, the TIM1 Prescaler is equal to 0 so the TIM1 counter
  clock used is 2 MHz. (By default, fTimer1= fMaster/8 )
  TIM1 frequency is defined as follow:
  TIM1 frequency = TIM1 counter clock / (TIM1_Period + 1) = 30.52 Hz.
  
  The Three Duty cycles are computed as the following description: 
  
  TIM1 Channel1 duty cycle = TIM1_CCR1 / (TIM1_Period + 1) = 50% 
  TIM1 Channel1N duty cycle = (TIM1_Period - TIM1_CCR1) / (TIM1_Period + 1) = 50%
  
  TIM1 Channel2 duty cycle = TIM1_CCR2 / (TIM1_Period + 1) = 25%
  TIM1 Channel2N duty cycle = (TIM1_Period - TIM1_CCR2) / (TIM1_Period + 1) = 75% 
  
  TIM1 Channel3 duty cycle = TIM1_CCR3 / TIM1_Period = 12.5% 
  TIM1 Channel3N duty cycle = (TIM1_Period - TIM1_CCR3) / (TIM1_Period + 1) = 87.5%
  
  A dead time of 59 us is inserted between the different complementary signals, 
  and the Lock level 1 is selected.
  The break Polarity is used at High level.

  DTG[7:0]= 117 = (01110101)binary  => DTG[7:5]=0xx => Dead Time = DTG[7:0]xTIM1Clk
  TIM1Clk =  500 ns (2MHz) . So TIM1 Dead time = 58.5 us
  
  The TIM1 waveform can be displayed using an oscilloscope. 
  
  @note  Connect the TIM1 break TIM1_BKIN pin (PE.3) to the GND.
  To generate a break event, switch this pin level from 0V to 3.3V.


  @par Directory contents

  - TIM1\TIM1_ComplementarySignals_DeadTime_Break_Lock\main.c                   Main file containing the "main" function
  - TIM1\TIM1_ComplementarySignals_DeadTime_Break_Lock\stm8s_conf.h             Library Configuration file
  - TIM1\TIM1_ComplementarySignals_DeadTime_Break_Lock\stm8s_it.c               Interrupt routines source (not used in this example)
  - TIM1\TIM1_ComplementarySignals_DeadTime_Break_Lock\stm8s_it.h               Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - Connect TIM1 pins to an oscilloscope as follows:
    - TIM1_CH1  pin (PC.1)  
    - TIM1_CH1N pin (PH.7)  
    - TIM1_CH2  pin (PC.2)  
    - TIM1_CH2N pin (PH.6)  
    - TIM1_CH3  pin (PC.3)  
   - TIM1_CH3N pin (PH.5)


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_tim1.c
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
  - Connect TIM1_BKIN pin(PE.3) to the GND.
  - Connect the TIM1 pins to an oscilloscope
  
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
