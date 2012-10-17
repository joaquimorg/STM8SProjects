/** @page UART1_MultiProcessor Use the UART1 in multi-processor mode


  @par Example description

  This example provides a description of how to use the UART1 in multi-processor mode.
  First, the UART1 and UART3 addresses are set to 0x1 and 0x2. 

  The UART1 sends continuously the byte 0x33 to the UART3. When UART3 receives byte (0x33), LED1 to LED4 toggle.

  When a falling edge is applied on EXTI PORTB (Joystick up push-button on STM8-EVAL board is 
  pressed), an interrupt is generated and in the EXTI_PORTB_IRQHandler routine, the UART3 is put
  into mute mode and stays in this mode(no LED toggling) until a rising edge is applied on EXTI
  PORTC(Key push-button on STM8-EVAL board is pressed). In this interrupt routine the UART1 sends
  the address mark data (0x102) to wake up the UART3. The LED restarts toggling.

  UART1 and UART3 are configured as follows:
   - BaudRate = 9600 baud  
   - Word Length = 9 Bits
   - One Stop Bit
   - No parity
   - Receive and transmit enabled
   - UART1 Clock disabled


  @par Directory contents

  - UART1\UART1_MultiProcessor\main.c                    Main file containing the "main" function
  - UART1\UART1_MultiProcessor\stm8s_conf.h              Library Configuration file
  - UART1\UART1_MultiProcessor\stm8s_it.c                Interrupt routines source (not used in this example)
  - UART1\UART1_MultiProcessor\stm8s_it.h                Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
    - Connect UART1_Tx(PA.05) to UART3_Rx(PD.06) and UART1_Rx(PA.04) to UART3_Tx(PD.05)
      or connect a null-modem female/female RS232 cable between CN15 and CN13.
     - Four leds LD1 to LD4, Key and Joystick UP push-button are used.
 

  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_clk.c
    - stm8s_gpio.c
    - stm8s_exti.c
    - stm8s_uart1.c
    - stm8s_uart3.c
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
