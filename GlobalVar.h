#ifndef GLOBALVARDEFINE
#define GLOBALVARDEFINE

#include "BD_Bistatic_SAR.h"


extern SOCKET ssd;
extern SOCKET csd;
extern Uint32 NetRecvBuf[8][50];
extern Uint32 SRIORecvCnt;


#define ECHODATA_POINT	16*1024 //回波点数
#define CUT_POINT	256		//截取的点数
#define	SENDBUF_NUM 64//发送缓冲区的个数

#define EMIF1_BACE 0x74000000//CMDR寄存器的首地址
#define EMIF2_BACE 0x78000000//CMDR寄存器的首地址

//定义CMDR寄存器
#define CMDR_REG ((CMDR_TYPEDEF*)EMIF1_BACE)

//获取指定核上变量的绝对地址
#define Get_L2_Global_Address(addr, core_num) (0x10000000 + (core_num)*0x1000000 + (addr))

/*******************************结构体声明*********************************************/
//配置参数结构体
typedef struct _ConfigParaStruct
{
	Uint32 uDataType;//数据类型码
	Uint32 uDataLen;//数据长度
	Uint32 uWaveForm;//波形模式码
	Uint32 uCodeType;//脉间编码模式
	Uint32 uFarWavePulsWidth;//远距波形脉宽
	Uint32 uPRT;//PRT 6.25nm为单位
	Uint32 uSampWinLen;//采样窗点数
	Uint32 uStartSampRange;//起始采样距离 6.25ns
	Uint32 uUploadSampNum;//实时成像上传点数
	Uint32 uUploadPeriod;//信息上传周期
	Uint32 FHSSPoint;//处理跳频点数
	Uint32 uAGC1;//AGC控制码1
	Uint32 uAGC2;//AGC控制码2
	Uint32 uStdChnEnable;//标准通道使能
	Uint32 uStdChnCtrl;//标准通道控制
	Uint32 AntennaScanMode;//天线转动模式
	Uint32 PitchWaveNumOrRoundNum;//俯仰波段数或者旋转圈数
	Uint32 AngleNum;//角度个数
	float uAziScanVel_New;//新增方位角扫描速度
	float uEleScanVel_New;
	float uAziScanStart;//方位向扫描起始角度/俯仰波位角1
	float uAziScanEnd;//方位向扫描终止角度/俯仰波位角2
	float uAziScanVel;//方位向扫描速度/俯仰波位角3
	float uEleScanStart;//俯仰向扫描起始角度/俯仰波位角4
	float uEleScanEnd;//俯仰向扫描终止角度/俯仰波位角5
	float uEleScanVel;//俯仰向扫描速度/俯仰波位角6
	Uint32 uReserved[8];
}ConfigParaStruct;


//SRIO传输的帧格式
#pragma pack(1)
typedef struct _SRIORECVFRAME
{
	/**********************通道1*************************/
	Uint32 	CH1_FrameInfo;//通道1帧头信息 4B
	Uint8  	CH1_AntennaLoc[12];//通道1天线位置 12B
	Uint32 	CH1_FrameNum;//通道1帧号 4B
	Uint8	CH1_RPNNum;//通道1RPN号  1B
	Uint8	CH1_TraceState;//通道1的跟踪状态 1B
	Uint8	CH1_MessagePhase;//通道1的电文相位 1B
	Uint8 	CH1_SNR;//通道1的信噪比 1B
	Uint32	CH1_BDTimeScale;//通道1的时标 4B
	Uint32	CH1_CarryWavePhase;//通道1的载波相位4B
	Uint32	CH1_Doppler;//通道1的多普勒4B
	Uint8	CH1_SatelliteLoc[12];//通道1的卫星位置 12B
	Uint32	CH1_Angle;//通道1的仰角方位角4B
	Uint8	CH1_Reserved[12];//通道1保留 12B
	float	CH1_EchoData[ECHODATA_POINT*2];//通道1回波数据 128KB

	/**********************通道2*************************/
	Uint32 	CH2_FrameInfo;//通道1帧头信息 4B
	Uint8  	CH2_AntennaLoc[12];//通道1天线位置 12B
	Uint32 	CH2_FrameNum;//通道1帧号 4B
	Uint8	CH2_RPNNum;//通道1RPN号  1B
	Uint8	CH2_TraceState;//通道1的跟踪状态 1B
	Uint8	CH2_MessagePhase;//通道1的电文相位 1B
	Uint8 	CH2_SNR;//通道1的信噪比 1B
	Uint32	CH2_BDTimeScale;//通道1的时标 4B
	Uint32	CH2_CarryWavePhase;//通道1的载波相位4B
	Uint32	CH2_Doppler;//通道1的多普勒4B
	Uint8	CH2_SatelliteLoc[12];//通道1的卫星位置 12B
	Uint32	CH2_Angle;//通道1的仰角方位角4B
	Uint8	CH2_Reserved[12];//通道1保留 12B
	float	CH2_EchoData[ECHODATA_POINT*2];//通道1回波数据 128KB

	/**********************通道3*************************/
	Uint32 	CH3_FrameInfo;//通道1帧头信息 4B
	Uint8  	CH3_AntennaLoc[12];//通道1天线位置 12B
	Uint32 	CH3_FrameNum;//通道1帧号 4B
	Uint8	CH3_RPNNum;//通道1RPN号  1B
	Uint8	CH3_TraceState;//通道1的跟踪状态 1B
	Uint8	CH3_MessagePhase;//通道1的电文相位 1B
	Uint8 	CH3_SNR;//通道1的信噪比 1B
	Uint32	CH3_BDTimeScale;//通道1的时标 4B
	Uint32	CH3_CarryWavePhase;//通道1的载波相位4B
	Uint32	CH3_Doppler;//通道1的多普勒4B
	Uint8	CH3_SatelliteLoc[12];//通道1的卫星位置 12B
	Uint32	CH3_Angle;//通道1的仰角方位角4B
	Uint8	CH3_Reserved[12];//通道1保留 12B
	float	CH3_EchoData[ECHODATA_POINT*2];//通道1回波数据 128KB
}SRIORECVFRAME;

