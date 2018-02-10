/**
  *****************************************************************************
  * @file    GUI_elements.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  *****************************************************************************
**/

/* Includes -----------------------------------------------------------------*/
#include "NGL_types.h"
#include "primitives.h"
#include "colors.h"
#include "clip.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private function  --------------------------------------------------------*/


/**
  * @brief  NGL_GUI_DrawGrid
  * @param  NGL_Grid *Grid, NGL_DrawState state
  * @retval None
  */
void NGL_GUI_DrawGrid(NGL_Grid *Grid, NGL_DrawState state)
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
    if(Grid->CentralLines == ENABLE)
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






/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
