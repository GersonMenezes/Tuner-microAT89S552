
#include <at89x52.h>
#include <string.h>
#include <math.h>

#define DAC P0
#define LCD P2
#define EN P3_7		// LCD enable
#define RS P3_6		// LCD instruction or dataa
#define count R5

// Prot�tipos das fun��es
void timer_config();
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
void DELAY_5US();
void DELAY_5MS();
void DELAY_25MS();
void nop();

void senoidal();

// Vari�veis globais
int count = 0;
char MSG1[] = "Welcome...";
char MSG2[] = "Choose mode...";
char MSG3[] = "1 Piano 2 Compos";
unsigned char char_temp;

// Fun��o principal
void main(void) {
	
	timer_config();
	serial_config();
	LCD_config();
	aponta_line1();
	write_lcd_string(MSG1);
	aponta_line1();
	write_lcd_string(MSG2);
	aponta_line2();
	write_lcd_string(MSG3);
	INT_config();

  // Entra em loop, esperando interrup��es
  while (1) {
		senoidal(5);
		delay_us(500);
		
		senoidal(15);
		delay_us(500);
  }
}

// senoidal
void senoidal(unsigned char index_note){

	// 256 points for period of the senoidal
    // period of each point in each note frequency
    const unsigned char frequencys[29] = {
        119, 106, 95, 89, 80, 71, 63, 60, 53, 47, 45, 40, 36, 32, 30, 27, 24, 22, 20, 18, 16, 15, 13, 12, 11, 10, 9, 8, 7
    };
    
	unsigned char delay = frequencys[index_note];
	unsigned char i;

    // Print numbers from 0 to 255
    for (i = 0; i <= 255; i = i + 2) {
        DAC = i;
		delay_us(delay)
    }

    // Print numbers from 254 down to 1
    for (i = 254; i >= 0; i = i + 2) {
        DAC = i
		delay_us(delay)
    }

}


void aponta_line1(){

	LCD = 0x80;    			// Cursor para a primeira linha    
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
	DELAY_5US();
	EN = 0;
	DELAY_5MS();
}

void WR_CHAR(){
	
	RS = 1;
	nop();
	EN = 1;
	DELAY_5US();
	EN = 0;
	DELAY_25MS();
}


// Fun��o para escrever uma string na LCD
void write_lcd_string(char *str) {
  while (*str) {
		LCD = (unsigned char) *str++;
    WR_CHAR();
  }
}

// Fun��o para atender a interrup��o serial
void isr_serial(void)  interrupt 4 {
  // Verifica se a interrup��o foi causada por um caractere recebido

    // Verifica se o contador de caracteres atingiu 16
    if (count == 15) {
      // Liga o scroll
      LCD = 0x07;
			WR_CMD();
			
    }
    // Verifica se o contador de caracteres atingiu 40
    else if (count == 39) {
      // Reseta o contador e o display
      count = 0;
      LCD_config();
			aponta_line1();
			write_lcd_string(MSG1);
			aponta_line2();
			RI = 0;
			return;
    }

    // L� o caractere recebido da serial
    char_temp = SBUF;
		// Escreve o caractere recebido na LCD
		LCD = char_temp;
		WR_CHAR();

    // Incrementa o contador de caracteres
    count++;

    // Limpa a flag de interrup��o serial
    RI = 0;
}
//---------------------------- Configura ----------------------------------------------------
void timer_config(){

		TMOD = 0x20;    // Configura T/C 1 em modo 8 bits com recarga.
    TH1 = 253;
    TL1 = 253;
    TR1 = 1;
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

void DELAY_5US() {
    nop();
    nop();
    nop();
}

void DELAY_5MS() {
    unsigned char R1, R2;
    for (R1 = 27; R1 > 0; R1--) {
        for (R2 = 255; R2 > 0; R2--);
    }
}

void DELAY_25MS() {
    unsigned char R3;
    for (R3 = 5; R3 > 0; R3--) {
        DELAY_5MS();
    }
}

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
