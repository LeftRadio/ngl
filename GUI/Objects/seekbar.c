/**
  *****************************************************************************
  * @file    seekbar.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  *****************************************************************************
**/

/* Includes -----------------------------------------------------------------*/
#include <math.h>
#include "NGL_types.h"
#include "colors.h"
#include "Graphics_Primitive.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private function  --------------------------------------------------------*/


/**
  * @brief  _SeekBar_DrawInit
  * @param
  * @retval None
  */
__inline static void _SeekBar_DrawInit(NGL_SeekBar* bar)
{
    int yc = bar->Y0 + ((bar->Y1 - bar->Y0)/2);
    int xc = bar->X0 + ((bar->X1 - bar->X0)/2);
    uint16_t color = NGL_Color_SetBrightness(bar->Color, 8);
    if(bar->Orientation == NGL_Horizontal) {
        NGL_GP_DrawFillRect(bar->X0, yc - 2, bar->X1, yc + 2, color, DISABLE, 0);
        bar->old_posX = bar->X0;
        bar->old_posY = yc;
    }
    else {
        NGL_GP_DrawFillRect(xc - 2, bar->Y0, xc + 2, bar->Y1, color, DISABLE, 0);
        bar->old_posX = xc;
        bar->old_posY = bar->Y0;
    }
}

/**
  * @brief  _Draw_SeekBar_Slider
  * @param
  * @retval None
  */
__inline static void _Draw_SeekBar_Slider(x, y, width, height, color, mode)
{
    uint16_t i;
    uint16_t sz = nMIN(width, height);
    uint16_t tcolor;

    if(mode == CLEAR) {
    	NGL_GP_DrawFillRect(
    			x - (width >> 1),
    			y - (height >> 1),
    			x + (width >> 1),
    			y + (height >> 1),
    			color,
                0,
                0);
    }
    else {
    	// draw slider
    	for (i = 0; i < sz; i++) {
    		// p.setPen(self._slider_color.lighter(100 + (i * 10)))
    		tcolor = NGL_Color_SetBrightness(color, i);

    		NGL_GP_DrawRect(
    				x - (width - i) / 2,
    				y - (height - i) / 2,
    				x + (width - i) / 2,
    				y + (height - i) / 2,
    				tcolor);
    	}
    }
}

/**
  * @brief  NGL_DrawSeekBar
  * @param
  * @retval None
  */
__inline static void _DrawSeekBar(NGL_SeekBar* bar)
{
    uint16_t width = bar->X1 - bar->X0;
    uint16_t height = bar->Y1 - bar->Y0;

    uint16_t fullscale = bar->Level_MAX - bar->Level_MIN;
    uint16_t posX, posY;
    uint16_t sl_Width, sl_HalfWidth;
    uint16_t sl_Height, sl_HalfHeight;
    uint16_t x0, y0, x1, y1;

    uint16_t color;

    if (bar->Orientation == NGL_Horizontal) {

        posX = bar->X0 + ((bar->Level * width) / fullscale);
        posY = bar->Y0 + (height / 2);

        sl_Width = 8;
        sl_HalfWidth = sl_Width >> 1;
        sl_Height = height - 1;

        y0 = posY - 2;
        y1 = posY + 2;

        // calc and draw
        if (posX > bar->old_posX) {
            x0 = bar->old_posX - sl_HalfWidth;
            x1 = posX - sl_HalfWidth;

            color = NGL_Color_SetBrightness(bar->Color, 16);
        }
        else if (posX < bar->old_posX) {
            x0 = posX + sl_HalfWidth;
            x1 = bar->old_posX + sl_HalfWidth;

            color = bar->Color;
        }
        else {
            // no change, return
            return;
        }
    }
    else {
        posX = bar->X0 + (width / 2);
        posY = bar->Y0 + ((bar->Level * height) / fullscale);

        sl_Width = width - 1;
        sl_HalfWidth = sl_Width >> 1;
        sl_Height = 8;
        sl_HalfHeight = sl_Height >> 1;

        x0 = posX - 2;
        x1 = posX + 2;

        // calc and draw
        if (posY > bar->old_posY) {
            y0 = bar->old_posY - sl_HalfHeight;
            y1 = posY - sl_HalfHeight;

            color = NGL_Color_SetBrightness(bar->Color, 16);
        }
        else if (posY < bar->old_posY) {
            y0 = posY + sl_HalfHeight;
            y1 = bar->old_posY + sl_HalfHeight;

            color = bar->Color;
        }
        else {
            // no change, return
            return;
        }
    }

    // clear slider for old position
    _Draw_SeekBar_Slider(bar->old_posX, bar->old_posY, sl_Width, sl_Height, NGL_Color_GetBackColor(), CLEAR);

    // update draw strip
    NGL_GP_DrawFillRect(x0, y0, x1, y1, color, DISABLE, 0);

    // draw slider for new position
    _Draw_SeekBar_Slider(posX, posY, sl_Width, sl_Height, bar->SliderColor, DRAW);

    // store new values
    bar->old_posX = posX;
    bar->old_posY = posY;
}

/**
  * @brief  NGL_DrawSeekBar
  * @param
  * @retval None
  */
void NGL_GUI_DrawSeekBar(const NGL_SeekBar* seekBar)
{
    NGL_SeekBar* bar = (NGL_SeekBar*)seekBar;

    // if init state
    if(bar->old_posX == 0xFFFF && bar->old_posY == 0xFFFF) {
        _SeekBar_DrawInit(bar);
    }

    // clip seekbar level value
    bar->Level = nMAX(nMIN(bar->Level, bar->Level_MAX), bar->Level_MIN);

    _DrawSeekBar(bar);
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
