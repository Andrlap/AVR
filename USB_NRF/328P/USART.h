
#ifndef AVR_USART_H_
#define AVR_USART_H_ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#if defined (__AVR_ATtiny2313__)
#  include "USART_t2313.h"
#elif defined (__AVR_ATtiny2313A__)
#  include "USART_t2313.h"
#elif defined (__AVR_ATmega328P__)
#  include "USART_M328.h"
#else
#  include "USART_mega.h"
#endif

#define USART_INIT USART_SET_BAUD(F_CPU, 9600, 1); USART_SET_ENABLE; USART_SET_FORMAT;

void usart_get_New(char * buf);
void usart_get_Last(char * buf);
extern void usart_send_buff(unsigned char * buff, unsigned char l);
extern void usart_send_str(char * str);
extern void usart_send_char(char chr);


/* transmit ready */
// mega8 #define usart_tx_ready ( UCSR0A & (1<<UDRE0))
/* received ready*/
// mega8 #define usart_rx_ready (UCSR0A & (1<<RXC0))



#endif
