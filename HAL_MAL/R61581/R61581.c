/**
  ******************************************************************************
  * @file    R61581.c
  * @author  LeftRadio
  * @version V1.2.0
  * @date
  * @brief   Low-lewel R61581 Layer sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "NGL_types.h"
#include "LCD_HAL.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void R61581_WriteRAM_Prepare(void);
static void R61581_Init(void);
static void R61581_SetRotation(NGL_RotationType NewState);
static void R61581_SetArea(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1);
static void R61581_SetCursor(uint16_t X, uint16_t Y);
static void R61581_GetPixels(uint8_t *ReadData, uint16_t NumPixels);

extern void (*NGL_Delay)(uint16_t delay);

/* Private variables ---------------------------------------------------------*/
LCD_Typedef R61581 = {

		LCD_FSMC_Connect,
		16,
		16,
		319,						// X Max
		479,						// Y Max
		153600,						// All points counter (X_Max + 1) * (Y_Max + 1) = 480 * 320 = 153600 points
		0x2A,						// X register
		0x2B,						// Y register
		_0_degree,

		R61581_WriteRAM_Prepare,	// Write RAM preapare
		(void*)0,					// Write command function pointer
		(void*)0,					// Write data function pointer
		(void*)0,					// Read data function pointer
		(void*)0,					// Write pixel function pointer
		R61581_Init,				// Initialization LCD controller
		R61581_SetRotation,
		R61581_SetArea,				// Set out area

		R61581_SetCursor,			// Set position for write cursor
		R61581_GetPixels,			// Read pixels
};


/* Private function  --------------------------------------------------------*/

/**
  * @brief  R61581_WriteRAM_Prepare
            Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  */
void R61581_WriteRAM_Prepare(void)
{
	R61581.WriteCommand(0x2C);
}


/**
  * @brief  R61581_Init
  * @param  None
  * @retval None
  */
void R61581_Init(void)
{
	R61581.WriteCommand(0xFF);
	R61581.WriteCommand(0xFF);
	NGL_Delay(5);
	R61581.WriteCommand(0xFF);
	R61581.WriteCommand(0xFF);
	R61581.WriteCommand(0xFF);
	R61581.WriteCommand(0xFF);
	NGL_Delay(10);
	R61581.WriteCommand(0xB0);
	R61581.WriteData(0x00);
	R61581.WriteCommand(0x11);
	NGL_Delay(150);
	R61581.WriteCommand(0xB3);
	R61581.WriteData(0x02);
	R61581.WriteData(0x00);
	R61581.WriteData(0x00);
	R61581.WriteData(0x00);
	R61581.WriteCommand(0xC0);
	R61581.WriteData(0x13);
	R61581.WriteData(0x3B);//480
	R61581.WriteData(0x00);
	R61581.WriteData(0x02);
	R61581.WriteData(0x00);
	R61581.WriteData(0x01);
	R61581.WriteData(0x00);//NW
	R61581.WriteData(0x43);
	/* Display Timing Setting for Normal Mode */
	R61581.WriteCommand(0xC1);
	R61581.WriteData(0x08);				// 0x08
	R61581.WriteData(0x16);				// 0x16 - 22 clocks per Line
	R61581.WriteData(0x08);				// 0x08  Back Porch Lines Number
	R61581.WriteData(0x08);				// 0x08  Front Porch Lines Number
//	/* Display Timing Setting for Partial Mode */
//	R61581.WriteCommand(0xC2);
//	R61581.WriteData(0x08);				// 0x08
//	R61581.WriteData(0x16);				// 0x16 - 22 clocks per Line
//	R61581.WriteData(0x08);				// 0x08  Back Porch Lines Number
//	R61581.WriteData(0x08);				// 0x08  Front Porch Lines Number
	R61581.WriteCommand(0xC4);
	R61581.WriteData(0x11);
	R61581.WriteData(0x07);
	R61581.WriteData(0x03);
	R61581.WriteData(0x03);
	R61581.WriteCommand(0xC6);
	R61581.WriteData(0x00);
	R61581.WriteCommand(0xC8);//GAMMA
	R61581.WriteData(0x03);
	R61581.WriteData(0x03);
	R61581.WriteData(0x13);
	R61581.WriteData(0x5C);
	R61581.WriteData(0x03);
	R61581.WriteData(0x07);
	R61581.WriteData(0x14);
	R61581.WriteData(0x08);
	R61581.WriteData(0x00);
	R61581.WriteData(0x21);
	R61581.WriteData(0x08);
	R61581.WriteData(0x14);
	R61581.WriteData(0x07);
	R61581.WriteData(0x53);
	R61581.WriteData(0x0C);
	R61581.WriteData(0x13);
	R61581.WriteData(0x03);
	R61581.WriteData(0x03);
	R61581.WriteData(0x21);
	R61581.WriteData(0x00);
	R61581.WriteCommand(0x35);
	R61581.WriteData(0x00);
	R61581.WriteCommand(0x36);
	R61581.WriteData((0x00 | (0x01 << 7) | (0x01 << 6) | (0x01 << 5)));			// register 36h; 5,6,7 bits; see datasheet page 116
	R61581.WriteCommand(0x3A);
	R61581.WriteData(0x55);
	R61581.WriteCommand(0x44);
	R61581.WriteData(0x00);
	R61581.WriteData(0x01);
	R61581.WriteCommand(0xD0);
	R61581.WriteData(0x07);
	R61581.WriteData(0x07);//VCI1
	R61581.WriteData(0x1D);//VRH
	R61581.WriteData(0x03);//BT
	R61581.WriteCommand(0xD1);
	R61581.WriteData(0x03);
	R61581.WriteData(0x30);//VCM
	R61581.WriteData(0x10);//VDV
	R61581.WriteCommand(0xD2);
	R61581.WriteData(0x03);
	R61581.WriteData(0x14);
	R61581.WriteData(0x04);
	R61581.WriteCommand(0x29);
	NGL_Delay(30);
	R61581.WriteCommand(0x2A);
	R61581.WriteData(0x00);
	R61581.WriteData(0x00);
	R61581.WriteData(0x01);
	R61581.WriteData(0x3F);//320
	R61581.WriteCommand(0x2B);
	R61581.WriteData(0x00);
	R61581.WriteData(0x00);
	R61581.WriteData(0x01);
	R61581.WriteData(0xDF);//480
	R61581.WriteCommand(0xB4);
	R61581.WriteData(0x00);

	NGL_Delay(100);
	R61581.WriteCommand(0x2C);
}


