/**
  ******************************************************************************
  * @file    GUI_elements.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "NGL_types.h"
#include "LCD_HAL.h"
#include "LCD_MAL.h"
#include "Graphics_Primitive.h"
#include "colors.h"
#include "Font.h"
#include "Picture.h"
//#include "Clip.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NGL_Page *SelectedPage;
uint8_t Button_ShiftPixelNum = 2;


/* Private function prototypes -----------------------------------------------*/
/* Private function  --------------------------------------------------------*/

/**
  * @brief  NGL_SetActivePage
  * @param
  * @retval None
  */
void NGL_SetActivePage(const NGL_Page *NewPage)
{
    SelectedPage = (NGL_Page*)NewPage;
}


/**
  * @brief  NGL_DrawSimpleButton
  * @param  *Button, PushState
  * @retval None
  */
void NGL_DrawSimpleButton(const NGL_Button *Button, NGL_ButtonState PushState)
{
    uint16_t i, j;
    uint16_t Color;
    uint16_t smColor[160] = {0};
    uint8_t R, G, B;
    static uint16_t tmpText_X = 0;

    uint16_t X0 = Button->X;
    uint16_t Y0 = Button->Y;
    uint16_t X1 = (X0 + Button->Width);
    uint16_t Y1 = (Y0 + Button->Height);

    /* If button is pressed */
    if(PushState == SelectedButton)
    {
        Color = Button->SelectedColor;
    }
    else
    {
        Color = Button->Color;
    }

    /* Simple shadow */
    NGL_GP_DrawLine(X0 - 1, Y0 - 1, X0 - 1, Y1 - 1, NGL_Color_SetBrightness(Color, -2));
    NGL_GP_DrawLine(X0 - 1, Y0 - 1, X1 - 1, Y0 - 1, NGL_Color_SetBrightness(Color, -2));

    if(Button->Type == ColorFillButton)
    {
        /* раскладываем цвета перехода в массив */
        R = (uint8_t)(Color >> 11);
        G = (uint8_t)((Color >> 5) & 0x3F);
        B = (uint8_t)(Color & 0x1F);

        for(i = 0; i < (Y1 - Y0); i += Button_ShiftPixelNum)
        {
            for(j = i; j < i + Button_ShiftPixelNum; j++) smColor[j] = (R << 11) + (G << 5) + B;
            if((R < 0x1F) && (R > 0))R += 1;
            if((G < 0x3F) && (G > 0))G += 2;
            if((B < 0x1F) && (B > 0))B += 1;
        }

        /* Fill button, low level for best performance */
        CS_LCD_set;
        for(i = Y0; i < Y1; i++)
        {
            LCD->SetCursor(X0, i);  // установить курсор вывода на новую позицию
            for(j = X0; j < X1; j++)
            {
                LCD->WriteData(smColor[i - Y0]);			// нарисовать точку
            }
        }
        CS_LCD_clr;
        NGL_LCD_SetFullScreen();
    }
    else
    {
        NGL_LCD_ClearArea(X0, Y0, X1, Y1, Button->Color);
    }

    /* Set font, color and draw text */
    NGL_Font_SetFont(Button->Font);
    NGL_Color_SetTextColor(Button->TextColor);

    tmpText_X = NGL_Font_MeasureStringWidth(Button->Text);
    X0 = Button->X + ((Button->Width - tmpText_X) / 2);
    Y0 = Button->Y + ((Button->Height - Button->Font->Height) / 2) + Button->TextOffset_Y;

    NGL_Font_DrawString(X0, Y0 - 2, NotTransparent, Button->Text);
}


/**
  * @brief  NGL_DrawButton
  * @param
  * @retval None
  */
