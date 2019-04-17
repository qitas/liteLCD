/*
* Name :  spi.h 
* Author: qitas
*/
#include <unistd.h>
#include <stdlib.h>
#include "gpio.h"

#define SPI_CPOL   		0
#define SPI_CPHA   		0

#define RES_Pin   		7 	//GPIO6
#define MOSI_Pin  		11	//GPIO5
#define MISO_Pin  		17	//GPIO4
#define CLK_Pin   		14	//GPIO3
#define CS_Pin    		16	//GPIO2
#define DC_Pin    		12	



void delayus(uint32_t us);

void SPI_soft_init(void);
void SPI_send_byte(char data);
char SPI_read_byte();

void lcd_DC(char data) ;
void LCD_Reset(char data) ;
void SPI_SET_Clk(char data) ;
void SPI_SET_CS(char data) ;
void SPI_SET_MOSI(char data) ;
void bit_delay();
void tst();