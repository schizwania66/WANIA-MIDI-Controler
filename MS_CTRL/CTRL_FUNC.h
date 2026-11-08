//*******************************************************
//* autor: schizwania@gmail.com
//* platform: Atmega32
//* date: 19.05.2024
//* version: 1.0
//* project: Foot MIDI Controler - Black Sun
//*******************************************************

#ifndef MS_CTRL_CTRL_FUNC_H_
#define MS_CTRL_CTRL_FUNC_H_

#define LED_RAISE_DOWN_SPEED 70

//--------------declaration of functions for MIDI CTRL
extern volatile enum c_pwm_LED which_LED_pwm;



//klawisz dynamiczny
void MagicWanButton(uint8_t * key_state, volatile uint8_t *KPIN,
		uint8_t key_mask, uint16_t rep_time, uint16_t rep_wait,
		void (*push_proc)(void), void (*rep_proc)(void) );



void scene_A(void);
void scene_B(void);
void scene_C(void);
void scene_D(void);
void scene_E(void);
void scene_F(void);


void alt_A(void);
void alt_B(void);
void alt_C(void);
void alt_D(void);
void alt_E(void);
void alt_F(void);


//===================LED blinking, rising====================
void blinkLEDHandler (void);
void sunriseLEDHandler (void);

void intro_logo(void);
void sunriseLCD(void);
void sunriseLED(void);




#endif /* MS_CTRL_CTRL_FUNC_H_ */
