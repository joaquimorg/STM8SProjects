/**
  ******************************************************************************
  * @file stm8s_i2c.h
  * @brief This file contains all functions prototype and macros for the I2C peripheral.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.1
  * @date 06/05/2009
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_I2C_H
#define __STM8S_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/

/** @addtogroup I2C_Exported_Types
  * @{
  */

/**
  * @brief I2C duty cycle (fast mode only)
  */
typedef enum
{
  I2C_DUTYCYCLE_2    = (u8)0x00,  /*!< Fast mode Tlow/THigh = 2 */
  I2C_DUTYCYCLE_16_9 = (u8)0x40   /*!< Fast mode Tlow/Thigh = 16/9 */
} I2C_DutyCycle_TypeDef;

/**
  * @brief I2C Acknowledgement configuration
  */
typedef enum
{
  I2C_ACK_NONE = (u8)0x00,  /*!< No acknowledge */
  I2C_ACK_CURR = (u8)0x01,  /*!< Acknowledge on the current byte */
  I2C_ACK_NEXT = (u8)0x02   /*!< Acknowledge on the next byte */
} I2C_Ack_TypeDef;

/**
  * @brief I2C Addressing Mode (slave mode only)
  */
typedef enum
{
  I2C_ADDMODE_7BIT  = (u8)0x00,  /*!< 7-bit slave address (10-bit address not acknowledged) */
  I2C_ADDMODE_10BIT = (u8)0x80   /*!< 10-bit slave address (7-bit address not acknowledged) */
} I2C_AddMode_TypeDef;

/**
  * @brief I2C Interrupt sources
  * Warning: the values correspond to the bit position in the ITR register
  */
typedef enum
{
    I2C_IT_ERR     = (u8)0x01, 	/*!< Error Interruption */
    I2C_IT_EVT     = (u8)0x02, 	/*!< Event Interruption */
    I2C_IT_BUF     = (u8)0x04 	/*!< Buffer Interruption */
} I2C_IT_TypeDef;

/**
  * @brief I2C transfer direction
  * Warning: the values correspond to the ADD0 bit position in the OARL register
  */
typedef enum
{
  I2C_DIRECTION_TX = (u8)0x00,  /*!< Transmission direction */
  I2C_DIRECTION_RX = (u8)0x01   /*!< Reception direction */
} I2C_Direction_TypeDef;

/**
  * @brief I2C Flags
  * Elements values convention: 0xXYZZ
  *  X = choice between the SRx registers
  *      X = 1 : SR1
  *      X = 2 : SR2
  *      X = 3 : SR3
  *  Y = choice between different clearing methods
  *      Y = 0 : no action
  *      Y = 1 : clear flag directly in SR2
  *      Y = 2 : read SR1 and then write CR2
  *      Y = 3 : read SR1 and then read SR3
  *      Y = 4 : read SR1 and then read DR
  * ZZ = flag position in the SRx register (same as map file)
  */

