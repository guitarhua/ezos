/**
  ******************************************************************************
  * @file    drv_systick.h
  * @author  mousie
  * @version V2.1.0
  * @date    2016.02.25
  * @brief   ϵͳ���ʱ��, ���ṩ΢��ͺ�����ʱ����.
  *          ������΢�뺯���������������.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_SYSTICK_H
#define __DRV_SYSTICK_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_SysTick
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_SysTick_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_SysTick_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SysTick_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SysTick_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SysTick_Public_Function
  * @brief    ���幫�к���
  * @{
  */
void systick_init_1ms(void);                                                    // ϵͳ���ʱ�ӳ�ʼ������
void delay_scan_1ms(void);                                                      // ��ʱ�����õ�1msɨ��, ��1ms��ʱ�ж��е���
uint32_t delay_tick_get(void);                                                  // ��ȡ1ms��ʱʱ��ֵ
void delay_ms(uint16_t time);                                                   // ms��ʱ����, �������жϺ����е���
void delay_us(uint16_t time);                                                   // us��ʱ����, �������жϺ����е���
/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
