/**
  ******************************************************************************
  * @file    latch.c
  * @author  MR.YANG Team
  * @version V1.0.0
  * @date    2019.05.16
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 MR.YANG </center></h2>
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#include "hw_st7567.h"
#include "font.h"
#include "bsp_config.h"

/* Hard LCD

PD7 -->CS   PD5  -->RESET   PD4  -->RS    PD1  -->SCL   PD0 -->SDA

PC6 -->Backlight Ctrl    ZKCS-PC7 


*/

#define  SET_RST  GPIO_SetBits(GPIOD, GPIO_Pin_5)               //reset               
#define  CLR_RST  GPIO_ResetBits(GPIOD, GPIO_Pin_5)

#define  SET_CLK  GPIO_SetBits(GPIOD, GPIO_Pin_1)               //clk
#define  CLR_CLK  GPIO_ResetBits(GPIOD, GPIO_Pin_1)

#define  SET_DAT  GPIO_SetBits(GPIOD, GPIO_Pin_0)               //dat
#define  CLR_DAT  GPIO_ResetBits(GPIOD, GPIO_Pin_0)

#define  SET_RS  GPIO_SetBits(GPIOD, GPIO_Pin_4)                //rs
#define  CLR_RS  GPIO_ResetBits(GPIOD, GPIO_Pin_4)

#define  SET_CS  GPIO_SetBits(GPIOD, GPIO_Pin_7)                //cs
#define  CLR_CS  GPIO_ResetBits(GPIOD, GPIO_Pin_7)

#define  BL_ON   GPIO_SetBits(GPIOC, GPIO_Pin_6)                //BLK
#define  BL_OFF  GPIO_ResetBits(GPIOC, GPIO_Pin_6)

/*****************************************************************************
  * @brief  This function  delay_ms
  * @param  None
  * @retval None
  */
static void delay_ms(u32 cont)
{
   u16 x,y;
   
   for(x=cont;x>0;x--)
      for(y=500;y>0;y--);

}

/*****************************************************************************
  * @brief  This function LCD GPIO config
  * @param  None
  * @retval None
  */
void LCD_GPIO_Config( void )
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    /* GPIOC,D Peripheral clock enable */
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE );
    
    /* Configure PD0,PD1,PD4,PD5,PD7 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_Init( GPIOC, &GPIO_InitStructure );
	
	//  printf("LCD_GPIO_Init");

}
/*****************************************************************************
  * @brief  This function Write control data (command)
  * @param  None
  * @retval None
  */

void LCD_Write_Cmd( u8 cmd )
{
    u8 i;
    CLR_RS;
    CLR_CS;
    for( i = 0; i < 8; i++ )
    {
        CLR_CLK;
        if( cmd & 0x80 )
        {
            SET_DAT;
        }
        else
        {
            CLR_DAT;
        }
        SET_CLK;
        
        cmd = cmd << 1;
    }
    SET_CS;
}
/*****************************************************************************
  * @brief  This function Display Data Write
  * @param  None
  * @retval None
  */
void LCD_Write_Dat( u8 dat )
{
    u8 i;
    
    SET_RS;
    CLR_CS;
    for( i = 0; i < 8; i++ )
    {
        CLR_CLK;
        if( dat & 0x80 )
        {
            SET_DAT;
        }
        else
        {
            CLR_DAT;
        }
        SET_CLK;
        
       dat= dat << 1;
    }
    SET_CS;
}


/*****************************************************************************
  * @brief  This function  St7567 Init
  * @param  None
  * @retval None
  */
