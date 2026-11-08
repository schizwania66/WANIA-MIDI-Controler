//*******************************************************
//* autor: schizwania@gmail.com
//* platform: Atmega32
//* date: 19.05.2024
//* version: 1.0
//* project: Foot MIDI Controler - Black Sun
//*******************************************************
#define MIDI_PRESET_COUNT  6
#define MIDI_SCENE_COUNT   6
#define MIDI_PC_COUNT      3
#define MIDI_CC_COUNT      6
#define EEPROM_VERSION     1



extern volatile uint16_t STimer1, STimer2, STimer3, STimer4, STimer5, STimer6, STimerLED;

extern volatile uint8_t pwm_A, pwm_B, pwm_C, pwm_D, pwm_E, pwm_F, pwm_LCD;


//------------makra dla PORTS
#define PORT_BTN PORTA
#define PORT_LED PORTB
//------nazwa rejestru kierunku portu---------
#define DDR_BTN DDRA
#define DDR_LED DDRB

#define PIN_BTN PINA

//port dla pinu dla podswietlenia LCD
#define PORT_LCD_LED PORTD
#define DDR_LCD_LED DDRD

//----------definitions of pins for BUTTONS----------
#define BTN_A (1<<PA1) ///wire error
#define BTN_B (1<<PA0)
#define BTN_C (1<<PA2)
#define BTN_D (1<<PA3)
#define BTN_E (1<<PA4)
#define BTN_F (1<<PA5)

//---------piny led-----------
#define LED_A (1<<PB0)
#define LED_B (1<<PB1)
#define LED_C (1<<PB2)
#define LED_D (1<<PB3)
#define LED_E (1<<PB4)
#define LED_F (1<<PB5)

#define LCD_LED  (1<<PD7)

// ---------------------------deklaracje funkcji
void PORTS_Init (void);
void Timer0_Init(void);
void Timer1_Init(void);
void Timer2_Init(void);

void blinkLEDHandler (void);
void all_LED_off(void);