//网络发送的帧格式
typedef struct _NETSENDFRAME
{
	Uint8	IsHaveData;//是否有数据
	Uint32 	FrameInfo;//帧头信息 4B
	Uint8  	AntennaLoc[12];//天线位置 12B
	Uint8	Reseverd[16];//保留	   16B

	/**********************通道1*************************/
	Uint32 	CH1_FrameNum;//通道1帧号 4B
	Uint8	CH1_RPNNum;//通道1RPN号  1B
	Uint8	CH1_TraceState;//通道1的跟踪状态 1B
	Uint8	CH1_MessagePhase;//通道1的电文相位 1B
	Uint8 	CH1_SNR;//通道1的信噪比 1B
	Uint32	CH1_BDTimeScale;//通道1的时标 4B
	Uint32	CH1_CarryWavePhase;//通道1的载波相位4B
	Uint32	CH1_Doppler;//通道1的多普勒4B
	Uint8	CH1_SatelliteLoc[12];//通道1的卫星位置 12B
	Uint32	CH1_Angle;//通道1的仰角方位角4B
	Uint8	CH1_Reserved[28];//通道1保留 28B
	float	CH1_EchoData[CUT_POINT*2];//通道1回波数据 2KB

	/**********************通道2*************************/
	Uint32 	CH2_FrameNum;//通道1帧号 4B
	Uint8	CH2_RPNNum;//通道1RPN号  1B
	Uint8	CH2_TraceState;//通道1的跟踪状态 1B
	Uint8	CH2_MessagePhase;//通道1的电文相位 1B
	Uint8 	CH2_SNR;//通道1的信噪比 1B
	Uint32	CH2_BDTimeScale;//通道1的时标 4B
	Uint32	CH2_CarryWavePhase;//通道1的载波相位4B
	Uint32	CH2_Doppler;//通道1的多普勒4B
	Uint8	CH2_SatelliteLoc[12];//通道1的卫星位置 12B
	Uint32	CH2_Angle;//通道1的仰角方位角4B
	Uint8	CH2_Reserved[28];//通道1保留 12B
	float	CH2_EchoData[CUT_POINT*2];//通道1回波数据 2KB

	/**********************通道3*************************/
	Uint32 	CH3_FrameNum;//通道1帧号 4B
	Uint8	CH3_RPNNum;//通道1RPN号  1B
	Uint8	CH3_TraceState;//通道1的跟踪状态 1B
	Uint8	CH3_MessagePhase;//通道1的电文相位 1B
	Uint8 	CH3_SNR;//通道1的信噪比 1B
	Uint32	CH3_BDTimeScale;//通道1的时标 4B
	Uint32	CH3_CarryWavePhase;//通道1的载波相位4B
	Uint32	CH3_Doppler;//通道1的多普勒4B
	Uint8	CH3_SatelliteLoc[12];//通道1的卫星位置 12B
	Uint32	CH3_Angle;//通道1的仰角方位角4B
	Uint8	CH3_Reserved[28];//通道1保留 12B
	float	CH3_EchoData[CUT_POINT*2];//通道1回波数据 2KB
}NETSENDFRAME;


