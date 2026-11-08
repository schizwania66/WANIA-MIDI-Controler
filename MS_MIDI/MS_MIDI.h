//*******************************************************
//* autor: schizwania@gmail.com
//* platform: Atmega32
//* date: 19.05.2024
//* version: 1.0
//* project: Foot MIDI Controler - Black Sun
//*******************************************************

#ifndef MS_MIDI_H_
#define MS_MIDI_H_


#include <avr/io.h>
#include <util/delay.h>


//---------------------MIDI MACROS------------------------
// Definitions and macros of voice messages
#define STATUS_CTL_CHG  0xB0
#define STATUS_PROG_CHG 0xC0


#define UART_BAUD 31250		// tu definiujemy interesuj¹c¹ nas prêdkoœæ
#define __UBRR ((F_CPU+UART_BAUD*8UL)/(16UL*UART_BAUD)-1)  // obliczamy UBRR dla U2X=0

// definicje na potrzeby RS485
#define UART_DE_PORT PORTD
#define UART_DE_DIR DDRD
#define UART_DE_BIT (1<<PD2)

#define UART_DE_ODBIERANIE  UART_DE_PORT &= ~UART_DE_BIT
#define UART_DE_NADAWANIE  UART_DE_PORT |= UART_DE_BIT

#define UART_RX_BUF_SIZE 8 // definiujemy bufor o rozmiarze 32 bajtów
// definiujemy maskê dla naszego bufora
#define UART_RX_BUF_MASK ( UART_RX_BUF_SIZE - 1)

#define UART_TX_BUF_SIZE 50 // definiujemy bufor o rozmiarze 16 bajtów
// definiujemy maskê dla naszego bufora
#define UART_TX_BUF_MASK ( UART_TX_BUF_SIZE - 1)

extern volatile uint8_t ascii_line;

// deklaracje funkcji publicznych

void USART_Init( uint16_t baud );

int uart_getc(void);
void uart_putc( char data );
void uart_puts(char *s);
void uart_putint(int value, int radix);

char * uart_get_str(char * buf);

void UART_RX_STR_EVENT(char * rbuf);
void register_uart_str_rx_event_callback(void (*callback)(char * pBuf));


//------------------------------MIDI FUNCTIONS declarations -------------------------------------

void  MIDI_CC(char MidiChannel, char CC_NUMBER, char CC_VALUE);
void  MIDI_PC(char MidiChannel, char PC_VALUE);


#endif /* MS_MIDI_H_ */

