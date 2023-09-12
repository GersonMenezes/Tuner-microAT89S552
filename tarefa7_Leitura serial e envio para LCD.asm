/*
; Alunos: Gerson e Gustavo
; Configuração da serial: SMod 1, baudrate de 19200 bps e f = 11.0592 MHz.

				$mod51
	
				LCD equ P2		; Pinos de dados para LCD
				EN	equ P3.7	; Enable escrita no LCD
				RS	equ P3.6	; Escrita de controle ou de char no LCD. 0 para controle. 1 para char.
				count equ R5	; Contador de caracteres
					
				org 0000h	; RESET	
					ljmp INIT			; Pula para o início do programa.

				org 0003h	; /INT0
					ljmp ISR_EX0
					
				org 000Bh	; TIMER0
					ljmp ISR_TIMER0

				org 0013h	; /INT1
					ljmp ISR_EX1
					
				org 001Bh	; TIMER1
					ljmp ISR_TIMER1
					
				org 0023h	; SERIAL
					ljmp ISR_SERIAL
					

INIT:			lcall TIMER_CONFIG
				lcall SERIAL_CONFIG
				mov count, #0		
				lcall LCD_CONFIG
				lcall LINE_1		; Cursor para a primeira linha, escreve Reading Serial
				mov DPTR, #MSG1
				lcall LCD_WRITE
				lcall LINE_2		; Cursor para a segunda linha para esperar entradas do teclado
				lcall INTs_CONFIG	; Configuração de interrupção
				sjmp $				; Permanece parado esperando interrupções
	
LCD_WRITE:		mov R4, #0				; Carrega o iterador no acumulador.
LOOP:			mov A, R4				; 
				movc A, @A+DPTR			; Move para o acumulador o conteúdo da posição de memória apontada pelo iterador.
				cjne A, #0FFh, CONTINUE		; Compara pra verificar se a string não chegou ao fim.
				ret						; Se a string chegou ao fim, retorna da função.
CONTINUE:		mov LCD, A				; Do contrário, move o dado para o barramento e
				lcall WR_CHAR			; escreve um caractere.
				lcall DELAY_25MS		; Espera 25 ms.
				inc R4					; Incrementa o iterador.
				sjmp LOOP				; e continua no laço principal.
	
MSG1:			db 'Reading Serial..', 0FFh

LINE_1:			mov LCD, #80h	; Cursor para a primeira linha
				lcall WR_CMD
				ret

LINE_2:			mov LCD, #0C0h	; Cursor para a segunda linha
				lcall WR_CMD
				mov LCD, #0Fh	; Cursor Piscando.
				lcall WR_CMD
				ret
	
WR_CMD:			clr RS		; Write cmd when 0
				nop
				setb EN		; enable para escrita da palavra de controle no LCD
				lcall DELAY_5US
				clr EN
				lcall DELAY_5MS
				ret

WR_CHAR:		
				setb RS		; Write char when 1
				nop
				setb EN		; enable para escrita do char no LCD
				lcall DELAY_5US
				clr EN
				lcall DELAY_5MS
				
				ret

INTs_CONFIG:	mov IE, #90h			; Habilita interrupção serial.
				ret


TIMER_CONFIG:	mov TMOD, #20h			; Configura T/C 1 em modo 8 bits com recarga.
				mov TH1, #253
				mov TL1, #253
				setb TR1
				ret

SERIAL_CONFIG:	mov PCON, #80h			; SMOD = 1.
				mov SCON, #50h			; Modo 1, recepção habilitada (REN = 1).
				ret
				
LCD_CONFIG:		mov LCD, #38h	; Define a matriz 5x7 pontos.
				lcall WR_CMD
				mov LCD, #06h	; Deslocamento do cursor E->D.
				lcall WR_CMD
				mov LCD, #0Eh	; Cursor fixo.
				lcall WR_CMD
				mov LCD, #01h	; Limpa o display.
				lcall WR_CMD
				mov LCD, #02h	; Vai para a primeira linha (opcional).
				lcall WR_CMD
				ret
				
DELAY_5US:		nop
				nop
				nop
				ret

DELAY_5MS:		mov R1, #27
LDL1:			mov R2, #255
LDL2:			djnz R2, LDL2
				djnz R1, LDL1
				ret

DELAY_25MS:		mov R3, #5
LDL3:			lcall DELAY_5MS
				djnz R3, LDL3
				ret

RESET:			mov count, #0		; Reseta quando segunda linha atinge 40 character
				lcall LCD_CONFIG
				lcall LINE_1
				mov DPTR, #MSG1
				lcall LCD_WRITE
				lcall LINE_2
				ret
				
ISR_EX0:		reti					; Retorna da interrupção.
			
ISR_TIMER0:		reti					; Retorna da interrupção.
			
ISR_EX1:		reti					; Retorna da interrupção.

ISR_TIMER1:		reti					; Retorna da interrupção.
			
ISR_SERIAL:		cjne count, #15, continue_serial		; Verifica se tem 16 caracter e liga o scroll
				mov LCD, #07h							; Deslocamento do cursor E->D. Scroll ligado
				lcall WR_CMD
				continue_serial:
				
				cjne count, #39, continue_serial2		; Verifica se tem 40 caractere, se sim, reseta para estado inicial
				lcall RESET
				clr RI
				reti
				continue_serial2:
				
				mov A, SBUF				; Copia o conteúdo do buffer da serial para o acumulador.
				mov LCD, A				; Envia o character para o LCD.
				lcall WR_CHAR
				inc count
				clr RI					; Limpa a flag de detecção de recebimento serial.
				reti					; Retorna da interrupção.

end

*/