void ST7567_Init( void )
{
    SET_RST;        /*��λ���*/
    delay_ms( 20 );
    
    CLR_RST;        /*�͵�ƽ��λ*/
    delay_ms( 200 );
    
    SET_RST;        /*��λ���*/
    delay_ms( 20 );
    
    CLR_CS;
    
    LCD_Write_Cmd( 0xe2 ); /*CMD_INTERNAL_RESET*/
    delay_ms( 30 );
    
    LCD_Write_Cmd( 0x2c ); /*��ѹ���� 1*/
    delay_ms( 5 );
    LCD_Write_Cmd( 0x2e ); /*��ѹ���� 2*/
    delay_ms( 5 );
    LCD_Write_Cmd( 0x2f ); /*��ѹ���� 3*/
    delay_ms( 5 );
    LCD_Write_Cmd( 0xa6 ); /*CMD_SET_DISP_NORMAL*/

    LCD_Write_Cmd( 0x23 ); /* �ֵ��Աȶȣ������÷�Χ 0x20��0x27 */
    LCD_Write_Cmd( 0x82 ); /* ΢���Աȶ�*/
    //LCD_Write_Cmd( 0x3F ); /* 0x1a,΢���Աȶȵ�ֵ�������÷�Χ 0x00��0x3f */
    
    LCD_Write_Cmd( 0xa3 ); /*CMD_SET_BIAS_7*/
    LCD_Write_Cmd( 0xa4 ); /*CMD_SET_ALLPTS_NORMAL*/
    
    LCD_Write_Cmd( 0x40 ); /*CMD_SET_DISP_START_LINE*/
    
    LCD_Write_Cmd( 0xc8 ); /*CMD_SET_COM_REVERSE*/
    LCD_Write_Cmd( 0xa0 ); /*CMD_SET_ADC_NORMAL*/
    
    LCD_Write_Cmd( 0xaf ); /*CMD_DISPLAY_ON*/
		
		delay_ms(100);
    
    SET_CS;
		
   // printf("LCD_Init /n");
    
}
/*****************************************************************************
  * @brief  This function  Backlight Ctrl
  * @param  None
  * @retval None
  */
void Backlight_On( void )
{

  GPIO_SetBits(GPIOC, GPIO_Pin_6);                //Backlight on

}

void Backlight_Off( void )
{

  GPIO_ResetBits(GPIOC, GPIO_Pin_6);              //Backlight off

}

/*****************************************************************************
  * @brief  This function  Page address set
  * @param  None
  * @retval None
  */
void LCD_Address_Set( u8 page, u8 column )
{
    column = column - 1;      //����ƽ����˵�ĵ� 1 �У��� LCD ���� IC ���ǵ� 0 �С������������ȥ 1.
    page = page - 1;          //����ҳ��ַ��ÿҳ�� 8 �С�һ������� 64 �б��ֳ� 8 ��ҳ������ƽ����˵�ĵ� 1 ҳ���� LCD ���� IC ���ǵ� 0 ҳ�������������ȥ 1*/
    LCD_Write_Cmd( 0xb0 + page );             //�����е�ַ�ĸ� 4 λ   page addres
	
   //0010 1000
	
    LCD_Write_Cmd( ( ( column >> 4 ) & 0x0f ) + 0x10 ); //�����е�ַ��MSB
    LCD_Write_Cmd( column & 0x0f );                     //�����е�ַ��LSB
}

/*****************************************************************************
  * @brief  This function
  * @param  None
  * @retval None
  */
void Disp_Str_8x16( u8 page, u8 column, u8* text )
{
    u16 i = 0, j, k, n;
    while( text[i] > 0x00 )
    {
        if( ( text[i] >= 0x20 ) && ( text[i] <= 0x7e ) )  //ASii��Χ
        {
            j = text[i] - 0x20;
            for( n = 0; n < 2; n++ )
            {
                LCD_Address_Set( page + n, column );
                for( k = 0; k < 8; k++ )
                {
                    LCD_Write_Dat( ascii_table_8x16[j][k + 8 * n] ); /*��ʾ 8x16 �� ASCII �ֵ� LCD �ϣ�yΪҳ��ַ��x Ϊ�е�ַ�����Ϊ����*/
                }
            }
            i++;
            column += 8;
        }
        else
        {
            i++;
        }
    }
}
/*****************************************************************************
  * @brief  This function  Clear LCD or Full LCD
  * @param  None
  * @retval None
  */
void LCD_Disp_Clr( u8 dat )
{
    u8 i, j; /* so we use this, set the address at each row */
    for( i = 0; i < 8; i++ )
    {
        LCD_Write_Cmd( 0xb0 | i );      /* Set row to 0  page addres     */
        LCD_Write_Cmd( 0x10 );          /* Set column to 0*/
        LCD_Write_Cmd( 0X00 ); //����columnʱ�����ADC����Ϊ����(131~0��������127~0)����ôÿ��Ӧ�ôӵ�5��(0~4)��ʼ�������һ���ַ���ʾ��ȫ��
        for( j = 0; j < 128; j++ )
        {
            LCD_Write_Dat( dat ); delay_ms(1);
        }
    }
}



