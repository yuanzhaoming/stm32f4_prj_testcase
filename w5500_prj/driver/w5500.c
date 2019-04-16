#include "w5500.h"
#include "sys_init.h"
#include "spi2.h"

#define USING_TCP_MODE   1

#define TCP_PORT        5000

typedef struct _W5500_REG{
	unsigned short address;
	unsigned char  control;
	unsigned char val;
}W5500_REG_T;

#define ADDRESS_VERSION   				0x0039
#define ADDRESS_GATEWAY_IP1       0x0001
#define ADDRESS_GATEWAY_IP2       0x0002
#define ADDRESS_GATEWAY_IP3       0x0003
#define ADDRESS_GATEWAY_IP4       0x0004

#define ADDRESS_NETMASK_IP1       0x0005
#define ADDRESS_NETMASK_IP2       0x0006
#define ADDRESS_NETMASK_IP3       0x0007
#define ADDRESS_NETMASK_IP4       0x0008


#define ADDRESS_MAC_SCR1       		0x0009
#define ADDRESS_MAC_SCR2       		0x000a
#define ADDRESS_MAC_SCR3       		0x000b
#define ADDRESS_MAC_SCR4       		0x000c
#define ADDRESS_MAC_SCR5       		0x000d
#define ADDRESS_MAC_SCR6       		0x000e

#define ADDRESS_SRC_IP1       		0x000f
#define ADDRESS_SRC_IP2       		0x0010
#define ADDRESS_SRC_IP3       		0x0011
#define ADDRESS_SRC_IP4       		0x0012



#define ADDRESS_Sn_MR             0x0000
#define ADDRESS_Sn_CR             0x0001
#define ADDRESS_Sn_SR             0x0003
#define ADDRESS_Sn_PORT1          0x0004
#define ADDRESS_Sn_PORT2          0x0005
#define ADDRESS_Sn_RX_RSR1        0x0026
#define ADDRESS_Sn_RX_RSR2        0x0027

#define ADDRESS_Sn_RxBUFFSIZE     0x001e
#define ADDRESS_Sn_TxBUFFSIZE     0x001f

#define ADDRESS_Sn_RXRD1     			0x0028
#define ADDRESS_Sn_RXRD2     			0x0029

#define ADDRESS_Sn_TX_FSR1     			0x0020
#define ADDRESS_Sn_TX_FSR2     			0x0021

#define ADDRESS_Sn_TX_RD1     			0x0022
#define ADDRESS_Sn_TX_RD2     			0x0023


#define ADDRESS_Sn_TX_WR1     			0x0024
#define ADDRESS_Sn_TX_WR2     			0x0025

#define ADDRESS_Sn_DIPR1          0x000c
#define ADDRESS_Sn_DIPR2          0x000d
#define ADDRESS_Sn_DIPR3          0x000e
#define ADDRESS_Sn_DIPR4          0x000f



#define ADDRESS_PHY_CFG           0x002e


#define CONTROL_BASE      				0

#define GATTEWAY_IP1_VAL      		192
#define GATTEWAY_IP2_VAL      		168
#define GATTEWAY_IP3_VAL      		25
#define GATTEWAY_IP4_VAL      		1

#define NETMASK_IP1_VAL      			255
#define NETMASK_IP2_VAL      			255
#define NETMASK_IP3_VAL      			255
#define NETMASK_IP4_VAL      			0


#define MAC_SRC1_VAL      				0x00
#define MAC_SRC2_VAL      				0x08
#define MAC_SRC3_VAL      				0xdc
#define MAC_SRC4_VAL      				0x01
#define MAC_SRC5_VAL      				0x02
#define MAC_SRC6_VAL      				0x03


#define IP1_VAL                   192
#define IP2_VAL                   168
#define IP3_VAL                   25
#define IP4_VAL                   18


