/*-----------------------------------------------
  名称：LCD1602
  引脚定义如下：1-VSS 2-VDD 3-V0 4-RS 5-R/W 6-E 7-14 DB0-DB7 15-BLA 16-BLK
------------------------------------------------*/
#include "1602.h"
#include "delay.h"

#define CHECK_BUSY
sbit dula=P2^6;
sbit wela=P2^7;
sbit RS = P3^5;   //定义端口
sbit RW = P3^6;
sbit EN = P3^4;

#define RS_CLR RS=0
#define RS_SET RS=1

#define RW_CLR RW=0
#define RW_SET RW=1

#define EN_CLR EN=0
#define EN_SET EN=1

#define DataPort P0

/*------------------------------------------------
              判忙函数
------------------------------------------------*/
bit LCD_Check_Busy(void)
{
#ifdef CHECK_BUSY
    DataPort= 0xFF;
    RS_CLR;
    RW_SET;
    EN_CLR;
    _nop_();
    EN_SET;
    return (bit)(DataPort & 0x80);
#else
    return 0;
#endif
}
/*------------------------------------------------
              写入命令函数
------------------------------------------------*/
void LCD_Write_Com(unsigned char com)
{
		 
    //while(LCD_Check_Busy()); //忙则等待
    RS_CLR;

    RW_CLR;
    EN_SET;

    DataPort= com;
    DelayMs(5);
    _nop_();
    EN_CLR;
}
/*------------------------------------------------
              写入数据函数
------------------------------------------------*/
void LCD_Write_Data(unsigned char Data)
{
    //while(LCD_Check_Busy()); //忙则等待
    RS_SET;
    RW_CLR;
    EN_SET;
	
    DataPort= Data;
    DelayMs(5);
    _nop_();
    EN_CLR;
}

/*------------------------------------------------
                清屏函数
------------------------------------------------*/
void LCD_Clear(void)
{
    LCD_Write_Com(0x01);
    DelayMs(5);
}
/*------------------------------------------------
              写入字符串函数
------------------------------------------------*/
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s)
{

    while (*s)
    {
        LCD_Write_Char(x,y,*s);
        s ++;  x++;
    }
}
/*------------------------------------------------
              写入字符函数
------------------------------------------------*/
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data)
{
    if (y == 0)
    {
        LCD_Write_Com(0x80 + x);
    }
    else
    {
        LCD_Write_Com(0xC0 + x);
    }
    LCD_Write_Data( Data);
}
/*------------------------------------------------
              初始化函数
------------------------------------------------*/
void LCD_Init(void)
{
	dula=0;
	wela=0;
	
    RW_CLR;
	
    EN_CLR;
	
    LCD_Write_Com(0x38);    /*显示模式设置*/
    DelayMs(10);

    LCD_Write_Com(0x08);    /*显示关闭*/
    LCD_Write_Com(0x01);    /*显示清屏*/
    LCD_Write_Com(0x06);    /*显示光标移动设置*/
    DelayMs(10);
    LCD_Write_Com(0x0C);    /*显示开及光标设置*/
}
   

