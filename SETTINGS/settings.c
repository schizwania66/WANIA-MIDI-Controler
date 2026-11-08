//*******************************************************
//* autor: schizwania@gmail.com
//* platform: Atmega32
//* date: 19.05.2024
//* version: 1.0
//* project: Foot MIDI Controler - Black Sun
//*******************************************************

#include <avr/io.h>
#include "settings.h"
#include <avr/interrupt.h>

volatile uint16_t STimer1, STimer2, STimer3, STimer4, STimer5, STimer6, STimer7, STimer8, STimer9, STimer10, STimerLED;

volatile uint8_t pwm_A, pwm_B, pwm_C, pwm_D, pwm_E, pwm_F, pwm_LCD;

//uint8_t blinkLED_i;

volatile uint8_t state_LED_A, state_LED_B, state_LED_C, state_LED_D, state_LED_E, state_LED_F;
volatile uint8_t LED_A_blink, LED_B_blink, LED_C_blink, LED_D_blink, LED_E_blink, LED_F_blink;

uint8_t sunriseLED_i;
uint8_t sunriseLED_flag;

void PORTS_Init(void){
	//-----------------settings for BUTTONS
	PORT_BTN |= BTN_A | BTN_B | BTN_C | BTN_D | BTN_E | BTN_F ;
	//port as input
	DDR_BTN   |= BTN_A | BTN_B | BTN_C | BTN_D | BTN_E | BTN_F ;

	//-------------------settings for LEDs
	DDR_LED   |= LED_A | LED_B | LED_C | LED_D | LED_E | LED_F;
	//port as output
    PORT_LED  |= LED_A | LED_B | LED_C | LED_D | LED_E | LED_F;

    //-------------------settings for LCD's LED
    //port as output
    DDR_LCD_LED |= LCD_LED;
    //LCD LED switch OFF
    PORT_LCD_LED &= ~LCD_LED;

    //initial LEDs switch off
    PORT_LED &= ~LED_A;
    PORT_LED &= ~LED_B;
    PORT_LED &= ~LED_C;
    PORT_LED &= ~LED_D;
    PORT_LED &= ~LED_E;
    PORT_LED &= ~LED_F;
}


//---------------------INIT dla Timers----------------------------

//timer for PWM
void Timer0_Init(void){
	//--------------------------------Timer0 begin-----------------------------------
	//tryb CTC
	TCCR0 |= (1<<WGM01);
	//prescaler 1  dla osiagu 62,5kHz: 16000000/1/256 = 62500Hz
	TCCR0 |= (1<<CS00);
	OCR0  = 255;
	//zezwolenie na przerwanie OVF (przepe³nienie)
	TIMSK |= (1 << OCIE0);
	//----------------------------------Timer0 end------------------------------------
}


void Timer1_Init(void){
	//--------------------------------Timer1 begin-----------------------------------
		//CTC
		TCCR1A |= (1<< WGM12);
	//	//prescaler 64
		TCCR1B |= (1<< CS11) | (1<< CS10);
		//16000000\64\1000(freq)=250
		//OCR1A = 250;
		TIMSK |= (1<< OCIE1B);
	//-------------------------Timer1 end--------------------------------------
}


void Timer2_Init(void){
//--------------------Timer2 begin----//timer for Software Timers--------------
	//CTC mode
	TCCR2 |= (1<<WGM21);
	//ustawienie preskalera
	TCCR2 |= (1<<CS22) ; //preskaler na 128 (16000000/128/1000(Hz) = 125 488Hz ?
	//125 wpisane do rejestru porównania
 	OCR2  |= 124;
	//zezwolenie na przerwanie Compare Match
 	TIMSK |= (1 << OCIE2);
	//--------------------------Timer2 end-------------------------------------
}
//-------------------------tooler functions------------------------
void all_LED_off(void){
	PORT_LED &= ~LED_A;
	PORT_LED &= ~LED_B;
	PORT_LED &= ~LED_C;
	PORT_LED &= ~LED_D;
	PORT_LED &= ~LED_E;
	PORT_LED &= ~LED_F;
	pwm_A=0;
	pwm_B=0;
	pwm_C=0;
	pwm_D=0;
	pwm_E=0;
	pwm_F=0;
}
//----------------------------Przerwania od Timerow--------------------------

//ISR for software PWM dedicated for LEDs
ISR(TIMER0_COMP_vect){
	static uint8_t cnt; // definicja naszego licznika PWM
	//sterowanie PWM dla LED_A - LED_F
    if(cnt>=pwm_A) PORT_LED &= ~(LED_A); else PORT_LED |= (LED_A);
	if(cnt>=pwm_B) PORT_LED &= ~(LED_B); else PORT_LED |= (LED_B);
	if(cnt>=pwm_C) PORT_LED &= ~(LED_C); else PORT_LED |= (LED_C);
	if(cnt>=pwm_D) PORT_LED &= ~(LED_D); else PORT_LED |= (LED_D);
	if(cnt>=pwm_E) PORT_LED &= ~(LED_E); else PORT_LED |= (LED_E);
	if(cnt>=pwm_F) PORT_LED &= ~(LED_F); else PORT_LED |= (LED_F);
	//sterowanie PWM dla LED_LCD
	if(cnt>=pwm_LCD) PORT_LCD_LED &= ~(LCD_LED); else PORT_LCD_LED |= (LCD_LED);

	cnt++;	// zwiêkszanie licznika o 1
}


ISR(TIMER1_COMPA_vect){

}

//ISR for soft timers
ISR(TIMER2_COMP_vect){
	//soft timers /* 1kHz Timer1 */
	uint16_t n;
	n = STimer1;
	if (n) STimer1 = --n;
	n = STimer2;
	if (n) STimer2 = --n;
	n = STimer3;
	if (n) STimer3 = --n;
	n = STimer4;
	if (n) STimer4 = --n;
	n = STimer5;
	if (n) STimer5 = --n;
	n = STimer6;
	if (n) STimer6 = --n;
	n = STimer7;
	if (n) STimer7 = --n;
	n = STimer8;
	if (n) STimer8 = --n;
	n = STimer9;
	if (n) STimer9 = --n;
	n = STimer10;
	if (n) STimer10 = --n;
}

