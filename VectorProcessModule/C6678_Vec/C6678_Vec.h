/*
 * my.h
 *
 *  Created on: 2012-2-20
 *      Author: GuoHongZhu
 *  modefied by Guohongzhu， 2013-11-16:
 *   1 超大点数FFT接口说明声明（原位计算和非原位计算）
 *   2 增加一个带插值计算的三角函数查表模块的接口声明
 *  			添加新lib函数
 *  modefied by Guohongzhu， 2013-11-27:
 *  更新了超大点数FFT的实现,本次只发布了非原位的计算，最大支持128K点的FFT
 */

#ifndef MY_H_
#define MY_H_

#include <ti/csl/tistdtypes.h>
#include "../../C6678_driver/c6678.h"

typedef struct complex
{
	float re;
	float im;
}COMPLEX_TYPE;
//说明:复数是2个float型组成,第一个float表示实部,第二个float表示虚部
//三角函数查表的表大小

extern const float exptable[16384+2];
extern const float cos_sin_table[16384+4];
/***************************************************************************************
 *	函数名:		void CplxMulCplx(float*pX, float*pY, float*pZ, unsigned int n)
 *	函数功能: 	实现两个复数相乘运算
 *	参数说明:	float*pX:第一个复数序列
 *		    	float*pY:第二个复数序列
 *		    	float*pZ:存放计算结果,实部在前,虚部在后,该地址可以覆盖pX或pY
 *		    	unsigned int n:复数点个数,需要满足:输入点数n>=12,且输入点数必须是4的倍数
 *	返回值: 		void
 ***************************************************************************************/
extern void Vec_CplxMulCplx(float*pX, float*pY, float*pZ, unsigned int n);


/***************************************************************************************
 *	函数名:		void CplxAddCplx(float*pX, float*pY, float*pZ, unsigned int n)
 *	函数功能: 	实现两个复数相加运算
 *	参数说明:	float*pX:第一个复数序列
 *		    	float*pY:第二个复数序列
 *		    	float*pZ:存放计算结果,实部在前,虚部在后,该地址可以覆盖pX或pY
 *		    	unsigned int n:复数点个数,需要满足:输入点数n>=12,且输入点数必须是4的倍数
 *	返回值: 		void
 ***************************************************************************************/
extern void Vec_CplxAddCplx(float*pX, float*pY, float*pZ, unsigned int n);


/***************************************************************************************
 *	函数名:		void CplxSubCplx(float*pX, float*pY, float*pZ, unsigned int n)
 *	函数功能: 	实现两个复数相减运算
 *	参数说明:	float*pX:第一个复数序列
 *		    	float*pY:第二个复数序列
 *		    	float*pZ:存放计算结果,实部在前,虚部在后,该地址可以覆盖pX或pY
 *		    	unsigned int n:复数点个数,需要满足:输入点数n>=12,且输入点数必须是4的倍数
 *	返回值: 		void
 ***************************************************************************************/
extern void Vec_CplxSubCplx(float*pX, float*pY, float*pZ, unsigned int n);


/***************************************************************************************
 *	函数名:		void CplxConj(float*pX, float*pY, unsigned int n)
 *	函数功能: 	实现复数共轭运算
 *	参数说明:	float*pX:输入复数序列
 *		    	float*pY:存放计算结果,实部在前,虚部在后,该地址可以覆盖pX
 *		    	unsigned int n:复数点个数,需要满足:输入点数n>=24,且输入点数必须是8的倍数
 *	返回值: 		void
 ***************************************************************************************/
extern void Vec_CplxConj(float*pX, float*pY, unsigned int n);


/***************************************************************************************
 *	函数名:		void CplxMulConst(float*pX, float*pY, float fVal, unsigned int n)
 *	函数功能: 	实现复数乘以常数运算
 *	参数说明:	float*pX:第一个复数序列
 *		    	float*pY:存放计算结果,实部在前,虚部在后,该地址可以覆盖pX
 *		    	float fVal: 和复数相乘的常数
 *		    	unsigned int n:复数点个数,需要满足:输入点数N>=24,且输入点数必须是8的倍数
 *	返回值: 		void
 ***************************************************************************************/
extern void Vec_CplxMulConst(float*pX, float*pY, float fVal, unsigned int n);


/***************************************************************************************
 *	函数名:		CplxAddConst(float*pX, float fVal, float*pZ, unsigned int n)
 *	函数功能: 	实现复数加常数运算
 *	参数说明:	float*pX:第一个复数序列
 *		    	float fVal: 和复数相加的常数
 *		    	float*pZ:存放计算结果,实部在前,虚部在后,该地址可以覆盖pX
 *		    	unsigned int n:复数点个数,需要满足:输入点数N>=24,且输入点数必须是8的倍数
 *	返回值: 		void
 ***************************************************************************************/
extern void Vec_CplxAddConst(float*pX, float fVal, float*pZ, unsigned int n);

/***************************************************************************************
 *	函数名:		CplxSubConst(float*pX, float fVal, float*pZ, unsigned int n)
 *	函数功能: 	实现复数减常数运算
 *	参数说明:	float*pX:输入的复数序列起始地址
 *		    	float fVal: 和复数相减的常数
 *		    	float*pZ:存放计算结果,实部在前,虚部在后,该地址可以覆盖pX
 *		    	unsigned int n:复数点个数,需要满足:输入点数N>=24,且输入点数必须是8的倍数
 *	返回值: 		void
 ***************************************************************************************/
extern void Vec_CplxSubConst(float*pX, float fVal, float*pZ, unsigned int n);


/***************************************************************************************
 *	函数名:		CplxMulVec(float*pX, float*pY, float*pZ, unsigned int n)
 *	函数功能: 	实现复数乘以向量运算,公式:Z[i] = X[i]*Y[i] + X[i+1]*Y[i];X[i]表示实部,X[i+1]表示虚部
 *	参数说明:	float*pX:输入的复数序列起始地址
 *		    	float*pY:向量的起始地址
 *		    	float*pZ:存放计算结果,实部在前,虚部在后,该地址可以覆盖pX
 *		    	unsigned int n:复数点个数,需要满足:输入点数N>=12,且输入点数必须是4的倍数
 *	返回值: 		void
 ***************************************************************************************/
