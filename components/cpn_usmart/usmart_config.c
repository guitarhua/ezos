#include "usmart.h"
#include "usmart_str.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2011/6/13
//�汾��V2.1
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2011-2021
//All rights reserved
//********************************************************************************
//����˵��
//V1.4
//�����˶Բ���Ϊstring���͵ĺ�����֧��.���÷�Χ������.
//�Ż����ڴ�ռ��,��̬�ڴ�ռ��Ϊ79���ֽ�@10������.��̬��Ӧ���ּ��ַ�������
//V2.0 
//1,�޸���listָ��,��ӡ�������������ʽ.
//2,������idָ��,��ӡÿ����������ڵ�ַ.
//3,�޸��˲���ƥ��,֧�ֺ��������ĵ���(������ڵ�ַ).
//4,�����˺��������Ⱥ궨��.	
//V2.1 20110707		 
//1,����dec,hex����ָ��,�������ò�����ʾ����,��ִ�н���ת��.
//ע:��dec,hex����������ʱ��,���趨��ʾ��������.�����������ʱ��,��ִ�н���ת��.
//��:"dec 0XFF" ��Ὣ0XFFתΪ255,�ɴ��ڷ���.
//��:"hex 100" 	��Ὣ100תΪ0X64,�ɴ��ڷ���
//2,����usmart_get_cmdname����,���ڻ�ȡָ������.
/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����)
#include "includes.h"

extern void read_ibuton(u8 addr,u8 len);			
extern void write_ibuton(u8 addr,u8 data);
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
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
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	0,		//Ĭ��Ϊ����,1ʱΪ�ַ���
	0,		//�������ȱ�
	0,	  	//����������
	0,		//Ĭ��10���Ʋ���
};	   



















