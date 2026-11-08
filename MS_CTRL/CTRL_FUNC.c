//*******************************************************
//* autor: schizwania@gmail.com
//* platform: Atmega32
//* date: 19.05.2024
//* version: 1.0
//* project: Foot MIDI Controler - Black Sun
//*******************************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "../SETTINGS/settings.h"
#include "../MS_MIDI/MS_MIDI.h"
#include "../LCD/LCD.h"
#include "CTRL_FUNC.h"

//variables for buttons
volatile uint16_t STimer1, STimer2, STimer3, STimer4, STimer5, STimer6, STimer7, STimer8, STimer9, STimer10;

volatile uint8_t preset_nr=1;

volatile uint8_t state_LED_A, state_LED_B, state_LED_C, state_LED_D, state_LED_E, state_LED_F;
volatile uint8_t LED_A_blink, LED_B_blink, LED_C_blink, LED_D_blink, LED_E_blink, LED_F_blink;

uint8_t sunriseLED_i;
uint8_t sunriseLED_flag;


uint8_t tuner_state =0;

//---Stan klawisza
uint8_t A_switch_state = 0;
uint8_t B_switch_state = 0;
uint8_t C_switch_state = 0;
uint8_t D_switch_state = 0;
uint8_t E_switch_state = 0;
uint8_t F_switch_state = 0;

uint8_t blinkLED_i;

enum c_pwm_LED {
	flash_led_A,
	flash_led_B,
	flash_led_C,
	flash_led_D,
	flash_led_E,
	flash_led_F
};

enum c_which_channel {
	blue,
	red,
	green
};

volatile enum c_which_channel which_preset=blue;
volatile enum c_pwm_LED which_LED_pwm=flash_led_A;

//--------------------funkcja do obsługi przytrzymania klawisza i akcji REPEAT----------------------

///************** funkcja SuperDebounce do obsługi pojedynczych klawiszy ***************
// * ZALETY:
// * 		- nie wprowadza najmniejszego spowalnienia
// * 		- posiada funkcjê REPEAT (powtarzanie akcji dla dłużej wziśniętego klawisza)
// * 		- można przydzielić różne akcje dla trybu REPEAT i pojedynczego klikniêcia
// * 		- można przydzielić tylko jedną akcję wtedy w miejsce drugiej przekazujemy 0 (NULL)
// *
// * Wymagania:
// * 	Timer programowy utworzony w oparciu o Timer sprzętowy (przerwanie 1000Hz)
// *
// * 	Parametry wejściowe:
// *
// * 	*key_state - wskaźnik na zmienną w pamiêci RAM (1 bajt) - do przechowywania stanu klawisza
// *  *KPIN - nazwa PINx portu na którym umieszczony jest klawisz, np: PINB
// *  key_mask - maska klawisza np: (1<<PB3)
// *  rep_time - czas powtarzania funkcji rep_proc w trybie REPEAT
// *  rep_wait - czas oczekiwania do przejœcia do trybu REPEAT
// *  push_proc - wskaźnik do własnej funkcji wywoływanej raz po zwolenieniu przycisku
// *  rep_proc - wskaźnik do własnej funkcji wykonywanej w trybie REPEAT
// **************************************************************************************/
void MagicWanButton(uint8_t * key_state, volatile uint8_t *KPIN,
		uint8_t key_mask, uint16_t rep_time, uint16_t rep_wait,
		void (*push_proc)(void), void (*rep_proc)(void) ) {

	enum {idle, debounce, go_rep, wait_rep, rep};
	//----------domyślne wartości dla zmiennych czsowych
	if(!rep_time) rep_time=200;
	if(!rep_wait) rep_wait=1500;

	uint8_t key_press = !(*KPIN & key_mask);

	if( key_press && !*key_state ) {
		*key_state = debounce;
		//czas na  ustalenie drgań styków
		STimer1 = 40;
	} else
	if( *key_state  ) {

	 	if( key_press && debounce==*key_state && !STimer1 ) {
			//*key_state = 2;
			*key_state = go_rep; //podmiana z 2 na go_rep
			STimer1=40;
		} else
		if( !key_press && *key_state>1 && *key_state<4 ) {
			if(push_proc) push_proc();						/* KEY_UP */
			*key_state=idle;
		} else
		if( key_press && go_rep==*key_state && !STimer1 ) {
			*key_state = wait_rep;
			STimer1=rep_wait;
		} else
		if( key_press && wait_rep==*key_state && !STimer1 ) {
			*key_state = rep;
		} else
		if( key_press && rep==*key_state && !STimer1 ) {
			STimer1 = rep_time;
			if(rep_proc) rep_proc();						/* KEY_REP */
		}
	}
	if( *key_state>=3 && !key_press ) *key_state = idle;

}



