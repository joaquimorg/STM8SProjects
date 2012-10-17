/**
  ******************************************************************************
  * @file mono_lcd.c
  * @brief This file contains basic functions to control the mono LCD device.
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
#include "mono_lcd.h"

/* Private define ------------------------------------------------------------*/
#define STATUS_TYPE 0xFC

/* This table contains the "S" of ST logo */
__CONST u8 S_CGRAM[] =
  {
    /* 0~7 */
    0b00000011, 0b11111111,
    0b00000010, 0b00000000,
    0b00000100, 0b00000000,
    0b00000100, 0b00000000,
    0b00001100, 0b01111111,
    0b00001100, 0b01111111,
    0b00011100, 0b00111111,
    0b00011110, 0b00011111,
    /* 8~15 */
    0b00111111, 0b00001111,
    0b00111111, 0b10000111,
    0b01111111, 0b11000011,
    0b01111111, 0b11100011,
    0b00000000, 0b00000011,
    0b00000000, 0b00000011,
    0b00000000, 0b00000111,
    0b11111111, 0b11111110,
  };

/* This table contains the "T" of ST logo */
__CONST u8 T_CGRAM[] =
  {
    /* 0~7 */
    0b11111111, 0b11111111,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b00000000, 0b00000000,
    0b11111000, 0b11111000,
    0b11110000, 0b11111000,
    0b11110000, 0b11110000,
    0b11110000, 0b11110000,
    /* 8~15 */
    0b11100001, 0b11100000,
    0b11100011, 0b11100000,
    0b11000011, 0b11000000,
    0b11000111, 0b11000000,
    0b10000111, 0b11000000,
    0b10001111, 0b10000000,
    0b00001111, 0b10000000,
    0b00011111, 0b00000000
  };

/** @addtogroup LCD_Private_Functions
  * @{
  */

void LCD_Delay(u16 nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}

/**
  * @brief read status from LCD
  * @par Parameters:
  * None
  * @retval None
  * @par Required preconditions:
  * None
  * @par Functions called:
  * - LCD_SendByte()
	* - LCD_Delay()
  */
void LCD_ReadStatus(void)
{
  LCD_SendByte(STATUS_TYPE, 0x00);
  LCD_Delay(200);
  return;
}

/**
  * @brief Send a byte to LCD through the SPI peripheral
  * @param[in] DataToSend Data to be sent
  * @retval u8 Data read
  */
static void LCD_SPISendByte(u8 DataToSend)
{

  /* Send byte through the SPI peripheral */
  SPI->DR = DataToSend;

  while ((SPI->SR & SPI_SR_TXE) == 0)
  {
    /* Wait while the byte is transmitted */
  }
}

/**
  * @}
  */

/** @addtogroup LCD_Public_Functions
  * @{
  */

/**
  * @brief Enable or Disable the LCD through CS pin
  * @param[in] NewState CS pin state
  * @retval None
  * @par Required preconditions:
  * None
  * @par Functions called:
  * - GPIO_WriteHigh()
  * - GPIO_WriteLow()
  * @par Example:
  * @code
  * LCD_ChipSelect(ENABLE);
  * @endcode
  */
void LCD_ChipSelect(FunctionalState NewState)
{
  if (NewState == DISABLE)
  {
    GPIO_WriteLow(LCD_CS_PORT, LCD_CS_PIN); /* CS pin low: LCD disabled */
  }
  else
  {
    GPIO_WriteHigh(LCD_CS_PORT, LCD_CS_PIN); /* CS pin high: LCD enabled */
  }
}

/**
  * @brief Enable or Disable the LCD backlight
  * @param[in] NewState Backlight state
  * @retval None
  * @par Required preconditions:
  * None
  * @par Functions called:
  * - GPIO_WriteHigh()
  * - GPIO_WriteLow()
  * @par Example:
  * @code
  * LCD_Backlight(ENABLE);
  * @endcode
  */
