/**
  ******************************************************************************
  * @file    	Graphics_Primitive.c
  * @author  	LeftRadio
  * @version 	V1.2.0
  * @date
  * @brief		NGL :: Graphics primitive Layer sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "NGL_types.h"
#include "LCD_HAL.h"
#include "LCD_MAL.h"
#include "Graphics_Primitive.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t Antialiasing = 0;
volatile uint16_t NumLevels = 256;
volatile uint16_t IntensityBits = 8;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private function  --------------------------------------------------------*/

/**
  * @brief  SetAntialiasingState
  * @param  NewState
  * @retval None
  */
void NGL_GP_SetAntialiasingLine_State(uint8_t NewState)
{
	if((NewState != 0) && (NewState != 1)) return;
	Antialiasing = NewState;
}


/**
  * @brief  NGL_GP_AntialiasingParam
  * @param  NumLevels is number of color levels. Pass 256.
            IntensityBits denotes bits used to represent color component. Pass 8.
  * @retval None
  */
void NGL_GP_AntialiasingParam(uint16_t numLevels, uint16_t intensityBits)
{
	NumLevels = numLevels;
	IntensityBits = intensityBits;
}



/**
  * @brief  DrawPixel
  * @param  X, Y, Color
  * @retval None
  */
void NGL_GP_DrawPixel(uint16_t X, uint16_t Y, uint16_t Color)
{
	CS_LCD_set;
	LCD->SetCursor(X, Y);			// Установка курсора в нужную позицию
	LCD->WritePixel(Color);		// Нарисовать точку на установленной позиции
	CS_LCD_clr;
}


/**
  * @brief  NGL_GP_Draw_Vertical_Line
  * @param
  * @retval None
  */
static __inline void NGL_GP_Draw_Vertical_Line(uint16_t X0, uint16_t Y0, int16_t Length, uint16_t Color)
{
	if(Length < 0){ Length = -Length; Y0 -= Length; }

	if(X0 > LCD->X_Max) X0 = LCD->X_Max;
	if(Y0 > LCD->Y_Max) Y0 = LCD->Y_Max;
	if(Y0 + Length > LCD->Y_Max) Length = LCD->Y_Max - Y0;

	CS_LCD_set;
	/* Horizontal line */
	do
	{
		LCD->SetCursor(X0, Y0);
		LCD->WritePixel(Color);
		Y0++;
	}
	while(Length--);
	CS_LCD_clr;
}
/**
  * @brief  NGL_GP_Draw_Horizontal_Line
  * @param
  * @retval None
  */
static __inline void NGL_GP_Draw_Horizontal_Line(uint16_t X0, uint16_t Y0, int16_t Length, uint16_t Color)
{
	if(Length < 0){ Length = -Length; X0 -= Length; }

	if(X0 > LCD->X_Max) X0 = LCD->X_Max;
	if(X0 + Length > LCD->X_Max) Length = LCD->X_Max - X0;
	if(Y0 > LCD->Y_Max) Y0 = LCD->Y_Max;

	CS_LCD_set;
	LCD->SetCursor(X0, Y0);
	/* Vertical line */
	do
	{
		LCD->WritePixel(Color);
	}
	while(Length--);

	CS_LCD_clr;
}


/**
  * @brief  NGL_GP_DrawLine
  * @param  (X0, Y0) is start point of line.
            (X1, Y1) is end point of line.
            Color
  * @retval None
  */
void NGL_GP_DrawLine(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color)
{
//	unsigned short IntensityShift;
	short DeltaX = X1 - X0;
	short DeltaY = Y1 - Y0;
	unsigned short ErrorAcc;

	short XDir, YDir, error2;

	if (DeltaX >= 0) XDir = 1;
	else { XDir = -1; DeltaX = -DeltaX; /* make DeltaX positive */	}

	if (DeltaY >= 0) YDir = 1;
	else{ YDir = -1; DeltaY = -DeltaY; /* make DeltaX positive */	}

#ifdef NGL_CLIP_VER
	if(X0 > LCD->X_Max) X0 = LCD->X_Max;
	if(X1 > LCD->X_Max) X1 = LCD->X_Max;
	if(Y0 > LCD->Y_Max) Y0 = LCD->Y_Max;
	if(Y1 > LCD->Y_Max) Y1 = LCD->Y_Max;
#endif

	/* Special-case horizontal, vertical, and diagonal lines, which
	require no weighting because they go right through the center of
	every pixel */
	if((DeltaX == 0) && (DeltaY == 0))
	{
		LCD->SetCursor(X0, Y0);
		LCD->WritePixel(Color);
	}
	else if (DeltaY == 0)
	{
		if (XDir < 0) X0 = X1;
		NGL_GP_Draw_Horizontal_Line(X0, Y0, DeltaX, Color);
	}
	else if (DeltaX == 0)
	{
		if (YDir < 0) Y0 = Y1;
		NGL_GP_Draw_Vertical_Line(X0, Y0, DeltaY, Color);
	}
	else if (DeltaX == DeltaY)
	{
		/* Diagonal line */
		CS_LCD_set;
		do
		{
			LCD->SetCursor(X0, Y0);
			LCD->WritePixel(Color);

			X0 += XDir;
			Y0 += YDir;
		}
		while (--DeltaY != 0);
		CS_LCD_clr;
	}
	else /* ---- Line is not horizontal, diagonal or vertical ---- */
	{
		CS_LCD_set;
		ErrorAcc = DeltaX - DeltaY;

		for (;;)
		{
			LCD->SetCursor(X0, Y0);
			LCD->WritePixel(Color);

			if(X0 == X1 && Y0 == Y1) break;

			error2 = ErrorAcc * 2;

			if(error2 > -DeltaY)
			{
				ErrorAcc -= DeltaY;
				X0 += XDir;
			}

			if(error2 < DeltaX)
			{
				ErrorAcc += DeltaX;
				Y0 += YDir;
			}
		}
		CS_LCD_clr;
	}
}


