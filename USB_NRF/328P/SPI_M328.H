#ifndef SPI_MEGA_H_
#define SPI_MEGA_H_


#define SPI_PORT    PORTB
#define SPI_DDR     DDRB

#define SPI_MISO     PORTB4
#define SPI_MOSI     PORTB3
#define SPI_SS       PORTB2
#define SPI_SCK      PORTB5

#define	SET_BYTE(v,r,s) (v=(v&~r)|s)
#define	SET_BIT(r, b, s) r = (s ? r|b : r&(~b))


// fosc/{2, 4, 8, 16, 32, 64, 128}
#define SPI_MASTER_CLOCK(x)\
	SET_BIT(SPSR, (1<<SPI2X), x&(2|8|32));\
	SET_BIT(SPCR, (1<<SPR1), x&(32|64|128));\
	SET_BIT(SPCR, (1<<SPR0), x&(8|16|128));
//#define SPI_SPCR_SET(x) SET_BYTE(SPCR,(1<<SPIE)|(1<<SPE)|(1<<DORD)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0) ,x)
#define SPI_SPCR_SET(x) SET_BYTE(SPCR, 0 ,x)
#define SPI_MASTER_PORT SET_BYTE(SPI_DDR, ((1<<SPI_MOSI)|(1<<SPI_MISO)|(1<<SPI_SS)|(1<<SPI_SCK)), ((1<<SPI_MOSI)|(1<<SPI_SS)|(1<<SPI_SCK)))
#define SPI_MASTER_ENABLE SPI_SPCR_SET(((1<<SPE)|(1<<MSTR)))

#define SPI_MASTER_INIT SPI_MASTER_PORT; SPI_MASTER_ENABLE;
#define SPI_SS_0 SPI_PORT &= ~(1<<SPI_SS)
#define SPI_SS_1 SPI_PORT |= (1<<SPI_SS)


// SPCR    = (1<<SPE)|(1<<MSTR);
 
#endif /* SPI_MEGA_H_ */