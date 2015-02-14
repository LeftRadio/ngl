/**
  ******************************************************************************
  * @file    Clip.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date    2015-02-12-10.50
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "Clip.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ClipMaxObjects				4

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
Clip_Obj_TypeDef ClipObjects[ClipMaxObjects];
uint8_t NumOfActiveClipObjects = 0;
FunctionalState gClipState = DISABLE;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
FlagStatus CS_ClipLine_Out_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum);
FlagStatus CS_ClipLine_IN_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum);


/* Private function  ---------------------------------------------------------*/

/******************************************************************************
* Function Name  : Set_NewRestrictObject
* Description    : задать новый объект для отсечения
* Input          : Xpos, Ypos, Color
* Return         : X0, Y0, X1, Y1
*******************************************************************************/
ErrorStatus Set_New_ClipObject(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, ClipMode NewMode, uint8_t NumInd)
{
	uint8_t i;

	if(NumInd == 0) return ERROR;
	/* verify coordinates */
	else if(X0 >= X1) return ERROR;
	else if(Y0 >= Y1) return ERROR;

	/* Find first empty Object */
	for(i = 0; i < ClipMaxObjects; i++)
	{
		//str_cmp = strcmp (ClipObjects[i].Name, Name);

		if(ClipObjects[i].NumInd == NumInd) return ERROR;
		else if(ClipObjects[i].State != ENABLE)
		{
			/* init new restriction object */
			ClipObjects[i].X_Left = X0;
			ClipObjects[i].Y_Down = Y0;
			ClipObjects[i].X_Right = X1;
			ClipObjects[i].Y_Up = Y1;
			ClipObjects[i].NumInd = NumInd;
			ClipObjects[i].State = ENABLE;

			if(NewMode == IN_OBJECT) ClipObjects[i].pClipLine = CS_ClipLine_IN_Window;
			else ClipObjects[i].pClipLine = CS_ClipLine_Out_Window;

			NumOfActiveClipObjects++;

			gClipState = ENABLE;
			return SUCCESS;
		}
	}
	return ERROR;
}


/******************************************************************************
* Function Name  : Clear_RestrictObject
* Description    : удалить объект для отсечения
* Input          : Xpos, Ypos, Color
* Return         : X0, Y0, X1, Y1
*******************************************************************************/
ErrorStatus Clear_ClipObject(uint8_t NumInd)
{
	uint8_t i, cnt = 0;

	if(NumInd == 0) return ERROR;

	/* Find Object compare Name */
	for(i = 0; i < ClipMaxObjects; i++)
	{
		if(ClipObjects[i].NumInd == NumInd)
		{
			/* delete restriction object */
			ClipObjects[i].X_Left = 0;
			ClipObjects[i].Y_Down = 0;
			ClipObjects[i].X_Right = 0;
			ClipObjects[i].Y_Up = 0;
			ClipObjects[i].NumInd = 0;
			ClipObjects[i].State = DISABLE;
			ClipObjects[i].pClipLine = CS_ClipLine_IN_Window;

			NumOfActiveClipObjects--;
			break;
		}
		else if(ClipObjects[i].State == DISABLE) cnt++;
	}

	if(NumOfActiveClipObjects == 0)	gClipState = DISABLE;
	else if(cnt == ClipMaxObjects){ gClipState = DISABLE; return ERROR; }

	return SUCCESS;
}



/*******************************************************************************
* Function Name  : Verify_Restriction_Point
* Description    :
* Input          : uint16_t X0, uint16_t Y0
* Return         : FlagStatus  SET or RESET
*******************************************************************************/
FlagStatus Verify_Clip_Point(uint16_t X0, uint16_t Y0)
{
	uint8_t i;

	/* if restriction is ON for one or more objects */
	if(gClipState == ENABLE)
	{
		for(i = 0; i <= NumOfActiveClipObjects; i++)
		{
			if(ClipObjects[i].State == ENABLE)
			{
				if(Y0 <= ClipObjects[i].Y_Up)
				{
					if((Y0 >= ClipObjects[i].Y_Down))
					{
						if((X0 >= ClipObjects[i].X_Left) && (X0 <= ClipObjects[i].X_Right))
						{
							return SET;
						}
					}
				}
			}
		}
	}

	return RESET;
}