W5500_REG_T W5500_REG_W_t[] = {
	{ADDRESS_GATEWAY_IP1,(CONTROL_BASE | (1<<2)),GATTEWAY_IP1_VAL},
	{ADDRESS_GATEWAY_IP2,(CONTROL_BASE | (1<<2)),GATTEWAY_IP2_VAL},
	{ADDRESS_GATEWAY_IP3,(CONTROL_BASE | (1<<2)),GATTEWAY_IP3_VAL},
	{ADDRESS_GATEWAY_IP4,(CONTROL_BASE | (1<<2)),GATTEWAY_IP4_VAL},
	
	{ADDRESS_NETMASK_IP1,(CONTROL_BASE | (1<<2)),NETMASK_IP1_VAL},
	{ADDRESS_NETMASK_IP2,(CONTROL_BASE | (1<<2)),NETMASK_IP2_VAL},
	{ADDRESS_NETMASK_IP3,(CONTROL_BASE | (1<<2)),NETMASK_IP3_VAL},
	{ADDRESS_NETMASK_IP4,(CONTROL_BASE | (1<<2)),NETMASK_IP4_VAL},	
	
	{ADDRESS_MAC_SCR1,(CONTROL_BASE | (1<<2)),MAC_SRC1_VAL},
	{ADDRESS_MAC_SCR2,(CONTROL_BASE | (1<<2)),MAC_SRC2_VAL},
	{ADDRESS_MAC_SCR3,(CONTROL_BASE | (1<<2)),MAC_SRC3_VAL},
	{ADDRESS_MAC_SCR4,(CONTROL_BASE | (1<<2)),MAC_SRC4_VAL},	
	{ADDRESS_MAC_SCR5,(CONTROL_BASE | (1<<2)),MAC_SRC5_VAL},
	{ADDRESS_MAC_SCR6,(CONTROL_BASE | (1<<2)),MAC_SRC6_VAL},

	{ADDRESS_SRC_IP1,(CONTROL_BASE | (1<<2)),IP1_VAL},
	{ADDRESS_SRC_IP2,(CONTROL_BASE | (1<<2)),IP2_VAL},
	{ADDRESS_SRC_IP3,(CONTROL_BASE | (1<<2)),IP3_VAL},
	{ADDRESS_SRC_IP4,(CONTROL_BASE | (1<<2)),IP4_VAL},	

#if USING_TCP_MODE
	{ADDRESS_Sn_MR,(CONTROL_BASE | (1<<2) | (1<<3)),0x21},/*socket0 register*/
	{ADDRESS_Sn_RxBUFFSIZE,(CONTROL_BASE | (1<<2) | (1<<3)),0x08},/*socket0 rx buffer size 8kB*/
	{ADDRESS_Sn_TxBUFFSIZE,(CONTROL_BASE | (1<<2) | (1<<3)),0x08},/*socket0 tx buffer size 8KB*/
	{ADDRESS_Sn_PORT1,(CONTROL_BASE | (1<<2) | (1<<3)),(TCP_PORT>>8)&0xff},
	{ADDRESS_Sn_PORT2,(CONTROL_BASE | (1<<2) | (1<<3)),(TCP_PORT)&0xff},
	{ADDRESS_Sn_CR,(CONTROL_BASE | (1<<2) | (1<<3)),0x01},/*socket 0 tcp open*/
	{ADDRESS_Sn_CR,(CONTROL_BASE | (1<<2) | (1<<3)),0x02},/*socket 0 tcp listen*/
	//{ADDRESS_Sn_CR,(CONTROL_BASE | (1<<2) | (1<<3)),0x40},/*socket 0 tcp receive*/
#endif
};


/*default 4*/
int w5500_version(void)
{
	unsigned char byte = 0;
	SPI2_CS=0;                            							//使能器件   
	SPI2_ReadWriteByte((ADDRESS_VERSION>>8)&0xff);             
	SPI2_ReadWriteByte((ADDRESS_VERSION)&0xff);             
	SPI2_ReadWriteByte(0);            							 	
	byte=SPI2_ReadWriteByte(0);          								//读取一个字节 
	SPI2_CS=1; 	

	printf("w5500 version:%d\r\n",byte);
	return 0;
}



 
void w5500_set()
{
	int i = 0;
	
	for(i = 0 ; i < sizeof(W5500_REG_W_t)/sizeof(W5500_REG_T) ; i ++){
		SPI2_CS=0; 
		SPI2_ReadWriteByte((W5500_REG_W_t[i].address>>8)&0xff);             
		SPI2_ReadWriteByte((W5500_REG_W_t[i].address)&0xff); 
		SPI2_ReadWriteByte(W5500_REG_W_t[i].control);
		SPI2_ReadWriteByte(W5500_REG_W_t[i].val);
		SPI2_CS=1;
	}
	
}


void w5500_get_dest_address()
{
	int i = 0;
	unsigned char val;
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((ADDRESS_Sn_DIPR1)>>8)&0xff);             
	SPI2_ReadWriteByte((ADDRESS_Sn_DIPR1)&0xff); 
	SPI2_ReadWriteByte((CONTROL_BASE | (1<<3)));
	for(i = 0 ; i < 4 ; i ++){
		val=SPI2_ReadWriteByte(0); 
		printf("dest ip%d:%d\r\n",i+1,val);
	}
	SPI2_CS=1;	
	
}

