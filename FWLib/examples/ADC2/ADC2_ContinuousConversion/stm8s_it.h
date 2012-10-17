/**
  ******************************************************************************
  * @file stm8s_it.h
  * @brief This file contains the headers of the interrupt handlers, for Cosmic
  *	compiler.
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
#ifndef __STM8S_IT_H
#define __STM8S_IT_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef _COSMIC_

 void _stext(void); /* RESET startup routine */
 @far @interrupt void NonHandledInterrupt(void);
 @far @interrupt void TRAP_IRQHandler(void); /* TRAP */
 @far @interrupt void TLI_IRQHandler(void); /* TLI */
 @far @interrupt void AWU_IRQHandler(void); /* AWU */
 @far @interrupt void CLK_IRQHandler(void); /* CLOCK */
 @far @interrupt void EXTI_PORTA_IRQHandler(void); /* EXTI PORTA */
 @far @interrupt void EXTI_PORTB_IRQHandler(void); /* EXTI PORTB */
 @far @interrupt void EXTI_PORTC_IRQHandler(void); /* EXTI PORTC */
 @far @interrupt void EXTI_PORTD_IRQHandler(void); /* EXTI PORTD */
 @far @interrupt void EXTI_PORTE_IRQHandler(void); /* EXTI PORTE */

#ifdef STM8S903
 @far @interrupt void EXTI_PORTF_IRQHandler(void); /* EXTI PORTF */
#endif /*STM8S903*/

#ifdef STM8S208
 @far @interrupt void CAN_RX_IRQHandler(void); /* CAN RX */
 @far @interrupt void CAN_TX_IRQHandler(void); /* CAN TX/ER/SC */
#endif /*STM8S208*/

 @far @interrupt void SPI_IRQHandler(void); /* SPI */
 @far @interrupt void TIM1_CAP_COM_IRQHandler(void); /* TIM1 CAP/COM */
 @far @interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void); /* TIM1 UPD/OVF/TRG/BRK */

#ifdef STM8S903
 @far @interrupt void TIM5_UPD_OVF_BRK_TRG_IRQHandler(void); /* TIM5 UPD/OVF/BRK/TRG */
 @far @interrupt void TIM5_CAP_COM_IRQHandler(void); /* TIM5 CAP/COM */
#else /*STM8S208, STM8S207, STM8S105 or STM8S103*/
 @far @interrupt void TIM2_UPD_OVF_BRK_IRQHandler(void); /* TIM2 UPD/OVF/BRK */
 @far @interrupt void TIM2_CAP_COM_IRQHandler(void); /* TIM2 CAP/COM */
#endif /*STM8S903*/

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)
 @far @interrupt void TIM3_UPD_OVF_BRK_IRQHandler(void); /* TIM3 UPD/OVF/BRK */
 @far @interrupt void TIM3_CAP_COM_IRQHandler(void); /* TIM3 CAP/COM */
#endif /*STM8S208, STM8S207 or STM8S105*/

#ifndef STM8S105
 @far @interrupt void UART1_TX_IRQHandler(void); /* UART1 TX */
 @far @interrupt void UART1_RX_IRQHandler(void); /* UART1 RX */
#endif /*STM8S105*/

 @far @interrupt void I2C_IRQHandler(void); /* I2C */

#ifdef STM8S105
 @far @interrupt void UART2_RX_IRQHandler(void); /* UART2 RX */
 @far @interrupt void UART2_TX_IRQHandler(void); /* UART2 TX */
#endif /* STM8S105*/

#if defined(STM8S207) || defined(STM8S208)
 @far @interrupt void UART3_RX_IRQHandler(void); /* UART3 RX */
 @far @interrupt void UART3_TX_IRQHandler(void); /* UART3 TX */
#endif /*STM8S207, STM8S208*/

#if defined(STM8S207) || defined(STM8S208)
 @far @interrupt void ADC2_IRQHandler(void); /* ADC2 */
#else /*STM8S105, STM8S103 or STM8S903*/
 @far @interrupt void ADC1_IRQHandler(void); /* ADC1 */
#endif /*STM8S207, STM8S208*/

#ifdef STM8S903
 @far @interrupt void TIM6_UPD_OVF_TRG_IRQHandler(void); /* TIM6 UPD/OVF/TRG */
#else /*STM8S208, STM8S207, STM8S105 or STM8S103*/
 @far @interrupt void TIM4_UPD_OVF_IRQHandler(void); /* TIM4 UPD/OVF */
#endif /*STM8S903*/
 @far @interrupt void EEPROM_EEC_IRQHandler(void); /* EEPROM ECC CORRECTION */
#endif /* _COSMIC_ */

#endif /* __STM8S_IT_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
