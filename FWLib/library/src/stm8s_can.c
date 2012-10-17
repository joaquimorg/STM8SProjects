/**
  ******************************************************************************
  * @file stm8s_can.c
  * @brief This file contains all the functions for the CAN peripheral.
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

/* Includes ------------------------------------------------------------------*/
#include "stm8s_can.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CAN_IDLIST_IDMASK_MASK ((u8) 0x55)
#define CAN_IDMASK_IDLIST_MASK ((u8) 0xAA)
#define CAN_MODE_MASK ((u8) 0x03)
#define CAN_ACKNOWLEDGE_TIMEOUT ((u16)0xFFFF)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
vu32 _Id;
vu8 _IDE;
vu8 _RTR;
vu8 _DLC;
vu8 _Data[8];
vu8 _FMI;
/* Private function prototypes -----------------------------------------------*/
static ITStatus CheckITStatus(u8 CAN_Reg, u8 It_Bit);

/* Private functions ---------------------------------------------------------*/
/**
  * @addtogroup CAN_Public_Functions
  * @{
  */

/**
  * @brief Deinitializes the CAN peripheral registers to their default reset values.
  * @par Parameters:
  * None
  * @retval None
  */
void CAN_DeInit(void)
{
    /* Request initialisation */
    CAN->MCR = CAN_MCR_INRQ;
    CAN->PSR = CAN_Page_Config;
    CAN_OperatingModeRequest(CAN_OperatingMode_Initialization);
    CAN->Page.Config.ESR = CAN_ESR_RESET_VALUE;
    CAN->Page.Config.EIER = CAN_EIER_RESET_VALUE;
    CAN->Page.Config.BTR1 = CAN_BTR1_RESET_VALUE;
    CAN->Page.Config.BTR2 = CAN_BTR2_RESET_VALUE;
    CAN->Page.Config.FMR1 = CAN_FMR1_RESET_VALUE;
    CAN->Page.Config.FMR2 = CAN_FMR2_RESET_VALUE;
    CAN->Page.Config.FCR1 = CAN_FCR_RESET_VALUE;
    CAN->Page.Config.FCR2 = CAN_FCR_RESET_VALUE;
    CAN->Page.Config.FCR3 = CAN_FCR_RESET_VALUE;
    CAN_OperatingModeRequest(CAN_OperatingMode_Normal);
    CAN->PSR = CAN_Page_RxFifo;
    CAN->Page.RxFIFO.MDLCR = CAN_MDLC_RESET_VALUE;
    CAN->PSR = CAN_Page_TxMailBox0;
    CAN->Page.TxMailbox.MDLCR = CAN_MDLC_RESET_VALUE;
    CAN->PSR = CAN_Page_TxMailBox1;
    CAN->Page.TxMailbox.MDLCR = CAN_MDLC_RESET_VALUE;
    CAN->PSR = CAN_Page_TxMailBox2;
    CAN->Page.TxMailbox.MDLCR = CAN_MDLC_RESET_VALUE;

    CAN->MCR = CAN_MCR_RESET_VALUE;
    CAN->MSR = (u8)(~CAN_MSR_RESET_VALUE);/* rc_w1 */
    CAN->TSR = (u8)(~CAN_TSR_RESET_VALUE);/* rc_w1 */
    CAN->RFR = (u8)(~CAN_RFR_RESET_VALUE);/* rc_w1 */
    CAN->IER = CAN_IER_RESET_VALUE;
    CAN->DGR = CAN_DGR_RESET_VALUE;
    CAN->PSR = CAN_PSR_RESET_VALUE;
}
/**
  * @brief Initializes the CAN peripheral according to the specified parameters.
  * @param[in] CAN_MasterCtrl : Master control option, can be one or a combinaison of @ref CAN_MasterCtrl_TypeDef.
  * @param[in] CAN_Mode : CAN mode , can be one of @ref CAN_Mode_TypeDef.
  * @param[in] CAN_SynJumpWidth : CAN Synchronisation Jump Width , can be one of @ref CAN_SynJumpWidth_TypeDef.
  * @param[in] CAN_BitSeg1 : CAN bit segment 1 , can be one of @ref CAN_BitSeg1_TypeDef.
  * @param[in] CAN_BitSeg2 : CAN bit segment 2 , can be one of @ref CAN_BitSeg2_TypeDef.
  * @param[in] CAN_ClockSource : CAN clock source , can be one of @ref CAN_ClockSource_TypeDef.
  * @param[in] CAN_Prescaler : CAN Baud Rate Prescaler , can be a value from 0x01 to 0xFF.
  * @retval Indicates if initialization is succeed. it can be one of @ref CAN_InitStatus_TypeDef enumeration.
  */

