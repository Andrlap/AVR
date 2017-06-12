#include "USART.h"

#define CBUF_MAX 4
#ifndef BUFF_MAX
#define BUFF_MAX  CBUF_MAX
#endif
/*
char brx[CBUF_MAX];
unsigned char Ind_brx = 0;
unsigned char i_lline = 0;
unsigned char i_nline = 0;

usart_brx_UDR(void)
{
	// Чтение даных из UDR в циклическую строку brx
    while ( !(USART_RX_READY) ); 
    unsigned char s = USART_UDR;
	unsigned char i = Ind_brx;
	i++;
	i %= CBUF_MAX;
	if ((s==13)) {s=0; i_lline=i_nline; i_nline=i;}
	brx[Ind_brx] = s;
    if (USART_ERROR) 
	  {brx[i++]='E';	i %= CBUF_MAX;}
	Ind_brx = i;
	brx[Ind_brx] = 0;
}

void usart_get_pos(char * buf, unsigned char ri)
{
	//Копирование строки в buf из цикла brx с позиции ri
	unsigned char i = 0;
	while (brx[ri])
	{
		buf[i++] = brx[ri++];
		if (i>=BUFF_MAX) {i--; break;}
		ri%=CBUF_MAX;
		//	  if (ri>=CBUF_MAX) {ri = 0;}
	}
	buf[i] = 0;
}

void usart_read_Last(char * buf)
{
	//Извлечение строки в buf из цикла brx
	unsigned char i = 0;
	while (brx[i_lline])
	{
		buf[i++] = brx[i_lline++];
		if (i>=BUFF_MAX) {i--; break;}
		i_lline%=CBUF_MAX;
	}
	buf[i] = 0;
}

void usart_get_New(char * buf)
{
	usart_get_pos(buf, i_nline);
}

void usart_get_Last(char * buf)
{
	usart_get_pos(buf, i_lline);
}

*/

void usart_send_buff(unsigned char * buff, unsigned char l)
{
	for(unsigned char i=0; i<l; i++) {
	  while ( !(USART_TX_READY) );
		USART_UDR = buff[i];}
}


void usart_send_str(char * str)
{
	for(;*str;str++) {
		while ( !(USART_TX_READY) );
		USART_UDR = str[0];}
}

void usart_send_char(char chr)
{
    while ( !(USART_TX_READY) );
    USART_UDR = chr;
}