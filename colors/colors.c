/**
  ******************************************************************************
  * @file		   colors.c
  * @author		 Neil Lab :: Left Radio
  * @version	 v1.2.0
  * @date		   2015-02-13-15.43
  * @brief		 sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "NGL_types.h"
#include "colors.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint16_t TextColor;
volatile uint16_t BackColor;
volatile uint16_t GraphicsColor;

/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LCD_SetTextColor
  * @param  color
  * @retval None
  */
void NGL_Color_SetTextColor(uint16_t color)
{
	TextColor = color;
}


/**
  * @brief  LCD_GetTextColor
  * @param  None
  * @retval TextColor
  */
uint16_t NGL_Color_GetTextColor(void)
{
	return TextColor;
}


/**
  * @brief  LCD_SetGraphicsColor
  * @param  color
  * @retval None
  */
void NGL_Color_SetGraphicsColor(uint16_t color)
{
	GraphicsColor = color;
}


/**
  * @brief  LCD_GetGraphicsColor
  * @param  None
  * @retval GraphicsColor
  */
uint16_t NGL_Color_GetGraphicsColor(void)
{
	return GraphicsColor;
}


/**
  * @brief  LCD_SetBackColor
  * @param  color
  * @retval None
  */
void NGL_Color_SetBackColor(uint16_t color)
{
	BackColor = color;
}


/**
  * @brief  LCD_GetBackColor
  * @param  None
  * @retval BackColor
  */
uint16_t NGL_Color_GetBackColor(void)
{
	return BackColor;
}


/**
  * @brief  Color_ChangeBrightness
  * @param
  * @retval None
  */
uint16_t NGL_Color_SetBrightness(uint16_t ColorIn, int8_t level)
{
	int32_t R = 0, G = 0, B = 0;

  if(level == 0) {
      return ColorIn;
  }

  level = nMIN( nMAX(-32, level), 32 );

	R = ((ColorIn & 0xF800) >> 11) + (level);
	G = ((ColorIn & 0x07E0) >> 5) + (level * 2);
	B = (ColorIn & 0x001F) + (level);

	if(R < 0) R = 0;
	else if(R > 31) R = 31;

	if(G < 0) G = 0;
	else if(G > 63) G = 63;

	if(B < 0) B = 0;
	else if(B > 31) B = 31;

	return (uint16_t)((R << 11) | (G << 5) | (B));
}








/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
