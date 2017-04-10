/**
  ******************************************************************************
  * @file    drv_extint.h
  * @author  mousie
  * @version V1.0.0
  * @date    2011.12.22
  * @brief   外部中断驱动函数. 需要定义触发回调函数
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_EXTINT_H
#define __DRV_EXTINT_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "misc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_ExtInt
  * @{
  */
/**
  ******************************************************************************
  * @defgroup DRV_ExtInt_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

///**
//  ******************************************************************************
//  * 使用限制: 5个中断源必须使用不同的中断函数名!!!
//  * 硬件设计: 有多个中断源时, 必须使用不同的 PIN 口.
//  *           建议使用 PIN0, PIN1, PIN2, PIN3 和 PIN4.
//  *           对于 PIN5-PIN9, 只能取其一. 对于PIN10-PIN15, 也只能取其一.
//  *           譬如 PA0, PA1, PA2, PB3, PC4 可以作为5个有效的中断源.
//  *           而   PA0, PB0, PC0, PD0, PE0 是不可取的, 软件无法区分这5个外部中断.
//  ******************************************************************************
//  */
//#define EXTINT_NUM                      5                                       ///< 设置外部中断数量.该驱动最多允许5个外部中断
//
//#if (EXTINT_NUM > 0)
//    #define EXTINT0_PORT                GPIOA                                   ///< 设置外部中断PORT口
//    #define EXTINT0_CLK                 RCC_APB2Periph_GPIOA                    ///< 设置外部中断PORT口时钟模块
//    #define EXTINT0_PORT_SOURCE         GPIO_PortSourceGPIOA                    ///< 设置外部中断PORT口源
//    #define EXTINT0_PIN                 GPIO_Pin_0                              ///< 设置外部中断PIN口
//    #define EXTINT0_PIN_SOURCE          GPIO_PinSource0                         ///< GPIO_Pin_X, 则设置为 GPIO_PinSourceX
//    #define EXTINT0_LINE                EXTI_Line0                              ///< GPIO_Pin_X, 则设置为 EXTI_LineX
//
//    /**
//      * @brief 设置外部中断的中断号
//      *   @arg EXTI0_IRQn,              GPIO_Pin_0 使用此中断号
//      *   @arg EXTI1_IRQn,              GPIO_Pin_1 使用此中断号
//      *   @arg EXTI2_IRQn,              GPIO_Pin_2 使用此中断号
//      *   @arg EXTI3_IRQn,              GPIO_Pin_3 使用此中断号
//      *   @arg EXTI4_IRQn,              GPIO_Pin_4 使用此中断号
//      *   @arg EXTI9_5_IRQn,            GPIO_Pin_5  - GPIO_Pin_9  使用此中断号
//      *   @arg EXTI15_10_IRQn,          GPIO_Pin_10 - GPIO_Pin_15 使用此中断号
//      */
//    #define EXTINT0_IRQ                 EXTI0_IRQn
//    #define EXTINT0_IRQ_HANDLER         EXTI0_IRQHandler                        ///< 外部中断函数名
//
//    /**
//      * @brief 设置外部中断回调函数
//      *   @arg 0,                       不使用回调函数, 不对外部中断进行处理
//      *   @arg void fun(void),          回调函数格式, 宏定义fun函数名称即可
//      */
//    #define EXTINT0_CALLBACK            0
//    #define EXTINT0_IRQ_PRIORITY        0xF                                     ///< 设置中断优先级. 高2位为可抢占主优先级, 低2位为非抢占的子优先级
//#endif
//
//#if (EXTINT_NUM > 1)
//    #define EXTINT1_PORT                GPIOA
//    #define EXTINT1_CLK                 RCC_APB2Periph_GPIOA
//    #define EXTINT1_PORT_SOURCE         GPIO_PortSourceGPIOA
//    #define EXTINT1_PIN                 GPIO_Pin_2
//    #define EXTINT1_PIN_SOURCE          GPIO_PinSource2
//    #define EXTINT1_LINE                EXTI_Line2
//
//    #define EXTINT1_IRQ                 EXTI2_IRQn
//    #define EXTINT1_IRQ_HANDLER         EXTI2_IRQHandler
//
//    #define EXTINT1_CALLBACK            0
//    #define EXTINT1_IRQ_PRIORITY        0xF
//#endif
//
//#if (EXTINT_NUM > 2)
//    #define EXTINT2_PORT                GPIOA
//    #define EXTINT2_CLK                 RCC_APB2Periph_GPIOA
//    #define EXTINT2_PORT_SOURCE         GPIO_PortSourceGPIOA
//    #define EXTINT2_PIN                 GPIO_Pin_4
//    #define EXTINT2_PIN_SOURCE          GPIO_PinSource4
//    #define EXTINT2_LINE                EXTI_Line4
//
//    #define EXTINT2_IRQ                 EXTI4_IRQn
//    #define EXTINT2_IRQ_HANDLER         EXTI4_IRQHandler
//
//    #define EXTINT2_CALLBACK            0
//    #define EXTINT2_IRQ_PRIORITY        0xF
//#endif
//
//#if (EXTINT_NUM > 3)
//    #define EXTINT3_PORT                GPIOA
//    #define EXTINT3_CLK                 RCC_APB2Periph_GPIOA
//    #define EXTINT3_PORT_SOURCE         GPIO_PortSourceGPIOA
//    #define EXTINT3_PIN                 GPIO_Pin_8
//    #define EXTINT3_PIN_SOURCE          GPIO_PinSource8
//    #define EXTINT3_LINE                EXTI_Line8
//
//    #define EXTINT3_IRQ                 EXTI9_5_IRQn
//    #define EXTINT3_IRQ_HANDLER         EXTI9_5_IRQHandler
//
//    #define EXTINT3_CALLBACK            0
//    #define EXTINT3_IRQ_PRIORITY        0xF
//#endif
//
//#if (EXTINT_NUM > 4)
//    #define EXTINT4_PORT                GPIOA
//    #define EXTINT4_CLK                 RCC_APB2Periph_GPIOA
//    #define EXTINT4_PORT_SOURCE         GPIO_PortSourceGPIOA
//    #define EXTINT4_PIN                 GPIO_Pin_10
//    #define EXTINT4_PIN_SOURCE          GPIO_PinSource10
//    #define EXTINT4_LINE                EXTI_Line10
//
//    #define EXTINT4_IRQ                 EXTI15_10_IRQn
//    #define EXTINT4_IRQ_HANDLER         EXTI15_10_IRQHandler
//
//    #define EXTINT4_CALLBACK            0
//    #define EXTINT4_IRQ_PRIORITY        0xF
//#endif
//
///// 外部中断名称类型定义
//typedef enum
//{
//    EXTINT1 = 0,                                                                ///< 命名为易于理解的外部中断名称, 值0对应EXTIN0_xxx
//    EXTINT2,                                                                    ///< 值1对应EXTINT1_xxx, 以下类推
//    EXTINT3,
//    EXTINT4,
//    EXTINT5,
//} extint_name_t;



#if (EXTINT_NUM > 5)
    #error "EXTINT_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_ExtInt_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_ExtInt_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_ExtInt_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_ExtInt_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (EXTINT_NUM > 0)

/**
  ******************************************************************************
  * @brief  外部中断初始化
  * @param  extint_name
  *   @arg  可用值请参考 extint_name_t 类型定义中的枚举值
  * @param  extint_trigger
  *   @arg  EXTI_Trigger_Rising,        上升沿触发
  *   @arg  EXTI_Trigger_Falling,       下降沿触发
  *   @arg  EXTI_Trigger_Rising_Falling,跳变沿触发
  * @retval none
  ******************************************************************************
  */
void extint_init(extint_name_t extint_name, EXTITrigger_TypeDef extint_trigger);
void extint_deinit(extint_name_t extint_name);                                  // 禁用外部中断
void extint_software(extint_name_t extint_name);                                // 软件产生一次外部中断

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
