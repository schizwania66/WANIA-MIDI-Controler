//*******************************************************
//* autor: schizwania@gmail.com
//* platform: Atmega32
//* date: 19.05.2025
//* version: 1.0
//* project: Foot MIDI Controler - Black Sun
//*******************************************************

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "MS_MIDI/MS_MIDI.h"
#include "SETTINGS/settings.h"
#include "LCD/LCD.h"
#include "MS_CTRL/CTRL_FUNC.h"

//==============================================================================================
//--------------------------------BACKUP
//G:\Inne komputeravrdude -c usbasp -p m32 -u -U flash:w:WANIA_MIDI_CTRL.hexy\StacjonarnyPC\AVR_WORKSPACE_ECLIPSE\WANIA_MIDI_C

// avrdude -c usbasp -p m32 -u -U flash:w:WANIA_MIDI_C.hex

// avrdude -c usbasp -p m32 -u -U flash:w:WANIA_MIDI_CTRL_LOA.hex
//jkjljk


//Dupa test





//avrdude -p atmega32 -c usbasp -P usb  -U flash:r:"WANIA_MIDI_CTRL_golden_bck1.hex":i
//==============================================================================================

//------------------------------STRUKTURA dla EEPROM'a -------------------------------
#define NUM_PRESETS 12
#define SCENES_PER_PRESET 8
#define PC_MESSAGES_PER_SCENE 5
#define CC_MESSAGES_PER_SCENE 10
#define NAME_LENGTH 16

// Struktura MIDI PC - komunikat Program Change
typedef struct {
    uint8_t channel;    // Kana³ MIDI (0-15)
    uint8_t program;    // Numer programu (0-127)
} MidiPC;

// Struktura MIDI CC - komunikat Control Change
typedef struct {
    uint8_t channel;    // Kana³ MIDI (0-15)
    uint8_t control;    // Numer kontrolera (0-127)
    uint8_t value;      // Wartoœæ kontrolera (0-127)
} MidiCC;

// Struktura pojedynczej sceny
typedef struct {
    MidiPC programChanges[PC_MESSAGES_PER_SCENE];  // 3 komunikaty PC
    MidiCC controlChanges[CC_MESSAGES_PER_SCENE]; // 6 komunikatów CC
} Scene;

// Struktura presetu zawieraj¹cego 6 scen
typedef struct {
    Scene scenes[SCENES_PER_PRESET];
} Preset;

// Struktura ca³ego kontrolera z 6 presetami
typedef struct {
    Preset presets[NUM_PRESETS];
} MidiControllerConfig;

//---------------------------------------------------------------------------------------
//                                           GLOBAL VARIABLES
//---------------------------------------------------------------------------------------
char DefaultMidiChannel = 10;

uint8_t key_lock_btn1; //for WaniaButton
uint8_t key_lock_btn2; //for WaniaButton
uint8_t key_lock_btn3; //for WaniaButton
uint8_t key_lock_btn4; //for WaniaButton
uint8_t key_lock_btn5; //for WaniaButton
uint8_t key_lock_btn6; //for WaniaButton

uint8_t k1, k2, k3, k4, k5, k6;

uint8_t blinkLED_i;
uint8_t i_liczba = 0 ;
uint8_t scene = 0;

uint8_t LED_D_blink=1;

uint8_t action;
uint8_t action1;

//-------------------------------STRUKTURA  PRESETÓW MIDI---------------------------------



//--------------------------------------------------------------------------------------
//                                 MAIN FUNCTION
//--------------------------------------------------------------------------------------
int main (void) {
//--------------------------------------------------------------------------------------
//                                 INIT SECTION
//--------------------------------------------------------------------------------------
	PORTS_Init();	    //inicjalizacja portów/pinów
    USART_Init(__UBRR); //inicjalizacja UART'a
	Timer0_Init();      //Soft PWM
	Timer2_Init();      //Soft Timers
    LCD_Init();         //LCD init

	sei();              //global interrupts
	intro_logo();
	MIDI_CC(4,85,100);  //Pela efektów za³aczona
	lcd_cls();


	//init preset
	MIDI_PC(4,120);  //aktywowanie czystego kana³u
	printNumber(0, 0);
	lcd_locate(0,0);
	lcd_str("C");
	lcd_locate(1,3);
	lcd_str(":");
	lcd_locate(1,4);
	lcd_locate(0,4);
	lcd_str("CLN");
	all_LED_off();
	MIDI_CC(4,85,100);  //PÊTLA XF LOOP ON
	MIDI_CC(2,102,100); //1 effekt (room) Plethora X3 on



//--------------------------------------------------------------------------
//                               ENDLESS LOOP
//--------------------------------------------------------------------------
	while (1){

		MagicWanButton(&k1, &PIN_BTN, BTN_A, 1000, 2000, scene_A, alt_A);
		MagicWanButton(&k2, &PIN_BTN, BTN_B, 1500, 2000, scene_B, alt_B);
		MagicWanButton(&k3, &PIN_BTN, BTN_C, 1000, 2000, scene_C, alt_C);
		MagicWanButton(&k4, &PIN_BTN, BTN_D, 1500, 2000, scene_D, alt_D);
		MagicWanButton(&k5, &PIN_BTN, BTN_E, 1500, 2000, scene_E, alt_E);
		MagicWanButton(&k6, &PIN_BTN, BTN_F, 1000, 1000, scene_F, alt_F);
		//MagicWanButton1(&k6, &PIN_BTN, BTN_F, 1500, 2000, scene_X, scene_X);



		blinkLEDHandler();
		sunriseLEDHandler();

   	 if (!STimer1) {
   	        STimerLED=20;
   	    }
	} // END for ENDLESS LOOP
} // END for MAIN FUNCTION



