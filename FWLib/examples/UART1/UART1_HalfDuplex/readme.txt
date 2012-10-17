 /** @page UART1_HalfDuplex UART1 in Half-Duplex mode


  @par Example description

  This example provides a basic Half-Duplex communication between UART1 and UART3 using flags.

  When running the transmitter, the UART1 sends data from TxBuffer1 buffer to UART3 using TXE flag.
  Data received using RXNE flag by UART3 is stored in RxBuffer2 then compared with
  the sent ones and the result of this comparison is stored in the "TransferStatus1" 
  variable.
 
  When running the receiver, the UART3 sends data from TxBuffer2 buffer to UART1 using TXE flag.
  Data received using RXNE flag by UART1 is stored in RxBuffer1 then compared with
  the sent ones and the result of this comparison is stored in the "TransferStatus2" 
  variable. 

  The UART3 and UART1 are configured as follows:
   - BaudRate = 230400 baud  
   - Word Length = 8 Bits
   - One Stop Bit
   - None parity
   - Receive and transmit enabled
   - UART1 Clock disabled


  @par Directory contents

  - UART1\UART1_HalfDuplex\main.c                   Main file containing the "main" function
  - UART1\UART1_HalfDuplex\stm8s_conf.h             Library Configuration file
  - UART1\UART1_HalfDuplex\stm8s_it.c               Interrupt routines source (not used in this example)
  - UART1\UART1_HalfDuplex\stm8s_it.h               Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - To run the transmitter: Connect the UART1_TX (PA5) on the UART3_RX(PD6),
     uncomment the line "#define sender" and comment the line "#define receiver" in main.c 
 
  - To run the receiver: Connect the UART1_TX (PA5)on the UART3_TX (PD5), 
     uncomment the line "#define receiver" and comment the line "#define sender" in main.c

 
  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_clk.c
    - stm8s_gpio.c
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
  - Communication result in Receiver/ Transmitter modes can be checked via respectively 
  TransferStatus2 and TransferStatus1 variables.
  
 
  */
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
