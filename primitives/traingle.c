/**
  ******************************************************************************
  * @file       traingle.c
  * @author     LeftRadio
  * @version    1.2.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>                           /* Include standard types */
#include <math.h>

#include "NGL_types.h"
#include "traingle.h"
#include "primitives.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NGL_Point *pntL, *pntC, *pntU;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function  ---------------------------------------------------------*/

/**
  * @brief  Sort traingle vertex
  * @param  NGL_Point A, NGL_Point B, NGL_Point C
  * @retval None
  */
static void sortTraingleVertex(NGL_Point *A, NGL_Point *B, NGL_Point *C)
{
  if(A->Y < B->Y)
  {
	if(A->Y < C->Y)
	{
	  if(B->Y < C->Y){ pntC = B; pntU = C; }
	  else { pntC = C; pntU = B; }
	  pntL = A;
	}
	else {pntL = C; pntC = A; pntU = B; }
  }
  else if(B->Y < C->Y){ pntL = B; pntC = C; pntU = A; }
  else { pntL = C; pntC = B; pntU = A; }
}

/**
  * @brief  Sort traingle vertex
  * @param  Traingle vertex pntL, pntC, pntU
  * @retval None
  */
static void _drawLowerHalf(NGL_Point *pntL, NGL_Point *pntC, NGL_Point *pntU, uint16_t Color)
{
    signed short deltaX0, deltaX1, deltaY0, deltaY1;
    signed short signX0, signX1, signY0, signY1;
    signed short error0, error1;
    signed short error2_0, error2_1;

    NGL_Point A1, B1;
    NGL_Point A2, C2;

    A1.X = pntL->X; A1.Y = pntL->Y;
    A2.X = pntL->X; A2.Y = pntL->Y;
    B1.X = pntC->X; B1.Y = pntC->Y;
    C2.X = pntU->X; C2.Y = pntU->Y;

    deltaX0 = (int)fabsf(B1.X - A1.X);
    deltaY0 = B1.Y - A1.Y;
    signX0 =  A1.X < B1.X ? 1 : -1;
    signY0 =  1;
    error0 = deltaX0 - deltaY0;

    deltaX1 = (int)fabsf(C2.X - A2.X);
    deltaY1 = C2.Y - A2.Y;
    signX1 =  A2.X < C2.X ? 1 : -1;
    signY1 = 1 ;
    error1 = deltaX1 - deltaY1;

    // рисуем линию от нижней вершины до средней вершины
    for (;;)
    {
        if(A1.Y == B1.Y) {
          break;
        }

        error2_0 = error0 * 2;

        if(error2_0 > -deltaY0){ error0 -= deltaY0; A1.X += signX0; }
        if(error2_0 < deltaX0){ error0 += deltaX0; A1.Y += signY0; }

        // рисуем линию от нижней вершины до верхней вершины
        error2_1 = error1 * 2;

        if(error2_1 > -deltaY1){ error1 -= deltaY1; A2.X += signX1; }
        if(error2_1 < deltaX1){ error1 += deltaX1; A2.Y += signY1; }


        NGL_GP_DrawLine(A1.X, A1.Y, A2.X, A1.Y, Color);
    }
}

/**
  * @brief  Sort traingle vertex
  * @param  Traingle vertex pntL, pntC, pntU
  * @retval None
  */
static void _drawUpperHalf(NGL_Point *pntL, NGL_Point *pntC, NGL_Point *pntU, uint16_t Color)
{
    signed short deltaX0, deltaX1, deltaY0, deltaY1;
    signed short signX0, signX1, signY0, signY1;
    signed short error0, error1;
    signed short error2_0, error2_1;

    NGL_Point B1, C1;
    NGL_Point A2, C2;

    A2.X = pntL->X; A2.Y = pntL->Y;
    B1.X = pntC->X; B1.Y = pntC->Y;
    C1.X = pntU->X; C1.Y = pntU->Y;
    C2.X = pntU->X; C2.Y = pntU->Y;


    deltaX0 = (int)fabsf(B1.X - C1.X);
    deltaY0 = -(B1.Y - C1.Y);
    signX0 =  C1.X < B1.X ? 1 : -1;
    signY0 =  -1;;
    error0 = deltaX0 - deltaY0;

    deltaX1 = (int)fabsf(A2.X - C2.X);
    deltaY1 = -(A2.Y - C2.Y);
    signX1 =  C2.X < A2.X ? 1 : -1;
    signY1 = -1;
    error1 = deltaX1 - deltaY1;

    for (;;)
    {
        // рисуем линию от верхней вершины до средней вершины
        if(C1.Y == B1.Y)
        {
          break;
        }
        error2_0 = error0 * 2;

        if(error2_0 > -deltaY0){ error0 -= deltaY0; C1.X += signX0; }
        if(error2_0 < deltaX0){ error0 += deltaX0; C1.Y += signY0; }


        // рисуем линию от верхней вершины до нижней вершины
        error2_1 = error1 * 2;

        if(error2_1 > -deltaY1){ error1 -= deltaY1; C2.X += signX1; }
        if(error2_1 < deltaX1){ error1 += deltaX1; C2.Y += signY1; }

        // закрашиваем прямую лежащую между этими двумя линиями
        NGL_GP_DrawLine(C1.X, C1.Y, C2.X, C1.Y, Color);
    }
}

/**
  * @brief  LCD_GP_DrawTriangle
  * @param  x/y for every vertex
  * @retval None
  */
void LCD_GP_DrawTriangle(uint16_t x1,
                         uint16_t y1,
                         uint16_t x2,
                         uint16_t y2,
                         uint16_t x3,
                         uint16_t y3,
                         uint16_t Color)
{
    NGL_GP_DrawLine(x1, y1, x2, y2, Color);
    NGL_GP_DrawLine(x2, y2, x3, y3, Color);
    NGL_GP_DrawLine(x3, y3, x1, y1, Color);
}

/**
  * @brief  LCD_GP_DrawFillTriangle
  * @param  x/y for every vertex
  * @retval None
  */
void LCD_GP_DrawFillTriangle(uint16_t x1,
                             uint16_t y1,
                             uint16_t x2,
                             uint16_t y2,
                             uint16_t x3,
                             uint16_t y3,
                             uint16_t Color)
{
    NGL_Point A, B, C;

    A.X = x1; A.Y = y1;
	B.X = x2; B.Y = y2;
	C.X = x3; C.Y = y3;

	sortTraingleVertex(&A, &B, &C);

	_drawLowerHalf(pntL, pntC, pntU, Color);
	_drawUpperHalf(pntL, pntC, pntU, Color);
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
