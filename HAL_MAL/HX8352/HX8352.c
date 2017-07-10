/**
  ******************************************************************************
  * @file		     HX8352.c
  * @author		Neil Lab :: Left Radio
  * @version	v1.0.0
  * @date		2015-03-10-22.27
  * @brief		HAL sourse for HX8352 LCD controller
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "NGL_types.h"
#include "LCD_HAL.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void HX8352_WriteRAM_Prepare(void);
static void HX8352_Init(void);
static void HX8352_SetRotation(NGL_RotationLCD NewState);
static void HX8352_SetArea(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1);
static void HX8352_SetCursor(uint16_t X, uint16_t Y);
static void HX8352_GetPixels(uint8_t *ReadData, uint16_t NumPixels);

/* Private variables ---------------------------------------------------------*/
NGL_HAL_Typedef HX8352 =
{

    LCD_GPIO_Connect,  // помним что у нас GPIO
    8,                             // 8-ми битная шина данных
    18,                           // цвет 18 бит на точку
    399,                         // максимум по Х для "родной" ореинтации ЖК
    239,                         // максимум по Y для "родной" ореинтации ЖК
    96000,                      // количество точек (X_Max + 1) * (Y_Max + 1) = 240 * 400 = 96000
    0x06,                    // регистр установки курсора по X
    0x02,                     // регистр установки курсора по Y
    _0_degree,               // поворот ЖК 0 гр.

    HX8352_WriteRAM_Prepare,    // указатель функции внутренней подготовки ЖК к записи в буферную память, ее мы реализуем позже
    (void*)0,                    // указатель на функцию записи команды в ЖК
    (void*)0,                    // указатель на функцию записи данных в ЖК
    (void*)0,                    // указатель на функцию чтения данных из ЖК
    (void*)0,                    // указатель на функцию записи точки
    HX8352_Init,                // указатель на функцию инициализации контроллера ЖК
    HX8352_SetRotation,   // указатель на функцию поворота ЖК
    HX8352_SetArea,           // указатель на функцию установки области для вывода на ЖК

    HX8352_SetCursor,           // указатель на функцию установки курсора ЖК
    HX8352_GetPixels,           // указатель на функцию чтения из ЖК
};


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  HX8352_WriteRAM_Prepare
            Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  */
void HX8352_WriteRAM_Prepare(void)
{
    HX8352.WriteCommand(0x22);
}


/**
  * @brief  HX8352_Init
  * @param  None
  * @retval None
  */
