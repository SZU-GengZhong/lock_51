
#include<reg52.h>

#include "1602.h"

#define uchar unsigned char

#define uint unsigned int






#ifndef __DHT11_H__
#define __DHT11_H__

void DHT11_delay_us(uchar n);

void DHT11_delay_ms(uint z);

uchar* dht11();

#endif
