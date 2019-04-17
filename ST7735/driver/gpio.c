
/*
* Name :  gpio.c
* Author: qitas
*/

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "gpio.h"

#define MMAP_PATH   "/dev/mem"

static void *mmap_reg=NULL;
static void *data_addr=NULL;
static int mmap_fd = 0;

int mem_init(uint32_t addr,uint32_t len)
{

    uint32_t target;
    uint32_t page_size, mapped_size, offset_in_page;
    uint32_t width = 8 * sizeof(int);

    target = addr; 

    mmap_fd = open(MMAP_PATH, (O_RDWR | O_SYNC));

    mapped_size = page_size = getpagesize();

    offset_in_page = (uint32_t)target & (page_size - 1);

    if (offset_in_page + width > page_size) 
    {
        mapped_size *= 2;
    }
    mapped_size =len;
    mmap_reg = mmap(NULL,
            mapped_size,
            (PROT_READ | PROT_WRITE),
            MAP_SHARED,
            mmap_fd,
            target& ~(uint32_t)(page_size - 1));// 

    if (mmap_reg == MAP_FAILED)
    {
        perror("get mmap reg fail. ");
    }
    data_addr = (char*)mmap_reg + offset_in_page;
    return 0;
}

void gpio_mem_init() 
{
    //uint32_t tmp;
    //printf("%x, %x\n", PIO_REG,IOREG_LEN);
    if (mem_init(PIO_REG,IOREG_LEN)) fprintf(stderr, "failed to init gpio. ");
    //tmp = *(volatile uint32_t*)data_addr;
    //printf("%x\n", tmp);
}


void PB_init(int pin, uint8_t stat)
{
    uint32_t tmp;
    uint32_t addr;
    int pin_tmp;

    if (pin < 8) 
    {
        addr=PB_REG + CONF0_REG;
        //addr= CONF0_REG;
        pin_tmp = (4*pin);
       
        tmp = *(volatile uint32_t *)(data_addr+addr);
        printf("%x\n", tmp);
        tmp &= ~(0x0f << pin_tmp);
        tmp |=  ((stat&0x0f) << pin_tmp);
        *(volatile uint32_t *)(data_addr+addr) = tmp;
        //printf("%x\n", tmp);
    } 
    else if (pin <= 15) 
    {
        addr=PB_REG + CONF1_REG;
        //addr= CONF1_REG;
        pin_tmp = (4*(pin-8));
        tmp = *(volatile uint32_t *)(data_addr + addr);
        tmp &= ~(0x0f << pin_tmp);
        tmp |=  ((stat&0x0f) << pin_tmp);
        *(volatile uint32_t *)(data_addr + addr) = tmp;
        //printf("%x\n", tmp);
    }      
}

int PB_get(int pin)
{
    uint32_t tmp = 0;
    uint32_t addr = PB_REG+ DATA_REG;
    tmp = *(volatile uint32_t*)(data_addr + addr);
    tmp = (tmp >> pin) & 1u;
    return tmp;
}

void PB_set(int pin, uint8_t value)
{
    uint32_t tmp;
    uint32_t addr = PB_REG+ DATA_REG;
    //addr = DATA_REG;
    if (pin <= 24) 
    {    
        tmp = *(volatile uint32_t*)(data_addr + addr);
        if (value)
        {
            tmp |= (1u << pin);
            *(volatile uint32_t*)(data_addr + addr) = tmp;
        }
        else
        {
            tmp &= ~(1u << pin);
            *(volatile uint32_t*)(data_addr + addr) = tmp;
        }
        //printf("%x\n", tmp);
    }
    
}



void PE_init(int pin, uint8_t stat)
{
    uint32_t tmp;
    uint32_t addr;
    int pin_tmp;
    
    if (pin <= 7) 
    {
        addr=PE_REG + CONF0_REG;
        //addr= CONF0_REG;
        pin_tmp = (4*pin);
        printf("%d\n", pin_tmp);  
        tmp = *(volatile uint32_t *)(data_addr+addr);
        tmp &= ~(0x0f << pin_tmp);
        tmp |=  ((stat&0x0f) << pin_tmp);
        *(volatile uint32_t *)(data_addr + addr) = tmp;
    } 
    else if (pin <= 15) 
    {
        addr=PE_REG + CONF1_REG;
        //addr= CONF1_REG;
        pin_tmp = (4*(pin-8));
        tmp = *(volatile uint32_t *)(data_addr + addr);
        tmp &= ~(0x0f << pin_tmp);
        tmp |=  ((stat&0x0f) << pin_tmp);
        *(volatile uint32_t *)(data_addr + addr) = tmp;
    } 
    else if (pin <= 23) 
    {
        addr=PE_REG + CONF2_REG;
        //addr = CONF2_REG;
        pin_tmp =  (4*(pin-16));
        tmp = *(volatile uint32_t *)(data_addr + addr);
        tmp &= ~(0x0f << pin_tmp);
        tmp |=  ((stat&0x0f) << pin_tmp);
        *(volatile uint32_t *)(data_addr + addr) = tmp;
    } 
}

int PE_get(int pin)
{
    uint32_t tmp = 0;
    uint32_t addr = PE_REG+ DATA_REG;
    tmp = *(volatile uint32_t*)(data_addr + addr);
    tmp = (tmp >> pin) & 1u;
    return tmp;
}

void PE_set(int pin, uint8_t value)
{
    uint32_t tmp;
    uint32_t addr = PE_REG+ DATA_REG;
    //addr = DATA_REG;
    if (pin <= 24) 
    {    
        tmp = *(volatile uint32_t*)(data_addr + addr);
        if (value)
        {
            tmp |= (1u << pin);
            *(volatile uint32_t*)(data_addr + addr) = tmp;
        }
        else
        {
            tmp &= ~(1u << pin);
            *(volatile uint32_t*)(data_addr + addr) = tmp;
        }
        //printf("%x\n", tmp);
    }
    
}



void gpio_close() 
{
    if (mmap_fd)
    {
        munmap(mmap_reg, 0x100);
        close(mmap_fd);
    }
}
