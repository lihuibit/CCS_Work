#include "BD_Bistatic_SAR.h"

//函数声明
void ConfigFPGA(ConfigParaStruct*  CFGParam);
void CommandToFPGA(unsigned char cmd);


/*******************************************************
 * 函数说明：上位机指令解析及反馈，任务优先级仅次于网络接收任务
 *******************************************************/
void HostCmdResponse()
{
	NetDataInfoStruct RecvCmd;//从上位机端接收到的数据
	ConfigParaStruct  CFGParam;//配置参数
	while(1)
	{
		Mailbox_pend(hHostCmdResponseMail,&RecvCmd,BIOS_WAIT_FOREVER);
		if(RecvCmd.pDataAddr[0] == 0x5a5a5a5a) //0x5a5a5a5a 为指令
		{
			switch (RecvCmd.pDataAddr[2])
			{
			case 1://自检
				CommandToFPGA(0x10);
				Mailbox_post(hNetSendMail,&RecvCmd,BIOS_WAIT_FOREVER);
				break;
			case 3://开始
				CommandToFPGA(0x11);
				Mailbox_post(hNetSendMail,&RecvCmd,BIOS_WAIT_FOREVER);
				break;
			case 4://停止
				CommandToFPGA(0x12);
				Mailbox_post(hNetSendMail,&RecvCmd,BIOS_WAIT_FOREVER);
				break;
			default:
				break;
			}

		}
		else if(RecvCmd.pDataAddr[0] == 0x6b6b6b6b) //0x6b6b6b6b 为系统配置
		{
			//解析上位机的配置参数
			memcpy((void*)&CFGParam, (void*)RecvCmd.pDataAddr, RecvCmd.uDataLen);
			//给FPGA配置参数
			ConfigFPGA(&CFGParam);
			ProcInit();//初始化算法
			Mailbox_post(hNetSendMail,&RecvCmd,BIOS_WAIT_FOREVER);
		}
	}
}

/*******************************************************
 * 函数说明：方位向和俯仰角拼接函数，用于匹配配置FPGA的参数
 *******************************************************/
void DegreeSplit(float srcStart, float srcEnd, unsigned char* HXDW, unsigned char* HXDW1, unsigned char* HXDW2)
{
	unsigned short degreeStartNum, degreeEndNum;

	degreeStartNum = 2048 + srcStart/0.1;
	degreeEndNum = 2048 + srcEnd/0.1 ;
	*HXDW = (unsigned char)((degreeStartNum&0xF00)>>4 | (degreeEndNum&0xF00)>>8);
	*HXDW1 = (unsigned char)(degreeStartNum&0xff);
	*HXDW2 = (unsigned char)(degreeEndNum&0xff);
}

/*******************************************************
 * 函数说明：方位向俯仰向速度拼接，用于匹配配置FPGA的参数
 *******************************************************/
void VelocitySplit(float HXSRC, float FYSRC, unsigned char* HXFY_H, unsigned char* HXCS_L, unsigned char* FYCS_L)
{
	unsigned short HXSRC_NUM, FYSRC_NUM;
	HXSRC_NUM = HXSRC/0.02;
	FYSRC_NUM = FYSRC/0.02;
	*HXFY_H = (unsigned char)((HXSRC_NUM&0xF00)>>4 | (FYSRC_NUM&0xF00)>>8);
	*HXCS_L = (unsigned char)(HXSRC_NUM&0xff);
	*FYCS_L = (unsigned char)(FYSRC_NUM&0xff);
}

/*******************************************************
 * 函数说明：通过EMIF给FPGA配置参数
 *******************************************************/
