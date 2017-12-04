#include "BD_Bistatic_SAR.h"

SOCKET ssd = INVALID_SOCKET;
SOCKET csd = INVALID_SOCKET;
Uint32 NetRecvBuf[8][50];
Uint32 SRIORecvCnt;



/*-----------------------------------------------------------
 * 				声明在L2中的参数变量
 *------------------------------------------------------------*/
#pragma SET_DATA_SECTION(".L2")

Uint32 SRIO_INTC_NUM 	= 0;//SRIO接收计数
Uint8  IsEchoDataReady 	= 0;//srio接收到数据后将该标志位置1,其他核处理完成后将该标志置0
Uint32 Echo_Procd_Index = 0;//核1~核3处理完成计数，模64存储到缓冲区
Uint32 Echo_Send_Index 	= 0;//核4网络发送计数，模64

complex	IFFT_Echo_L2[ECHODATA_POINT];//临时存储IFFT结果
#pragma SET_DATA_SECTION()




/*-------------------------------------------------------------
 * 声明在L3中的参数变量
 *-------------------------------------------------------------*/
#pragma SET_DATA_SECTION(".L3")

#pragma DATA_ALIGN (Srio_Recv_L3,	64);
#pragma DATA_ALIGN (NetSendFrames_L3,	64);
#pragma DATA_ALIGN (cFFTTwidTable_L3,	64);


#pragma LOCATION(Srio_Recv_L3,0xC010000)
#pragma RETAIN(Srio_Recv_L3)

SRIORECVFRAME 	Srio_Recv_L3;//用于存储srio接收到的原始帧数据，ping-pong缓存
NETSENDFRAME 	NetSendFrames_L3[64];//用于存储处理完成待发送的数据，存放64组，用于缓冲
complex 		cFFTTwidTable_L3[ECHODATA_POINT];//IFFT蝶形因子

#pragma SET_DATA_SECTION()







