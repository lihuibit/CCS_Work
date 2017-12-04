#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/_stack.h>
#include <ti/ndk/inc/tools/console.h>
#include <ti/ndk/inc/tools/servers.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/ndk/inc/socket.h>
#include "BD_Bistatic_SAR.h"

static HANDLE hEcho=0,hEchoUdp=0;

extern char *LocalIPAddr;

void CreateSocket()
{
	int nsendTimeOut = 10;
	struct sockaddr_in sin_svr,sin_cnt;
	int sock_len;

	fdOpenSession( (HANDLE)Task_self());

	ssd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( ssd == INVALID_SOCKET )
	{
		goto exit;
	}

	bzero( &sin_svr, sizeof(struct sockaddr_in) );
	sin_svr.sin_family = AF_INET;
	sin_svr.sin_len = sizeof( sin_svr );
	sin_svr.sin_port = htons(6000);//¶Ô·½¶Ë¿ÚºÅ
	sin_svr.sin_addr.s_addr = inet_addr(LocalIPAddr);

    /* Bind the socket */
    if ( bind( ssd, (PSA) &sin_svr, sizeof(sin_svr) ) < 0 ) {
    	goto exit;
    }

    /* If the socket is bound and TCP, start listening */
	if( listen( ssd, 5) < 0 )
	{
	  fdClose( ssd );
	  ssd = INVALID_SOCKET;
	  goto exit;
	}

  	C6678_TimeCounter_Delaycycles(1000000000);

     csd = accept(ssd, (PSA)&sin_cnt, &sock_len);
     if (csd == INVALID_SOCKET)
     {
           goto exit;
     }
     setsockopt(csd, SOL_SOCKET, SO_SNDTIMEO,&nsendTimeOut,sizeof(int));

exit:
	return;
}

void CloseSocket()
{
	fdClose( ssd );
	fdCloseSession( (HANDLE)Task_self());
}

void NetworkOpenHookFunc()
{
	CreateSocket();
	Task_setPri(hNetworkSendTask,23);
	Task_setPri(hNetRecvTask,27);

//    hEcho = DaemonNew( SOCK_STREAMNC, 0, 7, dtask_tcp_echo,
//                       OS_TASKPRINORM, OS_TASKSTKNORM, 0, 3 );
//    hEchoUdp = DaemonNew( SOCK_DGRAM, 0, 7, dtask_udp_echo,
//                          OS_TASKPRINORM, OS_TASKSTKNORM, 0, 1 );
}

void NetworkCloseHookFunc()
{
	CloseSocket();
    DaemonFree( hEchoUdp );
    DaemonFree( hEcho );
}
