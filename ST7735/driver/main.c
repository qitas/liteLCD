#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>

#include "pwm.h"
#include "spi.h"
#include "lcd.h"
#include "fd.h"

int main(int argc, char *argv[]) 
{ 	
	uint16_t cnt=0;
	char bright=90;
	char* src=0;
	Lcd_Init();
	Reg_Init();
	//LCD_Clear(BLUE);
	//sleep(1);
	LCD_Clear(WHITE);
	//sleep(1);
	refresh();

    if(strcmp(argv[1],"init")==0 && argc==2) 
	{
		Reg_Init();
		LCD_Clear(BLUE); 
	}	   
    if(strcmp(argv[1],"pwm")==0 && argc==3) 
	{
		bright = atoi(argv[2]);
		pwm_rate(1,1000,bright);
	}

	if(strcmp(argv[1],"str")==0) 
	{
		src = argv[2];
		//printf(src);
		LCD_Clear(BLUE); 
		BACK_COLOR=BLUE;
		POINT_COLOR=RED;	
	 	LCD_ShowString(8,8,src);
	}	
	else 
	{
		LCD_Clear(BLUE); 
		BACK_COLOR=BLUE;
		POINT_COLOR=RED;
		LCD_ShowString(20,10,"OAZON V.ZEN");
	}
    //pwm_init(1);
	//pwm_start(1);
	//pwm_rate(1,100,60);
	//LCD_Clear(BLUE); 
	//xianshi(GREEN); 
	while(0)
	{
		cnt++;
		//showimage(); 
		//delayms(50);
		
		//xianshi(cnt); 
		//usleep(50000);		
		//LCD_Clear(cnt*10); 
		//usleep(50000);

		if((cnt%128) < 64)
		{
			Draw_Circle(64,96,cnt%64,RED);
		}		
		else
		{
			Draw_Circle(64,96,(64-cnt%64),BLUE);
		}
		if(argc>2) 
		{
			BACK_COLOR=BLUE;
			POINT_COLOR=RED;	
		 	LCD_ShowString(20,10,src);
		}
		else 
		{
			BACK_COLOR=BLUE;
			POINT_COLOR=RED;
			LCD_ShowString(20,10,"OAZON V.ZEN");
		}

		//if(cnt%64==0) LCD_Clear(BLUE); 
		usleep(50000);

		
		
    }
}