/**
  * @brief  R61581_SetRotation
  * @param  NGL_RotationType NewState - new rotation state
  * @retval None
  */
void R61581_SetRotation(NGL_RotationType NewState)
{
		switch(NewState)
		{
			case _0_degree:

				R61581.WriteCommand(0x36);
				R61581.WriteData(0x00 | (0x01 << 7));

				R61581.X_Register = 0x2A;
				R61581.Y_Register = 0x2B;

				break;

			case _90_degree:

				R61581.WriteCommand(0x36);
				R61581.WriteData(0x00 | (0x01 << 7) | (0x01 << 6) | (0x01 << 5));

				R61581.X_Register = 0x2A;
				R61581.Y_Register = 0x2B;

				break;

			case _180_degree:


				break;

			case _270_degree:


				break;

			default: break;
		}

		R61581_SetArea(0, 0, R61581.X_Max, R61581.Y_Max);
}


/**
  * @brief  R61581_SetCursor
  * @param  uint16_t X, Y
  * @retval None
  */
void R61581_SetCursor(uint16_t X, uint16_t Y)
{
	R61581.WriteCommand(R61581.X_Register);
	R61581.WriteData((uint8_t)(X >> 8)); 	/* Colum start 	SC[8] */
	R61581.WriteData((uint8_t)X); 			/* Colum start	SC[7:0] */

	R61581.WriteCommand(R61581.Y_Register);
	R61581.WriteData((uint8_t)(Y >> 8)); 	/* Row start 	SR[8] */
	R61581.WriteData((uint8_t)Y); 			/* Row start 	SR[7:0] */

	R61581.WriteCommand(0x2C);
}


/**
  * @brief  R61581_SetArea
  * @param  uint16_t X0, Y0, X1, Y1
  * @retval None
  */
void R61581_SetArea(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1)
{
	R61581.WriteCommand(R61581.X_Register);
	R61581.WriteData(X0 >> 8); 	/* Colum start 	SC[8] */
	R61581.WriteData(X0); 		/* Colum start	SC[7:0] */
	R61581.WriteData(X1 >> 8); 	/* Colum end 	EC[8] */
	R61581.WriteData(X1); 		/* Colum end 	EC[7:0] */

	R61581.WriteCommand(R61581.Y_Register);
	R61581.WriteData(Y0 >> 8); 	/* Row start 	SR[8] */
	R61581.WriteData(Y0); 		/* Row start 	SR[7:0] */
	R61581.WriteData(Y1 >> 8);	/* Row end 		ER[8] */
	R61581.WriteData(Y1); 		/* Row end 		ER[7:0] */

	R61581.WriteCommand(0x2C);
}


/**
  * @brief  R61581_GetPixels
  * @param
  * @retval None
  */
void R61581_GetPixels(uint8_t *ReadData, uint16_t NumPixels)
{
	uint16_t i;
	volatile uint16_t dummy;

	/* Write 16-bit Index (then Read Reg) */
	R61581.WriteCommand(0x2E);					// Select GRAM Reg
	dummy = R61581.ReadData();

	for(i = 0; i < NumPixels/2; i++)
	{
		dummy = R61581.ReadData();
		(*ReadData) = (uint8_t)(dummy >> 10);					// RED First Pixel
		(*(ReadData+1)) = (uint8_t)((dummy & 0x00FF) >> 2);		// GREEN First Pixel

		dummy = R61581.ReadData();
		(*(ReadData+2)) = (uint8_t)(dummy >> 10);				// BLUE First Pixel
		(*(ReadData+3)) = (uint8_t)((dummy & 0x00FF) >> 2);		// RED Second Pixel

		dummy = R61581.ReadData();
		(*(ReadData+4)) = (uint8_t)(dummy >> 10);				// GREEN Second Pixel
		(*(ReadData+5)) = (uint8_t)((dummy & 0x00FF) >> 2);		// BLUE Second Pixelv

		ReadData += 6;
	}
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
