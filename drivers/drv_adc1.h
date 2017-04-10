/**
  ******************************************************************************
  * @file    drv_adc1.h
  * @author  mousie
  * @version V1.0.0
  * @date    2014.4.22
  * @brief   ADC基础功能, 此驱动仅支持ADC1 DMA模式单通道采样
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_ADC11_H
#define __DRV_ADC11_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_ADC1
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_ADC1_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件
/**
  ******************************************************************************
  * 模数转换 ADC1配置
  *     ADC1通道    与          硬件引脚 的对应关系
  *     ADC_Channel_0       PA0
  *     ADC_Channel_1       PA1
  *     ADC_Channel_2       PA2
  *     ADC_Channel_3       PA3
  *     ADC_Channel_4       PA4
  *     ADC_Channel_5       PA5
  *     ADC_Channel_6       PA6
  *     ADC_Channel_7       PA7
  *     ADC_Channel_8       PB0
  *     ADC_Channel_9       PB1
  *     ADC_Channel_10      PC0
  *     ADC_Channel_11      PC1
  *     ADC_Channel_12      PC2
  *     ADC_Channel_13      PC3
  *     ADC_Channel_14      PC4
  *     ADC_Channel_15      PC5
  *     ADC_Channel_16      TEMPERATURE (芯片温度)
  *     ADC_Channel_17      REFERENCE   (参考电压)
  ******************************************************************************
  */

//#define ADC1_NUM                        8                                       ///< 设置ADC1通道数, 最多8个
//
//#if (ADC1_NUM > 0)
//    #define CHANNEL0_CHANNEL            ADC_Channel_0                           ///< 设置ADC1的通道号
//    #define CHANNEL0_PORT               GPIOA                                   ///< 设置外部中断PORT口
//    #define CHANNEL0_CLK                RCC_APB2Periph_GPIOA                    ///< 设置外部中断PORT口时钟模块
//    #define CHANNEL0_PIN                GPIO_Pin_0                              ///< 设置外部中断PIN口
//#endif
//
//#if (ADC1_NUM > 1)
//    #define CHANNEL1_CHANNEL            ADC_Channel_16                          ///< 设置ADC1的通道号
//    #define CHANNEL1_PORT               0                                       ///< ADC1_CHANNEL16 引脚置0
//    #define CHANNEL1_CLK                0
//    #define CHANNEL1_PIN                0
//#endif
//
//#if (ADC1_NUM > 2)
//    #define CHANNEL2_CHANNEL            ADC_Channel_17                          ///< 设置ADC1的通道号
//    #define CHANNEL2_PORT               0                                       ///< ADC1_CHANNEL17 引脚置0
//    #define CHANNEL2_CLK                0
//    #define CHANNEL2_PIN                0
//#endif
//
//#if (ADC1_NUM > 3)
//    #define CHANNEL3_CHANNEL            ADC_Channel_1
//    #define CHANNEL3_PORT               GPIOA
//    #define CHANNEL3_CLK                RCC_APB2Periph_GPIOA
//    #define CHANNEL3_PIN                GPIO_Pin_1
//#endif
//
//#if (ADC1_NUM > 4)
//    #define CHANNEL4_CHANNEL            ADC_Channel_2
//    #define CHANNEL4_PORT               GPIOA
//    #define CHANNEL4_CLK                RCC_APB2Periph_GPIOA
//    #define CHANNEL4_PIN                GPIO_Pin_2
//#endif
//
//#if (ADC1_NUM > 5)
//    #define CHANNEL5_CHANNEL            ADC_Channel_3
//    #define CHANNEL5_PORT               GPIOA
//    #define CHANNEL5_CLK                RCC_APB2Periph_GPIOA
//    #define CHANNEL5_PIN                GPIO_Pin_3
//#endif
//
//#if (ADC1_NUM > 6)
//    #define CHANNEL6_CHANNEL            ADC_Channel_4
//    #define CHANNEL6_PORT               GPIOA
//    #define CHANNEL6_CLK                RCC_APB2Periph_GPIOA
//    #define CHANNEL6_PIN                GPIO_Pin_4
//#endif
//
//#if (ADC1_NUM > 7)
//    #define CHANNEL7_CHANNEL            ADC_Channel_5
//    #define CHANNEL7_PORT               GPIOA
//    #define CHANNEL7_CLK                RCC_APB2Periph_GPIOA
//    #define CHANNEL7_PIN                GPIO_Pin_5
//#endif
//
///// 外部中断名称类型定义
//typedef enum
//{
//    ADC_CH1 = 0,                                                                ///< 命名为易于理解的外部中断名称, 值0对应CHANNEL0_xxx
//    ADC_CH2,                                                                    ///< 值1对应CHANNEL1_xxx, 以下类推
//    ADC_CH3,
//    ADC_CH4,
//    ADC_CH5,
//    ADC_CH6,
//    ADC_CH7,
//    ADC_CH8,
//} adc1_name_t;

#if (ADC1_NUM > 8)
    #error "ADC1_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_ADC1_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_ADC1_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_ADC1_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */
#if (ADC1_NUM > 0)

extern volatile uint16_t ADC1ConvertedValue[ADC1_NUM];

#endif
/**
  * @}
  */

/** @defgroup DRV_ADC1_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (ADC1_NUM > 0)

void adc1_init(void);
void adc1_deinit(void);
uint16_t adc1_get_value(adc1_name_t name);

void adc1_start(void);
void adc1_stop(void);
void adc1_clear_value(void);

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
