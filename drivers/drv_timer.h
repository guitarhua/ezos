/**
  ******************************************************************************
  * @file    drv_timer.h
  * @author  mousie
  * @version V1.0.0
  * @date    2012.2.18
  * @brief   硬件定时器驱动, 仅提供最基本的定时溢出中断功能.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_TIMER_H
#define __DRV_TIMER_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_Timer
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_Timer_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define TIMER_NUM                       3                                       ///< 设置使用的Timer数量.该驱动最多允许3个Timer数
//
//#if (TIMER_NUM > 0)
//    /**
//      * @brief 设置该Timer的外设名称. TIM1为高级定时器, 此驱动不支持使用TIM1!!!
//      *   @arg TIM2,                    使用TIM2外设, CLK为 RCC_APB1Periph_TIMx
//      *   @arg TIM3,                    使用TIM3外设
//      *   @arg TIM4,                    使用TIM4外设
//      *   @arg TIM5,                    使用TIM5外设
//      *   @arg TIM6,                    使用TIM6外设
//      *   @arg TIM7,                    使用TIM7外设
//      */
//    #define TIMER0_PERIPHERAL           TIM2
//    #define TIMER0_IRQ                  TIM2_IRQn                               ///< 设置该Timer的中断
//    #define TIMER0_IRQ_HANDLER          TIM2_IRQHandler                         ///< 设置该Timer的中断函数
//    #define TIMER0_CLK                  RCC_APB1Periph_TIM2                     ///< 设置该Timer的时钟模块
//
//    /**
//      * @brief 设置该Timer的溢出中断回调函数
//      *   @arg 0,                       不使用回调函数, 不对接收数据进行处理
//      *   @arg void fun(void),          回调函数格式, 宏定义fun函数名称即可
//      */
//    #define TIMER0_OVERTIME_CALLBACK    0
//    #define TIMER0_IRQ_PRIORITY         0xF                                     ///< 设置中断优先级. 高2位为可抢占主优先级, 低2位为非抢占的子优先级
//#endif
//
//#if (TIMER_NUM > 1)
//    #define TIMER1_PERIPHERAL           TIM3
//    #define TIMER1_IRQ                  TIM3_IRQn
//    #define TIMER1_IRQ_HANDLER          TIM3_IRQHandler
//    #define TIMER1_CLK                  RCC_APB1Periph_TIM3
//
//    #define TIMER1_OVERTIME_CALLBACK    0
//    #define TIMER1_IRQ_PRIORITY         0xF
//#endif
//
//#if (TIMER_NUM > 2)
//    #define TIMER2_PERIPHERAL           TIM4
//    #define TIMER2_IRQ                  TIM4_IRQn
//    #define TIMER2_IRQ_HANDLER          TIM4_IRQHandler
//    #define TIMER2_CLK                  RCC_APB1Periph_TIM4
//
//    #define TIMER2_OVERTIME_CALLBACK    0
//    #define TIMER2_IRQ_PRIORITY         0xF
//#endif
//
///// 定时器名称类型定义
//typedef enum
//{
//    TIMER_200US = 0,                                                            ///< 命名为易于理解的Timer名称, 值0对应TIMER0_xxx
//    TIMER_1MS,                                                                  ///< 值1对应TIMER1_xxx, 以下类推
//    TIMER_1S,
//} timer_name_t;



#if (TIMER_NUM > 3)
    #error "TIMER_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_Timer_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Timer_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Timer_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Timer_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (TIMER_NUM > 0)

// 定时时间 T(us) = prescaler * period / 72
void timer_init(timer_name_t timer_name, uint16_t prescaler, uint16_t period);
void timer_deinit(timer_name_t timer_name);

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
