/*
* Name :  spi.h 
* Author: qitas
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>    
#include <string.h>
#include <unistd.h>

#define GPIO_DIR_IN      0
#define GPIO_DIR_OUT     1
#define GPIO_PWM_OUT     2

#define PIO_REG			           	0x01C20800
#define PORT_LEN			    	(0x24)
#define IOREG_LEN			    	(6*PORT_LEN)

#define PB_REG						(1*PORT_LEN)
#define PE_REG						(4*PORT_LEN)


#define CONF0_REG				0x0
#define CONF1_REG				0x4
#define CONF2_REG				0x8
#define CONF3_REG				0xC
#define DATA_REG				0x10

void gpio_mem_init();
void gpio_close();

void PE_init(int pin, uint8_t stat);
void PE_set(int pin, uint8_t value);
int  PE_get(int pin);


void PB_init(int pin, uint8_t stat);
void PB_set(int pin, uint8_t value);
int PB_get(int pin);