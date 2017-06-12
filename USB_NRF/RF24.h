#ifndef SE8R01_H
#define SE8R01_H

//SPEED_1Mbps, SPEED_500Kbps
#define SPEED_RF SPEED_2Mbps
#define ADR_WIDTH 4
#define BUFFER_LEN 16

typedef struct
{	uint8_t status;
	uint8_t data[BUFFER_LEN];
	uint8_t len;
	uint8_t tp; //0 - free; 1 - uart; 2 - nrf
	uint8_t eof;
} t_buff;

extern t_buff buff;

//extern void nf_switch_bank(unsigned char bankindex);
//extern void nf_bank1Init(void);
//extern void SetupPgmArr(uint8_t *  Pos);
//extern void SetupPgmAdr (uint8_t Reg, uint8_t Addr);

extern uint8_t	rf24_wr2_rd		(uint8_t byte1, uint8_t byte2);
extern void		r24_wrd_bytes	(void * data, uint8_t len);
extern void		r24_wr_rd_bytes	(void * datain, void * dataout, uint8_t len) ;
extern void		rf24_rd_bytes	(uint8_t byte1, uint8_t * bytes, uint8_t len) ;
extern void		rf24_wr_bytes	(uint8_t byte1, char * bytes, uint8_t len);
extern uint8_t  rf24_cmd_status	(uint8_t byte1);
extern void		rf24_wr_addr5	(uint8_t byte1, uint8_t * addresses, uint8_t addr);
//extern void		init			(uint8_t byte1, uint8_t * addresses, uint8_t addr);

#define _RegS(r, s) ((s << 5)|(r & 0b11111))


extern void rf24_init(void);

#ifdef RF24_PIN_CSN_1

extern uint8_t rf24_no;
extern void rf24_set_pin(uint8_t c, uint8_t v);

#define rf24_CSN(b)	rf24_set_pin(0, b)
#define rf24_CE(b)	rf24_set_pin(1, b)

#define rf24_CSN_lo	rf24_set_pin(0, 0)
#define rf24_CSN_hi	rf24_set_pin(0, 1)
#define rf24_CE_lo	rf24_set_pin(1, 0)
#define rf24_CE_hi	rf24_set_pin(1, 1)


#else

#define RF24_PIN_CSN	(1<<RF24_PIN_CSN_0)
#define RF24_PIN_CE		(1<<RF24_PIN_CE_0)

#define rf24_CSN(b)		RF24_PIN_PORT = (b)? RF24_PIN_PORT|RF24_PIN_CSN : RF24_PIN_PORT & ~RF24_PIN_CSN;
#define rf24_CE(b)		RF24_PIN_PORT = (b)? RF24_PIN_PORT|RF24_PIN_CE  : RF24_PIN_PORT & ~RF24_PIN_CE;

#define rf24_CSN_lo		RF24_PIN_PORT &= ~	RF24_PIN_CSN;
#define rf24_CSN_hi		RF24_PIN_PORT |=	RF24_PIN_CSN;
#define rf24_CE_lo		RF24_PIN_PORT &= ~	RF24_PIN_CE;
#define rf24_CE_hi		RF24_PIN_PORT |=	RF24_PIN_CE;


#endif

//#define	SET_BIT(v,r,s) (s ? v|r : v&(~r))



#define SPI_RW(b)				rf24_cmd_status(b)
#define SPI_RW_Reg(c, b)		rf24_wr2_rd(c, b)
#define SPI_Write_Buf(c, b, l)	rf24_wr_bytes(c, b, l)

////////////////////////////////

extern void nrf_init (void);
extern void scan08_graf(void);
extern void rx08(void);


#endif /* SE8R01_H */