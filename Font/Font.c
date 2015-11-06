/**
  ******************************************************************************
  * @file    Font.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date    2015-02-12-10.50
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <string.h>

#include "NGL_types.h"
#include "LCD_HAL.h"
#include "LCD_MAL.h"
#include "Font.h"
#include "colors.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CHAR_WIDTH(c)        nfont->CharTable[(c) - nfont->FirstChar].width
#define CHAR_HEIGHT(c)       nfont->CharTable[(c) - nfont->FirstChar].height
#define CHAR_START_INDEX(c)  nfont->CharTable[(c) - nfont->FirstChar].start
#define CHAR_DATA(c)         &nfont->CharBitmaps[CHAR_START_INDEX(c)]
#define CHAR_SPACE           nfont->FontSpace

/* Private variables ---------------------------------------------------------*/
NGL_Font *nfont = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private function  --------------------------------------------------------*/

/**
  * @brief  NGL_Font_SetFont
  * @param
  * @retval None
  */
void NGL_Font_SetFont(const NGL_Font *selectFont)
{
	nfont = (NGL_Font*)selectFont;
}


/**
  * @brief  NGL_Font_DrawChar
  * @param
  * @retval CHAR_WIDTH(c);
  */
uint8_t NGL_Font_DrawChar(uint16_t X, uint16_t Y, NGL_TransparentState trspr, char c)
{
	int16_t i;
	uint8_t k; // переменные для циклов
	uint16_t Xa;
  uint16_t Ya; // позиция по Y для пропуска точек
	uint8_t *ptr = (uint8_t*)CHAR_DATA(c); // указатель на данные символа
	uint8_t CharWidht_Bytes = (CHAR_WIDTH(c) + 7) / 8;
	uint8_t CharEndByteBitMask = 1 << ((CharWidht_Bytes * 8) - CHAR_WIDTH(c));
	uint8_t CharByteBitMask = 0x01;
	uint8_t mask;  // маска для расшифровки байтов данных символа, в двоичном формате = 0b10000000

  uint16_t CharHeight = CHAR_HEIGHT(c) - 1;

  uint16_t TextColor = NGL_Color_GetTextColor();
  uint16_t BackColor = NGL_Color_GetBackColor();

	/* Если шрифт не определен или символ находится вне границ символов для шрифта то выход */
	if(!nfont) return 0;
	if(c < nfont->FirstChar || c > nfont->LastChar) return 0;

	CS_LCD_set;

	/* выводим символ вертикальными линиями слева-направо */
	for(i = 0; i <= CharHeight ; i++)
	{
		/* установка курсора LCD */
		Xa = X; // позиция по X для пропуска точек
		Ya = (Y + (nfont->Height-1)) - i;
    LCD->SetCursor(Xa, Ya);
		Xa++;

		/* расшифровываем байты данных, один байт содержит информацию о 8-ми точках по горизонтали */
		for(k = 0; k < CharWidht_Bytes; k++)
		{
			if((*ptr & 0xFF) != 0)									// --- если байт не равен нулю то
			{
				if(k < CharWidht_Bytes - 1) CharByteBitMask = 0x01;
				else CharByteBitMask = CharEndByteBitMask;

				mask = 0x80;										// сбрасываем битовую маску
				do
				{
					if(*ptr & mask)
					{
						LCD->WritePixel(TextColor);		// иначе если совпало с битом маски выводим точку цветом текста
					}
					else if(trspr == Transparent)
					{
						LCD->SetCursor(Xa, Ya);				// если выводим символ с прозрачным фоном пропускаем точку
					}
					else
					{
						LCD->WritePixel(BackColor);
					}

					Xa++;
					mask >>= 1;

				} while (mask >= CharByteBitMask);
			}
			else													// --- иначе пропускаем байт
			{
				LCD->SetCursor(Xa + 7, Ya);
				Xa += 8;
			}

			ptr++;                                        // инкремент указателя на байт данных символа
		}
	}

	CS_LCD_clr;

	/* возвращаем ширину символа */
	return CHAR_WIDTH(c);
}


