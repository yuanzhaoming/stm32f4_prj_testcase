#include "w5500.h"
#include "sys_init.h"
#include "spi2.h"
#include "timer3.h"

typedef struct _TCP_STATUS_T{
		int val;
		char *status;
}TCP_STATUS_T;

typedef struct _SOCKET_STATUS_T{
		int val;
		char *status;
}SOCKET_STATUS_T;

static unsigned char src_ip[4]={SRC_IP1,SRC_IP2,SRC_IP3,SRC_IP4};
static unsigned char src_netmask[4]={SRC_SUB_NETMASK1,SRC_SUB_NETMASK2,SRC_SUB_NETMASK3,SRC_SUB_NETMASK4};
static unsigned char src_gateway[4]={SRC_GATEWAY1,SRC_GATEWAY2,SRC_GATEWAY3,SRC_GATEWAY4};
static unsigned char src_mac[6]={SRC_MAC_ADDR1,SRC_MAC_ADDR2,SRC_MAC_ADDR3,SRC_MAC_ADDR4,SRC_MAC_ADDR5,SRC_MAC_ADDR6};

TCP_STATUS_T tcp_status_t[10] = {
		{OPEN,"tcp open"},
		{LISTEN,"tcp listen"},
		{CONNECT,"tcp connect"},
		{DISCON,"tcp discon"}
};

SOCKET_STATUS_T socket_status_t[10] = {
		{SOCK_CLOSED,"socket closed"},
		{SOCK_INIT,"socket init"},
		{SOCK_LISTEN,"socket listen"},
		{SOCK_ESTABLISHED,"socket established"},
		{SOCK_CLOSE_WAIT,"socket close wait"},
		{SOCK_UDP,"socket udp"},
};


unsigned char W550_CONTROL(unsigned char bsb,unsigned char n,unsigned char off,unsigned char rwb, unsigned char om)
{
	if(bsb==SOCKn_COM) 
		return (SOCKn_COM | rwb | om);
	else 
		return (SOCKn_REG(n,off) | rwb | om);
}

void w5500_set_cfg_data(unsigned short address,unsigned char control,unsigned char *data,int len)
{
	int i = 0;
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((address)>>8)&0xff);             
	SPI2_ReadWriteByte((address)&0xff); 
	SPI2_ReadWriteByte(control);
	for(i = 0 ; i < len ; i ++){
		SPI2_ReadWriteByte(*data & 0xff );
		data++;
	}
	SPI2_CS=1;	
}


unsigned short w5500_get_cfg_worddata(unsigned short address,unsigned char control)
{
	int i = 0;
	unsigned short value;
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((address)>>8)&0xff);             
	SPI2_ReadWriteByte((address)&0xff); 
	SPI2_ReadWriteByte(control);
	value = SPI2_ReadWriteByte(0); 
	value = SPI2_ReadWriteByte(0) | (value << 8);
	SPI2_CS=1;	
	return value;
}


unsigned short w5500_get_cfg_bytedata(unsigned short address,unsigned char control)
{
	int i = 0;
	unsigned char value;
	SPI2_CS=0; 
	SPI2_ReadWriteByte(((address)>>8)&0xff);             
	SPI2_ReadWriteByte((address)&0xff); 
	SPI2_ReadWriteByte(control);
	value = SPI2_ReadWriteByte(0); 
	SPI2_CS=1;	
	return value;
}

