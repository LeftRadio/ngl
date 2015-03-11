/**
  ******************************************************************************
  * @file	 	NGL :: API
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.1.0
  * @date
  * @brief		header // fpv4-sp-d16
  ******************************************************************************
**/

#ifndef __NGL_H
#define __NGL_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "LCD_GPIO.h"
#include "NGL_types.h"

/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported constant ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/

///* --------------------------------  Hardware Aplication Level  -------------------------------- */
extern void LCD_Set_RESET_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void LCD_Set_RD_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void LCD_Set_RS_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void LCD_Set_WR_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void LCD_Set_CS_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void LCD_SetDataPort(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pins);

extern void NGL_LCD_DelayInit(void *pfunc);
extern int8_t NGL_LCD_ControllerInit(LCD_Typedef* LCD_init, void *pDelayFunc);
extern LCD_Typedef* NGL_LCD_GetController_FromIndex(uint8_t index);
extern void NGL_LCD_SetRotation(NGL_RotationLCD NewState);
extern void NGL_LCD_WriteRegister(uint16_t RegisterAddress, uint16_t Data);

///* ----------------------------------  Mid Aplication Level  ----------------------------------- */
extern void NGL_LCD_SetFullScreen(void);
extern void NGL_LCD_SetCursor(uint16_t X, uint16_t Y);

extern void NGL_LCD_ClearArea(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color);
extern void NGL_LCD_Clear_AllScreen(uint16_t Color);

extern void NGL_LCD_PutPixel(uint16_t Color);
extern void NGL_LCD_GetPixels(uint8_t *ReadData, uint16_t NumPixels);

///* -------------------------------------  Set/Get Colors  -------------------------------------- */
extern void NGL_Color_SetTextColor(uint16_t color);
extern uint16_t NGL_Color_GetTextColor(void);
extern void NGL_Color_SetGraphicsColor(uint16_t color);
extern uint16_t NGL_Color_GetGraphicsColor(void);
extern void NGL_Color_SetBackColor(uint16_t color);
extern uint16_t NGL_Color_GetBackColor(void);

extern uint16_t NGL_Color_SetBrightness(uint16_t ColorIn, int8_t BrightLevel);


///* -----------------------------------  Graphics primitive  ------------------------------------ */
extern void NGL_GP_SetAntialiasingLine_State(uint8_t NewState);

extern void NGL_GP_DrawPixel(uint16_t X, uint16_t Y, uint16_t Color);
extern void NGL_GP_DrawLine(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color);
extern void NGL_GP_DrawRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color);
extern void NGL_GP_DrawFillRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color, NGL_DrawState border, uint16_t borderColor);
extern void NGL_GP_DrawCircle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Color);
extern void NGL_GP_DrawBeveledRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Radius, uint16_t Color);
extern void NGL_GP_DrawFillBeveledRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Radius, uint16_t Color, uint8_t border, uint16_t borderColor);
//extern void NGL_GP_DrawTriangle(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);
//extern void NGL_GP_DrawFillTriangle(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);
//extern void NGL_GP_DrawSIN(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, float periods, uint16_t Color);

///* ---------------------------------  Fonts, draw char/string  --------------------------------- */
extern void NGL_Font_SetFont(const NGL_Font *selectFont);
extern uint8_t NGL_Font_DrawChar(uint16_t x, uint16_t y, NGL_TransparentState trspr, char c);
extern uint16_t NGL_Font_DrawString(uint16_t XPos, uint16_t YPos, uint8_t trspr, char *str);
extern uint16_t NGL_Font_DrawColorString(uint16_t X, uint16_t Y, uint16_t Color, NGL_TransparentState trspr, char *str);
extern uint16_t NGL_Font_DrawCropString(uint16_t X, uint16_t Y, NGL_TransparentState trspr, char *str, uint8_t Pos, uint8_t Lenght, uint16_t Color);
extern uint16_t NGL_Font_MeasureStringWidth(char *str);
extern uint16_t NGL_Font_MeasureCropStringWidth(char *str, uint8_t NumSymbols);

extern void NGL_uintToString(uint32_t Num, char* Str, uint8_t NumSymbol);

///* -----------------------------------------  Images  ----------------------------------------- */
extern void NGL_DrawImage(uint16_t X, uint16_t Y, const NGL_Image *Image);

///* -------------------------------------  GUI Functions  -------------------------------------- */
extern void NGL_DrawGrid(NGL_Grid *Grid, NGL_DrawState state);
extern void NGL_DrawButton(const NGL_Button *Button, NGL_ButtonState pushState);
extern void NGL_DrawLabel(const NGL_Label *Label);
extern void NGL_UpdateLabel(const NGL_Label *Label, char *NewText);
extern void NGL_DrawFillBar(NGL_FillBar* FillBar, uint16_t level);

///* ------------------------------  Object clip service function  ------------------------------ */
//extern ErrorStatus Set_New_ClipObject(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, NGL_ClipMode NewClipMode, uint8_t NumInd);
//extern ErrorStatus Clear_ClipObject(uint8_t NumInd);
//extern FlagStatus Verify_Clip_Point(uint16_t X0, uint16_t Y0);
//extern FlagStatus Verify_Clip_Line(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1);
//extern FlagStatus CS_ClipLine_Out_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1);








#endif /* __NGL_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
