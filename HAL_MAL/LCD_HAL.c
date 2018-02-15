/**
  ******************************************************************************
  * @file    LCD_HAL.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "LCD_HAL.h"
#include "LCD_GPIO.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WRITE_STROBE()                { WR_LCD_clr; WR_LCD_set; }

/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NGL_HAL_Typedef *LCD;

void (*NGL_Delay)(uint16_t delay) = (void*)0;

/* Private function prototypes -----------------------------------------------*/
/* Private function  --------------------------------------------------------*/

/**
  * @brief  LCD_FSMC_WriteCommand
  * @param
  * @retval None
  */
static void __FSMC_WriteCommand(uint16_t Data) {
    LCD_FMSC_COMMAND = Data;
}

/**
  * @brief  __FSMC_WriteData
  * @param
  * @retval None
  */
static void __FSMC_WriteData(uint16_t Data) {
    LCD_FMSC_DATA = Data;
}

/**
  * @brief  __FSMC_ReadData
  * @param  None
  * @retval
  */
static uint16_t __FSMC_ReadData(void) {
    return LCD_FMSC_DATA;
}

/**
  * @brief  __GPIO_WriteCommand
  * @param  None
  * @retval None
  */
static void __GPIO_WriteCommand(uint16_t Data) {
    /* register index write active */
    RS_LCD_clr;
    /* write data to register*/
    LCD_GPIO_DATA(Data);
    WRITE_STROBE();
    /* register index write reset */
    RS_LCD_set;
}

/**
  * @brief  __GPIO_WriteData
  * @param
  * @retval None
  */
static void __GPIO_WriteData(uint16_t Data) {
    LCD_GPIO_DATA(Data);
    WRITE_STROBE();
}

/**
  * @brief  __GPIO_WriteData_16bpp_8b
  * @param
  * @retval None
  */
static void __GPIO_WriteData_16bpp_8b(uint16_t Data) {
    LCD_GPIO_DATA(Data >> 8);
    WRITE_STROBE();
    LCD_GPIO_DATA(Data & 0x00FF);
    WRITE_STROBE();
}

/**
  * @brief  __GPIO_WriteData_18bpp_8b
  * @param
  * @retval None
  */
static void __GPIO_WriteData_18bpp_8b(uint16_t Data) {
    LCD_GPIO_DATA((Data & 0xF800) >> 8);
    WRITE_STROBE();
    LCD_GPIO_DATA((Data & 0x07E0) >> 3);
    WRITE_STROBE();
    LCD_GPIO_DATA((Data & 0x001F) << 3);
    WRITE_STROBE();
}

/**
  * @brief  __GPIO_WriteData_18bpp_16b
  * @param
  * @retval None
  */
static void __GPIO_WriteData_18bpp_16b(uint16_t Data) {
    LCD_GPIO_DATA((Data & 0xFF00) >> 8);
    HC573_LE_set; HC573_LE_clr;
    LCD_GPIO_DATA(Data & 0x00FF);
    WRITE_STROBE();
}

/**
  * @brief  LCD_Set_ConnectionAndColor
  * @param  None
  * @retval None
  */
static void __Set_ConnectionType(void) {
  if(LCD->ConnectionType == LCD_FSMC_Connect)
  {
    LCD->WriteCommand = __FSMC_WriteCommand;
    LCD->WriteData = __FSMC_WriteData;
    LCD->ReadData = __FSMC_ReadData;
    LCD->WritePixel = __FSMC_WriteData;
  }
  else if(LCD->ConnectionType == LCD_GPIO_Connect)
  {
    LCD->WriteCommand = __GPIO_WriteCommand;
    LCD->WriteData = __GPIO_WriteData;

    if((LCD->ColorBits == 16) && (LCD->DataBusBits == 8))
    {
      LCD->WritePixel = __GPIO_WriteData_16bpp_8b;
    }
    else if((LCD->ColorBits == 16) && (LCD->DataBusBits == 16))
    {
      LCD->WritePixel = __GPIO_WriteData;
    }
    else if((LCD->ColorBits == 18) && (LCD->DataBusBits == 8))
    {
      LCD->WritePixel = __GPIO_WriteData_18bpp_8b;
    }
    else if((LCD->ColorBits == 18) && (LCD->DataBusBits == 16))
    {
      LCD->WritePixel = __GPIO_WriteData_18bpp_16b;
    }

    LCD->ReadData = (void*)0;
  }
}

/**
  * @brief  NGL_LCD_ControllerInit
  * @param  None
  * @retval None
  */
int8_t NGL_HAL_Init(NGL_HAL_Typedef* controller, void *pDelayFunc) {
    /* */
    LCD = controller;
    /* */
    __Set_ConnectionType();
    /* */
    if(pDelayFunc != (void*)0) {
        NGL_Delay = pDelayFunc;
        LCD->ControllerInit();
        return 0;
    }
    else {
        return -1;
    }
}

/**
  * @brief  NGL_LCD_WriteRegister
  * @param
  * @retval None
  */
void NGL_HAL_WriteRegister(uint16_t RegisterAddress, uint16_t Data) {
    LCD->WriteCommand(RegisterAddress);
    LCD->WriteData(Data);
}


/*******************************************************************************
      END FILE
*******************************************************************************/