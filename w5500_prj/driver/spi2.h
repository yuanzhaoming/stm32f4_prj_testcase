#ifndef _spi2_h_
#define _spi2_h_

#include "stm32f4xx.h"
#include "sys_init.h"

#define	SPI2_CS 		PGout(10)  		//W5500µÄÆ¬Ñ¡ÐÅºÅ
 
extern u8 SPI2_ReadWriteByte(u8 TxData);


#endif

