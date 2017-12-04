#include "BD_Bistatic_SAR.h"


/************************************函数声明**********************************/
void SendDataByNet();
void Process();
extern void DSPF_sp_ifftSPxSP (int N, float *ptr_x, float *ptr_w, float *ptr_y,
    unsigned char *brev, int n_min, int offset, int n_max);
/***************************************************************************/


/*******************************************************
 * 函数说明：数据处理线程，其中核0通过网络向上位机发送数据，核1~4进行IFFT变换，
 * 其他核阻塞
 *******************************************************/
void SignalProcess()
{
	while(1)
	{
		if((DNUM==5) || (DNUM==6) || (DNUM == 7))//核0，核5~7不做处理,阻塞
		{
			Mailbox_pend(hProcBufMail,NULL,BIOS_WAIT_FOREVER);
		}
		else if (DNUM == 0)//通过网络向上位机发送数据
		{
			if (NetSendFrames_L3[Echo_Send_Index%SENDBUF_NUM].IsHaveData == 1)//如果当前数据准备好了，则发送
			{
				SendDataByNet();//通过网络发送
				NetSendFrames_L3[Echo_Send_Index%SENDBUF_NUM].IsHaveData = 0;//标志清0
				Echo_Send_Index++;//计数加1
			}
		}
		else//核1~核4等待IsEchoDataReady信号进行数据处理
		{
			if (IsEchoDataReady)
			{
				Process();//数据处理
				IsEchoDataReady = 0;//清标志位
			}
		}
	}
}



/*******************************************************
 * 函数说明：核4拷贝包头参数信息到NetSendFrames_L3缓冲中
 *******************************************************/
void CopyHeadInfo()
{
	//头部信息
	NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].IsHaveData = 1;
	NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].FrameInfo = 0x5b5b5b5b;//帧头信息 4B
	memcpy((void*)NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].AntennaLoc,
			(void*)Srio_Recv_L3.CH1_AntennaLoc, sizeof(Srio_Recv_L3.CH1_AntennaLoc));
	//通道1参数信息
	memcpy((void*)&NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].CH1_FrameNum,
			(void*)&Srio_Recv_L3.CH1_FrameNum, ((Uint32)&Srio_Recv_L3.CH1_Reserved) - (Uint32)(&Srio_Recv_L3.CH1_FrameNum));

	//通道2的参数信息
	memcpy((void*)&NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].CH2_FrameNum,
			(void*)&Srio_Recv_L3.CH2_FrameNum, (Uint32)(&Srio_Recv_L3.CH2_Reserved) - (Uint32)(&Srio_Recv_L3.CH2_FrameNum));

	//通道3参数信息
	memcpy((void*)&NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].CH3_FrameNum,
			(void*)&Srio_Recv_L3.CH3_FrameNum, (Uint32)(&Srio_Recv_L3.CH3_Reserved) - (Uint32)(&Srio_Recv_L3.CH3_FrameNum));

}
/*******************************************************
 * 函数说明：数据处理
 * 			其中核1~3最回波数据进行IFFT运算将结果发送到NetSendFrames_L3缓冲中
 * 			核4将包头及参数信息拷贝到缓冲区中
 *******************************************************/
void Process()
{
	if (DNUM == 1)
	{
		DSPF_sp_ifftSPxSP (ECHODATA_POINT, (float*)Srio_Recv_L3.CH1_EchoData,
				(float*)cFFTTwidTable_L3, (float*)IFFT_Echo_L2, 0, 0, 0, 0);
		//通道1回波数据 **长度对不对
		memcpy((void*)NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].CH1_EchoData,
					(void*)IFFT_Echo_L2, sizeof(NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].CH1_EchoData));
	}
	else if (DNUM == 2)
	{
		DSPF_sp_ifftSPxSP (ECHODATA_POINT, (float*)Srio_Recv_L3.CH2_EchoData,
				(float*)cFFTTwidTable_L3, (float*)IFFT_Echo_L2, 0, 0, 0, 0);
		//通道2回波数据
		memcpy((void*)NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].CH2_EchoData,
						(void*)IFFT_Echo_L2, sizeof(NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].CH2_EchoData));
	}
	else if (DNUM == 3)
	{
		DSPF_sp_ifftSPxSP (ECHODATA_POINT, (float*)Srio_Recv_L3.CH3_EchoData,
				(float*)cFFTTwidTable_L3, (float*)IFFT_Echo_L2, 0, 0, 0, 0);
		//通道3回波数据
		memcpy((void*)NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].CH3_EchoData,
							(void*)IFFT_Echo_L2, sizeof(NetSendFrames_L3[Echo_Procd_Index%SENDBUF_NUM].CH3_EchoData));
	}
	else if (DNUM == 4)
	{
		CopyHeadInfo();
	}
	Echo_Procd_Index++;//
}

/*******************************************************
 * 函数说明：核0通过网络向上位机发送帧数据
 *******************************************************/
void SendDataByNet()
{
	NetDataInfoStruct SendDataInfo;
	Uint32 uSendedLen,uTmp;
	SendDataInfo.pDataAddr = (Uint32*)&NetSendFrames_L3[Echo_Send_Index%SENDBUF_NUM].FrameInfo;
	SendDataInfo.uDataLen = sizeof(NETSENDFRAME) - 1;//实际每帧发送的长度
	uSendedLen = 0;
	fdOpenSession( (HANDLE)Task_self());
	do
	{
		uTmp=send(csd,(char*)SendDataInfo.pDataAddr+uSendedLen,SendDataInfo.uDataLen-uSendedLen,0);
		uSendedLen += uTmp;
	}while(uSendedLen < SendDataInfo.uDataLen);
}