extern void Vec_CplxMulVec(float*pX, float*pY, float*pZ, unsigned int n);


/***************************************************************************************
 *	函数名:		CplxPower2(float *pX, float*pY, unsigned int n)
 *	函数功能: 	实现复数平方,公式:Y[i] = X[i]*X[i] + X[i+1]*X[i+1];X[i]表示实部,X[i+1]表示虚部
 *	参数说明:	float*pX:输入的复数序列起始地址
 *		    	float*pY:计算结果存放起始地址
 *		    	unsigned int n:复数点个数,需要满足:输入点数N>=24,且输入点数必须是8的倍数
 *	返回值: 		void
 ***************************************************************************************/
extern void Vec_CplxPower2(float *pX, float*pY, unsigned int n);


/***************************************************************************************
 *	函数名:		UncharToFloattest(unsigned char*pX, float *pW, unsigned int nLen)
 *	函数功能: 	实现unsigned char 转换成 float型
 *	参数说明:	unsigned char*pX:输入的unsigned char序列起始地址
 *		    	float*pW:计算结果存放起始地址
 *		    	unsigned int nLen:复数点个数,需要满足:输入点数nLen>=24,且输入点数必须是8的倍数
 *	返回值: 		void
 ***************************************************************************************/
extern void Vec_UncharToFloat(unsigned char*pX, float *pW, unsigned int nLen);


/***************************************************************************************
 *	函数名:		VectorSum(float *pX, unsigned int nLen)
 *	函数功能: 	对一个float型数组的所有元素求和运算,并输出求和结果
 *	参数说明:	float *pX:输入的向量序列起始地址
 *		    	unsigned int nLen:输入float数据的长度,需要满足:数据长度必须大于16,且必须是8的整数倍
 *	返回值: 		float
 ***************************************************************************************/
extern float Vec_VectorSum(float *pX, unsigned int nLen);


/***************************************************************************************
 *	函数名:		CplxSum(float *pX, unsigned int nLen, float*pY)
 *	函数功能: 	复数数组的求和运算,并输出求和结果
 *	参数说明:	float *pX:输入的复数序列起始地址
 *		    	unsigned int nLen:输入复数点的个数,需要满足:数据长度必须大于8,且必须是4的整数倍
 *		    	float*pY:计算的结果存放地址
 *	返回值: 		float
 ***************************************************************************************/
extern void Vec_CplxSum(float *pX, unsigned int nLen, float*pY);


/***************************************************************************************
 *	函数名:		Cos_Sin_LookUp_Table_CplxMul(float*InPut, float*OutPut, float*pCos_Sin_Table, int InPutLen, int nPortion)
 *	函数功能: 	对输入的数组通过三角函数查表，计算出相应的cos和sin值，然后和该数组进行复数乘法
 *	参数说明:	float *pX:输入的复数序列起始地址
 *				float*OutPut：输出结果起始地址
 *				float*pCos_Sin_Table:生成的三角函数表的地址,exptable
 *		    	int InPutLen:输入复数点的个数,需要满足:数据长度必须大于8,且必须是2的整数倍
 *		    	int nPortion:将0-2PI区域nPortion等份
 *	返回值: 		void
 ***************************************************************************************/
extern void Vec_Cos_Sin_LookUp_Table_CplxMul(float*InPut, float*OutPut, float*pCos_Sin_Table, int InPutLen, int nPortion);

/***************************************************************************************
 *	函数名:		FloatSqrtVec(float *pX, float *pY, unsigned int nLen)
 *	函数功能: 	单精度向量开方函数;
 *	参数说明:	float *pX:表示输入数组的起始地址,要求输入的浮点数据序列所有数据必须为大于等于0.0的浮点数
 *				float *pY:表示计算的结果存放的起始地址,要求输出地址空间大小与输入空间大小一致;
 *		    	unsigned int nLen:表示输入的点数;需要满足:输入点数N>=16,且必须是8的倍数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_FloatSqrtVec(float *pX, float *pY, unsigned int nLen);

/***************************************************************************************
 *	函数名:		FloatAbsVec(float *pX, float *pY, unsigned int nLen)
 *	函数功能: 	向量求绝对值函数;
 *	参数说明:	float *pX:表示输入数组的起始地址;
 *				float *pY:表示计算的结果存放的起始地址;
 *		    	unsigned int nLen:表示输入的点数;需要满足:输入点数N>=16,且输入点数必须是8的倍数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_FloatAbsVec(float *pX, float *pY, unsigned int nLen);

/***************************************************************************************
 *	函数名:		MemCopy(void * src,void * dst,unsigned int nLen);
 *	函数功能: 	连续内存拷贝函数;
 *	参数说明:	void * src:表示源数组的起始地址 src;
 *				void * dst:表示目的地址的起始地址 dst；
 *		    	unsigned int nLen:表示需要拷贝的字节数;需要满足:输入点数nLen>=16*8,且输入字节数必须是32的倍数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_MemCopy(void * src,void * dst,unsigned int nLen);
/***************************************************************************************
 *	函数名:void Memoryset(float*pInPut, float*pOutPut,int n)
 *	函数功能: 对应于C语言的memset ，连写写入8个字节，即2个FLOAT数据
 *	参数说明:
 *	1.	float*pInPut:需要连续拷贝的8个字节，2个字
 *  2.	float*pOutPut：输出数据
 *  3.  int n：字数，必须是8的倍数
 *	返回值: 	无
 ***************************************************************************************/
