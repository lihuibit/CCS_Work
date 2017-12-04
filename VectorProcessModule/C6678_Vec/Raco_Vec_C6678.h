#ifndef RACO_Vec_C6678_H_
#define RACO_Vec_C6678_H_

/****************************************
符号定义:
单精度\双精度\整形+实数\复数+常数\序列\矩阵

单精度浮点:F
双精度浮点:D
32位有\无符号整形:I32\U32
16位有\无符号整形:I16\U16
8位有\无符号整形:I8\U8

实数:R
复数:C

常数:C
序列:S
矩阵:M

例:
单精度浮点复数序列:FCS
16位无符号整形实数矩阵:U16RM
****************************************/



/**********************FFT Start********************/
typedef enum {
	VLFFT_2k	= 11,
	VLFFT_4k	= 12,
	VLFFT_8k	= 13,
	VLFFT_16k	= 14,
	VLFFT_32k	= 15,
	VLFFT_64k	= 16,
	VLFFT_128k	= 17,
	VLFFT_256k	= 18,
	VLFFT_512k	= 19,
	VLFFT_1024k	= 20
}VLFFT_NUM;

typedef struct
{
	unsigned int CC[8];
	unsigned int Chan_in[8];
	unsigned int Para_in[8];
	unsigned int TCNum[8];
	unsigned int RegionNum[8];
}VLFFT_EDMA_Channel;			//EDMA参数初始化,存放控制器、通道号、参数号

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_TwGen(float*fpOut, int N, int Mode)
 *	函数功能:		在指定地址上生成FFT或IFFT使用的旋转因子
 *	算术公式表达:	if(Mode==0)			Out=twiddle(N);
 *				else if(Mode==1)	Out=itwiddle(N);
 *	参数说明:		float*fpOut		:输出旋转因子起始地址,8B对齐
 *				int N			:处理点数N,8<=N<=131072,且N为2的整数次幂
 *				int Mode		:Mode==0,生成FFT运算所需的旋转因子twiddle
 *								:Mode==1,生成IFFT运算所需的旋转因子itwiddle
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		进行FFT或IFFT前,需要调用此函数来生成运算所需的旋转因子;
 ***************************************************************************************/
extern void Raco_Vec_C6678_TwGen(float*fpOut, int N, int Mode);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_HiGen(float*fpOut, VLFFT_POINTNUM N, int N2_PerCore, int CoreNum, int Mode)
 *	函数功能:		在指定地址上生成FFT或IFFT使用的铰链因子
 *	算术公式表达:	if(Mode==0)			Out=hinge(N2_PerCore);
 *				else if(Mode==1)	Out=ihinge(N2_PerCore);
 *	参数说明:		float*fpOut		:输出铰链因子起始地址,8B对齐
 *				VLFFT_NUM N_Log2:VLFFT或VLIFFT的处理总点数N,取以2为底的对数
 *				int N2_PerCore	:N2_PerCore=N2/参与计算的核数
 *				int Mode		:Mode==0,生成FFT运算所需的铰链因子hinge
 *								:Mode==1,生成IFFT运算所需的铰链因子ihinge
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		进行大点数FFT或IFFT处理前,需要调用此函数来生成运算所需的铰链因子;
 *				生成的铰链因子长度为N2_PerCore*8B;
 *				由于每个核生成的铰链因子都不一样,因此要求各核的铰链因子地址不能有重叠;
 *				推荐各核使用自身的L2独立空间;
 *				不参与大点数FFT及IFFT运算的核不需要计算此因子;
 ***************************************************************************************/
extern void Raco_Vec_C6678_HiGen(float*fpOut, VLFFT_NUM N_Log2, int N2_PerCore, int Mode);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_fft(float*fpIn, float*fpOut, int N, float*Twiddle)
 *	函数功能:		对输入单精度复数序列进行FFT处理
 *	算术公式表达:	Out[i]=fft(In[i]), i=0...N-1;
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,8B对齐
 *				float*fpOut		:输出单精度复数序列起始地址,8B对齐
 *				int N			:处理点数N,8<=N<=131072,且N为2的整数次幂
 *				float*Twiddle	:输入FFT旋转因子起始地址,8B对齐
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		在调用此函数前,Twiddle应该已经被计算完成;
 *				由于在运算过程中,fpIn会被复用,所以输入序列将会被破坏;
 *				如果需要保留输入数据,应该在执行此函数前进行备份;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_fft(float*fpIn, float*fpOut, int N, float*Twiddle);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_ifft(float*fpIn, float*fpOut, int N, float*iTwiddle)
 *	函数功能:		对输入单精度复数序列进行IFFT处理
 *	算术公式表达:	Out[i]=ifft(In[i]),i=0...N-1;
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,8B对齐
 *				float*fpOut		:输出单精度复数序列起始地址,8B对齐
 *				int N			:处理点数N,8<=N<=131072,且N为2的整数次幂
 *				float*itwiddle	:输入IFFT旋转因子起始地址,8B对齐
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		在调用此函数前,iTwiddle应该已经被计算完成;
 *				由于在运算过程中,fpIn会被复用,所以输入序列将会被破坏;
 *				如果需要保留输入数据,应该在执行此函数前进行备份;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_ifft(float*fpIn, float*fpOut, int N, float*iTwiddle);

/***************************************************************************************
 *	函数名:		int Raco_Vec_C6678_VLFFT_EDMA_Init(VLFFT_EDMA_Channel Param, unsigned int CoreInfo)
 *	函数功能: 		大点数FFT的EDMA初始化
 *	算术公式表达:	无
 *	参数说明:		VLFFT_EDMA_Channel Param	:EDMA初始化信息
 *				unsigned int CoreInfo		:初始化核选择
 *	调用核:		所有核均可自行调用
 *	返回值: 		0				:初始化失败
 *				1				:初始化成功
 *	备注说明:		只有参与大点数FFT运算的核需要初始化此函数,不参与运算的核此函数会直接跳出,并返回1;
 *				参与初始化的核的信息由CoreInfo给出,对应如下:
 *					core7	core6	core5	core4	core3	core2	core1	core0
 *				0x	0		0		0		0		0		0		0		0
 *				如果该核参与初始化,则将该核对应位置1;如果该核不参与初始化,则将该核对应位置0;
 *				例如:
 *				八核初始化			:CoreInfo=0x11111111
 *				0、2、4、6核初始化	:CoreInfo=0x01010101
 *				1、3、4核初始化		:CoreInfo=0x00011010
 ***************************************************************************************/
extern int Raco_Vec_C6678_VLFFT_EDMA_Init(VLFFT_EDMA_Channel EDMA_Param, unsigned int CoreInfo);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_VLFFT_HomePos(
					float*				fpInOut,
					VLFFT_NUM			N_Log2,
					VLFFT_EDMA_Channel	EDMA_Param,
					float*				Twiddle_Col,
					float*				Twiddle_Row,
					float*				Hinge,
					float*				Buffer_1,
					float*				Buffer_2,
					unsigned int		Core_Info,
					int					Mode
				);
 *	函数功能: 		多核并行大点数FFT或IFFT计算,原位计算
 *	算术公式表达:	if(Mode==0)			Out[i]=fft(In), i=0...i-1;
 *				else if(Mode==1)	Out[i]=ifft(In), i=0...i-1;
 *	参数说明:		float*		fpInOut				:输入单精度复数序列及输出复数序列起始地址,8字节对齐
				VLFFT_NUM	N_Log2				:处理点数N,取以2为底的对数
				VLFFT_EDMA_Channel EDMA_Param	:各核EDMA通道的参数,要求之前已初始化过EDMA
				float*		Twiddle_Col			:列向旋转因子,生成点数为N1,要求之前已初始化过列向旋转因子,推荐放在L2上
				float*		Twiddle_Row			:行向旋转因子,生成点数为N2,要求之前已初始化过行向旋转因子,推荐放在L2上
				float*		Hinge				:铰链因子,生成点数为N2/处理总核数,要求之前已初始化过铰链因子,推荐放在L2上
				float*		Buffer_1			:各核计算临时缓存1,占用大小为max(8*N/处理总核数,256*N1)字节,各个核之间的缓存空间不能重叠,推荐放在L2上
				float*		Buffer_2			:各核计算临时缓存2,占用大小为128*N2字节,各个核间的缓存空间不能重叠,推荐放在L2上
				int			Core_Info			:参与处理的核的信息
				int			Mode				:计算模式,Mode==0,进行VLFFT运算,Mode==1,进行VLIFFT运算
 *	调用核:		需联合进行大点数FFT或IFFT处理的核调用此函数,其他核不需调用
 *	返回值: 		void
 *	备注说明:		原位计算占用内存较小,但效率要低于非原位计算;
 *				非原位计算占用内存较大,但效率要高于原位计算;
 *				在执行此函数之前,需确保以下操作已经完成:
 *				各核的EDMA通道已被初始化完成;
 *				列向旋转因子(各核相同)、行向旋转因子(各核相同)、铰链因子(各核不同)已初始化完成;
 *				各核的Buffer_1、Buffer_2缓存已被开辟完成;
 *				N、N1、N2之间的关系为:N=N1*N2
 *				N1、N2的的计算方式为:
 *				if(N%2 == 0)
 *				{
 *					N1 = 1<<(N/2);
 *					N2 = 1<<(N/2);
 *				}
 *				else if(N%2 == 1)
 *				{
 *					N1 = 1<<(N/2+1);
 *					N2 = 1<<(N/2);
 *				}
 *				2k:N1==64 N2==32;		4k:N1==64 N2==64;		8k:N1==128 N2==64;
 *				16k:N1==128 N2==128;	32k:N1==256 N2==128;	64k:N1==256 N2==256;
 *				128k:N1==512 N2==256;	256k:N1==512 N2==512;	512K:N1==1024 N2==512;
 *				1024K:N1==1024 N2==1024;
 *				参与处理的核的信息由CoreInfo给出,对应如下:
 *					core7	core6	core5	core4	core3	core2	core1	core0
 *				0x	0		0		0		0		0		0		0		0
 *				如果该核参与处理,则将该核对应位置1;如果该核不参与处理,则将该核对应位置0;
 *				例如:
 *				八核处理			:CoreInfo=0x11111111
 *				0、1核处理			:CoreInfo=0x00000011
 *				4、5、6、7核处理	:CoreInfo=0x11110000
 *				该算法目前支持2核、4核、8核的并行计算,但对并行核的组合有一些限制,目前支持的组合如下:
 *				2核并行:01/23/45/67
 *				4核并行:0123/4567
 *				8核并行:01234567
 ***************************************************************************************/