/**
  * @brief  NGL_GP_DrawRect
  * @param  X0, Y0, X1, Y1
            Color
  * @retval None
  */
void NGL_GP_DrawRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color)
{
    int yl = (Y1 - Y0);
    int xl = (X1 - X0);

	NGL_GP_Draw_Vertical_Line(X0, Y0, yl, Color);     // Left
	NGL_GP_Draw_Vertical_Line(X1, Y0, yl, Color);     // Right

	NGL_GP_Draw_Horizontal_Line(X0, Y0, xl, Color);   // Bottom
	NGL_GP_Draw_Horizontal_Line(X0, Y1, xl, Color);   // Top
}


/**
  * @brief  NGL_GP_DrawFillRect
  * @param  X0, Y0, X1, Y1
            Color
            border, borderColor
  * @retval None
  */
void NGL_GP_DrawFillRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color, FunctionalState border, uint16_t borderColor)
{
	/* Fill rectangle */
	NGL_LCD_ClearArea(X0, Y0, X1, Y1, Color);

	/* Set new graphics color and draw border */
	if(border == ENABLE) {
		NGL_GP_DrawRect(X0, Y0, X1, Y1, borderColor);
	}
}


/**
  * @brief  NGL_GP_DrawCircle
  * @param  X0, Y0
            Radius, Color
  * @retval None
  */
void NGL_GP_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint16_t Color)
{
	int32_t  D;       /* Decision Variable */
	uint32_t  CurX;   /* Current X Value */
	uint32_t  CurY;   /* Current Y Value */

	D = 3 - (Radius << 1);
	CurX = 0;
	CurY = Radius;

	while (CurX <= CurY)
	{
		NGL_LCD_SetCursor(Xpos + CurX, Ypos + CurY);
		NGL_LCD_PutPixel(Color);

		NGL_LCD_SetCursor(Xpos + CurX, Ypos - CurY);
		NGL_LCD_PutPixel(Color);

		NGL_LCD_SetCursor(Xpos - CurX, Ypos + CurY);
		NGL_LCD_PutPixel(Color);

		NGL_LCD_SetCursor(Xpos - CurX, Ypos - CurY);
		NGL_LCD_PutPixel(Color);

		NGL_LCD_SetCursor(Xpos + CurY, Ypos + CurX);
		NGL_LCD_PutPixel(Color);

		NGL_LCD_SetCursor(Xpos + CurY, Ypos - CurX);
		NGL_LCD_PutPixel(Color);

		NGL_LCD_SetCursor(Xpos - CurY, Ypos + CurX);
		NGL_LCD_PutPixel(Color);

		NGL_LCD_SetCursor(Xpos - CurY, Ypos - CurX);
		NGL_LCD_PutPixel(Color);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}


/**
  * @brief  NGL_GP_DrawBeveledRect
  * @param  X0, Y0, X1, Y1, Radius, Color
  * @retval None
  */
void NGL_GP_DrawBeveledRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Radius, uint16_t Color)
{
  int32_t  D;           /* Decision Variable */
  uint32_t  CurX;   /* Current X Value */
  uint32_t  CurY;   /* Current Y Value */

  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;

  /* --- Рисуем закругления  --- */
  while (CurX <= CurY)
  {
    NGL_LCD_SetCursor(X2 + CurX, Y1 + CurY);         // 1 Квадрант
    NGL_LCD_PutPixel(Color);
	NGL_LCD_SetCursor(X2 + CurY, Y1 + CurX);
    NGL_LCD_PutPixel(Color);

	NGL_LCD_SetCursor(X2 + CurX, Y2 - CurY);         // 2 Квадрант
    NGL_LCD_PutPixel(Color);
	NGL_LCD_SetCursor(X2 + CurY, Y2 - CurX);
    NGL_LCD_PutPixel(Color);

	NGL_LCD_SetCursor(X1 - CurX, Y2 - CurY);         // 3 Квадрант
    NGL_LCD_PutPixel(Color);
	NGL_LCD_SetCursor(X1 - CurY, Y2 - CurX);
    NGL_LCD_PutPixel(Color);

	NGL_LCD_SetCursor(X1 - CurX, Y1 + CurY);        // 4 Квадрант
    NGL_LCD_PutPixel(Color);
	NGL_LCD_SetCursor(X1 - CurY, Y1 + CurX);
    NGL_LCD_PutPixel(Color);


    if (D < 0) D += (CurX << 2) + 6;
    else { D += ((CurX - CurY) << 2) + 10;  CurY--; }
    CurX++;
  }

  /* --- Рисуем соединяющие линии --- */
  NGL_GP_DrawLine(X1, (Y1 + Radius), X2, (Y1 + Radius), Color);	// Верхняя соединяющая линия
  NGL_GP_DrawLine((X1 - Radius), Y2, (X1 - Radius), Y1, Color);	// Левая соединяющая линия
  NGL_GP_DrawLine((X2 + Radius), Y2, (X2 + Radius), Y1, Color);	// Правая соединяющая линия
  NGL_GP_DrawLine(X1, (Y2 - Radius), X2, (Y2 - Radius), Color);	// Нижняя соединяющая линия
}


