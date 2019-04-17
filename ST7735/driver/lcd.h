#ifndef __LCD_H
#define __LCD_H	

#include "spi.h"
#include "font.h"


#define LCD_W 128
#define LCD_H 160


#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 
#define BRRED 			 0XFC07 
#define GRAY  			 0X8430 

#define DARKBLUE      	 0X01CF	
#define LIGHTBLUE      	 0X7D7C	 
#define GRAYBLUE       	 0X5458 

 
#define LIGHTGREEN     	 0X841F 
#define LGRAY 			 0XC618 

#define LGRAYBLUE        0XA651 
#define LBBLUE           0X2B12 


extern  u16 BACK_COLOR, POINT_COLOR;  

void delayms(int count);

void Lcd_Init(void); 
void Reg_Init(void);
void LCD_Clear(u16 Color);
void Address_set(u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_WR_DATA8(u8 da); 
void LCD_WR_DATA(u16 da);
void LCD_WR_REG(u8 da);

void LCD_DrawPoint(u16 x,u16 y);
void LCD_DrawPoint_big(u16 x,u16 y);

void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);
void LCD_ShowString(u16 x,u16 y,char *p);	
void showhanzi(unsigned int x,unsigned int y,unsigned char index);

void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);

u16  LCD_ReadPoint(u16 x,u16 y); 

#endif  
	 
	 



