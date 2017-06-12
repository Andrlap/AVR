/*
 * spi_t2313.h
 *
 * Created: 16.10.2013 21:42:03
 *  Author: 1
 */ 


#ifndef SPI_T2313_H_
#define SPI_T2313_H_

#define SPI_INIT DDRB |= (1<<PORTB6)|(1<<PORTB7)

#define SPI_WR_RD_BYTE(data, return)  {\
   USIDR = data; USISR |= 1<<USIOIF;\
   while(!(USISR&(1<<USIOIF))){ USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);}\
   return = USIDR;}

#define SPI_RD_BYTES(datain, len) {\
	for (uint8_t i = 0; i < len; i++) {/*USIDR = 0;*/ USISR |= 1<<USIOIF; \
	  while(!(USISR&(1<<USIOIF))){USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);};\
		datain[i] = USIDR;} }
		
#define SPI_WR_BYTES(dataout, len) {\
   for (uint8_t i = 0; i < len; i++) {\
	  USISR |= 1<<USIOIF; USIDR = dataout[i];\
	  while(!(USISR&(1<<USIOIF))){USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);};};}\

/*
extern void spi_rd_bytes (uint8_t * datain, uint8_t len);
extern void spi_wr_rd_bytes (uint8_t * dataout, uint8_t * datain, uint8_t len);
extern void spi_wr_bytes (uint8_t * dataout, uint8_t len);
extern uint8_t spi_wr_rd_byte (uint8_t data);
*/

#endif /* SPI_T2313_H_ */