void ConfigFPGA(ConfigParaStruct*  CFGParam)
{
	int i = 0;
	unsigned char HXDW, HXDW1, HXDW2, FXDW, FXDW1, FXDW2,HXFY_H,HXCS_L,FYCS_L;
	//给fpga配置参数
	DegreeSplit(CFGParam->uAziScanStart, CFGParam->uAziScanEnd, &HXDW, &HXDW1, &HXDW2);
	DegreeSplit(CFGParam->uEleScanStart, CFGParam->uEleScanEnd, &FXDW, &FXDW1, &FXDW2);
	VelocitySplit(CFGParam->uAziScanVel, CFGParam->uEleScanVel, &HXFY_H, &HXCS_L, &FYCS_L);
//	CMDR_REG->REG_CMDR_SR_MASK_CLR |= 0x01;//使能0号中断
//	//发送第一帧数据
//	CMDR_REG->REG_CMDR_CONFIGO_REG0 = 0x20EB;
//	CMDR_REG->REG_CMDR_CONFIGO_REG1 = 0x0113;
//	CMDR_REG->REG_CMDR_CONFIGO_REG2 = 0x0042;
//	CMDR_REG->REG_CMDR_CONFIGO_REG3 = 0x0000;
//
//	CMDR_REG->REG_CMDR_CONFIGO_REG4 = 15*(CFGParam->uStartSampRange&0x3f)*20/18.75;//采样起始点？
//	CMDR_REG->REG_CMDR_CONFIGO_REG5 = 0x0000;//采样起始点
//	CMDR_REG->REG_CMDR_CONFIGO_REG6 = 256 << (CFGParam->uSampWinLen&0x3);//采样窗点数 1024
//	CMDR_REG->REG_CMDR_CONFIGO_REG7 = 0x2800;//极化方式？
//	if (CFGParam->uSampWinLen&0x3 == 3)//说明是2048点则脉宽变为40us
//	{
//		CFGParam->uPRT = 3;
//	}
//	CMDR_REG->REG_CMDR_CONFIGO_REG8 = (((CFGParam->uPRT&0x3)*5)+25)*1000/6.25;//prt周期,6.25nm为单位 4000
//	CMDR_REG->REG_CMDR_CONFIGO_REG9 = 0x0000;
//	CMDR_REG->REG_CMDR_DATAO_REG0 = (HXDW<<8 | AntennaScanMode[CFGParam->AntennaScanMode&0x7]);//伺服工作模式
//	CMDR_REG->REG_CMDR_DATAO_REG1 = (HXDW2<<8 | HXDW1);
//	CMDR_REG->REG_CMDR_DATAO_REG2 = (FXDW1<<8 | FXDW);
//	CMDR_REG->REG_CMDR_DATAO_REG3 = (HXFY_H<<8|FXDW2);
//	CMDR_REG->REG_CMDR_DATAO_REG4 = (FYCS_L<<8|HXCS_L);
//	CMDR_REG->REG_CMDR_DATAO_REG5 = (unsigned short)((CFGParam->AngleNum<<8)|CFGParam->PitchWaveNumOrRoundNum);
//	CMDR_REG->REG_CMDR_DATAO_REG6 = (unsigned short)(2048 + CFGParam->uAziScanStart*10);
//	CMDR_REG->REG_CMDR_DATAO_REG7 = (unsigned short)(2048 + CFGParam->uAziScanEnd*10);
//	CMDR_REG->REG_CMDR_DATAO_REG8 = (unsigned short)(2048 + CFGParam->uAziScanVel*10);
//	CMDR_REG->REG_CMDR_DATAO_REG9 = (unsigned short)(2048 + CFGParam->uEleScanStart*10);
//
//	//DSP通过CMDR寄存器（REG_CMDR_PUL0）向Koala发送中断
//	CMDR_REG->REG_CMDR_PUL0 |= 0x01;
//	//gai
//	while(!(CMDR_REG->REG_CMDR_SR_PENDING & 0x01));//等待fpga端的1号中断
//	CMDR_REG->REG_CMDR_SR_PENDING |= 0x01;//清除中断标志位
//	//发送第二帧数据
//	CMDR_REG->REG_CMDR_CONFIGO_REG0 = 0x12EB;
//	CMDR_REG->REG_CMDR_CONFIGO_REG1 = 0x0213;
//	CMDR_REG->REG_CMDR_CONFIGO_REG2 = 0x0042;
//	CMDR_REG->REG_CMDR_CONFIGO_REG3 = 0x0000;
//
//	for (i = 0; i < CFGParam->AngleNum-4; i++)
//	{
//		*(&CMDR_REG->REG_CMDR_CONFIGO_REG4+i) = (unsigned short)(2048+ *((float*)(&CFGParam->uEleScanEnd + i))*10);
//	}
//	CMDR_REG->REG_CMDR_DATAO_REG0 = (unsigned short)(2048+ *((float*)(&CFGParam->uEleScanEnd + 6))*10);
//	CMDR_REG->REG_CMDR_DATAO_REG1 = (unsigned short)(CFGParam->uAziScanVel_New/0.02);//方位扫描速度,上位机需要给出？
//	CMDR_REG->REG_CMDR_DATAO_REG2 = (unsigned short)((CFGParam->PitchWaveNumOrRoundNum&0xff)|(CFGParam->AntennaScanMode>7)<<8);  //|天线扫描模式高八位);
//	CMDR_REG->REG_CMDR_DATAO_REG3 = (unsigned short)(((CFGParam->uCodeType+1)&0xff)<<8 | ((CFGParam->uWaveForm+1)&0xff));//脉间编码模式，波形模式
//	CMDR_REG->REG_CMDR_DATAO_REG4 = (unsigned short)(CFGParam->uFarWavePulsWidth&0xff == 5)?1:(((CFGParam->uFarWavePulsWidth+1)&0xff)*10);
//	if (CFGParam->uFarWavePulsWidth == 5)
//		CMDR_REG->REG_CMDR_DATAO_REG4 = 1;
//	else
//		CMDR_REG->REG_CMDR_DATAO_REG4 = (unsigned short)(((CFGParam->uFarWavePulsWidth+1)&0xff)*10);
//
//	CMDR_REG->REG_CMDR_DATAO_REG5 = (unsigned short)(CFGParam->uEleScanVel_New/0.02);//俯仰扫描速度
//	CMDR_REG->REG_CMDR_DATAO_REG6 = (unsigned short)(((CFGParam->uAGC2)&0xFF)<<8 | ((CFGParam->uAGC2)&0xFF));//AGC1和AGC2
//	CMDR_REG->REG_CMDR_DATAO_REG7 = (((CFGParam->uStdChnCtrl)&0xFF)<<8 | ((CFGParam->uStdChnEnable)&0xFF));//校准通道使能（地位）和选择（高位）
//	//TEST
//	//DSP通过CMDR寄存器（REG_CMDR_PUL0）向Koala发送中断
//	CMDR_REG->REG_CMDR_PUL0 |= 0x01;
//	while(!(CMDR_REG->REG_CMDR_SR_PENDING & 0x01));//等待fpga端的1号中断
//	CMDR_REG->REG_CMDR_SR_PENDING |= 0x01;//清除中断标志位
}

/*******************************************************
 * 函数说明：通过EMIF给FPGA发送指令
 *******************************************************/
void CommandToFPGA(unsigned char cmd)
{
//	CMDR_REG->REG_CMDR_CONFIGO_REG0 = 0x00EB;
//	CMDR_REG->REG_CMDR_CONFIGO_REG1 = 0x0100|cmd;
//	CMDR_REG->REG_CMDR_CONFIGO_REG2 = 0x0008;
//	CMDR_REG->REG_CMDR_CONFIGO_REG3 = 0x0000;
//
//	//DSP通过CMDR寄存器（REG_CMDR_PUL0）向Koala发送中断
//	CMDR_REG->REG_CMDR_PUL0 |= 0x01;
//	while(!(CMDR_REG->REG_CMDR_SR_PENDING & 0x01));//等待fpga端的1号中断
//	CMDR_REG->REG_CMDR_SR_PENDING |= 0x01;//清除中断标志位
}

