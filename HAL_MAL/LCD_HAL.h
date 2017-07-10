/**
  ******************************************************************************
  * @file    LCD_HAL.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

#ifndef __LCD_HAL_H
#define __LCD_HAL_H

/* Includes ------------------------------------------------------------------*/
#include "NGL_types.h"

/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/

/* Exported/Extern variables -------------------------------------------------*/
extern NGL_HAL_Typedef *LCD;
extern void (*NGL_Delay)(uint16_t delay);

/* Exported function ---------------------------------------------------------*/
int8_t NGL_HAL_Init(NGL_HAL_Typedef* controller, void *pDelayFunc);
void NGL_HAL_WriteRegister(uint16_t RegisterAddress, uint16_t Data);

#endif /* __LCD_HAL_H */
/*******************************************************************************
      END FILE
*******************************************************************************/
