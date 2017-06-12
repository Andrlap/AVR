#ifndef PIN_H_
#define PIN_H_

#include <stdint.h>
#include <avr/io.h>

extern uint8_t	rf24_wr2_rd		(uint8_t byte1, uint8_t byte2);
extern void		r24_wrd_bytes	(void * data, uint8_t len);
extern void		r24_wr_rd_bytes	(void * datain, void * dataout, uint8_t len) ;
extern void		rf24_rd_bytes	(uint8_t byte1, uint8_t * bytes, uint8_t len) ;
extern void		rf24_wr_bytes	(uint8_t byte1, uint8_t * bytes, uint8_t len);
extern uint8_t  rf24_cmd_status	(uint8_t byte1);
extern void		rf24_wr_addr5	(uint8_t byte1, uint8_t * addresses, uint8_t addr);
//extern void		init			(uint8_t byte1, uint8_t * addresses, uint8_t addr);

#define LED_GREEN_OFF   //PORTD |= (1<<4)
#define LED_GREEN_ON	//PORTD &= ~(1<<4)
#define LED_GREEN_BL	//PORTD ^= (1<<4)
#define LED_RED_OFF		PORTD |= (1<<2)|(1<<3);
#define LED_RED_ON		PORTD &= ~(1<<2); PORTD |= (1<<3); 
#define LED_RED_BL		PORTD ^= (1<<2);
#define LED_INIT		DDRD |= (1<<2)|(1<<3); LED_RED_OFF;


#define NRF_PORT_0 PORTB
#define NRF_DDR_0 DDRB
#define RF24_PIN_CSN_0	2
#define RF24_PIN_CE_0	1
//#define NRF_PORT_1 PORTB
//#define NRF_DDR_1 DDRB
//#define RF24_PIN_CSN_1	5
//#define RF24_PIN_CE_1	3

extern void rf24_init(void);

#ifdef RF24_PIN_CSN_1

	extern uint8_t rf24_no;
	extern void rf24_set_pin(uint8_t c, uint8_t v);

	#define rf24_CSN_lo	rf24_set_pin(0, 0)
	#define rf24_CSN_hi	rf24_set_pin(0, 1)
	#define rf24_CE_lo	rf24_set_pin(1, 0)
	#define rf24_CE_hi	rf24_set_pin(1, 1)

#else

	#define RF24_PIN_PORT	NRF_PORT_0
	#define RF24_PIN_CSN	(1<<RF24_PIN_CSN_0)
	#define RF24_PIN_CE		(1<<RF24_PIN_CE_0)

	#define rf24_PIN_INIT  {NRF_DDR_0 |= RF24_PIN_CSN|RF24_PIN_CE; NRF_PORT_0 |= RF24_PIN_CSN;}
	#define rf24_CSN_lo		NRF_PORT_0 &= ~	RF24_PIN_CSN;
	#define rf24_CSN_hi		NRF_PORT_0 |=	RF24_PIN_CSN;
	#define rf24_CE_lo		NRF_PORT_0 &= ~	RF24_PIN_CE;
	#define rf24_CE_hi		NRF_PORT_0 |=	RF24_PIN_CE;

#endif

//#define	SET_BIT(v,r,s) (s ? v|r : v&(~r))

//NRF24_SE8R01
#define SE8R01_DR_1M
#define ADR_WIDTH 4
#define BUFFER_LEN 16

#define DADDRESS0	{0xF0, 0xC2, 0xC3, 0xC5, 0xC2}
#define DADDRESS1	{0xF1, 0xC2, 0xC3, 0xC5, 0xC2}
#define DADDRESS2	{0xF2, 0xC2, 0xC3, 0xC5, 0xC2}
#define DADDRESS3	{0xF3, 0xC2, 0xC3, 0xC5, 0xC2}
#define DADDRESS4	{0xF4, 0xC2, 0xC3, 0xC5, 0xC2}
#define DADDRESS5	{0xF5, 0xC2, 0xC3, 0xC5, 0xC2}
extern uint8_t ADDRESS0[];
extern uint8_t ADDRESS1[];
extern uint8_t ADDRESS2[];
extern uint8_t ADDRESS3[];
extern uint8_t ADDRESS4[];
extern uint8_t ADDRESS5[];


#define SPI_RW(b)				rf24_cmd_status(b)
#define SPI_RW_Reg(c, b)		rf24_wr2_rd(c, b)
#define SPI_Write_Buf(c, b, l)	rf24_wr_bytes(c, b, l)


#endif /* PIN_H_ */