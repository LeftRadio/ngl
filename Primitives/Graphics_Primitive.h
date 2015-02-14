/**
  ******************************************************************************
  * @file    	Graphics_Primitive.h
  * @author  	LeftRadio
  * @version 	V1.2.0
  * @date
  * @brief		NGL :: Graphics primitive Layer header
  ******************************************************************************
**/

#ifndef __GRAPHIC_PRIMITIVE_H
#define __GRAPHIC_PRIMITIVE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported typedef -----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function --------------------------------------------------------*/
void NGL_GP_SetAntialiasingLine_State(uint8_t NewState);

void NGL_GP_DrawPixel(uint16_t X, uint16_t Y, uint16_t Color);
void NGL_GP_DrawLine(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color);
void NGL_GP_DrawRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color);
void NGL_GP_DrawFillRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color, NGL_DrawState border, uint16_t borderColor);
void NGL_GP_DrawCircle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Color);
void NGL_GP_DrawBeveledRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Radius, uint16_t Color);
void NGL_GP_DrawFillBeveledRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Radius, uint16_t Color, uint8_t border, uint16_t borderColor);
//void NGL_GP_DrawTriangle(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);
//void NGL_GP_DrawFillTriangle(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);
//void NGL_GP_DrawSIN(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, float periods, uint16_t Color);






#endif /* __GRAPHIC_PRIMITIVE_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
