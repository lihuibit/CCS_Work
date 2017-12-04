
#include "BD_Bistatic_SAR.h"

Uint32 uRecvCnt;
void NetworkRecv(UArg a0, UArg a1)
{
	Uint32 recvlen;
	Uint32 totrecvlen,uPacketLen;
	NetDataInfoStruct RecvInfo;

	fdOpenSession( (HANDLE)Task_self());

	while(1)
	{
		recvlen=recv( csd, (char *)NetRecvBuf[uRecvCnt&0x7], 2*4, 0 );

		uPacketLen=NetRecvBuf[uRecvCnt&0x7][1];
		totrecvlen=0;
		do
		{
			recvlen=recv( csd, (char *)NetRecvBuf[uRecvCnt&0x7]+2*4+totrecvlen, uPacketLen-totrecvlen, 0 );
			totrecvlen += recvlen;
		}while(totrecvlen < uPacketLen);

		RecvInfo.pDataAddr=NetRecvBuf[uRecvCnt&0x7];
		RecvInfo.uDataLen=totrecvlen+2*4;

		Mailbox_post(hHostCmdResponseMail,&RecvInfo,BIOS_WAIT_FOREVER);

		uRecvCnt++;
	}
}
