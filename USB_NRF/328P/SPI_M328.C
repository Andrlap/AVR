/**/

#include "SPI_M328.H"

#include <avr/io.h>
#include <avr/interrupt.h>

void spi_init()
// Initialize pins for spi communication
{


}


void spi_wr_rd_bytes (uint8_t * dataout, uint8_t * datain, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++) {
            SPDR = dataout[i];
            while(!(SPSR & (1<<SPIF)));
            datain[i] = SPDR;
    }
}


void spi_rd_bytes (uint8_t * datain, uint8_t len)
{
	for (uint8_t i = 0; i < len; i++) {
		SPDR = 0;
		while(!(SPSR & (1<<SPIF)));
		datain[i] = SPDR;
	}
}

void spi_wr_bytes (uint8_t * dataout, uint8_t len)
{
    uint8_t i;
    uint8_t data;
    for (i = 0; i < len; i++) {
            SPDR = dataout[i];
            while(!(SPSR & (1<<SPIF)));
		data = SPDR;
    }
}


uint8_t spi_wr_rd_byte (uint8_t data)
{
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}

