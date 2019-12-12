/*-----------------------------------------------
  ���ƣ�Զ��������
------------------------------------------------*/
//����ͷ�ļ�
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

unsigned char code password[]={1,2,3,4,5,6,7,8};//���Ը��Ĵ��������������
//�������룬ʵ����������Ҫ����eeprom�У�����ͨ��������ġ����û��������
unsigned char str[32],Temp,count;		//�����Ȳ��ø���ֵ����Ϊ��Ƭ����ʼ����ʱ������һ�δ��ڵ���count+1
unsigned char mode=1,open_flag=0,send_flag=0;   //1��������ģʽ��2�������������ͣģʽ��3������ģʽ�� 
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
	LCD_Init();         //��ʼ��Һ����
 	uart_init();		//��ʼ������
 
 	DelayMs(10);        //��ʱ�����ȶ�������ȥ��
 	LCD_Clear();        //����
 	LCD_Write_String(0,0,"  Welcome! ");    //д���һ����Ϣ����ѭ���в��ٸ��Ĵ���Ϣ��������while֮ǰд��
 	LCD_Write_String(0,1,"Input password!");    //д��ڶ�����Ϣ����ʾ��������
 	count=0;
 	time2=0;
 	TMOD=0x21;	//���ö�ʱ��һ
 	//���ö�ʱ��
    TH0 = 0x3c;
    TL0 = 0xb7;
   	ET0=1;
   	TR0=1;
   	DelayMs(1000);

}
/*------------------------------------------------
                    ������
------------------------------------------------*/
main()
{
 unsigned char num,i,j; 
 unsigned char* rec_dat;  		   //��õ���ʪ������
               
 unsigned char passwordtemp[16];        //�������16��
 unsigned char inputtimes;              //��������������
 unsigned char passwordlength,PLEN;     //�������볤�ȣ�ʵ�����볤��
 bit Flag;
 
 PLEN=sizeof(password)/sizeof(password[0]);//���ڼ����ʵ�����볤��
 init();
 dht11();
while (1)         //��ѭ��
  {
	//����ʹ��
    //LCD_Write_Char(10,0,time/10000+'0');
    //LCD_Write_Char(11,0,time/1000%10+'0');
	LCD_Write_Char(11,0,mode+'0');
	LCD_Write_Char(13,0,time/2%100/10+'0');
	//������ʪ���źű�־λ
	if(send_flag){
	//������ݲ�����
  	rec_dat = dht11();
	PutString(rec_dat);
	send_flag=0;
	}
	//�����źű�־λ
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
	  
	 
 	num=KeyPro();  //ɨ�����
 
 	if(num!=0xff && mode == 1)  //���ɨ���ǰ�����Чֵ����д���
   	{ 
    if(i==0)    //�����ǵ�һ���ַ���ʱ����Ҫ�Ѹ�����գ�����ۿ�����
	  LCD_Write_String(0,1,"                ");//�������
	if(i<16)
	  {
       passwordtemp[i]=num;
       LCD_Write_Char(i,1,num+'0');//�����������"*"����
	   }
	i++;   //������ֵ�ۼ�
	if((15==num)|| (i==16))//���밴��ֵ15�����������뵽���ֵ16����ʾ�����������Ҫ���бȶ�
	  {
      passwordlength=i-1;  //�����������볤��
	  i=0;  //��������λ
	  if(passwordlength==PLEN)//���������Ƚϣ�����ֱ���������
	    {
	     Flag=1;//�ȰѱȽ�λ��1
	     for(j=0;j<PLEN;j++)//ѭ���Ƚ�8����ֵ�������һ������ ������FlagֵΪ0
	        Flag=Flag&&(passwordtemp[j]==password[j]);//�Ƚ�����ֵ����������
         }
      if(Flag)//����Ƚ�ȫ����ͬ����־λ��1
	   {
	    LCD_Write_String(0,1,"                ");//�������
        LCD_Write_String(0,1,"Right Open!>>>>");//������ȷ��ʾ����Ϣ
	
		open_flag=1;
        inputtimes=0;//������ȷ��������㣬���¼���
        Flag=0;      //�����ȷ��־
		}
	  else 
	    {
		LCD_Write_String(0,1,"                ");//�������
		LCD_Write_String(0,1,"Wrong! Retry!");//���������ʾ��������
		inputtimes++;//�����������������ۼ�
		if(inputtimes==3)
		  {
		   LCD_Write_String(0,1,"                ");//�������
		   LCD_Write_String(0,1,"Wrong 3 times!");//���������ʾ��������
		   mode = 2; // ֹͣ��ʱ
		   time2 =0;
		   //�л���ֹ����ģʽ
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
if(RI)                        //�ж��ǽ����жϲ���
     {
      RI=0;                      //��־λ����
      Temp=SBUF;                 //���뻺������ֵ
      
	  
      if(count < 9)
      {	
        str[count] = Temp;
        count++;
      }
      if(count == 9)
      {
         str[count] = '\0';
		 //������
		 open_flag=1;
		 count=0;
		 LCD_Write_String(0,1,"Right Open!>>>>");
      }                   
                  
     }

    if(TI == 1)    //����������             
       TI = 0;      //�����־.  
       
      

} 
void timer0() interrupt 1
{
    TH0 = 0x3c;
    TL0 = 0xb7;
    time++;
	 

	if(time==20*5) // ��ʱ50ms    ��ʱ20����1�룬��5������
   {   	
  	time=0;
	send_flag = 1;
	time2++;
	}
	//���¹ر�
	if(time2==10&mode==3)
	{
	lock=!lock;
	mode = 1;
	LCD_Write_String(0,1,"Input password!");  
	}
  
     
}
