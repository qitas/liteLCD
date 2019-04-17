
#include "spi.h"
#include "lcd.h"
#include "font.h"
#include "pwm.h"

u16 BACK_COLOR, POINT_COLOR;  

void delayms(int count)  
{
   	int i;
   	for(i=0;i<count;i++) usleep(1000);
}


void LCD_Writ_Bus(char dat)   
{	
	for(char i = 0; i<8; i++)
	{
		SPI_SET_Clk(0);
		bit_delay();		
		if(dat&0x80) SPI_SET_MOSI(1);
		else SPI_SET_MOSI(0);
		bit_delay();
		//bit_delay();
		SPI_SET_Clk(1);
		dat <<= 1; 
		bit_delay();
	}
	//bit_delay();		  		
}

void LCD_WR_DATA8(uint8_t da) 
{
	SPI_SET_CS(0);
	lcd_DC(1);
	LCD_Writ_Bus(da);
	SPI_SET_CS(1);
}  

 void LCD_WR_DATA(uint16_t da)
{
	SPI_SET_CS(0);
	lcd_DC(1);
	LCD_Writ_Bus(da>>8);
    LCD_Writ_Bus(da);
    SPI_SET_CS(1);
}	

void LCD_WR_REG(uint8_t da)	 
{	
	SPI_SET_CS(0);
	lcd_DC(0);
	LCD_Writ_Bus(da);
	SPI_SET_CS(1);
	lcd_DC(1);
}

 void LCD_WR_REG_DATA(uint16_t reg,uint16_t da)
{
    LCD_WR_REG(reg);
	LCD_WR_DATA(da);

}
void Address_set(u16 x1,u16 y1,u16 x2,u16 y2)
{ 
	LCD_WR_REG(0x2a);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(x1+2);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(x2+2);

	LCD_WR_REG(0x2b);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(y1+1);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(y2+1);	
   	LCD_WR_REG(0x2C);					 						 
}

void Lcd_Init(void)
{
	//printf("start init spi.\n");	
	SPI_soft_init();
	PE_init(RES_Pin, GPIO_DIR_OUT);
	PE_init(DC_Pin, GPIO_DIR_OUT);
	//PB_init(2, GPIO_DIR_OUT);
	//PB_set(2, 1);
	//PE_init(BLK_Pin, GPIO_DIR_OUT);

	//printf("start init blk pwm.\n");	
	pwm_init(1);
	pwm_rate(1, 1000, 80);
	pwm_start(1);
}
void Reg_Init(void)
{
	//printf("start to write reg.\n");

   	LCD_WR_REG_DATA(0,0);
   	LCD_ShowNum(0,0,0,0);
   	LCD_Show2Num(0,0,0,0);
   	//LCD_DrawPoint_big(0,0);
   	//LCD_DrawRectangle(0,0,0,0);
 	LCD_Reset(0) ;
	//LCD_REST=0;
	delayms(20);
	LCD_Reset(1) ;
	//LCD_REST=1;
	delayms(20);
	//
	
	LCD_WR_REG(0x11);
	delayms (120);
		
	//ST7735R Frame Rate
	LCD_WR_REG(0xB1); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 

	LCD_WR_REG(0xB2); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 

	LCD_WR_REG(0xB3); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 
	
	LCD_WR_REG(0xB4); //Column inveLCD_DC ion 
	LCD_WR_DATA8(0x07); 
	
	//ST7735R Power Sequence
	LCD_WR_REG(0xC0); 
	LCD_WR_DATA8(0xA2); 
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x84); 
	LCD_WR_REG(0xC1); 
	LCD_WR_DATA8(0xC5); 

	LCD_WR_REG(0xC2); 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0x00); 

	LCD_WR_REG(0xC3); 
	LCD_WR_DATA8(0x8A); 
	LCD_WR_DATA8(0x2A); 
	LCD_WR_REG(0xC4); 
	LCD_WR_DATA8(0x8A); 
	LCD_WR_DATA8(0xEE); 
	
	LCD_WR_REG(0xC5); //VCOM 
	LCD_WR_DATA8(0x0E); 
	
	LCD_WR_REG(0x36); //MX, MY, RGB mode 
	LCD_WR_DATA8(0xC0); 
#ifdef USE_LANDSCAPE
	LCD_WR_DATA8(0xA8); //竖屏C8 横屏08 A8
#else
	LCD_WR_DATA8(0xC8); //竖屏C8 横屏08 A8
#endif		
	//ST7735R Gamma Sequence
	LCD_WR_REG(0xe0); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x1a); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x18); 
	LCD_WR_DATA8(0x2f); 
	LCD_WR_DATA8(0x28); 
	LCD_WR_DATA8(0x20); 
	LCD_WR_DATA8(0x22); 
	LCD_WR_DATA8(0x1f); 
	LCD_WR_DATA8(0x1b); 
	LCD_WR_DATA8(0x23); 
	LCD_WR_DATA8(0x37); 
	LCD_WR_DATA8(0x00); 	
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x10); 

	LCD_WR_REG(0xe1); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x1b); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x17); 
	LCD_WR_DATA8(0x33); 
	LCD_WR_DATA8(0x2c); 
	LCD_WR_DATA8(0x29); 
	LCD_WR_DATA8(0x2e); 
	LCD_WR_DATA8(0x30); 
	LCD_WR_DATA8(0x30); 
	LCD_WR_DATA8(0x39); 
	LCD_WR_DATA8(0x3f); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0x10);  
	
	LCD_WR_REG(0x2a);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00+2);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x80+2);

	LCD_WR_REG(0x2b);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00+3);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x80+3);
	
	LCD_WR_REG(0xF0); //Enable test command  
	LCD_WR_DATA8(0x01); 
	LCD_WR_REG(0xF6); //Disable ram power save mode 
	LCD_WR_DATA8(0x00); 
	
	LCD_WR_REG(0x3A); //65k mode 
	LCD_WR_DATA8(0x05); 
		
	LCD_WR_REG(0x29);//Display on
	//printf("done lcd init.\n");
  }

//***************************************************************

//清屏函数
//Color:要清屏的填充色
void LCD_Clear(u16 Color)
{
	u16 i,j;  	
	Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	LCD_WR_DATA(Color);	 			 
	    }

	  }
}


void showhanzi(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j;
	unsigned char *temp=hanzi;    
    Address_set(x,y,x+31,y+31); //设置区域      
	temp+=index*128;	
	for(j=0;j<128;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
		temp++;
	 }
}
//画点
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	Address_set(x,y,x,y);
	LCD_WR_DATA(POINT_COLOR); 	    
} 	 

void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	Address_set(xsta,ysta,xend,yend);      //设置光标位置 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//设置光标位置 	    
	} 					  	    
} 

void LCD_DrawPoint_big(u16 x,u16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
}

void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	int di;
	//BACK_COLOR=WHITE;
	POINT_COLOR=color;	
	a=0;b=r;	  
	di=3-(r<<1);            
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++; 
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    		   
	num=num-' ';
	Address_set(x,y,x+8-1,y+16-1);    
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//画一个点     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   

u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
	
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;
	num=(u16)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
	}
} 
	 
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 

void LCD_ShowString(u16 x,u16 y,char *p)
{ 
	//BACK_COLOR=back;        
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=8;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}












