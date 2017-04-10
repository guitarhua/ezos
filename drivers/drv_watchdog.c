/**
  ******************************************************************************
  * @file    drv_watchdog.c
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.6
  * @brief   看门狗驱动程序, 提供独立看门狗以及窗口看门狗功能
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_watchdog.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_wwdg.h"



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_WatchDog
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup DRV_WatchDog_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_WatchDog_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_WatchDog_Private_Function
  * @brief    定义私有函数
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_WatchDog_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  看门狗初始化
  * @param  none
  * @retval none
  ******************************************************************************
  */
void watchdog_init(void)
{
#ifdef IWATCHDOG_ENABLE
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);                               // 使能独立看门口狗寄存器写入功能
    IWDG_SetPrescaler(IWDG_Prescaler_256);                                      // 独立看门狗时钟分频, 40K/256=156HZ(6.4ms)
    IWDG_SetReload(IWATCHDOG_VALUE);                                            // 设置独立看门狗复位时间. 参数值不能大于0xfff
    IWDG_ReloadCounter();                                                       // 喂狗
    IWDG_Enable();                                                              // 使能独立看门狗
#endif

#ifdef WWATCHDOG_ENABLE
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);                        // 窗口看门狗时钟允许
    WWDG_SetPrescaler(WWDG_Prescaler_8);                                        // 窗口看门狗节拍 (36M/4096)/8 = 1098 Hz
    WWDG_SetWindowValue(WWATCHDOG_VALUE);                                       // 设置窗口看门狗窗口值
    WWDG_Enable(WWATCHDOG_COUNT);                                               // 使能窗口看门狗并初始化定时器值
    WWDG_ClearFlag();                                                           // 清除窗口看门狗中断标记
//  WWDG_EnableIT();                                                            // 使能窗口看门狗复位前中断(可用于复位前的数据保护, 不建议用此中断喂狗)
#endif
}

/**
  ******************************************************************************
  * @brief  独立看门狗喂狗, 建议在优先级最低的一个任务中执行.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void iwatchdog_clear(void)
{
#ifdef IWATCHDOG_ENABLE
    IWDG_ReloadCounter();                                                       // 喂独立看门狗
#endif
}

/**
  ******************************************************************************
  * @brief  窗口看门狗喂狗, 建议放在定时器中断中执行.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void wwatchdog_clear(void)
{
#ifdef WWATCHDOG_ENABLE
    WWDG_SetCounter(WWATCHDOG_COUNT);                                           // 喂窗口看门狗
#endif
}

/**
  ******************************************************************************
  * @brief  停用窗口看门狗. 原理是切断其Clock, 使其不再计数. 特殊情况使用
  * @param  none
  * @retval none
  ******************************************************************************
  */
void wwatchdog_disable(void)
{
#ifdef WWATCHDOG_ENABLE
    WWDG_DeInit();
#endif
}

/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

/* END OF FILE ---------------------------------------------------------------*/
