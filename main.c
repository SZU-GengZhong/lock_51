/*-----------------------------------------------
  名称：远程密码锁
------------------------------------------------*/
//包含头文件
#include<reg52.h> 
#include<stdio.h>
#include<intrins.h>
#include"1602.h"
#include"delay.h"
#include"keyboard.h"
#include"uart.h"
#include"dht11.h"

#define uchar unsigned char
#define uint unsigned int

unsigned char code password[]={1,2,3,4,5,6,7,8};//可以更改此密码做多组测试
//定义密码，实际上密码需要存在eeprom中，可以通过程序更改。请用户自行添加
unsigned char str[32],Temp,count;		//这里先不用赋初值，因为单片机初始化的时候会调用一次串口导致count+1
unsigned char mode=1,open_flag=0,send_flag=0;   //1代表正常模式，2代表密码错误暂停模式，3代表开锁模式。 
unsigned int time,time2;
sbit buzzer = P2^3;
sbit lock  = P2^5;


void PutString(unsigned char *TXStr)  
{                
    ES=0;     
     while(*TXStr!=0) 
    {                      
        SBUF=*TXStr;
        while(TI==0);
        TI=0;    
        TXStr++;
    }
    ES=1; 
}   
void init(){
	LCD_Init();         //初始化液晶屏
 	uart_init();		//初始化串口
 
 	DelayMs(10);        //延时用于稳定，可以去掉
 	LCD_Clear();        //清屏
 	LCD_Write_String(0,0,"  Welcome! ");    //写入第一行信息，主循环中不再更改此信息，所以在while之前写入
 	LCD_Write_String(0,1,"Input password!");    //写入第二行信息，提示输入密码
 	count=0;
 	time2=0;
 	TMOD=0x21;	//启用定时器一
 	//设置定时器
    TH0 = 0x3c;
    TL0 = 0xb7;
   	ET0=1;
   	TR0=1;
   	DelayMs(1000);

}
/*------------------------------------------------
                    主程序
------------------------------------------------*/
main()
{
 unsigned char num,i,j; 
 unsigned char* rec_dat;  		   //获得的温湿度数据
               
 unsigned char passwordtemp[16];        //最大输入16个
 unsigned char inputtimes;              //密码输入错误次数
 unsigned char passwordlength,PLEN;     //输入密码长度，实际密码长度
 bit Flag;
 
 PLEN=sizeof(password)/sizeof(password[0]);//用于计算出实际密码长度
 init();
 dht11();
while (1)         //主循环
  {
	//调试使用
    //LCD_Write_Char(10,0,time/10000+'0');
    //LCD_Write_Char(11,0,time/1000%10+'0');
	LCD_Write_Char(11,0,mode+'0');
	LCD_Write_Char(13,0,time/2%100/10+'0');
	//发送温湿度信号标志位
	if(send_flag){
	//获得数据并发送
  	rec_dat = dht11();
	PutString(rec_dat);
	send_flag=0;
	}
	//开门信号标志位
	if(open_flag&&mode!=3)
	{
	
	time2=0;
	lock = !lock;
	buzzer = !buzzer;
	DelayMs(15); 
	buzzer = !buzzer;
	DelayMs(200);
	buzzer = !buzzer;
	DelayMs(15); 
	buzzer = !buzzer;
	open_flag=0;
	mode=3;
	}
	  
	 
 	num=KeyPro();  //扫描键盘
 
 	if(num!=0xff && mode == 1)  //如果扫描是按键有效值则进行处理
   	{ 
    if(i==0)    //输入是第一个字符的时候需要把改行清空，方便观看密码
	  LCD_Write_String(0,1,"                ");//清除该行
	if(i<16)
	  {
       passwordtemp[i]=num;
       LCD_Write_Char(i,1,num+'0');//输入的密码用"*"代替
	   }
	i++;   //输入数值累加
	if((15==num)|| (i==16))//输入按键值15或者密码输入到最大值16，表示输入结束，需要进行比对
	  {
      passwordlength=i-1;  //计算输入密码长度
	  i=0;  //计数器复位
	  if(passwordlength==PLEN)//长度相等则比较，否则直接输出错误
	    {
	     Flag=1;//先把比较位置1
	     for(j=0;j<PLEN;j++)//循环比较8个数值，如果有一个不等 则最终Flag值为0
	        Flag=Flag&&(passwordtemp[j]==password[j]);//比较输入值和已有密码
         }
      if(Flag)//如果比较全部相同，标志位置1
	   {
	    LCD_Write_String(0,1,"                ");//清除该行
        LCD_Write_String(0,1,"Right Open!>>>>");//密码正确显示的信息
	
		open_flag=1;
        inputtimes=0;//输入正确则次数清零，重新计数
        Flag=0;      //清除正确标志
		}
	  else 
	    {
		LCD_Write_String(0,1,"                ");//清除该行
		LCD_Write_String(0,1,"Wrong! Retry!");//密码错误，提示重新输入
		inputtimes++;//连续输入错误，则次数累加
		if(inputtimes==3)
		  {
		   LCD_Write_String(0,1,"                ");//清除该行
		   LCD_Write_String(0,1,"Wrong 3 times!");//密码错误，提示重新输入
		   mode = 2; // 停止计时
		   time2 =0;
		   //切换禁止开锁模式
		   while(mode == 2){
		   if(time2 ==3)
 			 	{
  				mode=1;
				time2=0;
				}
			  LCD_Write_Char(15,0,time2+'0');
		  }
		 
		  }
		}

	  }
    }	
  }		
}
void uart_rx(void) interrupt 4
{ 
if(RI)                        //判断是接收中断产生
     {
      RI=0;                      //标志位清零
      Temp=SBUF;                 //读入缓冲区的值
      
	  
      if(count < 9)
      {	
        str[count] = Temp;
        count++;
      }
      if(count == 9)
      {
         str[count] = '\0';
		 //代表开锁
		 open_flag=1;
		 count=0;
		 LCD_Write_String(0,1,"Right Open!>>>>");
      }                   
                  
     }

    if(TI == 1)    //如果发送完毕             
       TI = 0;      //清除标志.  
       
      

} 
void timer0() interrupt 1
{
    TH0 = 0x3c;
    TL0 = 0xb7;
    time++;
	 

	if(time==20*5) // 定时50ms    定时20次是1秒，乘5即五秒
   {   	
  	time=0;
	send_flag = 1;
	time2++;
	}
	//重新关闭
	if(time2==10&mode==3)
	{
	lock=!lock;
	mode = 1;
	LCD_Write_String(0,1,"Input password!");  
	}
  
     
}