CAN_InitStatus_TypeDef CAN_Init(CAN_MasterCtrl_TypeDef CAN_MasterCtrl,
                                CAN_Mode_TypeDef CAN_Mode,
                                CAN_SynJumpWidth_TypeDef CAN_SynJumpWidth,
                                CAN_BitSeg1_TypeDef CAN_BitSeg1,
                                CAN_BitSeg2_TypeDef CAN_BitSeg2,
                                CAN_ClockSource_TypeDef CAN_ClockSource,
                                u8 CAN_Prescaler)
{
    CAN_InitStatus_TypeDef InitStatus =  CAN_InitStatus_Failed;
    u16 timeout = CAN_ACKNOWLEDGE_TIMEOUT;
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();


    /* Check the parameters */
    assert_param(IS_CAN_MASTERCTRL_OK(CAN_MasterCtrl));
    assert_param(IS_CAN_MODE_OK(CAN_Mode));
    assert_param(IS_CAN_SYNJUMPWIDTH_OK(CAN_SynJumpWidth));
    assert_param(IS_CAN_BITSEG1_OK(CAN_BitSeg1));
    assert_param(IS_CAN_BITSEG2_OK(CAN_BitSeg2));
    assert_param(IS_CAN_CLOCKSOURCE_OK(CAN_ClockSource));
    assert_param(IS_CAN_PRESCALER_OK(CAN_Prescaler));

    /* Request initialisation */
    CAN->MCR = CAN_MCR_INRQ;
    /* Wait the acknowledge */
    while ((timeout != 0)&&((CAN->MSR & CAN_MSR_INAK) != CAN_MSR_INAK))
    {
        timeout--;
    }

    /* Check acknowledged */
    if ((CAN->MSR & CAN_MSR_INAK) != CAN_MSR_INAK)
    {

        InitStatus =  CAN_InitStatus_Failed;

    }
    else
    {
        /* Set the time triggered communication mode  &  Set the automatic bus-off management  &   Set the automatic wake-up mode
        &     Set the no automatic retransmission & Set the receive FIFO locked mode & Set the transmit FIFO priority */
        CAN->MCR |= (u8)CAN_MasterCtrl;

        /* Set the bit timing register */
        CAN->DGR |= (u8)CAN_Mode ;
        CAN->PSR = CAN_Page_Config;
        CAN->Page.Config.BTR1 = (u8)(CAN_SynJumpWidth | (CAN_Prescaler - 1));
        CAN->Page.Config.BTR2 = (u8)(CAN_BitSeg1 | CAN_BitSeg2 | CAN_ClockSource);

        /* Request leave initialisation */
        CAN->MCR &= (u8)(~CAN_MCR_INRQ);
        /* Wait the acknowledge */
        timeout = 0xFFFF;
        while ((timeout != 0)&&((CAN->MSR & CAN_MSR_INAK) == CAN_MSR_INAK))
        {
            timeout--;
        }
        /* Check acknowledged */
        if ((CAN->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
        {
            InitStatus = CAN_InitStatus_Failed;
        }
        else
        {
            InitStatus = CAN_InitStatus_Success;
        }
    }
    /*Restore Last Page*/
    CAN_SelectPage(can_page);

    /* Return the status of initialization */
    return (CAN_InitStatus_TypeDef)InitStatus;
}

/**
  * @brief Initializes the CAN peripheral Filter according to the specified parameters.
  * @param[in] CAN_FilterNumber : CAN Filter number , can be one of @ref CAN_FilterNumber_TypeDef
  * @param[in] CAN_FilterActivation : CAN Filter Activation state , can be one of @ref FunctionalState
  * @param[in] CAN_FilterMode : CAN Filter Mode , can be one of @ref CAN_FilterMode_TypeDef
  * @param[in] CAN_FilterScale : CAN Filter Scale , can be one of @ref CAN_FilterScale_TypeDef
  * @param[in] CAN_FilterID1 : CAN Filter ID 1 , can be a value from 0x00 to 0xFF
  * @param[in] CAN_FilterID2 : CAN Filter ID 2 , can be a value from 0x00 to 0xFF
  * @param[in] CAN_FilterID3 : CAN Filter ID 3 , can be a value from 0x00 to 0xFF
  * @param[in] CAN_FilterID4 : CAN Filter ID 4 , can be a value from 0x00 to 0xFF
  * @param[in] CAN_FilterIDMask1 : CAN Filter ID 1/ Mask 1  , can be a value from 0x00 to 0xFF depending of CAN_FilterMode parameter
  * @param[in] CAN_FilterIDMask2 : CAN Filter ID 2/ Mask 2 , can be a value from 0x00 to 0xFF depending of CAN_FilterMode parameter
  * @param[in] CAN_FilterIDMask3 : CAN Filter ID 3/ Mask 3 , can be a value from 0x00 to 0xFF depending of CAN_FilterMode parameter
  * @param[in] CAN_FilterIDMask4 : CAN Filter ID 4/ Mask 4 , can be a value from 0x00 to 0xFF depending of CAN_FilterMode parameter
  * @retval None
  */
void CAN_FilterInit(CAN_FilterNumber_TypeDef CAN_FilterNumber,
                    FunctionalState CAN_FilterActivation,
                    CAN_FilterMode_TypeDef CAN_FilterMode,
                    CAN_FilterScale_TypeDef CAN_FilterScale,
                    u8 CAN_FilterID1,
                    u8 CAN_FilterID2,
                    u8 CAN_FilterID3,
                    u8 CAN_FilterID4,
                    u8 CAN_FilterIDMask1,
                    u8 CAN_FilterIDMask2,
                    u8 CAN_FilterIDMask3,
                    u8 CAN_FilterIDMask4)
{
    u8 fact = 0;
    u8 fsc  = 0;
    u8 fmhl = 0;

    CAN_Page_TypeDef can_page_filter = CAN_Page_Filter01;
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();

    /* Check the parameters */
    assert_param(IS_CAN_FILTER_NUMBER_OK(CAN_FilterNumber));
    assert_param(IS_FUNCTIONALSTATE_OK(CAN_FilterActivation));
    assert_param(IS_CAN_FILTER_MODE_OK(CAN_FilterMode));
    assert_param(IS_CAN_FILTER_SCALE_OK(CAN_FilterScale));


    if (CAN_FilterNumber == CAN_FilterNumber_0)
    {
        fact = 0x01;
        fsc  = 0x00;
        fmhl = 0x03;

        can_page_filter = CAN_Page_Filter01;
    }
    else if (CAN_FilterNumber == CAN_FilterNumber_1)
    {
        fact = 0x10;
        fsc  = 0x04;
        fmhl = 0x0C;

        can_page_filter = CAN_Page_Filter01;
    }
    else if (CAN_FilterNumber == CAN_FilterNumber_2)
    {
        fact = 0x01;
        fsc  = 0x00;
        fmhl = 0x30;

        can_page_filter = CAN_Page_Filter23;
    }
    else if (CAN_FilterNumber == CAN_FilterNumber_3)
    {
        fact = 0x10;
        fsc  = 0x04;
        fmhl = 0xC0;

        can_page_filter = CAN_Page_Filter23;
    }
    else if (CAN_FilterNumber == CAN_FilterNumber_4)
    {
        fact = 0x01;
        fsc  = 0x00;
        fmhl = 0x03;

        can_page_filter = CAN_Page_Filter45;
    }
    else /*if (CAN_FilterNumber == CAN_FilterNumber_5)*/
    {
        fact = 0x10;
        fsc  = 0x04;
        fmhl = 0x0C;

        can_page_filter = CAN_Page_Filter45;
    }


    CAN_OperatingModeRequest(CAN_OperatingMode_Initialization);

    CAN->PSR = CAN_Page_Config;
    /*---------------------------------------------------------*/
    /*Configuration of Filter Scale                            */
    /*---------------------------------------------------------*/

    if (can_page_filter == CAN_Page_Filter01) /* FCR1 */
    {
        /* Filter Deactivation  & Reset the Filter Scale */
        CAN->Page.Config.FCR1 &= (u8)~( fact | ((CAN_FCR1_FSC00|CAN_FCR1_FSC01) << fsc ));
        /* Set the new Filter Scale */
        CAN->Page.Config.FCR1 |= (u8)(CAN_FilterScale << fsc);
    }
    else if (can_page_filter == CAN_Page_Filter23) /* FCR2*/
    {
        /* Filter Deactivation  & Reset the Filter Scale */
        CAN->Page.Config.FCR2 &= (u8)~( fact | ((CAN_FCR1_FSC00|CAN_FCR1_FSC01) << fsc ));

        /* Set the new Filter Scale */
        CAN->Page.Config.FCR2 |= (u8)(CAN_FilterScale << fsc);

    }
    else /*if(can_page_filter == CAN_Page_Filter45)*/ /* FCR3*/
    {
        /* Filter Deactivation  & Reset the Filter Scale */
        CAN->Page.Config.FCR3 &= (u8)~( fact | ((CAN_FCR1_FSC00|CAN_FCR1_FSC01) << fsc ));

        /* Set the new Filter Scale */
        CAN->Page.Config.FCR3 |= (u8)(CAN_FilterScale << fsc);
    }

    /*---------------------------------------------------------*/
    /*Configuration of Filter Mode                             */
    /*---------------------------------------------------------*/
    if (can_page_filter != CAN_Page_Filter45) /* FMR1*/
    {
        /* Filter Mode */
        if (CAN_FilterMode == CAN_FilterMode_IdMask)
        {
            /*Id/Mask mode for the filter*/
            CAN->Page.Config.FMR1 &= (u8)~(fmhl);
        }
        else if ( CAN_FilterMode == CAN_FilterMode_IdList)
        {
            /*Identifier list mode for the filter*/
            CAN->Page.Config.FMR1 |= (u8)(fmhl);
        }
        else if ( CAN_FilterMode == CAN_FilterMode_IdList_IdMask)
        {
            /*Identifier list mode is first  for the filter*/
            CAN->Page.Config.FMR1 |= (u8)(fmhl & CAN_IDLIST_IDMASK_MASK);
        }
        else /* ( CAN_FilterMode == CAN_FilterMode_IdMask_IdList)*/
        {
            /*Id Mask mode is first  for the filter*/
            CAN->Page.Config.FMR1 |= (u8)(fmhl & CAN_IDMASK_IDLIST_MASK);
        }


    }
    else /* FMR2 */
    {

        /* Filter Mode */
        if (CAN_FilterMode == CAN_FilterMode_IdMask)
        {
            /*Id/Mask mode for the filter*/
            CAN->Page.Config.FMR2 &= (u8)~(fmhl);
        }
        else if ( CAN_FilterMode == CAN_FilterMode_IdList)
        {
            /*Identifier list mode for the filter*/
            CAN->Page.Config.FMR2 |= (u8)(fmhl);
        }
        else if ( CAN_FilterMode == CAN_FilterMode_IdList_IdMask)
        {
            /*Identifier list mode is first  for the filter*/
            CAN->Page.Config.FMR2 |= (u8)(fmhl & CAN_IDLIST_IDMASK_MASK);
        }
        else /* ( CAN_FilterMode == CAN_FilterMode_IdMask_IdList)*/
        {
            /*Id Mask mode is first  for the filter*/
            CAN->Page.Config.FMR2 |= (u8)(fmhl & CAN_IDMASK_IDLIST_MASK);
        }
    }
    /*---------------------------------------------------------*/
    /*Configuration of Filter IDs                              */
    /*---------------------------------------------------------*/
    CAN->PSR = (u8)can_page_filter;
    if (fsc != 0)
    {
        /* Filter Scale */
        if (CAN_FilterScale == CAN_FilterScale_8Bit)
        {
            CAN->Page.Filter.FR09 = CAN_FilterID1;
            CAN->Page.Filter.FR10 = CAN_FilterIDMask1;
            CAN->Page.Filter.FR11 = CAN_FilterID2;
            CAN->Page.Filter.FR12 = CAN_FilterIDMask2;
            CAN->Page.Filter.FR13 = CAN_FilterID3;
            CAN->Page.Filter.FR14 = CAN_FilterIDMask3;
            CAN->Page.Filter.FR15 = CAN_FilterID4;
            CAN->Page.Filter.FR16 = CAN_FilterIDMask4;
        }
        else if (CAN_FilterScale == CAN_FilterScale_16_8Bit)
        {
            CAN->Page.Filter.FR09 = CAN_FilterID1;
            CAN->Page.Filter.FR10 = CAN_FilterID2;
            CAN->Page.Filter.FR11 = CAN_FilterIDMask1;
            CAN->Page.Filter.FR12 = CAN_FilterIDMask2;
            CAN->Page.Filter.FR13 = CAN_FilterID3;
            CAN->Page.Filter.FR14 = CAN_FilterIDMask3;
            CAN->Page.Filter.FR15 = CAN_FilterID4;
            CAN->Page.Filter.FR16 = CAN_FilterIDMask4;
        }
        else if (CAN_FilterScale == CAN_FilterScale_16Bit)
        {
            CAN->Page.Filter.FR09 = CAN_FilterID1;
            CAN->Page.Filter.FR10 = CAN_FilterID2;
            CAN->Page.Filter.FR11 = CAN_FilterIDMask1;
            CAN->Page.Filter.FR12 = CAN_FilterIDMask2;
            CAN->Page.Filter.FR13 = CAN_FilterID3;
            CAN->Page.Filter.FR14 = CAN_FilterID4;
            CAN->Page.Filter.FR15 = CAN_FilterIDMask3;
            CAN->Page.Filter.FR16 = CAN_FilterIDMask4;
        }
        else if (CAN_FilterScale == CAN_FilterScale_32Bit)
        {
            CAN->Page.Filter.FR09 = CAN_FilterID1;
            CAN->Page.Filter.FR10 = CAN_FilterID2;
            CAN->Page.Filter.FR11 = CAN_FilterID3;
            CAN->Page.Filter.FR12 = CAN_FilterID4;
            CAN->Page.Filter.FR13 = CAN_FilterIDMask1;
            CAN->Page.Filter.FR14 = CAN_FilterIDMask2;
            CAN->Page.Filter.FR15 = CAN_FilterIDMask3;
            CAN->Page.Filter.FR16 = CAN_FilterIDMask4;
        }
    }
    else
    {
        /* Filter Scale */
        if (CAN_FilterScale == CAN_FilterScale_8Bit)
        {
            CAN->Page.Filter.FR01 = CAN_FilterID1;
            CAN->Page.Filter.FR02 = CAN_FilterIDMask1;
            CAN->Page.Filter.FR03 = CAN_FilterID2;
            CAN->Page.Filter.FR04 = CAN_FilterIDMask2;
            CAN->Page.Filter.FR05 = CAN_FilterID3;
            CAN->Page.Filter.FR06 = CAN_FilterIDMask3;
            CAN->Page.Filter.FR07 = CAN_FilterID4;
            CAN->Page.Filter.FR08 = CAN_FilterIDMask4;
        }
        else if (CAN_FilterScale == CAN_FilterScale_16_8Bit)
        {
            CAN->Page.Filter.FR01 = CAN_FilterID1;
            CAN->Page.Filter.FR02 = CAN_FilterID2;
            CAN->Page.Filter.FR03 = CAN_FilterIDMask1;
            CAN->Page.Filter.FR04 = CAN_FilterIDMask2;
            CAN->Page.Filter.FR05 = CAN_FilterID3;
            CAN->Page.Filter.FR06 = CAN_FilterIDMask3;
            CAN->Page.Filter.FR07 = CAN_FilterID4;
            CAN->Page.Filter.FR08 = CAN_FilterIDMask4;
        }
        else if (CAN_FilterScale == CAN_FilterScale_16Bit)
        {
            CAN->Page.Filter.FR01 = CAN_FilterID1;
            CAN->Page.Filter.FR02 = CAN_FilterID2;
            CAN->Page.Filter.FR03 = CAN_FilterIDMask1;
            CAN->Page.Filter.FR04 = CAN_FilterIDMask2;
            CAN->Page.Filter.FR05 = CAN_FilterID3;
            CAN->Page.Filter.FR06 = CAN_FilterID4;
            CAN->Page.Filter.FR07 = CAN_FilterIDMask3;
            CAN->Page.Filter.FR08 = CAN_FilterIDMask4;
        }
        else if (CAN_FilterScale == CAN_FilterScale_32Bit)
        {
            CAN->Page.Filter.FR01 = CAN_FilterID1;
            CAN->Page.Filter.FR02 = CAN_FilterID2;
            CAN->Page.Filter.FR03 = CAN_FilterID3;
            CAN->Page.Filter.FR04 = CAN_FilterID4;
            CAN->Page.Filter.FR05 = CAN_FilterIDMask1;
            CAN->Page.Filter.FR06 = CAN_FilterIDMask2;
            CAN->Page.Filter.FR07 = CAN_FilterIDMask3;
            CAN->Page.Filter.FR08 = CAN_FilterIDMask4;
        }
    }


    /*---------------------------------------------------------*/
    /*Configuration of Filter Activation                       */
    /*---------------------------------------------------------*/
    /* Filter activation */
    CAN->PSR = CAN_Page_Config;
    if (CAN_FilterActivation != DISABLE)
    {
        if ((CAN_FilterNumber & 0x06) == 0x00) /* FCR1*/
        { 	CAN->Page.Config.FCR1 |= (u8)fact;
        }
        else if ((CAN_FilterNumber & 0x06) == 0x02) /*FCR2*/
        { CAN->Page.Config.FCR2 |= (u8)fact;
        }
        else /*if((CAN_FilterNumber & 0x06) == 0x04)*/ /*FCR3*/
        { CAN->Page.Config.FCR3 |= (u8)fact;
        }
    }
    CAN_OperatingModeRequest(CAN_OperatingMode_Normal);
    /*Restore Last Page*/
    CAN_SelectPage(can_page);
}


/**
  * @brief  Enables or disables the specified CAN interrupts.
  * @param[in]  CAN_IT: specifies the CAN interrupt sources to be enabled or disabled.
  * @param[in] NewState : CAN_IT new state , can be one of @ref FunctionalState
  * @retval None
  */
void CAN_ITConfig(CAN_IT_TypeDef CAN_IT, FunctionalState NewState)
{
    u8 tmperrorinterrupt = 0;
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();


    /* Check the parameters */
    assert_param(IS_CAN_IT_CONFIG_OK(CAN_IT));
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));

    tmperrorinterrupt = (u8)(((u16)CAN_IT) >>7);
    tmperrorinterrupt = (u8)(((u8)(tmperrorinterrupt & 0xF0)) | ((u8)((tmperrorinterrupt & 0x0F) >>1)));

    CAN->PSR = CAN_Page_Config;
    if (NewState != DISABLE)
    {
        /* Enable the selected CAN interrupt */
        CAN->IER |= (u8)(CAN_IT);
        CAN->Page.Config.EIER |= (u8)(tmperrorinterrupt);
    }
    else
    {
        /* Disable the selected CAN interrupt */
        CAN->IER &= (u8)(~CAN_IT);
        CAN->Page.Config.EIER &= (u8)~(tmperrorinterrupt);
    }
    /*Restore Last Page*/
    CAN_SelectPage(can_page);
}