void scene_A(void){
	//lcd_cls();
	printNumber(1, 0);

	lcd_locate(0,0);
	lcd_str("R");
	lcd_locate(1,3);
	lcd_str(":");
	lcd_locate(1,4);
	lcd_int(preset_nr);
	lcd_locate(0,4);
	lcd_str("EVH");
	//which_LED_pwm=flash_led_A;
	//which_preset=blue;
	all_LED_off();
	MIDI_CC(4,85,100);  //PĘTLA XF LOOP ON
	MIDI_CC(2,102,100); //1 effekt (room) Plethora X3 on
	MIDI_CC(2,103,50); //1 effekt (room) Plethora X3 on
	MIDI_CC(2,104,50); //1 effekt (room) Plethora X3 on
	MIDI_PC(4,60);     //PC dla aktywowania RED


}

void alt_A(void){
	if(A_switch_state==0){
	//lcd_cls();
	printNumber(1, 0);

	lcd_locate(0,0);
	lcd_str("B");
	lcd_locate(1,3);
	lcd_str(":");
	lcd_locate(1,4);
	lcd_int(preset_nr);
	lcd_locate(0,4);
	lcd_str("EVH");
	//which_LED_pwm=flash_led_A;
	//which_preset=blue;
	all_LED_off();
	MIDI_CC(4,85,100);  //PĘTLA FX LOOP ON
	MIDI_CC(2,102,100); //1 effekt (room) Plethora X3 on

	MIDI_PC(4,10);  //PC dla aktywowania BLUE;
	A_switch_state=1;

	} else {


	}
}

void scene_B(void){
	//lcd_cls();
	if(B_switch_state==0){
		MIDI_CC(4,85,100);  //PĘTLA XF LOOP ON
		MIDI_CC(2,103,100); //Załaczenie  drugiego efekty na  Plethora X3
		B_switch_state=1;
		printNumber(1, 0);
		lcd_locate(1,3);
		lcd_str(":");
		lcd_locate(1,4);
		lcd_int(preset_nr);
		lcd_locate(0,10);
		lcd_str("DEL->1");

		lcd_locate(1,3);
			lcd_str(":");
			lcd_locate(1,4);
			lcd_int(preset_nr);
			lcd_locate(0,4);
			lcd_str("EVH");


	}else
	{
		MIDI_CC(2,103,50); //wyłączenie  drugiego efekty na  Plethora X3
		B_switch_state=0;
		printNumber(1, 0);
		lcd_locate(1,3);
		lcd_str(":");
		lcd_locate(1,4);
		lcd_int(preset_nr);
		lcd_locate(0,10);
		lcd_str("DEL->0");

		lcd_locate(1,3);
			lcd_str(":");
			lcd_locate(1,4);
			lcd_int(preset_nr);
			lcd_locate(0,4);
			lcd_str("EVH");


	}
}
void scene_C(void){
	//lcd_cls();
	if(C_switch_state==0){
		MIDI_CC(4,85,100);  //Pela efektów załaczona
		MIDI_CC(2,104,100); //Załaczenie  3 effektu Plethora X3
		C_switch_state=1;
		printNumber(1, 0);
		lcd_locate(1,3);
		lcd_str(":");
		lcd_locate(1,4);
		lcd_int(preset_nr);
		lcd_locate(1,10);
		lcd_str("REV->1");

	}else
	{  //Pela efektów załaczona
		MIDI_CC(4,85,100);  //Pela efektów załaczona	MIDI_CC(4,85,100);  //Pela efektów załaczona
		MIDI_CC(2,104,50); //Wyłączenie   3 effektu Plethora X3
		C_switch_state=0;
		printNumber(1, 0);
		lcd_locate(1,3);
		lcd_str(":");
		lcd_locate(1,4);
		lcd_int(preset_nr);
		lcd_locate(1,10);
		lcd_str("REV->0");


	}
}
void scene_D(void){
	lcd_cls();
	MIDI_CC(1,43,7); //CC0 value 0 - presets 0-127, value 1 presets 128-256
	printNumber(4, 0);
	lcd_locate(1,3);
	lcd_str(":");
	lcd_locate(1,4);
	lcd_int(preset_nr);
	lcd_locate(0,4);
	lcd_str("EVH_CLEAN1");
	which_LED_pwm=flash_led_D;
	which_preset=green;
	all_LED_off();
	MIDI_PC(4,120);  //PC dla aktywowania kanału CLEAN

}
void scene_E(void){
	lcd_cls();
	//MIDI_CC(1,43,8); //CC0 value 0 - presets 0-127, value 1 presets 128-256
	if(E_switch_state==0){
		MIDI_CC(2,102,100);
		//MIDI_CC(2,103,50); //CC0 value 0 - presets 0-127, value 1 presets 128-256
		//MIDI_CC(2,104,50);
		E_switch_state=1;
		printNumber(5, 0);
		lcd_locate(1,3);
		lcd_str(":");
		lcd_locate(1,4);
		lcd_int(preset_nr);
		lcd_locate(0,4);
		lcd_str("EVH_+_ROOM");
		which_LED_pwm=flash_led_E;
		all_LED_off();
	}else
	{
		MIDI_CC(2,102,50);
		//MIDI_CC(2,103,50); //CC0 value 0 - presets 0-127, value 1 presets 128-256
		//MIDI_CC(2,104,50);
		E_switch_state=0;

		printNumber(5, 0);
		lcd_locate(1,3);
		lcd_str(":");
		lcd_locate(1,4);
		lcd_int(preset_nr);
		lcd_locate(0,4);
		lcd_str("EVH_-_ROOM");
		which_LED_pwm=flash_led_E;
		all_LED_off();
	}


}
void scene_F(void){
	lcd_cls();
	//MIDI_CC(1,43,9); //CC0 value 0 - presets 0-127, value 1 presets 128-256
	//printNumber(6, 0);
	lcd_locate(1,3);
	lcd_str(":");
	lcd_locate(1,4);
	lcd_int(preset_nr);
	lcd_locate(0,4);
	lcd_str("EVH_RED_SUKA");
	//which_LED_pwm=flash_led_F;
	//which_preset=green;
	//all_LED_off();
	MIDI_PC(4,10);  //PC dla aktywowania BLUE;

}
//=====================FUNKCJA TESTOWA=============================

