#include <at89x52.h>

#define		DELAY				1

void delay_1second(void);
void updateDisplay(void);
void delay(int x);
void increment(void);
void internalUpdate(void);
unsigned char incrementNibble(unsigned char byte);

static int RUN = 0;
static int timerFlagCounter = 0;
static int seconds = 0;
static int minutes = 0;
static int flip = 0;

static unsigned char timeD0 = 0x01;
static unsigned char timeD1 = 0x00;
static unsigned char timeD2 = 0x00;
static unsigned char timeD3 = 0x00;

void main(void){
	
	IE = 0x8F;
	
	//IT0 = 0;
	//IT1 = 0;
	
	while(RUN == 0);
	
	//IT0 = 0;
	//IT1 = 0;
	
	while(1){
		updateDisplay();
		//delay_1second();
	};
	
}

void delay(int x){
	int i = 0;
	
	for(i = 0; i < x; i++){
		
		TMOD = 0x01;
		TL1 = 0xAF;
		TH1 = 0x3C;
		TR1 = 1;
		
		while(TF1 == 0);
		
		TF1 = 0;
	}	
}

void updateDisplay(){
	
	// Display 0
	
	P2 = timeD0;
	P2_4 = 0;
	P2_5 = 0;
	delay(DELAY);
	
	// Display 1
	
	P2 = timeD1;
	P2_4 = 1;
	P2_5 = 0;
	delay(DELAY);
	
	// Display 2
	
	P2 = timeD2;
	P2_4 = 0;
	P2_5 = 1;
	delay(DELAY);
	
	// Display 3
	
	P2 = timeD3;
	P2_4 = 1;
	P2_5 = 1;
	delay(DELAY);
}

void increment(void){
	timeD0 = 0x01;
	seconds++;
	if(seconds % 60 == 0 && seconds != 0){
		minutes++;
		seconds = 0;
	}
}

unsigned char incrementNibble(unsigned char byte) {
	
    unsigned char nibble = byte & 0x0F; // Pegue os 4 bits menos significativos.

    nibble++; // Incrementa o nibble.

    if (nibble > 9) {
        nibble = 0; // Volta para 0 após 9.
    }

    byte = (byte & 0xF0) | (nibble & 0x0F); // Atualize o byte com o nibble atualizado.

    return byte;
}

void delay_1second(){
	
	TMOD = 0x01;
	
	if(timerFlagCounter < 20){
		TL0 = 0xAF;
		TH0 = 0x3C;
		TR0 = 1;
	}else{
		increment();
		TF0 = 0;
		timerFlagCounter = 0;
	}
}

void secondCounter(void) interrupt 1{
		timerFlagCounter++;
		TF0 = 0;
}

void start(void) interrupt 0{
	RUN = 1;
	return;
}

void stop(void) interrupt 2{
	RUN = 0;
	return;
}