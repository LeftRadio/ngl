/**
  ******************************************************************************
  * @file    LCD_GPIO.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "LCD_GPIO.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t LCD_GPIO_portMask = 0xFFFF;

LCD_Pin_TypeDef RES_LCD, RD_LCD, RS_LCD, WR_LCD, CS_LCD, HC573_LE, TE_Input;
LCD_GPIO_DataPort_TypeDef LCD_GPIO_DataPort;


/* Private function prototypes -----------------------------------------------*/
/* Private function  --------------------------------------------------------*/

/* Functions for init LCD control signals and data port in GPIO mode */
void LCD_Set_RESET_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
	RES_LCD.GPIOx = GPIOx;
	RES_LCD.Pin = Pin;
}


void LCD_Set_RD_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
	RD_LCD.GPIOx = GPIOx;
	RD_LCD.Pin = Pin;
}


void LCD_Set_RS_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
	RS_LCD.GPIOx = GPIOx;
	RS_LCD.Pin = Pin;
}


void LCD_Set_WR_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
	WR_LCD.GPIOx = GPIOx;
	WR_LCD.Pin = Pin;
}


void LCD_Set_CS_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
	CS_LCD.GPIOx = GPIOx;
	CS_LCD.Pin = Pin;
}


void LCD_SetDataPort(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pins)
{
	LCD_GPIO_DataPort.GPIOx = GPIOx;
	LCD_GPIO_portMask = GPIO_Pins;
}



