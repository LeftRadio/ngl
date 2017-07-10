/**
  ******************************************************************************
  * @file    SSD1289.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   Low-lewel SSD1289 Layer sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "NGL_types.h"
#include "LCD_HAL.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SSD1289_WriteRAM_Prepare(void);
static void SSD1289_Init(void);
static void SSD1289_SetRotation(NGL_RotationLCD NewState);
static void SSD1289_SetArea(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1);
static void SSD1289_SetCursor(uint16_t X, uint16_t Y);
static void SSD1289_GetPixels(uint8_t *ReadData, uint16_t NumPixels);


/* Private variables ---------------------------------------------------------*/
NGL_HAL_Typedef SSD1289 = {

		LCD_GPIO_Connect,
		16,							// Data bus bits
		16,							// LCD color bits
		239,						// X Max
		319,						// Y Max
		76800,						// All points counter (X_Max + 1) * (Y_Max + 1)
		0x004e,						// X register
		0x004f,						// Y register
		_0_degree,

		SSD1289_WriteRAM_Prepare,	// Write RAM preapare function
		(void*)0,					// Write command function pointer
		(void*)0,					// Write data function pointer
		(void*)0,					// Read data function pointer
		(void*)0,					// Write pixel function pointer
		SSD1289_Init,				// Initialization LCD controller
		SSD1289_SetRotation,
		SSD1289_SetArea,			// Set out area

		SSD1289_SetCursor,			// Set position for write cursor
		SSD1289_GetPixels,			// Read pixels
};


/* Private function  --------------------------------------------------------*/

/**
  * @brief  SSD1289_WriteRAM_Prepare
            Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  */
void SSD1289_WriteRAM_Prepare(void)
{
	SSD1289.WriteCommand(0x0022);
}


/**
  * @brief  SSD1289_Init
  * @param  None
  * @retval None
  */
void SSD1289_Init(void)
{
	NGL_Delay(50);  /* delay 50 ms */

	CS_LCD_set;

	NGL_HAL_WriteRegister(0x0000,0x0001);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0003,0xA8A4);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x000C,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x000D,0x080C);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x000E,0x2B00);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x001E,0x00B0);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0001,0x293F | (0 << 10) | (0 << 14) | (0 << 9));	// SM(10) = 0, RL(14) = 0,  TB(9) = 0	// 0 degree
	NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0002,0x0600);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0010,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0011,0x6040 | (1 << 3) | (1 << 5) | (1 << 4));		// AM(3) = 1, ID[1..0](5, 4) = 11
	NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0005,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0006,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0016,0xEF1C);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0017,0x0003);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0007,0x0133);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x000B,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x000F,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0041,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0042,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0048,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0049,0x013F);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x004A,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x004B,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0044,0xEF00);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0045,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0046,0x013F);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0030,0x0707);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0031,0x0204);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0032,0x0204);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0033,0x0502);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0034,0x0507);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0035,0x0204);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0036,0x0204);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0037,0x0502);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x003A,0x0302);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x003B,0x0302);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0023,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0024,0x0000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x0025,0x8000);    NGL_Delay(5);
	NGL_HAL_WriteRegister(0x004f,0);
	NGL_HAL_WriteRegister(0x004e,0);

	SSD1289.WriteCommand(0x0022);

	CS_LCD_clr;

	NGL_Delay(50);  /* delay 50 ms */
}


/**
  * @brief  SSD1289_SetRotation
  * @param  NGL_RotationLCD NewState - new rotation state
  * @retval None
  */