void NGL_DrawButton(const NGL_Button *Button, NGL_ButtonState pushState)
{
    uint16_t tmpX_ICO = 0;
    uint16_t Text_X0 = 0;

    if(Button->Enabled != SET) return;

    if(Button->ICO != 0)
    {
        NGL_DrawImage(Button->X, Button->Y, Button->ICO);

        if((Button->Font != (void*)0) && (Button->Text != 0))
        {
            NGL_Font_SetFont(Button->Font);
            NGL_Color_SetTextColor(Button->TextColor);

            tmpX_ICO = NGL_Font_MeasureStringWidth((char*)Button->Text);
            Text_X0 = Button->X + ((Button->Width - tmpX_ICO) / 2);

            NGL_Font_DrawString(Text_X0, Button->Y + Button->TextOffset_Y, NotTransparent, (char*)Button->Text);
        }
    }
    else
    {
        NGL_DrawSimpleButton(Button, pushState);
    }
}


/**
  * @brief  NGL_DrawLabel
  * @param
  * @retval None
  */
void NGL_DrawLabel(const NGL_Label *Label)
{
    NGL_Font_SetFont(Label->Font);
    NGL_Font_DrawColorString(Label->X, Label->Y, Label->Color, Label->Transparent, Label->Text);
}


/**
  * @brief  NGL_UpdateLabel
            Update Label text, starts on the first left changed char for minimum LCD flicr.
			NewText is must contain +1 char for end of string byte
  * @param  const NGL_Label *Label  - label pointer
  *         char *NewText           - new label text pointer
  * @retval None
  */
void NGL_UpdateLabel(const NGL_Label *Label, char *NewText)
{
    uint8_t i;
    uint8_t TextLenght = strlen(Label->Text);
    uint8_t NewTextLenght = strlen(NewText);

    /* Set font */
    NGL_Font_SetFont(Label->Font);

    /* Update changed chars */
    for(i = 0; i < NewTextLenght; i++)
    {
        if(Label->Text[i] != NewText[i])
        {
            /* Clear from i to strlen(Label->Text) - i */
            NGL_Font_DrawCropString(Label->X, Label->Y, Label->Transparent, Label->Text, i, TextLenght - i, SelectedPage->BackColor);

            /* Draw from i to strlen(NewText) - i */
            NGL_Font_DrawCropString(Label->X, Label->Y, Label->Transparent, NewText, i, NewTextLenght - i, Label->Color);

            /* Copy changed text and exit */
            do
            {
                Label->Text[i] = NewText[i];
                i++;
            }
            while(i < NewTextLenght);

            return;
        }
    }
}


/**
  * @brief  LCD_DrawGrid
  * @param  NGL_Grid *Grid, NGL_DrawState state
  * @retval None
  */
