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
// #include <string.h>
#include "NGL_types.h"
#include "clip.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ClipMaxObjects				4

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
NGL_ClipObject ClipObjects[ClipMaxObjects];
uint8_t NumOfActiveClipObjects = 0;
ngl_state gClipState = ngl_disable;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function  ---------------------------------------------------------*/

/**
  * @brief  Get code for Cohen-Sutherland algorithm
  * @param	obj index, point x/y
  * @retval Code for point
  */
static __inline int32_t CScode (uint8_t objNum, uint16_t X, uint16_t Y)
{
	uint8_t i = 0;

	if (X < ClipObjects[objNum].X_Left) ++i;
	else if (X > ClipObjects[objNum].X_Right) i+= 2;

	if (Y < ClipObjects[objNum].Y_Down) i+= 4;
	else if (Y > ClipObjects[objNum].Y_Up) i+= 8;

	return i;
}

/**
  * @brief  CS_ClipLine_Out_Window
  * @param
  * @retval Clip status - ngl_set or ngl_reset
  */
static ngl_flag CS_ClipLine_Out_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum)
{

	uint16_t X0, Y0, X1, Y1; 			/* ���������� ����� ������� */
	uint16_t c0, c1; 					/* ���� ������ ������� */
	ngl_flag visible = ngl_reset; 		/* 0/1 - �� �����/�����*/
	int32_t ii, s; 						/* ������� ���������� */
	uint32_t dx, dy, 					/* ���������� ���������*/
	dxdy,dydx, 							/* ���������� ��������� ����� */
	r; 									/* ������� ���������� */

	/* ���������� ���� ����� ������ ����� */
	X0 = *x0; Y0 = *y0;
	X1= *x1; Y1= *y1;

	c0 = CScode(objNum, X0, Y0);
	c1 = CScode(objNum, X1, Y1);


	/* ����������� ���������� ��������� � �������� �������
	* � ����. ������ ����� �� ���������� ���������� �������,
	* ��������� �� ������������ �����, �������� � ����
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


	/* �������� ���� ��������� */
	visible = ngl_reset;  ii= 4;
	do
	{
		if(c0 & c1)	   /* ������� ��� ���� */
		{
			break;
		}
		if(c0 == 0 && c1 == 0)		/* ������� ������ ���� */
		{
			visible = ngl_set;
			break;
		}

		/* ���� P0 ������ ����, �� */
		if(c0 == 0)
		{
			s= c0; c0= c1; c1= s; /* ����������� ���� ����� P0,P1 � �����*/
			r=X0; X0=X1; X1=r; 		/* ���, ����� P0 */
			r=Y0; Y0=Y1; Y1=r; 		/* ��������� ��� ���� */
		}

		/* ������ ������� �����������. P0 ���������� � �����
		* ����������� ������� �� �������� ����.
		*/
		if (c0 & 1)				/* ����������� � ����� �������� */
		{
			Y0= Y0 + dydx * (ClipObjects[objNum].X_Left - X0);
			X0= ClipObjects[objNum].X_Left;
		}
		else if (c0 & 2)   		/* ����������� � ������ ��������*/
		{
			Y0= Y0 + dydx * (ClipObjects[objNum].X_Right - X0);
			X0= ClipObjects[objNum].X_Right;
		} else if (c0 & 4)	   	/* ����������� � ������ ��������*/
		{
			X0= X0 + dxdy * (ClipObjects[objNum].Y_Down - Y0);
			Y0= ClipObjects[objNum].Y_Down;
		} else if (c0 & 8)	  	/*����������� � ������� ��������*/
		{
			X0= X0 + dxdy * (ClipObjects[objNum].Y_Up - Y0);
			Y0= ClipObjects[objNum].Y_Up;
		}

		c0 = CScode(objNum, X0, Y0); /* �������������� ���� ����� Pn */

	} while (--ii >= 0);

	if(visible == ngl_set)
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

/**
  * @brief  CS_ClipLine_IN_Window
  * @param
  * @retval Clip status - ngl_set or ngl_reset
  */
static ngl_flag CS_ClipLine_IN_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum)
{

	uint16_t X0, Y0, X1, Y1; 			/* ���������� ����� ������� */
	uint16_t c0, c1; 					/* ���� ������ ������� */
	ngl_flag visible = ngl_set; 			/* 0/1 - �� �����/�����*/
	int32_t ii, s; 						/* ������� ���������� */
	uint32_t dx, dy, 					/* ���������� ���������*/
	dxdy,dydx, 							/* ���������� ��������� ����� */
	r = 0; 								/* ������� ���������� */

	/* ���������� ���� ����� ������ ����� */
	X0 = *x0; Y0 = *y0;
	X1= *x1; Y1= *y1;

	c0 = CScode(objNum, X0, Y0);
	c1 = CScode(objNum, X1, Y1);

	/* ����������� ���������� ��������� � �������� �������
	* � ����. ������ ����� �� ���������� ���������� �������,
	* ��������� �� ������������ �����, �������� � ����
	*/
	dx= X1 - X0;
	dy= Y1 - Y0;
	if(dx != 0) dydx = 0;
	else if(dy == 0)
	{
		if ((c0 == 0) & (c1 == 0)) { visible = ngl_set; goto all; }
		else{ visible = ngl_reset; goto out; }
	}
	if (dy != 0) dxdy= 0;


	/* �������� ���� ��������� */
	visible = ngl_set;  ii= 4;
	do
	{
		if(c0 & c1)	   /* ������� ��� ���� */
		{
			visible = ngl_reset;
			break;
		}
		if(c0 == 0 && c1 == 0)		/* ������� ������ ���� */
		{
			visible = ngl_set;
			break;
		}

		/* ���� P0 ��� ����, �� */
		if(c0 != 0)
		{
			s= c0; c0= c1; c1= s;  /* ����������� ���� ����� P0,P1 � �����*/
			r=X0; X0=X1; X1=r; 		/* ���, ����� P0 */
			r=Y0; Y0=Y1; Y1=r; 		/* ��������� � ���� */
			r = 1;
		}

		/* ������ ������� �����������. P0 ���������� � �����
		* ����������� ������� �� �������� ����.
		*/
		if (c1 & 1)				/* ����������� � ����� �������� */
		{
			Y0 = Y0 + dydx * (ClipObjects[objNum].X_Left - X0);
			X0 = ClipObjects[objNum].X_Left;
		}
		else if (c1 & 2)   		/* ����������� � ������ ��������*/
		{
			Y0 = Y1 + dydx * (ClipObjects[objNum].X_Right - X0);
			X0 = ClipObjects[objNum].X_Right;
		} else if (c1 & 4)	   	/* ����������� � ������ ��������*/
		{
			X0 = X0 + dxdy * (ClipObjects[objNum].Y_Down - Y0);
			Y0 = ClipObjects[objNum].Y_Down;
		} else if (c1 & 8)	  	/*����������� � ������� ��������*/
		{
			X0 = X0 + dxdy * (ClipObjects[objNum].Y_Up - Y0);
			Y0 = ClipObjects[objNum].Y_Up;
		}

		c0 = ~(CScode(objNum, X0, Y0)); /* �������������� ���� ����� Pn */


	} while (--ii >= 0);

	if(visible == ngl_reset)
	{
		out:
		if(r == 0)
		{
			*x0 = X0; *x1 = X1;
			*y0 = Y0; *y1 = Y1;
		}
		else
		{
			*x0 = X1; *x1 = X0;
			*y0 = Y1; *y1 = Y0;
		}
	}

	all:
	return visible;
}