//=======================Alt functions =======================

void alt_B(void){

}
void alt_C(void){
	lcd_cls();
	lcd_locate(0,0);
	lcd_str("PRESET");
	lcd_locate(1,0);
	lcd_str("DOWN:");
	MIDI_CC(2,95,100);
	printNumber(preset_nr, 13);
	preset_nr++;
	if(preset_nr>8) preset_nr=1;
	MIDI_CC(1,0,0);
	MIDI_PC(1,preset_nr);
	printNumber(preset_nr, 13);


}
void alt_D(void){

}

void alt_E(void){

}
void alt_F(void){
	if(tuner_state==0){
		//TUNER ON
		//MIDI_CC(1,45,100); //cortex tuner
		MIDI_CC(2,93,100);   //x3 tuner
		lcd_cls();
		lcd_locate(0,0);
		lcd_str("T");
		printNumber(0, 1);
		lcd_locate(5,0);
		lcd_str("TUNER_MUTE");
		tuner_state=1;
		MIDI_PC(4,120);  //PC dla aktywowania kanału CLEAN
	}
	else
	{
		//TUNER OFF - CURRENT PRESET ON
		//MIDI_CC(1,45,0);  //cortex tuner
		MIDI_CC(2,93,50);   //x3 tuner
		lcd_cls();
		lcd_locate(0,0);
		lcd_str("t");
		printNumber(0, 1);
		lcd_locate(5,0);
		lcd_str("TUNER_OFF");
		tuner_state=0;
		pwm_A=50;
		MIDI_PC(4,60);  //PC dla aktywowania BLUE
	}
}

void scene_test(void * preset){

}


//============================LED blinking, rising

void blinkLEDHandler (void) {
	if(LED_A_blink==1){
		if( !STimer2 ) {
		if (state_LED_A==0){
			pwm_A = 25;
			state_LED_A = 1;
		} else	{
			pwm_A = 0;
			state_LED_A = 0;
		}
		STimer2=1000;
		}
	}
}


//===========================LED rising========================

