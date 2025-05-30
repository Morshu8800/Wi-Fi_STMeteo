/*

 ******************************************************************************
 * @file 			( фаил ):   LCD1602.h
 * @brief 		( описание ):
 ******************************************************************************
 * @attention 	( внимание ):	author: Golinskiy Konstantin	e-mail: golinskiy.konstantin@gmail.com
 ******************************************************************************

 */

#ifndef _LCD1602_H
#define _LCD1602_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ----------------------------------------------------------*/

#include "main.h"
#include "LCD1602_config.h"
#include "stdio.h"
#include "stdbool.h"

#define LCD_I2C_ADDR (0x27 << 1)
#define LCD_I2C_TYPEDEF hi2c1 // если нужно заменить шину i2c, то нужно поменять занчение hi2c1 на hi2cx, где x - это номер шины i2c
extern I2C_HandleTypeDef LCD_I2C_TYPEDEF;

extern uint8_t portLcd;

#ifdef LCD1602_I2C
extern I2C_HandleTypeDef LCD_I2C;
#endif

/*#ifdef	FONT_CYRILLIC
 extern const uint8_t Font_Cyrillic[];
 #endif

 Functions prototypes ----------------------------------------------*/

void LCD_SendByte(uint8_t bt, uint8_t mode);

bool LCD_Connect_test(void);

/*
 ******************************************************************************
 * @brief	 ( описание ):  Функция инициализации дисплея
 * @param	( параметры ):
 * @return  ( возвращает ):

 ******************************************************************************
 */
void LCD_Init(void);
//----------------------------------------------------------------------------------------------------------------------

/*
 ******************************************************************************
 * @brief	 ( описание ): Функция печати строки на дисплее ( поддерживает как Киррилицу так и латиницу )
 * @param	( параметры ): Строка
 * @return  ( возвращает ):

 ******************************************************************************
 */
void LCD_PrintString(char *str);
//-----------------------------------------------------------------------------------------------------------------------

void LCD_PrintNum(uint32_t num);
//-----------------------------------------------------------------------------------------------------------------------
/*
 ******************************************************************************
 * @brief	 ( описание ):  Функция очистки всего дисплея
 * @param	( параметры ):
 * @return  ( возвращает ):

 ******************************************************************************
 */
void LCD_Clear(void);
//--------------------------------------------------------------------------------------------------------------------------

/*
 ******************************************************************************
 * @brief	 ( описание ):  Функция включения и выключения подсветки дисплея
 * @param	( параметры ):	( 0-выкл, 1 (либо другое от 0)- вкл )
 * @return  ( возвращает ):

 ******************************************************************************
 */
void LCD_LedOnOff(uint8_t status);
//-----------------------------------------------------------------------------------------------------------------

/*
 ******************************************************************************
 * @brief	 ( описание ):  Функция установки курсора ( позиционирования )
 * @param	( параметры ):	( 1-й параметр позиция символа , 2-й параметр строка, ( Счет с нуля )
 * @return  ( возвращает ):

 ******************************************************************************
 */
void LCD_SetCursor(uint8_t x, uint8_t y);
//--------------------------------------------------------------------------------------------------------------------

/*
 ******************************************************************************
 * @brief	 ( описание ): функция двигает надпись вправо на 1 символ ( бегущая строка )
 * @param	( параметры ):
 * @return  ( возвращает ):

 ******************************************************************************
 */
void LCD_StringRunR(void);
//--------------------------------------------------------------------------------------------------------------------

/*
 ******************************************************************************
 * @brief	 ( описание ): функция двигает надпись влево на 1 символ ( бегущая строка )
 * @param	( параметры ):
 * @return  ( возвращает ):

 ******************************************************************************
 */
void LCD_StringRunL(void);
//--------------------------------------------------------------------------------------------------------------------

/*
 ******************************************************************************
 * @brief	 ( описание ): функция выключает надпись на єкране (єкран пустой ) и включает ее обратно
 * @param	( параметры ): ( 0- выкл, 1 ( или любое отличное от 0 ) вкл )
 * @return  ( возвращает ):

 ******************************************************************************
 */
//void LCD_TextOnOff( uint8_t status );
//---------------------------------------------------------------------------------------------------------------------------

/*
 ******************************************************************************
 * @brief	 ( описание ):	функция создания своего символа и записи его в память
 * @param	( параметры ): 	Первый параметр массив с символом
 второй парамет номер ячеки куда записали от 0 до 7 ( всего 8 символов можно записать )
 * @return  ( возвращает ):

 ******************************************************************************
 */
void LCD_CreateChar(uint8_t *mc, uint8_t add);
//----------------------------------------------------------------------------------------------------------------------------

/*
 ******************************************************************************
 * @brief	 ( описание ): функция для отображения собственного символа ( печать на дисплей )
 перед вызавом своего символа обязательно вызываем LCD_SetCursor();
 * @param	( параметры ):	номер памяти куда сохранили символ от 0 до 7
 * @return  ( возвращает ):

 ******************************************************************************
 */
void LCD_PrintMyChar(uint8_t numChar);
//------------------------------------------------------------------------------------------------------------------------------

/*
 ******************************************************************************
 * @brief	 ( описание ):  Функция включения и выключения мигания курсора
 * @param	( параметры ):	( 0-выкл, 1 (либо другое от 0)- вкл )
 * @return  ( возвращает ):

 ******************************************************************************
 */
void LCD_CursorOnOff(uint8_t status);
//------------------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif	/*	_LCD1602_H */

/************************ (C) COPYRIGHT GKP *****END OF FILE****/
