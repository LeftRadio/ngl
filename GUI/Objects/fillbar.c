/**
  *****************************************************************************
  * @file    fillbar.c
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
  * @brief  NGL_Drawbar
  * @param
  * @retval None
  */
void NGL_GUI_Drawbar(const NGL_FillBar* FillBar)
{
    NGL_FillBar* bar = (NGL_FillBar*)FillBar;

    uint16_t fullscale;
    float level;
    uint16_t color;
    uint16_t fX0, fY0, fX1, fY1;

    // clip fillbar level value
    bar->Level = nMAX(nMIN(bar->Level, bar->Level_MAX), bar->Level_MIN);

    // if log type fillbar
    if(bar->Level != 0 && bar->Logarithmic == ENABLE) {
        fullscale = bar->FullScale_dB;
        level = fullscale - 20 * log10f( (float)(bar->Level_MAX - bar->Level_MIN) / (float)bar->Level );
    }
    else {
        fullscale = bar->Level_MAX - bar->Level_MIN;
        level = bar->Level;
    }

    // calc respect orientation
    if(bar->Orientation == NGL_Horizontal) {
        fX1 = bar->X0 + (uint16_t)( level * ( (float)(bar->X1 - bar->X0) / (float)fullscale ) );
        fY0 = bar->Y0;
        fY1 = bar->Y1;

        // if new X1 > old X1
        if(fX1 > bar->sfX1) {
            fX0 = bar->sfX1;
            color = bar->Color;
        }
        else { // else if new X1 < old X1
            fX0 = fX1;
            fX1 = bar->sfX1;
            color = NGL_Color_GetBackColor();
        }
    }
    else {
        fX0 = bar->X0;
        fX1 = bar->X1;
        fY1 = bar->Y0 + (uint16_t)( level * ( (float)(bar->Y1 - bar->Y0) / (float)fullscale ) );

        // if new Y1 > old Y1
        if(fY1 > bar->sfY1) {
            fY0 = bar->sfY1;
            color = bar->Color;
        }
        else { // else if new Y1 < old Y1
            fY0 = fY1;
            fY1 = bar->sfY1;
            color = NGL_Color_GetBackColor();
        }
    }

    // draw fillbar
    NGL_GP_DrawFillRect( fX0, fY0, fX1, fY1, color, DISABLE, 0 );

    // draw fillbar border
    if(bar->Border == ENABLE) {
        NGL_GP_DrawRect(bar->X0, bar->Y0, bar->X1, bar->Y1, bar->Color);
    }

    // draw markers
    if(bar->Markers == ENABLE) {

        if(bar->Orientation == NGL_Horizontal) {
            NGL_GP_DrawLine(bar->X0, bar->Y0, bar->X0, bar->Y1, bar->MarkersColor);
            NGL_GP_DrawLine(bar->X1, bar->Y0, bar->X1, bar->Y1, bar->MarkersColor);
        }
        else {
            NGL_GP_DrawLine(bar->X0, bar->Y0, bar->X1, bar->Y0, bar->MarkersColor);
            NGL_GP_DrawLine(bar->X0, bar->Y1, bar->X1, bar->Y1, bar->MarkersColor);
        }
    }

    // store new X/Y (for Horz/Vert respect) level coords
    bar->sfX1 = fX1;
    bar->sfY1 = fY1;
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
