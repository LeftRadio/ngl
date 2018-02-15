/**
  ******************************************************************************
  * @file    LCD_MAL.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "NGL_types.h"
#include "LCD_GPIO.h"
#include "LCD_HAL.h"
#include "LCD_MAL.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function  --------------------------------------------------------*/

/**
  * @brief  LCD_SetCursor
  * @param  X, Y
  * @retval None
  */
void NGL_LCD_SetCursor(uint16_t X, uint16_t Y)
{
	CS_LCD_set;
	LCD->SetCursor(X, Y);
	CS_LCD_clr;
}

/**
  * @brief  LCD_PutPixel
  * @param  Color
  * @retval None
  */
void NGL_LCD_PutPixel(uint16_t Color)
{
	if(LCD->ConnectionType == LCD_GPIO_Connect)
	{
		CS_LCD_set;
		LCD->WritePixel(Color);
		CS_LCD_clr;
	}
	else if(LCD->ConnectionType == LCD_FSMC_Connect)
	{
		LCD->WritePixel(Color);
	}
}

/**
  * @brief  LCD_PutSeveralPixels
  * @param  Counter - pixel counter to write, Color - pixels color
  * @retval None
  */
static __inline void PutPixelsFast(uint32_t Counter, uint16_t Color)
{
	if(LCD->ConnectionType == LCD_GPIO_Connect)
	{
		CS_LCD_set;

		/* Write pointers */
		do
		{
			LCD->WritePixel(Color);
			// WR_LCD_clr;	WR_LCD_set;
		}
		while(Counter--);

		CS_LCD_clr;
	}
	else
	{
		do
		{
			LCD_FMSC_DATA = Color;
		}
		while(Counter--);
	}
}

/**
  * @brief  LCD_GetPixels
  * @param
  * @retval
  */
void NGL_LCD_GetPixels(uint8_t *ReadData, uint16_t NumPixels)
{
	if(LCD->ConnectionType == LCD_GPIO_Connect)
	{
		CS_LCD_set;
		LCD->GetPixels(ReadData, NumPixels);
		CS_LCD_clr;
	}
	else if(LCD->ConnectionType == LCD_FSMC_Connect)
	{
		LCD->GetPixels(ReadData, NumPixels);
	}
}

/**
  * @brief  LCD_SetFullScreen
  * @param  None
  * @retval None
  */
void NGL_LCD_SetFullScreen(void)
{
	LCD->SetArea(0, 0, LCD->X_Max, LCD->Y_Max);
}

/**
  * @brief  LCD_ClearArea
  * @param  X0, Y0, X1, Y1, Color
  * @retval None
  */
void NGL_LCD_ClearArea(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color)
{
	uint32_t pointCounter = ((X1 - X0) + 1) * ((Y1 - Y0) + 1);

	LCD->SetArea(X0, Y0, X1, Y1);  // ”становка области вывода LCD равной области очистки
	PutPixelsFast(pointCounter, Color);
	NGL_LCD_SetFullScreen();
}

/**
  * @brief  LCD_Clear_AllScreen
  * @param  Color
  * @retval None
  */
void NGL_LCD_Clear_AllScreen(uint16_t Color)
{
	NGL_LCD_SetFullScreen();
	PutPixelsFast(LCD->Max_Points, Color);
}

/**
  * @brief  NGL_LCD_SetRotation
  * @param
  * @retval None
  */
void NGL_LCD_SetRotation(NGL_RotationLCD NewState)
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




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
