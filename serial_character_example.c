// Trabalho piano digital de Micontroladores
// Gerson Menezes e Gustavo Machado
// NO MODO PIANO: Para tocar as notas usar as teclas de letras, ponto e vírgula
// NO MODO COMPOSER: primeiro escolher um character válido, descrito acima, e depois um número de apenas
// um dígito, que será o tempo.

// clk 110592 mhz
// baudrate 19200

/* ------ Teclas, notas correspondentes do piano, frequências e periodo em micro segundos. -------

q =  C1	    32.70   30581.039755352 µs(p) 
w =  D1   	36.71   27240.533914465 µs(p)  
e =  E1	    41.20   24271.844660194 µs(p)   
r =  F1	    43.65   22909.50744559 µs(p)   
t =  G1	    49.00   20408.163265306 µs(p)    
y =  A1	    55.00   18181.818181818 µs(p)    
u =  B1	    61.74   16196.954972465 µs(p)    
i =  C2	    65.41   15288.182235132 µs(p)    
o =  D2	    73.42   13620.266957232 µs(p)    
p =  E2	    82.41   12134.449702706 µs(p)   
a =  F2  	87.31   11453.441759249 µs(p)    
s =  G2  	98.00   10204.081632653 µs(p)    
d =  A2	    110.00  9090.9090909091 µs(p)    
f =  B2	    123.47  8099.133392727 µs(p)    
g =  C3	    130.81  7644.6754835257 µs(p)    
h =  D3	    146.83  6810.5972893823 µs(p)    
j =  E3	    164.81  6067.5929858625 µs(p)   
k =  F3	    174.61  5727.0488517267 µs(p)   
l =  G3  	196.00  5102.0408163265 µs(p)   
ç =  A3	    220.00  4545.4545454545 µs(p)   
z =  B3  	246.94  4049.5666963635 µs(p)  
x =  C4  	261.63  3822.1916446891 µs(p)   
c =  D4	    293.66  3405.2986446911 µs(p)   
v =  E4	    329.63  3033.7044565118 µs(p)   
b =  F4	    349.23  2863.4424304899 µs(p)   
n =  G4	    392.00  2551.0204081633 µs(p)    
m =  A4  	440.00  2272.7272727273 µs(p)   
, =  B4  	493.88  2024.7833481817 µs(p) 
. =  C5	    523.25  1911.132345915 µs(p)   
*/

#include <at89x52.h>
#include <string.h>
#include <math.h>

#define DAC P0
#define LCD P2
#define EN P3_7 // LCD enable
#define RS P3_6 // LCD instruction or dataa

// Prototipos das funçoes
void playComposer();
void reset();
void timer_config();
void serial_config();
void LCD_config();
void INT_config(); // Configura interrupçoes
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
void play_note(unsigned char note, unsigned int rep_of_250ms);
void testNotes();
void timer_config();

// Variaveis globais
char notes[40];
char char_temp;
// flag caso isr da serial ocorra dentro do estado de tocar nota, e n se perca ela
bit flag = 0;
unsigned int count = 0;
unsigned int countNotes = 0;
unsigned char note;
// Mensagens para o lcd
const char code MSG2[] = "Choose mode...";
const char code MSG3[] = "1 Piano 2 Compos";
const char code MSG4[] = "-- PIANO MODE --";
const char code MSG5[] = " Play any key ";
const char code MSG6[] = "COMPOSER MODE";
const char code MSG7[] = "Enter with notes and its time";
const char code MSG8[] = "Playing...";

// aqui fica o array com 8bits superiores, que junto com os inferiores, representam a contagem necessaria para o timer executar o periodo de cada nota
const unsigned char code f_Htimer[] = {
	0xc4, 0xca, 0xd0, 0xd3, 0xd8, 0xdc, 0xe0, 0xe2, 0xe5, 0xe8, 0xe9, 0xec, 0xee, 0xf0, 0xf1, 0xf2, 0xf4, 0xf4, 0xf6, 0xf7, 0xf8, 0xf8, 0xf9, 0xfa, 0xfa, 0xfb, 0xfb, 0xfc, 0xfc};

// aqui fica o array com 8bits inferiores, que junto com os superiores, representam a contagem necessaria para o timer executar o periodo de cada nota
const unsigned char code f_Ltimer[] = {
	0x45, 0xcc, 0x98, 0x41, 0x24, 0x7d, 0x5e, 0x24, 0x66, 0x4d, 0xa1, 0x12, 0x3f, 0x2e, 0x12, 0xb3, 0x26, 0xd0, 0x09, 0x1f, 0x17, 0x89, 0x59, 0x13, 0x68, 0x04, 0x90, 0x0c, 0x44};
// e aqui a quantidade necessária de repetições dos períodos que somados chegam a +-250ms
const unsigned char code reps_notes_to_250ms[] = {
	8, 9, 10, 11, 12, 14, 15, 16, 18, 21, 22, 25, 28, 31, 33, 37, 41, 44, 49, 55, 62, 65, 73, 82, 87, 98, 110, 123, 131
};

// Estados possíveis
#define BEGIN 0
#define MENU_PRINCIPAL 1
#define MSG_PIANO_MODE 100
#define MSG_COMPOSER_MODE 101
#define PIANO_ESPERANDO_NOTA 2
#define TOCAR_NOTA 3
#define COMPOSER_MODE 50
#define PLAYING_COMPOSITION 51
#define RESET 255