typedef enum
{
  /* SR1 register flags */
  I2C_FLAG_TXEMPTY             = (u16)0x1080,  /*!< Transmit Data Register Empty flag */
  I2C_FLAG_RXNOTEMPTY          = (u16)0x1040,  /*!< Read Data Register Not Empty flag */
  I2C_FLAG_STOPDETECTION       = (u16)0x1210,  /*!< Stop detected flag */
  I2C_FLAG_HEADERSENT          = (u16)0x1408,  /*!< 10-bit Header sent flag */
  I2C_FLAG_TRANSFERFINISHED    = (u16)0x1404,  /*!< Data Byte Transfer Finished flag */
  I2C_FLAG_ADDRESSSENTMATCHED  = (u16)0x1302,  /*!< Address Sent/Matched (master/slave) flag */
  I2C_FLAG_STARTDETECTION      = (u16)0x1401,  /*!< Start bit sent flag */

  /* SR2 register flags */
  I2C_FLAG_WAKEUPFROMHALT      = (u16)0x2120,  /*!< Wake Up From Halt Flag */
  I2C_FLAG_OVERRUNUNDERRUN     = (u16)0x2108,  /*!< Overrun/Underrun flag */
  I2C_FLAG_ACKNOWLEDGEFAILURE  = (u16)0x2104,  /*!< Acknowledge Failure Flag */
  I2C_FLAG_ARBITRATIONLOSS     = (u16)0x2102,  /*!< Arbitration Loss Flag */
  I2C_FLAG_BUSERROR            = (u16)0x2101,  /*!< Misplaced Start or Stop condition */

  /* SR3 register flags */
  I2C_FLAG_GENERALCALL         = (u16)0x3010,  /*!< General Call header received Flag */
  I2C_FLAG_TRANSMITTERRECEIVER = (u16)0x3004,  /*!< Transmitter Receiver Flag */
  I2C_FLAG_BUSBUSY             = (u16)0x3002,  /*!< Bus Busy Flag */
  I2C_FLAG_MASTERSLAVE         = (u16)0x3001   /*!< Master Slave Flag */
} I2C_Flag_TypeDef;

/**
  * @brief I2C Pending bits
  * Elements values convention: 0xXYZZ
  *  X = choice between the SRx registers
  *      X = 1 : SR1
  *      X = 2 : SR2
  *  Y = choice between different clearing methods
  *      Y = 0 : no action
  *      Y = 1 : clear flag directly in SR2
  *      Y = 2 : read SR1 and then write CR2
  *      Y = 3 : read SR1 and then read SR3
  *      Y = 4 : read SR1 and then read DR
  * ZZ = flag position in the SRx register (same as map file)
  */

typedef enum
{
    /* SR1 register flags */
    I2C_ITPENDINGBIT_TXEMPTY             = (u16)0x1080, 	/*!< Transmit Data Register Empty  */
    I2C_ITPENDINGBIT_RXNOTEMPTY          = (u16)0x1040, 	/*!< Read Data Register Not Empty  */
    I2C_ITPENDINGBIT_STOPDETECTION       = (u16)0x1210, 	/*!< Stop detected  */
    I2C_ITPENDINGBIT_HEADERSENT          = (u16)0x1408, 	/*!< 10-bit Header sent */
    I2C_ITPENDINGBIT_TRANSFERFINISHED    = (u16)0x1404, 	/*!< Data Byte Transfer Finished  */
    I2C_ITPENDINGBIT_ADDRESSSENTMATCHED  = (u16)0x1302, 	/*!< Address Sent/Matched (master/slave)  */
    I2C_ITPENDINGBIT_STARTDETECTION      = (u16)0x1401, 	/*!< Start bit sent  */

    /* SR2 register flags */
    I2C_ITPENDINGBIT_WAKEUPFROMHALT      = (u16)0x2120, 	/*!< Wake Up From Halt  */
    I2C_ITPENDINGBIT_OVERRUNUNDERRUN     = (u16)0x2108, 	/*!< Overrun/Underrun  */
    I2C_ITPENDINGBIT_ACKNOWLEDGEFAILURE  = (u16)0x2104, 	/*!< Acknowledge Failure  */
    I2C_ITPENDINGBIT_ARBITRATIONLOSS     = (u16)0x2102, 	/*!< Arbitration Loss  */
    I2C_ITPENDINGBIT_BUSERROR            = (u16)0x2101  	/*!< Misplaced Start or Stop condition */
} I2C_ITPendingBit_TypeDef;

/**
  * @brief I2C possible events
  * Values convention: 0xXYZZ
  *  X = choice between master/slave
  *      X = 0 : slave
  *      X = 1 : master
  *  Y = choice between SRx registers
  *      Y = 7 : SR1
  *      Y = 8 : SR2
  * ZZ = flag position in the SRx register (same as map file)
  */