void Raco_Vec_C6678_VLFFT_HomePos(
	float*				fpInOut,
	VLFFT_NUM			N_Log2,
	VLFFT_EDMA_Channel	EDMA_Param,
	float*				Twiddle_Col,
	float*				Twiddle_Row,
	float*				Hinge,
	float*				Buffer_1,
	float*				Buffer_2,
	unsigned int		Core_Info,
	int					Mode
);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_VLFFT_NotHomePos(
					float*				fpInOut,
					VLFFT_NUM			N_Log2,
					VLFFT_EDMA_Channel	EDMA_Param,
					float*				Twiddle_Col,
					float*				Twiddle_Row,
					float*				Hinge,
					float*				Buffer_1,
					float*				Buffer_2,
					int					Core_Info,
					int					Mode
				);
 *	函数功能: 		多核并行大点数FFT或IFFT计算,非原位计算
 *	算术公式表达:	if(Mode==0)			Out[i]=fft(In), i=0...i-1;
 *				else if(Mode==1)	Out[i]=ifft(In), i=0...i-1;
 *	参数说明:		float*		fpInOut				:输入单精度复数序列及输出单精度复数序列起始地址,8字节对齐
				VLFFT_NUM	N_Log2				:处理点数N,取以2为底的对数
				VLFFT_EDMA_Channel EDMA_Param	:各核EDMA通道的参数,要求之前已初始化过EDMA
				float*		Twiddle_Col			:列向旋转因子,生成点数为N1,要求之前已初始化过列向旋转因子,推荐放在L2上
				float*		Twiddle_Row			:行向旋转因子,生成点数为N2,要求之前已初始化过行向旋转因子,推荐放在L2上
				float*		Hinge				:铰链因子,生成点数为N2/处理总核数,要求之前已初始化过铰链因子,推荐放在L2上
				float*		Buffer_1			:各核计算临时缓存1,占用大小为256*N1字节,各个核之间的缓存空间不能重叠,推荐放在L2上
				float*		Buffer_2			:各核计算临时缓存2,占用大小为输入总数据大小,需放在共享内存MSMC上
				int			Core_Info			:参与处理的核的信息
				int 		Mode				:计算模式,Mode==0,进行VLFFT运算,Mode==1,进行VLIFFT运算
 *	调用核:		需联合进行大点数FFT或IFFT处理的核调用此函数,其他核不需调用
 *	返回值: 		void
 *	备注说明:		原位计算占用内存较小,但效率要低于非原位计算;
 *				非原位计算占用内存较大,但效率要高于原位计算;
 *				在执行此函数之前,需确保以下操作已经完成:
 *				各核的EDMA通道已被初始化完成;
 *				列向旋转因子(各核相同)、行向旋转因子(各核相同)、铰链因子(各核不同)已初始化完成;
 *				各核的Buffer_1、Buffer_2缓存已被开辟完成;
 *				N、N1、N2之间的关系为:N=N1*N2
 *				N1、N2的的计算方式为:
 *				if(N%2 == 0)
 *				{
 *					N1 = 1<<(N/2);
 *					N2 = 1<<(N/2);
 *				}
 *				else if(N%2 == 1)
 *				{
 *					N1 = 1<<(N/2+1);
 *					N2 = 1<<(N/2);
 *				}
 *				2k:N1==64 N2==32;		4k:N1==64 N2==64;		8k:N1==128 N2==64;
 *				16k:N1==128 N2==128;	32k:N1==256 N2==128;	64k:N1==256 N2==256;
 *				128k:N1==512 N2==256;	256k:N1==512 N2==512;	512K:N1==1024 N2==512;
 *				1024K:N1==1024 N2==1024;
 *				参与处理的核的信息由CoreInfo给出,对应如下:
 *					core7	core6	core5	core4	core3	core2	core1	core0
 *				0x	0		0		0		0		0		0		0		0
 *				如果该核参与处理,则将该核对应位置1;如果该核不参与处理,则将该核对应位置0;
 *				例如:
 *				八核处理			:CoreInfo=0x11111111
 *				0、1核处理			:CoreInfo=0x00000011
 *				4、5、6、7核处理	:CoreInfo=0x11110000
 *				该算法目前支持2核、4核、8核的并行计算,但对并行核的组合有一些限制,目前支持的组合如下:
 *				2核并行:01/23/45/67
 *				4核并行:0123/4567
 *				8核并行:01234567
 ***************************************************************************************/
extern void Raco_Vec_C6678_VLFFT_NotHomePos(
	float*				fpInOut,
	VLFFT_NUM			N_Log2,
	VLFFT_EDMA_Channel	EDMA_Param,
	float*				Twiddle_Col,
	float*				Twiddle_Row,
	float*				Hinge,
	float*				Buffer_1,
	float*				Buffer_2,
	int					Core_Info,
	int					Mode
);
/**********************FFT   End**********************/