extern void Vec_Memoryset(float*pInPut, float*pOutPut,int n);
/***************************************************************************************
 *	函数名:		MemCopyStride(float * src,float * dst,unsigned int nLen, unsigned int stride);
 *	函数功能: 	跳变内存拷贝函数;
 *	参数说明:	void * src:表示源数组的起始地址 src;
 *				void * dst:表示目的地址的起始地址 dst；
 *		    	unsigned int nLen:表示需要表示拷贝的个数(按字算32位);需要满足:输入点数N>=16,且输入点数必须是8的倍数
 *				unsigned int stride:拷贝的跳变个数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_MemCopyStride(float * src,float * dst,unsigned int nLen, unsigned int stride);

/***************************************************************************************
 *	函数名:		Linspace(float First,float Last,unsigned int nLen,float *pOut);
 *	函数功能: 	实现matlab中linspace()函数;
 *	参数说明:	float First:产生数组的第一个值
 *				float Last:产生数组的最后一个值
 *		    	unsigned int nLen:产生数组的个数;需要满足:nLen>=12，且是4的倍数
 *				float *pOut:产生数组的输出首地址
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_Linspace(float First,float Last,unsigned int nLen,float *pOut);

/***************************************************************************************
 *	函数名:		void f32_bit8_Ext(void *buf1, void *buf2,unsigned int nl, float fstep, float fmin);
 *	函数功能: 	削峰量化函数;
 *	参数说明:	void *buf1:输入数组的起始地址，要求调用该函数前对输入数据做求绝对值处理，保证输入数据不会为负数；
 *				void *buf2:输出地址的起始地址；
 *		    	unsigned int nl:产生数组的个数;需要满足:要求为>=24，且是8的倍数。
 *				float fstep:产生数组的输出首地址，fstep =255/(max*0.9-min);max为量化输入数组的最大值；
 *				float fmin：量化数据的最小值
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_f32_bit8_Ext(void *buf1, void *buf2,unsigned int nl, float fstep, float fmin);
/***************************************************************************************
 *	函数名:	Vec_f32_bit16(float* pIn,short * pOut,float fMax,unsigned int Num);
 *	函数功能:  实现对输入数组利用输入门限量化
 *  参数说明:  pIn:输入float数据指针
 *          pOut:输出int16数据指针
 *          fMax:量化门限
 *          Num:数据长度，float型数据长度，如是实数则是点数，如是复数则是复数点数的2倍
 *  公式:    int16（pIn/fMax*32767）
 *	返回值: 	无
 ***************************************************************************************/
extern void Vec_f32_bit16(float* pIn,short * pOut,float fMax,unsigned int Num);
/***************************************************************************************
 *	函数名:		float Mat_sp_maxval(float *x,int row,int col )
 *	函数功能: 	实现矩阵求最大值
 *	参数说明:	float*x:矩阵序列
 *		    	int row:矩阵的行数
 *		    	int col：矩阵的列数   row*col必须是4的倍数且 >= 4
 *	返回值: 		矩阵最大值
 ***************************************************************************************/

extern float Mat_sp_maxval(float *x,int row,int col );
/***************************************************************************************
 *	函数名:		float  Mat_sp_minval(float *x,int row,int col )
 *	函数功能: 	实现矩阵求最小值
 *	参数说明:	float*x:矩阵序列
 *		    	int row:矩阵的行数
 *		    	int col：矩阵的列数   row*col必须是4的倍数且 >= 4
 *	返回值: 		矩阵最小值
 ***************************************************************************************/
extern float Mat_sp_minval(float *x,int row,int col );
/***************************************************************************************
 *	函数名:Vec_mat_max(float*pInPut, int nPointNum,float*Value);
 *	函数功能: 查找向量中的最大值以及其位置
 *	参数说明:
 *	1.	float*pInPut:输入向量
 *  2.	int nPointNum：点数，必须是4的倍数
 *  3.  float*Value：最大值存放地址
 *	返回值: 	最大值位置
 ***************************************************************************************/
extern int Vec_mat_max(float*pInPut, int nPointNum,float*Value);
/***************************************************************************************
 *	函数名:		float Vec_ave(float *p,float n)
 *	函数功能: 	实现向量均值
 *	参数说明:	float*x:向量序列
 *		    	float n：点数n必须是4的倍数且 >= 4
 *	返回值: 		向量均值
 ***************************************************************************************/
extern float Vec_ave(float *p,float n);
/***************************************************************************************
 *	函数名:		float standard_sa_c(float*pX, float n)
 *	函数功能: 	实现向量求标准差
 *	参数说明:	float*pX:向量序列
 *		    	float n：输入向量的点数
 *	返回值: 		向量均值
 ***************************************************************************************/
extern float standard_sa_c(float*pX, float n);
/***************************************************************************************
 *	函数名:		void Mat_Mul(float*X1,float*X2,int r,int c,float *Y)
 *	函数功能: 	实现实数矩阵点乘
 *	参数说明:	float*X1:输入第一个点乘矩阵
 *		    	float*X2：输入第二个点乘矩阵
 *		    	int r：点乘矩阵的行数，r必须是2的倍数行并且大于等于2
 *		    	int c：点乘矩阵的列数，c必须是2的倍数行并且大于等于2
 *		    	float *Y：输出计算结果的矩阵
 *	返回值: 		无
 ***************************************************************************************/
extern void Mat_Mul(float*X1,float*X2,int r,int c,float *Y);
/***************************************************************************************
 *	函数名:		void Mat_Mul_Cplx(float*X1,float*X2,int r,int c,float *Y))
 *	函数功能: 	实现复数数矩阵点乘
 *	参数说明:	float*X1:输入第一个点乘复数矩阵
 *		    	float*X2：输入第二个点乘复数矩阵
 *		    	int r：点乘矩阵的行点数，r必须是2的倍数行并且大于等于2
 *		    	int c：点乘矩阵的列点数，c必须是2的倍数行并且大于等于2
 *		    	float *Y：输出计算结果的矩阵
 *	返回值: 		无
 ***************************************************************************************/
extern void Mat_Mul_Cplx(float*X1,float*X2,int r,int c,float *Y);
/***************************************************************************************
 *	函数名:Vec_cplxmulmat(float*X1,int r,int c,float*X2,float *Y);
 *	函数功能: 复数矩阵乘一个复数常量
 *	参数说明:
 *	1.	float*X1:复数常量地址
 *  2.	int r：输入复数矩阵行数
 *  3.  int c：输入复数矩阵列数
 *  4.  float*X2：输入复数矩阵
 *  5.  float*  y：输出矩阵
 *	返回值: 	无
 ***************************************************************************************/
extern void Vec_cplxmulconstmat(float*X1,int r,int c,float*X2,float *Y);
/***************************************************************************************
 *	函数名:		void Mat_add_cplx(float* x1,float* x2,float *Y,int r1, int c1)
 *	函数功能: 	实现复数矩阵相加
 *	参数说明:	float*x1:输入第一个复数矩阵
 *		    	float*x2：输入第二个复数矩阵
 *		    	float *Y：输出计算结果的矩阵
 *		    	int r1：矩阵的行点数，r1必须是2的倍数行并且大于等于2
 *		    	int c1：矩阵的列点数，c1必须是2的倍数行并且大于等于2
 *
 *	返回值: 		无
 ***************************************************************************************/