typedef enum
{
  I2C_EVENT_SLAVE_ADDRESS_MATCHED         = (u16)0x0702,  /*!< EV1: ADDR=1 */
  I2C_EVENT_SLAVE_BYTE_RECEIVED           = (u16)0x0740,  /*!< EV2: RXNE=1 */
  I2C_EVENT_SLAVE_BYTE_TRANSMITTED        = (u16)0x0780,  /*!< EV3: TXE=1 */
  I2C_EVENT_SLAVE_ACK_FAILURE             = (u16)0x0804,  /*!< EV3-2: AF=1 */
  I2C_EVENT_SLAVE_STOP_DETECTED           = (u16)0x0710,  /*!< EV4: STOPF=1 */

  I2C_EVENT_MASTER_START_SENT             = (u16)0x1701,  /*!< EV5: SB=1 */
  I2C_EVENT_MASTER_ADDRESS_ACKED          = (u16)0x1702,  /*!< EV6: ADDR=1 */
  I2C_EVENT_MASTER_BYTE_RECEIVED          = (u16)0x1740,  /*!< EV7: RXNE=1 */
  I2C_EVENT_MASTER_BYTE_TRANSMITTING      = (u16)0x1780,  /*!< EV8: TXE=1 */
  I2C_EVENT_MASTER_BYTE_TRANSMITTED 	  = (u16)0x1784,  /*!< EV8-2: TXE=1, BTF=1 */
  I2C_EVENT_MASTER_HEADER_ACKED           = (u16)0x1708   /*!< EV9: ADD10=1 */
} I2C_Event_TypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @addtogroup I2C_Exported_Constants
  * @{
  */
#define I2C_MAX_STANDARD_FREQ ((u32)100000)
#define I2C_MAX_FAST_FREQ     ((u32)400000)
#if defined (STM8S208) || defined (STM8S207) 
 #define I2C_MAX_INPUT_FREQ     ((u8)24)
#else
 #define I2C_MAX_INPUT_FREQ     ((u8)16)
#endif

/**
  *@}
  */

/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/** @addtogroup I2C_Private_Macros
  * @{
  */

/**
  * @brief Macro used by the assert function to check the different functions parameters.
  */

/**
  * @brief  Macro used by the assert function to check the different I2C duty cycles.
  */
#define IS_I2C_DUTYCYCLE_OK(DUTY) \
  (((DUTY) == I2C_DUTYCYCLE_2) || \
   ((DUTY) == I2C_DUTYCYCLE_16_9))

/**
  * @brief  Macro used by the assert function to check the different acknowledgement configuration
  */
#define IS_I2C_ACK_OK(ACK) \
  (((ACK) == I2C_ACK_NONE) || \
   ((ACK) == I2C_ACK_CURR) || \
   ((ACK) == I2C_ACK_NEXT))

/**
  * @brief  Macro used by the assert function to check the different I2C addressing modes.
  */
#define IS_I2C_ADDMODE_OK(ADDMODE) \
  (((ADDMODE) == I2C_ADDMODE_7BIT) || \
   ((ADDMODE) == I2C_ADDMODE_10BIT))

/**
  * @brief  Macro used by the assert function to check the different I2C interrupt types.
  */
#define IS_I2C_INTERRUPT_OK(IT) \
  (((IT) == I2C_IT_ERR)     || \
   ((IT) == I2C_IT_EVT)     || \
   ((IT) == I2C_IT_BUF)     || \
   ((IT) == (I2C_IT_ERR | I2C_IT_EVT)) || \
   ((IT) == (I2C_IT_ERR | I2C_IT_BUF)) || \
   ((IT) == (I2C_IT_EVT | I2C_IT_BUF)) || \
   ((IT) == (I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR)))
/**
  * @brief  Macro used by the assert function to check the different I2C communcation direction.
  */
#define IS_I2C_DIRECTION_OK(DIR) \
  (((DIR) == I2C_DIRECTION_TX) || \
   ((DIR) == I2C_DIRECTION_RX))

/**
  * @brief  Macro used by the assert function to check the different I2C flags.
  */
#define IS_I2C_FLAG_OK(FLAG) \
  (((FLAG) == I2C_FLAG_TXEMPTY)         || \
   ((FLAG) == I2C_FLAG_RXNOTEMPTY)       || \
   ((FLAG) == I2C_FLAG_STOPDETECTION)      || \
   ((FLAG) == I2C_FLAG_HEADERSENT)       || \
   ((FLAG) == I2C_FLAG_TRANSFERFINISHED)   || \
   ((FLAG) == I2C_FLAG_ADDRESSSENTMATCHED)  || \
   ((FLAG) == I2C_FLAG_STARTDETECTION)    || \
   ((FLAG) == I2C_FLAG_WAKEUPFROMHALT)    || \
   ((FLAG) == I2C_FLAG_OVERRUNUNDERRUN)    || \
   ((FLAG) == I2C_FLAG_ACKNOWLEDGEFAILURE)  || \
   ((FLAG) == I2C_FLAG_ARBITRATIONLOSS)    || \
   ((FLAG) == I2C_FLAG_BUSERROR)       || \
   ((FLAG) == I2C_FLAG_GENERALCALL)      || \
   ((FLAG) == I2C_FLAG_TRANSMITTERRECEIVER) || \
   ((FLAG) == I2C_FLAG_BUSBUSY) || \
   ((FLAG) == I2C_FLAG_MASTERSLAVE))
/**
  * @brief  Macro used by the assert function to check the I2C flags to clear.
  */

#define IS_I2C_CLEAR_FLAG_OK(FLAG)\
  (((FLAG) == I2C_FLAG_TXEMPTY)         || \
   ((FLAG) == I2C_FLAG_RXNOTEMPTY)       || \
   ((FLAG) == I2C_FLAG_STOPDETECTION)      || \
   ((FLAG) == I2C_FLAG_HEADERSENT)       || \
   ((FLAG) == I2C_FLAG_TRANSFERFINISHED)   || \
   ((FLAG) == I2C_FLAG_ADDRESSSENTMATCHED)  || \
   ((FLAG) == I2C_FLAG_STARTDETECTION)    || \
   ((FLAG) == I2C_FLAG_WAKEUPFROMHALT)    || \
   ((FLAG) == I2C_FLAG_OVERRUNUNDERRUN)    || \
   ((FLAG) == I2C_FLAG_ACKNOWLEDGEFAILURE)  || \
   ((FLAG) == I2C_FLAG_ARBITRATIONLOSS)    || \
   ((FLAG) == I2C_FLAG_BUSERROR)       || \
   ((FLAG) == I2C_FLAG_GENERALCALL)      || \
   ((FLAG) == I2C_FLAG_TRANSMITTERRECEIVER) || \
   ((FLAG) == I2C_FLAG_BUSBUSY) || \
   ((FLAG) == I2C_FLAG_MASTERSLAVE))

/**
  * @brief  Macro used by the assert function to check the different I2C possible pending bits.
  */
#define IS_I2C_ITPENDINGBIT_OK(ITPENDINGBIT) \
   (((ITPENDINGBIT) == I2C_ITPENDINGBIT_TXEMPTY)   					|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_RXNOTEMPTY) 				|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_STOPDETECTION) 			|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_HEADERSENT) 				|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_TRANSFERFINISHED) 	|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_ADDRESSSENTMATCHED) || \
		((ITPENDINGBIT) == I2C_ITPENDINGBIT_STARTDETECTION) 		|| \
		((ITPENDINGBIT) == I2C_ITPENDINGBIT_WAKEUPFROMHALT)		 	|| \
		((ITPENDINGBIT) == I2C_ITPENDINGBIT_OVERRUNUNDERRUN) 		|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_ACKNOWLEDGEFAILURE) || \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_ARBITRATIONLOSS) 		|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_BUSERROR))
