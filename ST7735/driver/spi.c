
/*
* Name :  spi.c
* Author: qitas
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>    
#include <string.h>
#include "spi.h"


void delayus(uint32_t us)
{
    volatile uint32_t i,j;
    for(i=0;i<us;i++)
    {
    	j=0x0f;
       	while(j--);
    }
}

void bit_delay()
{
	volatile uint16_t i=0x10;
    while(i--);
}
//*****************************

inline void lcd_DC(char data) 
{
	PE_set(DC_Pin, data);
}
inline void LCD_Reset(char data) 
{
	PE_set(RES_Pin, data);
}

//*****************************


inline void SPI_SET_CS(char data) 
{
	PE_set(CS_Pin, data);
}

inline void SPI_SET_Clk(char data) 
{
	PE_set(CLK_Pin, data);
}

inline void SPI_SET_MOSI(char data) 
{
	PE_set(MOSI_Pin, data);
}

inline char SPI_GET_MISO() 
{
	return PE_get(MISO_Pin);
}

//*****************************

void SPI_soft_init(void) 
{
	gpio_mem_init();
	//printf("gpio open done\n");	
	
	PE_init(MOSI_Pin, GPIO_DIR_OUT);
	PE_init(CLK_Pin, GPIO_DIR_OUT);
	PE_init(CS_Pin, GPIO_DIR_OUT);
	PE_init(MISO_Pin, GPIO_DIR_IN);

}


#if SPI_CPOL==0 && SPI_CPHA==0  

void SPI_send_byte(char data)
{
	SPI_SET_CS(0);
	SPI_SET_Clk(0);

	for(char i = 0; i<8; i++)
	{
		SPI_SET_Clk(0);
		bit_delay();		
		if(data & 0x80) SPI_SET_MOSI(1);
		else SPI_SET_MOSI(0);
		bit_delay();
		SPI_SET_Clk(1);
		data <<= 1; 
		bit_delay();
	}

	SPI_SET_Clk(0);
	SPI_SET_CS(1);
	//bit_delay();
}

char SPI_read_byte()
{
	char  ret=0; 
	SPI_SET_CS(0);
	SPI_SET_Clk(0);

	for(char i = 0; i<8; i++)
	{
		SPI_SET_Clk(0);
		bit_delay();	
		if(SPI_GET_MISO()) ret+=1; 
		bit_delay();
		SPI_SET_Clk(1);
		ret <<= 1; 
		bit_delay();
	}

	SPI_SET_Clk(0);
	SPI_SET_CS(1);
	bit_delay();
	return ret;
}
#endif


#if SPI_CPOL==1 && SPI_CPHA==0  
void SPI_send_byte(char data)
{
	SPI_SET_CS(0);
	SPI_SET_Clk(1);

	for(char i = 0; i<8; i++)
	{
		SPI_SET_Clk(0);
		bit_delay();		
		if(data & 0x80) SPI_SET_MOSI(1);
		else SPI_SET_MOSI(0);
		bit_delay();
		//CLK = 1
		SPI_SET_Clk(1);
		data <<= 1; 
		bit_delay();
	}

	SPI_SET_Clk(1);
	SPI_SET_CS(1);
	bit_delay();
}

char SPI_read_byte()
{
	char  ret; 
	SPI_SET_CS(0);
	SPI_SET_Clk(0);
 	ret=0;
	for(char i = 0; i<8; i++)
	{
		SPI_SET_Clk(0);
		bit_delay();	
		if(SPI_GET_MISO()) ret+=1; 
		bit_delay();
		SPI_SET_Clk(1);
		ret <<= 1; 
		bit_delay();
	}

	SPI_SET_Clk(0);
	SPI_SET_CS(1);
	bit_delay();
	return ret;
}
#endif


#if SPI_CPOL==0 && SPI_CPHA==1  
void SPI_send_byte(char data)
{
	SPI_SET_CS(0);
	SPI_SET_Clk(0);

	for(char i = 0; i<8; i++)
	{
		SPI_SET_Clk(1);
		bit_delay();		
		if(data & 0x80) SPI_SET_MOSI(1);
		else SPI_SET_MOSI(0);
		bit_delay();
		SPI_SET_Clk(0);
		data <<= 1; 
		bit_delay();
	}

	SPI_SET_Clk(0);
	SPI_SET_CS(1);
	bit_delay();
}

char SPI_read_byte()
{
	char  ret=0; 
	SPI_SET_CS(0);
	SPI_SET_Clk(0);

	for(char i = 0; i<8; i++)
	{
		SPI_SET_Clk(0);
		bit_delay();	
		if(SPI_GET_MISO()) ret+=1; 
		bit_delay();
		SPI_SET_Clk(1);
		ret <<= 1; 
		bit_delay();
	}

	SPI_SET_Clk(0);
	SPI_SET_CS(1);
	bit_delay();
	return ret;
}
#endif


#if SPI_CPOL==1 && SPI_CPHA==1  
void SPI_send_byte(char data)
{
	SPI_SET_CS(0);
	SPI_SET_Clk(1);

	for(char i = 0; i<8; i++)
	{
		SPI_SET_Clk(1);
		bit_delay();		
		if(data & 0x80) SPI_SET_MOSI(1);
		else SPI_SET_MOSI(0);
		bit_delay();
		SPI_SET_Clk(0);
		data <<= 1; 
		bit_delay();
	}

	SPI_SET_Clk(1);
	SPI_SET_CS(1);
	bit_delay();
}

char SPI_read_byte()
{
	char  ret=0; 
	SPI_SET_CS(0);
	SPI_SET_Clk(0);

	for(char i = 0; i<8; i++)
	{
		SPI_SET_Clk(0);
		bit_delay();	
		if(SPI_GET_MISO()) ret+=1; 
		bit_delay();
		SPI_SET_Clk(1);
		ret <<= 1; 
		bit_delay();
	}

	SPI_SET_Clk(0);
	SPI_SET_CS(1);
	bit_delay();
	return ret;
}
#endif





void tst()
{
	while(1)
	{
		PE_init(CS_Pin, 1);
		delayus(2000);
		PE_init(CS_Pin, 3);
		delayus(2000);
		PE_init(CS_Pin, 0);
		delayus(2000);
		PE_init(CS_Pin, 7);
		delayus(2000);
	}
}