/*default 4*/
int w5500_get_version(void)
{
	unsigned char version;
	version = w5500_get_cfg_bytedata(W5500_ADDRESS_CHIP_VERSION,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
	printf("w5500 version:%d\r\n",version);
	return version;
}



void w5500_set_ip(void)
{
	int i = 0;
	unsigned char ip_address[4];
	w5500_set_cfg_data(W5500_ADDRESS_IP,W550_CONTROL(SOCKn_COM,0,0,W5500_WRITE,W5500_VDM),src_ip,4);
	
	
	W5500_DEBUG_PRINT("ip:");
	for(i = 0 ; i < 3; i ++){
		ip_address[i] = w5500_get_cfg_bytedata(W5500_ADDRESS_IP+i,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
		W5500_DEBUG_PRINT("%d.",ip_address[i]);
	}	
	ip_address[i] = w5500_get_cfg_bytedata(W5500_ADDRESS_IP+i,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
	W5500_DEBUG_PRINT("%d",ip_address[i]);
	W5500_DEBUG_PRINT("\r\n");
}

void w5500_set_netmask(void)
{
	int i = 0;
	unsigned char netmask_address[4];
	w5500_set_cfg_data(W5500_ADDRESS_SUBNETMASK,W550_CONTROL(SOCKn_COM,0,0,W5500_WRITE,W5500_VDM),src_netmask,4);
	
	W5500_DEBUG_PRINT("netmask:");
	for(i = 0 ; i < 3; i ++){
		netmask_address[i] = w5500_get_cfg_bytedata(W5500_ADDRESS_SUBNETMASK+i,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
		W5500_DEBUG_PRINT("%d.",netmask_address[i]);
	}
	netmask_address[i] = w5500_get_cfg_bytedata(W5500_ADDRESS_SUBNETMASK+i,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
	W5500_DEBUG_PRINT("%d",netmask_address[i]);	
	W5500_DEBUG_PRINT("\r\n");
}


void w5500_set_gateway(void)
{
	int i = 0;
	unsigned char gateway_address[4];
	w5500_set_cfg_data(W5500_ADDRESS_GATEWAY,W550_CONTROL(SOCKn_COM,0,0,W5500_WRITE,W5500_VDM),src_gateway,4);
	
	W5500_DEBUG_PRINT("gateway:");
	for(i = 0 ; i < 3; i ++){
		gateway_address[i] = w5500_get_cfg_bytedata(W5500_ADDRESS_GATEWAY+i,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
		W5500_DEBUG_PRINT("%d.",gateway_address[i]);
	}
	gateway_address[i] = w5500_get_cfg_bytedata(W5500_ADDRESS_GATEWAY+i,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
	W5500_DEBUG_PRINT("%d",gateway_address[i]);	
	W5500_DEBUG_PRINT("\r\n");
}

void w5500_set_macaddress(void)
{
	int i = 0;
	unsigned char mac_address[4];
	w5500_set_cfg_data(W5500_ADDRESS_SOURCEHARDWARE,W550_CONTROL(SOCKn_COM,0,0,W5500_WRITE,W5500_VDM),src_mac,6);
	
	W5500_DEBUG_PRINT("mac:");
	for(i = 0 ; i < 5; i ++){
		mac_address[i] = w5500_get_cfg_bytedata(W5500_ADDRESS_SOURCEHARDWARE+i,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
		W5500_DEBUG_PRINT("%02x.",mac_address[i]);
	}
	mac_address[i] = w5500_get_cfg_bytedata(W5500_ADDRESS_SOURCEHARDWARE+i,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
	W5500_DEBUG_PRINT("%02x",mac_address[i]);	
	W5500_DEBUG_PRINT("\r\n");
}

/*set w5500 interrupt*/
void w5500_set_interrupt(void)
{
	int i = 0;
	unsigned char imr = IM_IR7 | IM_IR6;
	unsigned char tmp;
	unsigned char tmp_arr[2] = {0,1} ;
	w5500_set_cfg_data(W5500_INTERRUPT_MASK,W550_CONTROL(SOCKn_COM,0,0,W5500_WRITE,W5500_VDM),&imr,1);
	
	
	W5500_DEBUG_PRINT("imr:");
	tmp = w5500_get_cfg_bytedata(W5500_INTERRUPT_MASK,W550_CONTROL(SOCKn_COM,0,0,W5500_READ,W5500_VDM));
	W5500_DEBUG_PRINT("0x%0x\r\n",tmp);
	W5500_DEBUG_PRINT("\r\n");
	
	
	tmp = 0x01;/*enable socket0 interrrupt*/
	/**/
	w5500_set_cfg_data(W5500_SOCKET_INTERRUPT_MASK,W550_CONTROL(SOCKn_COM,0,0,W5500_WRITE,W5500_VDM),&tmp,1);
	
	/*int time*/
	w5500_set_cfg_data(W5500_INTLEVEL,W550_CONTROL(SOCKn_COM,0,0,W5500_WRITE,W5500_VDM),tmp_arr,2);
	
	tmp = 0x1f;
	w5500_set_cfg_data(W5500_Sn_IMR,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_WRITE,W5500_VDM),&tmp,1);
}


void w5500_set_socket_mode(unsigned char mode)
{
	int i = 0;
	unsigned char tmp;
	w5500_set_cfg_data(W5500_Sn3_MR,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_WRITE,W5500_VDM),&mode,1);
	
	
	W5500_DEBUG_PRINT("mode:");
	
	tmp = w5500_get_cfg_bytedata(W5500_Sn3_MR,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_READ,W5500_VDM));
	W5500_DEBUG_PRINT("0x%0x\r\n",tmp);
	W5500_DEBUG_PRINT("\r\n");	
}

void w5500_set_socket_status(unsigned char mode)
{
	int i = 0;
	unsigned char tmp;
	w5500_set_cfg_data(W5500_Sn_CR,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_WRITE,W5500_VDM),&mode,1);
	
	
#if 0  /*we can only set it ,then it will clear the register to zero,so read will be zero*/
	W5500_DEBUG_PRINT("tcp status:");
	
	tmp = w5500_get_cfg_bytedata(W5500_Sn_CR,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_READ,W5500_VDM));
	printf("tmp:0x%0x\r\n",tmp);
	
	if(tmp < 10){
		for(i = 0 ; i < sizeof(tcp_status_t)/sizeof(TCP_STATUS_T) ; i ++){
			if(tcp_status_t[i].val == tmp){
				break;
			}
		}
	}
	W5500_DEBUG_PRINT("%s\r\n",tcp_status_t[i].status);
	W5500_DEBUG_PRINT("\r\n");	
#endif
}

void w5500_set_buff_size()
{
	int i = 0;
	unsigned char len = 2;
	unsigned char tmp;
	w5500_set_cfg_data(W5500_Sn_RXBUF_SIZE,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_WRITE,W5500_VDM),&len,1);
	w5500_set_cfg_data(W5500_Sn_TXBUF_SIZE,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_WRITE,W5500_VDM),&len,1);
	
	W5500_DEBUG_PRINT("rx bufsize:");
	tmp = w5500_get_cfg_bytedata(W5500_Sn_RXBUF_SIZE,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_READ,W5500_VDM));
	W5500_DEBUG_PRINT("%dKbytes\r\n",tmp);

	W5500_DEBUG_PRINT("tx bufsize:");
	tmp = w5500_get_cfg_bytedata(W5500_Sn_TXBUF_SIZE,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_READ,W5500_VDM));
	W5500_DEBUG_PRINT("%dKbytes\r\n",tmp);
}

