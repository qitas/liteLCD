/*
* Name :  pwm.h 
* Author: qitas
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>     
#include <string.h>
#include <unistd.h>


#define 	PWM_REG				0x01C21400
#define 	PWM_REG_LEN			0x0C

#define 	PWM_CTL_REG			0x00
#define 	PWM_CHA0_REG		0x04
#define 	PWM_CHA1_REG		0x08

#define 	PWM_PRE_1			0x0f
#define 	PWM_PRE_120			0x00
#define 	PWM_PRE_240			0x02
#define 	PWM_PRE_480			0x04
#define 	PWM_PRE_12k			0x08
#define 	PWM_PRE_24k			0x09
#define 	PWM_PRE_72k			0x0c


#define 	PWM0_Pin    4
#define 	PWM1_Pin  	5

void pwm_init(int CHA);
void pwm_set(int CHA, uint16_t base, uint16_t data);
void pwm_rate(int CHA, uint32_t freq, uint8_t rate);
void pwm_prescalar(int CHA, uint8_t data);
void pwm_start(int CHA);
void pwm_stop(int CHA);
