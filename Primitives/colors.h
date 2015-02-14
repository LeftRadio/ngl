/**
  ******************************************************************************
  * @file		colors.h
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date		2015-02-13-15.44
  * @brief		header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COLORS_H
#define __COLORS_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
//extern volatile uint16_t TextColor, BackColor, GraphicsColor;

/* Exported function ---------------------------------------------------------*/
void NGL_Color_SetTextColor(uint16_t color);
uint16_t NGL_Color_GetTextColor(void);
void NGL_Color_SetGraphicsColor(uint16_t color);
uint16_t NGL_Color_GetGraphicsColor(void);
void NGL_Color_SetBackColor(uint16_t color);
uint16_t NGL_Color_GetBackColor(void);

uint16_t NGL_Color_SetBrightness(uint16_t ColorIn, int8_t BrightLevel);


#endif /* __COLORS_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
