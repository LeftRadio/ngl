/**
  ******************************************************************************
  * @file	 	UI_touch.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "NGL_types.h"
#include "LCD_HAL.h"
#include "LCD_MAL.h"
#include "Graphics_Primitive.h"


/* Private typedef -----------------------------------------------------------*/
typedef struct Matrix
{
long double An,
            Bn,
            Cn,
            Dn,
            En,
            Fn,
            Divider;
} Matrix;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile Matrix matrix;

/* Private function prototypes -----------------------------------------------*/
static void DrawCross(uint16_t Xpos, uint16_t Ypos, uint8_t Size, uint16_t Color);
void NGL_Touch_SetCalibrationMatrix(uint16_t* DisplaySamples_X, uint16_t* DisplaySamples_Y, uint16_t* ScreenSamples_X, uint16_t* ScreenSamples_Y);

/* Exported function ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
int8_t NGL_GUI_Touch_Calibrate(
		uint16_t* DisplaySamples_X,
		uint16_t* DisplaySamples_Y,
		NGL_GUI_Touch_TypeDef* NGL_GUI_TouchSruct)
{
	uint8_t S = LCD->X_Max / 32;
	uint8_t i = 0, verify_cnt = 5;
	uint16_t ScreenSample_X[3], ScreenSample_Y[3];;
	bool EndCalibration = FALSE;

	
	/* Calibration cycle */
	while(EndCalibration == FALSE)
	{
		/* Sampling 3 points touch data */
		for(i = 0; i < 3; i++)
		{
			DrawCross(DisplaySamples_X[i], DisplaySamples_Y[i], S, 0xFFFF);

			/* wait touch and after get coordinates */
			while(NGL_GUI_TouchSruct->TouchDataReady != TRUE);
			ScreenSample_X[i] = NGL_GUI_TouchSruct->X;
			ScreenSample_Y[i] = NGL_GUI_TouchSruct->Y;
			
			/* wait while touched */
			while(NGL_GUI_TouchSruct->TouchDataReady != FALSE);

			/* Clear cross */
			NGL_LCD_ClearArea(
					DisplaySamples_X[i] - S,
					DisplaySamples_Y[i] - S,
					DisplaySamples_X[i] + S,
					DisplaySamples_Y[i] + S,
					0x0000
					);
		}

		/* End of get 3 point touch data, set new calibration matrix */
		NGL_Touch_SetCalibrationMatrix(
				DisplaySamples_X, DisplaySamples_Y,
				ScreenSample_X, ScreenSample_Y );

		/* Verify, 5 count */
		while(verify_cnt)
		{
			DrawCross(LCD->X_Max / 2, LCD->Y_Max / 2, S, 0xFFFF);

			/* wait touch and after get coordinates */
			while(NGL_GUI_TouchSruct->TouchDataReady != TRUE);
			ScreenSample_X[0] = NGL_GUI_TouchSruct->X;
			ScreenSample_Y[0] = NGL_GUI_TouchSruct->Y;

			/* wait while touched */
			while(NGL_GUI_TouchSruct->TouchDataReady != FALSE);


			/* break if all ok, else increment couner and try again */
			if((ScreenSample_X[0] > (LCD->X_Max / 2) - 10) && (ScreenSample_X[0] < (LCD->X_Max / 2) + 10))
			{
				if((ScreenSample_Y[0] > (LCD->Y_Max / 2) - 10) && (ScreenSample_Y[0] < (LCD->Y_Max / 2) + 10))
				{
					break;
				}
			}
			else
			{
				/* Reset matrix and decrement verify counter */
				matrix.Divider = 0;
				verify_cnt--;
			}
		}

		/* Clear */
		NGL_LCD_Clear_AllScreen(0x0000);
		EndCalibration = TRUE;

		/* Write success flag and calibration data */
		if(verify_cnt != 0)
		{		
			return 0;
		}
	}

	return -1;
}