/*--------------------------------------------------- CS_RestrictLine
* Реализует алгоритм отсечения Коэна-Сазерленда с
* кодированием концов отсекаемого отрезка
*
* int V_CSclip (float *x0, float *y0, float *x1, float *y1)
*
* Отсекает отрезок, заданный значениями координат его
* точек (x0,y0), (x1,y1), по окну отсечения, заданному
* глобальными скалярами Wxlef, Wybot, Wxrig, Wytop
*
* Конечным точкам отрезка приписываются коды,
* характеризующие его положение относительно окна отсечения
* по правилу:
*

 *  1001 | 1000 | 1010
 *  -----|------|-----
 *       | Окно |
 *  0001 | 0000 | 0010
 *  -----|------|-----
 *  0101 | 0100 | 0110
 *


* Отрезок целиком видим если оба его конца имеют коды 0000
* Если логическое И кодов концов не равно 0, то отрезок
* целиком вне окна и он просто отбрасывается.
* Если же результат этой операции = 0, то отрезок
* подозрительный. Он может быть и вне и пересекать окно.
* Для подозрительных отрезков определяются координаты их
* пересечений с теми сторонами, с которыми они могли бы
* пересечься в соответствии с кодами концов.
* При этом используется горизонтальность и вертикальность
* сторон окна, что позволяет определить одну из координат
* без вычислений.
* Часть отрезка, оставшаяся за окном отбрасывается.
* Оставшаяся часть отрезка проверяется на возможность его
* принятия или отбрасывания целиком. Если это невозможно,
* то процесс повторяется для другой стороны окна.
* На каждом цикле вычислений конечная точка отрезка,
* выходившая за окно, заменяется на точку, лежащую или на
* стороне окна или его продолжении.
*
* Вспомогательная процедура Code вычисляет код положения
* для конца отрезка.
*
*/



/*******************************************************************************
* Function Name  : Verify_Clip_Line
* Description    :
* Input          : uint16_t X0, uint16_t Y0
* Return         : FlagStatus  SET or RESET
* Output         : if line is part restrict return new X0, Y0 coordinate
*******************************************************************************/
FlagStatus Verify_Clip_Line(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1)
{
	uint8_t i;
	FlagStatus Line_VisibleStatus = SET;

	/* if restriction is ON for one or more objects */
	if(gClipState != DISABLE)
	{
		for(i = 0; i <= NumOfActiveClipObjects; i++)
		{
			if(ClipObjects[i].State == ENABLE)
			{
				Line_VisibleStatus = ClipObjects[i].pClipLine(x0, y0, x1, y1, i);

				if(NumOfActiveClipObjects == 1) return Line_VisibleStatus;
				else if(Line_VisibleStatus == SET) return SET;
			}
		}

		return RESET;
	}

	return RESET;
}



static uint16_t X0, Y0; /* Координаты начала отрезка */

/*******************************************************************************
* Function Name  : CScode
* Description    :
* Input          : uint8_t obj
* Return         : Code of virify Point
*******************************************************************************/
static __inline int32_t CScode (uint8_t objNum) /* Определяет код точки x0, y0 */
{
	uint8_t i = 0;

	if (X0 < ClipObjects[objNum].X_Left) ++i;
	else if (X0 > ClipObjects[objNum].X_Right) i+= 2;

	if (Y0 < ClipObjects[objNum].Y_Down) i+= 4;
	else if (Y0 > ClipObjects[objNum].Y_Up) i+= 8;

	return i;
} /* CScode */



