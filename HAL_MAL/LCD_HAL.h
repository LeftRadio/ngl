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

/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported/Extern variables -------------------------------------------------*/
extern LCD_Typedef *LCD;

/* Exported function ---------------------------------------------------------*/
void NGL_LCD_DelayInit(void *pfunc);
extern void (*NGL_Delay)(uint16_t delay);

int8_t NGL_LCD_ControllerInit(LCD_Typedef* LCD_init, void *pDelayFunc);
LCD_Typedef* NGL_LCD_GetController_FromIndex(uint8_t index);
void NGL_LCD_SetRotation(NGL_RotationLCD NewState);
void NGL_LCD_WriteRegister(uint16_t RegisterAddress, uint16_t Data);



#endif /* __LCD_HAL_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
