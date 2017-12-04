/******************************************************************************
 * Copyright (c) 2010-2011 Texas Instruments Incorporated - http://www.ti.com
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *****************************************************************************/
/*
 *  Linker command file
 *
 */

SECTIONS
{

	/**************矢量算法库所用段*******************/
	.racovec: >MSMCSRAM
	{
			*.* (.text:Raco_Vec_C6678_Text) {START(Raco_Vec_C6678_Start) SIZE(Raco_Vec_C6678_Size)}
	}

//	.test > L2SRAM
//	{
//		*(.fardata:ti_sysbios_knl_Task_Object__table__V)
//	}
	.Raco_Vec_C6678_L2	load >> L2SRAM
	.Raco_Vec_C6678_L3	load >> MSMCSRAM

	/**************驱动所用段*******************/
    GROUP  //必须放在MSMCSRAM
    {
    	.qmssSharedMem
      	.cppiSharedMem
    	.i2ceeprom         //i2ceeprom段必须放在MSMCSRAM
    	.emif16nandflash 	//emif16nandflash段必须放在MSMCSRAM
    	.srioSharedMem    	//srio放在msmc上的代码
    	.ChipIntc           //片级中断控制器的段
    	.Sharemem           //edma3多核测试main.c程序中的段
    	.timerSharedMem      //timer多核测试main.c程序中的段
    	.GPIO
    	.csl_vect
    } > MSMCSRAM
    GROUP //必须放在L2上
    {
    	.srioL2Mem	//srio放在L2上的代码
    	.qmssL2Mem          //qmss放在L2上的代码
    	.cppiL2Mem          //cppi放在L2上的代码
    	.CoreIntcL2Mem		//coreintc放在L2上的代码
    	.edma3				//edma3放在L2上的代码
    	.VlfftSyn
    } > L2SRAM

	/***************用户自定义段**********************/
	.L2:  > L2SRAM
	.L3   > MSMCSRAM
	.DDR  > DDR3

}
