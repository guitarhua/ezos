/**
  ******************************************************************************
  * @file    assert.h
  * @author  mousie
  * @version V2.1.0
  * @date    2013.1.3
  * @brief   调试驱动程序. 改名为assert.h 实现assert_printf
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ASSERT_H
#define __ASSERT_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup Assert
  * @{
  */



/**
  ******************************************************************************
  * @defgroup Assert_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define ASSERT_ENABLE                                                           ///< 使能调试. 调试模式会增加代码量, 建议开发阶段使用
/**
  * @}
  */



/** @defgroup Assert_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup Assert_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup Assert_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

#ifdef ASSERT_ENABLE
/** @defgroup Assert_Public_Function
  * @brief    定义公有函数
  * @{
  */
void assert_init(void);
void assert_err(char* file, uint32_t line);

#define assert_printf(...)              printf(__VA_ARGS__)
#define assert_param(expr)              ((expr) ? (void)0 : assert_err(__FILE__, __LINE__))
/**
  * @}
  */

#else // 禁用调试功能, 重定义assert为空函数

#define assert_init()
#define assert_printf(...)
#define assert_param(expr)

#endif



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
