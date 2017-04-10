/**
  ******************************************************************************
  * @file    cpn_ring.h
  * @author  mousie
  * @version V1.0.0
  * @date    2014.5.31
  * @brief   播放特定频率的铃声.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CPN_RING_H
#define __CPN_RING_H

/* Includes ------------------------------------------------------------------*/
#include "drv_ctrlport.h"

#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup CPN_Ring
  * @{
  */



/**
  ******************************************************************************
  * @defgroup CPN_Ring_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define RING_ENABLE
//#define RING_OUT                        FREQ_RING_OUT                           ///< 设置振铃IO口, 由"drv_ctrlport"提供功能
//
//#define RING_TIMER_PERIPHERAL           TIM4
//#define RING_TIMER_IRQ                  TIM4_IRQn
//#define RING_TIMER_IRQ_HANDLER          TIM4_IRQHandler
//#define RING_TIMER_CLK                  RCC_APB1Periph_TIM4
/**
  * @}
  */



/** @defgroup CPN_Ring_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Ring_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Ring_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */
const extern uint16_t alpha1[];
const extern uint16_t alpha2[];
const extern uint16_t alpha3[];
const extern uint16_t alpha4[];
const extern uint16_t alpha5[];
const extern uint16_t alpha6[];
const extern uint16_t alpha7[];
const extern uint16_t alpha8[];
const extern uint16_t du[];
const extern uint16_t du_[];
const extern uint16_t di[];
const extern uint16_t didu[];
const extern uint16_t di_du_[];
const extern uint16_t di_di_di_[];
/**
  * @}
  */

/** @defgroup CPN_Ring_Public_Function
  * @brief    定义公有函数
  * @{
  */
#ifdef RING_ENABLE

void ring_init(void);
void ring_deinit(void);

void ring_start(const uint16_t *pRing);
void ring_try_start(const uint16_t *pRing);
void ring_stop(void);
uint8_t ring_get_status(void);

void ring_scan_1ms(void);

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
