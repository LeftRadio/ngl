/**
  ******************************************************************************
  * @file		NGL_types.h
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date		2015-02-12-10.50
  * @brief		header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NGL_TYPES_H
#define __NGL_TYPES_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "LCD_GPIO.h"

/* Exported define ------------------------------------------------------------*/
#define NGL_MAX_BUTTONS				20
#define NGL_MAX_LABELS				30
#define NGL_LABEL_MAX_CHARS			30

#define __R61581__                  0
#define __SSD1289__                 1

/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
typedef enum {RESET = 0, SET = !RESET} FlagStatus;

/* --------------------------  HAL/GL elements enums & structs  -------------------------- */
typedef void (*pfunc)(void);
typedef enum { _0_degree, _90_degree, _180_degree, _270_degree } NGL_RotationLCD;
typedef enum { NGL_Vertical, NGL_Horizontal } NGL_VertHoriz_Type;
typedef enum { LCD_FSMC_Connect = 0, LCD_GPIO_Connect = 1 } NGL_HardConnectType;
typedef enum { IN_OBJECT = 0, OUT_OBJECT = !IN_OBJECT } NGL_ClipMode;
typedef enum { CLEAR = 0, DRAW = 1 } NGL_DrawState;
typedef enum { Transparent = 0, NotTransparent = 1 } NGL_TransparentState;
typedef enum { TextButton, ColorFillButton, IconButton } NGL_ButtonType;
typedef enum { ReClick_ENABLE, ReClick_DISABLE } NGL_ReClickState;
typedef enum { ResetButton = 0, SelectedButton = 1 } NGL_ButtonState;

/* LCD object type */
typedef struct
{
	NGL_HardConnectType ConnectionType;
	uint8_t DataBusBits;
	uint8_t ColorBits;
	uint16_t X_Max, Y_Max;
	uint32_t Max_Points;
	uint16_t X_Register, Y_Register;
	NGL_RotationLCD Rotation;

	void (*WriteRAM_Prepare)(void);
	void (*WriteCommand)(uint16_t Command);
	void (*WriteData)(uint16_t Data);
	uint16_t (*ReadData)(void);
	void (*WritePixel)(uint16_t Data);

	void (*ControllerInit)(void);
	void (*SetRotation)(NGL_RotationLCD NewState);
	void (*SetArea)(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1);

	void (*SetCursor)(uint16_t X, uint16_t Y);
	void (*GetPixels)(uint8_t *ReadData, uint16_t NumPixels);

} LCD_Typedef;

/* Font chars table element type */
typedef struct {
	const uint16_t	width;			// Ширина символа
	const uint16_t	start;			// Стартовый индекс на первый байт символа в массиве данных символов
} NGL_CharInfo;

/* NGL Font type */
typedef struct
{
	const uint8_t Height;			// Высота символов
	const uint8_t HeightBytes;		// Высота символов в байтах
	const uint8_t FirstChar;		// Индекс первого символа
	const uint8_t LastChar;			// Индекс последнего символа
	const uint8_t FontSpace;		// Пробел между символами
	const NGL_CharInfo *CharTable;	// Таблица индексов символов
	const uint8_t *CharBitmaps;		// Указатель на массив с данными о символах
} NGL_Font;

/* NGL Image type */
typedef struct
{
   const uint16_t Width;			// Picture Width
   const uint16_t Height;			// Picture Height
   const uint8_t Compressed;		// Copressed flag, 0 - NonCompressed, 1- RLE, 2 - picoJPG

   const uint8_t ColorBits;			// color bit per pixel
   const uint8_t DataBits;			// bitmap data array value bits
   const uint32_t DataArraySize;	// Last index of bitmap data array

   const void *Data;				// data pointer

} NGL_Image;


/* --------------------------------  UI elements structs  -------------------------------- */
/* NGL Label type */
typedef struct
{
	const uint16_t X;					        // Left X coordinate
	const uint16_t Y;					        // Down Y coordinate
	uint16_t Color;						        // Text color
	const NGL_TransparentState Transparent;     // Transparent state
	char* Text;                                 // Text
	const NGL_Font *Font;				        // Label font
} NGL_Label;

/* NGL UI Button type */
typedef struct
{
	const uint16_t X;					    // Left X coordinate
	const uint16_t Y;					    // Down Y coordinate
	const uint16_t Width;				    // Button width (X1 = X0 + Width)
	const uint16_t Height;				    // Button height (Y1 = Y0 + height)
	const NGL_ButtonType Type;			    // Type of button
	uint16_t Color;						    // Forecolor for "Fill" type
	uint16_t SelectedColor;				    // Forecolor selected button for "Fill" type
	const FunctionalState ColorShift;	    // Forecolor shift for "Fill" type
	const NGL_Image *ICO;				    // Icon/bitmap
	const NGL_Font *Font;				    // Font for text
	const int16_t TextOffset_X;			    // Text X offset
	const int16_t TextOffset_Y;			    // Text Y offset
	uint16_t TextColor;					    // Color for text
	char* Text;                             // Text

	const NGL_ReClickState ReClickState;    // Enable or disable repeated clicks events
	FunctionalState Visible;			    // Visible state
	FlagStatus Enabled;					    // Enabled state, if RESET button not active and not request all clicks events

	const pfunc ClickEvent;                 // Click event function pointer
} NGL_Button;

/* NGL UI Grid type */
typedef struct
{
	const uint16_t CenterX;						//
	const uint16_t CenterY;						//
	const uint16_t Width;						//
	const uint16_t Height;						//
	const uint16_t Color;						//
	const FunctionalState  EnableCentralLines;	//

} NGL_Grid;

/* NGL UI FillBar type */
typedef struct
{
	const uint16_t X0, X1, Y0, Y1;
	const NGL_VertHoriz_Type VertHoriz;
	const uint16_t Level_MIN, Level_MAX;
	uint16_t Level;
	uint16_t Color;
} NGL_FillBar;



/* NGL Pages type */
typedef struct
{
	const uint16_t Size[4];				                // [X0, Y0, X1, Y1]
	const FunctionalState ClipState;	                // Clip for Page/Menu
	const uint8_t Clip_ID;     			                // Clip object ID
	const uint16_t BackColor;
	const uint8_t FirstButtonsIndex;     	            // First item index
	const uint8_t ButtonsCount;				            // All items count
	uint8_t SelectedButtonsIndex;		                // Selected item index
	uint8_t OldButtonsIndex;			                // Old selected item index
	uint8_t LabelsCount;			                    // Old selected item index
	const NGL_Button* Buttons[NGL_MAX_BUTTONS];     	// Items pointers array
	const NGL_Label* Labels[NGL_MAX_LABELS];			// Items pointers array
	const bool ExitAllowed;				                // Exit allowed

	const pfunc Draw;
	const pfunc CallBack;
} NGL_Page;


/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/






#endif /* __NGL_TYPES_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
