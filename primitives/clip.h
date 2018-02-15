/**
  ******************************************************************************
  * @file       clip.h
  * @author     LeftRadio
  * @version    V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

#ifndef __CLIP_H
#define __CLIP_H

/* Includes ------------------------------------------------------------------*/
/* Exported typedef -----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function --------------------------------------------------------*/
extern ngl_status NGL_GP_NewClipObject(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, NGL_ClipType clipType, uint8_t NumInd);
extern ngl_status NGL_GP_ClipClearObject(uint8_t NumInd);

extern ngl_flag NGL_GP_ClipPoint(uint16_t X0, uint16_t Y0);
extern ngl_flag NGL_GP_ClipLine(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1);



#endif /* __CLIP_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