extern void Mat_add_cplx(float* x1,float* x2,float *Y,int r1, int c1);

/***************************************************************************************
 *	函数名:		void Mat_add(float* x1,float* x2,float *Y,int r1, int c1)
 *	函数功能: 	实现复数矩阵相加
 *	参数说明:	float*x1:输入第一个点乘复数矩阵
 *		    	float*x2：输入第二个点乘复数矩阵
 *		    	float *Y：输出计算结果的矩阵
 *		    	int r1：矩阵的行点数，r1必须是2的倍数行并且大于等于2
 *		    	int c1：矩阵的列点数，c1必须是2的倍数行并且大于等于2
 *
 *	返回值: 		无
 ***************************************************************************************/
extern void Mat_add(float* x1,float* x2,float *Y,int r1, int c1);//

/***************************************************************************************
 *	函数名:		double Vec_mu_dp(double* x,int nx)
 *	函数功能: 	实现向量求模双精度求模
 *	参数说明:	float*x:输入向量
 *		    	int nx：输入向量的点数，nx点数为2的倍数
 *	返回值: 		模值
 ***************************************************************************************/
extern double Vec_mu_dp(double* x,int nx);
/***************************************************************************************
 *	函数名:		float Vec_mu_sp(float* x,int nx)
 *	函数功能: 	实现向量求模单精度求模
 *	参数说明:	float*x:输入向量
 *		    	int nx：输入向量的点数，nx点数为2的倍数
 *	返回值: 		模值
 ***************************************************************************************/
extern float Vec_mu_sp(float* x,int nx);
/***************************************************************************************
 *	函数名:		int Mat_sp_inv(float *a, int n)
 *	函数功能: 	实现实数矩阵求逆
 *	参数说明:	float*a:输入矩阵		求逆的结果矩阵存放在a
 *		    	int n：输入矩阵的维数
 *	返回值: 		输出1：矩阵能够求逆 		输出0：矩阵不能求逆
 ***************************************************************************************/
extern int Mat_sp_inv(float *a, int n);  
/***************************************************************************************
 *	函数名:		int Mat_inv_cplx(float *pInPut,int n,int *is,int *js,float *frow,float *fcol);
 *	函数功能: 	实现复数矩阵求逆
 *	参数说明:
 *	1.	float *pInPut:输入复数矩阵		求逆的结果矩阵存放在pInPut
 *  2.	int n：输入复数矩阵的维数
 *  3.	int *is,int *js，中间变量存放空间，空间需大于复数矩阵点数
 *  4.	float *frow,float *fcol中间变量存放空间，空间需大于复数矩阵点数
 *	返回值: 		输出1：矩阵能够求逆 		输出0：矩阵不能求逆
 ***************************************************************************************/
extern int Mat_invc_cplx( float *pInPut,int n,int *is,int *js,float *frow,float *fcol);

