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
extern void NGL_Font_SetFont(const NGL_Font *selectFont);
extern uint8_t NGL_Font_DrawChar(uint16_t x, uint16_t y, NGL_TransparentState trspr, char c);
extern uint16_t NGL_Font_DrawString(uint16_t X, uint16_t Y, uint16_t Color, const NGL_Font *font, NGL_TransparentState trspr, char *str);
extern uint16_t NGL_Font_DrawFastString(uint16_t XPos, uint16_t YPos, NGL_TransparentState trspr, char *str);
extern uint16_t NGL_Font_DrawColorString(uint16_t X, uint16_t Y, uint16_t Color, NGL_TransparentState trspr, char *str);
extern uint16_t NGL_Font_DrawCropString(uint16_t X, uint16_t Y, NGL_TransparentState trspr, char *str, uint8_t Pos, uint8_t Lenght, uint16_t Color);
extern uint16_t NGL_Font_MeasureStringWidth(const NGL_Font *font, char *str);
extern uint16_t NGL_Font_MeasureCropStringWidth(char *str, uint8_t NumSymbols);

extern void NGL_intToString(int32_t val, char* Str);
extern void NGL_uintToString(uint32_t val, char* Str);
extern void NGL_uintToFixedString(uint32_t val, char* Str, uint8_t NumSymbol);
extern int NGL_StrtToInt(char* str);


#endif /* __FONT_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
