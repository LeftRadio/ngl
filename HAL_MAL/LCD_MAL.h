/**
  ******************************************************************************
  * @file		LCD_MAL.h
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date		2015-02-13-16.24
  * @brief		header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAL_H
#define __MAL_H

/* Includes ------------------------------------------------------------------*/
#include "NGL_types.h"

/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
void NGL_LCD_SetFullScreen(void);
void NGL_LCD_SetCursor(uint16_t X, uint16_t Y);

void NGL_LCD_ClearArea(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color);
void NGL_LCD_Clear_AllScreen(uint16_t Color);

void NGL_LCD_PutPixel(uint16_t Color);
void NGL_LCD_GetPixels(uint8_t *ReadData, uint16_t NumPixels);

void NGL_LCD_SetRotation(NGL_RotationLCD NewState);
void NGL_LCD_WriteRegister(uint16_t RegisterAddress, uint16_t Data);


#endif /* ___H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