/**
  * @brief  NGL_GP_DrawFillBeveledRect
  * @param  X0, Y0, X1, Y1, Radius, Color, border, borderColor
  * @retval None
  */
void NGL_GP_DrawFillBeveledRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Radius, uint16_t Color, uint8_t border, uint16_t borderColor)
{
	int32_t  D = 3 - (Radius << 1);     /* Decision Variable */
	uint16_t  CurX = 0;                 /* Current X Value */
	uint16_t  CurY = Radius;            /* Current Y Value */

	/* --- Рисуем закругления  --- */
	while (CurX <= CurY)
	{
		NGL_LCD_SetCursor(X2 + CurX, Y1 + CurY);         // 1 Квадрант
		NGL_LCD_PutPixel(Color);
		NGL_LCD_SetCursor(X2 + CurY, Y1 + CurX);
		NGL_LCD_PutPixel(Color);

		NGL_LCD_SetCursor(X2 + CurX, Y2 - CurY);         // 2 Квадрант
		NGL_LCD_PutPixel(Color);
		NGL_LCD_SetCursor(X2 + CurY, Y2 - CurX);
		NGL_LCD_PutPixel(Color);

		NGL_LCD_SetCursor(X1 - CurX, Y2 - CurY);         // 3 Квадрант
		NGL_LCD_PutPixel(Color);
		NGL_LCD_SetCursor(X1 - CurY, Y2 - CurX);
		NGL_LCD_PutPixel(Color);

		NGL_LCD_SetCursor(X1 - CurX, Y1 + CurY);         // 4 Квадрант
		NGL_LCD_PutPixel(Color);
		NGL_LCD_SetCursor(X1 - CurY, Y1 + CurX);
		NGL_LCD_PutPixel(Color);

		// 1,4 Квадрант
		NGL_GP_DrawLine(X2 + CurX, Y2 - CurY, X2 + CurX, (Y1 - 1) + (CurY * 2), Color);
		NGL_GP_DrawLine(X2 + CurY, Y2 - CurX , X2 + CurY, (Y1 - 1) + (CurX * 2), Color);

		// 2,3 Квадрант
		NGL_GP_DrawLine(X1 - CurX, Y2 - CurY, X1 - CurX, (Y1 - 1) + (CurY * 2), Color);
		NGL_GP_DrawLine(X1 - CurY, Y2 - CurX, X1 - CurX, (Y1 - 1) + (CurX * 2), Color);


		if (D < 0) D += (CurX << 2) + 6;
		else { D += ((CurX - CurY) << 2) + 10;  CurY--; }
		CurX++;
	}

    /* --- Рисуем соединяющие линии --- */
    if(border != 0)
    {
        NGL_GP_DrawLine(X1, (Y1 + Radius), X2, (Y1 + Radius), borderColor);	    // Верхняя соединяющая линия
        NGL_GP_DrawLine((X1 - Radius), Y2 + 1, (X1 - Radius), Y1, borderColor);	// Левая соединяющая линия
        NGL_GP_DrawLine((X2 + Radius), Y2 + 1, (X2 + Radius), Y1, borderColor);	// Правая соединяющая линия
        NGL_GP_DrawLine(X1, (Y2 - Radius), X2, (Y2 - Radius), borderColor);	    // Нижняя соединяющая линия
    }

	NGL_LCD_ClearArea(X1 + border, X2  - border, (Y2 - Radius) + border, (Y1 + Radius) - border, Color);

	NGL_LCD_SetFullScreen();
}





/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
