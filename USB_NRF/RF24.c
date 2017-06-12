#include <util/delay.h>
#include <avr/pgmspace.h>
#include "pin.h"
#include "RF24REG.H"
#include "NRF24L01.H"
#include "RF24.h"
#include "spi.h"
#include "USART.h"
#include <avr/eeprom.h>
#include "xitoa.h"

#pragma pack(push, 1)

t_buff buff;

typedef struct
{	uint8_t status;
	uint8_t config;
} t_st;

t_st st;

#ifdef RF24_PIN_CSN_1
typedef struct {
	uint8_t * port;
	uint8_t c[2]; //[0]csn, [1]ce
} t_rf24;

volatile t_rf24 rf24_arr[] = {{&RF24_PIN_PORT, {RF24_PIN_CSN_0, RF24_PIN_CE_0}}, {&RF24_PIN_PORT, {RF24_PIN_CSN_1, RF24_PIN_CE_1}}};
uint8_t rf24_no;

void rf24_init(void){
	RF24_PIN_PORT |= (1<<RF24_PIN_CSN_0)|(1<<RF24_PIN_CSN_1);
	RF24_PIN_PORT &= ~((1<<RF24_PIN_CE_0)|(1<<RF24_PIN_CE_1));
	RF24_PIN_DDR  |= (1<<RF24_PIN_CSN_0)|(1<<RF24_PIN_CE_0)|(1<<RF24_PIN_CSN_1)|(1<<RF24_PIN_CE_1);
	rf24_no = 0;
}

void rf24_set_pin(uint8_t c, uint8_t v){
	uint8_t data = *(rf24_arr[rf24_no].port);
	uint8_t bit = (1 << rf24_arr[rf24_no].c[c]);
	*(rf24_arr[rf24_no].port) = v ? data | bit : data  & ~bit;
}

#else