void HX8352_Init(void)
{
    NGL_Delay(50);  /* delay 50 ms */

    CS_LCD_set;

    NGL_HAL_WriteRegister(0x83,0x02); // TESTM=1
    NGL_HAL_WriteRegister(0x85,0x03); // VDC_SEL=011.
    NGL_HAL_WriteRegister(0x8C,0x93); // STBA[7]=1, STBA[5:4]=01, STBA[1:0]=11
    NGL_HAL_WriteRegister(0x91,0x01); // DCDC_SYNC=1
    NGL_HAL_WriteRegister(0x83,0x00); // TESTM=0
    //--------------------------------------------------------------------------------
    // Gamma Setting
    NGL_HAL_WriteRegister(0x3E,0xB0);
    NGL_HAL_WriteRegister(0x3F,0x03);
    NGL_HAL_WriteRegister(0x40,0x10);
    NGL_HAL_WriteRegister(0x41,0x56);
    NGL_HAL_WriteRegister(0x42,0x13);
    NGL_HAL_WriteRegister(0x43,0x46);
    NGL_HAL_WriteRegister(0x44,0x23);
    NGL_HAL_WriteRegister(0x45,0x76);
    NGL_HAL_WriteRegister(0x46,0x4F);
    NGL_HAL_WriteRegister(0x47,0x5F);
    NGL_HAL_WriteRegister(0x48,0x4F);
    NGL_HAL_WriteRegister(0x49,0x4F);
    //--------------------------------------------------------------------------------
    // Power Supply Setting
    NGL_HAL_WriteRegister(0x17,0x61); // RADJ=0110(85%), OSC_EN=1
    NGL_HAL_WriteRegister(0x2B,0xF9); // N_DCDC=0xF9.
    NGL_Delay(10);
    NGL_HAL_WriteRegister(0x1B,0x14); // BT=0001, AP=100
    NGL_HAL_WriteRegister(0x1A,0x11); // VC3=001, VC1=001
    NGL_HAL_WriteRegister(0x1C,0x0d); // VRH=1101
    NGL_HAL_WriteRegister(0x1F,0x3D); // VCM=100_0010
    NGL_Delay(20);
    NGL_HAL_WriteRegister(0x19,0x0A); // GASENB=0, PON=0, DK=1, XDK=0,
    // VLCD_TRI=1, STB=0
    NGL_HAL_WriteRegister(0x19,0x1A); // GASENB=0, PON=1, DK=1, XDK=0,
    // VLCD_TRI=1, STB=0
    NGL_Delay(40);
    NGL_HAL_WriteRegister(0x19,0x12); // GASENB=0, PON=1, DK=0, XDK=0,
    // VLCD_TRI=1, STB=0
    NGL_Delay(40);
    NGL_HAL_WriteRegister(0x1E,0x20); // VCOMG=1, VDV=0_1100
    NGL_Delay(50);
    //--------------------------------------------------------------------------------
    // Display ON Setting
    NGL_HAL_WriteRegister(0x3C,0x60); // N_SAP=0110 000
    NGL_HAL_WriteRegister(0x3D,0x40); // I_SAP =0100 0000
    NGL_HAL_WriteRegister(0x34,0x38); // EQS=0011 1000
    NGL_HAL_WriteRegister(0x35,0x38); // EQP=0011 1000
    NGL_HAL_WriteRegister(0x24,0x38); // GON=1, DTE=1, D=10
    NGL_Delay(40);
    NGL_HAL_WriteRegister(0x24,0x3C); // GON=1, DTE=1, D=11
    NGL_HAL_WriteRegister(0x16,0x1C); // BGR=0
    NGL_HAL_WriteRegister(0x01,0x06); // INVON=0, NORNO=1
    NGL_HAL_WriteRegister(0x55,0x00);
    //--------------------------------------------------------------------------------
    NGL_HAL_WriteRegister(0x02,0x00); // Column address start2
    NGL_HAL_WriteRegister(0x03,0x00); // Column address start1
    NGL_HAL_WriteRegister(0x04,0x00); // Column address end2
    NGL_HAL_WriteRegister(0x05,0xEF); // Column address end1
    NGL_HAL_WriteRegister(0x06,0x00); // Row address start2
    NGL_HAL_WriteRegister(0x07,0x00); // Row address start1
    NGL_HAL_WriteRegister(0x08,0x01); // Row address end2
    NGL_HAL_WriteRegister(0x09,0x8F); // Row address end1

    HX8352.WriteCommand(0x22);

    CS_LCD_clr;

    NGL_Delay(50);  /* delay 50 ms */

}


/**
  * @brief  HX8352_SetRotation
  * @param  NGL_RotationLCD NewState - new rotation state
  * @retval None
  */
void HX8352_SetRotation(NGL_RotationLCD NewState)
{
    CS_LCD_set;

    switch(NewState)
    {
    case _0_degree:


        break;

    case _90_degree:


        break;

    case _180_degree:


        break;

    case _270_degree:


        break;

    default:
        break;
    }

    CS_LCD_clr;
}


/**
  * @brief  LCD_SetCursor
  * @param  uint16_t X, Y
  * @retval None
  */
void HX8352_SetCursor(uint16_t X, uint16_t Y)
{
    CS_LCD_set;

    NGL_HAL_WriteRegister(HX8352.X_Register, X >> 8); /* Row */
    NGL_HAL_WriteRegister(HX8352.X_Register + 1, (uint8_t)X); /* Row */

    NGL_HAL_WriteRegister(HX8352.Y_Register, Y >> 8); /* Line */
    NGL_HAL_WriteRegister(HX8352.Y_Register + 1, (uint8_t)Y); /* Line */

    HX8352.WriteCommand(0x0022);

    CS_LCD_clr;
}


/**
  * @brief  HX8352_SetArea
  * @param  uint16_t X0, Y0, X1, Y1
  * @retval None
  */
void HX8352_SetArea(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1)
{
	CS_LCD_set;

	NGL_HAL_WriteRegister(0x02,Y0 >> 8); // Column address start2
	NGL_HAL_WriteRegister(0x03,Y0);   // Column address start1
	NGL_HAL_WriteRegister(0x04,Y1 >> 8); // Column address end2
	NGL_HAL_WriteRegister(0x05,Y1);   // Column address end1
	NGL_HAL_WriteRegister(0x06,X0 >> 8); // Row address start2
	NGL_HAL_WriteRegister(0x07,X0); // Row address start1
	NGL_HAL_WriteRegister(0x08,X1 >> 8); // Row address end2
	NGL_HAL_WriteRegister(0x09,X1); // Row address end1

	HX8352.WriteCommand(0x22);

	CS_LCD_clr;
}


/**
  * @brief  HX8352_GetPixels
  * @param
  * @retval None
  */
void HX8352_GetPixels(uint8_t *ReadData, uint16_t NumPixels)
{

}










/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
