#include <avr/delay.h>
#include "pin.h"
#include "RF24.H"
#include "se8r01.h"

void SE8R01_powerup(void)
{
	SE8R01_switch_bank(iBANK0);
	SPI_RW_Reg(iRF_CMD_WRITE_REG|iRF_BANK0_CONFIG,0x03);
	SPI_RW_Reg(iRF_CMD_WRITE_REG|iRF_BANK0_RF_CH,0x32);
	SPI_RW_Reg(iRF_CMD_WRITE_REG|iRF_BANK0_RF_SETUP,0x48);
	SPI_RW_Reg(iRF_CMD_WRITE_REG|iRF_BANK0_PRE_GURD,0x77); //2450 calibration
}


void SE8R01_switch_bank(unsigned char bankindex)
{
	if ((SPI_RW(iRF_BANK0_STATUS) & 0x80) != bankindex)
        SPI_RW_Reg(iRF_CMD_ACTIVATE,0x53);
}

#define writeToReg1(r, b)	rf24_wr2_rd(iRF_CMD_WRITE_REG|r,b)
#define writeToReg(r, b, l) rf24_wr_bytes(iRF_CMD_WRITE_REG|r,b,l)

void bank1Init(void) {


	SE8R01_switch_bank(iBANK1);

	//repeat, but necessary

	
	#ifdef SE8R01_DR_2M
	uint8_t p1[] = { 0x40, 0x00, 0x10, 0x6 };
	#else
	uint8_t p1[] = { 0x40, 0x00, 0x10, 0xE4 };
	#endif

	writeToReg(REG_PLL_CTL0, p1, sizeof(p1));

	uint8_t p2[] = { 0x20, 0x08, 0x50, 0x40, 0x50 };

	writeToReg(REG_CAL_CTL, p2, sizeof(p2));


	#ifdef SE8R01_DR_2M
	uint8_t p3[] = { 0x00, 0x00, 0x1E };
	#else
	uint8_t p3[] = { 0x00, 0x00, 0x1F };
	#endif

	writeToReg(REG_IF_FREQ, p3, sizeof(p3));

	//repeat, but necessary

	#ifdef SE8R01_DR_2M
	writeToReg1(REG_FDEV, 0x29);
	#else
	writeToReg1(REG_FDEV, 0x14);
	#endif

	writeToReg1(REG_DAC_CAL_LOW, 0x00);

	writeToReg1(REG_DAC_CAL_HI, 0x7F);


	//repeat, but necessary

	uint8_t p4[] = { 0x02, 0xC1, 0xEB, 0x1C };

	writeToReg(REG_AGC_GAIN, p4, sizeof(p4));



	//repeat, but necessary

	uint8_t p5[] = { 0x97, 0x64, 0x00, 0x81 };

	writeToReg(REG_RF_IVGEN, p5, sizeof(p5));

	SE8R01_switch_bank(iBANK0);

	rf24_CE_hi;
	_delay_us(30);
	rf24_CE_lo;
	_delay_us(50);
	rf24_CE_hi;
	_delay_us(30);
	rf24_CE_lo;
	_delay_us(50);

	uint8_t p55[] = {0x28, 0x32,0x80, 0x90, 0x00};
	writeToReg(iRF_BANK0_SETUP_VALUE, p55, sizeof(p55));

	_delay_us(30);

	SE8R01_switch_bank(iBANK1);

	uint8_t p6[] = { 0x40, 0x01, 0x30, 0xE2 };

	writeToReg(REG_PLL_CTL0, p6, sizeof(p6));

	uint8_t p7[] = { 0x29, 0x89, 0x55, 0x40, 0x50 };

	writeToReg(REG_CAL_CTL, p7, sizeof(p7));



	#ifdef SE8R01_DR_2M
	writeToReg1(REG_FDEV, 0x29);
	#else
	writeToReg1(REG_FDEV, 0x14);
	#endif


	uint8_t p8[] = { 0x55, 0xC2, 0x09, 0xAC };

	writeToReg(REG_RX_CTRL, p8, sizeof(p8));



	uint8_t p9[] = { 0x00, 0x14, 0x08, 0x29 };

	writeToReg(REG_FAGC_CTRL_1, p9, sizeof(p9));



	uint8_t p10[] = { 0x02, 0xC1, 0xCB, 0x1C };

	writeToReg(REG_AGC_GAIN, p10, sizeof(p10));



	uint8_t p11[] = { 0x97, 0x64, 0x00, 0x01 };

	writeToReg(REG_RF_IVGEN, p11, sizeof(p11));



	uint8_t p12[] = { 0x2A, 0x04, 0x00, 0x7D };

	writeToReg(REG_TEST_PKDET, p12, sizeof(p12));



	SE8R01_switch_bank(iBANK0);

}