/** @page project_r Template project for RIDE toolchain with Raisonance compiler

  @par Project description

  This folder contains a standard RIDE template workspace that includes 4 
  projects related to the FW Lib supported products.
  Each project include all the user-modifiable files that are necessary 
  to create a new project.
  This project templates can be used by mean of minor updates in the library files
  to run the FWLib examples, or custom user applications. 
  
  @b Tip: You can tailor the provided project template to run this example, for more
          details please refer to "stm8s_fwlib_um.chm" user manual; select "Peripheral Examples"
          then follow the instructions provided in "How to proceed" section.
  
  @par Directories contents

  - project:
     - main.c                    Main file containing the "main" function
     - stm8s_conf.h              Library Configuration file
     - stm8s_it.c                Interrupt routines source
  
  - project\RIDE
     - project.rprj              Workspace file
     - stm8s103.rapp             Project file for STM8S103 product 
     - stm8s105.rapp             Project file for STM8S105 product
     - stm8s207.rapp             Project file for STM8S207 product
     - stm8s208.rapp             Project file for STM8S208 product
     - STM8_OB.asm               File required for option bytes configuration



  @par How to use it ?

  - Open the RIDE workspace (project.rprj)
  - Choose the desired project related to the used product from the list: 
  in the "view Project Explorer" --> Right click on the desired project 
  and select "Set as StartUp application" 
  - Rebuild all files: : in the "view Project Explorer" Right click on 
  the active application and select "Build" 
  - Load project image: Debug->Load(Ctrl+L)
  - Run program: Debug->Start (Ctrl+D)

  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