void NGL_DrawGrid(NGL_Grid *Grid, NGL_DrawState state)
{
    uint16_t i, j, k, l;
    uint16_t tmpColor = (state == CLEAR)?  NGL_Color_GetBackColor() : Grid->Color;
    uint16_t X0, Y0, X1, Y1;

    uint16_t left  = Grid->CenterX - (Grid->Width / 2);
    uint16_t right = Grid->CenterX + (Grid->Width / 2);
    uint16_t lower = Grid->CenterY - (Grid->Height / 2);
    uint16_t upper = Grid->CenterY + (Grid->Height / 2);

    NGL_Color_SetGraphicsColor(tmpColor);

    /* if need draw central lines */
    if(Grid->EnableCentralLines == ENABLE)
    {
        X0 = left;
        Y0 = Grid->CenterY;
        X1 = right;
        Y1 = Grid->CenterY;
//		if(Verify_Clip_Line(&X0, &Y0, &X1, &Y1) != SET)
        NGL_GP_DrawLine(X0, Y0, X1, Y1, tmpColor);

        X0 = Grid->CenterX;
        Y0 = lower;
        X1 = Grid->CenterX;
        Y1 = upper;
//		if(Verify_Clip_Line(&X0, &Y0, &X1, &Y1) != SET)
        NGL_GP_DrawLine(X0, Y0, X1, Y1, tmpColor);
    }

    /* Draw 1 & 4 quadrant */
    // отрисовываем вертикальные линии из точек от центра вправо
    for(i = Grid->CenterX; i < right - 10; i = i + 25)
    {
        l = 0;

        for(j = Grid->CenterY; j <= upper; j = j + 5)
        {
            // отрисовываем горизонтальные линии из точек от центра вверх и вниз
            if(l++ == 5)
            {
                for(k = i; k < i + 25; k = k + 5)
                {
//					if(Verify_Clip_Point(k, j) != SET)
                    NGL_GP_DrawPixel(k, j, tmpColor);
//					if(Verify_Clip_Point(k, (Grid->CenterY * 2) - j) != SET)
                    NGL_GP_DrawPixel(k, (Grid->CenterY * 2) - j, tmpColor);
                }
                l = 1;
            }
//			if(Verify_Clip_Point(i, j) != SET)
            NGL_GP_DrawPixel(i, j, tmpColor);
//			if(Verify_Clip_Point(i, (Grid->CenterY * 2) - j) != SET)
            NGL_GP_DrawPixel(i, (Grid->CenterY * 2) - j, tmpColor);
        }
    }

    /* Draw 2 & 3 quadrant */
    // отрисовываем вертикальные линии из точек от центра влево
    for(i = Grid->CenterX; i > left + 10; i = i - 25)
    {
        l = 0;

        for(j = Grid->CenterY; j <= upper; j = j + 5)
        {
            // отрисовываем горизонтальные линии из точек от центра вверх и вниз
            if(l++ == 5)
            {
                for(k = i; k > i - 25; k = k - 5)
                {
//					if(Verify_Clip_Point(k, j) != SET)
                    NGL_GP_DrawPixel(k, j, tmpColor);
//					if(Verify_Clip_Point(k, (Grid->CenterY * 2) - j) != SET)
                    NGL_GP_DrawPixel(k, (Grid->CenterY * 2) - j, tmpColor);
                }
                l = 1;
            }
//			if(Verify_Clip_Point(i, j) != SET)
            NGL_GP_DrawPixel(i, j, tmpColor);
//			if(Verify_Clip_Point(i, (Grid->CenterY * 2) - j) != SET)
            NGL_GP_DrawPixel(i, (Grid->CenterY * 2) - j, tmpColor);
        }
    }
}


/**
  * @brief  NGL_DrawFillBar
  * @param
  * @retval None
  */
void NGL_DrawFillBar(NGL_FillBar* FillBar, uint16_t Level)
{
    uint16_t x0, x1, y0, y1, Color;

    if(Level > FillBar->Level_MAX) Level = FillBar->Level_MAX;
    if(Level < FillBar->Level_MIN) Level = FillBar->Level_MIN;

    /* Draw */
    if(Level > FillBar->Level)
    {
        if(FillBar->VertHoriz == NGL_Horizontal)
        {
            x0 = FillBar->X0 + FillBar->Level;
            x1 = FillBar->X0 + Level;
            y0 = FillBar->Y0;
            y1 = FillBar->Y1;
        }
        else
        {
            x0 = FillBar->X0;
            x1 = FillBar->Y1;
            y0 = FillBar->Y0 + FillBar->Level;
            y1 = FillBar->Y0 + Level;
        }
        Color = FillBar->Color;
    }
    else if(Level < FillBar->Level)
    {
        if(FillBar->VertHoriz == NGL_Horizontal)
        {
            x0 = FillBar->X0 + Level;
            x1 = FillBar->X0 + FillBar->Level;
            y0 = FillBar->Y0;
            y1 = FillBar->Y1;
        }
        else
        {
            x0 = FillBar->X0;
            x1 = FillBar->Y1;
            y0 = FillBar->Y0 + Level;
            y1 = FillBar->Y0 + FillBar->Level;
        }
        Color = NGL_Color_GetBackColor();
    }
    else return;

    NGL_LCD_ClearArea(x0, y0, x1, y1, Color);
    FillBar->Level = Level;
}






/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