void SSD1289_SetRotation(NGL_RotationLCD NewState)
{
	CS_LCD_set;

	switch(NewState)
	{
		case _0_degree:

			NGL_HAL_WriteRegister(0x0001,0x293F | (0 << 10) | (0 << 14) | (0 << 9));	// SM(10) = 0, RL(14) = 0,  TB(9) = 0	// 0 degree
			NGL_Delay(5);
			NGL_HAL_WriteRegister(0x0011,0x6040 | (0 << 3) | (1 << 5) | (1 << 4));		// AM(3) = 1, ID[1..0](5, 4) = 11
			NGL_Delay(5);

			SSD1289.X_Register = 0x004e;
			SSD1289.Y_Register = 0x004f;

			break;

		case _90_degree:

			NGL_HAL_WriteRegister(0x0001,0x293F | (0 << 10) | (0 << 14) | (1 << 9));	// SM(10) = 0, RL(14) = 0,  TB(9) = 1	// 90 degree
			NGL_Delay(5);
			NGL_HAL_WriteRegister(0x0011,0x6040 | (1 << 3) | (1 << 5) | (1 << 4));		// AM(3) = 1, ID[1..0](5, 4) = 11
			NGL_Delay(5);

			SSD1289.X_Register = 0x004f;
			SSD1289.Y_Register = 0x004e;

			break;

		case _180_degree:

			NGL_HAL_WriteRegister(0x0001,0x293F | (0 << 10) | (1 << 14) | (1 << 9));	// SM(10) = 0, RL(14) = 1,  TB(9) = 1	// 180 degree
			NGL_Delay(5);
			NGL_HAL_WriteRegister(0x0011,0x6040 | (0 << 3) | (1 << 5) | (1 << 4));		// AM(3) = 1, ID[1..0](5, 4) = 11
			NGL_Delay(5);

			SSD1289.X_Register = 0x004e;
			SSD1289.Y_Register = 0x004f;

			break;

		case _270_degree:

			NGL_HAL_WriteRegister(0x0001,0x293F | (0 << 10) | (1 << 14) | (0 << 9));	// SM(10) = 0, RL(14) = 1,  TB(9) = 0	// 270 degree
			NGL_Delay(5);
			NGL_HAL_WriteRegister(0x0011,0x6040 | (1 << 3) | (1 << 5) | (1 << 4));		// AM(3) = 1, ID[1..0](5, 4) = 11
			NGL_Delay(5);

			SSD1289.X_Register = 0x004f;
			SSD1289.Y_Register = 0x004e;

			break;

		default: break;
	}

	CS_LCD_clr;
}


/**
  * @brief  LCD_SetCursor
  * @param  uint16_t X, Y
  * @retval None
  */
void SSD1289_SetCursor(uint16_t X, uint16_t Y)
{
	NGL_HAL_WriteRegister(SSD1289.X_Register, X); /* Row */
	NGL_HAL_WriteRegister(SSD1289.Y_Register, Y); /* Line */
	SSD1289.WriteCommand(0x0022);
}


/**
  * @brief  SSD1289_SetArea
  * @param  uint16_t X0, Y0, X1, Y1
  * @retval None
  */
void SSD1289_SetArea(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1)
{
	uint16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;

	CS_LCD_set;

	NGL_HAL_WriteRegister(SSD1289.X_Register, X0); /* Row */
	NGL_HAL_WriteRegister(SSD1289.Y_Register, Y0); /* Line */

	if((SSD1289.Rotation == _90_degree) || (SSD1289.Rotation == _270_degree))
	{
		x0 = Y0, y0 = X0, x1 = Y1, y1 = X1;
	}

	NGL_HAL_WriteRegister(0x0044, x0 | (x1 << 8));	// Horizontal RAM address position, POR - 0xFE00
	NGL_HAL_WriteRegister(0x0045, y0);				// Vertical RAM	address start position, POR - 0x0000
	NGL_HAL_WriteRegister(0x0046, y1);				// Vertical RAM	address end position, POR - 0x013F

	SSD1289.WriteCommand(0x0022);
	CS_LCD_clr;
}


/**
  * @brief  SSD1289_GetPixels
  * @param
  * @retval None
  */
void SSD1289_GetPixels(uint8_t *ReadData, uint16_t NumPixels)
{

}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
