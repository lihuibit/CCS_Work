#include "./c6678_driver/c6678.h"
#include "./c6678_driver/emac.h"
/* Platform utilities include */
#include "ti/platform/platform.h"
/* Resource manager for QMSS, PA, CPPI */
#include "ti/platform/resource_mgr.h"

#include "BD_Bistatic_SAR.h"


#define BRADYPOD_CPM_TYPE ((BRADYPOD_CPM_TYPEDEF*)EMIF2_BACE)

extern void init_config(void);
extern void pcie_init_config(void);
void testMDIOAccess ()
{
     /* Setup MDIO Interface */
     CSL_MDIO_enablePreamble();
     CSL_MDIO_setClkDivVal(200);
     CSL_MDIO_enableStateMachine();
}

platform_info	gPlatformInfo;

void qmss_cppi_pa_init()
{
	QMSS_CFG_T      qmss_cfg;
    CPPI_CFG_T      cppi_cfg;
    /* Initialize QMSS */
    if (platform_get_coreid() == 0)
    {
        qmss_cfg.master_core        = 1;
    }
    else
    {
        qmss_cfg.master_core        = 0;
    }
    qmss_cfg.max_num_desc       = MAX_NUM_DESC;
    qmss_cfg.desc_size          = MAX_DESC_SIZE;
    qmss_cfg.mem_region         = Qmss_MemRegion_MEMORY_REGION0;
    if (res_mgr_init_qmss (&qmss_cfg) != 0)
    {
//        platform_write ("Failed to initialize the QMSS subsystem \n");
    }
    else
    {
//    	platform_write ("QMSS successfully initialized \n");
    }

    /* Initialize CPPI */
    if (platform_get_coreid() == 0)
    {
        cppi_cfg.master_core        = 1;
    }
    else
    {
        cppi_cfg.master_core        = 0;
    }
    cppi_cfg.dma_num            = Cppi_CpDma_PASS_CPDMA;
    cppi_cfg.num_tx_queues      = NUM_PA_TX_QUEUES;
    cppi_cfg.num_rx_channels    = NUM_PA_RX_CHANNELS;
    if (res_mgr_init_cppi (&cppi_cfg) != 0)
    {
//        platform_write ("Failed to initialize CPPI subsystem \n");
    }
    else
    {
//    	platform_write ("CPPI successfully initialized \n");
    }


    if (res_mgr_init_pass()!= 0) {
//        platform_write ("Failed to initialize the Packet Accelerator \n");

    }
    else
    {
//    	platform_write ("PA successfully initialized \n");
    }
}

/*******************************************************
 * 函数说明：DSP初始化函数,用于初始化DSP的时钟及外设
 *******************************************************/
void DSPInit()
{
	Srio_SockBindAddrInfo bindInfo;
	int multiid[3]={0};
	int srioid[4]={0};
	/************由核0进行芯片基本的初始化操作****************/
	if(DNUM==0)
	{
		C6678_Pll_Init(PLATFORM_PLL1_PLLM_val); //初始化PLL
		C6678_Power_UpDomains(); //使能所有模块供电
		C6678_Ecc_Enable(); //使能存储器的错误检测和校验
	    //初始化EMIF16_FPGA
	    C6678_Emif16_Fpga_Init();

	    /*
			//给fpga 物理层复位信号，与网络初始化有关！
			BRADYPOD_CPM_TYPE->REG_CPM_GPIO1_I = 0x04;
			C6678_Pll_Delay(10000000);
			BRADYPOD_CPM_TYPE->REG_CPM_GPIO1_I = 0x0;
			C6678_Pll_Delay(10000000);
			BRADYPOD_CPM_TYPE->REG_CPM_GPIO1_I = 0x04;
	    */
	}

	/**********需要各个核都调用的初始化函数***************/
	C6678_TimeCounter_Enable(); //使能每个核内的时间戳计数器

	//核0进行后续的外设初始化
	if (DNUM == 0)
	{
		/***********片级中断控制器初始化************/
		C6678_ChipInt_Init (0);

		/*************DDR3初始化****************/
		C6678_Ddr3_Init(PLLM_DDR3,DDR3_TYPE); //初始化DDR

		/*************EDMA全局初始化*******************/
	   //EDMA全局初始化
		//C6678_Edma3_Global_Init();
	}

//	//初始化0核EDMA通道,用来将数据搬移到DDR上
//	if (DNUM == 0)
//	{
//		edma_channel.CC= 0;
//		edma_channel.Chan_in = DNUM;
//		edma_channel.Para_in = 4*DNUM;
//		edma_channel.TCNum = DNUM%4;
//		edma_channel.RegionNum = DNUM;
//		C6678_Edma3_Channel_Init(edma_channel.CC,edma_channel.Chan_in,edma_channel.TCNum,edma_channel.RegionNum,edma_channel.Para_in);
//	}

	//初始化srio
	if(DNUM==0)
	{
		multiid[0]=0;
		srioid[0]=DEVICE_ID1_8BIT;
		memset((void*)&bindInfo, 0, sizeof(Srio_SockBindAddrInfo));
	    //初始化SRIO
	    if (C6678_Srio_Init(SRIO_RATE,srioid[0],multiid,0)!= TRUE)
	    {
	    	//printf("SRIO failed to initialize!!!!!!!!!!!!!!!!!!!!!!!!! \n" );
	    }

	    if(C6678_Srio_Bind(Srio_SocketType_DIO,bindInfo) != TRUE)
		{
			//printf("C6678_Srio_Bind() error!\n");
		}

	    C6678_TimeCounter_Delaycycles(100000);
	}
	Cache_wbInvAll();


	/****************Network Related Initialization Start******************/
	if(DNUM==0)
	{
		testMDIOAccess();
		C6678_TimeCounter_Delaycycles(1000000);

		C6678_ConfigSerdes();
		C6678_TimeCounter_Delaycycles(1000000);

		C6678_Init_SGMII_Auto(1);
		C6678_TimeCounter_Delaycycles(1000000);

		qmss_cppi_pa_init();
	}

	/****************Network Related Initialization End******************/

}
