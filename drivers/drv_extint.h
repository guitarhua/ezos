/**
  ******************************************************************************
  * @file    drv_extint.h
  * @author  mousie
  * @version V1.0.0
  * @date    2011.12.22
  * @brief   �ⲿ�ж���������. ��Ҫ���崥���ص�����
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
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

///**
//  ******************************************************************************
//  * ʹ������: 5���ж�Դ����ʹ�ò�ͬ���жϺ�����!!!
//  * Ӳ�����: �ж���ж�Դʱ, ����ʹ�ò�ͬ�� PIN ��.
//  *           ����ʹ�� PIN0, PIN1, PIN2, PIN3 �� PIN4.
//  *           ���� PIN5-PIN9, ֻ��ȡ��һ. ����PIN10-PIN15, Ҳֻ��ȡ��һ.
//  *           Ʃ�� PA0, PA1, PA2, PB3, PC4 ������Ϊ5����Ч���ж�Դ.
//  *           ��   PA0, PB0, PC0, PD0, PE0 �ǲ���ȡ��, ����޷�������5���ⲿ�ж�.
//  ******************************************************************************
//  */
//#define EXTINT_NUM                      5                                       ///< �����ⲿ�ж�����.�������������5���ⲿ�ж�
//
//#if (EXTINT_NUM > 0)
//    #define EXTINT0_PORT                GPIOA                                   ///< �����ⲿ�ж�PORT��
//    #define EXTINT0_CLK                 RCC_APB2Periph_GPIOA                    ///< �����ⲿ�ж�PORT��ʱ��ģ��
//    #define EXTINT0_PORT_SOURCE         GPIO_PortSourceGPIOA                    ///< �����ⲿ�ж�PORT��Դ
//    #define EXTINT0_PIN                 GPIO_Pin_0                              ///< �����ⲿ�ж�PIN��
//    #define EXTINT0_PIN_SOURCE          GPIO_PinSource0                         ///< GPIO_Pin_X, ������Ϊ GPIO_PinSourceX
//    #define EXTINT0_LINE                EXTI_Line0                              ///< GPIO_Pin_X, ������Ϊ EXTI_LineX
//
//    /**
//      * @brief �����ⲿ�жϵ��жϺ�
//      *   @arg EXTI0_IRQn,              GPIO_Pin_0 ʹ�ô��жϺ�
//      *   @arg EXTI1_IRQn,              GPIO_Pin_1 ʹ�ô��жϺ�
//      *   @arg EXTI2_IRQn,              GPIO_Pin_2 ʹ�ô��жϺ�
//      *   @arg EXTI3_IRQn,              GPIO_Pin_3 ʹ�ô��жϺ�
//      *   @arg EXTI4_IRQn,              GPIO_Pin_4 ʹ�ô��жϺ�
//      *   @arg EXTI9_5_IRQn,            GPIO_Pin_5  - GPIO_Pin_9  ʹ�ô��жϺ�
//      *   @arg EXTI15_10_IRQn,          GPIO_Pin_10 - GPIO_Pin_15 ʹ�ô��жϺ�
//      */
//    #define EXTINT0_IRQ                 EXTI0_IRQn
//    #define EXTINT0_IRQ_HANDLER         EXTI0_IRQHandler                        ///< �ⲿ�жϺ�����
//
//    /**
//      * @brief �����ⲿ�жϻص�����
//      *   @arg 0,                       ��ʹ�ûص�����, �����ⲿ�жϽ��д���
//      *   @arg void fun(void),          �ص�������ʽ, �궨��fun�������Ƽ���
//      */
//    #define EXTINT0_CALLBACK            0
//    #define EXTINT0_IRQ_PRIORITY        0xF                                     ///< �����ж����ȼ�. ��2λΪ����ռ�����ȼ�, ��2λΪ����ռ�������ȼ�
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
///// �ⲿ�ж��������Ͷ���
//typedef enum
//{
//    EXTINT1 = 0,                                                                ///< ����Ϊ���������ⲿ�ж�����, ֵ0��ӦEXTIN0_xxx
//    EXTINT2,                                                                    ///< ֵ1��ӦEXTINT1_xxx, ��������
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
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_ExtInt_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_ExtInt_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_ExtInt_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (EXTINT_NUM > 0)

/**
  ******************************************************************************
  * @brief  �ⲿ�жϳ�ʼ��
  * @param  extint_name
  *   @arg  ����ֵ��ο� extint_name_t ���Ͷ����е�ö��ֵ
  * @param  extint_trigger
  *   @arg  EXTI_Trigger_Rising,        �����ش���
  *   @arg  EXTI_Trigger_Falling,       �½��ش���
  *   @arg  EXTI_Trigger_Rising_Falling,�����ش���
  * @retval none
  ******************************************************************************
  */
void extint_init(extint_name_t extint_name, EXTITrigger_TypeDef extint_trigger);
void extint_deinit(extint_name_t extint_name);                                  // �����ⲿ�ж�
void extint_software(extint_name_t extint_name);                                // �������һ���ⲿ�ж�

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