/**
  * @brief  Macro used by the assert function to check the different I2C possible events.
  */
#define IS_I2C_EVENT_OK(EVENT) \
  (((EVENT) == I2C_EVENT_SLAVE_ADDRESS_MATCHED)   || \
   ((EVENT) == I2C_EVENT_SLAVE_BYTE_RECEIVED)    || \
   ((EVENT) == I2C_EVENT_SLAVE_BYTE_TRANSMITTED)  || \
   ((EVENT) == I2C_EVENT_SLAVE_ACK_FAILURE)     || \
   ((EVENT) == I2C_EVENT_SLAVE_STOP_DETECTED)    || \
   ((EVENT) == I2C_EVENT_MASTER_START_SENT)     || \
   ((EVENT) == I2C_EVENT_MASTER_ADDRESS_ACKED)   || \
   ((EVENT) == I2C_EVENT_MASTER_BYTE_RECEIVED)   || \
   ((EVENT) == I2C_EVENT_MASTER_BYTE_TRANSMITTING) || \
   ((EVENT) == I2C_EVENT_MASTER_BYTE_TRANSMITTED) || \
   ((EVENT) == I2C_EVENT_MASTER_HEADER_ACKED))

/**
  * @brief  Macro used by the assert function to check the different I2C possible own address.
  */
