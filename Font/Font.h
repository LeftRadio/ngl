/**
  ******************************************************************************
  * @file		Font.h
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date		2015-02-12-10.50
  * @brief		header
  ******************************************************************************
**/

#ifndef __FONT_H
#define __FONT_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>                           /* Include standard types */

/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
void NGL_Font_SetFont(const NGL_Font *selectFont);
uint8_t NGL_Font_DrawChar(uint16_t x, uint16_t y, NGL_TransparentState trspr, char c);
uint16_t NGL_Font_DrawString(uint16_t XPos, uint16_t YPos, uint8_t trspr, char *str);
uint16_t NGL_Font_DrawColorString(uint16_t X, uint16_t Y, uint16_t Color, NGL_TransparentState trspr, char *str);
uint16_t NGL_Font_DrawCropString(uint16_t X, uint16_t Y, NGL_TransparentState trspr, char *str, uint8_t Pos, uint8_t Lenght, uint16_t Color);
uint16_t NGL_Font_MeasureStringWidth(char *str);
uint16_t NGL_Font_MeasureCropStringWidth(char *str, uint8_t NumSymbols);

void NGL_uintToString(uint32_t Num, char* Str, uint8_t NumSymbol);




#endif /* __FONT_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
