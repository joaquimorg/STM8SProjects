/** @page SPI_FastCommunicationMicroSD Fast communication with a MicroSD memory card


  @par Example description

  This example provides a short description of how to make a communication (Write & Read data)  
  between the SPI and a MicroSD memory card like the one present on the STM8/128-EVAL evaluation board.
	
  First, Fill the Tx_Buffer and write them into the MicroSD memory followed by a read of the written data.
  Then data read from the MicroSD memory stored in the Rx_Buffer are compared with the expected values 
  of the Tx_Buffer. The result of this comparison is stored in the "TransferStatus" variable.

 @note To avoid the conflict between LCD and MicroSD memory card on the STM8/128-EVAL evaluation board 
       the LCD should be disabled.

 @par Directory contents

  - SPI\SPI_FastCommunicationMicroSD\main.c                    Main file containing the "main" function
  - SPI\SPI_FastCommunicationMicroSD\stm8s_conf.h              Library Configuration file
  - SPI\SPI_FastCommunicationMicroSD\stm8s_it.c                Interrupt routines source (not used in this example)
  - SPI\SPI_FastCommunicationMicroSD\stm8s_it.h                Interrupt routines declaration
    

 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - STM8/128-EVAL Set-up
     - Plug a MicroSD memory card into the dedicated connector (CN9).

  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_clk.c
    - stm8s_gpio.c
    - stm8s_spi.c    
    - mono_lcd.c (under FWLib\examples\Common)
    - microsd.c (under FWLib\examples\Common)
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
  - If the program is pass, the four LEDs toggles. Otherwise only the LED1 toggles.
  
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