/*******************************************************************************
* Function Name  : CS_ClipLine
* Description    :
* Input          : uint16_t X0, uint16_t Y0
* Return         : FlagStatus  SET or RESET
* Output         : if line is part restrict return new X0, Y0 coordinate
*******************************************************************************/
FlagStatus CS_ClipLine_Out_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum)
{

	uint16_t X1, Y1; 					/* Координаты конца отрезка */
	uint16_t c0, c1; 					/* Коды концов отрезка */
	FlagStatus visible = RESET; 		/* 0/1 - не видим/видим*/
	int32_t ii, s; 						/* Рабочие переменные */
	uint32_t dx, dy, 					/* Приращения координат*/
	dxdy,dydx, 							/* Напрвление координат линии */
	r; 									/* Рабочая переменная */

	/* Определяем коды обеих концов линии */
	X1= *x1; Y1= *y1;
	X0 = *x1; Y0= *y1; c1 = CScode(objNum);
	X0= *x0; Y0= *y0; c0 = CScode(objNum);

	/* Определение приращений координат и наклонов отрезка
	* к осям. Заодно сразу на построение передается отрезок,
	* состоящий из единственной точки, попавшей в окно
	*/
	dx= X1 - X0;
	dy= Y1 - Y0;
	if(dx != 0) dydx = 0;
	else if(dy == 0)
	{
		if ((c0 == 0) && (c1 == 0)) goto all;
		else goto out;
	}
	if (dy != 0) dxdy= 0;


	/* Основной цикл отсечения */
	visible = RESET;  ii= 4;
	do
	{
		if(c0 & c1)	   /* Целиком вне окна */
		{
			break;
		}
		if(c0 == 0 && c1 == 0)		/* Целиком внутри окна */
		{
			visible = SET;
			break;
		}

		/* Если P0 внутри окна, то */
		if(c0 == 0)
		{
			s= c0; c0= c1; c1= s; /* переместить коды точек P0,P1 и точки*/
			r=X0; X0=X1; X1=r; 		/* так, чтобы P0 */
			r=Y0; Y0=Y1; Y1=r; 		/* оказалась вне окна */
		}

		/* Теперь отрезок разделяется. P0 помещается в точку
		* пересечения отрезка со стороной окна.
		*/
		if (c0 & 1)				/* Пересечение с левой стороной */
		{
			Y0= Y0 + dydx * (ClipObjects[objNum].X_Left - X0);
			X0= ClipObjects[objNum].X_Left;
		}
		else if (c0 & 2)   		/* Пересечение с правой стороной*/
		{
			Y0= Y0 + dydx * (ClipObjects[objNum].X_Right - X0);
			X0= ClipObjects[objNum].X_Right;
		} else if (c0 & 4)	   	/* Пересечение в нижней стороной*/
		{
			X0= X0 + dxdy * (ClipObjects[objNum].Y_Down - Y0);
			Y0= ClipObjects[objNum].Y_Down;
		} else if (c0 & 8)	  	/*Пересечение с верхней стороной*/
		{
			X0= X0 + dxdy * (ClipObjects[objNum].Y_Up - Y0);
			Y0= ClipObjects[objNum].Y_Up;
		}
		c0= CScode (objNum); /* Перевычисление кода точки Pn */

	} while (--ii >= 0);

	if(visible == SET)
	{
		out:
		if(X0 < X1){ *x0 = X0; *x1= X1; }
		else{ *x0 = X1; *x1= X0; }

		if(Y0 < Y1){ *y0= Y0; *y1= Y1; }
		else{ *y0= Y1; *y1= Y0; }
	}

	all:
	return visible;
} /* V_CSclip */



