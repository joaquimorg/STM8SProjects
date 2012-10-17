/** @page FLASH_BlockOperationOnFlashProgramMemory program Block on FLASH Program memory


  @par Example description

  This example shows the possibility to continue execution from RAM while program operation on FLASH Program is on going 
  - Program the last block in the FLASH Program memory .
  - Check program block (Read byte).

  
  @par Directory contents

  - FLASH\FLASH_BlockOperationOnFlashProgramMemory\main.c                    Main file containing the "main" function
  - FLASH\FLASH_BlockOperationOnFlashProgramMemory\stm8s_conf.h              Library Configuration file
  - FLASH\FLASH_BlockOperationOnFlashProgramMemory\stm8s_it.c                Interrupt routines source (not used in this example)
  - FLASH\FLASH_BlockOperationOnFlashProgramMemory\stm8s_it.h                Interrupt routines declaration
  - FLASH\FLASH_BlockOperationOnFlashProgramMemory\stm8s208.lkf              STM8S208 and STM8S207 Linker file for STVD and cosmic compiler
  - FLASH\FLASH_BlockOperationOnFlashProgramMemory\stm8s105.lkf              STM8S105 Linker file for STVD and cosmic compiler
  - FLASH\FLASH_BlockOperationOnFlashProgramMemory\stm8s103.lkf              STM8S103 and STM8S903 Linker file for STVD and cosmic compiler	



  @par Hardware and Software environment

  - This example was developped to run on STM8S208 device with STMicroelectronics 
  STM8/128-EVAL evaluation board.
  
  - It can be tailored to run on any other supported device and developpement board.
  
  - This example has been tested with ST Visual Develop (STVD) toolchain (Cosmic
  compiler).

  @par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Use the linker file given in the directory (stm8sxxx.lkf)
    -> In STVD Select Project\Settings\Linker\Category "input" then unchecks the "Auto" box 
       and in the "Script Lkf file name" field add the dedicated linker file.
    - When using the STM8S208 or STM8S207 device select the stm8s208.lkf linker file
    - When using the STM8S105 device select the stm8s105.lkf linker file
    - When using the STM8S103 or STM8S903 device select the stm8s103.lkf linker file

  - Add the required Library files:
    - stm8s_flash.c
  - Edit stm8s.h file to select the device you are working on (#define STM8S208, in this case).

  @b Tip: You can tailor the provided project template to run this example, for more
          details please refer to "stm8s_fwlib_um.chm" user manual; select "Peripheral Examples"
          then follow the instructions provided in "How to proceed" section.  
   
  - With STVD ?
   - Rebuild all files: Build-> Rebuild all.     
   - Load project image: Debug->Start Debugging
   - Run program: Debug->Run (Ctrl+F5)

  - You can check the value of the bytes at the specified addresses by using the memory window in STVD.
  - The data read from the data memory is compared with the expected values.
  	The result of this comparison is stored in the "OperationStatus" variable.
   
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
