/**
  *****************************************************************************
  * @file    page.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  *****************************************************************************
**/

/* Includes -----------------------------------------------------------------*/
#include <string.h>
#include "NGL.h"
#include "touch_events.h"

/* Private typedef ----------------------------------------------------------*/
typedef void (*__pDrawOpbject)(const void* object);

/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
NGL_Page *NGL_GUI_SelectedPage = (void*)0;

/* Private function prototypes ----------------------------------------------*/
extern void NGL_GUI_ButtonEvent(NGL_Button* button, NGL_TouchType eventType, int* evnentData);
extern void NGL_GUI_SeekBarEvent(NGL_SeekBar *bar, Coordinate point, NGL_TouchType eventType, int* evnentData);
extern void NGL_GUI_CheckBoxEvent(NGL_CheckBox* box, NGL_TouchType eventType, int* evnentData);

/* Private function  --------------------------------------------------------*/

/**
  * @brief  NGL_SetActivePage
  * @param
  * @retval None
  */
void NGL_GUI_SelectPage(const NGL_Page *page)
{
    NGL_GUI_SelectedPage = (NGL_Page*)page;
}

/**
  * @brief  NGL_GUI_GetSelectedPage
  * @param
  * @retval None
  */
NGL_Page* NGL_GUI_GetSelectedPage(void)
{
    return NGL_GUI_SelectedPage;
}

/**
  * @brief  NGL_GUI_DrawPageObjects
  * @param
  * @retval None
  */
static void _drawObjects(void **objects, uint16_t cnt, __pDrawOpbject drawFunc)
{
  int i;
  for(i = 0; i < cnt; i++) {
        drawFunc(objects[i]);
  }
}

/**
  * @brief  NGL_GUI_DrawPageObjects
  * @param
  * @retval None
  */
void NGL_GUI_DrawPageObjects(void)
{
    NGL_Page* pg = NGL_GUI_SelectedPage;

    _drawObjects((void**)pg->Objects.buttons, pg->Objects.btn_count, NGL_GUI_DrawButton);
    _drawObjects((void**)pg->Objects.lables, pg->Objects.lbl_count, NGL_GUI_DrawLabel);
    _drawObjects((void**)pg->Objects.fillbars, pg->Objects.flb_count, NGL_GUI_DrawFillBar);
    _drawObjects((void**)pg->Objects.seekbars, pg->Objects.skb_count, NGL_GUI_DrawSeekBar);
    _drawObjects((void**)pg->Objects.graphscales, pg->Objects.grf_count, NGL_GUI_DrawGraphScale);
    _drawObjects((void**)pg->Objects.checkboxs, pg->Objects.chxb_count, NGL_GUI_DrawCheckBox);
}

/**
  * @brief  NGL_GUI_ClickPage
  * @param
  * @retval None
  */
int NGL_GUI_ClickPage(Coordinate point, NGL_TouchType type)
{
    NGL_Page* pg = NGL_GUI_SelectedPage;
    uint16_t X = point.x;
    uint16_t Y = point.y;
    uint16_t X0, Y0, X1, Y1;
    int i;
    static int btn_i = -1, chk_i = -1, sbr_i = -1;

    // No search if already 'locked' object
    if (btn_i >= 0) {

        if(NGL_Touch_ButtonEventFilter(pg->Objects.buttons[btn_i], type) == 0) {

          NGL_GUI_ButtonEvent((NGL_Button*)pg->Objects.buttons[btn_i], type, (void*)0);
          pg->Objects.buttons[btn_i]->ClickEvent();

          if(type != NGL_Touch_NoTouch) {
            return btn_i;
          }
        }
    }
    else if( chk_i == -1 && sbr_i == -1 ) {

      if(pg->Objects.buttons != (void*)0) {

        for(i = 0; i < pg->Objects.btn_count; i++) {

          if(NGL_Touch_ButtonEventFilter(pg->Objects.buttons[i], type) == 0) {

              X0 = pg->Objects.buttons[i]->X;
              X1 = pg->Objects.buttons[i]->X + pg->Objects.buttons[i]->Width;
              Y0 = pg->Objects.buttons[i]->Y;
              Y1 = pg->Objects.buttons[i]->Y + pg->Objects.buttons[i]->Height;

              if(X > X0 && X < X1 && Y > Y0 && Y < Y1) {
                NGL_GUI_ButtonEvent((NGL_Button*)pg->Objects.buttons[i], type, (void*)0);
                pg->Objects.buttons[i]->ClickEvent();
                btn_i = i;
                return i;
              }
          }
        }
      }
    }
    btn_i = -1;

    // No search if already 'locked' object
    if (chk_i >= 0) {

        if( !NGL_Touch_CheckBoxEventFilter((void*)0, type) ) {

          NGL_GUI_CheckBoxEvent((NGL_CheckBox*)pg->Objects.checkboxs[chk_i], type, (void*)0);
          pg->Objects.checkboxs[chk_i]->ClickEvent();

          if(type != NGL_Touch_NoTouch) {
            return chk_i;
          }
        }
    }
    else if( btn_i == -1 && sbr_i == -1 ) {

      if( pg->Objects.checkboxs != (void*)0 ) {

        for(i = 0; i < pg->Objects.chxb_count; i++) {

          if( !NGL_Touch_CheckBoxEventFilter((void*)0, type) ) {

            if( pg->Objects.checkboxs[i]->ClickEvent != (void*)0 ) {

              X0 = pg->Objects.checkboxs[i]->X0;
              X1 = pg->Objects.checkboxs[i]->X1;
              Y0 = pg->Objects.checkboxs[i]->Y0;
              Y1 = pg->Objects.checkboxs[i]->Y1;

              if( X > X0 && X < X1 && Y > Y0 && Y < Y1 ) {
                NGL_GUI_CheckBoxEvent((NGL_CheckBox*)pg->Objects.checkboxs[i], type, (void*)0);
                pg->Objects.checkboxs[i]->ClickEvent();
                chk_i = i;
                return i;
              }
            }
          }
        }
      }
    }
    chk_i = -1;

    // No search if already 'locked' object
    if (sbr_i >= 0) {

        NGL_GUI_SeekBarEvent((NGL_SeekBar*)pg->Objects.seekbars[sbr_i], point, type, (void*)0);
        pg->Objects.seekbars[sbr_i]->ClickEvent();

        if(type != NGL_Touch_NoTouch) {
          return sbr_i;
        }
    }
    else if( btn_i == -1 && chk_i == -1 ) {

      if(pg->Objects.seekbars != (void*)0) {

        for(i = 0; i < pg->Objects.skb_count; i++) {

          if(pg->Objects.seekbars[i]->ClickEvent != (void*)0) {

            X0 = pg->Objects.seekbars[i]->X0;
            X1 = pg->Objects.seekbars[i]->X1;
            Y0 = pg->Objects.seekbars[i]->Y0;
            Y1 = pg->Objects.seekbars[i]->Y1;

            if(X > X0 && X < X1 && Y > Y0 && Y < Y1) {
              NGL_GUI_SeekBarEvent((NGL_SeekBar*)pg->Objects.seekbars[i], point, type, (void*)0);
              pg->Objects.seekbars[i]->ClickEvent();
              sbr_i = i;
              return i;
            }
          }
        }
      }
    }

    sbr_i = -1;

    return -1;
}


/*******************************************************************************
      END FILE
*******************************************************************************/