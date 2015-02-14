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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LCD_WriteStrobe()		        { WR_LCD_clr; WR_LCD_set; }

/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern LCD_Typedef R61581, SSD1289;

/* Private variables ---------------------------------------------------------*/
LCD_Typedef *LCD = &R61581;

/* Private function prototypes -----------------------------------------------*/
static void NGL_LCD_FSMC_WriteCommand(uint16_t Data);
static void NGL_LCD_FSMC_WriteData(uint16_t Data);
static uint16_t NGL_LCD_FSMC_ReadData(void);

static void NGL_LCD_GPIO_WriteCommand(uint16_t Data);
static __inline void NGL_LCD_GPIO_WriteData(uint16_t Data);
static void NGL_LCD_GPIO_WriteData_16bpp_8b(uint16_t Data);
static void NGL_LCD_GPIO_WriteData_18bpp_8b(uint16_t Data);
static void NGL_LCD_GPIO_WriteData_18bpp_16b(uint16_t Data);

void (*NGL_Delay)(uint16_t delay) = (void*)0;


/* Private function  --------------------------------------------------------*/

/**
  * @brief  LCD_Set_ConnectionAndColor
  * @param  None
  * @retval None
  */
static void NGL_LCD_Set_ConnectionType_DataBusBits_ColorBits(void)
{
	if(LCD->ConnectionType == LCD_FSMC_Connect)
	{
		LCD->WriteCommand = NGL_LCD_FSMC_WriteCommand;
		LCD->WriteData = NGL_LCD_FSMC_WriteData;
		LCD->ReadData = NGL_LCD_FSMC_ReadData;
		LCD->WritePixel = NGL_LCD_FSMC_WriteData;
	}
	else if(LCD->ConnectionType == LCD_GPIO_Connect)
	{
		LCD->WriteCommand = NGL_LCD_GPIO_WriteCommand;
		LCD->WriteData = NGL_LCD_GPIO_WriteData;

		if((LCD->ColorBits == 16) && (LCD->DataBusBits == 8))
		{
			LCD->WritePixel = NGL_LCD_GPIO_WriteData_16bpp_8b;
		}
		else if((LCD->ColorBits == 16) && (LCD->DataBusBits == 16))
		{
			LCD->WritePixel = NGL_LCD_GPIO_WriteData;
		}
		else if((LCD->ColorBits == 18) && (LCD->DataBusBits == 8))
		{
			LCD->WritePixel = NGL_LCD_GPIO_WriteData_18bpp_8b;
		}
		else if((LCD->ColorBits == 18) && (LCD->DataBusBits == 16))
		{
			LCD->WritePixel = NGL_LCD_GPIO_WriteData_18bpp_16b;
		}

		LCD->ReadData = (void*)0;
	}
}


/**
  * @brief  NGL_LCD_ControllerInit
  * @param  None
  * @retval None
  */
int8_t NGL_LCD_ControllerInit(LCD_Typedef* LCD_init, void *pDelayFunc)
{
    LCD = LCD_init;
    NGL_LCD_Set_ConnectionType_DataBusBits_ColorBits();

    if(pDelayFunc != (void*)0)
    {
    	 NGL_Delay = pDelayFunc;
    	 LCD->ControllerInit();
        return 0;
    }
    else
    {
        return -1;
    }
}


/**
  * @brief  NGL_LCD_GetController_FromIndex
  * @param  None
  * @retval None
  */
LCD_Typedef* NGL_LCD_GetController_FromIndex(uint8_t index)
{
    switch(index)
    {
    case 0:
        return &R61581;
        break;
    case 1:
        return &SSD1289;
        break;
    default:
        return (void*)0;
    }
}


/**
  * @brief  NGL_LCD_SetRotation
  * @param
  * @retval None
  */
void NGL_LCD_SetRotation(NGL_RotationType NewState)
{
	uint16_t tmp;

	LCD->SetRotation(NewState);

	if(  (((NewState == _90_degree) || (NewState == _270_degree)) &&
			((LCD->Rotation == _0_degree) || (LCD->Rotation == _180_degree))) ||

			(((NewState == _0_degree) || (NewState == _180_degree)) &&
					((LCD->Rotation == _90_degree) || (LCD->Rotation == _270_degree))) )
	{
		tmp = LCD->X_Max;
		LCD->X_Max = LCD->Y_Max;
		LCD->Y_Max = tmp;
	}

	LCD->Rotation = NewState;
}


/**
  * @brief  LCD_FSMC_WriteCommand
  * @param
  * @retval None
  */
static void NGL_LCD_FSMC_WriteCommand(uint16_t Data)
{
	LCD_FMSC_COMMAND = Data;
}


/**
  * @brief  NGL_LCD_FSMC_WriteData
  * @param
  * @retval None
  */
static void NGL_LCD_FSMC_WriteData(uint16_t Data)
{
	LCD_FMSC_DATA = Data;
}


/**
  * @brief  NGL_LCD_FSMC_ReadData
  * @param  None
  * @retval
  */
static uint16_t NGL_LCD_FSMC_ReadData(void)
{
	return LCD_FMSC_DATA;
}


/**
  * @brief  NGL_LCD_GPIO_WriteCommand
  * @param  None
  * @retval None
  */
static void NGL_LCD_GPIO_WriteCommand(uint16_t Data)
{
	RS_LCD_clr;				  //register index write active

	LCD_GPIO_DATA(Data);
	LCD_WriteStrobe();

	RS_LCD_set;               //register index write inactive
}


/**
  * @brief  NGL_LCD_GPIO_WriteData
  * @param
  * @retval None
  */
static __inline void NGL_LCD_GPIO_WriteData(uint16_t Data)
{
	LCD_GPIO_DATA(Data);
	LCD_WriteStrobe();
}


/**
  * @brief  NGL_LCD_GPIO_WriteData_16bpp_8b
  * @param
  * @retval None
  */
static void NGL_LCD_GPIO_WriteData_16bpp_8b(uint16_t Data)
{
	LCD_GPIO_DATA(Data >> 8);			//
	LCD_WriteStrobe();

	LCD_GPIO_DATA(Data & 0x00FF);		//
	LCD_WriteStrobe();
}


/**
  * @brief  NGL_LCD_GPIO_WriteData_18bpp_8b
  * @param
  * @retval None
  */
static void NGL_LCD_GPIO_WriteData_18bpp_8b(uint16_t Data)
{
	LCD_GPIO_DATA((Data & 0xF800) >> 8);      //
	LCD_WriteStrobe();

	LCD_GPIO_DATA((Data & 0x07E0) >> 3);      //
	LCD_WriteStrobe();

	LCD_GPIO_DATA((Data & 0x001F) << 3);      //
	LCD_WriteStrobe();
}


/**
  * @brief  NGL_LCD_GPIO_WriteData_18bpp_16b
  * @param
  * @retval None
  */
static void NGL_LCD_GPIO_WriteData_18bpp_16b(uint16_t Data)
{
	LCD_GPIO_DATA((Data & 0xFF00) >> 8);      // HC573
	HC573_LE_set; HC573_LE_clr;

	LCD_GPIO_DATA(Data & 0x00FF);
	LCD_WriteStrobe();
}


/**
  * @brief  NGL_LCD_WriteRegister
  * @param
  * @retval None
  */
void NGL_LCD_WriteRegister(uint16_t RegisterAddress, uint16_t Data)
{
	LCD->WriteCommand(RegisterAddress);
	LCD->WriteData(Data);
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
