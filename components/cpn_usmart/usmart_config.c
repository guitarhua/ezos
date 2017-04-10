#include "usmart.h"
#include "usmart_str.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2011/6/13
//版本：V2.1
//版权所有，盗版必究。
//Copyright(C) 正点原子 2011-2021
//All rights reserved
//********************************************************************************
//升级说明
//V1.4
//增加了对参数为string类型的函数的支持.适用范围大大提高.
//优化了内存占用,静态内存占用为79个字节@10个参数.动态适应数字及字符串长度
//V2.0 
//1,修改了list指令,打印函数的完整表达式.
//2,增加了id指令,打印每个函数的入口地址.
//3,修改了参数匹配,支持函数参数的调用(输入入口地址).
//4,增加了函数名长度宏定义.	
//V2.1 20110707		 
//1,增加dec,hex两个指令,用于设置参数显示进制,及执行进制转换.
//注:当dec,hex不带参数的时候,即设定显示参数进制.当后跟参数的时候,即执行进制转换.
//如:"dec 0XFF" 则会将0XFF转为255,由串口返回.
//如:"hex 100" 	则会将100转为0X64,由串口返回
//2,新增usmart_get_cmdname函数,用于获取指令名字.
/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加)
#include "includes.h"

extern void read_ibuton(u8 addr,u8 len);			
extern void write_ibuton(u8 addr,u8 data);
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
	(void*)delay_ms,"void delay_ms(u16 nms)",
	(void*)delay_us,"void delay_us(u32 nus)",
	(void*)tok_set_sol,"void tok_set_sol(u8 solx,u8 sta)",
	(void*)tok_read_sens,"u8 tok_read_sens(void)",	 
	(void*)diled_set_sw,"void diled_set_sw(u8 swx,u8 sta)",	   
	(void*)doorsens_read,"u8 doorsens_read(void)",

	(void*)SWB_Init,"u8 SWB_Init(void)",
	(void*)SWB_Write_Byte,"void SWB_Write_Byte(u8 dat)",
	(void*)SWB_Read_Bit,"u8 SWB_Read_Byte(void)",
	(void*)SWB_Check,"u8 SWB_Check(void)",

	(void*)read_ibuton,"void read_ibuton(u8 addr,u8 len)",
	(void*)write_ibuton,"void write_ibuton(u8 addr,u8 data)",

};	  							  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	0,		//默认为数据,1时为字符串
	0,		//参数长度表
	0,	  	//函数参数表
	0,		//默认10进制参量
};	   



















