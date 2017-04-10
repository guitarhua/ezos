/**
  ******************************************************************************
  * @file    ezos_conf.h
  * @author  mousie
  * @version V2.3.0
  * @date    2014.3.28
  * @brief   协作式RTOS配置文件
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EZOS_CONF_H
#define __EZOS_CONF_H

/** @addtogroup OS
  * @{
  */
/** @addtogroup ezOS
  * @{
  */



// ezos配置
/**
  * 设置ezos系统时钟.
  * 有效值1-1000. 建议值 1ms 或 10ms.
  * 需同时修改 ezos_scan_tick() 的调用时间.
  */
#define __EZOS_TICK_MS                  1

#define __EZOS_TASK_NUM                 16                                      ///< 设置ezos的最大任务数, 有效值1-250
//#define __EZOS_DEBUG                                                            ///< 使能ezos性能调试

#ifdef __EZOS_DEBUG
    #include <stdio.h>
    #define ezos_printf(...)            assert_printf(__VA_ARGS__)              ///< 设置ezos调试打印函数
#else
    #define ezos_printf(...)
#endif



#define __EZOS_SEM                                                              ///< 使能ezos信号量
#define __EZOS_MUTEX                                                            ///< 使能ezos互斥量
#define __EZOS_EVENT                                                            ///< 使能ezos事件
#define __EZOS_MAILBOX                                                          ///< 使能ezos邮箱
#define __EZOS_MESSAGE                                                          ///< 使能ezos消息队列



#endif
/* END OF FILE ---------------------------------------------------------------*/
