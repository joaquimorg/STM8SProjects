/** @page project Template project for ST Visual Develop (STVD) toolchain with Cosmic compiler

  @par Project description

  This folder contains a standard STVD template workspace that includes 5 
  projects related to the FW Lib supported products.
  Each project include all the user-modifiable files that are necessary 
  to create a new project.
  This project templates can be used by mean of minor updates in the library files
  to run the FWLib examples, or custom user applications. 
  
  @b Tip: You can tailor the provided project template to run this example, for more
          details please refer to "stm8s_fwlib_um.chm" user manual; select "Peripheral Examples"
          then follow the instructions provided in "How to proceed" section.
  
  @par Directories contents

  - project
     - main.c                    Main file containing the "main" function
     - stm8s_conf.h              Library Configuration file
     - stm8s_it.c                Interrupt routines source
     - stm8s_it.h                Interrupt routines declaration
  
  - project\STVD\Cosmic
     - project.stw              Workspace file
     - stm8s103.stp             Project file for STM8S103 product 
     - stm8s105.stp             Project file for STM8S105 product
     - stm8s207.stp             Project file for STM8S207 product
     - stm8s208.stp             Project file for STM8S208 product
     - stm8s903.stp             Project file for STM8S903 product
     - stm8_interrupt_vector.c  Interrupt vectors table


  @par How to use it ?

  - Open the STVD workspace
  - Set active the project related to the used product: Project-> Set Active Project, 
  and choose the desired project 
  - Select your debug instrument: Debug instrument-> Target Settings, select the 
  target you want to use for debug session (Swim Stice or Swim Rlink)
  - Rebuild all files: Build-> Rebuild all. 
  - Load project image: Debug->Start Debugging
  - Run program: Debug->Run (Ctrl+F5)
  
  @b Tip: If it is your first time using STVD, you have to confirm the default 
  toolset and path information that will be used when building your application, 
  to do so:
    - Select Tools-> Options
    - In the Options window click on the Toolset tab
    - Select your toolset from the Toolset list box.
    If the path is incorrect you can type the correct path in the Root Path 
    field, or use the browse button to locate it.
    - In the subpath fields, type the correct subpath if necessary 
  
 
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
