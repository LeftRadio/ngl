/**
  ******************************************************************************
  * @file       NGL_Touch_Events.h
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date
  * @brief      header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UI_TOUCH_EVENTS_H
#define __UI_TOUCH_EVENTS_H

/* Includes ------------------------------------------------------------------*/
#include "NGL_types.h"

/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
int8_t NGL_Touch_Event(uint16_t data, NGL_TouchState state);
int8_t NGL_Touch_DoTouch(void);

int8_t NGL_Touch_GetEventStatus(void);
void NGL_Touch_ResetEventStatus(void);

int8_t NGL_Touch_ButtonEventFilter(const NGL_Button* btn, NGL_TouchType type);
int8_t NGL_Touch_CheckBoxEventFilter(const NGL_CheckBox* box, NGL_TouchType type);
int8_t NGL_Touch_SeekBarEventFilter(const NGL_SeekBar* bar, NGL_TouchType type);


#endif /* __UI_TOUCH_EVENTS_H */
/*******************************************************************************
      END FILE
*******************************************************************************/