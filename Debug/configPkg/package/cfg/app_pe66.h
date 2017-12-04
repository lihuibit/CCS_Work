/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-y30
 */

#include <xdc/std.h>

#include <ti/sysbios/heaps/HeapMem.h>
extern const ti_sysbios_heaps_HeapMem_Handle heapMem0;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle hNetRecvTask;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle hSigProcTask;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle hHostCmdRespTask;

#include <ti/sysbios/knl/Mailbox.h>
extern const ti_sysbios_knl_Mailbox_Handle hHostCmdResponseMail;

#include <ti/sysbios/knl/Mailbox.h>
extern const ti_sysbios_knl_Mailbox_Handle hNetSendMail;

#include <ti/sysbios/knl/Mailbox.h>
extern const ti_sysbios_knl_Mailbox_Handle hProcBufMail;

#include <ti/sysbios/hal/Hwi.h>
extern const ti_sysbios_hal_Hwi_Handle hwi0;

#include <ti/sysbios/knl/Mailbox.h>
extern const ti_sysbios_knl_Mailbox_Handle hUpLoadDataMail;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle hNetworkSendTask;

extern int xdc_runtime_Startup__EXECFXN__C;

extern int xdc_runtime_Startup__RESETFXN__C;

#ifndef ti_sysbios_knl_Task__include
#ifndef __nested__
#define __nested__
#include <ti/sysbios/knl/Task.h>
#undef __nested__
#else
#include <ti/sysbios/knl/Task.h>
#endif
#endif

extern ti_sysbios_knl_Task_Struct TSK_idle;

