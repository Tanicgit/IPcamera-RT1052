#ifndef __MYHEAP4_H
#define __MYHEAP4_H
#include "FreeRTOS.h"
#include "task.h"

#define	SDRAM1_ADDR	0X80000000
#define	SDRAM1_SIZE	0X800000	
#define	SDRAM2_ADDR SDRAM1_ADDR+SDRAM1_SIZE
#define	SDRAM2_SIZE	0X800000
#define	SDRAM3_ADDR	SDRAM2_ADDR+SDRAM2_SIZE
#define	SDRAM3_SIZE	0X1000000

//void *mypvPortMalloc( size_t xWantedSize );
//void myvPortFree( void *pv );

size_t myxPortGetFreeHeapSize( void );
size_t myxPortGetMinimumEverFreeHeapSize( void );
void myvPortInitialiseBlocks( void );
void *staticMalloc(uint32_t size);
#endif
