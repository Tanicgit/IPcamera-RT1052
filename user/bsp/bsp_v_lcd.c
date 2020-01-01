/*
8080 方式需要实现此文件
*/
#include "bsp_v_lcd.h"
#include "bsp_base.h"

#include "myheap4.h"

#define SWAP16(a)		((uint16_t)a>>8|(uint16_t)a<<8)

static uint16_t *LCD_RAM;
void V_lcd_init()
{
	LCD_RAM = staticMalloc(V_LCD_W*V_LCD_H*2);
	if(LCD_RAM==NULL)
	{
		__aeabi_assert("LCD_RAM Malloc Err",__FILE__,__LINE__);
	}
}

void drawpoint(int x,int y,uint16_t c)
{
	*(LCD_RAM+y*V_LCD_W+x) = c;
}

void drawHlineData(int x,int y,int size, uint16_t *c)
{
  for (;size > 0; size--, x++, c++) {
		*(LCD_RAM+y*V_LCD_W+x) = (*c);
  }	
}

uint16_t readpoint(int x,int y)
{
  return *(LCD_RAM+y*V_LCD_W+x);
}










