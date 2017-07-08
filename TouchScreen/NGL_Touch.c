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
#include "NGL_Types.h"
#include "LCD_HAL.h"
#include "LCD_MAL.h"
#include "Graphics_Primitive.h"

#include "NGL_Touch.h"
#include "NGL_Touch_Events.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static int8_t _touchConfiguration(void);
static int8_t _touchCalibrate(void);
static int8_t _touchPoint(Coordinate sc, Coordinate* ds);
static int8_t _touchReset(void);

static int8_t __setCalibrationMatrix(Coordinate *screenPtr);
static void __drawCross(uint16_t Xpos, uint16_t Ypos, uint8_t Size, uint16_t Color);

/* Private variables ---------------------------------------------------------*/
NGL_TouchScreen* _touch = (void*)0;
/* */
const NGL_TouchScreenFoops _touchFoops = {
	_touchConfiguration,
	_touchCalibrate,
	_touchPoint,
	NGL_Touch_Event,
	NGL_Touch_DoTouch,
	_touchReset
};

/* Exported function ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  __setCalibrationMatrix
 * @param  None
 * @retval None
 */
static int8_t __setCalibrationMatrix(Coordinate *ptr) {

	const Coordinate *displayPtr = _touch->calibrateSamples;
	Matrix_Typedef *matrix = &_touch->Matrix;

	matrix->Divider = ((ptr[0].x - ptr[2].x) * (ptr[1].y - ptr[2].y)) -
			((ptr[1].x - ptr[2].x) * (ptr[0].y - ptr[2].y)) ;

	if( _touch == (void*)0 || matrix->Divider == 0 ) {
		return -1;
	}

	matrix->An = ((displayPtr[0].x - displayPtr[2].x) * (ptr[1].y - ptr[2].y)) -
			((displayPtr[1].x - displayPtr[2].x) * (ptr[0].y - ptr[2].y)) ;

	matrix->Bn = ((ptr[0].x - ptr[2].x) * (displayPtr[1].x - displayPtr[2].x)) -
			((displayPtr[0].x - displayPtr[2].x) * (ptr[1].x - ptr[2].x)) ;

	matrix->Cn = (((ptr[2].x * displayPtr[1].x) - (ptr[1].x * displayPtr[2].x)) * ptr[0].y) +
			(((ptr[0].x * displayPtr[2].x) - (ptr[2].x * displayPtr[0].x)) * ptr[1].y) +
			(((ptr[1].x * displayPtr[0].x) - (ptr[0].x * displayPtr[1].x)) * ptr[2].y) ;

	matrix->Dn = ((displayPtr[0].y - displayPtr[2].y) * (ptr[1].y - ptr[2].y)) -
			((displayPtr[1].y - displayPtr[2].y) * (ptr[0].y - ptr[2].y)) ;

	matrix->En = ((ptr[0].x - ptr[2].x) * (displayPtr[1].y - displayPtr[2].y)) -
			((displayPtr[0].y - displayPtr[2].y) * (ptr[1].x - ptr[2].x)) ;

	matrix->Fn = (((ptr[2].x * displayPtr[1].y) - (ptr[1].x * displayPtr[2].y)) * ptr[0].y) +
			(((ptr[0].x * displayPtr[2].y) - (ptr[2].x * displayPtr[0].y)) * ptr[1].y) +
			(((ptr[1].x * displayPtr[0].y) - (ptr[0].x * displayPtr[1].y)) * ptr[2].y) ;

	return 0;
}

/**
 * @brief  __drawCross
 * @param  None
 * @retval None
 */
static void __drawCross(uint16_t Xpos, uint16_t Ypos, uint8_t size, uint16_t Color) {
	uint8_t SizeDiv2 = size >> 1;

	NGL_GP_DrawLine(Xpos - size, Ypos, Xpos - 2, Ypos, Color);
	NGL_GP_DrawLine(Xpos + 2, Ypos, Xpos + size, Ypos, Color);
	NGL_GP_DrawLine(Xpos, Ypos - size, Xpos, Ypos - 2, Color);
	NGL_GP_DrawLine(Xpos, Ypos + 2, Xpos, Ypos + size, Color);

	NGL_GP_DrawLine(Xpos - size, Ypos + size, Xpos - SizeDiv2, Ypos + size, Color);
	NGL_GP_DrawLine(Xpos - size, Ypos + SizeDiv2, Xpos - size, Ypos + size, Color);

	NGL_GP_DrawLine(Xpos - size, Ypos - size, Xpos - SizeDiv2, Ypos - size, Color);
	NGL_GP_DrawLine(Xpos - size, Ypos - size, Xpos - size, Ypos - SizeDiv2, Color);

	NGL_GP_DrawLine(Xpos + SizeDiv2, Ypos + size, Xpos + size, Ypos + size, Color);
	NGL_GP_DrawLine(Xpos + size, Ypos + SizeDiv2, Xpos + size, Ypos + size, Color);

	NGL_GP_DrawLine(Xpos + SizeDiv2, Ypos - size, Xpos + size, Ypos - size, Color);
	NGL_GP_DrawLine(Xpos + size, Ypos - size, Xpos + size, Ypos - SizeDiv2, Color);
}

