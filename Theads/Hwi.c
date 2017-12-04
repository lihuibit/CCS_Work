#include "BD_Bistatic_SAR.h"

///*******************************************************
// * 函数说明：通过L2相对地址获得，指定核的L2数据的绝对地址返回
// *******************************************************/
//Uint32 Get_L2_Global_Address(Uint32 addr, int core_num)
//{
//    	Uint32 Globle_L2_addr;
//    	Globle_L2_addr = 0x10000000 + core_num*0x1000000 +addr;
//    	return  Globle_L2_addr;
//}



/*******************************************************
 * 函数说明：SRIO中断处理函数(核0处理srio中断)
 *******************************************************/
void SrioDoorBell_Isr_handler()
{
	int i= 0;
	//失效Cache
	Cache_inv((Uint32 *)&Srio_Recv_L3, sizeof(Srio_Recv_L3), Cache_Type_L2,TRUE);

	//置标志位给核1，核2，核3,核4通知其处理数据
	for(i = 1; i <= 4; i++)
	{
		*((Uint8*)Get_L2_Global_Address((Uint32)&IsEchoDataReady,i)) = 1;
	}

	//Mailbox_post(hProcBufMail,(Uint32 *)&Srio_Recv_L3,BIOS_WAIT_FOREVER);
	C6678_Srio_DoorbellInt_Clear(DNUM,0);//清中断
}