/**
  * @brief  NGL_Font_DrawString
  * @param
  * @retval
  */
uint16_t NGL_Font_DrawString(uint16_t X, uint16_t Y, uint16_t Color, const NGL_Font *font, NGL_TransparentState trspr, char *str)
{
	nfont = (NGL_Font*)font;
	return NGL_Font_DrawColorString(X, Y, Color, trspr, str);
}

/**
  * @brief  NGL_Font_DrawString
  * @param
  * @retval
  */
uint16_t NGL_Font_DrawFastString(uint16_t XPos, uint16_t YPos, NGL_TransparentState trspr, char *str)
{
	/* переменная для запоминания ширины последнего напечатоного символа */
	uint8_t lastPrintCharWidth = 0;

	/* если шрифт не определен то выход */
	if(!nfont || !str) return 0;

	/* печатаем пока есть символы в строке */
	while(*str!=0)
	{
		lastPrintCharWidth = NGL_Font_DrawChar(XPos, YPos, trspr, *str);  // выводим сивол и запоминаем его ширину

		XPos += (lastPrintCharWidth + CHAR_SPACE);                   // смещаем курсор на ширину напечатонного символа + межсимвольное растояние
		str++;	                                                     // инкремент указателя на строку

//		if(trspr == 0)
//		{
//			NGL_LCD_ClearArea(XPos - CHAR_SPACE, YPos, XPos, YPos + nfont->Height, NGL_Color_GetBackColor());
//		}
	}
	return XPos;
}


/**
  * @brief  NGL_Font_DrawString
  * @param
  * @retval
  */
uint16_t NGL_Font_DrawColorString(uint16_t X, uint16_t Y, uint16_t Color, NGL_TransparentState trspr, char *str)
{
	NGL_Color_SetTextColor(Color);
	return NGL_Font_DrawFastString(X, Y, trspr, str);
}


/**
  * @brief  NGL_Font_DrawCropStrimg
  * @param
  * @retval
  */
uint16_t NGL_Font_DrawCropString(uint16_t X, uint16_t Y, NGL_TransparentState trspr, char *str, uint8_t Pos, uint8_t Lenght, uint16_t Color)
{
	uint8_t i;
	char CropString[Lenght + 1];
	uint16_t X_Start = X + NGL_Font_MeasureCropStringWidth(str, Pos);
	uint16_t X_End = 0;

	for(i = 0; i < Lenght; i++) CropString[i] = *(str + Pos + i);
	CropString[Lenght] = 0;

	X_End = NGL_Font_DrawColorString(X_Start, Y, Color, trspr, CropString);

	return X_End;
}


/**
  * @brief  NGL_Font_MeasureStrimgWidth
  * @param  Pointer to measuring string
  * @retval String width
  */
uint16_t NGL_Font_MeasureStringWidth(NGL_Font *_nfont, char *str)
{
	uint16_t Pos = 0;

  NGL_Font_SetFont(_nfont);

	/* пока есть символы в строке */
	while(*str != 0)
	{
		Pos += (CHAR_WIDTH(*str) + CHAR_SPACE);
		str++;
	}

	str--;
	return Pos - CHAR_SPACE;
}


/**
  * @brief  NGL_Font_MeasureCropStringWidth
            Measuring string in pixels

  * @param  str - Pointer to measuring string
            NumSymbols - symbols num in pointer string to measure
  * @retval Width in pixels symbols from *str to *(str + NumSymbols)
  */
uint16_t NGL_Font_MeasureCropStringWidth(char *str, uint8_t NumSymbols)
{
	uint16_t Pos = 0;

	/* пока есть символы в строке */
	while((*str != 0) && (NumSymbols > 0))
	{
		Pos += (CHAR_WIDTH(*str) + CHAR_SPACE);
		str++;
		NumSymbols--;
	}

	return Pos;
}



