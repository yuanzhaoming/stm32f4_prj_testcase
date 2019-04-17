#ifndef _spi2_h_
#define _spi2_h_

#include "stm32f4xx.h"
#include "sys_init.h"

#define	SPI2_CS 		PGout(10)  		//W5500µÄÆ¬Ñ¡ÐÅºÅ
 
extern void SPI2_Init(void);
extern void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);

extern u8 SPI2_ReadWriteByte(u8 TxData);

extern unsigned char w5500_get_interrupt(void);

extern void w5500_set_interrupt_flag(unsigned char value);

extern void w5500_interrupt_line(void);
#endif

