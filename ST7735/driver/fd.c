#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "pwm.h"
#include "spi.h"
#include "lcd.h"
#include "fd.h"

#define FB_PATH   "/dev/fb0"

int fbfd = 0;
uint16_t fb2[LCD_W * LCD_H];

long int screensize = 0;
struct fb_var_screeninfo vinfo;
void* fbp=NULL;

int refresh()
{
	const int PADDING = getpagesize();
	int i,j;
	//uint32_t offset_in_page=0;
	int xx = 0;
	int yy = 0;
	uint8_t Block = ((LCD_W*LCD_H)/ PADDING);
	uint32_t resBlock = ((LCD_W*LCD_H)% PADDING);
	
	
	memset (fb2, 0, sizeof(fb2));
	//printf ("fb2size = %d\n", sizeof(fb2));

	system("fbset -fb /dev/fb0 -g 128 160 128 320 16");

	//fbfd = open("/dev/fb0", O_RDWR);
	fbfd = open(FB_PATH, (O_RDWR|O_SYNC));
		
	//printf ("get start: \n");
	ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
	//printf("%s\n", &vinfo);

	//int xoffset = vinfo.xres/LCD_H;
	//int yoffset = vinfo.xres*((vinfo.yres/LCD_W )- 1);
	
	//int xoffset = vinfo.xres/LCD_H;
	//int yoffset = vinfo.xres*((vinfo.yres/LCD_W )- 1);

	screensize = vinfo.xres * vinfo.yres * (vinfo.bits_per_pixel/8);
	int mmapsize = (screensize + PADDING - 1) & ~(PADDING - 1);
	
#ifdef debug
	printf ("xoffset = %d, yoffset = %d\n", xoffset, yoffset);
	printf ("screensize = %ld\n", screensize);
	printf ("mapsize = %d\n", mmapsize);
#endif

	while (1)
	{
		usleep(50000);
		//sleep (1);
		//fbp = mmap(NULL,mmapsize,(PROT_READ | PROT_WRITE),MAP_SHARED,fbfd,0); 
		fbp =(char *) mmap (0, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
		//fb_addr = (char*)fbp + offset_in_page;
		uint16_t *p = fb2;
		uint16_t *x = (uint16_t *)fbp;

		for (xx = 0; xx < LCD_W; xx++)
		{
			for (yy = 0; yy < LCD_H; yy++)
			{
			  	*p++ = htons(*x);
			  	x += 1;//xoffset;
			  	//printf("%d\n", *(x));
			}
			//printf("%d\n", *(p));
			x += 1;
		}
		Address_set(0,0,LCD_W-1,LCD_H-1);
		//Address_set(0,0,LCD_W,LCD_H);
		//printf("%d\n", Block);

		p = fb2;
		for (i = 0; i <Block; i++)
		{
			for (j = 0; j < PADDING; j++)
			{
				//LCD_WR_DATA(0xf0ff);
				LCD_WR_DATA(*(p));
				//SPI_send_byte((char)( *p));
				p++;
			}
			//printf("%d\n", p);
		}
		//printf("%d\n", resBlock);
		for (i = 0; i < resBlock; i++)
		{
			LCD_WR_DATA(RED);
			//LCD_WR_DATA(*(p));
			//SPI_send_byte((char)( *p));
			//p++;
		}
		munmap (fbp, screensize);
		//cnt++;
		//printf("%d\n", cnt);
	}

	close (fbfd);
	return 0;
}
