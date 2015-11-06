/**
  *****************************************************************************
  * @file    label.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  *****************************************************************************
**/

/* Includes -----------------------------------------------------------------*/
#include <string.h>
#include "NGL_types.h"
#include "page.h"
#include "font.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private function  --------------------------------------------------------*/

/**
  * @brief  NGL_DrawLabel
  * @param
  * @retval None
  */
void NGL_GUI_DrawLabel(const NGL_Label *Label)
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
void NGL_GUI_UpdateLabel(const NGL_Label *Label, char *NewText)
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
            NGL_Font_DrawCropString(
              Label->X,
              Label->Y,
              Label->Transparent,
              Label->Text,
              i,
              TextLenght - i,
              NGL_GUI_SelectedPage->BackColor);

            /* Draw from i to strlen(NewText) - i */
            NGL_Font_DrawCropString(
              Label->X,
              Label->Y,
              Label->Transparent,
              NewText,
              i,
              NewTextLenght - i,
              Label->Color);

            /* Copy changed text and exit */
            do {
                Label->Text[i] = NewText[i];
                i++;
            } while(i < NewTextLenght);

            return;
        }
    }
}