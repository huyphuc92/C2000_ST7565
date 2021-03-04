/**
  ******************************************************************************
  * @file    st7567.h 
  * @author  MR.YANG Team
  * @version V1.0.0
  * @date    2020-02-10
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2020 MR.YANG </center></h2>
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __ST7567_H__
#define __ST7567_H__

#ifdef __cplusplus
 extern "C" {
#endif 


#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"



void LCD_GPIO_Config( void );

void ST7567_Init( void );

void LCD_Write_Dat( u8 dat );
void LCD_Write_Cmd( u8 cmd );

void Backlight_On( void );

void Backlight_Off( void );

void LCD_Address_Set( u8 page, u8 column );

void Disp_Str_8x16( u8 page, u8 column, u8* text );

void LCD_Disp_Clr( u8 dat );

void PE5TEST(void);

#ifdef __cplusplus
}
#endif   
   

#endif
   


/******************* (C) COPYRIGHT 2020  MT.YANG *****END OF FILE****/  