/**
  * @brief  Enables or Disables the ST7 CAN Compatibility.
  * if the ST7 compatibilty is Enabled, CAN provides only 2 mailboxes.
  * if the ST7 compatibilty is Disabled, CAN provides 3 mailboxes.
  * @param[in] CAN_ST7Compatibility : CAN ST7 Compatibility , this parameter can be one of @ref CAN_ST7Compatibility_TypeDef enumeration.
  * @retval None
  */
void CAN_ST7CompatibilityCmd(CAN_ST7Compatibility_TypeDef CAN_ST7Compatibility)
{
    /* Check the parameters */
    assert_param(IS_CAN_ST7_COMPATIBILITY_OK(CAN_ST7Compatibility));
    /*Reset the old configuration of TXM2E */
    CAN->DGR &= (u8)(~CAN_DGR_TXM2E);

    /*Set the old configuration of TXM2E */
    CAN->DGR |= (u8)CAN_ST7Compatibility;
}
/**
  * @brief Enables or disabes the CAN Time TriggerOperation communication mode.
  * @param[in] NewState : Mode new state , can be one of @ref FunctionalState
  * @retval None
  */
void CAN_TTComModeCmd(FunctionalState NewState)
{
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();
    /* Check the parameters */
    assert_param(IS_FUNCTIONALSTATE_OK(NewState));
    if (NewState != DISABLE)
    {
        /*Enable the TTCM mode */
        CAN->MCR |= CAN_MCR_TTCM;
        /*Set TGT bits setting in Tx and FIFO pages*/
        CAN->PSR = CAN_Page_TxMailBox0;
        CAN->Page.TxMailbox.MDLCR |= CAN_MDLCR_TGT;
        CAN->PSR = CAN_Page_TxMailBox1;
        CAN->Page.TxMailbox.MDLCR |= CAN_MDLCR_TGT;
        CAN->PSR = CAN_Page_TxMailBox2;
        CAN->Page.TxMailbox.MDLCR |= CAN_MDLCR_TGT;
        CAN->PSR = CAN_Page_RxFifo;
        CAN->Page.RxFIFO.MDLCR |= CAN_MDLCR_TGT;
    }
    else
    {
        /*Disable the TTCM mode */
        CAN->MCR &= ((u8)~CAN_MCR_TTCM);
        /*Reset TGT bits setting in Tx and FIFO pages*/
        CAN->PSR = CAN_Page_TxMailBox0;
        CAN->Page.TxMailbox.MDLCR &= ((u8)~CAN_MDLCR_TGT);
        CAN->PSR = CAN_Page_TxMailBox1;
        CAN->Page.TxMailbox.MDLCR &= ((u8)~CAN_MDLCR_TGT);
        CAN->PSR = CAN_Page_TxMailBox2;
        CAN->Page.TxMailbox.MDLCR &= ((u8)~CAN_MDLCR_TGT);
        CAN->PSR = CAN_Page_RxFifo;
        CAN->Page.RxFIFO.MDLCR &= ((u8)~CAN_MDLCR_TGT);
    }
    /*Restore Last Page*/
    CAN_SelectPage(can_page);
}

