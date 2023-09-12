/*

#include <at89x52.h>
#include <string.h>
#include <math.h>

#define CS			P1_0
#define	CLK			P1_1
#define	DO			P1_2
#define LCD			P2
#define EN			P3_6
#define RS			P3_7
#define PI      3.14159265359

void delay_us(int us);
void delay_ms(int ms);

void main(void)
{
	
	int sinal = 1;
	unsigned char VD = 0;
	float VA = 0;
	float out = 0;
	int tmp = 1;
	int delay = 0;
	int tensao = 0;
	CS = 1;
	
	delay = 10; //ms
	tensao = 1;
	/*
	while(1){
		
			LCD = tmp;
			if(tmp > 50){
				sinal = sinal * (-1);
				tmp = 50;
			}else if(tmp < 1){
				sinal = sinal * (-1);
				tmp = 1;
			}
			tmp = tmp + (1 * sinal);
			delay_us(100);
	}*/
	/*		
	while(1){
		
		LCD = 0;
		delay_us(500);
		LCD = 10;
		delay_us(500);
		LCD = 50;
		delay_us(500);
		LCD = 60;
		delay_us(500);
		LCD = 70;
		delay_us(500);

	}*/
	/*
	while(1){
		
		//out = sin(2*PI*60*tmp*delay/1000);
		//LCD = (unsigned char)(127 + 127*out);
		LCD = (unsigned char) tensao;
		tensao = tensao + (1*sinal);
		//tmp = tmp+1;
		delay_us((delay*1000)/500);
		
		// Período
		if(tensao > 254 || tensao < 1){
			sinal = sinal * (-1);
		}
	}
}*/


/*

void delay_us(int us)
{
	int i;
	for(i = 0; i < us; i++){}
}

void delay_ms(int ms){
	TMOD |= 0x01;
	
	while(ms){
		TL0 = 0x65;
		TH0 = 0xFC;
		TR0 = 1;
		while(!TF0);
		TF0 = 0;
		TR0 = 0;
		ms--;
	}
}
*/