/*******************************************************************************
* Function Name  : CS_ClipLine
* Description    :
* Input          : uint16_t X0, uint16_t Y0
* Return         : FlagStatus  SET or RESET
* Output         : if line is part restrict return new X0, Y0 coordinate
*******************************************************************************/
FlagStatus CS_ClipLine_IN_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum)
{

	uint16_t X1, Y1; 					/* Координаты конца отрезка */
	uint16_t c0, c1; 					/* Коды концов отрезка */
	FlagStatus visible = SET; 		/* 0/1 - не видим/видим*/
	int32_t ii, s; 						/* Рабочие переменные */
	uint32_t dx, dy, 					/* Приращения координат*/
	dxdy,dydx, 							/* Напрвление координат линии */
	r = 0; 									/* Рабочая переменная */

	/* Определяем коды обеих концов линии */
	X1= *x1; Y1= *y1;
	X0 = *x1; Y0= *y1; c1 = CScode(objNum);
	X0= *x0; Y0= *y0; c0 = CScode(objNum);

	/* Определение приращений координат и наклонов отрезка
	* к осям. Заодно сразу на построение передается отрезок,
	* состоящий из единственной точки, попавшей в окно
	*/
	dx= X1 - X0;
	dy= Y1 - Y0;
	if(dx != 0) dydx = 0;
	else if(dy == 0)
	{
		if ((c0 == 0) & (c1 == 0)) { visible = SET; goto all; }
		else{ visible = RESET; goto out; }
	}
	if (dy != 0) dxdy= 0;


	/* Основной цикл отсечения */
	visible = SET;  ii= 4;
	do
	{
		if(c0 & c1)	   /* Целиком вне окна */
		{
			visible = RESET;
			break;
		}
		if(c0 == 0 && c1 == 0)		/* Целиком внутри окна */
		{
			visible = SET;
			break;
		}

		/* Если P0 вне окна, то */
		if(c0 != 0)
		{
			s= c0; c0= c1; c1= s;  /* переместить коды точек P0,P1 и точки*/
			r=X0; X0=X1; X1=r; 		/* так, чтобы P0 */
			r=Y0; Y0=Y1; Y1=r; 		/* оказалась в окне */
			r = 1;
		}

		/* Теперь отрезок разделяется. P0 помещается в точку
		* пересечения отрезка со стороной окна.
		*/
		if (c1 & 1)				/* Пересечение с левой стороной */
		{
			Y0 = Y0 + dydx * (ClipObjects[objNum].X_Left - X0);
			X0 = ClipObjects[objNum].X_Left;
		}
		else if (c1 & 2)   		/* Пересечение с правой стороной*/
		{
			Y0 = Y1 + dydx * (ClipObjects[objNum].X_Right - X0);
			X0 = ClipObjects[objNum].X_Right;
		} else if (c1 & 4)	   	/* Пересечение в нижней стороной*/
		{
			X0 = X0 + dxdy * (ClipObjects[objNum].Y_Down - Y0);
			Y0 = ClipObjects[objNum].Y_Down;
		} else if (c1 & 8)	  	/*Пересечение с верхней стороной*/
		{
			X0 = X0 + dxdy * (ClipObjects[objNum].Y_Up - Y0);
			Y0 = ClipObjects[objNum].Y_Up;
		}
		c0 = ~CScode (objNum); /* Перевычисление кода точки Pn */

	} while (--ii >= 0);

	if(visible == RESET)
	{
		out:
		if(r == 0)
		{
			*x0 = X0; *x1= X1;
			*y0= Y0; *y1= Y1;
		}
		else
		{
			*x0 = X1; *x1= X0;
			*y0= Y1; *y1= Y0;
		}
	}

	all:
	return visible;
} /* V_CSclip */






