/*
 * USART_mega.h
 *
 * Created: 15.10.2013 20:06:09
 *  Author: 1
 */ 


#ifndef USART_MEGA_H_
#define USART_MEGA_H_


/* transmit ready received ready */
#define USART_TX_READY ( UCSR0A & (1<<UDRE0))
#define USART_RX_READY ( UCSR0A & (1<<RXC0))
#define USART_UDR UDR0

#define USART_CALK_BAUD(f, b, x) ((f / (x ? 8 : 16))/ b -1)
#define USART_SET_BAUD(f, b, x) \
    if (x) UCSR0A |=(1<<U2X0); else UCSR0A &= ~(1<<U2X0); \
    UBRR0H = (unsigned char)(USART_CALK_BAUD(f, b, x) >> 8) ; \
	UBRR0L = (unsigned char)USART_CALK_BAUD(f, b, x);
    /* Enable receiver and transmitter */
#define USART_SET_ENABLE UCSR0B = (1<<RXEN0)|(1<<TXEN0)
    /* Set frame format: 8data, 2stop bit */
#define USART_SET_FORMAT UCSR0C = (1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)
#define USART_ERROR (UCSR0A & ((1<<FE)|(1<<DOR)|(1<<PE)))

#endif /* USART_MEGA_H_ */