/**********************Vector Start********************/

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Add_FCS(float*fpIn1, float*fpIn2, float*fpOut, int N)
  *	函数功能: 		单精度复数序列加单精度复数序列
 *	算术公式表达:	Out[i].re=In1[i].re+In2[i].re, i=0...N-1
 *				Out[i].im=In1[i].im+In2[i].im, i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度复数序列1起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度复数序列2起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Add_FCS(float*fpIn1, float*fpIn2, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Sub_FCS(float*fpIn1, float*fpIn2, float*fpOut, int N)
 *	函数功能: 		单精度复数序列减单精度复数序列
 *	算术公式表达:	Out[i].re=In1[i].re-In2[i].re, i=0...N-1
 *				Out[i].im=In1[i].im-In2[i].im, i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度复数序列1起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度复数序列2起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Sub_FCS(float*fpIn1, float*fpIn2, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Mul_FCS(float*fpIn1, float*fpIn2, float*fpOut, int N)
 *	函数功能: 		单精度复数序列乘单精度复数序列
 *	算术公式表达:	Out[i].re=In1[i].re*In2[i].re-In1[i].im*In2[i].im, i=0...N-1
 *				Out[i].im=In1[i].re*In2[i].im+In1[i].im*In2[i].re, i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度复数序列1起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度复数序列2起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Mul_FCS(float*fpIn1, float*fpIn2, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Mul_FCS_Stride(float*fpIn1, float*fpIn2, float*fpOut, int N, unsigned int Stride_In1, unsigned int Stride_In2, unsigned int Stride_Out)
 *	函数功能: 		单精度复数序列乘单精度复数序列,跳变相乘
 *	算术公式表达:	Out[i*Stride_Out].re=In1[i*Stride_In1].re*In2[i*Stride_In2].re-In1[i*Stride_In1].im*In2[i*Stride_In2].im, i=0...N-1
 *				Out[i*Stride_Out].im=In1[i*Stride_In1].re*In2[i*Stride_In2].im+In1[i*Stride_In1].im*In2[i*Stride_In2].re, i=0...N-1
 *	参数说明:		float*fpIn1					:输入单精度复数序列1起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2					:输入单精度复数序列2起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut					:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	int N						:处理点数N,N>0
 *		    	unsigned int Stride_In1		:输入单精度复数序列1每次跳变的复数点数
 *		    	unsigned int Stride_In2		:输入单精度复数序列2每次跳变的复数点数
 *		    	unsigned int Stride_Out		:输出单精度复数序列每次跳变的复数点数
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Mul_FCS_Stride(float*fpIn1, float*fpIn2, float*fpOut, int N, unsigned int Stride_In1, unsigned int Stride_In2, unsigned int Stride_Out);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Mul_FCS_Sum(float*fpIn1, float*fpIn2, float*fpOut, int N)
 *	函数功能: 		单精度复数序列与单精度复数序列相乘并累加
 *	算术公式表达:	Out[0].re+=In1[i].re*In2[i].re-In1[i].im*In2[i].im, i=0...N-1
 *				Out[0].im+=In1[i].re*In2[i].im+In1[i].im*In2[i].re, i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度复数序列1起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度复数序列2起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数起始地址,实部在前,虚部在后,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Mul_FCS_Sum(float*fpIn1, float*fpIn2, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Mul_FCS_Stride_Sum(float*fpIn1, float*fpIn2, float*fpOut, int N, unsigned int Stride_In1, unsigned int Stride_In2)
 *	函数功能: 		单精度复数序列与单精度复数序列跳变相乘并累加
 *	算术公式表达:	Out[0].re+=In1[i*Stride_In1].re*In2[i*Stride_In2].re-In1[i*Stride_In1].im*In2[i*Stride_In2].im, i=0...N-1
 *				Out[0].im+=In1[i*Stride_In1].re*In2[i*Stride_In2].im+In1[i*Stride_In1].im*In2[i*Stride_In2].re, i=0...N-1
 *	参数说明:		float*fpIn1					:输入单精度复数序列1起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2					:输入单精度复数序列2起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut					:输出单精度复数起始地址,实部在前,虚部在后,8B对齐
 *		    	int N						:处理点数N,N>0
 *		    	unsigned int Stride_In1		:输入单精度复数序列1每次跳变的复数点数
 *		    	unsigned int Stride_In2		:输入单精度复数序列2每次跳变的复数点数
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Mul_FCS_Stride_Sum(float*fpIn1, float*fpIn2, float*fpOut, int N, unsigned int Stride_In1, unsigned int Stride_In2);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Div_FCS(float*fpIn1, float*fpIn2, float*fpOut, int N)
 *	函数功能: 		单精度复数序列除以单精度复数序列
 *	算术公式表达:	temp=1/(In2[i].re*In2[i].re+In2[i].im*In2[i].im), i=0...N-1
 *				Out[i].re=(In1[i].re*In2[i].re+In1[i].im*In2[i].im)*temp, i=0...N-1
 *				Out[i].im=(In1[i].im*In2[i].re-In1[i].re*In2[i].im)*temp, i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度复数序列1起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度复数序列2起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Div_FCS(float*fpIn1, float*fpIn2, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Add_FRS(float*fpIn1, float*fpIn2, float*fpOut, int N)
 *	函数功能: 		单精度复数序列加单精度实数序列
 *	算术公式表达:	Out[i].re=In1[i].re+In2[i], i=0...N-1
 *				Out[i].im=In1[i].im, i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度实数序列起始地址,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Add_FRS(float*fpIn1, float*fpIn2, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Sub_FRS(float*fpIn1, float*fpIn2, float*fpOut, int N)
 *	函数功能: 		单精度复数序列减单精度实数序列
 *	算术公式表达:	Out[i].re=In1[i].re-In2[i], i=0...N-1
 *				Out[i].im=In1[i].im, i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度实数序列起始地址,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Sub_FRS(float*fpIn1, float*fpIn2, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Mul_FRS(float*fpIn1, float*fpIn2, float*fpOut, int N)
 *	函数功能: 		单精度复数序列乘单精度实数序列
 *	算术公式表达:	Out[i].re=In1[i].re*In2[i], i=0...N-1
 *				Out[i].im=In1[i].im*In2[i], i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度实数序列起始地址,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Mul_FRS(float*fpIn1, float*fpIn2, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Add_FRC(float*fpIn, float*fpOut, float fVal, int N)
 *	函数功能: 		单精度复数序列加单精度实常数
 *	算术公式表达:	Out[i].re=In[i].re+fVal, i=0...N-1
 *				Out[i].im=In[i].im, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float fVal		:与单精度复数序列相加的单精度实常数
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Add_FRC(float*fpIn, float*fpOut, float fVal, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Sub_FRC(float*fpIn, float*fpOut, float fVal, int N)
 *	函数功能: 		复数序列减实常数
 *	算术公式表达:	Out[i].re=In[i].re-fVal, i=0...N-1
 *				Out[i].im=In[i].im, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float fVal		:与单精度复数序列相减的单精度实常数
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Sub_FRC(float*fpIn, float*fpOut, float fVal, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Mul_FRC(float*fpIn, float*fpOut, float fVal, int N)
 *	函数功能: 		单精度复数序列乘单精度实常数
 *	算术公式表达:	Out[i].re=In[i].re*fVal, i=0...N-1
 *				Out[i].im=In[i].im*fVal, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float fVal		:与单精度复数序列相乘的单精度实常数
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Mul_FRC(float*fpIn, float*fpOut, float fVal, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Add_FCC(float*fpIn, float*fpOut, float fVal1, float fVal2, int N)
 *	函数功能: 		单精度复数序列加单精度复常数
 *	算术公式表达:	Out[i].re=In[i].re+fVal1, i=0...N-1
 *				Out[i].im=In[i].im+fVal2, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float fVal1		:与单精度复数序列相加的单精度复常数的实部
 *		    	float fVal2		:与单精度复数序列相加的单精度复常数的虚部
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Add_FCC(float*fpIn, float*fpOut, float fVal1, float fVal2, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Sub_FCC(float*fpIn, float*fpOut, float fVal1, float fVal2, int N)
 *	函数功能: 		单精度复数序列减单精度复常数
 *	算术公式表达:	Out[i].re=In[i].re-fVal1, i=0...N-1
 *				Out[i].im=In[i].im-fVal2, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float fVal1		:单精度复常数的实部
 *		    	float fVal2		:单精度复常数的虚部
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Sub_FCC(float*fpIn, float*fpOut, float fVal1, float fVal2, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Mul_FCC(float*fpIn, float*fpOut, float fVal1, float fVal2, int N)
 *	函数功能: 		单精度复数序列乘单精度复常数
 *	算术公式表达:	Out[i].re=In1[i].re*fVal1-In1[i].im*fVal2, i=0...N-1
 *				Out[i].im=In1[i].re*fVal2+In1[i].im*fVal1, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float fVal1		:单精度复常数的实部
 *		    	float fVal2		:单精度复常数的虚部
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Mul_FCC(float*fpIn, float*fpOut, float fVal1, float fVal2, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Conj(float*fpIn, float*fpOut, int N)
 *	函数功能: 		单精度复数序列共轭运算
 *	算术公式表达:	Out[i].re=In[i].re, i=0...N-1
 *				Out[i].im=-In[i].im, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Conj(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Conj_Mul_FCS(float*fpIn1, float*fpIn2, float*fpOut, int N)
 *	函数功能: 		单精度复数序列共轭乘单精度复数序列
 *	算术公式表达:	Out[i].re=In1[i].re*In2[i].re+In1[i].im*In2[i].im, i=0...N-1
 *				Out[i].im=In1[i].re*In2[i].im-In2[i].re*In1[i].im, i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度复数序列1起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度复数序列2起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:	 	对输入单精度复数序列1求共轭后,与输入单精度复数序列2点乘;
 *				输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Conj_Mul_FCS(float*fpIn1, float*fpIn2, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Power2(float*fpIn, float*fpOut, int N)
 *	函数功能: 		单精度复数序列平方
 *	算术公式表达:	Out[i]=In[i].re*In[i].re+In[i].im*In[i].im, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度实数序列起始地址,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Power2(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Splt(float*fpIn, float*fpOut1, float*fpOut2, int N)
 *	函数功能:		实现单精度复数序列奇偶拆分
 *	算术公式表达:	Out1[i].re=In[i*2].re, i=0...N-1
 *				Out1[i].im=In[i*2].im, i=0...N-1
 *				Out2[i].re=In[i*2+1].re, i=0...N-1
 *				Out2[i].im=In[i*2+1].im, i=0...N-1
 *	参数说明:		float*fpIn			:输入合并单精度复数序列起始地址,先偶后奇,8B对齐
 *				float*fpOut1		:输出偶数单精度复数序列起始地址,8B对齐
 *				float*fpOut2		:输出奇数单精度复数序列起始地址,8B对齐
 *				int N				:奇数序列或偶数序列的长度N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		奇数序列和偶数序列长度相等
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Splt(float*fpIn, float*fpOut1, float*fpOut2, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Comb(float*fpIn1, float*fpIn2, float*fpOut, int N)
 *	函数功能:		实现单精度复数序列奇偶合并
 *	算术公式表达:	Out[i*2].re=In1[i].re, i=0...N-1
 *				Out[i*2].im=In1[i].im, i=0...N-1
 *				Out[i*2+1].re=In2[i].re, i=0...N-1
 *				Out[i*2+1].im=In2[i].im, i=0...N-1
 *	参数说明:		float*fpIn1			:输入偶数单精度复数序列起始地址,8B对齐
 *				float*fpIn2			:输入奇数单精度复数序列起始地址,8B对齐
 *				float*fpOut			:输出合并单精度复数序列起始地址,先偶后奇,8B对齐
 *				int N				:奇数序列或偶数序列的长度N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		奇数序列和偶数序列长度相等
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Comb(float*fpIn1, float*fpIn2, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Sum(float*fpIn, float*fpOut, int N)
 *	函数功能: 		单精度复数序列求和
 *	算术公式表达:	Out.re=In[0].re+...+In[N-1].re
 *				Out.im=In[0].im+...+In[N-1].im
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *				float*fpOut		:输出单精度复数地址,实部在前,虚部在后,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值:		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Sum(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_Mod(float*fpIn, float*fpOut, int N)
 *	函数功能: 		单精度复数序列求模
 *	算术公式表达:	Out[i]=sqrt(In[i].re*In[i].re+In[i].im*In[i].im), i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度实数序列起始地址,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_Mod(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Add_FRC(float*fpIn, float*fpOut, float fVal, int N)
 *	函数功能:		单精度实数序列加单精度实常数
 *	算术公式表达:	Out[i]=In[i]+fVal, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				float fVal		:与单精度实数序列相加的单精度实常数
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输入序列、输出序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Add_FRC(float*fpIn, float*fpOut, float fVal, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Sub_FRC(float*fpIn, float*fpOut, float fVal, int N)
 *	函数功能:		单精度实数序列减单精度实常数
 *	算术公式表达:	Out[i]=In[i]+fVal, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				float fVal		:与单精度实数序列相减的单精度实常数
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输入序列、输出序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Sub_FRC(float*fpIn, float*fpOut, float fVal, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Mul_FRC(float*fpIn, float*fpOut, float fVal, int N)
 *	函数功能:		单精度实数序列乘单精度实常数
 *	算术公式表达:	Out[i]=In[i]+fVal, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				float fVal		:与单精度实数序列相乘的单精度实常数
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输入序列、输出序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Mul_FRC(float*fpIn, float*fpOut, float fVal, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Sqrt(float*fpIn, float*fpOut, int N)
 *	函数功能:		单精度实数序列求开方
 *	算术公式表达:	Out[i]=sqrtf(In[i]), i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *				float*fpOut		:输出单精度实数列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		如果输入值为负数,那么输出序列中对应位置会输出-0.0;
 *				输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 *				输入序列、输出序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Sqrt(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Abs(float*fpIn, float*fpOut, int N)
 *	函数功能:		单精度实数序列求绝对值
 *	算术公式表达:	Out[i]=absf(In[i]), i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输入序列、输出序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Abs(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Sum(float*fpIn, float*fpSum, int N)
 *	函数功能:		单精度实数序列求和
 *	算术公式表达:	Out=In[0]+...+In[N-1]
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *				float*fpSum		:输出单精度实数存放地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Sum(float*fpIn, float*fpSum, int N);

/***************************************************************************************
 *	函数名:		float Raco_Vec_C6678_FRS_Ave(float*fpIn, float*fpAve, int N)
 *	函数功能: 		单精度实数序列求均值
 *	算术公式表达:	Out=(In[0]+...+In[N-1])/N
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *				float*fpAve		:输出单精度实数存放地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern float Raco_Vec_C6678_FRS_Ave(float*fpIn, float*fpAve, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Min(float*fpIn, float*fpMinVal, int N)
 *	函数功能: 		单精度实数序列求最小值
 *	算术公式表达:	MinVal=min(In[0]...In[N-1])
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *		    	float*fpMinVal	:输出单精度实数存放地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Min(float*fpIn, float*fpMinVal, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Max(float*fpIn, float*fpMaxVal, int N)
 *	函数功能: 		单精度实数序列求最大值
 *	算术公式表达:	MaxVal=max(In[0]...In[N-1])
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *		    	float*fpMaxVal	:输出单精度实数存放地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Max(float*fpIn, float*fpMaxVal, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Min_Max(float*fpIn, float*fpMinVal, float*fpMaxVal, int N);
 *	函数功能: 		浮点数序列求最小值和最大值
 *	算术公式表达:	MinVal=min(In[0]...In[N-1])
				MaxVal=max(In[0]...In[N-1])
 *	参数说明:		float*fpIn		:表示输入浮点数序列首地址,4B对齐
 *				float*fpMinVal	:输出最小值存放地址,4B对齐
 *				float*fpMaxVal	:输出最大值存放地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Min_Max(float*fpIn, float*fpMinVal, float*fpMaxVal, int N);

/***************************************************************************************
 *	函数名:		int Raco_Vec_C6678_FRS_Min_Num(float*fpIn, int N, float*MinVal)
 *	函数功能: 		单精度实数序列求最小值,并返回最小值所在序列中的号数
 *	算术公式表达:	MinVal=min(In[0]...In[N-1])
 *				In[Return]=min(In[0]...In[N-1])
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *		    	float*MinVal	:输出单精度实数序列最小值存放地址,4B对齐
 *	调用核:		所有核均可自行调用
 *	返回值: 		int				:序列中最小值的位置
 *	备注说明:		函数的返回值表示最小值在序列中的序号(0...N-1)
 *				如果单精度实数序列中存在多个最小值,那么函数会返回序列号最小的最小值的序列号
 ***************************************************************************************/
extern int Raco_Vec_C6678_FRS_Min_Num(float*fpIn, int N, float*MinVal);

/***************************************************************************************
 *	函数名:		int Raco_Vec_C6678_FRS_Max_Num(float*fpIn, int N, float*, float*fpMaxVal)
 *	函数功能: 		单精度实数序列求最大值,并返回最大值所在序列中的号数
 *	算术公式表达:	MaxVal=max(In[0]...In[N-1])
 *				In[Return]=max(In[0]...In[N-1])
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *		    	float*fpMaxVal	输出单精度实数序列最大值存放地址,4B对齐
 *	调用核:		所有核均可自行调用
 *	返回值: 		int				:序列中最大值的位置
 *	备注说明:		函数的返回值表示最大值在序列中的序号(0...N-1)
 *				如果单精度实数序列中存在多个最大值,那么函数会返回序列号最小的最大值的序列号
 ***************************************************************************************/
extern int Raco_Vec_C6678_FRS_Max_Num(float*fpIn, int N, float*fpMaxVal);

/***************************************************************************************
 *	函数名:		float Raco_Vec_C6678_FRS_Mod(float*fpIn, int N)
 *	函数功能: 		单精度实数序列求模
 *	算术公式表达:	Out=sqrt(In[0]*In[0]+...+In[N-1]*In[N-1])
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		float			:单精度实数序列的模
 *	备注说明:		无
 ***************************************************************************************/
extern float Raco_Vec_C6678_FRS_Mod(float*fpIn, int N);

/***************************************************************************************
 *	函数名:		double Raco_Vec_C6678_DRS_Mod(double*dpIn, int N)
 *	函数功能: 		双精度实数序列求模
 *	算术公式表达:	Out=sqrt(In[0]*In[0]+...+In[N-1]*In[N-1])
 *	参数说明:		double*dpIn		:输入双精度实数序列起始地址,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		double			:双精度实数序列的模
 *	备注说明:		无
 ***************************************************************************************/
extern double Raco_Vec_C6678_DRS_Mod(double*dpIn, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Splt(float*fpIn, float*fpOut1, float*fpOut2, int N)
 *	函数功能:		单精度实数序列奇偶拆分
 *	算术公式表达:	Out1[i]=In[i*2], i=0...N-1
 *				Out2[i]=In[i*2+1], i=0...N-1
 *	参数说明:		float*fpIn		:输入合并单精度实数序列起始地址,先偶后奇,8B对齐
 *				float*fpOut1	:输出偶数单精度实数序列起始地址,8B对齐
 *				float*fpOut2	:输出奇数单精度实数序列起始地址,8B对齐
 *				int N			:奇数序列或偶数序列的长度N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		奇数序列和偶数序列长度相等
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Splt(float*fpIn, float*fpOut1, float*fpOut2, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Comb(float*fpIn1, float*fpIn2, float*fpOut, int N)
 *	函数功能:		单精度实数序列奇偶合并
 *	算术公式表达:	Out[i*2]=In1[i], i=0...N-1
 *				Out[i*2+1]=In2[i], i=0...N-1
 *	参数说明:		float*fpIn1		:输入偶数单精度实数序列起始地址,4B对齐
 *				float*fpIn2		:输入奇数单精度实数序列起始地址,4B对齐
 *				float*fpOut		:输出合并单精度实数序列起始地址,先偶后奇,8B对齐
 *				int N			:奇数序列或偶数序列的长度N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		奇数序列和偶数序列长度相等
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Comb(float*fpIn1, float*fpIn2, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Rcp(float*fpIn,float*fpOut,int N)
 *	函数功能:		单精度实数序列求倒数
 *	算术公式表达:	Out[i]=1.0/In[i], i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		如果输入值中包含0,那么输出值对应的位置会变成非法值,其他正常值的计算不会受到影响
 *				输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 *				输入序列、输出序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Rcp(float*fpIn,float*fpOut,int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Add_FRS(float*fpIn1,float*fpIn2,float*fpOut,int N)
 *	函数功能:		单精度实数序列加单精度实数序列
 *	算术公式表达:	Out[i]=In1[i]+In2[i], i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度实数序列1起始地址,4B对齐
 *				float*fpIn2		:输入单精度实数序列2起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 *				输入序列、输出序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Add_FRS(float*fpIn1,float*fpIn2,float*fpOut,int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Sub_FRS(float*fpIn1,float*fpIn2,float*fpOut,int N)
 *	函数功能:		单精度实数序列减单精度实数序列
 *	算术公式表达:	Out[i]=In1[i]-In2[i], i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度实数序列1起始地址,4B对齐
 *				float*fpIn2		:输入单精度实数序列2起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 *				输入序列、输出序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Sub_FRS(float*fpIn1,float*fpIn2,float*fpOut,int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Mul_FRS(float*fpIn1,float*fpIn2,float*fpOut,int N)
 *	函数功能:		单精度实数序列乘单精度实数序列
 *	算术公式表达:	Out[i]=In1[i]*In2[i], i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度实数序列1起始地址,4B对齐
 *				float*fpIn2		:输入单精度实数序列2起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 *				输入序列、输出序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Mul_FRS(float*fpIn1,float*fpIn2,float*fpOut,int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Div_FRS(float*fpIn1,float*fpIn2,float*fpOut,int N)
 *	函数功能:		单精度实数序列除以单精度实数序列
 *	算术公式表达:	Out[i]=In1[i]/In2[i], i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度实数序列1起始地址,4B对齐
 *				float*fpIn2		:输入单精度实数序列2起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		如果输入序列2中包含0,那么输出值对应的位置会变成非法值,其他正常值的计算不会受到影响
 *				输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 *				输入序列、输出序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Div_FRS(float*fpIn1,float*fpIn2,float*fpOut,int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_DRS_Add_DRS(double*fpIn1,double*fpIn2,double*fpOut,int N)
 *	函数功能:		双精度实数序列加双精度实数序列
 *	算术公式表达:	Out[i]=In1[i]+In2[i], i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度实数序列1起始地址,4B对齐
 *				float*fpIn2		:输入单精度实数序列2起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_DRS_Add_DRS(double*fpIn1,double*fpIn2,double*fpOut,int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Sub_FRS(double*fpIn1,double*fpIn2,double*fpOut,int N)
 *	函数功能:		双精度实数序列减双精度实数序列
 *	算术公式表达:	Out[i]=In1[i]-In2[i], i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度实数序列1起始地址,4B对齐
 *				float*fpIn2		:输入单精度实数序列2起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_DRS_Sub_DRS(double*fpIn1,double*fpIn2,double*fpOut,int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Mul_FRS(double*fpIn1,double*fpIn2,double*fpOut,int N)
 *	函数功能:		双精度实数序列乘双精度实数序列
 *	算术公式表达:	Out[i]=In1[i]*In2[i], i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度实数序列1起始地址,4B对齐
 *				float*fpIn2		:输入单精度实数序列2起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_DRS_Mul_DRS(double*fpIn1,double*fpIn2,double*fpOut,int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Div_FRS(double*fpIn1,double*fpIn2,double*fpOut,int N)
 *	函数功能:		双精度实数序列除以双精度实数序列
 *	算术公式表达:	Out[i]=In1[i]/In2[i], i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度实数序列1起始地址,4B对齐
 *				float*fpIn2		:输入单精度实数序列2起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		如果输入序列2中包含0,那么输出值对应的位置会变成非法值,其他正常值的计算不会受到影响
 *				输出序列fpOut可覆盖输入序列fpIn1或fpIn2,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_DRS_Div_DRS(double*fpIn1,double*fpIn2,double*fpOut,int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_I8RS_To_FRS(short*fpIn, float*fpOut, int N)
 *	函数功能: 		8位有符号整型实数序列转换为单精度实数序列
 *	算术公式表达:	Out[i]=(float)In[i], i=0...N-1
 *	参数说明:		float*spIn		:输入8位有符号整型实数序列起始地址,无对齐要求
 *		    	float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_I8RS_To_FRS(short*spIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_U8RS_To_FRS(unsigned char*fpIn, float*fpOut, int N)
 *	函数功能: 		8位无符号整型实数序列转换为单精度实数序列
 *	算术公式表达:	Out[i]=(float)In[i], i=0...N-1
 *	参数说明:		float*ucpIn		:输入8位无符号整型实数序列起始地址,2B对齐
 *		    	float*fpOut		:输出单精度实数序列起始地址,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_U8RS_To_FRS(unsigned char*ucpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_I16RS_To_FRS(short*fpIn, float*fpOut, int N)
 *	函数功能: 		16位有符号整型实数序列转换为单精度实数序列
 *	算术公式表达:	Out[i]=(float)In[i], i=0...N-1
 *	参数说明:		float*spIn		:输入16位有符号整型实数序列起始地址,2B对齐
 *		    	float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_I16RS_To_FRS(short*spIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_U16RS_To_FRS(unsigned short*fpIn, float*fpOut, int N)
 *	函数功能: 		16位无符号整型实数序列转换为单精度实数序列
 *	算术公式表达:	Out[i]=(float)In[i]
 *	参数说明:		float*uspIn		:输入16位无符号整型实数序列起始地址,2B对齐
 *		    	float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_U16RS_To_FRS(unsigned short*uspIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_I16RS_To_FRS_Swp(short*fpIn, float*fpOut, int N)
 *	函数功能: 		16位有符号整型实数序列转换为单精度实数序列,同时高低位对调
 *	算术公式表达:	Out[i]=(float)In[i+1], i=0...N*2-1
 *				Out[i+1]=(float)In[i], i=0...N*2-1
 *	参数说明:		float*spIn		:输入16位有符号整型实数序列起始地址,2B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,8B对齐
 *				int N			:处理点数N,N>0,且N为2的倍数
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_I16RS_To_FRS_Swp(short*spIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_U16RS_To_FRS_Swp(unsigned short*fpIn, float*fpOut, int N)
 *	函数功能: 		16位无符号整型实数序列转换为单精度实数序列,同时高低位对调
 *	算术公式表达:	Out[i]=(float)In[i+1]
 *				Out[i+1]=(float)In[i]
 *	参数说明:		float*uspIn		:输入16位无符号整型实数序列起始地址,2B对齐
 *		    	float*fpOut		:输出单精度实数序列起始地址,8B对齐
 *		    	int N			:处理点数N,N>0,且N为2的倍数
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_U16RS_To_FRS_Swp(unsigned short*uspIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_To_DRS(float*fpIn, double*fpOut, int N)
 *	函数功能: 		单精度实数序列转换为双精度实数序列
 *	算术公式表达:	Out[i]=(double)In[i], i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *		    	float*dpOut		:输出双精度实数序列起始地址,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_To_DRS(float*fpIn, double*dpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_I32RS_To_FRS(int*ipIn, float*fpOut, int N)
 *	函数功能: 		有符号32位整型实数序列转化为单精度实数序列
 *	算术公式表达:	Out[i]=(float)In[i], i=0...N-1
 *	参数说明:		int*ipIn			:输入有符号32位整型实数序列起始地址,4B对齐
 *		    	float*fpOut			:输出单精度实数序列起始地址,4B对齐
 *		    	int N				:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输入浮点数序列、输出浮点数序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_I32RS_To_FRS(int*ipIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_U32RS_To_FRS(unsigned int*uipIn, float*fpOut, int N)
 *	函数功能: 		无符号32位整型实数序列转化为单精度实数序列
 *	算术公式表达:	Out[i]=(float)In[i], i=0...N-1
 *	参数说明:		unsigned int*uipIn	:输入无符号32位整型实数序列起始地址,4B对齐
 *		    	float*fpOut			:输出单精度实数序列起始地址,4B对齐
 *		    	int N				:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输入浮点数序列、输出浮点数序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_U32RS_To_FRS(unsigned int*uipIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_FRS_To_I16RS(float*fpIn, short*spOut, int N)
 *	函数功能: 		单精度实数序列转化为16位有符号整型实数序列
 *	算术公式表达:	Out[i]=(int)In[i], i=0...N-1
 *	参数说明:		float*fpIn				:输入单精度实数序列起始地址,8B对齐
 *		    	short*spOut				:输出有符号16位整型实数序列起始地址,4B对齐
 *		    	int N					:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_FRS_To_I16RS(float*fpIn, short*spOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_FRS_To_U16RS(float*fpIn, unsigned short*uspOut, int N)
 *	函数功能: 		单精度实数序列转化为16位无符号整型实数序列
 *	算术公式表达:	Out[i]=(int)In[i], i=0...N-1
 *	参数说明:		float*fpIn				:输入单精度实数序列起始地址,8B对齐
 *		    	unsigned short*uspOut	:输出无符号16位整型实数序列起始地址,4B对齐
 *		    	int N					:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_FRS_To_U16RS(float*fpIn, unsigned short*uspOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_MemCpy(void*pIn, void*pOut, int N)
 *	函数功能: 		连续内存拷贝
 *	算术公式表达:	Out[i]=In[i], i=0...N-1
 *	参数说明:		void*pIn		:源数组的起始地址,4B对齐
 *				void*pOut		:目的数组的起始地址,4B对齐
 *				int N			:需要拷贝的字节数,N>0
 *	调用核:		所有核均可自行调用
 *	返回值:		void
 *	备注说明:		当源与目的地址均为8B对齐的情况下,推荐用此函数,效率要比官方库中的memcpy高;
 *				输入浮点数序列、输出浮点数序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_MemCpy(void*pIn, void*pOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_MemCpy_Stride(void*pIn, void*pOut, int N, int stride_In, int stride_Out)
 *	函数功能: 		跳变内存拷贝
 *	算术公式表达:	Out[i*Stride_Out]=in[i*Stride_In], i=0...N-1
 *	参数说明:		void*pIn		:源数组的起始地址,4B对齐
 *		    	void*pOut		:目的数组的起始地址,4B对齐
 *		    	int N			:需要拷贝的单位个数,N>0
 *		    	int Stride_In	:输入跳变单位个数
 *		    	int Stride_Out	:输出跳变单位个数
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		拷贝单位与跳变单位均为4B大小
 ***************************************************************************************/
extern void Raco_Vec_C6678_MemCpy_Stride(void*pIn, void*pOut, int N, int stride_In, int stride_Out);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_MemSet(void*pIn, void*pOut, int N, int Mode)
 *	函数功能: 		对应C语言的memset,连续内存写入
 *	算术公式表达:	Out[i]=in, i=0...N-1
 *	参数说明:		void*pIn		:需要进行连续拷贝的双字的起始地址,无对齐要求
 *		    	void*pOut		:输出数据存放起始地址,单位对齐
 *		    	int N			:连续拷贝的单位数N,N>0
 *		    	int Mode		:拷贝单位类型,Mode==0,拷贝单位为4B,Mode==1,拷贝单位为8B
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_MemSet(void*pIn, void*pOut, int N, int Mode);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_Rand(unsigned int*uipOut, int N)
 *	函数功能: 		随机整数序列生成
 *	算术公式表达:	Out[i]=rand(), i=0...N-1
 *	参数说明:		void*pOut		:输出无符号32位整型随机数序列首地址
 *		    	int N			:生成的点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_Rand(unsigned int*uipOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_SRand(unsigned int uiVal)
 *	函数功能: 		随机整数序列种子初始化
 *	算术公式表达:	Seed=uiVal
 *	参数说明:		unsigned int uiVal	:随机数种子初始化的值
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_SRand(unsigned int uiVal);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_GaussianNoise_Gen(float*fpOut, int N, float ave, float sigma, float sigma_fir, void*pBuffer)
 *	函数功能: 		高斯噪声序列生成
 *	算术公式表达:	u = (float)( rand() + 1 ) / ( RAND_MAX + 1 );
 *				v = (float)( rand() + 1 ) / ( RAND_MAX + 1 );
 *				r = sqrtsp( ( -2 * log2sp(v) ) / log2sp( expsp(1) ) ) ;
 *				z = r * cossp( 2 * PI * u );
 *				Out[i] = ( ave + z * sigma ) * Sigma_fir, i=0...N-1
 *	参数说明:		float*fpOut			:输出单精度实数序列首地址
 *				int N				:处理点数N,N>0
 *				float ave			:输出序列的期望
 *				float sigma			:输出序列的方差
 *				float sigma_fir		:输出序列的功率
 *				void*pBuffer		:临时缓存,占用大小为8*(N+N%2)个字节
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_GaussianNoise_Gen(float*fpOut, int N, float ave, float sigma, float sigma_fir, void*pBuffer);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Sin(float*fpIn, float*fpOut, int N)
 *	函数功能:		实现单精度实数序列求sin
 *	算术公式表达:	Out[i]=sin(In[i]*2*PI), i=0...N-1
 *	参数说明:		float*fpIn			:输入单精度实数序列起始地址,为弧度值X/2PI,8B对齐
 *				float*fpOut			:输出单精度实数序列起始地址,8B对齐
 *				int N				:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出地址fpOut可以覆盖输入地址fpIn,但不会带来效率的提升;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Sin(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Cos(float*fpIn, float*fpOut, int N)
 *	函数功能:		实现单精度实数序列求cos
 *	算术公式表达:	Out[i]=cos(In[i]*2*PI), i=0...N-1
 *	参数说明:		float*fpIn			:输入单精度实数序列起始地址,为弧度值X/2PI,8B对齐
 *				float*fpOut			:输出单精度实数序列起始地址,8B对齐
 *				int N				:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出地址fpOut可以覆盖输入地址fpIn,但不会带来效率的提升;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Cos(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Cos_Sin(float*fpIn, float*fpOut, int N)
 *	函数功能: 		实现单精度实数序列求cos和sin
 *	算术公式表达:	Out[i*2]=cos(In[i]*2*PI), i=0...N-1
 *				Out[i*2+1]=sin(In[i]*2*PI), i=0...N-1
 *	参数说明:		float*fpIn			:输入单精度实数序列起始地址,为弧度值X/2PI,8B对齐
 *				float*fpOut			:输出单精度实数序列起始地址,存放第一个是cos值,第二个是sin值,8B对齐
 *		    	int N				:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Cos_Sin(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Cos_Sin_Mul_FRC(float*fpIn, float*fpOut, int N, float fVal)
 *	函数功能:		对单精度实数序列求cos和sin,并与指定的浮点常数相乘
 *	算术公式表达:	Out[i*2]=cos(In[i]*2*PI)*fVal, i=0...N-1
 *				Out[i*2+1]=sin(In[i]*2*PI)*fVal, i=0...N-1
 *	参数说明:		float*fpIn			:输入单精度实数序列起始地址,为弧度值X/2PI,8B对齐
 *				float*fpOut			:输出单精度实数序列起始地址,8B对齐
 *				int N				:处理点数N,N>0
 *				float fVal			:与cos和sin结果相乘的单精度实常数
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Cos_Sin_Mul_FRC(float*fpIn, float*fpOut, int N, float fVal);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Mul_FRC_Cos_Sin(float*fpIn, float*fpOut, int N, float fVal)
 *	函数功能:		对单精度实数序列与指定的浮点常数相乘,并求sin和cos
 *	算术公式表达:	Out[i*2]=cos(In[i]*fVal*2*PI), i=0...N-1
 *				Out[i*2+1]=sin(In[i]*fVal*2*PI), i=0...N-1
 *	参数说明:		float*fpIn			:输入单精度实数序列起始地址,为弧度值X/2PI,8B对齐
 *				float*fpOut			:输出单精度实数序列起始地址,8B对齐
 *				int N				:处理点数N,N>0
 *				float fval			:与输入值相乘的单精度实常数
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Mul_FRC_Cos_Sin(float*fpIn, float*fpOut, int N, float fVal);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Mul_FRC_Cos_Sin(float*fpIn, float*fpOut, int N, float fVal)
 *	函数功能:		单精度实数序列求Log2
 *	算术公式表达:	Out[i]=log2(In[i]), i=0...N-1
 *	参数说明:		float*fpIn			:输入单精度实数序列起始地址,8B对齐
 *				float*fpOut			:输出单精度实数序列起始地址,8B对齐
 *				int N				:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Log2(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Qua_I8RS(float*fpIn, char*cpOut, int N, float fMin, float fMax, float fRatio, int iMin_Real)
 *	函数功能:		对输入单精度实数序列利用输入门限量化,有符号8位
 *	算术公式表达:	if(In[i]<Min)
 *				{
 *					Out[i]=0;
 *				}
 *				else if(In[i]>Max*Ratio )
 *				{
 *					Out[i]=127;
 *				}
 *				else
 *				{
 *					Out[i]=(In[i]-Min)/(Max*Ratio-Min)*127;
 *				}
 *	参数说明:		float*fpIn				:输入单精度实数序列起始地址,4B对齐
 *				char*cpOut				:输出有符号8位整型实数序列起始地址,8B对齐
 *				int N					:处理点数N,N>0
 *				float fMin				:量化数据的最小值
 *				float fMax				:量化数据的最大值
 *				float fRatio			:削峰系数,取值范围:0~1之间
 *				int iMin_Real			:Min_Real==0,存在比fMin还小的输入值
 *										:Min_Real==1,不存在比fMin还小的输入值
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		当输入地址为8B对齐时,效率要高于输入地址为4B对齐
 *				当不存在比fMin还小的输入值时,Min_Real无论为0还是1都可以计算得出正确的数据;
 *				但推荐选用Min_Real=1模式,因为此模式的执行效率要比Min_Real=0高;
 *				但如果存在比fMin还小的输入值时,Min_Real需设置为0,否则将输出错误结果;
 *				输出序列cpOut可以覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Qua_I8RS(float*fpIn, char*cpOut, int N, float fMin, float fMax, float fRatio, int iMin_Real);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Qua_U8RS(float*fpIn, unsigned char*ucpOut, int N, float fMin, float fMax, float fRatio, int iMin_Real)
 *	函数功能:		对输入单精度实数序列利用输入门限量化,无符号8位
 *	算术公式表达:	if(In[i]<Min)
 *				{
 *					Out[i]=0;
 *				}
 *				else if(In[i]>Max*Ratio )
 *				{
 *					Out[i]=255;
 *				}
 *				else
 *				{
 *					Out[i]=(In[i]-Min)/(Max*Ratio-Min)*255;
 *				}
 *	参数说明:		float*fpIn				:输入单精度实数序列起始地址,4B对齐
 *				unsigned char*ucpOut	:输出无符号8位整型实数序列起始地址,8B对齐
 *				int N					:处理点数N,N>0
 *				float fMin				:量化数据的最小值
 *				float fMax				:量化数据的最大值
 *				float fRatio			:削峰系数,取值范围:0~1之间
 *				int iMin_Real			:Min_Real==0,存在比fMin还小的输入值
 *										:Min_Real==1,不存在比fMin还小的输入值
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		当输入地址为8B对齐时,效率要高于输入地址为4B对齐;
 *				当不存在比fMin还小的输入值时,Min_Real无论为0还是1都可以计算得出正确的数据;
 *				但推荐选用Min_Real=1模式,因为此模式的执行效率要比Min_Real=0高;
 *				但如果存在比fMin还小的输入值时,Min_Real需设置为0,否则将输出错误结果;
 *				输出序列ucpOut可以覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Qua_U8RS(float*fpIn, unsigned char*ucpOut, int N, float fMin, float fMax, float fRatio, int iMin_Real);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Qua_I16RS(float*fpIn, short*spOut, int N, float fMin, float fMax, float fRatio, int iMin_Real)
 *	函数功能:		对输入单精度实数序列利用输入门限量化,有符号16位
 *	算术公式表达:	if(In[i]<Min)
 *				{
 *					Out[i]=0;
 *				}
 *				else if(In[i]>Max*Ratio )
 *				{
 *					Out[i]=32767;
 *				}
 *				else
 *				{
 *					Out[i]=(In[i]-Min)/(Max*Ratio-Min)*32767;
 *				}
 *	参数说明:		float*fpIn				:输入单精度实数序列起始地址,4B对齐
 *				short*spOut				:输出有符号16位整型数序列起始地址,8B对齐
 *				int N					:处理点数N,N>0
 *				float fMin				:量化数据的最小值
 *				float fMax				:量化数据的最大值
 *				float fRatio			:削峰系数,取值范围:0~1之间
 *				int iMin_Real			:Min_Real==0,存在比fMin还小的输入值
 *										:Min_Real==1,不存在比fMin还小的输入值
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		当输入地址为8B对齐时,效率要高于输入地址为4B对齐;
 *				当不存在比fMin还小的输入值时,Min_Real无论为0还是1都可以计算得出正确的数据;
 *				但推荐选用Min_Real=1模式,因为此模式的执行效率要比Min_Real=0高;
 *				但如果存在比fMin还小的输入值时,Min_Real需设置为0,否则将输出错误结果;
 *				输出序列spOut可以覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Qua_I16RS(float*fpIn, short*spOut, int N, float fMin, float fMax, float fRatio, int iMin_Real);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Qua_U16RS(float*fpIn, unsigned short*uspOut, int N, float fMin, float fMax, float fRatio, int MinReal)
 *	函数功能:		对输入数组利用输入门限量化,无符号16位
 *	算术公式表达:	if(In[i]<Min)
 *				{
 *					Out[i]=0;
 *				}
 *				else if(In[i]>Max*Ratio )
 *				{
 *					Out[i]=65535;
 *				}
 *				else
 *				{
 *					Out[i]=(In[i]-Min)/(Max*Ratio-Min)*65535;
 *				}
 *	参数说明:		float*fpIn				:输入浮点数序列起始地址,4B对齐
 *				unsigned short*uspOut	:输出16位整型数序列起始地址,8B对齐
 *				int N					:处理点数N,N>0
 *				float fMin				:量化数据的最小值
 *				float fMax				:量化数据的最大值
 *				float fRatio			:削峰系数,取值范围:0~1之间
 *				int iMin_Real			:Min_Real==0,存在比fMin还小的输入值
 *										:Min_Real==1,不存在比fMin还小的输入值
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		当输入地址为8B对齐时,效率要高于输入地址为4B对齐;
 *				当不存在比fMin还小的输入值时,Min_Real无论为0还是1都可以计算得出正确的数据;
 *				但推荐选用Min_Real=1模式,因为此模式的执行效率要比Min_Real=0高;
 *				但如果存在比fMin还小的输入值时,Min_Real需设置为0,否则将输出错误结果;
 *				输出序列uspOut可以覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Qua_U16RS(float*fpIn, unsigned short*uspOut, int N, float fMin, float fMax, float fRatio, int MinReal);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Cmp_FRC_1(float*fpIn, void*pOut, int N, float fCMP_C, void*pFlag_In, void*pFlag_Out, int Mode)
 *	函数功能:		单精度实数序列与单精度实常数比较
 *	算术公式表达:	if(In[i] cmp fCMP_C)	Out[i]=Flag_In
 *				else 					Out[i]=Flag_Out
 *				i=0...N-1, cmp为可选择的比较方式
 *	参数说明:		float*fpIn		:输入单精度实数序列的起始地址,4B对齐
 *				void*pOut		:输出序列的起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *				float fCMP_C	:与单精度实数序列比较的单精度实常数
 *				void*pFlag_In	:输入值落在区间内的标志变量地址,4B对齐
 *				void*pFlag_Out	:输入值落在区间外的标志变量地址,4B对齐
 *				int Mode		:比较的方式,0为等于,1为不等于,2为小于,3为小于等于,4为大于,5为大于等于
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		用户可自由选择pOut的的类型,来满足不同场合的	高效运算;
 *				共有三种类型可选择:float\int\unsigned int;
 *				但pFlag_In、pFlag_Out所指向的变量类型必须和期望输出*pOut的类型保持一致;
 *				输出序列pOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 *				对齐程度越高,执行速度越快,具体效率如下:
 *				输入输出序列均为8B对齐>输入输出序列均为4B对齐>输入输出序列既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Cmp_FRC_1(float*fpIn, void*pOut, int N, float fCMP_C, void*pFlag_In, void*pFlag_Out, int Mode);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Cmp_FRC_2(float*fpIn, int*ipOut, int N, float fCMP_L, float fCMP_R, int iFlag_In, int iFlag_Out)
 *	函数功能:		单精度实数序列与两个单精度实常数比较,若fCMP_L<In<=fCMP_R,则Out=Flag_In,否则Out=Flag_Out
 *	算术公式表达:	if(fCMP_L<In[i]<=fCMP_R)	Out[i]=Flag_In
 *				else 						Out[i]=Flag_Out
 *	参数说明:		float*fpIn		:输入单精度实数序列的起始地址
 *				int*ipOut		:输出32位有符号整型实数序列的起始地址
 *				int N			:处理点数N,N>0
 *				float fCMP_L	:与浮点数序列比较的左值,单精度实常数
 *				float fCMP_R	:与浮点数序列比较的右值,单精度实常数
 *				int iFlag_In	:输入值落在区间内的标志,32位有符号整型实数
 *				int iFlag_Out	:输入值落在区间外的标志,32位有符号整型实数
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Cmp_FRC_2(float*fpIn, int*ipOut, int N, float fCMP_L, float fCMP_R, int iFlag_In, int iFlag_Out);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Cmp_FRS(float*fpIn1, float*fpIn2, void*pOut, int N, void*Flag_In, void*Flag_Out, int Mode)
 *	函数功能:		单精度实数序列与单精度实数序列比较
 *	算术公式表达:	if(In1[i] cmp In1[i])		Out[i]=Flag_In
 *				else 						Out[i]=Flag_Out
 *	参数说明:		float*fpIn1		:输入单精度实数序列1的起始地址
 *				float*fpIn2		:输入单精度实数序列2的起始地址
 *				void*pOut		:输出单精度实数序列的起始地址
 *				int N			:处理点数N,N>0
 *				void*Flag_In	:输入值落在区间内的标志变量地址
 *				void*Flag_Out	:输入值落在区间外的标志变量地址
 *				int Mode		:比较的方式,0为等于,1为不等于,2为小于,3为小于等于,4为大于,5为大于等于
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		用户可自由选择pOut的的类型,来满足不同场合的高效运算;
 *				共有三种类型可选择:float\int\unsigned int;
 *				但pFlag_In、pFlag_Out所指向的变量类型必须和期望输出*pOut的类型保持一致;
 *				输出序列pOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 *				输入序列与输出序列均需4B对齐;
 *				对齐程度越高,执行速度越快,具体效率如下:
 *				输入输出序列均为8B对齐>输入输出序列均为4B对齐>输入输出序列既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Cmp_FRS(float*fpIn1, float*fpIn2, void*pOut, int N, void*Flag_In, void*Flag_Out, int Mode);
/**********************Vector End**********************/


/**********************Matrix Start********************/
/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Add_FCM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度复数矩阵加单精度复数矩阵
 *	算术公式表达:	Out[i][j].re=In1[i][j].re+In2[i][j].re;
 *				Out[i][j].im=In1[i][j].im+In2[i][j].im;
 *	参数说明:		float*fpIn1		:输入单精度复数矩阵1起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度复数矩阵2起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	int Row			:输入单精度复数矩阵行数,Row>0
 *		    	int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1或fpIn2,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Add_FCM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Sub_FCM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度复数矩阵减单精度复数矩阵
 *	算术公式表达:	Out[i][j].re=In1[i][j].re-In2[i][j].re;
 *				Out[i][j].im=In1[i][j].im-In2[i][j].im;
 *	参数说明:		float*fpIn1		:输入单精度复数矩阵1起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn1		:输入单精度复数矩阵2起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	int Row			:输入单精度复数矩阵行数,Row>0
 *		    	int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1或fpIn2,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Sub_FCM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Mul_FCM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度复数矩阵乘单精度复数矩阵,点乘
 *	算术公式表达:	Out[i][j].re=In1[i][j].re*In2[i][j].re-In1[i][j].im*In2[i][j].im;
 *				Out[i][j].im=In1[i][j].re*In2[i][j].im+In1[i][j].im*In2[i][j].re;
 *	参数说明:		float*fpIn1		:输入单精度复数矩阵1起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度复数矩阵2起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	int Row			:输入单精度复数矩阵行数,Row>0
 *		    	int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1或fpIn2,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Mul_FCM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_MMul_FCM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度复数矩阵乘单精度复数矩阵,矩阵乘
 *	算术公式表达:	Out[i][k]=In1[i][j]*In2[j][k]
 *	参数说明:		float*fpIn1		:输入单精度复数矩阵1起始地址,4B对齐
 *				float*fpIn2		:输入单精度复数矩阵2起始地址,4B对齐
 *		    	float*fpOut		:输出单精度复数矩阵起始地址
 *		    	int iRow1		:输入单精度复数矩阵1的行数,iRow1>0
 *		    	int iCol1Row2	:输入单精度复数矩阵1的列数,与输入单精度复数矩阵2的行数相等,iCol1Row2>0
 *		    	int iCol2		:输入单精度复数矩阵2的列数,iCol2>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_MMul_FCM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Add_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度复数矩阵加单精度实数矩阵
 *	算术公式表达:	Out[i][j].re=In1[i][j].re+In2[i][j];
 *				Out[i][j].im=In1[i][j].im;
 *	参数说明:		float*fpIn1		:输入单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度实数矩阵起始地址,8B对齐
 *		    	float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	int Row			:输入单精度复数矩阵行数,Row>0
 *		    	int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Add_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Sub_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度复数矩阵减单精度实数矩阵
 *	算术公式表达:	Out[i][j].re=In1[i][j].re-In2[i][j];
 *				Out[i][j].im=In1[i][j].im;
 *	参数说明:		float*fpIn1		:输入单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度实数矩阵起始地址,8B对齐
 *		    	float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	int Row			:输入单精度复数矩阵行数,Row>0
 *		    	int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Sub_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Mul_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度复数矩阵乘单精度实数矩阵,点乘
 *	算术公式表达:	Out[i][j].re=In1[i][j].re*In2[i][j];
 *				Out[i][j].im=In1[i][j].im*In2[i][j];
 *	参数说明:		float*fpIn1		:输入单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度实数矩阵起始地址,8B对齐
 *		    	float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	int Row			:输入单精度复数矩阵行数,Row>0
 *		    	int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Mul_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Add_FRC(float*fpIn, float*fpOut, float fVal, int Row, int Col)
 *	函数功能: 		单精度复数矩阵加单精度实常数
 *	算术公式表达:	Out[i][j].re=In1[i][j].re+fVal;
 *				Out[i][j].im=In1[i][j].im;
 *	参数说明:		float*fpIn		:输入单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	float fVal		:与单精度复数矩阵相加的单精度实常数
 *		    	int Row			:输入单精度复数矩阵行数,Row>0
 *		    	int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Add_FRC(float*fpIn, float*fpOut, float fVal, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Sub_FRC(float*fpIn, float*fpOut, float fVal, int Row, int Col)
 *	函数功能: 		单精度复数矩阵减单精度实常数
 *	算术公式表达:	Out[i][j].re=In1[i][j].re-fVal;
 *				Out[i][j].im=In1[i][j].im;
 *	参数说明:		float*fpIn		:输入单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	float fVal		:与单精度复数矩阵相减的单精度实常数
 *		    	int Row			:输入单精度复数矩阵行数,Row>0
 *		    	int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Sub_FRC(float*fpIn, float*fpOut, float fVal, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Mul_FRC(float*fpIn, float*fpOut, float fVal, int Row, int Col)
 *	函数功能: 		单精度复数矩阵乘单精度实常数,点乘
 *	算术公式表达:	Out[i][j].re=In1[i][j].re*fVal;
 *				Out[i][j].im=In1[i][j].im*fVal;
 *	参数说明:		float*fpIn		:输入单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *		    	float fVal		:与单精度复数矩阵相乘的实常数
 *		    	int Row			:输入单精度复数矩阵行数,Row>0
 *		    	int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Mul_FRC(float*fpIn, float*fpOut, float fVal, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Add_FCC(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度复数矩阵加单精度复常数
 *	算术公式表达:	Out[i][j].re=In1[i][j].re+In2.re;
 *				Out[i][j].im=In1[i][j].im+In2.im;
 *	参数说明:		float*fpIn1		:输入单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度复常数地址,实部在前,虚部在后,4B对齐
 *				float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *				int Row			:输入单精度复数矩阵行数,Row>0
 *				int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Add_FCC(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Sub_FCC(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度复数矩阵减单精度复常数
 *	算术公式表达:	Out[i][j].re=In1[i][j].re-In2.re;
 *				Out[i][j].im=In1[i][j].im-In2.im;
 *	参数说明:		float*fpIn1		:输入单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度复常数地址,实部在前,虚部在后,4B对齐
 *				float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *				int Row			:输入单精度复数矩阵行数,Row>0
 *				int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Sub_FCC(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCM_Mul_FCC(float*fpIn1, float*fpIn2, int Row, int Col, float*fpOut)
 *	函数功能: 		单精度复数矩阵乘单精度复常数,点乘
 *	算术公式表达:	Out[i][j].re=In1[i][j].re*In2.re-In1[i][j].im*In2.im;
 *				Out[i][j].im=In1[i][j].re*In2.im-In1[i][j].im*In2.re;
 *	参数说明:		float*fpIn1		:输入单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *				float*fpIn2		:输入单精度复常数地址,实部在前,虚部在后,4B对齐
 *				float*fpOut		:输出单精度复数矩阵起始地址,实部在前,虚部在后,8B对齐
 *				int Row			:输入单精度复数矩阵行数,Row>0
 *				int Col			:输入单精度复数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1,若进行覆盖,则函数执行效率可以更高;
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCM_Mul_FCC(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRM_Add_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度实数矩阵加单精度实数矩阵
 *	算术公式表达:	Out[i][j]=In1[i][j]+In2[i][j]
 *	参数说明:		float*fpIn1		:输入单精度实数矩阵1起始地址,4B对齐
 *				float*fpIn1		:输入单精度实数矩阵2起始地址,4B对齐
 *		    	float*fpOut		:输出单精度实数矩阵起始地址,4B对齐
 *		    	int Row			:输入单精度实数矩阵行数,Row>0
 *		    	int Col			:输入单精度实数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1或fpIn2,若进行覆盖,则函数执行效率可以更高;
 *				输入矩阵1、2,输出矩阵均需4B对齐;
 *				对齐程度越高,执行速度越快,具体效率如下:
 *				三个矩阵均为8B对齐>三个矩阵均为4B对齐>三个矩阵既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRM_Add_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRM_Sub_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度实数矩阵减单精度实数矩阵
 *	算术公式表达:	Out[i][j]=In1[i][j]-In2[i][j]
 *	参数说明:		float*fpIn1		:输入单精度实数矩阵1起始地址,4B对齐
 *				float*fpIn2		:输入单精度实数矩阵2起始地址,4B对齐
 *		    	float*fpOut		:输出单精度实数矩阵起始地址,4B对齐
 *		    	int Row			:输入单精度实数矩阵行数,Row>0
 *		    	int Col			:输入单精度实数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1或fpIn2,若进行覆盖,则函数执行效率可以更高;
 *				输入矩阵1、2,输出矩阵均需4B对齐;
 *				对齐程度越高,执行速度越快,具体效率如下:
 *				三个矩阵均为8B对齐>三个矩阵均为4B对齐>三个矩阵既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRM_Sub_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRM_Mul_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col)
 *	函数功能: 		单精度实数矩阵乘单精度实数矩阵,点乘
 *	算术公式表达:	Out[i][j]=In1[i][j]*In2[i][j]
 *	参数说明:		float*fpIn1		:输入单精度实数矩阵1起始地址,4B对齐
 *				float*fpIn1		:输入单精度实数矩阵2起始地址,4B对齐
 *		    	float*fpOut		:输出单精度实数矩阵起始地址,该地址可覆盖fpIn1或fpIn2
 *		    	int Row			:输入单精度实数矩阵行数,Row>0
 *		    	int Col			:输入单精度实数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出矩阵fpOut可以覆盖输入矩阵fpIn1或fpIn2,若进行覆盖,则函数执行效率可以更高;
 *				输入矩阵1、2,输出矩阵均需4B对齐;
 *				对齐程度越高,执行速度越快,具体效率如下:
 *				三个矩阵均为8B对齐>三个矩阵均为4B对齐>三个矩阵既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRM_Mul_FRM(float*fpIn1, float*fpIn2, float*fpOut, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRM_MMul_FRM(float*fpIn1,float *fpIn2,float *restrict fpOut,int iRow1,int iCol1Row2,int iCol2)
 *	函数功能: 		实数矩阵乘实数矩阵,矩阵乘
 *	算术公式表达:	Out[i][k]=In1[i][j]*In2[j][k]
 *	参数说明:		float*fpIn1		:输入实数矩阵1起始地址,4B对齐
 *				float*fpIn2		:输入实数矩阵2起始地址,4B对齐
 *		    	float*fpOut		:输出实数矩阵起始地址
 *		    	int iRow1		:输入实数矩阵1的行数,iRow1>0
 *		    	int iCol1Row2	:输入实数矩阵1的列数,与输入实数矩阵2的行数相等,iCol1Row2>0
 *		    	int iCol2		:输入实数矩阵2的列数,iCol2>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输入矩阵1、2,输出矩阵均需4B对齐;
 *				对齐程度越高,执行速度越快,具体效率如下:
 *				三个矩阵均为8B对齐>三个矩阵均为4B对齐>三个矩阵既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRM_MMul_FRM(float*fpIn1,float *fpIn2,float *restrict fpOut,int iRow1,int iCol1Row2,int iCol2);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRM_Min(float*fpIn, float*fpMinVal, int Row, int Col)
 *	函数功能: 		单精度实数矩阵求最小值
 *	算术公式表达:	Return=min(In[i][j]), i=0...Row-1, j=0...Col-1
 *	参数说明:		float*fpIn		:输入单精度实数矩阵起始地址,4B对齐
 *				float*fpMinVal	:输出单精度实数最小值存放地址,4B对齐
 *		    	int Row			:输入单精度实数矩阵行数,Row>0
 *		    	int Col			:输入单精度实数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRM_Min(float*fpIn, float*fpMinVal, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRM_Max(float*fpIn, float*fpMaxVal, int Row, int Col);
 *	函数功能: 		单精度实数矩阵求最大值
 *	算术公式表达:	Return=max(In[i][j]), i=0...Row-1, j=0...Col-1
 *	参数说明:		float*fpIn		:输入单精度实数矩阵起始地址,4B对齐
 *				float*fpMaxVal	:输出单精度实数最大值存放地址,4B对齐
 *		    	int Row			:输入单精度实数矩阵行数,Row>0
 *		    	int Col			:输入单精度实数矩阵列数,Col>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRM_Max(float*fpIn, float*fpMaxVal, int Row, int Col);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRM_Extract(float*fpIn, float*fpOut, int Row_Num, int R_S, int C_S, int R, int C)
 *	函数功能: 		单精度实数矩阵提取
 *	算术公式表达:	无
 *	参数说明:		float*fpIn		:输入单精度实数矩阵起始地址,8B对齐
 *				float*fpOut		:输出单精度实数矩阵存放地址,8B对齐
 *				int Row_Num		:输入单精度实数矩阵行点数,需满足:Row_Num>0
 *		    	int R_S			:提取单精度实数矩阵的起始行点数,需满足:R_S>=0
 *		    	int C_S			:提取单精度实数矩阵的起始列点数,需满足:C_S>=0
 *		    	int R			:提取单精度实数矩阵的行点数,需满足:R>0
 *				int C			:提取单精度实数矩阵的列点数,需满足:C>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		调用需遵循以下规则:
 *				fpIn	:8B对齐
 *				fpOut	:8B对齐
 *				Row_Num	:2的倍数
 *				R_S		:2的倍数
 *				R		:2的倍数
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRM_Extract(float*fpIn, float*fpOut, int Row_Num, int R_S, int C_S, int R, int C);

/***************************************************************************************
 *	函数名:		int Raco_Vec_C6678_FRM_Inv(float*fpIn, int N, void*Buffer)
 *	函数功能: 		单精度实数矩阵求逆
 *	算术公式表达:	Out=inverse(In);
 *	参数说明:		float*fpIn		:输入单精度实数矩阵起始地址,4B对齐
 *		    	int N			:输入单精度实数矩阵阶数,N>0
 *		    	void*Buffer		:计算临时变量,占用大小为N*8B
 *	调用核:		所有核均可自行调用
 *	返回值: 		int				:为1代表该矩阵可以求逆,为0代表该矩阵不可求逆
 *	备注说明:		无
 ***************************************************************************************/
extern int Raco_Vec_C6678_FRM_Inv(float*fpIn, int N, void*Buffer);

/***************************************************************************************
 *	函数名:		int Raco_Vec_C6678_FCM_Inv(float*fpIn, int N, void*Buffer)
 *	函数功能: 		单精度复数矩阵求逆
 *	算术公式表达:	Out=inverse(In);
 *	参数说明:		float*fpIn		:输入单精度复数矩阵起始地址,8B对齐
 *		    	int N			:输入单精度复数矩阵阶数,N>0
 *		    	void*Buffer		:计算临时变量,占用大小为N*24B
 *	调用核:		所有核均可自行调用
 *	返回值: 		int				:为1代表该矩阵可以求逆,为0代表该矩阵不可求逆
 *	备注说明:		无
 ***************************************************************************************/
extern int Raco_Vec_C6678_FCM_Inv(float*fpIn, int N, void*Buffer);

/***************************************************************************************
 *	函数名:		int Raco_Vec_C6678_DCM_Inv(double*fpIn, int N, void*Buffer)
 *	函数功能: 		双精度复数矩阵求逆
 *	算术公式表达:	Out=inverse(In)
 *	参数说明:		double*dpIn		:输入双精度复数矩阵起始地址,8B对齐
 *		    	int N			:输入双精度复数矩阵阶数,N>0
 *		    	void*Buffer		:计算临时变量,占用大小为N*40B
 *	调用核:		所有核均可自行调用
 *	返回值: 		int				:为1代表该矩阵可以求逆,为0代表该矩阵不可求逆
 *	备注说明:		无
 ***************************************************************************************/
extern int Raco_Vec_C6678_DCM_Inv(double*dpIn, int N, void*Buffer);
/**********************Matrix End**********************/


/**********************Matlab Start********************/
/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FCS_FFTShift(float*fpIn, float*fpOut, float fVal, int N)
 *	函数功能: 		实现Matlab中fftshift函数
 *	算术公式表达:	Out=fftshift(In)
 *	参数说明:		float*fpIn		:输入单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	float*fpOut		:输出单精度复数序列起始地址,实部在前,虚部在后,8B对齐
 *		    	int N			:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		函数计算为复数计算,非实数计算
 ***************************************************************************************/
extern void Raco_Vec_C6678_FCS_FFTShift(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Hamming(float*fpOut, int N)
 *	函数功能: 		实现Matlab中的汉明窗功能
 *	算术公式表达:	Out=hamming(N)
 *	参数说明:		float*fpOut			:输出单精度实数序列起始地址,4B对齐
 *		    	int N				:处理点数N,N>0
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Hamming(float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Linspace(float*fpOut, int N, float fa, float fb)
 *	函数功能:		实现Matlab中的linspace函数
 *	算术公式表达:	Out=linspace(fVal1,fVal2,N)
 *	参数说明:		float*fpOut		:输出单精度实数序列的起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *				float fVal1		:产生数组的第一个值,单精度实常数
 *				float fVal2		:产生数组的最后一个值,单精度实常数
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Linspace(float*fpOut, int N, float fVal1, float fVal2);

/***************************************************************************************
 *	函数名:		float Raco_Vec_C6678_FRS_Variance(float*fpOut, int N, int Mode)
 *	函数功能:		实现Matlab中的var函数,求单精度实数序列的方差
 *	算术公式表达:	In_Ave=(In[0]+In[1]+...+In[N-1])/N
 *				Out=( (In[0]-In_Ave)^2 + (In[1]-In_Ave)^2 + ... + (In[N-1]-In_Ave)^2 )/N-1
 *	参数说明:		float*fpIn		:输入单精度实数序列的起始地址,4B对齐
 *				int N			:处理点数N,N>1
 *				int Mode		:Mode==0,为低精度算法,Mode==1,为高精度算法
 *	返回值: 		float			:输入序列的方差
 *	备注说明:		在输入元素之和过大时,可能会导致低精度算法的输出结果错误,高精度算法无此问题
 *				低精度算法的执行效率要高于高精度算法
 ***************************************************************************************/
extern float Raco_Vec_C6678_FRS_Variance(float*fpOut, int N, int Mode);

/***************************************************************************************
 *	函数名:		float Raco_Vec_C6678_FRS_Standard(float*fpOut, int N, int Mode)
 *	函数功能:		实现Matlab中的std函数,求单精度实数序列的标准差
 *	算术公式表达:	In_Ave=(In[0]+In[1]+...+In[N-1])/N
 *				Out=sqrt( ( (In[0]-In_Ave)^2 + (In[1]-In_Ave)^2 + ... + (In[N-1]-In_Ave)^2 )/N-1 )
 *	参数说明:		float*fpIn		:输入单精度实数序列的起始地址,4B对齐
 *				int N			:处理点数N,N>1
 *				int Mode		:Mode==0,为低精度算法,Mode==1,为高精度算法
 *	返回值: 		float			:输入序列的标准差
 *	备注说明:		在输入元素之和过大时,可能会导致低精度算法的输出结果错误,高精度算法无此问题
 *				低精度算法的执行效率要高于高精度算法
 ***************************************************************************************/
extern float Raco_Vec_C6678_FRS_Standard(float*fpOut, int N, int Mode);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Ceil(float*fpIn, void*pOut, int N, int Mode)
 *	函数功能: 		实现Matlab中ceil函数,单精度实数序列向上取整
 *	算术公式表达:	Out=ceil(In)
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *		    	void*pOut		:输出序列起始地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *		    	int Mode		:输出格式,Mode==0,输出格式为整形,Mode==1,输出格式为浮点型
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 *				输入浮点数序列、输出浮点数序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Ceil(float*fpIn, void*pOut, int N, int Mode);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Floor(float*fpIn, void*pOut, int N, int Mode)
 *	函数功能: 		实现Matlab中floor函数,单精度实数序列向下取整
 *	算术公式表达:	Out=floor(In)
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *		    	void*pOut		:输出序列起始地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *		    	int Mode		:输出格式,Mode==0,输出格式为整形,Mode==1,输出格式为浮点型
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 *				输入浮点数序列、输出浮点数序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Floor(float*fpIn, void*pOut, int N, int Mode);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Fix(float*fpIn, void*pOut, int N, int Mode)
 *	函数功能: 		实现Matlab中fix函数,单精度实数序列向0取整
 *	算术公式表达:	Out=fix(In)
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *		    	void*pOut		:输出序列起始地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *		    	int Mode		:输出格式,Mode==0,输出格式为整形,Mode==1,输出格式为浮点型
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 *				输入浮点数序列、输出浮点数序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Fix(float*fpIn, void*pOut, int N, int Mode);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Round(float*fpIn, void*pOut, int N, int Mode)
 *	函数功能: 		实现Matlab中round函数,单精度实数序列序列四舍五入
 *	算术公式表达:	Out=round(In)
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *		    	void*pOut		:输出序列起始地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *		    	int Mode		:输出格式,Mode==0,输出格式为整形,Mode==1,输出格式为浮点型
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 *				输入浮点数序列、输出浮点数序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Round(float*fpIn, void*pOut, int N, int Mode);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_FRS_Rectpuls(float*fpIn, float*fpOut, int N, float fWidth, int Mode)
 *	函数功能: 		实现Matlab中Rectpuls函数
 *	算术公式表达:	Out=Rectpuls(In,fWidth)
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *		    	void*pOut		:输出序列起始地址,4B对齐
 *		    	int N			:处理点数N,N>0
 *		    	float fWidth	:矩形宽度
 *		    	int Mode		:输出格式,Mode==0,输出格式为整形,Mode==1,输出格式为浮点型
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高;
 *				输入浮点数序列、输出浮点数序列均需4B对齐,并且对齐程度越高,执行速度越快;
 *				具体效率如下:
 *				输入输出地址均为8B对齐>输入输出地址均为4B对齐>输入输出地址既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_FRS_Rectpuls(float*fpIn, float*fpOut, int N, float fWidth, int Mode);

/**********************Matlab End**********************/


/**********************Expression Start********************/
/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_Expression_01(float*fpIn, float*fpOut, int N)
 *	函数功能:		实现公式Out=sqrt(1-In)-1
 *	算术公式表达:	Out[i]=sqrt(1-In[i])-1, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度实数序列的起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列的起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	返回值: 		void
 *	备注说明:		输入值范围:In<=1
 *				输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高
 ***************************************************************************************/
extern void Raco_Vec_C6678_Expression_01(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_Expression_02(float*fpIn, float*fpOut, int N)
 *	函数功能:		实现公式Out=1/sqrt(1-In)-1
 *	算术公式表达:	Out[i]=1/sqrt(1-In[i])-1, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度实数序列的起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列的起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *	返回值: 		void
 *	备注说明:		输入值范围:In<=1
 *				输出序列fpOut可覆盖输入序列fpIn,若进行覆盖,函数执行效率会更高
 ***************************************************************************************/
extern void Raco_Vec_C6678_Expression_02(float*fpIn, float*fpOut, int N);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_Expression_03(float*fpIn, float*fpOut, int N, float fVal1, float fVal2)
 *	函数功能:		实现公式Out=In*fVal1+fVal2
 *	算术公式表达:	Out[i]=In[i]*fVal1+fVal2, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度实数序列起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *				float fVal1		:与单精度实数序列相乘的单精度实常数
 *				float fVal2		:与单精度实数序列相加的单精度实常数
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可以覆盖输入序列fpIn,若进行覆盖,则函数执行效率可以更高;
 *				对齐程度越高,执行速度越快,具体效率如下:
 *				输入输出序列均为8B对齐>输入输出序列均为4B对齐>输入输出序列既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_Expression_03(float*fpIn, float*fpOut, int N, float fVal1, float fVal2);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_Expression_04(float*fpIn1, float*fpIn2, float*fpOut, int N, float fVal1, float fVal2)
 *	函数功能:		实现公式Out=In1*fVal1+In2*fVal2
 *	算术公式表达:	Out[i]=In1[i]*fVal1+In2[i]*fVal2, i=0...N-1
 *	参数说明:		float*fpIn1		:输入单精度实数序列1起始地址,4B对齐
 *				float*fpIn2		:输入单精度实数序列2起始地址,4B对齐
 *				float*fpOut		:输出单精度实数序列起始地址
 *				int N			:处理点数N,N>0
 *				float fVal1		:与单精度实数序列1相乘的单精度实常数
 *				float fVal2		:与单精度实数序列2相乘的单精度实常数
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可以覆盖输入序列fpIn1或fpIn2,若进行覆盖,则函数执行效率可以更高;
 *				对齐程度越高,执行速度越快,具体效率如下:
 *				三个序列均为8B对齐>三个序列均为4B对齐>三个序列既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_Expression_04(float*fpIn1, float*fpIn2, float*fpOut, int N, float fVal1, float fVal2);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_Expression_05(float*fpIn, float*fpOut, int N, float fVal1, float fVal2, float fVal3, float fVal4)
 *	函数功能:		实现公式Out[i]=((In[i]+fVal1)*fVal2+fVal3)*fval4
 *	算术公式表达:	Out[i]=((In[i]+fVal1)*fVal2+fVal3)*fval4, i=0...N-1
 *	参数说明:		float*fpIn		:输入单精度浮点数序列的起始地址,4B对齐
 *				float*fpOut		:输出单精度浮点数序列的起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *				float fVal1		:与单精度浮点数序列相加的常数1
 *				float fVal2		:与单精度浮点数序列相乘的常数2
 *				float fVal3		:与单精度浮点数序列相加的常数3
 *				float fVal4		:与单精度浮点数序列相乘的常数4
 *	返回值: 		void
 *	备注说明:		输出序列fpOut可以覆盖输入序列fpIn,若进行覆盖,则函数执行效率可以更高;
 *				对齐程度越高,执行速度越快,具体效率如下:
 *				输入输出序列均为8B对齐>输入输出序列均为4B对齐>输入输出序列既存在4B对齐,也存在8B对齐
 ***************************************************************************************/
extern void Raco_Vec_C6678_Expression_05(float*fpIn, float*fpOut, int N, float fVal1, float fVal2, float fVal3, float fVal4);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_SeqGen_01(float*fpOut, int N, float fval1, float fval2, float fval3)
 *	函数功能:		序列数生成器1,公式Out=(i+fval1)*fval2+fval3
 *	算术公式表达:	Out[i]=(i+fval1)*fval2+fval3, i=0...N-1
 *	参数说明:		float*fpOut		:输出单精度实数序列起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *				float fval1		:相加常数fval1,单精度实常数
 *				float fval2		:相乘常数fval2,单精度实常数
 *				float fval3		:相加常数fval3,单精度实常数
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_SeqGen_01(float*fpOut, int N, float fval1, float fval2, float fval3);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_SeqGen_02(float*fpOut, int N, float fval1, float fval2)
 *	函数功能:		序列数生成器2,公式Out=(i+fval1)*(i+fval1)*fval2
 *	算术公式表达:	Out[i]=(i+fval1)*(i+fval1)*fval2, i=0...N-1
 *	参数说明:		float*fpOut		:输出单精度实数序列的起始地址,4B对齐
 *				int N			:处理点数N,N>0
 *				float fval1		:相加常数fval1,单精度实常数
 *				float fval2		:相乘常数fval2,单精度实常数
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_SeqGen_02(float*fpOut, int N, float fval1, float fval2);
/**********************Expression End**********************/


/**********************Middleware Start********************/
/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_CFAR_1D(float*fpIn, int*CFAR_Type, float*fpOut, int N, int Start_N, int Process_N, int Protect_N, float NoiseThr, int RefCell1, int RefCell2)
 *	函数功能:		一维CFAR检测
 *	算术公式表达:
 *	参数说明:		float*fpIn		:单精度实数处理序列的起始地址,
 *				int*CFAR_Type	:单精度实数参考序列的起始地址
 *								:CFAR_Type==0,该点参考单元长度为RefCell1
 *								:CFAR_Type==1,该点参考单元长度为RefCell2
 *				float*fpOut		:输出单精度实数序列的起始地址
 *				int N			:CFAR的总点数N,N>0
 *				int Start_N		:处理起始点数
 *				int Process_N	:处理点数
 *				int Protect_N	:保护单元的点数
 *				float NoiseThr	:噪声门限
 *				int RefCell1	:参考单元1的长度
 *				int RefCell2	:参考单元2的长度
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_CFAR_1D(float*fpIn, int*CFAR_Type, float*fpOut, int N, int Start_N, int Process_N, int Protect_N, float NoiseThr, int RefCell1, int RefCell2);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_PulseCompress_Serial(float*fpIn1, float*fpIn2, float*fpOut, int N, float*Twiddle, float*iTwiddle)
 *	函数功能:		对输入单精度复数序列进行脉冲压缩处理,单核串行版本
 *	算术公式表达:	Out=ifft( fft(In1) * In2 ) *为复乘
 *	参数说明:		float*fpIn1		:输入单精度复数处理序列首地址,8B对齐
 *				float*fpIn2		:输入单精度复数参考序列首地址,8B对齐
 *				float*fpOut		:输出单精度复数序列首地址,8B对齐
				int N			:处理点数N,8<=N<=131072,且N为2的整数次幂
				float*Twiddle	:FFT运算所需旋转因子的首地址,8B对齐
				float*iTwiddle	:IFFT运算所需旋转因子的首地址,8B对齐
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		在处理前,Twiddle和iTwiddle应该已经被计算完成;
 ***************************************************************************************/
extern void Raco_Vec_C6678_PulseCompress_Serial(float*fpIn1, float*fpIn2, float*fpOut, int N, float*Twiddle, float*iTwiddle);

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_PulseCompress_Parallel(float*fpIn1, float*fpIn2, float*fpOut, int N, float*Twiddle, float*iTwiddle)
 *	函数功能:		对输入单精度复数序列进行脉冲压缩处理,多核并行版本
 *	算术公式表达:	Out=ifft( fft(In1) * In2 ) *为复乘
 *	参数说明:		float*fpIn1				:输入单精度复数处理序列首地址,8B对齐
 *				float*fpIn2				:输入单精度复数参考序列首地址,8B对齐
 *				float*fpOut				:输出单精度复数序列首地址,8B对齐
				int N					:处理点数N,8<=N<=131072,且N为2的整数次幂
				float*Twiddle			:FFT运算所需旋转因子的首地址,8B对齐
				float*iTwiddle			:IFFT运算所需旋转因子的首地址,8B对齐
				unsigned int Multi_Info	:多核并行信息
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		在处理前,Twiddle和iTwiddle应该已经被计算完成;
 *				参与并行处理的核的信息由Multi_Info给出,对应如下:
 *					core7	core6	core5	core4	core3	core2	core1	core0
 *				0x	0		0		0		0		0		0		0		0
 *				如果该核参与并行处理,则将该核对应位置1;如果该核不参与并行处理,则将该核对应位置0;
 *				例如:
 *				八核并行处理		:SYN_Info=0x11111111
 *				0、2、4、6核并行处理	:SYN_Info=0x01010101
 *				1、3、4核并行处理	:SYN_Info=0x00011010
 *				如果不参与同步的核调用了该函数,则会直接跳出,不做任何处理;
 ***************************************************************************************/
extern void Raco_Vec_C6678_PulseCompress_Parallel(float*fpIn1, float*fpIn2, float*fpOut, int N, float*Twiddle, float*iTwiddle, unsigned int Multi_Info);
/**********************Middleware End**********************/


/**********************Function Start********************/
/***************************************************************************************
 *	函数名:		extern void Raco_Vec_C6678_SYN_MultiCore(unsigned int SYN_Info)
 *	函数功能:		片内多核同步
 *	算术公式表达:
 *	参数说明:		unsigned int SYN_Info	:多核同步信号
 *	调用核:		所有核均可自行调用
 *	返回值: 		void
 *	备注说明:		此函数可以实现片内任意核之间的同步;
 *				参与同步的核的信息由SYN_Info给出,对应如下:
 *					core7	core6	core5	core4	core3	core2	core1	core0
 *				0x	0		0		0		0		0		0		0		0
 *				如果该核参与同步,则将该核对应位置1;如果该核不参与同步,则将该核对应位置0;
 *				例如:
 *				八核同步			:SYN_Info=0x11111111
 *				0、2、4、6核同步	:SYN_Info=0x01010101
 *				1、3、4核同步		:SYN_Info=0x00011010
 *				如果不参与同步的核调用了该同步函数,则会直接跳出,不做任何处理;
 ***************************************************************************************/
extern void Raco_Vec_C6678_SYN_MultiCore(unsigned int SYN_Info);
/**********************Function   End********************/


/**********************Check	Start********************/
/**********************内部调用,用户无需关心********************/
extern far char Raco_Vec_C6678_Size;
extern far char Raco_Vec_C6678_Start;
static unsigned long long int Raco_Vec_C6678_Licence[]={0x10071007,
#include "Raco_Vec_C6678_Licence.lic"
};
extern int Raco_Vec_C6678_Get_Licence_I(unsigned int Core_Info, unsigned long long int*Licence, void*Code_Start, int Code_Length);
/**********************内部调用,用户无需关心********************/

/***************************************************************************************
 *	函数名:		void Raco_Vec_C6678_Printf_Version()
 *	函数功能:		打印当前矢量算法库版本信息
 *	参数说明:		无
 *	返回值: 		void
 *	备注说明:		无
 ***************************************************************************************/
extern void Raco_Vec_C6678_Printf_Version();

/***************************************************************************************
 *	函数名:		unsigned long long int Raco_Vec_C6678_Get_SN()
 *	函数功能:		获取当前DSP的序列号SN
 *	参数说明:		无
 *	返回值: 		unsigned long long int
 *	备注说明:		无
 ***************************************************************************************/
extern unsigned long long int Raco_Vec_C6678_Get_SN();

/***************************************************************************************
 *	函数名:		int Raco_Vec_C6678_Get_Licence(unsigned int Core_Info)
 *	函数功能:		获取矢量算法库使用权限
 *	参数说明:		unsigned int Core_Info				:八核中需要获取权限的核的信息
 *	调用核:		每一个需要获取权限的核均需调用此函数,不需要获取权限的核不需要调用
 *	返回值: 		int		:为1代表授权成功,为0代表授权失败
 *	备注说明:		通过调用此函数,可以获取高效运算软件的使用权限
 *				授权的成功与否取决于授权文件是否正确
 *				如果当前设备没有授权成功,在进入矢量算法库函数后会引发程序异常
 *				需要获取权限的核的信息由Core_Info给出,对应如下:
 *					core7	core6	core5	core4	core3	core2	core1	core0
 *				0x	0		0		0		0		0		0		0		0
 *				如果该核需要获取权限,则将该核对应位置1;如果该核不需要获取权限,则将该核对应位置0;
 *				例如:
 *				八核均获取权限		:Core_Info=0x11111111
 *				0、2、4、6核获取权限	:Core_Info=0x01010101
 *				1、3、4核获取权限	:Core_Info=0x00011010
 *				如果不需要获取权限的核调用了此函数,则会直接返回0;
 *				根据传入的核的信息,函数内部会进行一次多核同步,需要保证所有获取权限的核都运行此函数,才能够正常跳出;
 ***************************************************************************************/
#define Raco_Vec_C6678_Get_Licence(Core_Info) Raco_Vec_C6678_Get_Licence_I(Core_Info, Raco_Vec_C6678_Licence, (void*)&Raco_Vec_C6678_Start,(int)&Raco_Vec_C6678_Size)
/**********************Check	End********************/


#endif /* RACO_Vec_C6678_H_ */
