/**
  ******************************************************************************
  * @file    drv_ctrlport.h
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.15
  * @brief   控制引脚驱动程序. 如电源开关, 信号开关.
  *          如果外设芯片上有控制开关的引脚, 则该引脚应该归入芯片驱动代码.
  *          此驱动的控制引脚一般是通过三极管, MOS管等分立元件达到控制目的.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_CTRLPORT_H
#define __DRV_CTRLPORT_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_CtrlPort
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_CtrlPort_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define CTRLPORT_NUM                    5                                       ///< 设置控制引脚的数量. 该驱动最多允许16个控制引脚.
//
//#define CTRLPORT0_PORT                  GPIOC                                   ///< 设置该控制引脚的PORT口
//#define CTRLPORT0_CLK                   RCC_APB2Periph_GPIOC                    ///< 设置该控制引脚的时钟模块
//#define CTRLPORT0_PIN                   GPIO_Pin_8                              ///< 设置该控制引脚的PIN口
//#define CTRLPORT0_OFF                   1                                       ///< 设置关闭控制引脚的电平
//
//#define CTRLPORT1_PORT                  GPIOD
//#define CTRLPORT1_CLK                   RCC_APB2Periph_GPIOD
//#define CTRLPORT1_PIN                   GPIO_Pin_9
//#define CTRLPORT1_OFF                   0
//
//#define CTRLPORT2_PORT                  GPIOD
//#define CTRLPORT2_CLK                   RCC_APB2Periph_GPIOD
//#define CTRLPORT2_PIN                   GPIO_Pin_10
//#define CTRLPORT2_OFF                   0
//
//#define CTRLPORT3_PORT                  GPIOD
//#define CTRLPORT3_CLK                   RCC_APB2Periph_GPIOD
//#define CTRLPORT3_PIN                   GPIO_Pin_11
//#define CTRLPORT3_OFF                   0
//
///// 控制引脚名称类型定义
//typedef enum
//{
//    SD_POWER = 0,                                                               ///< 命名为易于理解的控制名称, 值0对应CTRLPORT0_xxx
//    SPK_POWER,                                                                  ///< 值1对应CTRLPORT1_xxx, 以下类推
//    SPK_SIGNAL,
//    LCD_BACKLIGHT,
//} ctrlport_name_t;



#if (CTRLPORT_NUM > 16)
    #error "CTRLPORT_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_CtrlPort_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_CtrlPort_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_CtrlPort_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_CtrlPort_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (CTRLPORT_NUM > 0)

void ctrlport_init_all(void);                                                     // 初始化所有的控制引脚
void ctrlport_init(ctrlport_name_t ctrlport_name);                                // 初始化指定的控制引脚
void ctrlport_on(ctrlport_name_t ctrlport_name);                                  // 控制引脚逻辑开
void ctrlport_off(ctrlport_name_t ctrlport_name);                                 // 控制引脚逻辑关
void ctrlport_toggle(ctrlport_name_t ctrlport_name);                              // 控制引脚翻转
uint8_t ctrlport_get_status(ctrlport_name_t ctrlport_name);                       // 读取控制引脚的逻辑状态. 0, 关闭. 1, 打开

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
