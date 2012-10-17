/** @page UART1_Synchronous A basic communication between UART1 and SPI using flags


  @par Example description

  This example provides a basic communication between UART1 (Synchronous mode) 
  and SPI using flags.

  First, the UART1 sends data from TxBuffer1 buffer to SPI using TXE flag.
  Data received using RXNE flag by SPI is stored in RxBuffer2 then compared with
  the sent ones and the result of this comparison is stored in the "TransferStatus1" 
  variable.
 
  Then, the SPI sends data from TxBuffer2 buffer to UART1 using TXE flag.
  Data received using RXNE flag by UART1 is stored in RxBuffer1 then compared with
  the sent ones and the result of this comparison is stored in the "TransferStatus2" 
  variable.   


  The UART1 is configured as follows:
    - BaudRate = 9600 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Receive and transmit enabled
    - UART1 Clock enabled
    - UART1 CPOL: Clock is active low
    - UART1 CPHA: Data is captured in middle of data bit 
    - UART1 LastBit: The clock pulse of the last data bit is output to the SCLK pin

  The SPI is configured as follows:
    - Direction = 2 Lines FullDuplex
    - Mode = Slave Mode
    - DataSize = 8 Bits
    - CPOL = Clock is active low
    - CPHA = Data is captured on the first edge 
    - NSS = NSS Software
    - First Bit = First Bit is the LSB   


  @par Directory contents

  - UART1\UART1_Synchronous\main.c                    Main file containing the "main" function
  - UART1\UART1_Synchronous\stm8s_conf.h              Library Configuration file
  - UART1\UART1_Synchronous\stm8s_it.c                Interrupt routines source (not used in this example)
  - UART1\UART1_Synchronous\stm8s_it.h                Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
     - Connect UART1 TX (PA5) to SPI MOSI (PC6)
     - Connect UART1 RX (PA4) to SPI MISO (PC7)
     - Connect UART1 CK (PA6) to SPI SCK (PC5)


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_clk.c
    - stm8s_spi.c
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
