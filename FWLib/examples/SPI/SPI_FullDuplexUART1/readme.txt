/** @page SPI_FullDuplexUART1 Full duplex master/slave communication with UART1.


  @par Example description

  This example provides a short description of how to make a master/slave full duplex 
  communication between the SPI and the UART1, the last is configured in SPI master mode.
  
  The UART1 sends data from TxBuffer1 buffer to SPI using TXE flag.
  Data received using RXNE flag by SPI is stored in RxBuffer2 then compared with
  the sent ones and the result of this comparison is stored in the "TransferStatus1" 
  variable.
 
  In the same Time, the SPI sends data from TxBuffer2 buffer to UART1 using TXE flag.
  Data received using RXNE flag by UART1 is stored in RxBuffer1 then compared with
  the sent ones and the result of this comparison is stored in the "TransferStatus2" 
  variable. 

  @par Directory contents

  - SPI\SPI_FullDuplexUART1\main.c                    Main file containing the "main" function
  - SPI\SPI_FullDuplexUART1\stm8s_conf.h              Library Configuration file
  - SPI\SPI_FullDuplexUART1\stm8s_it.c                Interrupt routines source (not used in this example)
  - SPI\SPI_FullDuplexUART1\stm8s_it.h                Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
      - Connect UART1 and SPI pins as follows:
         - UART1 TX (PA5) to SPI MOSI (PC6)
         - UART1 RX (PA4) to SPI MISO (PC7)
         - UART1 CLK (PA6) to SPI SCK  (PC5)


  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :

    - stm8s_uart1.c
    - stm8s_clk.c
    - stm8s_spi.c
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
