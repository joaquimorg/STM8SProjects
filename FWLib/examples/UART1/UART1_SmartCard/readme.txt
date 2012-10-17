/** @page UART1_SmartCard Smart Card mode


  @par Example description

  This example provides a description of how  to use the UART1 in Smart Card mode. 
  The example gives only the possibility to read the ATR and decode it into predefined buffer.

  First, the code  waits for a card insertion. If card detected, a reset signal is applied to 
  the card through its reset pin.

  As response to this reset, the card transmits the ATR which is stored in a predefined buffer.
  Once the ATR is received, it is decoded and stored in a specific structure (SC_A2R) and the 
  card protocol type is stored in a variable (ATRDecodeStatus).

  The UART1 Smartcard mode supports asynchronous protocol Smartcards as defined in the ISO 7816-3
  (Class A) standard, please refer to the ISO 7816-3 specification or to the application note AN2646
  (Smartcard interface with the STM8S20x) for more details...

  The UART1 is configured as follows:
    - Word Length = 9 Bits
    - 1.5 Stop Bit
    - Even parity
    - BaudRate = 10752 baud
    - Transmit and receive enabled
    - UART1 Clock enabled
    - UART1 CPOL: Clock is active low
    - UART1 CPHA: Data is captured on the second edge 
    - UART1 LastBit: The clock pulse of the last data bit is not output to 
                     the SCLK pin (Last bit Disabled).

	
  @par Directory contents

  - UART1\UART1_SmartCard\main.c                    Main file containing the "main" function
  - UART1\UART1_SmartCard\stm8s_conf.h              Library Configuration file
  - UART1\UART1_SmartCard\stm8s_it.c                Interrupt routines source (not used in this example)
  - UART1\UART1_SmartCard\stm8s_it.h                Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
    - Plug a smart card (ISO7816-3 T=0 compatible) into the dedicated smart card
      connector CN12.  

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
  - Cheek the card protocol type variable(ATRDecodeStatus)
  
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
