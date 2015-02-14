/*************************************************************************************
*
Project : HX8352 Graphics Library :: Low-lewel Layer header
Version : 
Date    : 
Author  : Left Radio                          
Comments: 
*
**************************************************************************************/

#ifndef __CLIP_H
#define __CLIP_H 

/* Includes ------------------------------------------------------------------*/	   
/* Exported typedef -----------------------------------------------------------*/
typedef struct	/* --- Restrict_TypeDef object  --- */
{ 
	uint16_t 			X_Left;
	uint16_t 			Y_Down;
	uint16_t 			X_Right;
	uint16_t 			Y_Up;
	uint8_t 			NumInd;
	FunctionalState 	State;
	FlagStatus  		(*pClipLine)(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum);
		  
} Clip_Obj_TypeDef;


/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function --------------------------------------------------------*/
extern ErrorStatus Set_New_ClipObject(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, ClipMode NewMode, uint8_t NumInd);
extern ErrorStatus Clear_ClipObject(uint8_t NumInd);
extern FlagStatus Verify_Clip_Point(uint16_t X0, uint16_t Y0);
extern FlagStatus Verify_Clip_Line(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1);

#endif /* __CLIP_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