void w5500_get_ip()
{
	int i = 0;
	unsigned char val;
	SPI2_CS=0; 
		SPI2_ReadWriteByte(((ADDRESS_SRC_IP1+i)>>8)&0xff);             
		SPI2_ReadWriteByte((ADDRESS_SRC_IP1+i)&0xff); 
		SPI2_ReadWriteByte(CONTROL_BASE);
	for(i = 0 ; i < 4 ; i ++){
		val=SPI2_ReadWriteByte(0); 
		printf("ip%d:%d\r\n",i+1,val);
	}
	SPI2_CS=1;	
}

void w5500_phy_status()
{
	unsigned char val;
	
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((ADDRESS_PHY_CFG)>>8)&0xff);             
	SPI2_ReadWriteByte((ADDRESS_PHY_CFG)&0xff); 
	SPI2_ReadWriteByte(CONTROL_BASE);
	val=SPI2_ReadWriteByte(0); 
	SPI2_CS=1;	
	printf("phy:0x%0x\r\n",val);
	 		
}


void socket_open(int channel)
{
	unsigned char val;
	unsigned short address;
	
	static unsigned char connect_flag = 0;
	/*1.Sn_MR(p[3:0])='0001' 
	* 2.open active  Sn_SR=SOCKET_INIT
	* 3.can use listen and open*/
	if(channel == 0){
		SPI2_CS=0; 
		address = ADDRESS_Sn_SR;
		SPI2_ReadWriteByte(((address)>>8)&0xff);             
		SPI2_ReadWriteByte((address)&0xff); 
		SPI2_ReadWriteByte((CONTROL_BASE | (1<<3)));
		val=SPI2_ReadWriteByte(0); 
		SPI2_CS=1;	
		printf("status:0x%0x\r\n",val);	


		if(val==0x17){
			if(connect_flag == 0){
				connect_flag = 1;
				w5500_get_dest_address();
			}
		}
	}
}


void w5500_open_cfg(int channel )
{
	unsigned char val;
	unsigned short address;	
	if(channel == 0){
		SPI2_CS=0; 
		address = ADDRESS_Sn_CR;
		SPI2_ReadWriteByte(((address)>>8)&0xff);             
		SPI2_ReadWriteByte((address)&0xff); 
		SPI2_ReadWriteByte((CONTROL_BASE | (1<<3) | ( 1<<2)));
		SPI2_ReadWriteByte(0); 
		SPI2_CS=1;	
		 
	}	
}

int w5500_read_socket()
{
	/*
	*  1. read rx free pointer 
	*
	*
	*/
	int i = 0;
	int val;
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((ADDRESS_Sn_RX_RSR1)>>8)&0xff);             
	SPI2_ReadWriteByte((ADDRESS_Sn_RX_RSR1)&0xff); 
	SPI2_ReadWriteByte((CONTROL_BASE | (1<<3)));
	val=SPI2_ReadWriteByte(0); 
	val=SPI2_ReadWriteByte(0) | (val<<8); 
	
	
	SPI2_CS=1;	
	
	printf("free %d\r\n",val);
	
	return val;
}

int w5500_read_Rx_pointer()
{
	int i = 0;
	int val;
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((ADDRESS_Sn_RXRD1)>>8)&0xff);             
	SPI2_ReadWriteByte((ADDRESS_Sn_RXRD1)&0xff); 
	SPI2_ReadWriteByte((CONTROL_BASE | (1<<3)));
	val=SPI2_ReadWriteByte(0);  
	val=SPI2_ReadWriteByte(0) | (val<<8);	  
	SPI2_CS=1;
	return val;
}


void w5500_set_Rx_pointer(int count)
{
	int i = 0;
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((ADDRESS_Sn_RXRD1)>>8)&0xff);             
	SPI2_ReadWriteByte((ADDRESS_Sn_RXRD1)&0xff); 
	SPI2_ReadWriteByte((CONTROL_BASE | (1<<3)) | (1<<2));
	SPI2_ReadWriteByte((count >>8) &0xff); 
	SPI2_ReadWriteByte(count & 0xff );
	SPI2_CS=1;
}

