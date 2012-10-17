/** @page FLASH_WriteWordOperation FLASH Write word, Erase byte


  @par Example description

  This example provides a short description on how to use FLASH Program word mode and Erase byte modes:
  - Program a word at a specified address (0x4100).
  - Check program word (Read byte).
  - Erase word using Erase byte.
  - Check erase.


  @par Directory contents

  - FLASH\FLASH_WriteWordOperation\main.c                    Main file containing the "main" function
  - FLASH\FLASH_WriteWordOperation\stm8s_conf.h              Library Configuration file
  - FLASH\FLASH_WriteWordOperation\stm8s_it.c                Interrupt routines source (not used in this example)
  - FLASH\FLASH_WriteWordOperation\stm8s_it.h                Interrupt routines declaration
  

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
