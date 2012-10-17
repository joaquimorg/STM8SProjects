/** @page Networking  Networking with CAN to control Leds

  @par Example description

  This example provides a short description of how to use the CAN peripheral:
  This example shows how to configure the CAN peripheral to send and receive CAN
  frames. The sent frames  are used to control Leds.
  An LCD-Glass is used to give information about : 
   - CAN sender (sender of the frame)
   - CAN receiver (receiver of the frame)
   - Led controlled (set ON)

  The CAN serial communication link is a bus to which a number of units may be
  connected. This number has no theoretical limit. Practically the total number
  of units will be limited by delay times and/or electrical loads on the bus line.

  @note This example is tested with a bus of 3 units. The same program example is loaded
  in all units to send and receive frames.

 The CAN is configured as follow:
    - Bit Rate   = 1 Mbit/s  
    - CAN Clock  = Internal (HSI)
    - ID Filter = All identifiers are allowed
    - RTR = Data
    - DLC = 1 byte

  @par Directory contents

  - CAN\CAN_Networking\main.c                    Main file containing the "main" function
  - CAN\CAN_Networking\main.h                    Main file declaration
  - CAN\CAN_Networking\stm8s_conf.h              Library Configuration file
  - CAN\CAN_Networking\stm8s_it.c                Interrupt routines source 
  - CAN\CAN_Networking\stm8s_it.h                Interrupt routines declaration
  

  @par Hardware environment

 - This example runs only on STM8S208 device.
  
  - This example has been tested with STMicroelectronics STM8/128-EVAL evaluation
    board (STM8S208 device) and ST Visual Develop (STVD) toolchain
  (Cosmic and Raisonance compilers). And can be easily tailored to any other 
  development board.

  - STM8/128-EVAL Set-up
     - Four leds LD1 to LD4 and Key button are used.
     - LCD-Glass is used to monitor the CAN Network.
 
  - If another board is used, please connect the folloing hardware connections :
      - PH3 -> LED1 -> resistor -> ground
      - PH2 -> LED2 -> resistor -> ground
      - PH1 -> LED3 -> resistor -> ground
      - PH0 -> LED4 -> resistor -> ground
      - PC0 -> Key Button -> ground + pull-up on 5V
      - PG1 (CAN Rx) and PG0 (CAN Tx) connected to a DB9 connector using a CAN Bus
        transceiver (eg. L9616).

  - Connect a female/female CAN cable between at least 2 EVAL CAN connectors
  (CN8 on STM8/128-EVAL board) 
    - Connector 1 DB9_PIN2 to Connector 2 DB9_PIN2  (CAN_L)
    - Connector 1 DB9_PIN5 to Connector 2 DB9_PIN5  ( GND )
    - Connector 1 DB9_PIN7 to Connector 2 DB9_PIN7  (CAN_H)


@par How to use it ?

  In order to make the program work, you must do the following :
  - Create a project and setup all project configuration
  - Compile the directory content files and required Library files :
     - stm8s_can.c
     - stm8s_spi.c	(For LCD usage)
     - stm8s_clk.c
     - stm8s_gpio.c
     - stm8s_exti.c
     - mono_lcd.c (under FWLib\examples\Common)
  - Edit stm8s.h file to select the device you are working on (#define STM8S208, in this case).

  @b Tip: You can tailor the provided project template to run this example, for more
          details please refer to "stm8s_fwlib_um.chm" user manual; select "Peripheral Examples"
          then follow the instructions provided in "How to proceed" section.  
   
  - With STVD ?
   - Rebuild all files: Build-> Rebuild all.     
   - Load project image: Debug->Start/Stop Debug Session in the N (total number
    of CAN units) STM8S208 products
   - Run program: Debug->Run (Ctrl+F5)

  - With RIDE ?
   - Open the RIDE workspace
   - Rebuild all files: Project->Build project(Alt+F9)
   - Load project image: Debug->Start (Ctrl+D)
   - Run program: Debug->Run (Ctrl+F9)
   
  - In the first time the LCD shows the message "STM8S CAN  Press Key".
  - (1) By Pressing on Key Button : CAN send a Frame to command LED1 (LD1 ON)
    on the N eval-boards connected to the bus, all other Leds are OFF. 
    The sender LCD shows "STM8S CAN  Sender  LD1 ON".
    The N-1 receivers LCD shows "STM8S CAN  Receiver  LD1 ON".
  - Press on Key Button again to send CAN Frame to command LD2 ON on the N eval-boards,
    all other Leds are OFF.
    The sender LCD shows "STM8S CAN  Sender  LD2 ON".
    The N-1 receivers LCD shows "STM8S CAN  Receiver  LD2 ON".
  - Press on Key Button again to send CAN Frame to command LD3 ON on the N eval-boards,
    all other Leds are OFF.
    The sender LCD shows "STM8S CAN  Sender  LD3 ON".
    The N-1 receivers LCD shows "STM8S CAN  Receiver  LD3 ON".
  - Press on Key Button again to send CAN Frame to command LD4 ON on the N eval-boards,
    all other Leds are OFF.
    The sender LCD shows "STM8S CAN  Sender  LD4 ON".
    The N-1 receivers LCD shows "STM8S CAN  Receiver  LD4 ON".
  - Press on Key Button again to go to step (1)
  @note Any unit in the CAN bus may play the role of sender (by pressing on the
        key) or receiver.

  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