#define IS_I2C_OWN_ADDRESS_OK(ADDRESS) \
  ((ADDRESS) <= (u16)0x03FF)

/* The address must be even */
#define IS_I2C_ADDRESS_OK(ADD) \
  (((ADD) & (u8)0x01) == (u8)0x00)

/**
  * @brief  Macro used by the assert function to check that I2C Input clock frequency must be between 1MHz and 50MHz.
  */
#define IS_I2C_INPUT_CLOCK_FREQ_OK(FREQ) \
  (((FREQ) >= (u8)1) && ((FREQ) <= I2C_MAX_INPUT_FREQ))


/**
  * @brief  Macro used by the assert function to check that I2C Output clock frequency must be between 1Hz and 400kHz.
  */
#define IS_I2C_OUTPUT_CLOCK_FREQ_OK(FREQ) \
  (((FREQ) >= (u8)1) && ((FREQ) <= I2C_MAX_FAST_FREQ))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */
/** @addtogroup I2C_Exported_Functions
  * @{
  */

void I2C_DeInit(void);
void I2C_Init(u32 OutputClockFrequencyHz, u16 OwnAddress, I2C_DutyCycle_TypeDef DutyCycle, I2C_Ack_TypeDef Ack, I2C_AddMode_TypeDef AddMode, u8 InputClockFrequencyMHz );
void I2C_Cmd(FunctionalState NewState);
void I2C_GeneralCallCmd(FunctionalState NewState);
void I2C_GenerateSTART(FunctionalState NewState);
void I2C_GenerateSTOP(FunctionalState NewState);
void I2C_SoftwareResetCmd(FunctionalState NewState);
void I2C_StretchClockCmd(FunctionalState NewState);
void I2C_AcknowledgeConfig(I2C_Ack_TypeDef Ack);
void I2C_FastModeDutyCycleConfig(I2C_DutyCycle_TypeDef DutyCycle);
void I2C_ITConfig(I2C_IT_TypeDef ITName, FunctionalState NewState);
ErrorStatus I2C_CheckEvent(I2C_Event_TypeDef I2C_Event);
u8 I2C_ReceiveData(void);
void I2C_Send7bitAddress(u8 Address, I2C_Direction_TypeDef Direction);
void I2C_SendData(u8 Data);
FlagStatus I2C_GetFlagStatus(I2C_Flag_TypeDef Flag);
void I2C_ClearFlag(I2C_Flag_TypeDef Flag);
ITStatus I2C_GetITStatus(I2C_ITPendingBit_TypeDef ITPendingBit);
void I2C_ClearITPendingBit(I2C_ITPendingBit_TypeDef ITPendingBit);

/**
  * @}
  */

#endif /* __STM8S_I2C_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
