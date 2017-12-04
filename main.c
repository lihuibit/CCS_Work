/*
 *  ======== main.c ========
 */

#include "BD_Bistatic_SAR.h"



void resetfunc()
{
}

void myfirstfunc()
{
	mulcoresyn();
}

void mylastfunc()
{
}


void main()
{
	int a=0;
	mulcoresyn();
	DSPInit();//DSP初始化
	mulcoresyn();
	a=Raco_Vec_C6678_Get_Licence(0x11111111);//匹配矢量算法库的lic，该lic与芯片有关
	while(a==0)
	{
		a=Raco_Vec_C6678_Get_Licence(0x11111111);
	}
	BIOS_start();     /* enable interrupts and start SYS/BIOS */
}





