/**
  * @brief Initiates the transmission of a message.
  * @param[in] CAN_Id the ID number of the message, its size depends on @ref CAN_IDE value.
	* @param[in] CAN_IDE the ID type of the message, this parameter can be one of the @ref CAN_Id_TypeDef enumeration.
	* @param[in] CAN_RTR the message type, this parameter can be one of the @ref CAN_RTR_TypeDef enumeration.
	* @param[in] CAN_DLC the number of data in the message type, this parameter can be a value between 0 to 7.
	* @param[in] CAN_Data pointer to a the @ref u8 table which contains data to sent.
  * @retval Transmit Status, this returned value can be one of the @ref CAN_TxStatus_TypeDef enumeration.
  */
CAN_TxStatus_TypeDef CAN_Transmit(u32 CAN_Id,
                                  CAN_Id_TypeDef CAN_IDE,
                                  CAN_RTR_TypeDef CAN_RTR,
                                  u8 CAN_DLC,
                                  u8 *CAN_Data)
{
    CAN_TxStatus_TypeDef CAN_TxStatus = CAN_TxStatus_NoMailBox;
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();
    /* Check the parameters */
    assert_param(IS_CAN_IDTYPE_OK(CAN_IDE));
    if (CAN_IDE != CAN_Id_Standard)
    {
        assert_param(IS_CAN_EXTID_OK(CAN_Id));
    }
    else
    {
        assert_param(IS_CAN_STDID_OK(CAN_Id));
    }
    assert_param(IS_CAN_RTR_OK(CAN_RTR));
    assert_param(IS_CAN_DLC_OK(CAN_DLC));
    /* Select one empty transmit mailbox */
    if ((CAN->TPR & CAN_TPR_TME0) == CAN_TPR_TME0)
    {
        CAN_TxStatus = CAN_TxStatus_MailBox0Ok;
    }
    else if ((CAN->TPR & CAN_TPR_TME1) == CAN_TPR_TME1)
    {
        CAN_TxStatus = CAN_TxStatus_MailBox1Ok;
    }
    else if ((CAN->TPR & CAN_TPR_TME2) == CAN_TPR_TME2)
    {
        CAN_TxStatus = CAN_TxStatus_MailBox2Ok;
    }
    else
    {
        CAN_TxStatus = CAN_TxStatus_NoMailBox;
    }
    if (CAN_TxStatus != CAN_TxStatus_NoMailBox)
    {
        CAN->PSR = (u8)CAN_TxStatus;
        /* Set up the Id */
        if (CAN_IDE != CAN_Id_Standard)
        {
            CAN_Id &= (u32)CAN_EXTID_SIZE;
            CAN->Page.TxMailbox.MIDR4 = (u8)(CAN_Id);
            CAN_Id = CAN_Id>>8;
            CAN->Page.TxMailbox.MIDR3 = (u8)(CAN_Id);
            CAN_Id = CAN_Id>>8;
            CAN->Page.TxMailbox.MIDR2 = (u8)(CAN_Id);
            CAN_Id = CAN_Id>>8;
            CAN->Page.TxMailbox.MIDR1 |= (u8)(CAN_Id |CAN_IDE | CAN_RTR);
        }
        else
        {
            CAN_Id &= (u16)CAN_STDID_SIZE;
            CAN->Page.TxMailbox.MIDR1 = (u8)((CAN_Id>>6) | (CAN_RTR)) ;
            CAN->Page.TxMailbox.MIDR2 = (u8)(CAN_Id<<2);
        }
        /* Set up the DLC                 */
        /*clear old DLC value*/
        CAN->Page.TxMailbox.MDLCR &= (u8)0xF0;
        /*set the new value of DLC*/
        CAN->Page.TxMailbox.MDLCR |= CAN_DLC;
        /* Set up the data field */
        CAN->Page.TxMailbox.MDAR1 = CAN_Data[0];
        CAN->Page.TxMailbox.MDAR2 = CAN_Data[1];
        CAN->Page.TxMailbox.MDAR3 = CAN_Data[2];
        CAN->Page.TxMailbox.MDAR4 = CAN_Data[3];
        CAN->Page.TxMailbox.MDAR5 = CAN_Data[4];
        CAN->Page.TxMailbox.MDAR6 = CAN_Data[5];
        CAN->Page.TxMailbox.MDAR7 = CAN_Data[6];
        CAN->Page.TxMailbox.MDAR8 = CAN_Data[7];
        /* Request transmission */
        CAN->Page.TxMailbox.MCSR |= CAN_MCSR_TXRQ;
    }
    /*Restore Last Page*/
    CAN_SelectPage(can_page);
    return (CAN_TxStatus_TypeDef)CAN_TxStatus;
}