/**
 * @brief  NGL_Touch_InitFoops
 * @param  None
 * @retval None
 */
void NGL_Touch_InitFoops(NGL_TouchScreen* touch) {
	_touch = touch;
	_touch->foops = &_touchFoops;
}

/**
 * @brief  NGL_Touch_getTouch
 * @param  None
 * @retval None
 */
NGL_TouchScreen* NGL_Touch_getTouch(void) {
	return _touch;
}

/**
 * @brief  _touchConfiguration
 * @param  None
 * @retval None
 */
static int8_t _touchConfiguration(void) {
	/* */
	int8_t status = -1;
	Coordinate samples[3];
	/* Init HAL level, GPIO, EXTI, ADC, SPI etc. */
	if ((_touch != (void*)0) && (_touch->callbacks->HAL_Init() == 0)) {

		if (_touch->callbacks->HAL_SetState(Touch_Default) == 0) {
			/* read calibrate samples */
			if (_touch->callbacks->HAL_ReadSamples(samples, 3) == 0) {
				/* touch is calibrated, set matrix */
				status = __setCalibrationMatrix(samples);
			}
			else {
				/* touchpanel is not calibrated */
				status = _touchCalibrate();
			}
		}
	}
	return status;
}

/**
 * @brief  _touchCalibrate
 * @param  None
 * @retval None
 */
static int8_t _touchCalibrate(void) {
	uint8_t size = LCD->X_Max / 32;
	uint8_t i = 0, verify_cnt = 5;
	Coordinate samples[4];
	Coordinate verifyPoint = { LCD->X_Max / 2, LCD->Y_Max / 2 };
	bool EndCalibration = FALSE;

	if(_touch != (void*)0) {

		/* reset touch matrix divider */
		_touch->Matrix.Divider = 0;

		/* Calibration cycle */
		while(!EndCalibration) {

			/* Sampling 3 points */
			for(i = 0; i < 3; i++)
			{
				__drawCross(
					_touch->calibrateSamples[i].x,
					_touch->calibrateSamples[i].y,
					size,
					0xFFFF );

				/* wait touch and after get coordinates */
				while( NGL_Touch_GetEventStatus() != 1 );
				NGL_Touch_ResetEventStatus();

				/* save data */
	    		samples[i].x = _touch->data.x;
	    		samples[i].y = _touch->data.y;

				/* wait while touched */
				_touch->callbacks->HAL_SetState(Touch_Polling);
	    		while ( _touch->callbacks->HAL_NoTouch() );
	    		_touch->callbacks->HAL_SetState(Touch_Default);

				/* Clear cross */
				NGL_LCD_ClearArea(
						_touch->calibrateSamples[i].x - size,
						_touch->calibrateSamples[i].y - size,
						_touch->calibrateSamples[i].x + size,
						_touch->calibrateSamples[i].y + size,
						0x0000 );
			}

			/* End of get 3 point touch data, set new calibration matrix */
			__setCalibrationMatrix(samples);

			/* Verify, 5 count */
			while(verify_cnt)
			{
				__drawCross(
					verifyPoint.x,
					verifyPoint.y,
					size,
					0xFFFF);

				/* wait touch and after get coordinates */
				while( NGL_Touch_GetEventStatus() != 1 );
				NGL_Touch_ResetEventStatus();

				_touch->foops->point(_touch->data, &samples[3]);

				/* wait while touched */
				_touch->callbacks->HAL_SetState(Touch_Polling);
	    		while ( _touch->callbacks->HAL_NoTouch() );
	    		_touch->callbacks->HAL_SetState(Touch_Default);

	    		// break if all ok, else increment couner and try again
				if( (samples[3].x > verifyPoint.x - 10) && (samples[3].x < verifyPoint.x + 10) && \
					(samples[3].y > verifyPoint.y - 10) && (samples[3].y < verifyPoint.y + 10) ) {
					break;
				}
				else {
					verify_cnt--;
				}
			}

			/* Clear */
			NGL_LCD_Clear_AllScreen(0x0000);
			EndCalibration = TRUE;

			/* Write success flag and calibration data */
			if(verify_cnt != 0)	{
				_touch->callbacks->HAL_SaveSamples(samples, 3);
				return 0;
			}
		}
	}

	return -1;
}

/**
 * @brief  _touchPoint
 * @param  None
 * @retval None
 */
static int8_t _touchPoint(Coordinate sc, Coordinate* ds) {
	/* */
	if(_touch == (void*)0) {
		return -1;
	}
	/* */
	Matrix_Typedef m = _touch->Matrix;
	/* */
	if( m.Divider != 0 ) {
		/* XD = AX+BY+C */
		ds->x = ( (m.An * sc.x) + (m.Bn * sc.y) + m.Cn ) / m.Divider ;
		/* YD = DX+EY+F */
		ds->y = ( (m.Dn * sc.x) + (m.En * sc.y) + m.Fn ) / m.Divider ;
	}
	else {
		ds->x = sc.x;
		ds->y = sc.y;
	}
	return 0;
}

/**
 * @brief  _touchReset
 * @param  None
 * @retval None
 */
static int8_t _touchReset(void) {
	return 0;
}


/*******************************************************************************
      END FILE
*******************************************************************************/