void LCD_Backlight(FunctionalState NewState)
{
  if (NewState == DISABLE)
  {
    GPIO_WriteLow(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
  }
  else
  {
    GPIO_WriteHigh(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
  }
}

/**
  * @brief Send a byte to LCD
  * @param[in] DataType Type of Data to be sent
  * @param[in] DataToSend Data to be sent
  * @retval None
  * @par Required preconditions:
  * None
  * @par Functions called:
  * - LCD_ChipSelect
  * - LCD_SPISendByte
  * @par Example:
  * @code
  * LCD_SendByte(COMMAND_TYPE, DISPLAY_ON);
  * @endcode
  */
void LCD_SendByte(u8 DataType, u8 DataToSend)
{

  LCD_ChipSelect(ENABLE); /* Enable access to LCD */

  LCD_SPISendByte(DataType); /* Send Synchro/Mode byte */
  LCD_SPISendByte((u8)(DataToSend & (u8)0xF0)); /* Send byte high nibble */
  LCD_SPISendByte((u8)((u8)(DataToSend << 4) & (u8)0xF0)); /* Send byte low nibble */
  LCD_Delay(80);
  LCD_ChipSelect(DISABLE); /* Disable access to LCD */

}
/**
  * @brief Send a buffer to LCD
  * @param[in] pTxBuffer Pointer to the buffer to send
  * @param[in] pRxBuffer Point to the read buffer
  * @param[in] NumByte Number of bytes to send and read
  * @retval None
  * @par Required preconditions:
  * None
  * @par Functions called:
  * - LCD_ChipSelect
  * - LCD_SPISendByte
  * @par Example:
  * @code
  * u8 txbuff[8], rxbuff[8];
  * LCD_SendBuffer(rxbuff, 8);
  * @endcode
  */
void LCD_SendBuffer(u8 *pTxBuffer, u8 *pRxBuffer, u8 NumByte)
{
  LCD_ChipSelect(ENABLE);
  while (NumByte--) /* while there is data to be read */
  {
    LCD_SPISendByte(*pTxBuffer);
    pTxBuffer++;
  }
  LCD_ChipSelect(DISABLE);
}

/**
  * @brief Initialize the LCD
  * @par Parameters:
  * None
  * @retval None
  * @par Required preconditions:
  * None
  * @par Functions called:
  * - GPIO_Init
  * - LCD_SendByte
  * - LCD_Clear
  * - LCD_Backlight
  * @par Example:
  * @code
  * LCD_Init();
  * @endcode
  */
void LCD_Init(void)
{
  /* Set LCD ChipSelect pin in Output push-pull low level (chip select disabled) */
  GPIO_Init(LCD_CS_PORT, LCD_CS_PIN, GPIO_MODE_OUT_PP_LOW_FAST);

  /* Set LCD backlight pin in Output push-pull low level (backlight off) */
  GPIO_Init(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, GPIO_MODE_OUT_PP_LOW_FAST);

  LCD_SendByte(COMMAND_TYPE, SET_TEXT_MODE); /* Set the LCD in TEXT mode */
  LCD_SendByte(COMMAND_TYPE, DISPLAY_ON); /* Enable the display */
  LCD_Clear(); /* Clear the LCD */
  LCD_SendByte(COMMAND_TYPE, ENTRY_MODE_SET_INC); /* Select the entry mode type */

}

/**
  * @brief Clear the LCD
  * @par Parameters:
  * None
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_Clear();
  * @endcode
  */
void LCD_Clear(void)
{

  u16 i;

  LCD_SendByte(COMMAND_TYPE, DISPLAY_CLR); /* Clear the LCD */

  /* Delay required to complete LCD clear command */
  for (i = 0; i < 5000; i++)
  {
    /* Wait */
  }

}

/**
  * @brief Set the LCD in text mode
  * @par Parameters:
  * None
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * - LCD_Clear
  * @par Example:
  * @code
  * LCD_SetTextMode();
  * @endcode
  */
void LCD_SetTextMode(void)
{
  LCD_SendByte(COMMAND_TYPE, SET_TEXT_MODE);
  LCD_Clear();
}

/**
  * @brief Set the LCD in graphic mode
  * @par Parameters:
  * None
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * - LCD_Clear
  * @par Example:
  * @code
  * LCD_SetGraphicMode();
  * @endcode
  */
void LCD_SetGraphicMode(void)
{
  LCD_Clear();
  LCD_SendByte(COMMAND_TYPE, SET_GRAPHIC_MODE);
  
}

/**
  * @brief Clear one LCD
  * @param[in] Line Line to be cleared (LCD_LINE1 or LCD_LINE2)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_ClearLine(LCD_LINE1);
  * @endcode
  */
void LCD_ClearLine(u8 Line)
{

  u8 CharPos;

  /* Select the line to be cleared */
  LCD_SendByte(COMMAND_TYPE, Line);

  /* Clear the selected line */
  for (CharPos = 0; CharPos < LCD_LINE_MAX_CHAR; CharPos++)
  {
    LCD_SendByte(DATA_TYPE, ' ');
  }

}

/**
  * @brief Set the LCD cursor to the specified location
  * @param[in] Line Line where the cursor will be set (LCD_LINE1 or LCD_LINE2)
  * @param[in] Offset is the position offset (only even position are supported)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_SetCursorPos(LCD_LINE1, 2);
  * @endcode
  */
void LCD_SetCursorPos(u8 Line, u8 Offset)
{
  LCD_SendByte(COMMAND_TYPE, (u8)(Line + Offset));
}

/**
  * @brief Display a character at the current cursor position
  * @param[in] Ascii is the character ascii code to be displayed
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_PrintChar('A');
  * @endcode
  */
void LCD_PrintChar(u8 Ascii)
{
  LCD_SendByte(DATA_TYPE, Ascii);
}

/**
  * @brief Display a string on the selected line of the LCD
  * @param[in] Line Line where to display the string (LCD_LINE1 or LCD_LINE2)
  * @param[in] AutoComplete Defines if the line must be completed with spaces or not
  * @param[in] Append Defines if the string is appended or not
  * @param[in] ptr Pointer to the string to display
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_PrintString(LCD_LINE1, DISABLE, DISABLE, "Hello");
  * @endcode
  */
void LCD_PrintString(u8 Line, FunctionalState AutoComplete, FunctionalState Append, u8 *ptr)
{

  u8 CharPos = 0;

  /* Set cursor position at beginning of Line if Append option is enabled */
  if (Append == DISABLE)
  {
    LCD_SendByte(COMMAND_TYPE, Line);
  }

  /* Display each character of the string */
  while ((*ptr != 0) && (CharPos < LCD_LINE_MAX_CHAR))
  {
    LCD_SendByte(DATA_TYPE, *ptr);
    CharPos++;
    ptr++;
  }

  /* Complete the line with spaces if AutoFill option is enabled */
  if (AutoComplete == ENABLE)
  {
    while (CharPos < LCD_LINE_MAX_CHAR)
    {
      LCD_SendByte(DATA_TYPE, ' ');
      CharPos++;
    }
  }

}

/**
  * @brief Display a string on the LCD with automatic carriage return
  * @param[in] ptr Pointer to the string to display
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The LCD is cleared prior to display the string
  * - The character % is not supported
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_PrintMsg("This is a very long message");
  * @endcode
  */
void LCD_PrintMsg(u8 *ptr)
{

  u8 Char = 0;
  u8 CharPos = 0;

  LCD_Clear(); /* Clear the LCD display */

  /* Set cursor to home position on line 1 */
  LCD_SendByte(COMMAND_TYPE, LCD_LINE1);

  /* Send String */
  while ((*ptr != 0) && (CharPos < (LCD_LINE_MAX_CHAR * 2)))
  {

    /* Check if string length is bigger than LINE1 */
    if (CharPos == LCD_LINE_MAX_CHAR)
    {
      LCD_SendByte(COMMAND_TYPE, LCD_LINE2); /* Select second line */
    }

    Char = *ptr;

    switch (Char)
    {
      case ('\r'):
        /* Carriage return */
        CharPos++;
        ptr++;
      break;
      case ('\n'):
        CharPos = 0;
        ptr++;
        /* Set cursor to line 2 */
        LCD_SendByte(COMMAND_TYPE, LCD_LINE2);
      break;
      default:
        /* Display characters different from (\r, \n) */
        LCD_SendByte(DATA_TYPE, Char);
        CharPos++;
        ptr++;
      break;
    }
  }

}

/**
  * @brief Display the Number in decimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 9 (max 1 digit)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintDec2(6);
  * @endcode
  */
void LCD_PrintDec1(u8 Number)
{

  u8 NbreTmp;

  if (Number < (u8)10)
  {
   
    /* Display second digit of the number : 10 */
    NbreTmp = (u8)(Number / (u8)10);
   
    /* Display last digit of the number : Units */
    NbreTmp = (u8)(Number - (u8)((u8)10 * NbreTmp));
    LCD_PrintChar((u8)(NbreTmp + (u8)0x30));
   
  }

}

/**
  * @brief Display the Number in decimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 99 (max 2 digits)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintDec2(60);
  * @endcode
  */
void LCD_PrintDec2(u8 Number)
{

  u8 NbreTmp;

  if (Number < (u8)100)
  {
   
    /* Display second digit of the number : 10 */
    NbreTmp = (u8)(Number / (u8)10);
    LCD_PrintChar((u8)(NbreTmp + (u8)0x30));
   
    /* Display last digit of the number : Units */
    NbreTmp = (u8)(Number - (u8)((u8)10 * NbreTmp));
    LCD_PrintChar((u8)(NbreTmp + (u8)0x30));
   
  }

}

/**
  * @brief Display the Number in decimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 999 (max 3 digits)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintDec3(879);
  * @endcode
  */
void LCD_PrintDec3(u16 Number)
{

  u8 Nbre1Tmp;
  u8 Nbre2Tmp;

  if (Number < (u16)1000)
  {
   
    /* Display first digit of the number : 100 */
    Nbre1Tmp = (u8)(Number / (u8)100);
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
   
    /* Display second digit of the number : 10 */
    Nbre1Tmp = (u8)(Number - ((u8)100 * Nbre1Tmp));
    Nbre2Tmp = (u8)(Nbre1Tmp / (u8)10);
    LCD_PrintChar((u8)(Nbre2Tmp + (u8)0x30));
   
    /* Display last digit of the number : Units */
    Nbre1Tmp = ((u8)(Nbre1Tmp - (u8)((u8)10 * Nbre2Tmp)));
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
  }
  
}

/**
  * @brief Display the Number in decimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 9999 (max 4 digits)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintDec4(1600);
  * @endcode
  */
void LCD_PrintDec4(u16 Number)
{

  u16 Nbre1Tmp;
  u16 Nbre2Tmp;

  if (Number < (u16)10000)
  {
   
    /* Display first digit of the number : 1000 */
    Nbre1Tmp = (u16)(Number / (u16)1000);
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
   
    /* Display second digit of the number : 100 */
    Nbre1Tmp = (u16)(Number - ((u16)1000 * Nbre1Tmp));
    Nbre2Tmp = (u16)(Nbre1Tmp / (u8)100);
    LCD_PrintChar((u8)(Nbre2Tmp + (u8)0x30));
   
    /* Display second digit of the number : 10 */
    Nbre1Tmp = (u16)(Nbre1Tmp - ((u16)100 * Nbre2Tmp));
    Nbre2Tmp = (u16)(Nbre1Tmp / (u16)10);
    LCD_PrintChar((u8)(Nbre2Tmp + (u8)0x30));
   
    /* Display last digit of the number : Units */
    Nbre1Tmp = ((u16)(Nbre1Tmp - (u16)((u16)10 * Nbre2Tmp)));
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
  }

}

/**
  * @brief Display the Number in Hexadecimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 15 (only 1 digit)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintHex1(10);
  * @endcode
  */
void LCD_PrintHex1(u8 Number)
{
  if (Number < (u8)0x0A)
  {
    LCD_PrintChar((u8)(Number + (u8)0x30));
  }
  else
    if (Number < (u8)0x10)
    {
      LCD_PrintChar((u8)(Number + (u8)0x37));
    }
    else
    {
      LCD_PrintChar('-');
    }
}

/**
  * @brief Display the Number in Hexadecimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 255 (max 2 digits)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintHex2(200);
  * @endcode
  */
void LCD_PrintHex2(u8 Number)
{
  LCD_PrintHex1((u8)(Number >> (u8)4));
  LCD_PrintHex1((u8)(Number & (u8)0x0F));
}

/**
  * @brief Display the Number in Hexadecimal format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 4095 (max 3 digits)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintHex3(1C4);
  * @endcode
  */
void LCD_PrintHex3(u16 Number)
{
  LCD_PrintHex1((u8)(Number >> (u8)8)); 
  LCD_PrintHex1((u8)((u8)(Number) >> (u8)4));
  LCD_PrintHex1((u8)((u8)(Number) & (u8)0x0F));
}

/**
  * @brief Display the Number in binary format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 3 (max 2 digits)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintBin2(2);
  * @endcode
  */
void LCD_PrintBin2(u8 Number)
{
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x02) >> (u8)1));
  LCD_PrintHex1((u8)(Number & (u8)0x01));
}

