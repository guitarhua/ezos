/**
  ******************************************************************************
  * @file    drv_timer.h
  * @author  mousie
  * @version V1.0.0
  * @date    2012.2.18
  * @brief   Ӳ����ʱ������, ���ṩ������Ķ�ʱ����жϹ���.
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
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define TIMER_NUM                       3                                       ///< ����ʹ�õ�Timer����.�������������3��Timer��
//
//#if (TIMER_NUM > 0)
//    /**
//      * @brief ���ø�Timer����������. TIM1Ϊ�߼���ʱ��, ��������֧��ʹ��TIM1!!!
//      *   @arg TIM2,                    ʹ��TIM2����, CLKΪ RCC_APB1Periph_TIMx
//      *   @arg TIM3,                    ʹ��TIM3����
//      *   @arg TIM4,                    ʹ��TIM4����
//      *   @arg TIM5,                    ʹ��TIM5����
//      *   @arg TIM6,                    ʹ��TIM6����
//      *   @arg TIM7,                    ʹ��TIM7����
//      */
//    #define TIMER0_PERIPHERAL           TIM2
//    #define TIMER0_IRQ                  TIM2_IRQn                               ///< ���ø�Timer���ж�
//    #define TIMER0_IRQ_HANDLER          TIM2_IRQHandler                         ///< ���ø�Timer���жϺ���
//    #define TIMER0_CLK                  RCC_APB1Periph_TIM2                     ///< ���ø�Timer��ʱ��ģ��
//
//    /**
//      * @brief ���ø�Timer������жϻص�����
//      *   @arg 0,                       ��ʹ�ûص�����, ���Խ������ݽ��д���
//      *   @arg void fun(void),          �ص�������ʽ, �궨��fun�������Ƽ���
//      */
//    #define TIMER0_OVERTIME_CALLBACK    0
//    #define TIMER0_IRQ_PRIORITY         0xF                                     ///< �����ж����ȼ�. ��2λΪ����ռ�����ȼ�, ��2λΪ����ռ�������ȼ�
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
///// ��ʱ���������Ͷ���
//typedef enum
//{
//    TIMER_200US = 0,                                                            ///< ����Ϊ��������Timer����, ֵ0��ӦTIMER0_xxx
//    TIMER_1MS,                                                                  ///< ֵ1��ӦTIMER1_xxx, ��������
//    TIMER_1S,
//} timer_name_t;



#if (TIMER_NUM > 3)
    #error "TIMER_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_Timer_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Timer_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Timer_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Timer_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (TIMER_NUM > 0)

// ��ʱʱ�� T(us) = prescaler * period / 72
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