unsigned char state = BEGIN;
// Funcao principal
void main(void){
	DAC = 0;
	
	timer_config();
	serial_config();
	LCD_config();
	INT_config();

	while (1){ //ações que acontecem em cada estado
		
		if (state == TOCAR_NOTA){ // Tocar Note e voltar para espera de outra nota

			play_note(char_temp, 1);
			if(!flag){ 
				state = PIANO_ESPERANDO_NOTA;
			}
			flag = 0;
		}
		else if (state == PIANO_ESPERANDO_NOTA){ // Espera de outra nota
			flag = 0;
		}
		else if(state == BEGIN){ // Carrega msg menu
			aponta_line1();
			write_lcd_string(MSG2);
			aponta_line2();
			write_lcd_string(MSG3);
			state = MENU_PRINCIPAL;
		}
		//else if(state == MENU_PRINCIPAL){ // Menu
		//}
		else if (state == MSG_PIANO_MODE){ //transicao msg Piano mode 
			aponta_line1();
			write_lcd_string(MSG4);
			aponta_line2();
			write_lcd_string(MSG5);
			state = PIANO_ESPERANDO_NOTA;
		}
		else if (state == MSG_COMPOSER_MODE){ //transicao msg Composer mode
			aponta_line1();
			write_lcd_string(MSG6);
			aponta_line2();
			write_lcd_string(MSG7);
			aponta_line1();
			write_lcd_string(MSG6);
			state = COMPOSER_MODE;
		}
		//else if (state == COMPOSER_MODE){ // Espera ISR_serial recolher musica
		//}
		else if (state == PLAYING_COMPOSITION){ // Toca composicao e volta para state MSG_COMPOSER_MODE
			aponta_line1();
			write_lcd_string(MSG8);
			playComposer();
			state = MSG_COMPOSER_MODE; 
		}
		else if (state == RESET){	// Resetar
			// unica sessao critica que pareceu necessaria até o momento
			EA = 0;
			reset();
			state = BEGIN;
			EA = 1;
		}
	}
		
}

// Funcao para atender a interrupcao serial
void isr_serial(void) interrupt 4 {	
	
	TF0 = 1;
	TR0 = 0;
	char_temp = SBUF;
	
	if (char_temp == 27){ //Tecla ESC. Reset
		state = RESET;
	}
	else if (state == PIANO_ESPERANDO_NOTA){ // Piano mode
		state = TOCAR_NOTA;
	}
	else if (state == TOCAR_NOTA){ // Piano mode
		flag = 1; // caso esteja já tocando uma nota e outra chegue para ser tocada
		//assim, a main irá acabar a execucao de uma iteracao do while, e entrar novamente para o estado TOCAR_NOTA
	}
	else if (state == MENU_PRINCIPAL) { // Menu mode
		if (char_temp == '1'){ 	//MSG Piano Mode transition
			state = MSG_PIANO_MODE;
		}
		else if (char_temp == '2'){	// MSG Composer Mode transition
			state = MSG_COMPOSER_MODE;
		}
	}
	else if (state == COMPOSER_MODE){ // Composer mode

		if (countNotes == 40 || char_temp == 13){ // caso o número de notas armazenadas chegue a 40 ou usuário pressione enter
			countNotes = 0;
			state = PLAYING_COMPOSITION;
		}
		else{ // caso contrario, apenas pegue os dados do usuario, Nota + numero de repeticoes de 250ms da nota
			// tá ainda sem checagem caso ocorram erros de digitacao...
			notes[countNotes] = char_temp;
			countNotes++;
		}
	}

	RI = 0; // Limpa a flag de interrupcao serial
}

// Funcao para atender a interrupcao timer 1
// não utilizada
void isr_tf1(void) interrupt 3 {
	//DAC = ~DAC; //Quando o timer estoura
}

// Funcao responsável por tocar uma nota, apenas 1 período da mesma
// recebe o index da nota
void play_period_note(unsigned char index){

	//DAC = 0;
	delay_notes(index);
	DAC = ~DAC;
	delay_notes(index);
	DAC = ~DAC; 
}

// Funcao que toca uma nota por X * 250ms
void play_note(unsigned char note, unsigned int rep_of_250ms){

	int note_number = -1;
	// switch converte de ascii para index, que é utizado nos arrays
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
		case 199: // ç, c cedilha em decimal do ascii
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
		temp = reps_notes_to_250ms[note_number] * rep_of_250ms;
		for (; temp > 0; --temp){
			play_period_note(note_number);
		}
	}
}

void playComposer(){ // Toca a musica que foi salva no vetor notes
	char *str = notes;
	char note;
	char time;

	while (*str){
		note = *str++;	// Primeira posicao nota
		time = *str++;	// Segunda posicao tempo, e assim sucessivamente
		play_note(note, time-48); // -48 para subtrair valor do ascii e obter o valor decimal correspondente
	}
}

// ---------------------- Delays ---------------------------
// Tudo meio aproximado...

// Seta o timer0 para criar o periodo de cada nota e espera esse período por poling
void delay_notes(unsigned char index){
	TMOD |= 0x01;
	TH0 = f_Htimer[index];
	TL0 = f_Ltimer[index];
	TR0 = 1;
	while (!TF0);
	TF0 = 0;
	TR0 = 0;
}

void delay_ms(int ms){
	TMOD |= 0x01;
	while (ms){

		TH0 = 0xFC;
		TL0 = 0x65;
		TR0 = 1;
		while (!TF0);
		TF0 = 0;
		TR0 = 0;
		ms--;
	}
}

// 1 ciclo é +- 1 us
void delay_us(int us){
	int i;
	for(i = 0; i < us; ++i){
	}
}

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
	delay_us(1);
	EN = 1;
	delay_us(5);
	EN = 0;
	delay_ms(5);
}

void WR_CHAR(){

	RS = 1;
	delay_us(1);
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
	//serial e global enable
	IE = 0x90;
}

// Configura T/C 1 em modo 8 bits com recarga que é usado na serial
void timer_config(){
	TMOD = 0x20;    
    TH1 = 253;
    TL1 = 253;
    TR1 = 1;
}
