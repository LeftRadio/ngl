/**
  ******************************************************************************
  * @file    	bitmap.c
  * @author  	LeftRadio
  * @version 	1.2.0
  * @date
  * @brief		NGL :: Bitmaps Layer sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "NGL_types.h"
#include "bitmap.h"
#include "LCD_HAL.h"
#include "LCD_MAL.h"
#include "Graphics_Primitive.h"
#include "picojpeg.h"
#include "colors.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BITMAP_NONCOMPRESSED				(uint8_t)0
#define BITMAP_COMPRESS_RLE					(uint8_t)1
#define BITMAP_COMPRESS_picoJPG				(uint8_t)2

/* Private macro -------------------------------------------------------------*/
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private function  --------------------------------------------------------*/

/**
  * @brief  PutOut_1bpp_Pixels
  * @param
  * @retval None
  */
static __inline void PutOut_1bpp_Pixels(uint8_t *Data, uint16_t iterationFlag, uint16_t cnt)
{
	uint8_t mask = 0x80;

	do
	{
		/* 1 byte containe 8 points */
		do
		{
			if(*(Data) & mask) LCD->WritePixel(NGL_Color_GetGraphicsColor());		/* if equal to mask draw graphics color pixel */
			else LCD->WritePixel(NGL_Color_GetBackColor());								/* else draw back color pixel */

			mask = mask >> 1;	// shift mask for the next bit

		} while(mask != 0);

		mask = 0x80;
		Data += iterationFlag;
		cnt--;
	} while(cnt != 0);
}

/**
  * @brief  PutOut_8bpp_Pixels
  * @param
  * @retval None
  */
static __inline void PutOut_8bpp_Pixels(uint8_t *Data, uint16_t iterationFlag, uint16_t cnt)
{
	uint16_t _16bppColor;

	do
	{
		_16bppColor = (((uint16_t)*Data & 0xE0) << 8) + (((uint16_t)*Data & 0x1C) << 6) + (((uint16_t)*Data & 0x03) << 3);
		LCD->WritePixel(_16bppColor);
		Data += iterationFlag;
		cnt--;
	} while(cnt != 0);
}

/**
  * @brief  PutOut_16bpp_Pixels
  * @param
  * @retval None
  */
static __inline void PutOut_16bpp_Pixels(uint16_t *Data, uint16_t iterationFlag, int cnt)
{
	do
	{
		LCD->WritePixel(*Data);
		Data += iterationFlag;
		cnt--;
	} while(cnt > 0);
}

/**
  * @brief  ConvertTo565Color
  * @param
  * @retval None
  */
static __inline uint16_t ConvertTo565Color(uint8_t R, uint8_t G, uint8_t B)
{
//	uint16_t a = (uint16_t)(((R >> 3) & 0x3F) << 11);
//	uint16_t b = (uint16_t)(((G >> 2) & 0x7F) << 5);
//	uint16_t c = (uint16_t)((B >> 3) & 0x3F);

//	return (a | b | c);
	return ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3);
}


/**
  * @brief  LCD_Draw_1bpp_IMG
  * @param
  * @retval None
  */
static void LCD_Draw_1bpp_IMG(const NGL_Image *Bitmap)
{
	uint8_t iterationFlag = 0, cnt;
	uint8_t *Data = (uint8_t*)Bitmap->Data;
	uint32_t i;

	/* Decompress image if compressed */
	if(Bitmap->Compressed == BITMAP_COMPRESS_RLE)
	{
		/** ѕробегаем все байты массива изображени€ **/
		for (i = 0; i < Bitmap->DataArraySize - 1; i++ )
		{
			iterationFlag = (*Data & 0x80) >> 7;
			cnt = (*Data & 0x7F) + 1;
			Data++;

			PutOut_1bpp_Pixels(Data, iterationFlag, cnt);
		}
	}
	else
	{
		PutOut_1bpp_Pixels(Data, 1, Bitmap->DataArraySize);
	}
}


/**
  * @brief  LCD_Draw_8bpp_IMG
  * @param
  * @retval None
  */
static void LCD_Draw_8bpp_IMG(const NGL_Image *Bitmap)
{
	uint16_t iterationFlag, cnt;
	uint8_t *Data = (uint8_t*)Bitmap->Data;
	int32_t i;

	/* If image is compressed so decompress and draw */
	if(Bitmap->Compressed == BITMAP_COMPRESS_RLE)
	{
		for (i = 0; i < Bitmap->DataArraySize - 1; i++ )
		{
			iterationFlag = (*Data & 0x80) >> 7;
			cnt = (*Data & 0x7F) + 1;
			Data++;

			PutOut_8bpp_Pixels(Data, iterationFlag, cnt);
		}
	}
	else	// else simple draw
	{
		PutOut_8bpp_Pixels(Data, 0, Bitmap->DataArraySize);
	}
}


/**
  * @brief  LCD_Draw_16bpp_IMG
  * @param
  * @retval None
  */
static void LCD_Draw_16bpp_IMG(const NGL_Image *Bitmap)
{
	uint16_t iterationFlag = 1, cnt = 0;
	uint16_t *Data = (uint16_t*)Bitmap->Data;
	int32_t pnt = 0, MAXpnt = 0;

	/* If image is compressed so decompress and draw */
	if(Bitmap->Compressed == BITMAP_COMPRESS_RLE)
	{
		MAXpnt = (Bitmap->Width + 1) * (Bitmap->Height + 1);
		while(pnt < MAXpnt)
		{
			Data += 1 - iterationFlag;
			iterationFlag = ((*Data & 0x8000) >> 15);
			cnt = (*Data & 0x7FFF);
			Data++;

			do
			{
				LCD->WritePixel(*Data);
				Data += iterationFlag;
				cnt--;
				pnt++;
			} while(cnt > 0);
		}
	}
	else	// else simple draw
	{
		PutOut_16bpp_Pixels(Data, 1, Bitmap->DataArraySize);
	}
}


