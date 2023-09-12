
#include <at89x52.h>
#include <string.h>
#include <math.h>

#define DAC P0
#define LCD P2
#define EN P3_7		// LCD enable
#define RS P3_6		// LCD instruction or dataa

// Prototipos das fun��es
void playComposer();
void play_note(char note, unsigned int ms);
void timer_config();
void reset();
void serial_config();
void LCD_config();
void INT_config();  // Configura interrup��es
void aponta_line1(); // Linha 1 do LCD
void aponta_line2();
void write_lcd_string(char *str);
void WR_CMD();
void WR_CHAR();
void isr_serial(void);	// Interruption
void delay_us(int us);
void delay_ms(int ms);
void nop();

// Variaveis globais
char notes[];
unsigned char char_temp;
int state = 0;
unsigned int count = 0;
unsigned int countNotes = 0;
//char MSG1[] = "Welcome...";
const char code MSG2[] = "Choose mode...";
char MSG3[] = "1 Piano 2 Compos";
char MSG4[] = "-- PIANO MODE --";
char MSG5[] = " Play any key ";
char MSG6[] = "COMPOSER MODE";
char MSG7[] = "Enter with notes and its time";


// Funcao principal
void main(void) {
	
	timer_config();
	serial_config();
	LCD_config();
	aponta_line1();
	write_lcd_string(MSG2);
	aponta_line2();
	write_lcd_string(MSG3);
	INT_config();

  
while (1) {
	// Entra em loop, esperando interrupcoes
	if(state == 3){
		playComposer();
	}
}
}

void aponta_line1(){
	LCD = 0x01;     // Limpa o display.
    WR_CMD();
	LCD = 0x80;    	// Cursor para a primeira linha    
	WR_CMD();
}

void aponta_line2(){
	
	LCD = 0xC0;	//Cursor para a segunda linha
	WR_CMD();
	LCD = 0x0F;	//Cursor Piscando
	WR_CMD();
}

void WR_CMD(){
	
	RS = 0;
	nop();
	EN = 1;
	delay_us(5);
	EN = 0;
	delay_ms(5);
}

void WR_CHAR(){
	
	RS = 1;
	nop();
	EN = 1;
	delay_us(5);
	EN = 0;
	delay_ms(150);
}


// Funcao para escrever uma string na LCD
void write_lcd_string(char *str) {
	count = 0;
	while (*str) {
		LCD = (unsigned char) *str++;
		WR_CHAR();
		count++;
		if(count == 16){	// Liga o scroll
      		LCD = 0x07;
			WR_CMD();
		}
	}
	LCD = 0x06;     // Deslocamento do cursor E->D.
    WR_CMD();
}

// Funcao para atender a interrupcao serial
void isr_serial(void)  interrupt 4 {

    char_temp = SBUF;		// Le o caractere recebido da serial
	//LCD = char_temp;		// Escreve o caractere recebido na LCD
	//WR_CHAR();
    
	if(state == 1){   		 // Piano mode
		if(char_temp == 27){
			reset();
		}
		play_note(char_temp, 1000);

	}else if(state == 2){    // Composer mode
		if(char_temp == 27){
			reset();
		}
		notes[countNotes] = char_temp;
		countNotes++;
		if(countNotes == 40 || char_temp == 13){
			playComposer();
			countNotes = 0;
			state = 3;
		}
		
	}else{					 // Menu mode
		if(char_temp == '1'){
			aponta_line1();
			write_lcd_string(MSG4);
			aponta_line2();
			write_lcd_string(MSG5);
			state = 1;
		}else if(char_temp == '2'){
			aponta_line1();
			write_lcd_string(MSG6);
			aponta_line2();
			write_lcd_string(MSG7);
			aponta_line1();
			write_lcd_string(MSG6);
			state = 2;
		}
	}
    count++;
    RI = 0;			// Limpa a flag de interrupcao serial
}

void playComposer(){
	char *str = notes;
	char note;
	char time;
	IE = 0x00;	// Desliga interrupção serial
	while(*str){
		note = *str++;
		time = *str++;
		play_note(note, time);
	}
	state = 0;
	IE = 0x90;    // Liga interrupção serial
}
//---------------------------- Configura ----------------------------------------------------
void timer_config(){

	TMOD = 0x20;    // Configura T/C 1 em modo 8 bits com recarga.
    TH1 = 253;
    TL1 = 253;
    TR1 = 1;
}

void reset(){
	count = 0;
    LCD_config();
	aponta_line1();
	write_lcd_string(MSG2);
	aponta_line2();
	write_lcd_string(MSG3);
	state = 0;
}

void serial_config() {
    PCON = 0x80;    // SMOD = 1.
    SCON = 0x50;    // Modo 1, recep��o habilitada (REN = 1).
}

void LCD_config() {
    LCD = 0x38;     // Define a matriz 5x7 pontos.
    WR_CMD();
    LCD = 0x06;     // Deslocamento do cursor E->D.
    WR_CMD();
    LCD = 0x0E;     // Cursor fixo.
    WR_CMD();
    LCD = 0x01;     // Limpa o display.
    WR_CMD();
    LCD = 0x02;     // Vai para a primeira linha (opcional).
    WR_CMD();
}

void INT_config(){
	
	IE = 0x90;
}
// ---------------------- Delays ---------------------------

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

void nop(){
}
