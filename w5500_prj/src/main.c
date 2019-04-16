#include "stm32f4xx.h"
#include "sys_init.h"
#include "uart2.h"
#include "timer3.h"
#include "led.h"

/**
*  	using usart2 we must connet p8 
*		usart2---> pa2<---> tx   pa3<--->rx
*   
*   spi2
*   	MOSI<--->PB15
*			MISO<--->PB14
*      SCK<--->PB13
*       CS<--->PG10
*			
*
*/


int main(void)
{
	static unsigned int now_time = 0;
	int led_val = 0;
	HAL_Init(); 
	Stm32_Clock_Init(360,25,2,8);	
	uart2_init(115200);
	
	LED_Init();
	//定时器3初始化，定时器时钟为90M，分频系数为9000-1，
	//所以定时器3的频率为90M/9000=10K，自动重装载为10-1，那么定时器周期就是1ms
	TIM3_Init(10-1,9000-1); 
	
	w5500_init();
	
	printf("usart2 init ok...\r\n");
	
	
	while(1){
		if(get_sys_time()-now_time >= 1){
				now_time = get_sys_time();
				led_val = !led_val;
				led_flash(1,led_val);
			
			socket_open(0);
			
			w5500_read_data();
		}
	}

	return 0;
}

