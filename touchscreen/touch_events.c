/**
  ******************************************************************************
  * @file       NGL_Touch_Events.c
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"
#include "NGL_types.h"
#include "touch.h"
#include "touch_events.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile int8_t touch_event_status = -1;

volatile Coordinate _event_touchPoint;
volatile NGL_TouchType _event_touchType;

/* Exported function ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  _touch_adc_event
 * @param  None
 * @retval None
 */
static __inline int8_t _touch_adc_event(NGL_TouchScreen* touch, uint16_t data, NGL_TouchState state) {

  int8_t status = -1;

  if((state == Touch_Interrupt) || (state == Touch_Default)) {
    touch->callbacks->HAL_SetState(Touch_AxisX_State);
    touch->state = Touch_AxisX_State;
    status = 0;
  }
  else if(state == Touch_Data){

    if (touch->state == Touch_AxisX_State) {
      touch->data.x = data;
      touch->callbacks->HAL_SetState(Touch_AxisY_State);
      touch->state = Touch_AxisY_State;
      status = 0;
    }
    else if(touch->state == Touch_AxisY_State) {

      touch->data.y = data;
      touch->callbacks->HAL_SetState(Touch_AxisX_State);
      touch->state = Touch_AxisX_State;

      status = 1;
    }
  }
  return status;
}

/**
 * @brief  _touch_spi_event
 * @param  None
 * @retval None
 */
static __inline int8_t _touch_spi_event( NGL_TouchScreen* touch, uint16_t data, NGL_TouchState state) {
  return 0;
}

/**
 * @brief  touch "low pass" filter
 * @param  None
 * @retval None
 */
static __inline int8_t _touch_filter(Coordinate* pnt, bool Reset)
{
  const uint8_t samples = 2;
  const uint8_t hysteresis = 12;

  static Coordinate old_pnts[4] = {{0, 0}};
  static uint8_t i = 0;
  uint8_t j;
  int8_t rng;

  if(Reset == TRUE) {
    for (j = 0; j < samples; j++)
    {
      old_pnts[j].x = 0;
      old_pnts[j].y = 0;
    }
    i = 0;
  }
  else {
    if (i < samples) {
      old_pnts[i].x = pnt->x;
      old_pnts[i].y = pnt->y;
      i++;
    }
    else {

      i = 0;
      for (j = 0; j < samples-1; j++)
      {
        rng = nRANGE(old_pnts[j].x, old_pnts[j+1].x, hysteresis);
        if ( rng == -1 ) {
          return -1;
        }
      }

      return 0;
    }
  }

  return -1;
}

/**
 * @brief  touch "hight pass" filter
 * @param  None
 * @retval None
 */
static __inline int8_t _touch_hp_filter(Coordinate* pnt, bool Reset) {
  int8_t status = -1;
  static Coordinate old_pnt = {UINT_LEAST16_MAX, UINT_LEAST16_MAX};

  if(Reset == TRUE) {
    old_pnt.x = UINT_LEAST16_MAX;
    old_pnt.y = UINT_LEAST16_MAX;
  }
  else {

    if ( (old_pnt.x != pnt->x) || (old_pnt.y != pnt->y) ) {

      if (  (old_pnt.x != UINT_LEAST16_MAX) && (old_pnt.y != UINT_LEAST16_MAX) ) {
        pnt->x = (pnt->x + old_pnt.x) >> 1;
        pnt->y = (pnt->y + old_pnt.y) >> 1;
        status = 0;
      }
      else {
        status = -1;
      }

      old_pnt.x = pnt->x;
      old_pnt.y = pnt->y;
    }
  }
  return status;
}

/**
 * @brief  NGL_Touch_DoTouch
 * @param  None
 * @retval None
 */
