/**
  *****************************************************************************
  * @file    button.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  *****************************************************************************
**/

/* Includes -----------------------------------------------------------------*/
#include "NGL_types.h"
#include "LCD_HAL.h"
#include "LCD_MAL.h"
#include "font.h"
#include "colors.h"
#include "primitives.h"
#include "bitmap.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private function  --------------------------------------------------------*/

/**
  * @brief  NGL_DrawSimpleButton
  * @param  *Button, PushState
  * @retval None
  */
void _drawSimpleButton(const NGL_Button* Button)
{
    uint16_t i, j;
    uint16_t Color;
    uint16_t smColor[160];
    uint16_t colorShiftPixels;
    uint8_t R, G, B, cmax;
    uint16_t tmpText_X = 0;

    uint16_t X0 = Button->X;
    uint16_t Y0 = Button->Y;
    uint16_t X1 = (X0 + Button->Width);
    uint16_t Y1 = (Y0 + Button->Height);

    /* If button is pressed */
    NGL_ButtonState PushState = ResetButton;
    if(PushState == SelectedButton) {
        Color = Button->SelectedColor;
    }
    else {
        Color = Button->Color;
    }

    /* Simple shadow */
    NGL_GP_DrawLine(X0 - 1, Y0 - 1, X0 - 1, Y1 - 1, NGL_Color_SetBrightness(Color, -2));
    NGL_GP_DrawLine(X0 - 1, Y0 - 1, X1 - 1, Y0 - 1, NGL_Color_SetBrightness(Color, -2));

    if(Button->Type == ColorFillButton)
    {
        /* r, g, b colors */
        R = (uint8_t)(Color >> 11);
        G = (uint8_t)((Color >> 5) & 0x3F);
        B = (uint8_t)(Color & 0x1F);

        cmax = 32 - nMAX( nMAX(R, G>>1), B );
        colorShiftPixels = (cmax + (Y1 - Y0)) / cmax;

        for(i = 0; i < (Y1 - Y0); i += colorShiftPixels)
        {
            for(j = i; j < i + colorShiftPixels; j++) {
                smColor[j] = (R << 11) | (G << 5) | B;
            }

            if((R < 0x1F) && (R > 0)) R += 1;
            if((G < 0x3F) && (G > 0)) G += 2;
            if((B < 0x1F) && (B > 0)) B += 1;
        }

        /* Fill button, low level for best performance */
        CS_LCD_set;
        for(i = Y0; i < Y1; i++)
        {
            LCD->SetCursor(X0, i);  // установить курсор вывода на новую позицию
            for(j = X0; j < X1; j++)
            {
                LCD->WriteData(smColor[i - Y0]);      // нарисовать точку
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

    tmpText_X = NGL_Font_MeasureStringWidth((NGL_Font*)Button->Font, Button->Text);
    X0 = Button->X + ((Button->Width - tmpText_X) / 2);
    Y0 = Button->Y + ((Button->Height - Button->Font->Height) / 2) + Button->TextOffset_Y;

    NGL_Font_DrawFastString(X0, Y0 - 2, Transparent, Button->Text);
}


/**
  * @brief  NGL_DrawButton
  * @param
  * @retval None
  */
void NGL_GUI_DrawButton(const NGL_Button* Button)
{
    uint16_t btnTextWidth;
    uint16_t txt_X;
    uint16_t ico_X, ico_Y;

    if(Button->Enabled != SET) return;

    if(Button->ICO != (void*)0)
    {
        ico_X = Button->X + ((Button->Width - Button->ICO->Width) / 2);
        ico_Y = (Button->Y + Button->Height) - Button->ICO->Height;

        NGL_DrawImage(ico_X, ico_Y, Button->ICO);

        if((Button->Font != (void*)0) && (Button->Text != 0))
        {
            NGL_Font_SetFont(Button->Font);
            NGL_Color_SetTextColor(Button->TextColor);

            btnTextWidth = NGL_Font_MeasureStringWidth((NGL_Font*)Button->Font, (char*)Button->Text);
            txt_X = Button->X + ((Button->Width - btnTextWidth) / 2);

            NGL_Font_DrawFastString(txt_X, Button->Y + Button->TextOffset_Y, Transparent, (char*)Button->Text);
        }
    }
    else
    {
        _drawSimpleButton(Button);
    }
}


/**
  * @brief  NGL_GUI_CheckBoxEvent
  * @param
  * @retval None
  */
void NGL_GUI_ButtonEvent(NGL_Button* button, NGL_TouchType eventType, int* evnentData) {

}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