/**
  * @brief Display the Number in binary format at the current cursor position
  * @param[in] Number Number to be displayed
  * @note Convert only numbers between 0 and 15 (max 4 digits)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * - The cursor position must be set before calling this function
  * @par Functions called:
  * - LCD_PrintChar
  * @par Example:
  * @code
  * LCD_PrintBin4(15);
  * @endcode
  */
void LCD_PrintBin4(u8 Number)
{
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x08) >> (u8)3));
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x04) >> (u8)2));
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x02) >> (u8)1));
  LCD_PrintHex1((u8)(Number & (u8)0x01));
}

/**
  * @brief Display CGRAM on even address
  * @param[in] address Display address
  * @param[in] ptrTable Pointer a the CGRAM table to be displayed
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_DisplayCGRAM0(0x80);
  * @endcode
  */
void LCD_DisplayCGRAM0(u8 address, u8 *ptrTable)
{

  u8 u;

  /* Set CGRAM Address */
  LCD_SendByte(COMMAND_TYPE, (u8)0x40);

  u = 32; /* Nb byte in the table */
  while (u)
  {
    LCD_SendByte(DATA_TYPE, ptrTable[32 - u]);
    u--;
  }

  /* Setup Display Address */
  LCD_SendByte(COMMAND_TYPE, address);
  LCD_SendByte(DATA_TYPE, (u8)0x00);
  LCD_SendByte(DATA_TYPE, (u8)0x00);

}