/***************************************************************************************
 *	函数名:		void Vec_Split(float*X1,float*pY1,float*pY2,int n)
 *	函数功能: 	实现实数向量 奇 偶拆分
 *	参数说明:	float*X1:输入实数向量
 *				float*pY1：奇数数组输出
 *				float*pY2：偶数数组输出
 *		    	int n：输入向量点数必须大于等于8且为8的倍数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_Split(float*X1,float*pY1,float*pY2,int n);
/***************************************************************************************
 *	函数名:		void Vec_Split_Cplx(float*X1,float*pY1,float*pY2,int n)
 *	函数功能: 	实现复数向量 奇 偶拆分
 *	参数说明:	float*X1:输入实数向量
 *				float*pY1：奇数数组输出
 *				float*pY2：偶数数组输出
 *		    	int n：输入向量复数个数必须大于等于4且为4的倍数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_Split_Cplx(float*X1,float*pY1,float*pY2,int n);
/***************************************************************************************
 *	函数名:		void Vec_Comb(float*X1,float*X2,float*pY,int n)
 *	函数功能: 	实现实数向量 奇 偶合并
 *	参数说明:	float*X1:输入偶数数组
 *				float*X2：输入奇数数组
 *				float*pY：数组输出
 *		    	int n：输入向量奇数个数必须大于等于2且为2的倍数	 奇数数组和偶数数组个数必须相同
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_Comb(float*X1,float*X2,float*pY,int n);
/***************************************************************************************
 *	函数名:		void Vec_Comb(float*X1,float*X2,float*pY,int n)
 *	函数功能: 	实现复数向量 奇 偶合并
 *	参数说明:	float*X1:输入偶数数组
 *				float*X2：输入奇数数组
 *				float*pY：数组输出
 *		    	int n：输入向量奇数个数必须大于等于2且为2的倍数	 奇数数组和偶数数组个数必须相同
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_Comb_Cplx(float*X1,float*X2,float*pY,int n);

/***************************************************************************************
 *	函数名:		void Vec_cos_lookup_table(float* X,float *Y,int n,const float *table,float  table_num)
 *	函数功能: 	实现复数向量 求COS
 *	参数说明:	float* X:输入数组
 *				float *Y：输出结果数组
 *				int n：输入数组的点数，n大于2且为2的倍数
 *				const float *table：查表地址,使用16384点的exptable表
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_cos_lookup_table(float* X,float *Y,int n,const float *table,float  table_num);
/***************************************************************************************
 *	函数名:		void Vec_sin_lookup_table(float* X,float *Y,int n,const float *table,float  table_num)
 *	函数功能: 	实现复数向量 求sin
 *	参数说明:	float* X:输入数组
 *				float *Y：输出结果数组
 *				int n：输入数组的点数，n大于2且为2的倍数
 *				const float *table：查表地址 ,使用16384点的exptable表
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_sin_lookup_table(float* X,float *Y,int n,const float *table,float  table_num);

/***************************************************************************************
 *	函数名:		void Vec_sin_cos(float* X,float *Y,int n,const float *table,float  table_num)
 *	函数功能: 	实现复数向量 求sin和cos
 *	参数说明:	float* X:输入数组   为弧度值X/2PI
 *				float *Y：输出结果数组 存放第一个是cos值，第二个是sin值
 *				int n：输入数组的点数，n大于2且为2的倍数
 *				const float *table：查表地址，使用16384点的exptable表
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_sin_cos(float* X,float *Y,int n,const float *table,float  table_num);
/***************************************************************************************
 *	函数名:		void Hamming(float* Y,int n,const float *table,float  table_num)
 *	函数功能: 	实现汉明窗
 *	参数说明:	float *Y：输出结果数组
 *				int n：汉明窗的长度  偶数长度
 *				const float *table：计算COS查表地址 ，使用16384点的exptable表
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 ***************************************************************************************/
extern void Hamming(float* Y,int n,const float *table,float  table_num);
/***************************************************************************************
 *	函数名:		void Vec_cos(float* X,float *Y,int n,const float *table,float  table_num)
 *	函数功能: 	实现复数向量 求COS,优化版本，精度稍差
 *	参数说明:	float* X:输入数组
 *				float *Y：输出结果数组
 *				int n：输入数组的点数，n大于2且为2的倍数
 *				const float *table：查表地址,使用8192点的cos_sin_table表
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_cos(float* X,float *Y,int n,const float *table,float  table_num);
/***************************************************************************************
 *	函数名:		void Vec_sin(float* X,float *Y,int n,const float *table,float  table_num)
 *	函数功能: 	实现复数向量 求sin，,优化版本，精度稍差
 *	参数说明:	float* X:输入数组
 *				float *Y：输出结果数组
 *				int n：输入数组的点数，n大于2且为2的倍数
 *				const float *table：查表地址    使用8192点的cos_sin_table表
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_sin(float* X,float *Y,int n,const float *table,float  table_num);
/***************************************************************************************
 *	函数名:		void Vec_sin_cos1(float* X,float *Y,int n,const float *table,float  table_num)
 *	函数功能: 	实现复数向量 求sin和cos，,优化版本，精度稍差
 *	参数说明:	float* X:输入数组   为弧度值X/2PI
 *				float *Y：输出结果数组 存放第一个是cos值，第二个是sin值
 *				int n：输入数组的点数，n大于2且为2的倍数
 *				const float *table：查表地址   ,使用8192点的cos_sin_table表
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_sin_cos1(float* X,float *Y,int n,const float *table,float  table_num);

/***************************************************************************************
 *	函数名:		void Hamming1(float* Y,int n,const float *table,float  table_num)
 *	函数功能: 	实现汉明窗,优化版本，精度稍差
 *	参数说明:	float *Y：输出结果数组
 *				int n：汉明窗的长度  偶数长度
 *				const float *table：计算COS查表地址 ,使用8192点的cos_sin_table表
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 ***************************************************************************************/
extern void Hamming1(float* Y,int n,const float *table,float  table_num);
/***************************************************************************************
 *	函数名:		void Mat_extract(float* X,int r_s,int c_s,int row_num,int r1,int c1,float* Y)
 *	函数功能: 	二维矩阵提取
 *	参数说明:	float *X：原始矩阵
 *				int r_s：提取原始矩阵的起始行点数
 *				int c_s：提取原始矩阵的起始列点数
 *				int row_num：提取原始矩阵行点数
 *				int r1：提取的行点数 
 *				int c1：提取的列点数
 *				float* Y：输出矩阵
 *	返回值: 		无
 ***************************************************************************************/
extern void Mat_extract(float* X,int r_s,int c_s,int row_num,int r1,int c1,float* Y);
/***************************************************************************************
 *	函数名:		 Cfar(float* input,int *output,int  N,int G, int L, float coeff, float thr );
 *	函数功能: 	恒虚警率接收机
 *	参数说明:	float* input：输入序列
 *				int *output：输出检测到目标的位置的序列
 *				 N：输入序列长度
 *				 G：保护单元长度
 *				 L：参考单元长度
 *				coeff：加权系数
 *			    thr：门限大小
 *	返回值: 		无
 ***************************************************************************************/
extern void Cfar(float* input,int *output,int  N,
		int G, int L, float coeff, float thr );

/***************************************************************************************
 *	函数名:		 void Vec_ShortToFloat(short *pX,float *pY,int n);
 *	函数功能: 	16位有符号数据转换为浮点数
 *	参数说明:	short *pX：输入向量
 *				float *pY：输出数据向量
 *				int n：向量点数   向量点数为2的倍数
 *				
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_ShortToFloat(short *pX,float *pY,int n);
/***************************************************************************************
 *	函数名:		 Vec_UnShortToFloat(short *pX,float *pY,int n);
 *	函数功能: 	16位无符号数据转换为浮点数
 *	参数说明:	unsigned short *pX：输入向量
 *				float *pY：输出数据向量
 *				int n：向量点数   向量点数为2的倍数
 *				
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_UnShortToFloat(unsigned short *pX,float *pY,int n);
/***************************************************************************************
 *	函数名:		Vec_cplxfftshift(float*pInPut,int r,float*pOutPut);
 *	函数功能: 	实现matlab中fftshift()函数;
 *	参数说明:	float*pInPut：输入复数数组
 * 	            int  r：输入点数，为4的倍数
 *	            float*pOutPu：输出复数数组
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_cplxfftshift(float*pInPut,int r,float*pOutPut);

/***************************************************************************************
 *	函数名:		 ReferenceGenerate( float fFdc,
									float fFdr,
									float fPluseband,
									float fFs,
									float *pfWindow,
									int   iMulWindflag,
									int   iPoint,
									float *pfTable,
									float   fTabNum,
									float *tempBuf,
									float *pfOut
									);
 *	函数功能: 	生成参考函数操作函数（chirp函数）
 *	参数说明:	fFdc： 参考函数的多普勒中心频率		
				fFdr：调频斜率
				fPluseband：脉宽     
				fFs： 采样频率
				pfWindow：要加的窗函数首地址
				iMulWindflag：是否乘窗函数，如果为0表示不乘窗函数，为1表示乘窗函数
				iPoint：采样点数
				pfTable ：cos计算查表地址
				fTabNum：查表的大小
				tempBuf： 缓存数组大小为iPoint
				pfOut：   参考函数输出结果


 *	返回值: 		无
 ***************************************************************************************/
