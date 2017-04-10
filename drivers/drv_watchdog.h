/**
  ******************************************************************************
  * @file    drv_watchdog.h
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.6
  * @brief   ���Ź���������, �ṩ�������Ź��Լ����ڿ��Ź�����
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_WATCHDOG_H
#define __DRV_WATCHDOG_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_WatchDog
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_WatchDog_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define IWATCHDOG_ENABLE                                                        ///< ʹ�ܶ������Ź�
//#define IWATCHDOG_TIME_MS               3000                                    ///< �������Ź���λ����ʱ��, ��Чʱ��, 100-26000ms
//
///**
//  * ���ڿ��Ź�ʱ��Ҫ�����ϸ�, �밴��ʽ���м��� WWATCHDOG_VALUE �� WWATCHDOG_COUNT ��ֵ.
//  * ����ֵΪ WWATCHDOG_VALUE = 0x7F, WWATCHDOG_COUNT = 0x7F, ��Чι��ʱ�䴰��Ϊ0ms - 58ms֮��.
//  * ����ϵͳĬ��ʱ�� PCLK1 = 36Mhzʱ, ʱ�䴰��(��λΪms)ֵ���㹫ʽ����:
//  * WWDG_MAX_MS = 4096 * 8 * (WWATCHDOG_COUNT - 63) / 36000
//  * WWDG_MIN_MS = 4096 * 8 * (WWATCHDOG_COUNT - WWATCHDOG_VALUE) / 36000
//  * ��Чι��ʱ�䴰�ڼ�Ϊ WWDG_MIN_MS - WWDG_MAX_MS ֮��, ʱ�䵥λΪms
//  */
//#define WWATCHDOG_ENABLE                                                        ///< ʹ�ܴ��ڿ��Ź�
//#define WWATCHDOG_COUNT                 0x7F                                    ///< ���ڿ��Ź���������ʼֵ, ��Чֵ, 0x40 - 0x7F
//#define WWATCHDOG_VALUE                 0x7F                                    ///< ���ڿ��Ź��������Ƚ�ֵ, ��Чֵ, 0x40 - WWATCHDOG_COUNT



#ifdef IWATCHDOG_ENABLE
#if ((IWATCHDOG_TIME_MS < 100) || (IWATCHDOG_TIME_MS > 26000))
    #error "IWATCHDOG_TIME_MS define error!"
#else
    #define IWATCHDOG_VALUE             ((IWATCHDOG_TIME_MS * 10) / 64)         ///< ����������Ź���װ��ֵ, 6.4MSΪ��С��λ
#endif
#endif

#ifdef WWATCHDOG_ENABLE
#if ((WWATCHDOG_COUNT < 0x40) || (WWATCHDOG_COUNT > 0x7F))
    #error "WWATCHDOG_COUNT define error!"
#endif
#if ((WWATCHDOG_VALUE < 0x40) || (WWATCHDOG_VALUE > WWATCHDOG_COUNT))
    #error "WWATCHDOG_VALUE define error!"
#endif
#endif
/**
  * @}
  */



/** @defgroup DRV_WatchDog_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Public_Function
  * @brief    ���幫�к���
  * @{
  */
void watchdog_init(void);                                                       ///< ���Ź���ʼ��
void iwatchdog_clear(void);                                                     ///< �������Ź�ι��, ���������ȼ���͵�һ��������ִ��

void wwatchdog_clear(void);                                                     ///< ���ڿ��Ź�ι��, ������ڶ�ʱ���ж���ִ��
void wwatchDog_disable(void);                                                   ///< ͣ�ô��ڿ��Ź�. ԭ�����ж���Clock, ʹ�䲻�ټ���. �������ʹ��
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