void rf24_init(void){
	rf24_CSN_hi; rf24_CE_lo;
	RF24_PIN_DDR  |= (1<<RF24_PIN_CSN_0)|(1<<RF24_PIN_CE_0);
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

void rf24_wr_bytes(uint8_t byte1, char * bytes, uint8_t len) {
	rf24_CSN_lo;
	spi_wr_rd_byte(byte1);
	spi_wr_bytes(bytes, len);
	rf24_CSN_hi;
}



//////////////////////////////////////

void rf_switch_bank(unsigned char bankindex)
{
	if ((SPI_RW(iRF_BANK0_STATUS) & 0x80) != bankindex)
		SPI_RW_Reg(iRF_CMD_ACTIVATE,0x53);
}

//#define MEM_PROGMEM
#define MEM_EEMEM
#if defined(MEM_PROGMEM)
	#define MEM_READ_BYTE(b) pgm_read_byte(b)
	#define MEM_TYPE PROGMEM
#elif defined(MEM_EEMEM)
	#define MEM_READ_BYTE(b) eeprom_read_byte(b)
	#define MEM_TYPE EEMEM
#else
	#define MEM_READ_BYTE(b) b[0]
	#define MEM_TYPE
#endif


MEM_TYPE const uint8_t nf_calibration[] = {
	_RegS(REG_PLL_CTL0,    4), 0x40, 0x00, 0x10, (SPEED_RF==SPEED_2Mbps) ? 0xE6 : 0xE4,
	_RegS(REG_CAL_CTL,     5), 0x20, 0x08, 0x50, 0x40, 0x50,
	_RegS(REG_IF_FREQ,     3), 0x00, 0x00, (SPEED_RF==SPEED_2Mbps) ? 0x1E : 0x1F,
	_RegS(REG_FDEV,        1), (SPEED_RF==SPEED_2Mbps) ? 0x29 : 0x14,
	_RegS(REG_DAC_CAL_LOW, 1), 0x00,
	_RegS(REG_DAC_CAL_HI,  1), 0x7F,
	_RegS(REG_AGC_GAIN,    4), 0x02, 0xC1, 0xEB, 0x1C,
	_RegS(REG_RF_IVGEN,    4), 0x97, 0x64, 0x00, 0x81,
	_RegS(REG_PLL_CTL0,    4), 0x40, 0x01, 0x30, 0xE2,
	_RegS(REG_CAL_CTL,     5), 0x29, 0x89, 0x55, 0x40, 0x50,
	_RegS(REG_FDEV,        1), (SPEED_RF==SPEED_2Mbps) ? 0x29: 0x14,
	_RegS(REG_RX_CTRL,     4), 0x55, 0xC2, 0x09, 0xAC,
	_RegS(REG_FAGC_CTRL_1, 4), 0x00, 0x14, 0x08, 0x29,
	_RegS(REG_AGC_GAIN,    4), 0x02, 0xC1, 0xCB, 0x1C,
	_RegS(REG_RF_IVGEN,    4), 0x97, 0x64, 0x00, 0x01,
	_RegS(REG_TEST_PKDET,  4), 0x2A, 0x04, 0x00, 0x7D,
0x00};


MEM_TYPE const uint8_t nf_addres[] = {
	0xF0, 0xF1,	0xF2, 0xF3,	0xF4, 0xF5, // первый байт пяти адресов
	0xC2, 0xC3, 0xC5, 0xC2				// 2-5 байт
};

MEM_TYPE const uint8_t nf_setup[] = {
	iRF_CMD_ACTIVATE,							0x73,
	iRF_CMD_WRITE_REG|NRF24L01_REG_RX_PW_P0,	BUFFER_LEN,
	iRF_CMD_WRITE_REG|NRF24L01_REG_DYNPD,		0b111111, //Enable dynamic payloads 0..5
	iRF_CMD_WRITE_REG|NRF24L01_REG_FEATURE,		NRF24L01_REG_EN_DPL,
	iRF_CMD_WRITE_REG|NRF24L01_REG_EN_AA,		0,
	iRF_CMD_WRITE_REG|NRF24L01_REG_EN_RXADDR,	0b000001, // pipe 0..5 ENABLE
	iRF_CMD_WRITE_REG|NRF24L01_REG_SETUP_AW,	(ADR_WIDTH==3)?0b01:(ADR_WIDTH==4)?0b10:(ADR_WIDTH==5)?0b11:0,
	iRF_CMD_WRITE_REG|NRF24L01_REG_RF_CH,		40,
	iRF_CMD_WRITE_REG|NRF24L01_REG_RF_SETUP,	SPEED_RF, // SPEED_1Mbps, SPEED_500Kbps
	iRF_CMD_WRITE_REG|NRF24L01_REG_CONFIG,		NRF24L01_REG_PWR_UP | NRF24L01_REG_PRIM_RX | NRF24L01_CRCO_0,
0x00};

void SetupArr(uint8_t *  Pos, const uint8_t cmd) {
	uint8_t Len=0, Reg;
	while (1) {

		Reg = MEM_READ_BYTE(Pos++);

		if (! Len--) { //Позиция регистра и размера данных

			rf24_CSN_hi;

			if (Reg==0) break;

			Len = 1;
			if (!cmd) { 
				Len = Reg >> 5;
				Reg = iRF_CMD_WRITE_REG|(Reg & 0b11111);
			}

			rf24_CSN_lo;

		} 
		
		spi_wr_rd_byte(Reg);
	}
}

void SetupAdr (uint8_t Reg, uint8_t Addr) {
	Reg |= iRF_CMD_WRITE_REG;
	rf24_CSN_lo;
	while (1) {
		spi_wr_rd_byte(Reg);
		if (Addr > (4 + ADR_WIDTH)) break;
		Reg = MEM_READ_BYTE(nf_addres + Addr++);
		if (Addr < 6) Addr = 6;
	}
	rf24_CSN_hi;
}

void nrf_init (void) {
	rf24_init();
	rf_switch_bank(iBANK1);
	SetupArr(nf_calibration, 0);
	rf_switch_bank(iBANK0);
	SetupArr(nf_setup, 1);

    SetupAdr(NRF24L01_REG_TX_ADDR, 0);
    SetupAdr(NRF24L01_REG_RX_ADDR_P0, 0);
/*    SetupAdr(NRF24L01_REG_RX_ADDR_P1, 1);
    SetupAdr(NRF24L01_REG_RX_ADDR_P2, 2);
    SetupAdr(NRF24L01_REG_RX_ADDR_P3, 3);
    SetupAdr(NRF24L01_REG_RX_ADDR_P4, 4);
    SetupAdr(NRF24L01_REG_RX_ADDR_P5, 5);
*/
	_delay_ms(5);
}


void test_io(void) {
	rf24_init();
	while(1) {
//		DDRD  = 0;
//	  	if (rf24_no) rf24_no = 0; else rf24_no = 1
//		rf24_no = 0;
		_delay_us(400);
		rf24_CSN_hi;
		rf24_CE_hi;
		_delay_us(400);
		rf24_CE_lo;
		_delay_us(400);
		rf24_CSN_lo;
		_delay_us(400);
		rf24_CE_hi;
		_delay_us(8);
		rf24_CSN_hi;
		_delay_us(800);
		NRF24L01_RF_CH_W(71);
		_delay_us(1400);
		NRF24L01_RF_CH_W(71);
	}
};


void test_register(void) {
	rf24_init();
	while(1) {
	  for (int i=1; i<32; i++) {
		NRF24L01_RF_CH_W(i);
		NRF24L01_RX_PW_P0_W(5);
		if ((i!=NRF24L01_RF_CH)||(5!=(NRF24L01_RX_PW_P0))) {
			LED_RED_ON;
			_delay_ms(50);	
		} else {
			LED_RED_OFF;
		}
	  }
	}

};


void test_status(void) {

#ifdef rf24_no
	xprintf(PSTR("\nmodule=%d"), rf24_no);
#endif
	xprintf(PSTR("\nCH=%d"), NRF24L01_RF_CH);
	xprintf(PSTR("\nPAYLOAD=%d"), NRF24L01_RX_PW_P0);
	xprintf(PSTR("\nPOWER=%s"), NRF24L01_PWR_UP ? "UP" : "DN" );
	xprintf(PSTR("\nMODE=%s"), NRF24L01_PRIM_RX ? "RX" : "TX" );
	xprintf(PSTR("\nDPL=%s"), NRF24L01_EN_DPL ? "ENABLE" : "DISABLE" );
	xprintf(PSTR("\nCRC=%s"),  NRF24L01_EN_CRC ? (NRF24L01_CRCO ? "2B" : "1B") : "DISABLE" );
	xprintf(PSTR("\nDR=%sbps"), (NRF24L01_RF_SETUP&NRF24L01_REG_RF_DR_LOW)
		? ((NRF24L01_RF_SETUP&NRF24L01_REG_RF_DR_HIGH) ? "500К" : "-")
		: ((NRF24L01_RF_SETUP&NRF24L01_REG_RF_DR_HIGH) ? "2M" : "1M"));
	xprintf(PSTR("\nAA=%05b"), NRF24L01_EN_AA);
}

void test_payload(void) {
	while(1) {
		rf24_CE_lo;
	    NRF24L01_MODE_SET_TX;
		rf24_wr_bytes(NRF24L01_SPI_W_TX_PAYLOAD, "12345", 5);
		_delay_ms(1);
		if (NRF24L01_TX_EMPTY) {
			LED_RED_ON;
			_delay_ms(500);
			NRF24L01_FLUSH_TX;
		}
		LED_RED_OFF;

	}
};

void scan08_graf(void){
	uint8_t lv;
	const uint8_t nch = 255;
	NRF24L01_MODE_SET_RX;
	rf24_CE_lo;
	uint8_t base_lavel = 0xff;
	while(1)
	{

		_delay_ms(200);
		usart_send_str("\n");
		for(uint8_t i=0 ; i<nch ; i++)
		{
			NRF24L01_RF_CH_W(i);
			NRF24L01_FLUSH_RX;
			NRF24L01_STATUS_RESET;
			_delay_us(200);
			rf24_CE_hi;
			_delay_us(700);

			rf24_CE_lo;
			lv = NRF24L01_CD;

			if (lv > base_lavel)
				lv -= base_lavel - 32;
			else {
				base_lavel = lv - 3;
			    lv = 32;
			}

			usart_send_char(lv);

/*
			if (NRF24L01_RX_P_NO < 7) {
				buff.len = 9;
				buff.status = RD_RX_PLOAD;
				r24_wrd_bytes(&buff, buff.len+1);

				buff.data[buff.len] = 0;
				//usart_send_str("\nRX=");
				//usart_send_str(buff.data);
				xprintf(PSTR("\n-RX:CH=%d;B=%s\n"), i,  buff.data);
				_delay_ms(100);
			}
*/

		}

	}

}



void rx08(void) {
	rf24_CE_lo;
	NRF24L01_MODE_SET_RX;
	rf24_CE_hi;
	while (1) {
		LED_RED_OFF;
//		NRF24L01_RF_CH_W(38);
		if (NRF24L01_RX_P_NO < 7) {

			buff.len = BUFFER_LEN;
			if (NRF24L01_EN_DPL) {
				buff.len = NRF24L01_R_RX_PL_WID;
				if (buff.len == BUFFER_LEN) LED_GREEN_ON; else LED_GREEN_OFF;
				if (buff.len > BUFFER_LEN) {buff.len = BUFFER_LEN;}
			}

			buff.status = RD_RX_PLOAD;
			r24_wrd_bytes(&buff, buff.len+1);

			usart_send_str("\nRX=");			
			usart_send_buff(buff.data, buff.len);

			LED_RED_ON;
			_delay_ms(10);
//			NRF24L01_FLUSH_RX;
			NRF24L01_STATUS_RESET;
		}
		//		test_status();
		_delay_ms(100);
}



void test123(void) {
	sei();

	uint8_t rf24_rx_mode;

	while(1) {

//	  	if (rf24_no) rf24_no = 0; else rf24_no = 1;
		  
		st.status = NRF24L01_SPI_R_REGISTER(NRF24L01_REG_CONFIG);
		r24_wrd_bytes(&st, 2);
		rf24_rx_mode = st.config & NRF24L01_REG_PRIM_RX;
		
		if (rf24_rx_mode && (buff.tp == 0)) {
			buff.tp = 2;

			st.config = st.status;
			st.status = NRF24L01_SPI_R_REGISTER(NRF24L01_REG_STATUS);
			r24_wrd_bytes(&st, 2);

 			if (((st.status & NRF24L01_REG_RX_P_NO) != NRF24L01_REG_RX_P_NO)) {
		 		buff.len = BUFFER_LEN;
 				if (NRF24L01_EN_DPL) {
						buff.len = NRF24L01_R_RX_PL_WID;
						if (buff.len > BUFFER_LEN) {buff.len = BUFFER_LEN;}
				}
				buff.status = RD_RX_PLOAD;
				r24_wrd_bytes(&buff, buff.len+1);
				buff.data[buff.len] = 0;
//				xprintf(PSTR(" RPL%d=%s"), rf24_no, buff.data);
				LED_RED_OFF;

			}
			buff.tp = 0;
		}	

		if (!NRF24L01_TX_EMPTY) {
			if (rf24_rx_mode) { 
				rf24_CE_lo;
				NRF24L01_MODE_SET_TX;
				rf24_rx_mode = 0;
			}
			rf24_CE_hi;
			_delay_us(300);
//			xprintf(PSTR(" TX%d"), rf24_no);
		}
			
		if (!rf24_rx_mode) {
			rf24_CE_lo;
			NRF24L01_MODE_SET_RX;
			rf24_CE_hi;
//			xprintf(PSTR(" RX%d"), rf24_no);
		};

		if ((buff.eof) && (buff.tp==1)) {
//			rf24_no = 1;
			buff.tp=2;
			buff.data[BUFFER_LEN] = 0;
//			xprintf(PSTR("\nTPL%d=%s"), rf24_no, buff.data);
			rf24_CE_lo;
			NRF24L01_MODE_SET_TX;
			buff.status = NRF24L01_SPI_W_TX_PAYLOAD;
			r24_wrd_bytes(&buff, BUFFER_LEN+1);
			rf24_CE_hi;
			_delay_us(800);
			buff.tp = 0;
			LED_GREEN_OFF;
			LED_RED_ON;
		}
	}
}

}