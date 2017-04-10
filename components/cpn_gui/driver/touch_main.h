/**
  ******************************************************************************
  * @file    touch_main.h
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2013.9.20
  * @brief   LCD 基础驱动程序. 包含LCD电源开关, 亮度调节, 驱动测试函数.
  *          并入 emwin, 内容做过简化!
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TOUCH_MAIN_H
#define __TOUCH_MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "lcdgui_conf.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup CPN_GUI
  * @{
  */

/** @defgroup TOUCH_MAIN_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup TOUCH_MAIN_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if GUI_SUPPORT_TOUCH && (defined LCD_ENABLE)

// 触摸屏控制函数
void touch_init(void);                                                          // 触摸屏初始化, 已包含 touch_start
void touch_stop(void);                                                          // 触摸屏停止工作.
void touch_start(void);                                                         // 触摸屏停止后重启

// 触摸屏测试函数
void touch_test_driver(void);

#else

#define touch_init()

#endif
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