/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
void NGL_Touch_SetCalibrationMatrix(uint16_t* DisplaySamples_X, uint16_t* DisplaySamples_Y, uint16_t* ScreenSamples_X, uint16_t* ScreenSamples_Y )
 // void NGL_Touch_SetCalibrationMatrix(Coordinate *DisplaySamples_X, Coordinate *ScreenSamples_X)
{
	/* K£½(X0£­X2) (Y1£­Y2)£­(X1£­X2) (Y0£­Y2) */
	matrix.Divider = ((ScreenSamples_X[0] - ScreenSamples_X[2]) * (ScreenSamples_Y[1] - ScreenSamples_Y[2])) -
			((ScreenSamples_X[1] - ScreenSamples_X[2]) * (ScreenSamples_Y[0] - ScreenSamples_Y[2])) ;

	if( matrix.Divider != 0 )
	{
		/* A£½((XD0£­XD2) (Y1£­Y2)£­(XD1£­XD2) (Y0£­Y2))£¯K	*/
		matrix.An = ((DisplaySamples_X[0] - DisplaySamples_X[2]) * (ScreenSamples_Y[1] - ScreenSamples_Y[2])) -
				((DisplaySamples_X[1] - DisplaySamples_X[2]) * (ScreenSamples_Y[0] - ScreenSamples_Y[2])) ;
		/* B£½((X0£­X2) (XD1£­XD2)£­(XD0£­XD2) (X1£­X2))£¯K	*/
		matrix.Bn = ((ScreenSamples_X[0] - ScreenSamples_X[2]) * (DisplaySamples_X[1] - DisplaySamples_X[2])) -
				((DisplaySamples_X[0] - DisplaySamples_X[2]) * (ScreenSamples_X[1] - ScreenSamples_X[2])) ;
		/* C£½(Y0(X2XD1£­X1XD2)+Y1(X0XD2£­X2XD0)+Y2(X1XD0£­X0XD1))£¯K */
		matrix.Cn = (((ScreenSamples_X[2] * DisplaySamples_X[1]) - (ScreenSamples_X[1] * DisplaySamples_X[2])) * ScreenSamples_Y[0]) +
				(((ScreenSamples_X[0] * DisplaySamples_X[2]) - (ScreenSamples_X[2] * DisplaySamples_X[0])) * ScreenSamples_Y[1]) +
				(((ScreenSamples_X[1] * DisplaySamples_X[0]) - (ScreenSamples_X[0] * DisplaySamples_X[1])) * ScreenSamples_Y[2]) ;
		
		/* D£½((YD0£­YD2) (Y1£­Y2)£­(YD1£­YD2) (Y0£­Y2))£¯K	*/
		matrix.Dn = ((DisplaySamples_Y[0] - DisplaySamples_Y[2]) * (ScreenSamples_Y[1] - ScreenSamples_Y[2])) -
				((DisplaySamples_Y[1] - DisplaySamples_Y[2]) * (ScreenSamples_Y[0] - ScreenSamples_Y[2])) ;
		/* E£½((X0£­X2) (YD1£­YD2)£­(YD0£­YD2) (X1£­X2))£¯K	*/
		matrix.En = ((ScreenSamples_X[0] - ScreenSamples_X[2]) * (DisplaySamples_Y[1] - DisplaySamples_Y[2])) -
				((DisplaySamples_Y[0] - DisplaySamples_Y[2]) * (ScreenSamples_X[1] - ScreenSamples_X[2])) ;
		/* F£½(Y0(X2YD1£­X1YD2)+Y1(X0YD2£­X2YD0)+Y2(X1YD0£­X0YD1))£¯K */
		matrix.Fn = (((ScreenSamples_X[2] * DisplaySamples_Y[1]) - (ScreenSamples_X[1] * DisplaySamples_Y[2])) * ScreenSamples_Y[0]) +
				(((ScreenSamples_X[0] * DisplaySamples_Y[2]) - (ScreenSamples_X[2] * DisplaySamples_Y[0])) * ScreenSamples_Y[1]) +
				(((ScreenSamples_X[1] * DisplaySamples_Y[0]) - (ScreenSamples_X[0] * DisplaySamples_Y[1])) * ScreenSamples_Y[2]) ;
	}
}


/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
void NGL_Touch_ConvertToDisplayPoint(uint16_t* InSample_X, uint16_t* InSample_Y)
{
	uint16_t Sample_X, Sample_Y;
	
	if( matrix.Divider != 0 )
	{
		/* XD = AX+BY+C */
		Sample_X = ( (matrix.An * (*InSample_X)) + (matrix.Bn * (*InSample_Y)) + matrix.Cn ) / matrix.Divider ;

		/* YD = DX+EY+F */
		Sample_Y = ( (matrix.Dn * (*InSample_X)) + (matrix.En * (*InSample_Y)) + matrix.Fn ) / matrix.Divider ;

		*InSample_X = Sample_X;
		*InSample_Y = Sample_Y;
	}
}



/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static void DrawCross(uint16_t Xpos, uint16_t Ypos, uint8_t Size, uint16_t Color)
{
	uint8_t SizeDiv2 = Size >> 1;

	NGL_GP_DrawLine(Xpos - Size, Ypos, Xpos - 2, Ypos, Color);
	NGL_GP_DrawLine(Xpos + 2, Ypos, Xpos + Size, Ypos, Color);
	NGL_GP_DrawLine(Xpos, Ypos - Size, Xpos, Ypos - 2, Color);
	NGL_GP_DrawLine(Xpos, Ypos + 2, Xpos, Ypos + Size, Color);

	NGL_GP_DrawLine(Xpos - Size, Ypos + Size, Xpos - SizeDiv2, Ypos + Size, Color);
	NGL_GP_DrawLine(Xpos - Size, Ypos + SizeDiv2, Xpos - Size, Ypos + Size, Color);

	NGL_GP_DrawLine(Xpos - Size, Ypos - Size, Xpos - SizeDiv2, Ypos - Size, Color);
	NGL_GP_DrawLine(Xpos - Size, Ypos - Size, Xpos - Size, Ypos - SizeDiv2, Color);

	NGL_GP_DrawLine(Xpos + SizeDiv2, Ypos + Size, Xpos + Size, Ypos + Size, Color);
	NGL_GP_DrawLine(Xpos + Size, Ypos + SizeDiv2, Xpos + Size, Ypos + Size, Color);

	NGL_GP_DrawLine(Xpos + SizeDiv2, Ypos - Size, Xpos + Size, Ypos - Size, Color);
	NGL_GP_DrawLine(Xpos + Size, Ypos - Size, Xpos + Size, Ypos - SizeDiv2, Color);
}





/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
int8_t NGL_Touch_GetTouchButtonIndex(NGL_Page* Page, uint16_t X, uint16_t Y)
{
	uint8_t i;
	
	for(i = 0; i < Page->ButtonsCount; i++)
	{
		if((Page->Buttons[i]->X < X) && ((Page->Buttons[i]->X + Page->Buttons[i]->Width > X)))
		{
			if((Page->Buttons[i]->Y < Y) && ((Page->Buttons[i]->Y + Page->Buttons[i]->Height > Y)))
			{
				return i;
			}
		}
	}

	return -1;
}



/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
NGL_Button* NGL_Touch_GetTouchButton(NGL_Page* Page, uint16_t X, uint16_t Y)
{
	int8_t i;

	i = NGL_Touch_GetTouchButtonIndex(Page, X, Y);
	if(i != -1)
	{
		return (NGL_Button*)Page->Buttons[i];
	}

	return (void*)0;
}









/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
