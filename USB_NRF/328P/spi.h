/*2013 Lapotko Andrej*/

#ifndef _SPI_H_
#define _SPI_H_

#include <avr/io.h>
#if defined (__AVR_ATtiny2313__)
#  include "spi_t2313.h"
#elif defined (__AVR_ATtiny2313A__)
#  include "spi_t2313.h"
#elif defined (__AVR_ATmega328P__)
#  include "SPI_M328.H"
#else
#  include "spi_mega.h"

/*#  if !defined(__COMPILING_AVR_LIBC__)
#    warning "spi: device type not defined"
#  endif
*/
#endif

extern void spi_rd_bytes (uint8_t * datain, uint8_t len);
extern void spi_wr_rd_bytes (uint8_t * datain, uint8_t * dataout , uint8_t len);
extern void spi_wr_bytes (uint8_t * dataout, uint8_t len);
extern uint8_t spi_wr_rd_byte (uint8_t data);


#endif /* _SPI_H_ */
