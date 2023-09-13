#include <at89x52.h>
#include <string.h>
#include <math.h>

#define DAC P0
#define LCD P2
#define EN P3_7 // LCD enable
#define RS P3_6 // LCD instruction or dataa

// Prototipos das fun??es
void playComposer();
void reset();
void nop();
void timer_config();
void serial_config();
void LCD_config();
void INT_config(); // Configura interrup??es
void aponta_line1(); // Linha 1 do LCD
void aponta_line2();
void write_lcd_string(char *str);
void WR_CMD();
void WR_CHAR();
void isr_serial(void); // Interruption
void delay_notes(unsigned char index);
void delay_us(int us);
void delay_ms(int ms);
void play_period_note(unsigned char index);
void play_note(unsigned char note, unsigned int ms);

// Variaveis globais
char notes[40];
char char_temp;
int state = 0;
unsigned int count = 0;
unsigned int countNotes = 0;

// char MSG1[] = "Welcome...";
const char code MSG2[] = "Choose mode...";
const char code MSG3[] = "1 Piano 2 Compos";
const char code MSG4[] = "-- PIANO MODE --";
const char code MSG5[] = " Play any key ";
const char code MSG6[] = "COMPOSER MODE";
const char code MSG7[] = "Enter with notes and its time";
const unsigned char code f_Htimer[] = {
	0xc4, 0xca, 0xd0, 0xd3, 0xd8, 0xdc, 0xe0, 0xe2, 0xe5, 0xe8, 0xe9, 0xec, 0xee, 0xf0, 0xf1, 0xf2, 0xf4, 0xf4, 0xf6, 0xf7, 0xf8, 0xf8, 0xf9, 0xfa, 0xfa, 0xfb, 0xfb, 0xfc, 0xfc};
const unsigned char code f_Ltimer[] = {
	0x45, 0xcc, 0x98, 0x41, 0x24, 0x7d, 0x5e, 0x24, 0x66, 0x4d, 0xa1, 0x12, 0x3f, 0x2e, 0x12, 0xb3, 0x26, 0xd0, 0x09, 0x1f, 0x17, 0x89, 0x59, 0x13, 0x68, 0x04, 0x90, 0x0c, 0x44};
const unsigned char code reps_notes[] = {
	35, 40, 45, 47, 53, 60, 67, 71, 80, 89, 95, 106, 119, 134, 142, 159, 179, 189, 213, 239, 268, 284, 319, 358, 379, 425, 477, 536, 568
};

