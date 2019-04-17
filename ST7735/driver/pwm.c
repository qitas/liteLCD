
/*
* Name :  pwm.c
* Author: qitas
*/
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "pwm.h"
#include "gpio.h"



#define MMAP_PATH   "/dev/mem"


static int pwm_mmap_fd = 0;
void *pwm_addr=NULL;



static int mmap_init(uint32_t addr,uint32_t len)
{
	void* pwm_mmap_reg=NULL;
    uint32_t target;
    uint32_t page_size, mapped_size, offset_in_page;
    uint32_t width = 8 * sizeof(int);

    target = addr; 

    pwm_mmap_fd = open(MMAP_PATH, (O_RDWR | O_SYNC));

    page_size = getpagesize();
	mapped_size =len;
    offset_in_page = (uint32_t)target & (page_size - 1);
	
    if (offset_in_page + width > page_size) 
    {
        mapped_size *= 2;
    }
    
    pwm_mmap_reg = mmap(NULL,
            mapped_size,
            (PROT_READ | PROT_WRITE),
            MAP_SHARED,
            pwm_mmap_fd,
            target& ~(uint32_t)(page_size - 1));// 

    if (pwm_mmap_reg == MAP_FAILED)
    {
        perror("get mmap reg fail. ");
    }
    pwm_addr = (char*)pwm_mmap_reg + offset_in_page;
    return 0;
}


void pwm_init(int CHA)
{
	uint32_t tmp;
    if (mmap_init(PWM_REG,PWM_REG_LEN)) fprintf(stderr, "failed to init pwm. ");
    tmp = *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG); 

    if((tmp>>28 & 0x01)==1) printf("PWM0 period regiter is busy!.\n ");
    if((tmp>>29 & 0x01)==1) printf("PWM1 period regiter is busy!.\n ");
    
    if (CHA == 0) 
    {
    	PB_init(PWM0_Pin, GPIO_PWM_OUT);
    	pwm_stop(0);
		pwm_set(0, 100,40);
		pwm_prescalar(0, PWM_PRE_12k);
		tmp = *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG);
		tmp |=  (1u << 5);
		tmp |=  (1u << 6);
        *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG) = tmp;
    } 
    else if(CHA == 1) 
    {
    	PB_init(PWM1_Pin, GPIO_PWM_OUT);   
		pwm_stop(1);
		pwm_set(1,1000,600);
		pwm_prescalar(1, PWM_PRE_120);
		tmp = *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG);
		tmp |=  (1u << 20);
		tmp |=  (1u << 21);
        *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG) = tmp;
    } 
}


void pwm_prescalar(int CHA, uint8_t data)
{
	uint32_t tmp;
	tmp = *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG);  
    if (CHA == 0) 
    {
    	tmp &= ~(0x0f << 0);     
        tmp |=  (data & 0x0F);
         *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG) = tmp;
    } 
    else if(CHA == 1) 
    {   
    	tmp &= ~(0x0f << 15);
        tmp |=  ((data & 0x0F) << 15);
         *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG) = tmp;
    } 	
	//printf("%x\n", tmp);
}

void pwm_set(int CHA,uint16_t base, uint16_t data)
{
	uint32_t tmp;	
	if (data>base) data=base-1;	
    if (CHA == 0) 
    {    
    	tmp = (base << 16); 
    	tmp += data; 
        *(volatile uint32_t *)(pwm_addr + PWM_CHA0_REG) = tmp;
    } 
    else if(CHA == 1) 
    {   
    	tmp = (base << 16); 
    	tmp += data; 
        *(volatile uint32_t *)(pwm_addr + PWM_CHA1_REG) = tmp;
    } 
    //printf("%x\n", tmp);
}

void pwm_rate(int CHA, uint32_t freq, uint8_t rate)
{
	uint16_t i,j,k;	
	//pwm_stop(1);

	if(freq>=10000)
	{ 
		pwm_prescalar(1, PWM_PRE_1);
		i=freq/10000;
		j=(240/i);
		k= (uint16_t )((24*rate)/(10*i));
		pwm_set(CHA,j,k);
	}
	else if(freq>=1000)
	{
		pwm_prescalar(1, PWM_PRE_120);
		i=freq/1000;
		j=(200/i);
		k= (2*rate)/i;
		pwm_set(CHA,j,k);
	}
	else if(freq>=100)
	{
		pwm_prescalar(1, PWM_PRE_120);
		i=freq/100;
		j=(2000/i);
		k= (20*rate)/i;
		pwm_set(CHA,j,k);
	}
	else if(freq>=10)
	{
		pwm_prescalar(1, PWM_PRE_480);
		i=freq/10;
		j=(5000/i);
		k= (50*rate)/i;
		pwm_set(CHA,j,k);
	}
	//pwm_start(1);

}


void pwm_start(int CHA)
{
	uint32_t tmp;
	tmp = *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG); 
    if (CHA == 0) 
    {    
        tmp |=  (1u << 4);
        *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG) = tmp;
    } 
    else if(CHA == 1) 
    { 
        tmp |=  (1u << 19);
        *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG) = tmp;
    } 	
    //printf("%x\n", tmp);
}

void pwm_stop(int CHA)
{
	uint32_t tmp;
	tmp = *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG); 
    if (CHA == 0) 
    {    
        tmp &= ~(1u << 4);
        *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG) = tmp;
    } 
    else if(CHA == 1) 
    {    
        tmp &= ~(1u << 19);
        *(volatile uint32_t *)(pwm_addr + PWM_CTL_REG) = tmp;
    } 
    //printf("%x\n", tmp);
}