/**
  * @brief  uint8_t_to_char
  * @param  uint8_t
  * @retval char
  */
static __inline char uint8_t_to_char(uint8_t val)
{
    return (48 + val);
}

/**
  * @brief  NGL_floatToString
  * @param
  * @retval None
  */
void NGL_floatToString(char *ValText, float Value, uint8_t Length)
{
    int d1, d2;     // integer & fractional parts
    float f2;     	// fractional part
    uint8_t i = Length;

    /* --- Convert to string, fixed 5 digit with float point position --- */
    if(Value < 0) Value *= -1;

    if(Value > 1e4)
    {
        Value = 99999;
        i = 0;
    }
    else if(Value < 1) i = Length - 1;
    else
    {
        f2 = Value;
        while(f2 >= 1)
        {
            f2 /= 10;
            i--;
        }
    }

    d1 = Value;		// Get the integer part

    if(i != 0)
    {
        f2 = Value - d1;     					// Get fractional part
        d2 = truncf(f2 * pow10f(i));		// Turn into integer

        NGL_uintToFixedString(d1, ValText, Length - i);
        ValText[strlen(ValText)] = '.';
        NGL_uintToFixedString(d2, &ValText[strlen(ValText)], i);
    }
    else NGL_uintToFixedString((uint32_t)Value, ValText, Length);

    ValText[strlen(ValText)] = 0;
}

/**
  * @brief  NGL_uintToFixedString
  * @param
  * @retval None
  */
void NGL_intToString(int32_t val, char* str)
{
    uint32_t _num;

    if(val >= 0) {
      _num = (uint32_t)val;
    }
    else {
      _num = (uint32_t)(-val);
      *str = '-';
      str++;
    }

    NGL_uintToString(_num, str);
}

/**
  * @brief  NGL_uintToFixedString
  * @param
  * @retval None
  */
void NGL_uintToString(uint32_t val, char* Str)
{
    uint32_t _num = val;
    uint32_t length = 1;

    while (_num >= 10) {
      _num /= 10;
      length++;
    }

    NGL_uintToFixedString(val, Str, length);
}

/**
  * @brief  NGL_uintToFixedString
  * @param
  * @retval None
  */
void NGL_uintToFixedString(uint32_t val, char* Str, uint8_t NumSymbol)
{
    int32_t i, j, Rate;
    uint8_t A[NumSymbol];
    uint32_t NumPow = 1;

    for(i = 0; i < NumSymbol+1; i++) NumPow *= 10;
    if(val > NumPow - 1) val = NumPow - 1;

    for(i = NumSymbol - 1; i >= 0; i--)
    {
        A[i] = 0;
        Rate = 1;

        for(j = 0; j < i; j++) Rate *= 10;
        while(val >= Rate)
        {
            val = val - Rate;
            A[i]++;
        }

        (*Str) = uint8_t_to_char(A[i]);
        Str++;
    }
}

/**
  * @brief  NGL_StrtToInt_n
  * @param
  * @retval None
  */
int NGL_StrtToInt_n(char* str, int n)
{
    int sign = 1;
    int place = 1;
    int ret = 0;

    int i;
    for (i = n-1; i >= 0; i--, place *= 10)
    {
        int c = str[i];
        switch (c)
        {
            case '-':
                if (i == 0) sign = -1;
                else return -1;
                break;
            default:
                if (c >= '0' && c <= '9')   ret += (c - '0') * place;
                else return -1;
        }
    }

    return sign * ret;
}

/**
  * @brief  NGL_StrtToInt
  * @param
  * @retval None
  */
int NGL_StrtToInt(char* str)
{
    char* temp = str;
    int n = 0;
    while (*temp != 0) {
        n++;
        temp++;
    }
    return NGL_StrtToInt_n(str, n);
}
