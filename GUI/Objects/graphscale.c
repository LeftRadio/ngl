/**
  *****************************************************************************
  * @file    graphscale.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  *****************************************************************************
**/

/* Includes -----------------------------------------------------------------*/
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "NGL_types.h"
#include "colors.h"
#include "font.h"
#include "Graphics_Primitive.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private function  --------------------------------------------------------*/

/**
  * @brief  drawHorizontale, paint horizontal graphscale
  * @param
  * @retval None
  */
__inline static uint16_t scaleLenght(const NGL_GraphScale* gScale)
{
    uint16_t width = gScale->X1 - gScale->X0;
    uint16_t height = gScale->Y1 - gScale->Y0;
    uint16_t lenght = 0;

    if (gScale->Orientation == NGL_Horizontal) {
        lenght = width - NGL_Font_MeasureStringWidth(gScale->Font, gScale->Units);
    }
    else {
        lenght = height - gScale->Font->Height;
    }

    return lenght;
}

/**
  * @brief  calcPos, return coordinate respect scale level
  * @param
  * @retval None
  */
__inline static uint16_t calcPos(const NGL_GraphScale* gScale, int16_t level)
{
    int16_t max_level = (int16_t)scaleLenght(gScale);
    uint16_t pos = (max_level * abs(level)) / abs(gScale->MAX - gScale->MIN);

    if (gScale->MIN < 0) {
        pos = max_level - pos;
    }

    if (pos > max_level) {
        pos = max_level;
    }
    else if (pos < 0) {
        pos = 0;
    }

    return gScale->X0 + pos;
}

/**
  * @brief  drawHorizontale, paint horizontal graphscale
  * @param
  * @retval None
  */
static void drawHorizontal(const NGL_GraphScale* gScale)
{
    int32_t i;
    uint32_t scale_X1 = gScale->X0 + scaleLenght(gScale);
    uint32_t uscalecent = (uint32_t)abs(gScale->Cent);
    int lblpos[20] = {INT_MAX};
    int lblpos_t;
    uint16_t lbl_width;
    uint16_t fheight = (gScale->ShowLabels == ENABLE)? fheight = gScale->Font->Height : 0;
    uint16_t pos = 0;
    uint16_t  y0, y1, y11, y12;
    char lbl[10] = {0};

    if (gScale->Flip == ENABLE) {
        y0 = gScale->Y1 - fheight;
        y1 = y0-3;
        y11 = y0-5;
        y12 = y0+1;
    }
    else {
        y0 = gScale->Y0 + fheight;
        y1 = y0+3;
        y11 = y0+5;
        y12 = gScale->Y0;
    }

    // labels positions
    for (i = 0; i < gScale->LabelsCount; i++) {
    	lblpos[i] = calcPos(gScale, gScale->Labels[i]);
    }

    // lines
    if (gScale->ShowLines == ENABLE) {

        // base line
        NGL_GP_DrawLine(gScale->X0, y0, scale_X1, y0, gScale->Color);

        // cent lines
        for (i = gScale->MIN; i <= gScale->MAX; i += uscalecent) {
            pos = calcPos(gScale, i);
            NGL_GP_DrawLine(pos, y0, pos, y1, gScale->Color);
        }

        // label cent lines
        for (i = 0; i < gScale->LabelsCount; i++) {
            NGL_GP_DrawLine(lblpos[i], y0, lblpos[i], y11, gScale->Color);
        }
    }

    // labels
    if(gScale->ShowLabels == ENABLE) {

        NGL_Color_SetTextColor(gScale->Color);
        NGL_Font_SetFont(gScale->Font);

        for (i = 0; i < gScale->LabelsCount; i++) {

        	sprintf(lbl, "%d", gScale->Labels[i]);

            lbl_width = NGL_Font_MeasureStringWidth(gScale->Font, lbl);
            lblpos_t = lblpos[i] - (lbl_width >> 1);
            lblpos_t = nMAX(lblpos_t, gScale->X0);
            lblpos_t = nMIN(lblpos_t, gScale->X1 - lbl_width);

            if(gScale->Labels[i] < 0) {
            	lblpos_t -= NGL_Font_MeasureStringWidth(gScale->Font, "-") + gScale->Font->FontSpace;
            }

            NGL_Font_DrawFastString(nMIN(lblpos_t, scale_X1 - lbl_width), y12, Transparent, lbl);
        }

        // draw units label
        NGL_Font_DrawFastString(scale_X1 + 2, y12, Transparent, gScale->Units);
    }

}

/**
  * @brief  NGL_DrawFillBar
  * @param
  * @retval None
  */
void NGL_GUI_DrawGraphScale(const NGL_GraphScale* gScale)
{
    if(gScale->Orientation == NGL_Horizontal) {
        drawHorizontal(gScale);
    }
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
