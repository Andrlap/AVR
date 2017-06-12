#ifndef PIN_H_
#define PIN_H_

#include <stdint.h>
#include <avr/io.h>

#define RF24_PIN_PORT	PORTD
#define RF24_PIN_DDR	DDRD
#define RF24_PIN_CSN_0	4
#define RF24_PIN_CE_0	6
//#define RF24_PIN_CSN_1	5
//#define RF24_PIN_CE_1	3

#define LED_GREEN_OFF   PORTB |= (1<<4)
#define LED_RED_OFF     PORTB |= (1<<3)
#define LED_GREEN_ON	PORTB &= ~(1<<4)
#define LED_RED_ON		PORTB &= ~(1<<3)
#define LED_RED_BL		PORTB ^= (1<<3)
#define LED_GREEN_BL	PORTB ^= (1<<4)
#define LED_INIT		DDRB |= 0x1F; LED_GREEN_OFF; LED_RED_OFF;



#endif /* PIN_H_ */