/**
  * @brief Checks the transmission of a message.
  * @param[in] TransmitMailbox: the number of the mailbox that is used for transmission, can be on of  @ref CAN_TransmitMailBox_TypeDef.
  * @retval CAN_TxStatus_Ok if the CAN driver transmits the message, CAN_TxStatus_Failed in an other case.
  */
CAN_TxStatus_TypeDef CAN_TransmitStatus(CAN_TransmitMailBox_TypeDef CAN_TransmitMailbox)
{
    /* RQCP, TXOK and TME bits */
    CAN_TxStatus_TypeDef tstate = CAN_TxStatus_Failed;
    u8 tmpstate=0;

    /* Check the parameters */
    assert_param(IS_CAN_TRANSMITMAILBOX_OK(CAN_TransmitMailbox));

    switch (CAN_TransmitMailbox)
    {
    case (CAN_TransmitMailBox_0): tmpstate = (u8)((CAN->TSR & (CAN_TSR_RQCP0|CAN_TSR_TXOK0)));
        tmpstate |= (u8)((CAN->TPR & CAN_TPR_TME0));
        break;
    case (CAN_TransmitMailBox_1): tmpstate = (u8)((CAN->TSR & (CAN_TSR_RQCP1|CAN_TSR_TXOK1))>>1);
        tmpstate |= (u8)((CAN->TPR & CAN_TPR_TME1) >> 1);
        break;
    case (CAN_TransmitMailBox_2): tmpstate = (u8)((CAN->TSR & (CAN_TSR_RQCP2|CAN_TSR_TXOK2))>>2);
        tmpstate |= (u8)((CAN->TPR & CAN_TPR_TME2) >> 2);
        break;
    default:
        tstate = CAN_TxStatus_Failed;
        break;
    }

    switch (tmpstate)
    {
        /*transmit pending  */
    case (0x00): tstate = CAN_TxStatus_Pending;
        break;
        /* transmit failed  */
    case (0x05): tstate = CAN_TxStatus_Failed;
        break;
        /* transmit succedeed */
    case (0x15): tstate = CAN_TxStatus_Ok;
        break;
        /* transmit mailbox is empty : no activity on this TX mail box */
    case (0x04): tstate = CAN_TxStatus_MailBoxEmpty;
        break;
    default:
        tstate = CAN_TxStatus_Failed;
        break;
    }

    return (CAN_TxStatus_TypeDef)tstate;
}

/**
  * @brief Cancels a transmit request.
  * @param[in] TransmitMailbox : the Transmission mailbox, can be one of CAN_TransmitMailBox_TypeDef
  * @retval None
  */
void CAN_CancelTransmit(CAN_TransmitMailBox_TypeDef CAN_TransmitMailbox)
{
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();
    /* Check the parameters */
    assert_param(IS_CAN_TRANSMITMAILBOX_OK(CAN_TransmitMailbox));
    /*switch to the specific page */
    CAN->PSR = (u8)CAN_TransmitMailbox;
    /* abort transmission */
    CAN->Page.TxMailbox.MCSR |= CAN_MCSR_ABRQ;
    /*Restore Last Page*/
    CAN_SelectPage(can_page);
}


/**
  * @brief  Releases the CAN FIFO.
  * @par Parameters:
  * None
  * @retval None
  */
void CAN_FIFORelease(void)
{
    /* Release FIFO*/
    CAN->RFR = CAN_RFR_RFOM; /*rc-w1*/
}


/**
  * @brief Returns the number of pending messages.
  * @retval Number of pending messages.
  */
CAN_NbrPendingMessage_TypeDef CAN_MessagePending(void)
{
    CAN_NbrPendingMessage_TypeDef msgpending = CAN_NbrPendingMessage_0;
    msgpending = (u8)(CAN->RFR & CAN_RFR_FMP01);
    return (CAN_NbrPendingMessage_TypeDef)msgpending;
}

/**
  * @brief  Receives a message which contains CAN Id, IDE, RTR
  * DLC, datas and FMI number.
  * In order to get these data, use CAN_GetReceivedId(), CAN_GetReceivedIDE(), CAN_GetReceivedRTR(),
  * CAN_GetReceivedDLC(), CAN_GetReceivedFMI() and CAN_GetReceivedData() functions.
  * @par Parameters:
  * None
  * @retval None
  */
void CAN_Receive(void)
{
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();

    /* select Fifo page*/
    CAN->PSR = CAN_Page_RxFifo;

    /* Get the Id */
    _IDE = (u8)(CAN->Page.RxFIFO.MIDR1 & CAN_Id_Extended);
    if (_IDE != CAN_Id_Standard)
    {
        _Id = (u32)CAN_EXTID_SIZE & ((CAN->Page.RxFIFO.MIDR4) | ((u32)((u32)CAN->Page.RxFIFO.MIDR3) << 8) | ((u32)((u32)CAN->Page.RxFIFO.MIDR2) << 16) | ((u32)((u32)CAN->Page.RxFIFO.MIDR1 & 0x1F) << 24) );
    }
    else
    {
        _Id = (u16)CAN_STDID_SIZE & ((u16)((u16)(CAN->Page.RxFIFO.MIDR1 & 0x1F) << 6) | ((CAN->Page.RxFIFO.MIDR2 >> 2)&0x3F) );
    }

    _RTR = (u8)(0x20 & CAN->Page.RxFIFO.MIDR1);

    /* Get the DLC */
    _DLC = (u8)(CAN->Page.RxFIFO.MDLCR & 0x0F);

    /* Get the FMI */
    _FMI = CAN->Page.RxFIFO.MFMI;

    /* Get the data field */
    _Data[0] = CAN->Page.RxFIFO.MDAR1;
    _Data[1] = CAN->Page.RxFIFO.MDAR2;
    _Data[2] = CAN->Page.RxFIFO.MDAR3;
    _Data[3] = CAN->Page.RxFIFO.MDAR4;
    _Data[4] = CAN->Page.RxFIFO.MDAR5;
    _Data[5] = CAN->Page.RxFIFO.MDAR6;
    _Data[6] = CAN->Page.RxFIFO.MDAR7;
    _Data[7] = CAN->Page.RxFIFO.MDAR8;

    /* Release the FIFO */
    CAN_FIFORelease();
    /*Restore Last Page*/
    CAN_SelectPage(can_page);
}