/**
  * @brief Display CGRAM on odd address
  * @param[in] address Display address
  * @param[in] ptrTable Pointer a the CGRAM table to be displayed
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_DisplayCGRAM1(0x80);
  * @endcode
  */
void LCD_DisplayCGRAM1(u8 address, u8 *ptrTable)
{

  u8 u;

 /* Set CGRAM Address */
  LCD_SendByte(COMMAND_TYPE, (u8)((u8)0x40 | (u8)0x10));

  u = 32; /* Nb byte in the table */
  while (u)
  {
    LCD_SendByte(DATA_TYPE, ptrTable[32 - u]);
    u--;
  }

  /* Setup Display Address */
  LCD_SendByte(COMMAND_TYPE, (u8)(address + 1));
  LCD_SendByte(DATA_TYPE, (u8)0x00);
  LCD_SendByte(DATA_TYPE, (u8)0x02);

}

/**
  * @brief Display ST logo
  * @param[in] address Display address (LINE1:0x80-0x87 and LINE2:0x90-0x97)
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_DisplayLogo(0x80);
  * @endcode
  */
void LCD_DisplayLogo(u8 address)
{
  LCD_DisplayCGRAM0(address, S_CGRAM);
  LCD_DisplayCGRAM1(address, T_CGRAM);
}

