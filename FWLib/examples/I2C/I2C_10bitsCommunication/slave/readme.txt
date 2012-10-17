/** @page I2C_10bitsSlaveCommunication Slave receiver Communication between two I2C peripherals


  @par Example description

  This example provides a short description of how to communicate with the I2C:
  - In 10bit addressing mode slave receiver with an own address set to 0x102.


  @par Directory contents

  - I2C\I2C_10bitsCommunication\slave\main.c                    Main file containing the "main" functions 
  - I2C\I2C_10bitsCommunication\slave\stm8s_conf.h              Library Configuration file
  - I2C\I2C_10bitsCommunication\slave\stm8s_it.c                Interrupt routines source (not used in this example)
  - I2C\I2C_10bitsCommunication\slave\stm8s_it.h                Interrupt routines declaration
  

 @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

  - This examples needs two STM8/128-EVAL boards (or equivalent hardware)
    - Connect the STM8S I2C to a second I2C 
      - SDA pin (PE2) -> SDA pin.
      - SCL pin (PE1) -> SCL pin.
      - On SDA & SCL pins do not forget pull-up resistors (4.7k ohms)
      --> already present on the STM8/128-EVAL board.
      
	  	  

  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_clk.c
    - stm8s_i2c.c
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
   - Run program: Debug->Run (Ctrl+F9).
  - The four LEDs toggle when the I2C communication is terminated.
    
  */
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
