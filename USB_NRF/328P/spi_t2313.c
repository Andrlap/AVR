#include "spi_t2313.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void spi_rd_bytes (uint8_t * datain, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		USISR |= 1<<USIOIF;
		while(!(USISR&(1<<USIOIF))){USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);};
		datain[i] = USIDR;
	}
}

void spi_wr_rd_bytes (uint8_t * datain, uint8_t * dataout , uint8_t len)
{
       uint8_t i;
       for (i = 0; i < len; i++) {
             USIDR = datain[i];
	 		 USISR |= 1<<USIOIF;
  			 while(!(USISR&(1<<USIOIF))){USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);};
             dataout[i] = USIDR;
       }
}

void spi_wr_bytes (uint8_t * dataout, uint8_t len)
{
       uint8_t i;
       for (i = 0; i < len; i++) {
	 		 USISR |= 1<<USIOIF;
             USIDR = dataout[i];
			 while(!(USISR&(1<<USIOIF))){USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);};
       }
}

uint8_t spi_wr_rd_byte (uint8_t data)
{ 
    /*no avto send*/
	USIDR = data;
	USISR |= 1<<USIOIF;
	while(!(USISR&(1<<USIOIF))){
		USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
	};
	return USIDR;
}