///*--------------------------------------------------- V_FCclip
// *  Реализует алгоритм отсечения FC (Fast Clipping)
// *  Собкова-Поспишила-Янга, с кодированием линий
// *
// * int  V_FCclip (float *x0, float *y0, float *x1, float *y1)
// *
// * Отсекает отрезок, заданный значениями координат его
// * точек (x0,y0), (x1,y1), по окну отсечения, заданному
// * глобальными скалярами Wxlef, Wybot, Wxrig, Wytop
// *
// * Возвращает:
// * -1 - ошибка в задании окна
// *  0 - отрезок не видим
// *  1 - отрезок видим
// */
//
//
//static float FC_xn, FC_yn, FC_xk, FC_yk;
//
//
//static void Clip0_Top(void)
//{FC_xn= FC_xn + (FC_xk-FC_xn)*(Wytop-FC_yn)/(FC_yk-FC_yn);
// FC_yn= Wytop; }
//
//
//static void Clip0_Bottom(void)
//{FC_xn= FC_xn + (FC_xk-FC_xn)*(Wybot-FC_yn)/(FC_yk-FC_yn);
// FC_yn= Wybot; }
//
//
//static void Clip0_Right(void)
//{FC_yn= FC_yn + (FC_yk-FC_yn)*(Wxrig-FC_xn)/(FC_xk-FC_xn);
// FC_xn= Wxrig; }
//
//
//static void Clip0_Left(void)
//{FC_yn= FC_yn + (FC_yk-FC_yn)*(Wxlef-FC_xn)/(FC_xk-FC_xn);
// FC_xn= Wxlef; }
//
//
//static void Clip1_Top(void)
//{FC_xk= FC_xk + (FC_xn-FC_xk)*(Wytop-FC_yk)/(FC_yn-FC_yk);
// FC_yk= Wytop; }
//
//
//static void Clip1_Bottom(void)
//{FC_xk= FC_xk + (FC_xn-FC_xk)*(Wybot-FC_yk)/(FC_yn-FC_yk);
// FC_yk= Wybot; }
//
//
//static void Clip1_Right(void)
//{FC_yk= FC_yk + (FC_yn-FC_yk)*(Wxrig-FC_xk)/(FC_xn-FC_xk);
// FC_xk= Wxrig; }
//
//
//static void Clip1_Left(void)
//{FC_yk= FC_yk + (FC_yn-FC_yk)*(Wxlef-FC_xk)/(FC_xn-FC_xk);
// FC_xk= Wxlef; }
//
//int  V_FCclip (x0, y0, x1, y1)
//float *x0, *y0, *x1, *y1;
//{  int  Code= 0;
//   int  visible= 0;             /* Отрезок невидим */
//
//
//   FC_xn= *x0;  FC_yn= *y0;
//   FC_xk= *x1;  FC_yk= *y1;
//
//
///*
// * Вычисление значения Code - кода отрезка
// * Биты 0-3 - для конечной точки, 4-7 - для начальной
// *
// */
//   if (FC_yk > Wytop) Code+= 8; else
//   if (FC_yk < Wybot) Code+= 4;
//
//
//   if (FC_xk > Wxrig) Code+= 2; else
//   if (FC_xk < Wxlef) Code+= 1;
//
//
//   if (FC_yn > Wytop) Code+= 128; else
//   if (FC_yn < Wybot) Code+= 64;
//
//
//   if (FC_xn > Wxrig) Code+= 32; else
//   if (FC_xn < Wxlef) Code+= 16;
//
//
///* Отсечение для каждого из 81-го случаев */
//
//
//   switch (Code) {
//
//
//     /* Из центра */
//
//
//     case 0x00: ++visible;  break;
//     case 0x01: Clip1_Left() ;   ++visible;  break;
//     case 0x02: Clip1_Right();  ++visible;  break;
//     case 0x04: Clip1_Bottom(); ++visible;  break;
//     case 0x05: Clip1_Left() ;
//                if (FC_yk < Wybot) Clip1_Bottom();
//                ++visible;  break;
//     case 0x06: Clip1_Right();
//                if (FC_yk < Wybot) Clip1_Bottom();
//                ++visible;  break;
//     case 0x08: Clip1_Top();    ++visible;  break;
//     case 0x09: Clip1_Left() ;
//                if (FC_yk > Wytop) Clip1_Top();
//                ++visible;  break;
//     case 0x0A: Clip1_Right();
//                if (FC_yk > Wytop) Clip1_Top();
//                ++visible;  break;
//
//
//
//
//
//
//
//
//
//
//     /* Слева */
//
//
//     case 0x10: Clip0_Left();   ++visible;
//     case 0x11: break;                          /* Отброшен */
//     case 0x12: Clip0_Left();   Clip1_Right();
//                ++visible;  break;
//     case 0x14: Clip0_Left();
//                if (FC_yn < Wybot) break;       /* Отброшен */
//                Clip1_Bottom();
//                ++visible;
//     case 0x15: break;                          /* Отброшен */
//     case 0x16: Clip0_Left();
//                if (FC_yn < Wybot) break;       /* Отброшен */
//                Clip1_Bottom();
//                if (FC_xk > Wxrig) Clip1_Right();
//                ++visible;
//                break;
//     case 0x18: Clip0_Left();
//                if (FC_yn > Wytop) break;       /* Отброшен */
//                Clip1_Top();
//                ++visible;
//     case 0x19: break;                          /* Отброшен */
//     case 0x1A: Clip0_Left();
//                if (FC_yn > Wytop) break;       /* Отброшен */
//                Clip1_Top();
//                if (FC_xk > Wxrig) Clip1_Right();
//                ++visible;
//                break;
//
//     /* Справа */
//
//
//     case 0x20: Clip0_Right(); ++visible;  break;
//     case 0x21: Clip0_Right(); Clip1_Left(); ++visible;
//     case 0x22: break;                          /* Отброшен */
//     case 0x24: Clip0_Right();
//                if (FC_yn < Wybot) break;       /* Отброшен */
//                Clip1_Bottom();
//                ++visible;
//                break;
//     case 0x25: Clip0_Right();
//                if (FC_yn < Wybot) break;       /* Отброшен */
//                Clip1_Bottom();
//                if (FC_xk < Wxlef) Clip1_Left();
//                ++visible;
//     case 0x26: break;                          /* Отброшен */
//     case 0x28: Clip0_Right();
//                if (FC_yn > Wytop) break;       /* Отброшен */
//                Clip1_Top();
//                ++visible;
//                break;
//     case 0x29: Clip0_Right();
//                if (FC_yn > Wytop) break;       /* Отброшен */
//                Clip1_Top();
//                if (FC_xk < Wxlef) Clip1_Left();
//                ++visible;
//     case 0x2A: break;                          /* Отброшен */
//
//     /* Снизу */
//
//
//     case 0x40: Clip0_Bottom(); ++visible;  break;
//     case 0x41: Clip0_Bottom();
//                if (FC_xn < Wxlef) break;       /* Отброшен */
//                Clip1_Left() ;
//                if (FC_yk < Wybot) Clip1_Bottom();
//                ++visible;
//                break;
//     case 0x42: Clip0_Bottom();
//                if (FC_xn > Wxrig) break;       /* Отброшен */
//                Clip1_Right();
//                ++visible;
//     case 0x44:
//     case 0x45:
//     case 0x46: break;                          /* Отброшен */
//     case 0x48: Clip0_Bottom();
//                Clip1_Top();
//                ++visible;
//                break;
//     case 0x49: Clip0_Bottom();
//                if (FC_xn < Wxlef) break;       /* Отброшен */
//                Clip1_Left() ;
//                if (FC_yk > Wytop) Clip1_Top();
//                ++visible;
//                break;
//     case 0x4A: Clip0_Bottom();
//                if (FC_xn > Wxrig) break;       /* Отброшен */
//                Clip1_Right();
//                if (FC_yk > Wytop) Clip1_Top();
//                ++visible;
//                break;
//
//     /* Снизу слева */
//
//
//     case 0x50: Clip0_Left();
//                if (FC_yn < Wybot) Clip0_Bottom();
//                ++visible;
//     case 0x51: break;                          /* Отброшен */
//     case 0x52: Clip1_Right();
//                if (FC_yk < Wybot) break;       /* Отброшен */
//                Clip0_Bottom();
//                if (FC_xn < Wxlef) Clip0_Left();
//                ++visible;
//     case 0x54:
//     case 0x55:
//     case 0x56: break;                          /* Отброшен */
//     case 0x58: Clip1_Top();
//                if (FC_xk < Wxlef) break;       /* Отброшен */
//                Clip0_Bottom();
//                if (FC_xn < Wxlef) Clip0_Left();
//                ++visible;
//     case 0x59: break;                          /* Отброшен */
//     case 0x5A: Clip0_Left();
//                if (FC_yn > Wytop) break;       /* Отброшен */
//                Clip1_Right();
//                if (FC_yk < Wybot) break;       /* Отброшен */
//                if (FC_yn < Wybot) Clip0_Bottom();
//                if (FC_yk > Wytop) Clip1_Top();
//                ++visible;
//                break;
//
//     /* Снизу-справа */
//
//
//     case 0x60: Clip0_Right();
//                if (FC_yn < Wybot) Clip0_Bottom();
//                ++visible;
//                break;
//     case 0x61: Clip1_Left() ;
//                if (FC_yk < Wybot) break;       /* Отброшен */
//                Clip0_Bottom();
//                if (FC_xn > Wxrig) Clip0_Right();
//                ++visible;
//     case 0x62:
//     case 0x64:
//     case 0x65:
//     case 0x66: break;                          /* Отброшен */
//     case 0x68: Clip1_Top();
//                if (FC_xk > Wxrig) break;       /* Отброшен */
//                Clip0_Right();
//                if (FC_yn < Wybot) Clip0_Bottom();
//                ++visible;
//                break;
//     case 0x69: Clip1_Left() ;
//                if (FC_yk < Wybot) break;       /* Отброшен */
//                Clip0_Right();
//                if (FC_yn > Wytop) break;       /* Отброшен */
//                if (FC_yk > Wytop) Clip1_Top();
//                if (FC_yn < Wybot) Clip0_Bottom();
//                ++visible;
//     case 0x6A: break;                          /* Отброшен */
//
//     /* Сверху */
//
//
//     case 0x80: Clip0_Top();
//                ++visible;
//                break;
//     case 0x81: Clip0_Top();
//                if (FC_xn < Wxlef) break;       /* Отброшен */
//                Clip1_Left() ;
//                ++visible;
//                break;
//     case 0x82: Clip0_Top();
//                if (FC_xn > Wxrig) break;       /* Отброшен */
//                Clip1_Right();
//                ++visible;
//                break;
//     case 0x84: Clip0_Top();
//                Clip1_Bottom();
//                ++visible;
//                break;
//     case 0x85: Clip0_Top();
//                if (FC_xn < Wxlef) break;       /* Отброшен */
//                Clip1_Left() ;
//                if (FC_yk < Wybot) Clip1_Bottom();
//                ++visible;
//                break;
//     case 0x86: Clip0_Top();
//                if (FC_xn > Wxrig) break;       /* Отброшен */
//                Clip1_Right();
//                if (FC_yk < Wybot) Clip1_Bottom();
//                ++visible;
//     case 0x88:
//     case 0x89:
//     case 0x8A: break;                          /* Отброшен */
//
//     /* Сверху-слева */
//
//
//     case 0x90: Clip0_Left();
//                if (FC_yn > Wytop) Clip0_Top();
//                ++visible;
//     case 0x91: break;                          /* Отброшен */
//     case 0x92: Clip1_Right();
//                if (FC_yk > Wytop) break;       /* Отброшен */
//                Clip0_Top();
//                if (FC_xn < Wxlef) Clip0_Left();
//                ++visible;
//                break;
//     case 0x94: Clip1_Bottom();
//                if (FC_xk < Wxlef) break;       /* Отброшен */
//                Clip0_Left();
//                if (FC_yn > Wytop) Clip0_Top();
//                ++visible;
//     case 0x95: break;                          /* Отброшен */
//     case 0x96: Clip0_Left();
//                if (FC_yn < Wybot) break;       /* Отброшен */
//                Clip1_Right();
//                if (FC_yk > Wytop) break;       /* Отброшен */
//                if (FC_yn > Wytop) Clip0_Top();
//                if (FC_yk < Wybot) Clip1_Bottom();
//                ++visible;
//     case 0x98:
//     case 0x99:
//     case 0x9A: break;                          /* Отброшен */
//
//     /* Сверху-справа */
//
//
//     case 0xA0: Clip0_Right();
//                if (FC_yn > Wytop) Clip0_Top();
//                ++visible;
//                break;
//     case 0xA1: Clip1_Left() ;
//                if (FC_yk > Wytop) break;       /* Отброшен */
//                Clip0_Top();
//                if (FC_xn > Wxrig) Clip0_Right();
//                ++visible;
//     case 0xA2: break;                          /* Отброшен */
//     case 0xA4: Clip1_Bottom();
//                if (FC_xk > Wxrig) break;       /* Отброшен */
//                Clip0_Right();
//                if (FC_yn > Wytop) Clip0_Top();
//                ++visible;
//                break;
//     case 0xA5: Clip1_Left() ;
//                if (FC_yk > Wytop) break;       /* Отброшен */
//                Clip0_Right();
//                if (FC_yn < Wybot) break;       /* Отброшен */
//                if (FC_yk < Wybot) Clip1_Bottom();
//                if (FC_yn > Wytop) Clip0_Top();
//                ++visible;
//     case 0xA6:                                 /* Отброшен */
//     case 0xA8:
//     case 0xA9:
//     case 0xAA: break;
//
//     /* Ошибка */
//
//
//     default:   visible= -1;
//                break;
//   }  /* switch */
//
//
//   if (visible > 0) {
//      *x0= FC_xn;  *y0= FC_yn;
//      *x1= FC_xk;  *y1= FC_yk;
//   }
//   return (visible);
//}  /* V_FCclip */