/**
  * @brief Gets the  CAN Id of the received message.
  * @par Parameters:
  * None
  * @retval the received CAN message Id.
  * @par Required preconditions:
  * This function is used to get data loaded by CAN_Receive function.
  * Before using this function, CAN_Receive function must be called.
  */
u32 CAN_GetReceivedId(void)
{
    return (_Id);
}

/**
  * @brief Gets the  CAN IDE of the received message.
  * @par Parameters:
  * None
  * @retval the received CAN message IDE.
  * @par Required preconditions:
  * This function is used to get data loaded by CAN_Receive function.
  * Before using this function, CAN_Receive function must be called.
  */
CAN_Id_TypeDef CAN_GetReceivedIDE(void)
{
    return (_IDE);
}

/**
  * @brief Gets the  CAN RTR of the received message.
  * @par Parameters:
  * None
  * @retval the received CAN message RTR.
  * @par Required preconditions:
  * This function is used to get data loaded by CAN_Receive function.
  * Before using this function, CAN_Receive function must be called.
  */
CAN_RTR_TypeDef CAN_GetReceivedRTR(void)
{
    return (_RTR);
}

/**
  * @brief Gets the  CAN DLC of the received message.
  * @par Parameters:
  * None
  * @retval the received CAN message DLC.
  * @par Required preconditions:
  * This function is used to get data loaded by CAN_Receive function.
  * Before using this function, CAN_Receive function must be called.
  */
u8 CAN_GetReceivedDLC(void)
{
    return (_DLC);
}

/**
  * @brief Gets the  CAN Data of the received message.
  * @param[in] CAN_DataIndexe : number of the received Data, it can
  *  be an integer between 0 to 7.
  * @retval the received CAN message ith Data.
  * @par Required preconditions:
  * This function is used to get data loaded by CAN_Receive function.
  * Before using this function, CAN_Receive function must be called.
  */
u8 CAN_GetReceivedData(u8 CAN_DataIndex)
{
    assert_param(IS_CAN_DLC_OK(CAN_DataIndex));
    return (_Data[CAN_DataIndex]);
}

/**
  * @brief Gets the  CAN FMI of the received message.
  * @par Parameters:
  * None
  * @retval the received CAN message FMI.
  * @par Required preconditions:
  * This function is used to get data loaded by CAN_Receive function.
  * Before using this function, CAN_Receive function must be called.
  */
u8 CAN_GetReceivedFMI(void)
{
    return (_FMI);
}
/**
  * @brief Returns the Received time stamp.
  * @par Parameters:
  * None
  * @retval u16 the received  time stamp.
  */
u16 CAN_GetMessageTimeStamp(void)
{
    u16 time;
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();

    /*switch to the specific page */
    CAN->PSR = CAN_Page_RxFifo;
    /* Get the Received Time stamp */
    time = CAN->Page.RxFIFO.MTSRL;
    time |= (u16)(((u16)CAN->Page.RxFIFO.MTSRH)<<8);

    /*Restore Last Page*/
    CAN_SelectPage(can_page);

    return (u16)(time);
}


/**
  * @brief Enters the Sleep low power mode.
  * @par Parameters:
  * None
  * @retval  CAN_Sleep_Ok if sleep entered, CAN_Sleep_Failed in an other case.
  */
CAN_Sleep_TypeDef CAN_Sleep(void)
{

    CAN_Sleep_TypeDef sleepstatus = CAN_Sleep_Failed;

    /* Request Sleep mode */
    CAN->MCR = (u8)(((CAN->MCR) & (u8)(~CAN_MCR_INRQ)) | CAN_MCR_SLEEP);

    /* Sleep mode status */
    if ((CAN->MSR & (CAN_MSR_SLAK|CAN_MSR_INAK)) == CAN_MSR_SLAK)
    {
        /* Sleep mode not entered */
        sleepstatus =  CAN_Sleep_Ok;
    }

    /* At this step, sleep mode status */
    return (CAN_Sleep_TypeDef) sleepstatus;
}




/**
  * @brief Wakes the CAN up.
  * @par Parameters:
  * None
  * @retval CAN_WakeUp_Ok if sleep mode left, CAN_WakeUp_Failed in an other  case.
  */
CAN_WakeUp_TypeDef CAN_WakeUp(void)
{
    CAN_WakeUp_TypeDef wakeupstatus = CAN_WakeUp_Failed;

    /* Wake up request */
    CAN->MCR &= (u8)(~CAN_MCR_SLEEP);

    /* Sleep mode status */
    if ((CAN->MSR & CAN_MSR_SLAK) != CAN_MSR_SLAK)
    {
        /* Sleep mode exited */
        wakeupstatus = CAN_WakeUp_Ok;
    }

    /* At this step, sleep mode status */
    return (CAN_WakeUp_TypeDef)wakeupstatus;
}


/**
  * @brief  Select the CAN clock input.
  * @param[in] CAN_ClockSource : CAN clock input, can be one of @ref CAN_ClockSource_TypeDef.
  * @retval None
  */
void CAN_SelectClock(CAN_ClockSource_TypeDef CAN_ClockSource)
{
    u16 timeout =0;
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();
    /* Check the parameters */
    assert_param(IS_CAN_CLOCKSOURCE_OK(CAN_ClockSource));
    /* Request initialisation */
    CAN->MCR |= (CAN_MCR_INRQ & ((u8)(~CAN_MCR_SLEEP)));

    CAN->PSR = CAN_Page_Config;
    if (CAN_ClockSource != CAN_ClockSource_Internal)
    {
        CAN->Page.Config.BTR2 |= (u8)CAN_BTR2_CLKS;
    }
    else
    {
        CAN->Page.Config.BTR2 &= (u8)(~CAN_BTR2_CLKS);
    }
    /* Request leave initialisation */
    CAN->MCR &= (u8)(~CAN_MCR_INRQ);
    /* Wait the acknowledge */
    while ((timeout != 0)&&((CAN->MSR & CAN_MSR_INAK) == CAN_MSR_INAK))
    {
        timeout--;
    }

    /*Restore Last Page*/
    CAN_SelectPage(can_page);
}

/**
  * @brief Select the CAN Operation mode.
  * @param[in] CAN_OperatingMode CAN Operating Mode ,
	* this parameter can be one of @ref CAN_OperatingMode_TypeDef enumeration.
  * @retval None
  */
