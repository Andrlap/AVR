/*
 * USB_NRF.c
 *
 * Created: 08.10.2013 20:02:19
 *  Author: 1
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "USART.h"
#include "xitoa.h"
#include "pin.h"
#include "spi.h"
//#include "NRF24L01.H"
//#include "RF24REG.H"
#include "RF24.h"



ISR(USART_RX_vect){
	switch (buff.tp) {
	case 0:
		buff.tp = 1; buff.eof = 0; buff.len = 0;
	case 1:
		break;
	default:
		return;	  
	}
	if (!buff.eof) {
		uint8_t c = USART_UDR;
		buff.eof = (c=='\n')||(c=='\r')||(buff.len>=BUFFER_LEN);
		if (!buff.eof)
			buff.data[buff.len++] = c;
	} 
}





int main(void)
{
	LED_INIT;
	SPI_INIT;
	USART_INIT;
	USART_SET_IRQ;
    MCUCR = 0; // MCUCR = (1<<ISC00)|(1<<ISC01);// over Level
    //   GIMSK |= (1<<INT0); RX1
	
	xfunc_out = usart_send_char;

//	test_register();
//test_io();
	nrf_init();
	scan08_graf();


//	rx08();

	
	//rf24_no = 0;
	
	


}
