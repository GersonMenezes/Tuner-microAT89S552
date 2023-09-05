#include <at89x52.h>
#include <string.h>

#define CS			P1_0
#define	CLK			P1_1
#define	DO			P1_2
#define LCD			P2
#define EN			P3_6
#define RS			P3_7

void ConfigLCD(void);
void Line1(void);
void Line2(void);
void WrSTR(char msg[]);
void delay_us(int us);
void delay_ms(int ms);
void WrCMD(void);
void WrCHAR(void);
void WrNUM(float num);
unsigned char ReadADC(void);

void main(void)
{
	unsigned char VD = 0;
	float VA = 0;
	CS = 1;
	
	ConfigLCD();
	Line1();
	WrSTR("Tensao lida [V]:");
	Line2();
	
	while(1){
		VD = ReadADC();
		P0 = VD;
		VA = 5*VD/255;     // 2^b - 1
		delay_ms(250);
		WrNUM(VA);
	}
}

void ConfigLCD(void)
{
	LCD = 0x38;
	WrCMD();
	LCD = 0x06;
	WrCMD();
	LCD = 0x0E;
	WrCMD();
	LCD = 0x01;
	WrCMD();
}

void Line1(void)
{	
	LCD = 0x00;
	WrCMD();
}

void Line2(void)
{
	LCD = 0xC0;
	WrCMD();
}

void WrSTR(char msg[])
{
	unsigned char i, L;
	L = strlen(msg);
	for(i = 0; i < L; i++)
	{
		LCD = msg[i];
		WrCHAR();
	}
}

void WrNUM(float num)
{
    unsigned char c = 0, d = 0, u = 0;

    num = num*100;
		
		c = num / 100;
	
		num = num - 100*c;
		d = num / 10;
		num = num - 10*d;
		u = num;
	
		c += 0x30;
		d += 0x30;
		u += 0x30;
	
		LCD = c;		
    WrCHAR();
		WrSTR(".");
		LCD = d;		
    WrCHAR();
		LCD = u;		
    WrCHAR();
		LCD = 0xC0;			// Retorna para a primeira posi??o na escrita subsequente.
		WrCMD();
}

void delay_us(int us)
{
	unsigned char i;
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

void WrCMD(void)
{
	RS = 0;
	EN = 1;
	delay_us(5);
	EN = 0;
	delay_ms(5);
}

void WrCHAR(void)
{
	RS = 1;
	EN = 1;
	delay_us(5);
	EN = 0;
	delay_ms(5);
}

unsigned char ReadADC(void){
	unsigned char DATA = 0;
	int i;
	
	CS = 0;
	delay_us(10);
	CLK = 1;
	delay_us(10);
	for(i = 0; i < 9; i++){
		CLK = 0;
		delay_us(10);
		DATA = DATA << 1;
		if(DO){
			DATA++;
			// ou MY_DATA |= DO;
		}	
		CLK = 1; 
		delay_us(10);
	}
	CS = 1;
	return DATA;
}