extern void ReferenceGenerate( float fFdc,
						float fFdr,
						float fPluseband,
						float fFs,
						float *pfWindow,
						int   iMulWindflag,
						int   iPoint,
						float *pfTable,
						float   fTabNum,
						float *tempBuf,
						float *pfOut
				  		);
/***************************************************************************************
 *	函数名:		void PulseCompress(
					float  *cf_PRTData,          
					float  *cf_PCReference,		 
					float  *cf_PCResult,		
					float  *tempBuf,             
					float  *cf_Twiddle,			
					float  *cf_iTwiddle,		
					int    i_PCPoint			 //脉压的处理点数
				  );
 *	函数功能: 	脉冲压缩处理
 *	参数说明:	
1.	cf_PRTData：输入数据的首地址	
2.	cf_PCReference：参考函数首地址	
3.	cf_PCResult：输出脉压结果
4.	tempBuf： 临时存储空间    大小为脉压的处理点数的2倍
5.	cf_Twiddle：fft旋转因子首地址 输出值大小为脉压的处理点数的2倍
6.	cf_iTwiddle：ifft旋转因子首地址 输出值大小为脉压的处理点数的2倍
7.	i_PCPoint：脉压的处理点数

 *	返回值: 		无
 ***************************************************************************************/						
extern void PulseCompress(
					float  *cf_PRTData,         
					float  *cf_PCReference,		
					float  *cf_PCResult,		
					float  *tempBuf,           
					float  *cf_Twiddle,			 
					float  *cf_iTwiddle,		
					int    i_PCPoint			 
				  );

//////////////////////////////////////////////////
//typedef struct {
//	Uint32   CC[8];
//	Uint32   Chan_in[8];
//	Uint32   Para_in[8];
//	Uint32	 TCNum[8];
//	Uint32	 RegionNum[8];
//}   EDMA_Channel;			//EDMA参数初始化，存放控制器、通道号、参数号

typedef struct _EDMA_Channel
{
	Uint32   CC;//EDMA控制器
	Uint32   Chan_in;//EDMA通道号
	Uint32   Para_in;//传输参数PARAM号
	Uint32	 TCNum;//传输码
	Uint32	 RegionNum;//区域号
}EDMA_Channel;

typedef enum {
	vlfft_2k = 11,
	vlfft_4k = 12,
	vlfft_8k = 13,
	vlfft_16k = 14,
	vlfft_32k = 15,
	vlfft_64k = 16,
	vlfft_128k = 17,
	vlfft_256k = 18
//	vlfft_512k = 19,
//	vlfft_1024k = 20
}FFT_POINTNUM;   //各变量的值正好是点数取底2的对数，这对接下来的参数计算有好处
/***************************************************************************************
 *	函数名:	void vlfft_tw_gen(float *w, int n);
 *	函数功能: 产生FFT的碟形因子
 *	参数说明:	float*pInPut：碟形因子存放地址
 * 	        int  n：点数
 *	返回值: 		无
 ***************************************************************************************/
extern void vlfft_tw_gen(float *w, int n);
/***************************************************************************************
 *	函数名:	void vlfft_itw_gen(float *w, int n);
 *	函数功能: 产生IFFT的碟形因子
 *	参数说明:	float*pInPut：碟形因子存放地址
 * 	        int  n：点数
 *	返回值: 		无
 ***************************************************************************************/
extern void vlfft_itw_gen(float *w, int n);
/***************************************************************************************
 *	函数名:	void vlfft_genTwiddle(float *vlfft_widdle,int numFFTsPerCore,int coreNum,FFT_POINTNUM pointnum));
 *	函数功能:  产生二维FFT的铰链因子
 *	参数说明:	float*vlfft_widdle：铰链因子存放地址
 * 	        int  numFFTsPerCore：每个核计算FFT的点数=N2/参与计算的核数;
 * 	        int coreNum ：核号
 * 	        FFT_POINTNUM pointnum：计算FFT点数N的2的指数（目前支持vlfft_2k~vlfft_256k）
 *
 *	返回值: 		无
 ***************************************************************************************/
extern void vlfft_genTwiddle(float *vlfft_widdle,int numFFTsPerCore,int coreNum,FFT_POINTNUM pointnum);
/***************************************************************************************
 *	函数名:	void vlfft_genITwiddle(float *vlfft_widdle,int numFFTsPerCore,int coreNum,FFT_POINTNUM pointnum));
 *	函数功能:  产生二维IFFT的铰链因子
 *	参数说明:	float*vlfft_widdle：铰链因子存放地址
 * 	        int  numFFTsPerCore：每个核计算FFT的点数=N2/参与计算的核数;
 * 	        int coreNum ：核号
 * 	        FFT_POINTNUM pointnum：计算FFT点数N的2的指数（目前支持vlfft_2k~vlfft_256k）
 *
 *	返回值: 		无
 ***************************************************************************************/
extern void vlfft_genITwiddle(float	*vlfft_widdle,int numFFTsPerCore,int coreNum,FFT_POINTNUM pointnum);

/***************************************************************************************
 *	函数名:		void vlfft_compute_notHomePos(float  *pInData,
						float 			*pOutData,
						FFT_POINTNUM 	pointnum,
						EDMA_Channel 	edma_channel,
						float 			*vlfft_wcolfft,
						float 			*vlfft_wrowfft,
						float 			*vlfft_twiddle,
						float			*computeBuf_1,
						float			*computeBuf_2,
						float			*fftBuf,
						int 			coreNum,
						int             mastercore,
						int 			mode,
						int             LOG2_NUM_OF_CORES_FOR_COMPUTE);
 *	函数功能: 	单片8核实现超长点数FFT以及IFFT（目前最大128k点），非原位计算
 *	参数说明:
1.  pInData: 原始数据输入以及计算结果输出地址。要求数据空间连续并字              节对齐。（位于共享内存MSMC，大小为计算点数实际占用 空间大小）
2.  pOutData: 各核计算临时缓存。（位于共享内存MSMC，大小为计算点数实际占用 空间大小）
3.  pointnum：计算点数N的2的指数（目前支持vlfft_2k~vlfft_256k）
4.  edma：EDMA初始化后通道（初始化方式见示例程序）
5.  vlfft_wcolfft：列FFT或列IFFT的蝶形因子，占用空间大小为8*N1字节，位于各核独立内存L2
6.  vlfft_wrowfft：行FFT或行IFFT的蝶形因子，占用空间大小为8*N2字节，位于各核独立内存L2
7.  vlfft_twiddle：旋转因子，占用空间大小为8*N2字节，位于各核独立内存L2
8.  computeBuf_1：各核计算临时缓存，大小为128* N1个字节，位于各核独立内存L2
9.  computeBuf_2；各核计算临时缓存，大小为128* N1个字节，位于各核独立内存L2，
10. fftBuf：大小可设为0
11. coreNum：相对核号，如核3和核4两核处理时，两个核分别输入0，1
12. mastercore：参与计算核中最小核号的绝对核号，如核3和核4两核处理时，两个核都输入3
13. mode：模式，0：aft，1：ifft
14. LOG2_NUM_OF_CORES_FOR_COMPUTE：参与计算总核数2的指数
 *	返回值: 		无
 ***************************************************************************************/
