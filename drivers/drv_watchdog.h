/**
  ******************************************************************************
  * @file    drv_watchdog.h
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.6
  * @brief   看门狗驱动程序, 提供独立看门狗以及窗口看门狗功能
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
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define IWATCHDOG_ENABLE                                                        ///< 使能独立看门狗
//#define IWATCHDOG_TIME_MS               3000                                    ///< 独立看门狗复位近似时间, 有效时间, 100-26000ms
//
///**
//  * 窗口看门狗时间要求及其严格, 请按公式自行计算 WWATCHDOG_VALUE 和 WWATCHDOG_COUNT 的值.
//  * 建议值为 WWATCHDOG_VALUE = 0x7F, WWATCHDOG_COUNT = 0x7F, 有效喂狗时间窗口为0ms - 58ms之间.
//  * 采用系统默认时钟 PCLK1 = 36Mhz时, 时间窗口(单位为ms)值计算公式如下:
//  * WWDG_MAX_MS = 4096 * 8 * (WWATCHDOG_COUNT - 63) / 36000
//  * WWDG_MIN_MS = 4096 * 8 * (WWATCHDOG_COUNT - WWATCHDOG_VALUE) / 36000
//  * 有效喂狗时间窗口即为 WWDG_MIN_MS - WWDG_MAX_MS 之间, 时间单位为ms
//  */
//#define WWATCHDOG_ENABLE                                                        ///< 使能窗口看门狗
//#define WWATCHDOG_COUNT                 0x7F                                    ///< 窗口看门狗计数器初始值, 有效值, 0x40 - 0x7F
//#define WWATCHDOG_VALUE                 0x7F                                    ///< 窗口看门狗计数器比较值, 有效值, 0x40 - WWATCHDOG_COUNT



#ifdef IWATCHDOG_ENABLE
#if ((IWATCHDOG_TIME_MS < 100) || (IWATCHDOG_TIME_MS > 26000))
    #error "IWATCHDOG_TIME_MS define error!"
#else
    #define IWATCHDOG_VALUE             ((IWATCHDOG_TIME_MS * 10) / 64)         ///< 计算独立看门狗的装填值, 6.4MS为最小单位
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
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Public_Function
  * @brief    定义公有函数
  * @{
  */
void watchdog_init(void);                                                       ///< 看门狗初始化
void iwatchdog_clear(void);                                                     ///< 独立看门狗喂狗, 建议在优先级最低的一个任务中执行

void wwatchdog_clear(void);                                                     ///< 窗口看门狗喂狗, 建议放在定时器中断中执行
void wwatchDog_disable(void);                                                   ///< 停用窗口看门狗. 原理是切断其Clock, 使其不再计数. 特殊情况使用
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
