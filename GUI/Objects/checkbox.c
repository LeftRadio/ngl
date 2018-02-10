/**
  *****************************************************************************
  * @file    checkbox.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  *****************************************************************************
**/

/* Includes -----------------------------------------------------------------*/
#include "NGL_types.h"
#include "font.h"
#include "colors.h"
#include "primitives.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private function  --------------------------------------------------------*/

/**
  * @brief  _drawBox
  * @param
  * @retval None
  */
__inline static void _drawBox(NGL_CheckBox* checkbox)
{
    uint8_t i;
    uint16_t x0 = checkbox->X0;
    uint16_t y0 = checkbox->Y0;
    uint16_t delta = checkbox->Y1 - checkbox->Y0;
    uint16_t width = delta + 1;
    uint16_t height = delta + 1;
    uint16_t sz;

    uint16_t box_color = checkbox->Color;
    uint16_t rect_color;
    uint16_t cnt = height >> 3;

    for(i = 0; i < cnt; i++) {
       rect_color = NGL_Color_SetBrightness( box_color, (16 / (i+1)) );
       sz = delta - (i*2);
       NGL_GP_DrawRect( x0+i, y0+i, x0+i+sz, y0+i+sz, rect_color );
    }

    rect_color = NGL_Color_SetBrightness(box_color, 24);
    sz = (delta - (cnt*2));
    NGL_GP_DrawFillRect( x0+cnt, y0+cnt, x0+cnt+sz, y0+cnt+sz, 0xFFFF, ENABLE, rect_color );

    // checked state
    if (checkbox->Checked == ENABLE) {

        uint16_t _color = 0x0000;    // black color

        NGL_GP_DrawLine(
        		x0 + 2,
        		y0 + (height / 2),
        		x0 + (height / 2),
        		y0 + 2,
        		_color);

        NGL_GP_DrawLine(
        		x0 + (width / 2),
        		y0 + 2,
        		x0 + (width - 2),
        		y0 + (height - 2),
        		_color);
    }
}

/**
  * @brief  _drawText
  * @param
  * @retval None
  */
__inline static void _drawText(const NGL_CheckBox* checkbox)
{
    uint16_t box_size = (checkbox->Y1 - checkbox->Y0) + 1;

    NGL_Font_DrawString(
        checkbox->X0 + box_size + 4,
        checkbox->Y0 + ((box_size - checkbox->Font->Height) / 2),
        checkbox->TextColor,
        checkbox->Font,
        Transparent,
        (char*)checkbox->Text);
}

/**
  * @brief  NGL_DrawCheckBox
  * @param
  * @retval None
  */
void NGL_GUI_DrawCheckBox(const NGL_CheckBox* checkbox)
{
    NGL_CheckBox* cbox = (NGL_CheckBox*)checkbox;

    _drawBox(cbox);
    _drawText(cbox);
}

/**
  * @brief  NGL_GUI_CheckBoxEvent
  * @param
  * @retval None
  */
void NGL_GUI_CheckBoxEvent(NGL_CheckBox* box, NGL_TouchType EventType, int* EvnentData) {

  if (box->Checked == ENABLE) box->Checked = DISABLE;
  else box->Checked = ENABLE;

  NGL_GUI_DrawCheckBox(box);
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
