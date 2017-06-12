/*
 * USART_t2313.h
 *
 * Created: 15.10.2013 20:06:39
 *  Author: 1
 */ 


#ifndef USART_T2313_H_
#define USART_T2313_H_


/* transmit ready received ready */
#define USART_TX_READY (UCSRA & (1<<UDRE))
#define USART_RX_READY (UCSRA & (1<<RXC))
#define USART_UDR UDR

#define USART_CALK_BAUD(f, b, x) ((f / (x ? 8 : 16))/ b -1)
#define USART_SET_BAUD(f, b, x) \
    if (x) UCSRA |=(1<<U2X); else UCSRA &= ~(1<<U2X); \
    UBRRH = (unsigned char)USART_CALK_BAUD(f, b, x)>>8 ; \
	UBRRL = (unsigned char)USART_CALK_BAUD(f, b, x);
    /* Enable receiver and transmitter */
#define USART_SET_ENABLE UCSRB = (1<<RXEN)|(1<<TXEN)
#define USART_SET_IRQ  UCSRB |= (1<<RXCIE)

    /* Set frame format: 8data, 2stop bit */
#define USART_SET_FORMAT UCSRC = (1<<USBS)|(3<<UCSZ0)
#define USART_ERROR (UCSRA & ((1<<FE)|(1<<DOR)|(1<<UPE)))



#endif /* USART_T2313_H_ */