// Funcao principal
void main(void){
	serial_config();
	LCD_config();
	aponta_line1();
	//write_lcd_string(MSG2);
	aponta_line2();
	//write_lcd_string(MSG3);
	INT_config();

	while (1){
		LCD = 'q';
		WR_CHAR();
		play_note('q', 10);
		delay_ms(1000);
		
		LCD = 'w';
		WR_CHAR();
		play_note('w', 10);
		delay_ms(1000);

		LCD = 'e';
		WR_CHAR();
		play_note('e', 10);
		delay_ms(1000);

		LCD = 'r';
		WR_CHAR();
		play_note('r', 10);
		delay_ms(1000);

		LCD = 't';
		WR_CHAR();
		play_note('t', 10);
		delay_ms(1000);

		LCD = 'y';
		WR_CHAR();
		play_note('y', 10);
		delay_ms(1000);

		LCD = 'u';
		WR_CHAR();
		play_note('u', 10);
		delay_ms(1000);
		
		LCD = 'i';
		WR_CHAR();
		play_note('i', 10);
		delay_ms(1000);

		LCD = 'o';
		WR_CHAR();
		play_note('o', 10);
		delay_ms(1000);

		LCD = 'p';
		WR_CHAR();
		play_note('p', 10);
		delay_ms(1000);

		LCD = 'a';
		WR_CHAR();
		play_note('a', 10);
		delay_ms(1000);

		LCD = 's';
		WR_CHAR();
		play_note('s', 10);
		delay_ms(1000);

		LCD = 'd';
		WR_CHAR();
		play_note('d', 10);
		delay_ms(1000);

		LCD = 'f';
		WR_CHAR();
		play_note('f', 10);
		delay_ms(1000);

		LCD = 'g';
		WR_CHAR();
		play_note('g', 10);
		delay_ms(1000);

		LCD = 'h';
		WR_CHAR();
		play_note('h', 10);
		delay_ms(1000);

		LCD = 'j';
		WR_CHAR();
		play_note('j', 10);
		delay_ms(1000);

		LCD = 'k';
		WR_CHAR();
		play_note('k', 10);
		delay_ms(1000);
		
		LCD = 'l';
		WR_CHAR();
		play_note('l', 10);
		delay_ms(1000);

		LCD = 199;
		WR_CHAR();
		play_note(199, 10);
		delay_ms(1000);

		LCD = 'z';
		WR_CHAR();
		play_note('z', 10);
		delay_ms(1000);

		LCD = 'x';
		WR_CHAR();
		play_note('x', 10);
		delay_ms(1000);

		LCD = 'c';
		WR_CHAR();
		play_note('c', 10);
		delay_ms(1000);

		LCD = 'v';
		WR_CHAR();
		play_note('v', 10);
		delay_ms(1000);

		LCD = 'b';
		WR_CHAR();
		play_note('b', 10);
		delay_ms(1000);

		LCD = 'n';
		WR_CHAR();
		play_note('n', 10);
		delay_ms(1000);

		LCD = 'm';
		WR_CHAR();
		play_note('m', 10);
		delay_ms(1000);

		LCD = '.';
		WR_CHAR();
		play_note('.', 10);
		delay_ms(1000);

		LCD = ';';
		WR_CHAR();
		play_note(';', 10);
		delay_ms(1000);
		// Entra em loop, esperando interrupcoes
		/*
		if (state == 3)
		{

			playComposer();
		}
		*/
	}
}

void play_period_note(unsigned char index){

	DAC = 0;
	delay_notes(index);
	DAC = 255;
	delay_notes(index);
	DAC = 0;
}

void play_note(unsigned char note, unsigned int ms){

	int note_number = -1;
	switch (note) {
		case 'q':
			note_number = 0;
			break;
		case 'w':
			note_number = 1;
			break;
		case 'e':
			note_number = 2;
			break;
		case 'r':
			note_number = 3;
			break;
		case 't':
			note_number = 4;
			break;
		case 'y':
			note_number = 5;
			break;
		case 'u':
			note_number = 6;
			break;
		case 'i':
			note_number = 7;
			break;
		case 'o':
			note_number = 8;
			break;
		case 'p':
			note_number = 9;
			break;
		case 'a':
			note_number = 10;
			break;
		case 's':
			note_number = 11;
			break;
		case 'd':
			note_number = 12;
			break;
		case 'f':
			note_number = 13;
			break;
		case 'g':
			note_number = 14;
			break;
		case 'h':
			note_number = 15;
			break;
		case 'j':
			note_number = 16;
			break;
		case 'k':
			note_number = 17;
			break;
		case 'l':
			note_number = 18;
			break;
		case 199:
			note_number = 19;
			break;
		case 'z':
			note_number = 20;
			break;
		case 'x':
			note_number = 21;
			break;
		case 'c':
			note_number = 22;
			break;
		case 'v':
			note_number = 23;
			break;
		case 'b':
			note_number = 24;
			break;
		case 'n':
			note_number = 25;
			break;
		case 'm':
			note_number = 26;
			break;
		case ',':
			note_number = 27;
			break;
		case '.':
			note_number = 28;
			break;
		default:
			// Handle the case where note does not match any of the above values
			break;
	}

	if (note_number >= 0){
		unsigned int temp;
		for (temp = reps_notes[note_number]; temp > 0; --temp){
			play_period_note(note_number);
		}
	}
}

