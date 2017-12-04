#include <xdc/std.h>
#include <ti/ndk/inc/netmain.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/ndk/inc/_stack.h>
#include <ti/sysbios/family/c66/Cache.h>
#include <ti/sysbios/hal/Hwi.h>


extern Mailbox_Handle hHostCmdResponseMail;
extern Mailbox_Handle hNetSendMail;
extern Mailbox_Handle hProcBufMail;
extern Mailbox_Handle hUpLoadDataMail;
extern Task_Handle hNetRecvTask;
extern Task_Handle hNetworkSendTask;