int8_t NGL_Touch_DoTouch(void) {
    /* */
    NGL_Page* pg = NGL_GUI_GetSelectedPage();
    /* */
    if ( touch_event_status != 1 || pg == (void*)0 ) {
        return -1;
    }
    /* */
    NGL_GUI_ClickPage( _event_touchPoint, _event_touchType );
    if ( pg->Click != (void*)0 ) {
        pg->Click( _event_touchPoint, _event_touchType );
    }
    /* */
    touch_event_status = -1;
    return 0;
}

/**
 * @brief  NGL_Touch_GetEventStatus
 * @param  None
 * @retval None
 */
int8_t NGL_Touch_GetEventStatus(void) {
  return touch_event_status;
}

/**
 * @brief  NGL_Touch_ResetEventStatus
 * @param  None
 * @retval None
 */
void NGL_Touch_ResetEventStatus(void) {
  touch_event_status = -1;
}

/**
 * @brief  NGL_Touch_Event
 * @param  None
 * @retval None
 */
int8_t NGL_Touch_Event(uint16_t data, NGL_TouchState state) {

  NGL_TouchScreen* touch = NGL_Touch_getTouch();
  Coordinate pnt = {0, 0};
  static Coordinate old_pnt = {0, 0};

  static NGL_TouchType touchtype = NGL_Touch_NoTouch;
  NGL_TouchState _touchState;

  /* work respect touch interface */
  if (touch->Interface == Touch_ADC_Interface) {
    touch_event_status = _touch_adc_event(touch, data, state);
  }
  else if (touch->Interface == Touch_SPI_Interface) {
    touch_event_status = _touch_spi_event(touch, data, state);
  }
  /* data complete */
  if (touch_event_status == 1) { // && (!_touch_hp_filter(&touch->data, FALSE)) ) {
    /* convert point to screen coordinates */
    touch->foops->point(touch->data, &pnt);
    /* If no touch */
    _touchState = touch->state;
    touch->callbacks->HAL_SetState(Touch_Polling);
    /* */
    if (touch->callbacks->HAL_NoTouch() == 0) {
      touchtype = NGL_Touch_NoTouch;
      touch->callbacks->HAL_SetState(Touch_Default);
      /* */
      pnt.x = old_pnt.x;
      pnt.y = old_pnt.y;
    }
    else {
      touch->callbacks->HAL_SetState(_touchState);
      /* if old state is NoTouch */
      if (touchtype == NGL_Touch_NoTouch) {
        touchtype = NGL_Touch_SingleTouch;
      }
      /* if old state is SingleTouch or RepeatTouch */
      else {
          touchtype = NGL_Touch_RepeatTouch;
      }
    }
    /* */
    old_pnt.x = pnt.x;
    old_pnt.y = pnt.y;
    /* Send event signal to aplication */
    touch->callbacks->EventSignal(pnt, touchtype);
    /*  */
    _event_touchPoint.x = pnt.x;
    _event_touchPoint.y = pnt.y;
    _event_touchType = touchtype;
  }
  return touch_event_status;
}

/**
 * @brief  NGL_Touch_ButtonEventFilter
 * @param  None
 * @retval None
 */
int8_t NGL_Touch_ButtonEventFilter(const NGL_Button* btn, NGL_TouchType type) {
  /* Button support click events */
  if ( btn->ClickEvent != (void*)0 ) {
    /* */
    if ( (type == NGL_Touch_SingleTouch) || \
         (type == NGL_Touch_RepeatTouch && btn->ReClickState == ReClick_ENABLE) ) {
      return 0;
    }
  }
  return -1;
}

/**
 * @brief  NGL_Touch_ChexkBoxEventFilter
 * @param  None
 * @retval None
 */
int8_t NGL_Touch_CheckBoxEventFilter(const NGL_CheckBox* box, NGL_TouchType type) {
  /* */
  if (type == NGL_Touch_SingleTouch) {
    return 0;
  }
  return -1;
}

/**
 * @brief  NGL_Touch_SeekBarEventFilter
 * @param  None
 * @retval None
 */
int8_t NGL_Touch_SeekBarEventFilter(const NGL_SeekBar* bar, NGL_TouchType type) {
  return 0;
}
