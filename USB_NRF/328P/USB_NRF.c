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
#include "RF24.H"
#include "NRF24L01.H"
#include "se8r01.h"


#pragma pack(push, 1)
typedef struct
{	uint8_t status;
	uint8_t config;
} t_st;

typedef struct
{	uint8_t status;
	uint8_t data[BUFFER_LEN];
	uint8_t len;
	uint8_t tp; //0 - free; 1 - uart; 2 - nrf
	uint8_t eof;
} t_buff;


static t_buff buff;

//static uint8_t rf24_addr0[ADR_WIDTH] = DADDRESS0;
//static uint8_t rf24_addr1[ADR_WIDTH] = {0xF1, 0xC2, 0xC3, 0xC5, 0xC2};
//static uint8_t rf24_addr1[ADR_WIDTH] = {0xF2, 0xC2, 0xC3, 0xC5, 0xC2};
//static uint8_t rf24_addr1[ADR_WIDTH] = {0xF3, 0xC2, 0xC3, 0xC5, 0xC2};
//static uint8_t rf24_addr1[ADR_WIDTH] = {0xF4, 0xC2, 0xC3, 0xC5, 0xC2};

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
	
t_st st;
//uint8_t rx_buff_len;


		
void nrf_init ()
{
	_delay_ms(50);
	rf24_init();
	rf24_CE_lo;
	SE8R01_powerup();
	bank1Init();
	NRF24L01_ACTIVATE;
	NRF24L01_DYNPD_W(0b111111) ;//Enable dynamic payloads 0..5
	NRF24L01_EN_DPL_1; //RF24_EN_DPL_1;
	NRF24L01_EN_AA_W(0);
	NRF24L01_EN_RXADDR_W(0b000001); //pipe 0..5 ENABLE
	NRF24L01_SETUP_AW_W(ADR_WIDTH); //Setup of Address Widths
	NRF24L01_TX_ADDR(ADDRESS0, ADR_WIDTH);
	NRF24L01_RX_ADDR_P0(ADDRESS0, ADR_WIDTH);
/*
	NRF24L01_RX_ADDR_P1(ADDRESS1, 1);
	NRF24L01_RX_ADDR_P2(ADDRESS2, 1);
	NRF24L01_RX_ADDR_P3(ADDRESS3, 1);
	NRF24L01_RX_ADDR_P4(ADDRESS4, 1);
	NRF24L01_RX_ADDR_P5(ADDRESS5, 1);
*/
	NRF24L01_CRC_DISABLE;
//	NRF24L01_RF_DR_2MBPS;
	NRF24L01_MODE_SET_RX;

//	NRF24L01_RF_CH_W(70);
nrf24l01_wr_reg_byte(iRF_BANK0_RF_CH, 40);
nrf24l01_wr_reg_byte(iRF_BANK0_RF_SETUP, 0x4f);        //2mps 0x4f

//	NRF24L01_PWR_UP_UP;
	_delay_ms(500);
}



