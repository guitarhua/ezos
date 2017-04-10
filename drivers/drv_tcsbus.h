/**
  ******************************************************************************
  * @file    drv_tcsbus.h
  * @author  mousie
  * @version V2.0.0
  * @date    2012.10.26
  * @brief   TCS BUS 底层驱动. 未测试
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_TCSBUS_H
#define __DRV_TCSBUS_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_TCSBUS
  * @{
  */
/**
  ******************************************************************************
  * @defgroup DRV_TCSBUS_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define TCSBUS_NUM                      3                                       ///< 设置使用的TCSBUS数量.该驱动最多允许3条TCSBUS总线
//
//#if (TCSBUS_NUM > 0)
//    #define TCSBUS0_TX_PORT             GPIOA                                   ///< 设置该TCSBUS的输出引脚PORT口
//    #define TCSBUS0_TX_CLK              RCC_APB2Periph_GPIOA                    ///< 设置该TCSBUS的输出引脚时钟模块
//    #define TCSBUS0_TX_PIN              GPIO_Pin_0                              ///< 设置该TCSBUS的输出引脚PIN口
//
//    #define TCSBUS0_RX_PORT             GPIOA                                   ///< 设置该TCSBUS的输入引脚PORT口
//    #define TCSBUS0_RX_CLK              RCC_APB2Periph_GPIOA                    ///< 设置该TCSBUS的输入引脚时钟模块
//    #define TCSBUS0_RX_PIN              GPIO_Pin_1                              ///< 设置该TCSBUS的输入引脚PIN口
//
//    /**
//      * @brief 设置该TCSBUS的接收回调函数
//      *   @arg 0,                                           不使用回调函数, 不对接收数据进行处理
//      *   @arg void fun(uint8_t type, uint32_t data),       回调函数格式, 宏定义fun函数名称即可
//      */
//    #define TCSBUS0_RX_CALLBACK         0
//#endif
//
//#if (TCSBUS_NUM > 1)
//    #define TCSBUS1_TX_PORT             GPIOA
//    #define TCSBUS1_TX_CLK              RCC_APB2Periph_GPIOA
//    #define TCSBUS1_TX_PIN              GPIO_Pin_2
//
//    #define TCSBUS1_RX_PORT             GPIOA
//    #define TCSBUS1_RX_CLK              RCC_APB2Periph_GPIOA
//    #define TCSBUS1_RX_PIN              GPIO_Pin_3
//
//    #define TCSBUS1_RX_CALLBACK         0
//#endif
//
//#if (TCSBUS_NUM > 2)
//    #define TCSBUS2_TX_PORT             GPIOB
//    #define TCSBUS2_TX_CLK              RCC_APB2Periph_GPIOB
//    #define TCSBUS2_TX_PIN              GPIO_Pin_0
//
//    #define TCSBUS2_RX_PORT             GPIOB
//    #define TCSBUS2_RX_CLK              RCC_APB2Periph_GPIOB
//    #define TCSBUS2_RX_PIN              GPIO_Pin_1
//
//    #define TCSBUS2_RX_CALLBACK         0
//#endif
//
///// TCSBUS总线名称类型定义
//typedef enum
//{
//    ETCSBUS = 0,                                                                ///< 命名为易于理解的TCSBUS名称, 值0对应TCSBUS0_xxx
//    LTCSBUS,                                                                    ///< 值1对应TCSBUS1_xxx, 以下类推
//    STCSBUS,
//} tcsbus_name_t;



#if (TCSBUS_NUM > 3)
    #error "TCSBUS_NUM define error!"
#endif

/**
  * @}
  */



/** @defgroup DRV_TCSBUS_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
/// TCSBUS总线协议类型定义
typedef enum
{
    A_LONG  = (0x00 | 0x01),                                                    ///< A长协议, 32bit
    A_SHORT = (0x00 | 0x00),                                                    ///< A短协议, 16bit
    Q_LONG  = (0x10 | 0x01),                                                    ///< Q长协议, 32bit
    Q_SHORT = (0x10 | 0x00),                                                    ///< Q短协议, 4bit
    Q_NONE  = (0x10 | 0x0F),                                                    ///< 等待Q协议超时
} tcsbus_type_t;
/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (TCSBUS_NUM > 0)

void tcsbus_init_all(void);                                                     // 初始化所有 TCSBUS 总线
void tcsbus_init(tcsbus_name_t tcsbus_name);                                    // 初始化指定 TCSBUS 总线
void tcsbus_scan_200us(tcsbus_name_t tcsbus_name);                              // TCSBUS 200us定时扫描函数

uint8_t tcsbus_tx(tcsbus_name_t tcsbus_name, tcsbus_type_t type, uint32_t data);// TCSBUS 发送协议, 返回0表示发送成功

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