/**
  * @brief  picoJPG_need_bytes_callback
  * @param
  * @retval None
  */
unsigned char picoJPG_need_bytes_callback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *pCallback_data)
{
	uint32_t n;
	jpeg_buffer_t *jpeg_buffer = (jpeg_buffer_t*)pCallback_data;

	n = MIN(jpeg_buffer->length - jpeg_buffer->position, buf_size);
	memcpy(pBuf, jpeg_buffer->buffer + jpeg_buffer->position, n);
	*pBytes_actually_read = n;

	jpeg_buffer->position += n;

	return 0;
}


/**
  * @brief  picoJPG_Show
  * @param
  * @retval None
  */
void picoJPG_Show(pjpeg_image_info_t image_info, int poX, int poY)
{
	unsigned char status;
	//pjpeg_image_info_t image_info;
	int mcu_x = 0;
	int mcu_y = 0;
	uint32_t row_pitch;
	uint32_t decoded_width;
//	uint32_t decoded_height;
//	uint32_t row_blocks_per_mcu, col_blocks_per_mcu;

	decoded_width = image_info.m_width;
//	decoded_height = image_info.m_height;

	row_pitch = decoded_width * image_info.m_comps;

//	row_blocks_per_mcu = image_info.m_MCUWidth >> 3;
//	col_blocks_per_mcu = image_info.m_MCUHeight >> 3;

	for (;;)
	{
		int y, x;
		uint8 *pDst_row;

		status = pjpeg_decode_mcu();

		if (status)
		{
			if (status != PJPG_NO_MORE_BLOCKS)
			{
				return;
			}

			break;
		}

		if (mcu_y >= image_info.m_MCUSPerCol)
		{
			return;
		}

		for (y = 0; y < image_info.m_MCUHeight; y += 8)
		{
			const int by_limit = MIN(8, image_info.m_height - (mcu_y * image_info.m_MCUHeight + y));

			for (x = 0; x < image_info.m_MCUWidth; x += 8)
			{
				// Compute source byte offset of the block in the decoder's MCU buffer.
				uint32_t src_ofs = (x * 8U) + (y * 16U);
				const uint8 *pSrcR = image_info.m_pMCUBufR + src_ofs;
				const uint8 *pSrcG = image_info.m_pMCUBufG + src_ofs;
				const uint8 *pSrcB = image_info.m_pMCUBufB + src_ofs;

				const int bx_limit = MIN(8, image_info.m_width - (mcu_x * image_info.m_MCUWidth + x));

				if (image_info.m_scanType == PJPG_GRAYSCALE)
				{
					int bx, by;
					for (by = 0; by < by_limit; by++)
					{
						for (bx = 0; bx < bx_limit; bx++)
						{
							int Gr = *pSrcR++;
							NGL_GP_DrawPixel((mcu_x * image_info.m_MCUWidth) + x + bx + poX, (mcu_y * image_info.m_MCUHeight)+ y + by + poY, ConvertTo565Color(Gr, Gr, Gr));
						}

						pSrcR += (8 - bx_limit);
					}
				}
				else
				{
					int bx, by;
					for (by = 0; by < by_limit; by++)
					{
						for (bx = 0; bx < bx_limit; bx++)
						{
//							int R = ;
//							int G = ;
//							int B = ;
							NGL_GP_DrawPixel(
                                        (mcu_x * image_info.m_MCUWidth) + x + bx + poX,
                                        (mcu_y * image_info.m_MCUHeight)+ y + by + poY,
                                        ConvertTo565Color(*pSrcR++, *pSrcG++, *pSrcB++)
                                        );
						}

						pSrcR += (8 - bx_limit);
						pSrcG += (8 - bx_limit);
						pSrcB += (8 - bx_limit);
					}
				}
			}

			pDst_row += (row_pitch * 8);
		}

		mcu_x++;
		if (mcu_x == image_info.m_MCUSPerRow)
		{
			mcu_x = 0;
			mcu_y++;
		}
	}

}


/**
  * @brief  NGL_DrawImage
  * @param
  * @retval None
  */
void NGL_DrawImage(uint16_t X0, uint16_t Y0, const NGL_Image *Image)
{
	/* Set out area */
	LCD->SetArea(X0, Y0, X0 + Image->Width, Y0 + Image->Height);

    /* */
	CS_LCD_set;
	LCD->SetCursor(X0, Y0);

    /* Draw */
	if(Image->Compressed == BITMAP_COMPRESS_picoJPG)
	{
		jpeg_buffer_t jpeg_buffer;
		unsigned char status;
		pjpeg_image_info_t image_info;

		memset(&jpeg_buffer, 0, sizeof(jpeg_buffer_t));
		jpeg_buffer.buffer = (uint8_t*)Image->Data;
		jpeg_buffer.length = Image->DataArraySize;

		status = pjpeg_decode_init(&image_info, picoJPG_need_bytes_callback, &jpeg_buffer);
		if (!status) {
			picoJPG_Show(image_info, X0, Y0);
		}
	}
	else if(Image->ColorBits == 1) LCD_Draw_1bpp_IMG(Image);
	else if(Image->ColorBits == 8) LCD_Draw_8bpp_IMG(Image);
	else if(Image->ColorBits == 16) LCD_Draw_16bpp_IMG(Image);
	CS_LCD_clr;

    /* Set out area to fullscreen */
	NGL_LCD_SetFullScreen();
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