void sunriseLEDHandler(void){

	if( !STimer3 ) {
		if(sunriseLED_flag == 0 )
		{
			//pwm_A=sunriseLED_i++;
						switch(which_LED_pwm)
						{
						case flash_led_A:
							pwm_A=sunriseLED_i++;
							break;
						case flash_led_B:
							pwm_B=sunriseLED_i++;
							break;
						case flash_led_C:
							pwm_C=sunriseLED_i++;
							break;
						case flash_led_D:
							pwm_D=sunriseLED_i++;
							break;
						case flash_led_E:
							pwm_E=sunriseLED_i++;
							break;
						case flash_led_F:
							pwm_F=sunriseLED_i++;
							break;
						}
			if(sunriseLED_i == 50)
			{
				sunriseLED_flag = 1;
			}
		}

		if(sunriseLED_flag == 1 )
		{
			//pwm_A=sunriseLED_i--;
						switch(which_LED_pwm)
						{
						case flash_led_A:
							pwm_A=sunriseLED_i--;
							break;
						case flash_led_B:
							pwm_B=sunriseLED_i--;
							break;
						case flash_led_C:
							pwm_C=sunriseLED_i--;
							break;
						case flash_led_D:
							pwm_D=sunriseLED_i--;
							break;
						case flash_led_E:
							pwm_E=sunriseLED_i--;
							break;
						case flash_led_F:
							pwm_F=sunriseLED_i--;
							break;
						}
			if(sunriseLED_i == 1)
			{
				sunriseLED_flag = 0;
			}
		}

		//STimer3=LED_RAISE_DOWN_SPEED; //ustawienie dla tempa rozjaśniania LEDa
		STimer3=18; //ustawienie dla tempa rozjaśniania LEDa
	}
}
//function to brighten LCD display
void sunriseLCD(void){

	for(blinkLED_i=0;blinkLED_i<255;blinkLED_i++)
	{
		pwm_LCD=blinkLED_i;
		if(blinkLED_i>50) _delay_ms(4);
		else _delay_ms(5);
	}
	pwm_LCD=255;
}

//function to flash of  LEDs
void sunriseLED(void){

	for(blinkLED_i=0;blinkLED_i<110;blinkLED_i++)
	{
		pwm_A=blinkLED_i;
		pwm_B=blinkLED_i;
		pwm_C=blinkLED_i;
		pwm_D=blinkLED_i;
		pwm_E=blinkLED_i;
		pwm_F=blinkLED_i;
		if(blinkLED_i>50) _delay_ms(4);
		else _delay_ms(5);
	}
	pwm_A=110;
	pwm_B=110;
	pwm_C=110;
	pwm_D=110;
	pwm_E=110;
	pwm_F=110;

	_delay_ms(50);

	// pętla stopniowo ściemniająca 6 diod LED
	for(blinkLED_i=110;blinkLED_i;blinkLED_i--)
	{
		pwm_A=blinkLED_i;
		pwm_B=blinkLED_i;
		pwm_C=blinkLED_i;
		pwm_D=blinkLED_i;
		pwm_E=blinkLED_i;
		pwm_F=blinkLED_i;
		if(blinkLED_i>50) _delay_ms(4);
		else _delay_ms(5);
	}

	pwm_A=0;
	pwm_B=0;
	pwm_C=0;
	pwm_D=0;
	pwm_E=0;
	pwm_F=0;

PORT_LED &= ~LED_A;
PORT_LED &= ~LED_B;
PORT_LED &= ~LED_C;
PORT_LED &= ~LED_D;
PORT_LED &= ~LED_E;
PORT_LED &= ~LED_F;
}

void intro_logo(void){
	lcd_cls();
	lcd_locate(0,1);
	lcd_str("-> IRONCLAD <-");
	lcd_locate(1,3);
	lcd_str(" presents ");
	_delay_ms(200);
    sunriseLCD();
    _delay_ms(600);
	//startup logo
	lcd_cls();
	lcd_locate(0,3);
	lcd_str("BLACK  SUN");
	lcd_locate(1,1);
	lcd_str("MIDI CTRL ver1.0");
    sunriseLED();
    _delay_ms(200);
    lcd_cls();
	MIDI_CC(1,43,4); //CC0 value 0 - presets 0-127, value 1 presets 128-256
	printNumber(1, 0);
	lcd_locate(1,3);
	lcd_str(":");
	lcd_locate(1,4);
	lcd_int(preset_nr);
	lcd_locate(0,4);
	lcd_str("BLUE INIT");
	MIDI_CC(4,85,100);   //zalączenie fx loopa w piecu

}