CAN_ModeStatus_TypeDef CAN_OperatingModeRequest(CAN_OperatingMode_TypeDef CAN_OperatingMode)
{

    u16 timeout = CAN_ACKNOWLEDGE_TIMEOUT;

    assert_param(IS_CAN_OPERATINGMODE_OK(CAN_OperatingMode));

    if (CAN_OperatingMode == CAN_OperatingMode_Initialization)
    {
        /* Request initialisation */
        CAN->MCR = (u8)((CAN->MCR & (u8)(~CAN_MCR_SLEEP)) | CAN_MCR_INRQ);

        /* Wait the acknowledge */
        while (((CAN->MSR & CAN_MODE_MASK) != CAN_MSR_INAK) && (timeout != 0))
        {
            timeout--;
        }
        if ((CAN->MSR & CAN_MODE_MASK) != CAN_MSR_INAK)
        {
            return (CAN_ModeStatus_TypeDef)(CAN_ModeStatus_Failed);
        }
        else
        {
            return (CAN_ModeStatus_TypeDef)(CAN_ModeStatus_Success);
        }

    }
    else  if (CAN_OperatingMode == CAN_OperatingMode_Normal)
    {
        /* Request leave initialisation and sleep mode  and enter Normal mode */
        CAN->MCR &= (u8)(~(CAN_MCR_SLEEP|CAN_MCR_INRQ));

        /* Wait the acknowledge */
        while (((CAN->MSR & CAN_MODE_MASK) != 0) && (timeout!=0))
        {
            timeout--;
        }
        if ((CAN->MSR & CAN_MODE_MASK) != 0)
        {
            return (CAN_ModeStatus_TypeDef)(CAN_ModeStatus_Failed);
        }
        else
        {
            return (CAN_ModeStatus_TypeDef)(CAN_ModeStatus_Success);
        }
    }
    else  if (CAN_OperatingMode == CAN_OperatingMode_Sleep)
    {
        /* Request Sleep mode */
        CAN->MCR = (u8)((CAN->MCR & (u8)(~CAN_MCR_INRQ)) | CAN_MCR_SLEEP);

        /* Wait the acknowledge */
        while (((CAN->MSR & CAN_MODE_MASK) != CAN_MSR_SLAK) && (timeout!=0))
        {
            timeout--;
        }
        if ((CAN->MSR & CAN_MODE_MASK) != CAN_MSR_SLAK)
        {
            return (CAN_ModeStatus_TypeDef)(CAN_ModeStatus_Failed);
        }
        else
        {
            return (CAN_ModeStatus_TypeDef)(CAN_ModeStatus_Success);
        }
    }
    else
    {
        return (CAN_ModeStatus_TypeDef)(CAN_ModeStatus_Failed);
    }

}

/**
  * @brief Gets the  Last Error Code.
  * @par Parameters:
  * None
  * @retval Error Code.
  */
CAN_ErrorCode_TypeDef CAN_GetLastErrorCode(void)
{
    CAN_ErrorCode_TypeDef errcode = CAN_ErrorCode_NoErr;
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();

    CAN->PSR = CAN_Page_Config;
    errcode = (u8)((CAN->Page.Config.ESR) & (CAN_ESR_LEC));

    /*Restore Last Page*/
    CAN_SelectPage(can_page);

    return (CAN_ErrorCode_TypeDef)(errcode);
}
/**
  * @brief Clears the CAN's pending flags.
  * @param[in] CAN_FLAG : Flag to be cleared, can be one of the folowing parameters:
  *						CAN_FLAG_RQCP0            Request MailBox0  Flag
  *						CAN_FLAG_RQCP1            Request MailBox1  Flag
  *						CAN_FLAG_RQCP2            Request MailBox2  Flag
  *						CAN_FLAG_FF               FIFO Full  Flag
  *						CAN_FLAG_FOV              FIFO Overrun  Flag
  *						CAN_FLAG_WKU              wake up   Flag
  *						CAN_FLAG_LEC               Last error code Flag
  * @retval None
  */
void CAN_ClearFlag(CAN_Flag_TypeDef CAN_Flag)
{
    CAN_Page_TypeDef can_page = 0;
    /* Check the parameters */
    assert_param(IS_CAN_FLAG_CLEAR_OK(CAN_Flag));
    if ((CAN_Flag & 0x0700)!=(u16)RESET)
    {
        if ((CAN_Flag & 0x020B)!=(u16)RESET)
        {
            /*Receive Flags*/
            CAN->RFR = (u8)(CAN_Flag);
        }
        else if ((CAN_Flag & 0x0403)!=(u16)RESET)
        {
            /*Transmit Flags*/
            CAN->TSR = (u8)(CAN_Flag);
        }
        else /*if((CAN_Flag & 0x0108)!=(u16)RESET)*/
        {
            /*wake up Flags*/
            CAN->MSR = (u8)(CAN_Flag);
        }
    }
    else
    {
        /*Error Flags*/
        can_page = CAN_GetSelectedPage();

        /* Clear the selected CAN flags */
        CAN->PSR = CAN_Page_Config;
        CAN->Page.Config.ESR = (u8)RESET;

        /*Restore Last Page*/
        CAN_SelectPage(can_page);
    }
}

/**
  * @brief Checks whether the specified CAN flag is set or not.
  * @param[in] CAN_FLAG: specifies the flag to check, can be one of @ref CAN_Flag_TypeDef enumeration.
  * @retval The new state of CAN_FLAG which can be one of @ref FlagStatus.
  */
FlagStatus CAN_GetFlagStatus(CAN_Flag_TypeDef CAN_Flag)
{
    FlagStatus bitstatus = RESET;
    CAN_Page_TypeDef can_page = 0;

    /* Check the parameters */
    assert_param(IS_CAN_FLAG_STATUS_OK(CAN_Flag));

    if ((CAN_Flag & 0x0700)!=(u16)RESET)
    {
        if ((CAN_Flag & 0x020B)!=(u16)RESET)
        {
            /*Receive Flags*/
            if ((CAN->RFR & CAN_Flag )!= (u16)RESET)
            {
                /* CAN_FLAG is set */
                bitstatus = SET;
            }
            else
            {
                /* CAN_FLAG is reset */
                bitstatus = RESET;
            }

        }
        else if ((CAN_Flag & 0x0403)!=(u16)RESET)
        {
            /*Transmit Flags*/
            if ((CAN->TSR & CAN_Flag )!= (u16)RESET)
            {
                /* CAN_FLAG is set */
                bitstatus = SET;
            }
            else
            {
                /* CAN_FLAG is reset */
                bitstatus = RESET;
            }
        }
        else /*if((CAN_Flag & 0x0108)!=(u16)RESET)*/
        {
            /*wake up Flags*/
            if ((CAN->MSR & CAN_Flag )!= (u16)RESET)
            {
                /* CAN_FLAG is set */
                bitstatus = SET;
            }
            else
            {
                /* CAN_FLAG is reset */
                bitstatus = RESET;
            }
        }
    }
    else
    {
        /*Error Flags*/
        can_page = CAN_GetSelectedPage();

        CAN->PSR = CAN_Page_Config;
        if ((CAN->Page.Config.ESR & CAN_Flag) != (u32)RESET)
        {
            /* CAN_FLAG is set */
            bitstatus = SET;
        }
        else
        {
            /* CAN_FLAG is reset */
            bitstatus = RESET;
        }
        /*Restore Last Page*/
        CAN_SelectPage(can_page);
    }


    /* Return the CAN_FLAG status */
    return  (FlagStatus)bitstatus;
}

/**
  * @brief Checks whether the specified CAN interrupt has occurred or not.
  * @param[in] CAN_IT: specifies the CAN interrupt source to check, can be one of @ref CAN_IT_TypeDef.
  * @retval The new state of CAN_IT, which can be one of @ref ITStatus.
  */