// Funcao para atender a interrupcao serial
void isr_serial(void) interrupt 4 {	
	/*
	char_temp = SBUF;
	if (state == 1){ // Piano mode

		if (char_temp == 27){
			reset();
		}
		play_note(char_temp, 1000);
	}
	else if (state == 2){ // Composer mode

		if (char_temp == 27){
			reset();
		}
		else if (countNotes == 40 || char_temp == 13){
			playComposer();
			countNotes = 0;
			state = 3;
		}
		else{
			notes[countNotes] = char_temp;
			countNotes++;
		}
	}
	else{ // Menu mode

		if (char_temp == '1'){

			aponta_line1();
			write_lcd_string(MSG4);
			aponta_line2();
			write_lcd_string(MSG5);
			state = 1;
		}
		else if (char_temp == '2'){
			aponta_line1();
			write_lcd_string(MSG6);
			aponta_line2();
			write_lcd_string(MSG7);
			aponta_line1();
			write_lcd_string(MSG6);
			state = 2;
		}
		else{
			reset();
		}
	}
	*/
	RI = 0; // Limpa a flag de interrupcao serial
}

void playComposer(){
	char *str = notes;
	char note;
	char time;

	//IE = 0x00; // Desliga interrup��o serial

	while (*str){
		note = *str++;
		time = *str++;
		play_note(note, time);
	}
	state = 0;
	//IE = 0x90; // Liga interrup��o serial
}

// ---------------------- Delays ---------------------------

void delay_notes(unsigned char index){
	TMOD |= 0x10;
	TH1 = f_Htimer[index];
	TL1 = f_Ltimer[index];
	TR1 = 1;
	while (!TF1);
	TF1 = 0;
	TR1 = 0;
}

void delay_ms(int ms){
	TMOD |= 0x01;
	while (ms){
		TL0 = 0x65;
		TH0 = 0xFC;
		TR0 = 1;
		while (!TF0);
		TF0 = 0;
		TR0 = 0;
		ms--;
	}
}

void delay_us(int us){
	int i;
	for(i = 0; i < us; ++i){
	}
}

void nop(){}

// -- LCD functions

void aponta_line1(){

	LCD = 0x01; // Limpa o display.
	WR_CMD();
	LCD = 0x80; // Cursor para a primeira linha
	WR_CMD();
}

void aponta_line2(){

	LCD = 0xC0; // Cursor para a segunda linha
	WR_CMD();
	LCD = 0x0F; // Cursor Piscando
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
	delay_ms(50);
}

// Funcao para escrever uma string na LCD
void write_lcd_string(char *str){

	count = 0;
	while (*str){
		LCD = (unsigned char)*str++;
		WR_CHAR();
		count++;
		if (count == 16){ // Liga o scroll
			LCD = 0x07;
			WR_CMD();
		}
	}
	LCD = 0x06; // Deslocamento do cursor E->D.
	WR_CMD();
}

//---------------------------- Configura ----------------------------------------------------

void reset(){
	count = 0;
	LCD_config();
	aponta_line1();
	write_lcd_string(MSG2);
	aponta_line2();
	write_lcd_string(MSG3);
	state = 0;
}

void serial_config(){
	PCON = 0x80; // SMOD = 1.
	SCON = 0x50; // Modo 1, recep??o habilitada (REN = 1).
}

void LCD_config(){
	LCD = 0x38; // Define a matriz 5x7 pontos.
	WR_CMD();
	LCD = 0x06; // Deslocamento do cursor E->D.
	WR_CMD();
	LCD = 0x0E; // Cursor fixo.
	WR_CMD();
	LCD = 0x01; // Limpa o display.
	WR_CMD();
	LCD = 0x02; // Vai para a primeira linha (opcional).
	WR_CMD();
}

void INT_config(){
	IE = 0x90;
}
