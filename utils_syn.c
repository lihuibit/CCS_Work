/*
 * utils.c
 *
 *  Created on: 2016-7-18
 *      Author: Administrator
 */
#include "BD_Bistatic_SAR.h"

#pragma SET_DATA_SECTION(".L2")
volatile unsigned short LocalSynFlagx[8]={0,0,0,0,0,0,0,0};

volatile unsigned short * pFlagFromCore0;
volatile unsigned short * pFlagToCore0x;
#pragma SET_DATA_SECTION()

/*******************************************************
 * 函数说明：八核同步函数，在各个核的L2上开辟一段空间，每个核执行到该位置都会在主核分配的空间的指定位置写一个标志位，主核循环检测是不是每个核都执行到该位置，
 * 		     如果是则在其他的L2上写一个标志，其他核检测到这个标志开始往后继续执行。
 *******************************************************/
void mulcoresyn()
{
	unsigned int sum=0;
	/*******************************************************************
	 * 比较消耗较多的cycle
	 *******************************************************************/
//	(pFlagToCore0x[0]==pFlagToCore0x[1])&&
//	(pFlagToCore0x[0]==pFlagToCore0x[2])&&
//	(pFlagToCore0x[0]==pFlagToCore0x[3])&&
//	(pFlagToCore0x[0]==pFlagToCore0x[4])&&
//	(pFlagToCore0x[0]==pFlagToCore0x[5])&&
//	(pFlagToCore0x[0]==pFlagToCore0x[6])&&
//	(pFlagToCore0x[0]==pFlagToCore0x[7]))
	//141 cycle
//	if(RankMarker.OneOrAllCore==0)
	{
		pFlagToCore0x=(unsigned short * )(((unsigned int)&LocalSynFlagx[0]|0x10000000)+DNUM*2);
		(*pFlagToCore0x)++;
		if(DNUM==0)
		{
			while(1)
			{
				//判定条件：53 cycle
				sum = pFlagToCore0x[0]+pFlagToCore0x[1]+pFlagToCore0x[2]+pFlagToCore0x[3]+
					  pFlagToCore0x[4]+pFlagToCore0x[5]+pFlagToCore0x[6]+pFlagToCore0x[7];
				if(pFlagToCore0x[0]==(sum>>3))
				{
					//core0 写7次：53cycle
					*(unsigned short*)((unsigned int)LocalSynFlagx|0x11000000)=1;
					*(unsigned short*)((unsigned int)LocalSynFlagx|0x12000000)=1;
					*(unsigned short*)((unsigned int)LocalSynFlagx|0x13000000)=1;
					*(unsigned short*)((unsigned int)LocalSynFlagx|0x14000000)=1;
					*(unsigned short*)((unsigned int)LocalSynFlagx|0x15000000)=1;
					*(unsigned short*)((unsigned int)LocalSynFlagx|0x16000000)=1;
					*(unsigned short*)((unsigned int)LocalSynFlagx|0x17000000)=1;

					break;
				}
				else;
			}
		}
		else
		{
			while(LocalSynFlagx[0]==0)
			{
				//do nothing
			}
			LocalSynFlagx[0]=0;
		}
	}
//	else;
}