void test_io(void) {
	rf24_init();
	while(1) {
//		DDRD  = 0;
//	  	if (rf24_no) rf24_no = 0; else rf24_no = 1
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
	NRF24L01_PWR_UP_UP;
	while(1) {
  	  _delay_ms(20);
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
//	xprintf(PSTR("\nmodule=%d"), rf24_no);
	
	xprintf(PSTR("\nCH=%d"), NRF24L01_RF_CH);
	for(uint8_t i =0; i < 20; i++) xprintf(PSTR("-"));
	xprintf(PSTR("\nPAYLOAD=%d"), NRF24L01_RX_PW_P0);
	xprintf(PSTR("\nPOWER=%s"), NRF24L01_PWR_UP ? "UP" : "DN" );
	xprintf(PSTR("\nMODE=%s"), NRF24L01_PRIM_RX ? "RX" : "TX" );
	xprintf(PSTR("\nDPL=%s"), NRF24L01_EN_DPL ? "ENABLE" : "DISABLE" );
	xprintf(PSTR("\nCRC=%s"), NRF24L01_EN_CRC ? NRF24L01_CRCO ? "2B" : "1B" : "DISABLE" );

	xprintf(PSTR("\nDR=%08b"), NRF24L01_RF_SETUP);
	xprintf(PSTR("\nDR=%sbps"), (NRF24L01_RF_SETUP&NRF24L01_REG_RF_DR_LOW)
		? ((NRF24L01_RF_SETUP&NRF24L01_REG_RF_DR_HIGH) ? "500Ê" : "-")
		: ((NRF24L01_RF_SETUP&NRF24L01_REG_RF_DR_HIGH) ? "2M" : "1M"));
	xprintf(PSTR("\nAA=%05b"), NRF24L01_EN_AA);
	xprintf(PSTR("\nTX_BUFF-%s"), NRF24L01_TX_FULL ? "F" : (NRF24L01_TX_EMPTY ? "E" : "N")); //NRF24L01_FIFO_FULL
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
#define CHANNELS 4
uint8_t channel[CHANNELS];

void scanChannels(void)
{
	rf24_CE_lo;
	for(uint8_t j=0 ; j<20; j++)
	{
		for(uint8_t i=0 ; i < CHANNELS ; i++)
		{
				NRF24L01_RF_CH_W(i<<1);
				NRF24L01_MODE_SET_RX;
				rf24_CE_hi;
				_delay_us(800);
				rf24_CE_lo;
//				if (NRF24L01_CD > 0xB0)   channel[i]++;
				if (NRF24L01_CD > 0xB0)   channel[i] += (NRF24L01_CD - 0xB0);

		}
	}
}



void displChannels(void)
{
	LED_GREEN_OFF;
	LED_RED_OFF;
	usart_send_char('\r');
	usart_send_char('\n');
		for( int i=0 ; i < CHANNELS ; i++)
		{
			usart_send_char('0'+channel[i]);
			if (channel[i] > 1) LED_GREEN_ON;
			if (channel[i] > 100) LED_RED_ON;
			channel[i] = 0;
		}

}
void scan(void){
	NRF24L01_ACTIVATE;
	NRF24L01_MODE_PWR_UP;
	_delay_us(1030);
	NRF24L01_EN_AA_W(0);
	nrf24l01_wr2_rd(NRF24L01_SPI_W_REGISTER(NRF24L01_REG_RF_SETUP), 0x0f);
	displChannels();
	LED_GREEN_OFF;
	LED_RED_OFF;
	while(1){
		scanChannels();
		displChannels();	
	}		
}

void scan08_term(void){
	int bl = 0;
	const uint8_t nch = 180;
	NRF24L01_MODE_SET_RX;
	rf24_CE_lo;
	uint8_t base_lavel = 0xA9;
	uint8_t bm;
	while(1)
	{
		_delay_ms(600);
		usart_send_str("\r\n");
		bm = 0;
		bl = 0;
		
		for(uint8_t i=0 ; i<nch ; i++)
		{
			NRF24L01_RF_CH_W(i);
			rf24_CE_hi;
			_delay_us(400);
			rf24_CE_lo;

			bl += NRF24L01_CD;
			if (bm < NRF24L01_CD) bm = NRF24L01_CD;

			if (NRF24L01_CD > base_lavel)
				usart_send_char('0'+(NRF24L01_CD-base_lavel));
			else
				usart_send_char('.');

		}
		base_lavel = (base_lavel + ((bl + ((int) bm * 12)) / (nch+12))) >> 1;

	}

}

void scan08_graf(void){
	uint8_t lv, rf_dr= 1;
	const uint8_t nch = 255;
	NRF24L01_MODE_SET_RX;
	rf24_CE_lo;
	uint8_t base_lavel = 0xA4;
	while(1)
	{
/*
		if (rf_dr > 2) rf_dr = 0; else rf_dr++;
		if (rf_dr==0) NRF24L01_RF_DR_5MBPS;
		else if (rf_dr==1) NRF24L01_RF_DR_1MBPS;
		else  NRF24L01_RF_DR_2MBPS;
*/
		usart_send_str("\n");
		for(uint8_t i=0 ; i<nch ; i++)
		{
			NRF24L01_RF_CH_W(i);
			rf24_CE_hi;
			_delay_us(800);
			
			if (NRF24L01_RX_P_NO != 7) {
				buff.status = RD_RX_PLOAD;
				r24_wrd_bytes(&buff, 9);
				buff.data[8] = 0;
				xprintf(PSTR("\n-RX:CH=%d;B=%s\n"), i, buff.data);
				NRF24L01_STATUS_RESET;
			}
			
			rf24_CE_lo;
			lv = NRF24L01_CD;

			if (lv > base_lavel)
				lv -= base_lavel - 32;
			else
			    lv = 32;

			usart_send_char(lv);

		}

	}

}

void TestFirst(void){
	while(1) {
		_delay_ms(500);
		LED_RED_BL;
		spi_wr_bytes("123456",6);
		usart_send_str("1234567890");
	}	
}
void  test_rx123(void){
	rf24_CE_lo;
//	NRF24L01_RF_SETUP_W(NRF24L01_REG_PLL_LOCK, NRF24L01_REG_PLL_LOCK);
	NRF24L01_MODE_SET_TX;
	NRF24L01_W_TX_PAYLOAD("12345678901234567890123456789012",32);
	rf24_CE_hi;
	_delay_ms(1);
	while(1) {
		rf24_cmd_status(REUSE_TX_PL);
	}
}


void  test_rx1234(void){
	uint8_t rf_dr = 0;
	NRF24L01_MODE_SET_TX;
	_delay_ms(1);
	rf24_CE_hi;
//	if (!NRF24L01_TX_EMPTY) LED_RED_ON;
	while(1) {
//	    writeTxPayload("1234567890123456",16);
		NRF24L01_W_TX_PAYLOAD("1234567890123456",16);
		_delay_us(600);
//		if (!NRF24L01_TX_EMPTY) LED_RED_BL;

//		rf24_cmd_status(REUSE_TX_PL);
/*
			rf24_CE_lo;
			rf_dr++;
			if (rf_dr > 2) rf_dr = 0;
			if (rf_dr==0) NRF24L01_RF_DR_5MBPS;
			else if (rf_dr==1) NRF24L01_RF_DR_1MBPS;
			else  NRF24L01_RF_DR_2MBPS;
			NRF24L01_FLUSH_TX;
			rf24_CE_hi;
			
			NRF24L01_W_TX_PAYLOAD("1234567890123456",16);
			_delay_us(500);
		*/
	}
}

int main(void)
{	SPI_MASTER_INIT;
	SPI_MASTER_CLOCK(8);
	USART_INIT;
	LED_INIT;
	xfunc_out = usart_send_char;

	nrf_init();
//	SE8R01_init();
	test_rx1234();
	
	test_status();	
	scan08_graf();


//	nrf_init();


	test_register();
	
	TestFirst();

	
	//SPI_INIT;
	//USART_SET_IRQ;
    MCUCR = 0; // MCUCR = (1<<ISC00)|(1<<ISC01);// over Level
    //   GIMSK |= (1<<INT0); RX1
	
	//rf24_no = 0;
	scan08_tri();
	
	//scan08();
//--------------------------

return 0;

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


while (1) {
//	if ( rf24_no == 1)  rf24_no = 0; else rf24_no = 1;
	test_status();
}

return 0;


}