ITStatus CAN_GetITStatus(CAN_IT_TypeDef CAN_IT)
{
    ITStatus pendingbitstatus = RESET;
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();

    /* Check the parameters */
    assert_param(IS_CAN_IT_STATUS_OK(CAN_IT));


    switch (CAN_IT)
    {
    case CAN_IT_TME:
        if ((CAN->IER & CAN_IER_TMEIE) !=RESET)
        {
            pendingbitstatus = CheckITStatus(CAN->TSR, CAN_TSR_RQCP012);
        }
        else
        {
            pendingbitstatus = RESET;
        }
        break;

    case CAN_IT_FMP:
        if ((CAN->IER & CAN_IER_FMPIE) !=RESET)
        {
            pendingbitstatus = CheckITStatus(CAN->RFR, CAN_RFR_FMP01);
        }
        else
        {
            pendingbitstatus = RESET;
        }
        break;
    case CAN_IT_FF:
        if ((CAN->IER & CAN_IER_FFIE) !=RESET)
        {
            pendingbitstatus = CheckITStatus(CAN->RFR, CAN_RFR_FULL);
        }
        else
        {
            pendingbitstatus = RESET;
        }
        break;
    case CAN_IT_FOV:
        if ((CAN->IER & CAN_IER_FOVIE) !=RESET)
        {
            pendingbitstatus = CheckITStatus(CAN->RFR, CAN_RFR_FOVR);
        }
        else
        {
            pendingbitstatus = RESET;
        }
        break;
    case CAN_IT_WKU:
        if ((CAN->IER & CAN_IER_WKUIE) !=RESET)
        {
            pendingbitstatus = CheckITStatus(CAN->MSR, CAN_MSR_WKUI);
        }
        else
        {
            pendingbitstatus = RESET;
        }
        break;

    case CAN_IT_ERR:
        CAN->PSR = CAN_Page_Config;
        if ((CAN->Page.Config.EIER & CAN_EIER_ERRIE) !=RESET)
        {
            pendingbitstatus = CheckITStatus(CAN->Page.Config.ESR, CAN_ESR_EWGF|CAN_ESR_EPVF|CAN_ESR_BOFF|CAN_ESR_LEC);
        }
        else
        {
            pendingbitstatus = RESET;
        }
        break;

    case CAN_IT_EWG:
        CAN->PSR = CAN_Page_Config;
        if ((CAN->Page.Config.EIER & CAN_EIER_EWGIE) !=RESET)
        {
            pendingbitstatus = CheckITStatus(CAN->Page.Config.ESR, CAN_ESR_EWGF);
        }
        else
        {
            pendingbitstatus = RESET;
        }
        break;

    case CAN_IT_EPV:
        CAN->PSR = CAN_Page_Config;
        if ((CAN->Page.Config.EIER & CAN_EIER_EPVIE) !=RESET)
        {
            pendingbitstatus = CheckITStatus(CAN->Page.Config.ESR, CAN_ESR_EPVF);
        }
        else
        {
            pendingbitstatus = RESET;
        }
        break;
    case CAN_IT_BOF:
        CAN->PSR = CAN_Page_Config;
        if ((CAN->Page.Config.EIER & CAN_EIER_BOFIE) !=RESET)
        {
            pendingbitstatus = CheckITStatus(CAN->Page.Config.ESR, CAN_ESR_BOFF);
        }
        else
        {
            pendingbitstatus = RESET;
        }
        break;
    case CAN_IT_LEC:
        CAN->PSR = CAN_Page_Config;
        if ((CAN->Page.Config.EIER & CAN_EIER_LECIE) !=RESET)
        {
            pendingbitstatus = CheckITStatus(CAN->Page.Config.ESR, CAN_ESR_LEC);
        }
        else
        {
            pendingbitstatus = RESET;
        }
        break;
    default :
        pendingbitstatus = RESET;
        break;
    }
    /*Restore Last Page*/
    CAN_SelectPage(can_page);
    /* Return the CAN_IT status */
    return  (ITStatus)pendingbitstatus;
}

/**
  * @brief  Clears the CAN’s interrupt pending bits.
  * @param[in] CAN_IT: specifies the interrupt pending bit to clear,
  *	can be one of the following parameters:
  *                            CAN_IT_TME = Transmit mailbox empty interrupt
  *												     CAN_IT_FF =FIFO  full    interrupt
  *                            CAN_IT_FOV =FIFO  overrun  interrupt
  *														 CAN_IT_WKU =Wake-up interrupt
  *														 CAN_IT_ERR =Genaral Error interrupt
  *														 CAN_IT_EWG =Error warning interrupt
  *														 CAN_IT_EPV  =Error passive  interrupt
  *														 CAN_IT_BOF = Bus-off   interrupt
  *														 CAN_IT_LEC  =Last error code interrupt
  * @retval None
  */
void CAN_ClearITPendingBit(CAN_IT_TypeDef CAN_IT)
{
    CAN_Page_TypeDef can_page = CAN_GetSelectedPage();
    /* Check the parameters */
    assert_param(IS_CAN_IT_PENDING_BIT_OK(CAN_IT));

    switch (CAN_IT)
    {
    case CAN_IT_TME:
        CAN->TSR = CAN_TSR_RQCP012;/* rc_w1*/
        break;

    case CAN_IT_FF:
        CAN->RFR = CAN_RFR_FULL; /* rc_w1*/
        break;

    case CAN_IT_FOV:
        CAN->RFR = CAN_RFR_FOVR; /* rc_w1*/
        break;

    case CAN_IT_WKU:
        CAN->MSR = CAN_MSR_WKUI;  /* rc_w1*/
        break;

    case CAN_IT_ERR:
        CAN->PSR = CAN_Page_Config;
        CAN->Page.Config.ESR = (u8)CAN_ESR_RESET_VALUE;
        CAN->MSR = CAN_MSR_ERRI;
        break;

    case CAN_IT_EWG:
        CAN->MSR = CAN_MSR_ERRI;
        break;

    case CAN_IT_EPV:
        CAN->MSR = CAN_MSR_ERRI;
        break;

    case CAN_IT_BOF:
        CAN->MSR = CAN_MSR_ERRI;
        break;

    case CAN_IT_LEC:
        CAN->PSR = CAN_Page_Config;
        CAN->Page.Config.ESR = (u8)CAN_ESR_RESET_VALUE;
        break;



    default :
        break;
    }
    /*Restore Last Page*/
    CAN_SelectPage(can_page);
}

/**
  * @brief Gets the selected registers page.
  * @par Parameters:
  * None
  * @retval the selected page which can be one of the @ref CAN_Page_TypeDef.
  */
CAN_Page_TypeDef CAN_GetSelectedPage(void)
{
    return (CAN_Page_TypeDef)(CAN->PSR);
}

/**
  * @brief Sets the registers page to be selected.
  * @param[in] the selected page which can be one of the @ref CAN_Page_TypeDef.
  * @retval None
  */
void CAN_SelectPage(CAN_Page_TypeDef CAN_Page)
{
    CAN->PSR = (u8)CAN_Page;
}

/**
  * @brief Checks whether the CAN interrupt has occurred or not.
  * @param[in] CAN_Reg: specifies the CAN interrupt register to check.
  * @param[in] It_Bit: specifies the interrupt source bit to check.
  * @retval The new state of the CAN Interrupt, which can be one of ITStatus.
  */
static ITStatus CheckITStatus(u8 CAN_Reg, u8 It_Bit)
{
    ITStatus pendingbitstatus = RESET;
    if ((CAN_Reg & It_Bit) != (u8)RESET)
    {
        /* CAN_IT is set */
        pendingbitstatus = SET;
    }
    else
    {
        /* CAN_IT is reset */
        pendingbitstatus = RESET;
    }
    return (ITStatus)pendingbitstatus;
}
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