/**
  * @brief  NGL_GP_ClipNewObject
  * @param
  * @retval Operation status
  */
ngl_status NGL_GP_ClipNewObject(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, NGL_ClipType clipType, uint8_t NumInd)
{
	uint8_t i;

	if(NumInd == 0) return ngl_error;
	/* verify coordinates */
	else if(X0 >= X1) return ngl_error;
	else if(Y0 >= Y1) return ngl_error;

	/* Find first empty Object */
	for(i = 0; i < ClipMaxObjects; i++)
	{
		//str_cmp = strcmp (ClipObjects[i].Name, Name);

		if(ClipObjects[i].NumInd == NumInd) return ngl_error;
		else if(ClipObjects[i].State != ngl_enable)
		{
			/* init new restriction object */
			ClipObjects[i].X_Left = X0;
			ClipObjects[i].Y_Down = Y0;
			ClipObjects[i].X_Right = X1;
			ClipObjects[i].Y_Up = Y1;
			ClipObjects[i].NumInd = NumInd;
			ClipObjects[i].Type = clipType;
			ClipObjects[i].State = ngl_enable;

			if(clipType == NGL_Clip_IN) ClipObjects[i].pClipLine = CS_ClipLine_IN_Window;
			else ClipObjects[i].pClipLine = CS_ClipLine_Out_Window;

			NumOfActiveClipObjects++;

			gClipState = ngl_enable;
			return ngl_success;
		}
	}
	return ngl_error;
}

