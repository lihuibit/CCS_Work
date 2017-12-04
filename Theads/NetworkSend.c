#include "BD_Bistatic_SAR.h"

void NetworkSend()
{
	Uint32 uSendedLen,uTmp;
	NetDataInfoStruct SendDataInfo;

	fdOpenSession( (HANDLE)Task_self());

	while(1)
	{
		Mailbox_pend(hNetSendMail,&SendDataInfo,BIOS_WAIT_FOREVER);
		uSendedLen=0;
		do
		{
			uTmp=send(csd,(char*)SendDataInfo.pDataAddr+uSendedLen,SendDataInfo.uDataLen-uSendedLen,0);
			uSendedLen += uTmp;
		}while(uSendedLen < SendDataInfo.uDataLen);
	}
}
