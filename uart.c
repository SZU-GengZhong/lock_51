#include<reg52.h>
#include "uart.h"
#define uchar unsigned char
#define uint unsigned int

sbit dula=P2^6;//����U1�������������
sbit wela=P2^7;//����U2�������������




void uart_init()
{
	TMOD=0x20;//�趨T1��ʱ��������ʽ2
	TH1=0xfd;//T1��ʱ��װ��ֵ
	TL1=0xfd;//T1��ʱ��װ��ֵ 
	TR1=1;//����
	REN=1;//�����ڽ���
	SM0=0;//�趨���ڹ�����ʽ1
	SM1=1;//ͬ��
	EA=1;//�����ж�
	ES=1;//�������ж�
	dula=0;
	wela=0;

}