void w5500_set_socket_port(unsigned short port)
{
	int i = 0;
	unsigned char tmp[2];
	unsigned char port_arr[2] = {(port>>8)&0xff,port&0xff};
	
	w5500_set_cfg_data(W5500_Sn_PORT,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_WRITE,W5500_VDM),port_arr,2);
	
	
	W5500_DEBUG_PRINT("socket port:");
	for(i = 0 ; i < 2 ; i ++){
		tmp[i] = w5500_get_cfg_bytedata(W5500_Sn_PORT+i,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_READ,W5500_VDM));
	}
	W5500_DEBUG_PRINT("%d\r\n",(tmp[0]<<8)|tmp[1]);
}

void w5500_set_keepalive(void)
{
	unsigned char tmp = 2;
	w5500_set_cfg_data(W5500_KEEPALIVE,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_WRITE,W5500_VDM),&tmp,1);
}


void socket_status()
{
	unsigned char value;
	int i = 0;
	value = w5500_get_cfg_bytedata(W5500_Sn_SR,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_READ,W5500_VDM));
	for (i = 0 ; i < sizeof(socket_status_t)/sizeof(SOCKET_STATUS_T); i ++){
		if(socket_status_t[i].val == value){
			break;
		}
	}
	printf("%s\r\n",socket_status_t[i].status);	

	if(socket_status_t[i].val == SOCK_CLOSE_WAIT){
		/*need close and reopen*/
		
		w5500_set_socket_status(DISCON);
		w5500_set_socket_status(OPEN);
		w5500_set_socket_status(LISTEN);
	}
}
 
void w5500_reset()
{
	unsigned char tmp = (1<<7);
	w5500_set_cfg_data(W5500_ADDRESS_MR,W550_CONTROL(SOCKn_COM,0,0,W5500_WRITE,W5500_VDM),&tmp,1);
}

int w5500_init(void)
{
	int i = 0;
	unsigned char value;
	SPI2_Init();
	SPI2_SetSpeed(SPI_BAUDRATEPRESCALER_2);
	
	w5500_reset();
	
	w5500_get_version();
	
	w5500_set_ip();
	w5500_set_netmask();
	w5500_set_gateway();
	w5500_set_macaddress();
	w5500_set_interrupt();
	w5500_set_keepalive();

#if USE_TCP_SERVER
	w5500_set_buff_size();
	w5500_set_socket_port(TCP_SRC_PORT);
	w5500_set_socket_mode(MODE_TCP);
	w5500_set_socket_status(OPEN);
	
	delay_ms(1000);
	
	value = w5500_get_cfg_bytedata(W5500_Sn_SR,W550_CONTROL(SOCKn_REG(0,0),0,0,W5500_READ,W5500_VDM));
	 
	if(value == SOCK_INIT){
		printf("socket init ok...\r\n");
	}
	else{
		printf("socket init error...\r\n");
		return -1;
	}
	
	
	w5500_set_socket_status(LISTEN);
	
	
	

	
	
#endif
	
	return 0;
}