extern void vlfft_compute_notHomePos(float  *pInData,
				float 			*pOutData,
				FFT_POINTNUM 	pointnum,
				EDMA_Channel 	edma_channel,
				float 			*vlfft_wcolfft,
				float 			*vlfft_wrowfft,
				float 			*vlfft_twiddle,
				float			*computeBuf_1,
				float			*computeBuf_2,
				float			*fftBuf,
				int 			coreNum,
				int             mastercore,
				int 			mode,
				int             LOG2_NUM_OF_CORES_FOR_COMPUTE
);


/***************************************************************************************
 *	函数名:	void vlfft_compute_HomePos(float *InOutData,
					FFT_POINTNUM 	pointnum,
					EDMA_Channel 	edma_channel,
					float 			*vlfft_wcolfft,
					float 			*vlfft_wrowfft,
					float 			*vlfft_twiddle,
					float			*computeBuf_1,
					float			*computeBuf_2,
					float			*fftBuf,
					int 			coreNum,
					int             mastercore,
					int 			mode,
					int             LOG2_NUM_OF_CORES_FOR_COMPUTE
);
									float *outBuf);
 *	函数功能: 	单片8核实现超长点数FFT以及IFFT（目前最大256k点），原位计算
 *	参数说明:
1.  pinoutData : 原始数据输入以及计算结果输出地址。要求数据空间连续并字              节对齐。（位于共享内存MSMC，大小为计算点数实际占用 空间大小）
2.  pointnum：计算点数N的2的指数（目前支持vlfft_2k~vlfft_256k）
3.  edma：EDMA初始化后通道（初始化方式见示例程序）
4.  vlfft_wcolfft：列FFT或列IFFT的蝶形因子，占用空间大小为8*N1字节，位于各核独立内存L2
5.  vlfft_wrowfft：行FFT或行IFFT的蝶形因子，占用空间大小为8*N2字节，位于各核独立内存L2
6.  vlfft_twiddle：旋转因子，占用空间大小为8*N2字节，位于各核独立内存L2
7.  computeBuf_1：各核计算临时缓存，大小为max（4*N/Corenum，128* N1）个字节，位于各核独立内存L2
8.  computeBuf_2；各核计算临时缓存，大小为max（4*N/Corenum，128* N1）个字节，其中N为总复数点数，Corenum为参与计算的核数，位于各核独立内存L2，并且与computeBuf_1连续存放
9.  fftBuf：各核计算临时缓存，大小为16*N2*8个字节，位于各核独立内存L2
10. coreNum：相对核号，如核3和核4两核处理时，两个核分别输入0，1
11. mastercore：参与计算核中最小核号的绝对核号，如核3和核4两核处理时，两个核都输入3
12. mode：模式，0：fft，1：ifft
13. LOG2_NUM_OF_CORES_FOR_COMPUTE：参与计算总核数2的指数

 *	返回值: 		无
 ***************************************************************************************/
extern void vlfft_compute_HomePos(float *InOutData,
	    FFT_POINTNUM 	pointnum,
		EDMA_Channel 	edma_channel,
		float 			*vlfft_wcolfft,
		float 			*vlfft_wrowfft,
		float 			*vlfft_twiddle,
		float			*computeBuf_1,
		float			*computeBuf_2,
		float			*fftBuf,
		int 			coreNum,
		int             mastercore,
		int 			mode,
		int             LOG2_NUM_OF_CORES_FOR_COMPUTE
);
/***************************************************************************************
 *	函数名:		void Vec_sin_cos_polatio(float* X,float *Y,int n,const float *table,float  table_num)
 *	函数功能: 	实现复数向量 求sin和cos
 *	参数说明:	float* X:输入数组   为弧度值X/2PI
 *				float *Y：输出结果数组 存放第一个是cos值，第二个是sin值
 *				int n：输入数组的点数，n大于2且为2的倍数
 *				const float *table：查表地址  ,，使用16384点的exptable表
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_sin_cos_polatio(float* X,float *Y,int n,const float *table,float  table_num);
/***************************************************************************************
 *	函数名:		void Vec_sin_cos_polatio1(float* X,float *Y,int n,const float *table,float  table_num)
 *	函数功能: 	实现复数向量 求sin和cos,优化版本，精度稍差
 *	参数说明:	float* X:输入数组   为弧度值X/2PI
 *				float *Y：输出结果数组 存放第一个是cos值，第二个是sin值
 *				int n：输入数组的点数，n大于2且为2的倍数
 *				const float *table：查表地址 ,，使用8192点的cos_sin_table表
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_sin_cos_polatio1(float* X,float *Y,int n,const float *table,float  table_num);

/***************************************************************************************
 *	函数名:		void Vec_LinearAdd_Mul(float pa,float pb,int pn,float *pcOut)
 *	函数功能: 	线性加常数后再乘常数
 *	参数说明:	float pa:输入加常数
 *				float pb：输入乘常数
 *				int pn：输入数组的个数，n大于2且为2的倍数
 *				 float *pcOut：输出结果数组，起始地址满足2的 整数倍
 *
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_LinearAdd_Mul(float pa,float pb,int pn,float *pcOut);

/***************************************************************************************
 *	函数名:		void Vec_cplxmulmat(float* pInPut1,float* pInPut2,Uint32 rol,Uint32 col, float* pOutPut);
 *	函数功能: 	复数常量乘以复数矩阵
 *	参数说明:	float pInPut1:输入复数常数
 *				float pInPut2：输入复数矩阵
 *
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_cplxmulmat(float* pInPut1,float* pInPut2,Uint32 rol,Uint32 col, float* pOutPut);

/***************************************************************************************
 *	函数名:		void Vec_LinearAdd_Pow_Mul(float pa,float pb,int pn,float *pcOut)
 *	函数功能: 	线性加常数的和平方再乘常数
 *	参数说明:	float pa:输入加常数
 *				float pb：输入乘常数
 *				int pn：输入数组的个数，n大于2且为2的倍数
 *				 float *pcOut：输出结果数组，起始地址满足2的 整数倍
 *
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_LinearAdd_Pow_Mul(float pa,float pb,int pn,float *pcOut);

/***************************************************************************************
 *	函数名:		void Vec_CplxConjMulCplx_1(float*pX, float*pY, float*pZ, unsigned int n)
 *	函数功能: 	复数共轭序列乘复数序列
 *	参数说明:	float pX:表示输入的复数数组的起始地址,第一个数是实部,第二个表示虚部
 *				float pY：表示另一输入的复数数组的起始地址,第一个数是实部,第二个表示虚部
 *				float pZ: 表示计算的结果存放的起始地址,该地址可以与第一个参数或者第二个参数相同,即结果覆盖输入源
 *				int pn：输入复数序列的点数，输入点数N>=12,且输入点数必须是4的倍数
 *
 *				 备注：1 测试结果发现输出覆盖第二个复数序列，效率相对输出覆盖第一个复数序列或输出不覆盖输入较高
 *                    2 主要功能是实现对第一个复数序列求共轭后，与第二个复数序列点乘
 *
 *	返回值: 		无
 ***************************************************************************************/