//CMDR寄存器组
typedef struct CMDR_TYPEDEF_STRU
{
	volatile Uint16 REG_CMDR_SR_PENDING             ;//0X00000000
	volatile Uint16 REG_CMDR_SR_MASK_SET            ;//0X00000002
	volatile Uint16 REG_CMDR_SR_MASK_CLR            ;//0X00000004
	volatile Uint16 REG_CMDR_PUL0                   ;//0X00000006
	volatile Uint16 RESERVED0[12];
	volatile Uint16 REG_CMDR_CONFIGO_REG0           ;//0X00000020
	volatile Uint16 REG_CMDR_CONFIGO_REG1           ;//0X00000022
	volatile Uint16 REG_CMDR_CONFIGO_REG2           ;//0X00000024
	volatile Uint16 REG_CMDR_CONFIGO_REG3           ;//0X00000026
	volatile Uint16 REG_CMDR_CONFIGO_REG4           ;//0X00000028
	volatile Uint16 REG_CMDR_CONFIGO_REG5           ;//0X0000002a
	volatile Uint16 REG_CMDR_CONFIGO_REG6           ;//0X0000002c
	volatile Uint16 REG_CMDR_CONFIGO_REG7           ;//0X0000002e
	volatile Uint16 REG_CMDR_CONFIGO_REG8           ;//0X00000030
	volatile Uint16 REG_CMDR_CONFIGO_REG9           ;//0X00000032
	volatile Uint16 RESERVED1[6];
	volatile Uint16 REG_CMDR_DATAO_REG0             ;//0X00000040
	volatile Uint16 REG_CMDR_DATAO_REG1             ;//0X00000042
	volatile Uint16 REG_CMDR_DATAO_REG2             ;//0X00000044
	volatile Uint16 REG_CMDR_DATAO_REG3             ;//0X00000046
	volatile Uint16 REG_CMDR_DATAO_REG4             ;//0X00000048
	volatile Uint16 REG_CMDR_DATAO_REG5             ;//0X0000004a
	volatile Uint16 REG_CMDR_DATAO_REG6             ;//0X0000004c
	volatile Uint16 REG_CMDR_DATAO_REG7             ;//0X0000004e
	volatile Uint16 REG_CMDR_DATAO_REG8             ;//0X00000050
	volatile Uint16 REG_CMDR_DATAO_REG9             ;//0X00000052
	volatile Uint16 RESERVED2[6];
	volatile Uint16 REG_CMDR_CONFIGI_REG0           ;//0X00000060
	volatile Uint16 REG_CMDR_CONFIGI_REG1           ;//0X00000062
	volatile Uint16 REG_CMDR_CONFIGI_REG2           ;//0X00000064
	volatile Uint16 REG_CMDR_CONFIGI_REG3           ;//0X00000066
	volatile Uint16 REG_CMDR_CONFIGI_REG4           ;//0X00000068
	volatile Uint16 REG_CMDR_CONFIGI_REG5           ;//0X0000006a
	volatile Uint16 REG_CMDR_CONFIGI_REG6           ;//0X0000006c
	volatile Uint16 REG_CMDR_CONFIGI_REG7           ;//0X0000006e
	volatile Uint16 REG_CMDR_CONFIGI_REG8           ;//0X00000070
	volatile Uint16 REG_CMDR_CONFIGI_REG9           ;//0X00000072
	volatile Uint16 RESERVED3[6];
	volatile Uint16 REG_CMDR_DATAI_REG0             ;//0X00000080
	volatile Uint16 REG_CMDR_DATAI_REG1             ;//0X00000082
	volatile Uint16 REG_CMDR_DATAI_REG2             ;//0X00000084
	volatile Uint16 REG_CMDR_DATAI_REG3             ;//0X00000086
	volatile Uint16 REG_CMDR_DATAI_REG4             ;//0X00000088
	volatile Uint16 REG_CMDR_DATAI_REG5             ;//0X0000008a
	volatile Uint16 REG_CMDR_DATAI_REG6             ;//0X0000008c
	volatile Uint16 REG_CMDR_DATAI_REG7             ;//0X0000008e
	volatile Uint16 REG_CMDR_DATAI_REG8             ;//0X00000090
	volatile Uint16 REG_CMDR_DATAI_REG9             ;//0X00000092
}CMDR_TYPEDEF;


typedef  struct _complex
{
	float     re;
	float     im;
}complex;

typedef  struct _complex_int16
{
	short     re;
	short     im;
}complex_int16;

typedef struct _NetDataInfoStruct
{
	Uint32 *pDataAddr;
	Uint32 uDataLen;
}NetDataInfoStruct;



/*******************************全局变量定义*********************************************/
extern far				Uint32 SRIO_INTC_NUM ;//接收srio中断计数
extern far				Uint8  IsEchoDataReady;//srio接收到数据后将该标志位置1,其他核处理完成后将该标志置0
extern far				Uint32 Echo_Procd_Index;//核1~核3处理完成计数，模64存储到缓冲区
extern far				Uint32 Echo_Send_Index;//核4网络发送计数，模64

extern complex			IFFT_Echo_L2[ECHODATA_POINT];//临时存储IFFT结果

extern SRIORECVFRAME 	Srio_Recv_L3;//用于存储srio接收到的原始帧数据
extern NETSENDFRAME 	NetSendFrames_L3[SENDBUF_NUM];//用于存储处理完成待发送的数据，存放64组，用于缓冲
extern complex 			cFFTTwidTable_L3[ECHODATA_POINT];//IFFT蝶形因子

#endif







