/*-----------------------------------------------
  名称：矩阵键盘
------------------------------------------------*/
#include<reg52.h> //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include"keyboard.h"
#include"delay.h"

#define KeyPort P1

/*------------------------------------------------
按键扫描函数，返回扫描键值
------------------------------------------------*/
unsigned char KeyScan(void)  //键盘扫描函数，使用行列反转扫描法
{
 unsigned char cord_h,cord_l;//行列值中间变量
 KeyPort=0x0f;            //行线输出全为0
 cord_h=KeyPort&0x0f;     //读入列线值
 if(cord_h!=0x0f)    //先检测有无按键按下
 {
  DelayMs(10);        //去抖
  if((KeyPort&0x0f)!=0x0f)
  {
    cord_h=KeyPort&0x0f;  //读入列线值
    KeyPort=cord_h|0xf0;  //输出当前列线值
    cord_l=KeyPort&0xf0;  //读入行线值

    while((KeyPort&0xf0)!=0xf0);//等待松开并输出

    return(cord_h+cord_l);//键盘最后组合码值
   }
  }return(0xff);     //返回该值
}
/*------------------------------------------------
按键值处理函数，返回扫键值
------------------------------------------------*/
unsigned char KeyPro(void)
{
 switch(KeyScan())
 {
  case 0x7e:return 'A'-'0';break;//0 按下相应的键显示相对应的码值
  case 0x7d:return 'B'-'0';break;//1
  case 0x7b:return 'C'-'0';break;//2
  case 0x77:return 'D'-'0';break;//3
  case 0xbe:return 3;break;//4
  case 0xbd:return 6;break;//5
  case 0xbb:return 9;break;//6
  case 0xb7:return 7;break;//7
  case 0xde:return 2;break;//8
  case 0xdd:return 5;break;//9
  case 0xdb:return 8;break;//a
  case 0xd7:return 0;break;//b
  case 0xee:return 1;break;//c
  case 0xed:return 4;break;//d
  case 0xeb:return 7;break;//e
  case 0xe7:return 15;break;//f
  default:return 0xff;break;
 }
}