//extern void Vec_CplxConjMulCplx(float*pX, float*pY, float*pZ, unsigned int n);

extern void Vec_CplxConjMulCplx_1_sa(float*pX, float*pY, float*pZ, unsigned int n);

/***************************************************************************************
 *	函数名:		void Vec_CplxAddConst2(float*pa, float*pb, float pfc, float pfd, unsigned int n)
 *	函数功能: 	复数序列加法，实部和虚部分别加不同的数据，复数实部虚步分别加不同浮点数 (CPLX * a, CPLX * b,float c, float d,int n)
 *	参数说明:	float pa:表示输入的复数数组的起始地址,第一个数是实部,第二个表示虚部
 *				float pa：表示输出结果的复数数组的起始地址,第一个数是实部,第二个表示虚部
 *				float pfc: 表示实部加的常数
 *				float pfd: 表示虚部加的常数
 *				int pn：输入复数序列的点数，输入点数N>=12,且输入点数必须是4的倍数
 *
 *				 备注：
 *
 *	返回值: 		无
 ***************************************************************************************/
extern void Vec_CplxAddConst2(float*pa, float*pb, float pfc, float pfd, unsigned int n);

/***************************************************************************************
 *	函数名:		void Vec_MulConst_sin_cos(float* X,float *Y,float pfc,int n,const float *table,float  table_num)
 *	函数功能: 	输入角度乘实现向量 求sin和cos
 *	参数说明:	float* X:输入数组   为角度值
 *				float *Y：输出结果数组 存放第一个是cos值，第二个是sin值 *
 *				float pfc: 输入被乘常数，fc = c/2PI
 *				int n：输入数组的点数，n大于2且为2的倍数
 *				const float *table：查表地址                        与cos表为一个
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 *	备注： 1 本函数实现（浮点数）角度乘浮点数后求sincos (float * a, CPLX * b, float c,nt n)
 *	for(i=0;i<n;i++)
 *	{
 *		b[i].re=cos(2*PI*a[i]*c);
 *		b[i].im=sin(2*PI*a[i]*c);
 *	}
 *	    2  若输入参数浮点角度序列a,那么输入给该接口的被乘的常数值 pfc= 实际被乘常数c;若输入参数浮点弧度值序列a,那么要求输入给该接口的被乘的常数值 pfc= 实际被乘常数c/(2*PI)
 ***************************************************************************************/
extern void Vec_MulConst_sin_cos(float* X,float *Y,float pfc,int n,const float *table,float  table_num);

/***************************************************************************************
 *	函数名:		void Vec_sin_cos_MulConst(float* X,float *Y,float pfc,int n,const float *table,float  table_num)
 *	函数功能: 	对输入角度乘实现向量 求sin和cos，并与指定的浮点常数相乘
 *	参数说明:	float* X:输入数组   为角度值,或者是弧度值/2PI
 *				float *Y：输出结果数组 存放第一个是cos值，第二个是sin值 *
 *				float pfc: 输入被乘常数，fc = c
 *				int n：输入数组的点数，n大于2且为2的倍数
 *				const float *table：查表地址                        与cos表为一个
 *		    	float  table_num：查表数组的个数
 *	返回值: 		无
 *	备注： 1 （浮点数）角度求sincos后 乘浮点数(float * a, CPLX * b, float c,nt n)
 *	for(i=0;i<n;i++)
 *	{
 *		b[i].re=c*cos(2*PI*a[i]);
 *		b[i].im=c*sin(2*PI*a[i]);
 *	}
 *	    2  输入参数浮点角度序列a,要求输入给该接口的被乘的常数值 pfc= 实际被乘常数c
 ***************************************************************************************/
extern void Vec_sin_cos_MulConst(float* X,float *Y,float pfc,int n,const float *table,float  table_num);

/***********************************************************
 * 输入参数
 * pUnCacheAddr：映射的不可cache空间
 * iSL2BaseAddr    : 共享L2内存的起始地址，在CMD中单独分出一个段来
 * CpuIndex:cpu一共可以映射16对。0,1,2已被默认映射，所以只有3到15可以用户修改。
			 3~15

 * 返回：
 * * 无
 */
extern void synFlagmap(Uint32 pUnCacheAddr,Uint32 iSL2BaseAddr,Uint32 CpuIndex);
/*******************************************
 * 函数接口：
 * 函数功能：采用不可cache共享空间进行8核同步，最大的同步次数是2的32次方次
 * 输入参数：无
 * 同步周期：300 个Cycle
 *
 *****************************************/
extern void C6678_core8_FlagSyn();//
#endif /* MY_H_ */
