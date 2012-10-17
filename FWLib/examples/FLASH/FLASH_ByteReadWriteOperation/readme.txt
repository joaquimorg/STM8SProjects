/** @page FLASH_ByteReadWriteOperation Read, Write, Erase byte


  @par Example description

  This example provides a short description on how to use FLASH Read/Write/Erase byte modes:
  - Read a byte at a specified address (0x40A5).
  - Write complement value (of previous byte value) at address +1 (0x40A6).
  - Check programmed byte (using read function)
  - Erase 2 bytes at address & address+1
  - Check 2 bytes are erased (value must be equal to 0x00).


  @par Directory contents

  - FLASH\FLASH_ByteReadWriteOperation\main.c                    Main file containing the "main" function
  - FLASH\FLASH_ByteReadWriteOperation\stm8s_conf.h              Library Configuration file
  - FLASH\FLASH_ByteReadWriteOperation\stm8s_it.c                Interrupt routines source (not used in this example)
  - FLASH\FLASH_ByteReadWriteOperation\stm8s_it.h                Interrupt routines declaration
  

  @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers).

 
  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Add the required Library files :
    - stm8s_flash.c
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
  
  - The data read from the data memory is compared with the expected values.
    The result of this comparison is stored in the "OperationStatus" variable.
   
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
