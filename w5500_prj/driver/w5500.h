#ifndef _w5500_h_
#define _w5500_h_

#include "stm32f4xx.h"
#include "sys_init.h"


#define W5500_DEBUG   1

#if W5500_DEBUG
#define W5500_DEBUG_PRINT(fmt,...) printf(fmt,##__VA_ARGS__)
#else
#define W5500_DEBUG_PRINT(fmt,...) do{}while(0);
#endif

#define USE_TCP_SERVER     1
#define USE_TCP_CLIENT     0

#define USE_UDP_CLIENT     0

#define PHY_CFG_LINK                   (1<<0)
#define PHY_CFG_DPX                    (1<<1)


#define IM_IR7                          (1<<7)   //ip conflict
#define IM_IR6                          (1<<6)   //dest reachable

#define MODE_CLOSED                     (0<<0)
#define MODE_TCP                        (1<<0)
#define MODE_UDP                        (2<<0)
#define MODE_MACRAW                     (4<<0)


#define OPEN                            0x01
#define LISTEN                          0x02
#define CONNECT                         0x04
#define DISCON                          0x08

#define SEND                            0x20
#define RECV                            0x40

#define SOCK_CLOSED                     0
#define SOCK_INIT                       0x13
#define SOCK_LISTEN                     0x14
#define SOCK_ESTABLISHED                0x17
#define SOCK_CLOSE_WAIT                 0x1c
#define SOCK_UDP                        0x22


#define W5500_ADDRESS_MR                0x0000
#define W5500_ADDRESS_GATEWAY           0x0001
#define W5500_ADDRESS_SUBNETMASK        0x0005
#define W5500_ADDRESS_SOURCEHARDWARE    0x0009
#define W5500_ADDRESS_IP                0x000f


#define W5500_INTLEVEL                  0x0013                 

#define W5500_INTERRUPT                 0x0015
#define W5500_INTERRUPT_MASK            0x0016
#define W5500_SOCKET_INTERRUPT          0x0017
#define W5500_SOCKET_INTERRUPT_MASK     0x0018

#define W5500_RETRY_TIME                0x0019
#define W5500_RETRY_COUNT               0x001b

#define W5500_PHYCFGR                   0x002e
#define W5500_KEEPALIVE                 0x002f

#define W5500_ADDRESS_CHIP_VERSION      0x0039



#define W5500_Sn3_MR                    0x0000
#define W5500_Sn_CR                     0x0001
#define W5500_Sn_IR                     0x0002
#define W5500_Sn_SR                     0x0003
#define W5500_Sn_PORT                   0x0004
#define W5500_Sn_IMR                    0x002c

#define W5500_Sn_RXBUF_SIZE             0x001e
#define W5500_Sn_TXBUF_SIZE             0x001f
#define W5500_Sn_TX_FSR                 0x0020
#define W5500_Sn_TX_RD                  0x0022
#define W5500_Sn_TX_WR                  0x0024

#define W5500_Sn_RX_RSR                 0x0026
#define W5500_Sn_RX_RD                  0x0028
#define W5500_Sn_WR                     0x002a
 



#define W5500_VDM          (0<<0)

#define W5500_READ         (0<<2)
#define W5500_WRITE        (1<<2)


#define SOCKETn_REGISTER_OFFSET         0
#define SOCKETn_SEND_REGISTER_OFFSET    1
#define SOCKETn_RECV_REGISTER_OFFSET    2

#define SOCKn_COM          							(0 << 3)
#define SOCKn_REG(n,off)       					((4*n+1+off) <<3)



	


/*source ip address*/
#define SRC_IP1  192
#define SRC_IP2  168
#define SRC_IP3  25
#define SRC_IP4  18

/*source subnetmask*/
#define SRC_SUB_NETMASK1 255
#define SRC_SUB_NETMASK2 255
#define SRC_SUB_NETMASK3 255
#define SRC_SUB_NETMASK4 0

/*source gateway*/
#define SRC_GATEWAY1 192
#define SRC_GATEWAY2 168
#define SRC_GATEWAY3 25
#define SRC_GATEWAY4 1


#define SRC_MAC_ADDR1  0x00
#define SRC_MAC_ADDR2  0x08
#define SRC_MAC_ADDR3  0xdc
#define SRC_MAC_ADDR4  0x01
#define SRC_MAC_ADDR5  0x02
#define SRC_MAC_ADDR6  0x03


#if USE_TCP_SERVER
#define TCP_SRC_PORT   5000
#endif



#endif

