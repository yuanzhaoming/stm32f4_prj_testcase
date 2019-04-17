#include "stm32f4xx.h"
#include "sys_init.h"
#include "uart2.h"
#include "timer3.h"
#include "led.h"
#include "spi2.h"
#include "w5500.h"

/**
*
*  	using usart2 we must connet p8 
*		usart2---> pa2<---> tx   pa3<--->rx
*   
*   spi2   mcu                           w5500
*   	MOSI<--->PB15           <--->       
*			MISO<--->PB14
*      SCK<--->PB13
*       CS<--->PG10
*interrupt<--->PI11           <--->       /INT
*		 reset<--->PG12(NRF_CE)   <--->       /RESET
*
*
*/


int main(void)
{
	static unsigned int now_time = 0;
	unsigned char value;
	unsigned char tmp;
	unsigned char socket_status_val; 
	int led_val = 0;
	HAL_Init(); 
	Stm32_Clock_Init(360,25,2,8);	
	uart2_init(115200);
	
	LED_Init();
	//定时器3初始化，定时器时钟为90M，分频系数为9000-1，
	//所以定时器3的频率为90M/9000=10K，自动重装载为10-1，那么定时器周期就是1ms
	TIM3_Init(10-1,9000-1); 
	w5500_interrupt_line();
	w5500_init();
	
	printf("usart2 init ok...\r\n");
	
	
	while(1){
		if(get_sys_time()-now_time >= 2){
			now_time = get_sys_time();
			led_val = !led_val;
			led_flash(1,led_val);
			
			 
			value = w5500_get_cfg_bytedata(W5500_PHYCFGR,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
			printf("%s\r\n",value&0x1?"link ok":"not linked");
			
			
			socket_status();
			
			if(w5500_get_interrupt_flag()){
				w5500_set_interrupt_flag(0);
				value = w5500_get_cfg_bytedata(W5500_INTERRUPT,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
				
				printf("value:0x%0x\r\n",value);
				if(value){
					tmp = 0;
					w5500_set_cfg_data(W5500_INTERRUPT,W550_CONTROL(SOCKn_COM,0,0,W5500_WRITE,W5500_VDM),&tmp,1);
					value = w5500_get_cfg_bytedata(W5500_INTERRUPT,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
					printf("IR:0x%0x\r\n",value);
					
				}
				
				value = w5500_get_cfg_bytedata(W5500_SOCKET_INTERRUPT,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
				if(value & 0x1){
					printf("socket0 interrupt:0x%0x\r\n",value);
					
					socket_status_val = w5500_get_cfg_bytedata(W5500_Sn_IR,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_READ,W5500_VDM));
					
					printf("socket_status_val:0x%0x\r\n",socket_status_val);
					w5500_set_cfg_data(W5500_Sn_IR,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_WRITE,W5500_VDM),&socket_status_val,1);
					
					value = w5500_get_cfg_bytedata(W5500_SOCKET_INTERRUPT,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
					printf("SIR:0x%0x\r\n",value);
					
					if(socket_status_val & 0x04){
						/*recv*/
						
					}
					if(socket_status_val & 0x10){
						/*send ok*/
						
					}				
					
				}
				
				
			}
		}
	}

	return 0;
}

