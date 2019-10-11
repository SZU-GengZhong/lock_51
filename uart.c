#include<reg52.h>
#include "uart.h"
#define uchar unsigned char
#define uint unsigned int

sbit dula=P2^6;//申明U1锁存器的锁存端
sbit wela=P2^7;//申明U2锁存器的锁存端




void uart_init()
{
	TMOD=0x20;//设定T1定时器工作方式2
	TH1=0xfd;//T1定时器装初值
	TL1=0xfd;//T1定时器装初值 
	TR1=1;//启动
	REN=1;//允许串口接收
	SM0=0;//设定串口工作方式1
	SM1=1;//同上
	EA=1;//开总中断
	ES=1;//开串口中断
	dula=0;
	wela=0;

}

