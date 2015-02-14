/**
  ******************************************************************************
  * @file    LCD_GPIO.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

#ifndef __LCD_GPIO_H
#define __LCD_GPIO_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported typedef ----------------------------------------------------------*/
/* GPIO_TypeDef */
typedef struct
{
  volatile uint32_t CRL;
  volatile uint32_t CRH;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t BRR;
  volatile uint32_t LCKR;
} GPIO_TypeDef;

/* LCD_Pin_TypeDef */
typedef struct
{
  GPIO_TypeDef* GPIOx;
  uint16_t Pin;
} LCD_Pin_TypeDef;

/* LCD_GPIO_DATAPort_TypeDef */
typedef struct
{
  GPIO_TypeDef* GPIOx;

} LCD_GPIO_DataPort_TypeDef;


/* Exported define -----------------------------------------------------------*/
#define LCD_FMSC_COMMAND           (*((volatile uint16_t *) 0x60000000))		/* RS = 0 */
#define LCD_FMSC_DATA              (*((volatile uint16_t *) 0x60020000))		/* RS = 1 */

#define LCD_GPIO_DATA(data)    LCD_GPIO_DataPort.GPIOx->BRR = LCD_GPIO_portMask; LCD_GPIO_DataPort.GPIOx->BSRR = data;	//

#define RES_LCD_set			RES_LCD.GPIOx->BSRR = RES_LCD.Pin;
#define RES_LCD_clr			RES_LCD.GPIOx->BSRR = RES_LCD.Pin << 16;

#define RD_LCD_set			RD_LCD.GPIOx->BSRR = RD_LCD.Pin;
#define RD_LCD_clr			RD_LCD.GPIOx->BSRR = RD_LCD.Pin << 16;

#define RS_LCD_set			RS_LCD.GPIOx->BSRR = RS_LCD.Pin;
#define RS_LCD_clr			RS_LCD.GPIOx->BSRR = RS_LCD.Pin << 16;

#define WR_LCD_set			WR_LCD.GPIOx->BSRR = WR_LCD.Pin;
#define WR_LCD_clr			WR_LCD.GPIOx->BSRR = WR_LCD.Pin << 16;

#define CS_LCD_clr			CS_LCD.GPIOx->BSRR = CS_LCD.Pin;
#define CS_LCD_set			CS_LCD.GPIOx->BSRR = CS_LCD.Pin << 16;

#define HC573_LE_set		HC573_LE.GPIOx->BSRR = HC573_LE.Pin;
#define HC573_LE_clr		HC573_LE.GPIOx->BSRR = HC573_LE.Pin << 16;

/* Exported variables --------------------------------------------------------*/
extern uint16_t LCD_GPIO_portMask;
extern LCD_Pin_TypeDef RES_LCD, RD_LCD, RS_LCD, WR_LCD, CS_LCD, HC573_LE;
extern LCD_GPIO_DataPort_TypeDef LCD_GPIO_DataPort;


/* Exported function ---------------------------------------------------------*/
void LCD_Set_RESET_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
void LCD_Set_RD_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
void LCD_Set_RS_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
void LCD_Set_WR_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
void LCD_Set_CS_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
void LCD_SetDataPort(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pins);



#endif /* __LCD_GPIO_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