/**
  * @brief  NGL_GP_ClipClearObject
  * @param
  * @retval Operation status
  */
ngl_status NGL_GP_ClipClearObject(uint8_t NumInd)
{
	uint8_t i, cnt = 0;

	if(NumInd == 0) return ngl_error;

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
			ClipObjects[i].State = ngl_disable;
			ClipObjects[i].pClipLine = CS_ClipLine_IN_Window;

			NumOfActiveClipObjects--;
			break;
		}
		else if(ClipObjects[i].State == ngl_disable) cnt++;
	}

	if(NumOfActiveClipObjects == 0)	gClipState = ngl_disable;
	else if(cnt == ClipMaxObjects){ gClipState = ngl_disable; return ngl_error; }

	return ngl_success;
}

/**
  * @brief  NGL_GP_ClipPoint
  * @param	Point X/Y
  * @retval Status ngl_set or ngl_reset
  */
ngl_flag NGL_GP_ClipPoint(uint16_t X0, uint16_t Y0)
{
	uint8_t i;

	/* if restriction is ON for one or more objects */
	if(gClipState == ngl_enable)
	{
		for(i = 0; i <= NumOfActiveClipObjects; i++)
		{
			if(ClipObjects[i].State == ngl_enable)
			{
				if( (Y0 >= ClipObjects[i].Y_Down) && (Y0 <= ClipObjects[i].Y_Up) && \
					(X0 >= ClipObjects[i].X_Left) && (X0 <= ClipObjects[i].X_Right) ) {

					return ngl_set;
				}
			}
		}
	}

	return ngl_reset;
}

/**
  * @brief  * ��������� �������� ��������� �����-���������� �
			* ������������ ������ ����������� �������
			*
			* int CSclip (float *x0, float *y0, float *x1, float *y1)
			*
			* �������� �������, �������� ���������� ��������� ���
			* ����� (x0,y0), (x1,y1), �� ���� ���������, ���������
			* ����������� ��������� Wxlef, Wybot, Wxrig, Wytop
			*
			* �������� ������ ������� ������������� ����,
			* ��������������� ��� ��������� ������������ ���� ���������
			* �� �������:
			*

			 *  1001 | 1000 | 1010
			 *  -----|------|-----
			 *       | ���� |
			 *  0001 | 0000 | 0010
			 *  -----|------|-----
			 *  0101 | 0100 | 0110
			 *


			* ������� ������� ����� ���� ��� ��� ����� ����� ���� 0000
			* ���� ���������� � ����� ������ �� ����� 0, �� �������
			* ������� ��� ���� � �� ������ �������������.
			* ���� �� ��������� ���� �������� = 0, �� �������
			* ��������������. �� ����� ���� � ��� � ���������� ����.
			* ��� �������������� �������� ������������ ���������� ��
			* ����������� � ���� ���������, � �������� ��� ����� ��
			* ���������� � ������������ � ������ ������.
			* ��� ���� ������������ ���������������� � ��������������
			* ������ ����, ��� ��������� ���������� ���� �� ���������
			* ��� ����������.
			* ����� �������, ���������� �� ����� �������������.
			* ���������� ����� ������� ����������� �� ����������� ���
			* �������� ��� ������������ �������. ���� ��� ����������,
			* �� ������� ����������� ��� ������ ������� ����.
			* �� ������ ����� ���������� �������� ����� �������,
			* ���������� �� ����, ���������� �� �����, ������� ��� ��
			* ������� ���� ��� ��� �����������.
			*
			* ��������������� ��������� Code ��������� ��� ���������
			* ��� ����� �������.

  * @param  line vertex
  * @retval if line is part restrict return new X0, Y0 coordinate
  */
ngl_flag NGL_GP_ClipLine(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1)
{
	uint8_t i;
	ngl_flag Line_VisibleStatus = ngl_set;

	/* if restriction is ON for one or more objects */
	if(gClipState != ngl_disable)
	{
		for(i = 0; i <= NumOfActiveClipObjects; i++)
		{
			if(ClipObjects[i].State == ngl_enable)
			{
				Line_VisibleStatus = ClipObjects[i].pClipLine(x0, y0, x1, y1, i);

				if(NumOfActiveClipObjects == 1) return Line_VisibleStatus;
				else if(Line_VisibleStatus == ngl_set) return ngl_set;
			}
		}

		return ngl_reset;
	}

	return ngl_reset;
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
