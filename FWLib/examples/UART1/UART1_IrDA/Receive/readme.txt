/** @page UART1_IrDA_Receiver UART1 IrDA receive mode communication


  @par Example description

  This example provides a basic communication UART1 IrDA receive mode.
  Four leds are used to show which byte is received. 

  - LED4 toggle when 0x04 (Joystick Up pressed in Transmitter board) is received
  - LED3 toggle when 0x05 (Joystick Down pressed in Transmitter board)is received
  - LED2 toggle when 0x03 (Joystick Left pressed in Transmitter board)is received
  - LED1 toggle when 0x02 (Joystick Right pressed in Transmitter board)is received
  - LED1 to LED4 toggle when 0x01 is received (Joystick Sel pressed in Transmitter board)       


  UART1 configured as follow:
   - BaudRate = 9600 baud  
   - Word Length = 8 Bits
   - One Stop Bit
   - No parity
   - Receive and transmit enabled

  The example provides two IrDA program: transmitter&receiver and requires two boards
  to be able to run the full demonstration:
   - one board will act as IrDA transmitter
   - one board will act as IrDA receiver
 

  @par Directory contents

  - UART1\UART1_IrDA\Receive\main.c                    Main file containing the "main" function
  - UART1\UART1_IrDA\Receive\stm8s_conf.h              Library Configuration file
  - UART1\UART1_IrDA\Receive\stm8s_it.c                Interrupt routines source (not used in this example)
  - UART1\UART1_IrDA\Receive\stm8s_it.h                Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
     - IrDA tranceiver (U14) is used, make sure that JP10 jumper must be fitted.
     - Four leds LD1 to LD4 are used. 


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_clk.c
    - stm8s_gpio.c
    - stm8s_uart1.c
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
    
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