unsigned short w5500_get_word_data(unsigned short address)
{
	int i = 0;
	unsigned short val = 0;
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((address)>>8)&0xff);             
	SPI2_ReadWriteByte((address)&0xff); 
	SPI2_ReadWriteByte((CONTROL_BASE | (1<<3)));
	val = SPI2_ReadWriteByte(0); 
	val=SPI2_ReadWriteByte(0) | (val<<8);	  
	SPI2_CS=1;	
	return val;
}


void w5500_set_word_data(unsigned short address,unsigned short value)
{
	int i = 0;
	 
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((address)>>8)&0xff);             
	SPI2_ReadWriteByte((address)&0xff); 
	SPI2_ReadWriteByte((CONTROL_BASE | (1<<3) | (1<<2)));
	SPI2_ReadWriteByte((value>>8)&0xff); 
	SPI2_ReadWriteByte(value&0xff);	  
	SPI2_CS=1;	
}



void w5500_send_recv_command()
{
	int i = 0;
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((ADDRESS_Sn_CR)>>8)&0xff);             
	SPI2_ReadWriteByte((ADDRESS_Sn_CR)&0xff); 
	SPI2_ReadWriteByte((CONTROL_BASE | (1<<3)) | (1<<2));
	 
	SPI2_ReadWriteByte(0x40);
	SPI2_CS=1;	
}


void w5500_set_byte_data(unsigned short address,unsigned char value)
{
	int i = 0;
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((address)>>8)&0xff);             
	SPI2_ReadWriteByte((address)&0xff); 
	SPI2_ReadWriteByte((CONTROL_BASE | (1<<3)) | (1<<2));
	SPI2_ReadWriteByte(value);
	SPI2_CS=1;		
}

unsigned char socket_data[100];

unsigned char socket_send_data[100] = "abcdefg";

void w5500_read_data()
{
	unsigned char val;
	int i = 0;
	int len;
	static int rx_pointer = 0;
	unsigned short free_tx = 0;
	unsigned short tx_pointer = 0;
	/*judge if have data in buffer...*/
	len = w5500_read_socket();
	if(len>0){
		
		//w5500_open_cfg(0);
		/*get the read pointer...*/
		rx_pointer = w5500_read_Rx_pointer();
		printf("rx_pointer:%d\r\n",rx_pointer);
	
		/*read data from the read pointer*/
		SPI2_CS=0; 
		SPI2_ReadWriteByte(((rx_pointer)>>8)&0xff);             
		SPI2_ReadWriteByte((rx_pointer)&0xff); 
		SPI2_ReadWriteByte((CONTROL_BASE | (3<<3)));
		for(i = 0 ; i < len ; i ++){
			socket_data[i]=SPI2_ReadWriteByte(0);
		}
		SPI2_CS=1;	
		
		printf("receive data...\r\n");
		for(i = 0 ; i < len ; i ++){
			printf("%c",socket_data[i]);
		}		
		printf("\r\n");
		
		rx_pointer += len;
		
		w5500_set_Rx_pointer(rx_pointer);
		
		w5500_send_recv_command();

		
		
		/*send data to client*/
		tx_pointer = w5500_get_word_data(ADDRESS_Sn_TX_RD1);
		printf("tx_pointer:0x%0x\r\n",tx_pointer);
		
		free_tx = w5500_get_word_data(ADDRESS_Sn_TX_FSR1);
		printf("free_tx:%d\r\n",free_tx);
		
		if(free_tx>sizeof(socket_send_data)){
			/*read data from the read pointer*/
			SPI2_CS=0; 
			SPI2_ReadWriteByte(((tx_pointer)>>8)&0xff);             
			SPI2_ReadWriteByte((tx_pointer)&0xff); 
			SPI2_ReadWriteByte((CONTROL_BASE | (2<<3)) | (1<<2));
			for(i = 0 ; i < sizeof(socket_send_data) ; i ++){
				SPI2_ReadWriteByte(socket_send_data[i]);
			}
			SPI2_CS=1;					
		}
		tx_pointer += sizeof(socket_send_data);
		w5500_set_word_data(ADDRESS_Sn_TX_WR1,tx_pointer);
		w5500_set_byte_data(ADDRESS_Sn_CR,0x20);
	}
	
	

	
}
 


int w5500_init(void)
{
	SPI2_Init();
	SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_2);
	
	
	w5500_version();
	
	w5500_set();
	w5500_get_ip();
	
	w5500_phy_status();
	
	delay_ms(100);
	socket_open(0);
}













