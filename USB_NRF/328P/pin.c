#include <avr/io.h>
#include "pin.h"
#include "spi.h"


typedef struct {
	uint8_t * port;
	uint8_t c[2]; //[0]csn, [1]ce
} t_rf24;

uint8_t ADDRESS0[ADR_WIDTH]  = DADDRESS0;
uint8_t ADDRESS1[ADR_WIDTH]  = DADDRESS1;
uint8_t ADDRESS2[ADR_WIDTH]  = DADDRESS2;
uint8_t ADDRESS3[ADR_WIDTH]  = DADDRESS3;
uint8_t ADDRESS4[ADR_WIDTH]  = DADDRESS4;
uint8_t ADDRESS5[ADR_WIDTH]  = DADDRESS5;


#ifdef RF24_PIN_CSN_1

	t_rf24 rf24_arr[] = {{&NRF_PORT_0, {RF24_PIN_CSN_0, RF24_PIN_CE_0}}, {&NRF_PORT_0, {RF24_PIN_CSN_1, RF24_PIN_CE_1}}};
	uint8_t rf24_no;

	void rf24_init(void){
//		PORTB = 0;
//		DDRB = 0;
		NRF_PORT_0 = (1<<RF24_PIN_CSN_0)|(1<<RF24_PIN_CSN_1);
		NRF_DDR_0  = (1<<RF24_PIN_CSN_0)|(1<<RF24_PIN_CE_0)|(1<<RF24_PIN_CSN_1)|(1<<RF24_PIN_CE_1);
		rf24_no = 0;
	}

	void rf24_set_pin(uint8_t c, uint8_t v){
		uint8_t data = *(rf24_arr[rf24_no].port);
		uint8_t bit = (1 << rf24_arr[rf24_no].c[c]);
		data = v ? data | bit : data  & ~bit;
		*(rf24_arr[rf24_no].port) = data;
	}
	
#else

	void rf24_init(void){
		NRF_PORT_0 |= (1<<RF24_PIN_CSN_0);
		NRF_DDR_0  |= (1<<RF24_PIN_CSN_0)|(1<<RF24_PIN_CE_0);
	}

#endif

uint8_t rf24_cmd_status(uint8_t byte1) {
	rf24_CSN_lo; //low CSN
	byte1=spi_wr_rd_byte(byte1);
	rf24_CSN_hi; //high CSN
	return byte1;
}

uint8_t rf24_wr2_rd(uint8_t byte1, uint8_t byte2) {
	rf24_CSN_lo; //low CSN
	byte1=spi_wr_rd_byte(byte1);
	byte1=spi_wr_rd_byte(byte2);
	rf24_CSN_hi; //high CSN
	return byte1;
}

void r24_wr_rd_bytes (void * datain, void * dataout, uint8_t len) {
	rf24_CSN_lo;
	spi_wr_rd_bytes((uint8_t *)datain, (uint8_t *)dataout, (uint8_t)len);
	rf24_CSN_hi;
}

void r24_wrd_bytes (void * data, uint8_t len) {
	rf24_CSN_lo;
	spi_wr_rd_bytes((uint8_t *)data, (uint8_t *)data, len);
	rf24_CSN_hi;
}

void rf24_rd_bytes(uint8_t byte1, uint8_t * bytes, uint8_t len) {
	rf24_CSN_lo;
	spi_wr_rd_byte(byte1);
	spi_rd_bytes(bytes, len);
	rf24_CSN_hi;
}

void rf24_wr_bytes(uint8_t byte1, uint8_t * bytes, uint8_t len) {
	rf24_CSN_lo;
	spi_wr_rd_byte(byte1);
	spi_wr_bytes(bytes, len);
	rf24_CSN_hi;
}