/**
  * @brief Display a string in rolling mode
  * @param[in] Line Line used for displaying the text (LCD_LINE1 or LCD_LINE2)
  * @param[in] ptr Pointer to the text to display
  * @param[in] speed Rolling speed
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * - LCD_ClearLine
  * - LCD_Delay
  * @par Example:
  * @code
  * u8 *pText;
  * pText = "Welcome into the fabulous world of STM8...";
  * LCD_RollString(LCD_LINE2, pText, 0xC000);
  * @endcode
  */
void LCD_RollString(u8 Line, u8 *ptr, u16 speed)
{

  u8 CharPos = 0;
  u8 *ptr2;
  
  /* Set cursor position at beginning of line */
  LCD_SendByte(COMMAND_TYPE, Line);
  
  ptr2 = ptr;
  
  /* Display each character of the string */
  while (*ptr2 != 0)
  {
   
    if (*ptr != 0)
    {
      LCD_SendByte(DATA_TYPE, *ptr);
      ptr++;
    }
    else
    {
      LCD_SendByte(DATA_TYPE, ' ');
    }
    
    CharPos++;
   
    if (CharPos == LCD_LINE_MAX_CHAR)
    {
      LCD_Delay(speed);
      LCD_ClearLine(Line);
      LCD_SendByte(COMMAND_TYPE, Line);
      CharPos = 0;
      ptr2++;
      ptr = ptr2;
    }
    
  }

}
/**
  * @brief Display a string from current position of the LCD cursor
  * @param[in] ptr Pointer to the string to display
  * @retval None
  * @par Required preconditions:
  * - LCD must be enabled
  * @par Functions called:
  * - LCD_SendByte
  * @par Example:
  * @code
  * LCD_Print("Hello");
  * @endcode
  */
void LCD_Print(u8 *ptr) {
  while (*ptr) 					// Display the string */
  LCD_SendByte(DATA_TYPE, *ptr++);
}
/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
