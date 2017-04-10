/**
  ******************************************************************************
  * @file    drv_i2c.c
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.16
  * @brief   提供 I2C 驱动程序. 主机模式, 7位地址. 同时支持IO口软件模拟和硬件I2C.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_I2C_H
#define __DRV_I2C_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_I2C
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_I2C_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define I2C_NUM                         5                                       ///< 设置使用的I2C数量.该驱动最多允许5个I2C数
//
//#if (I2C_NUM > 0)
//    /**
//      * @brief 设置该I2C的外设名称
//      *   @arg 0,                       使用IO口模拟SPI
//      *   @arg I2C1,                    使用I2C1外设
//      *   @arg I2C2,                    使用I2C2外设
//      */
//    #define I2C0_PERIPHERAL             I2C1                                    ///< 设置该I2C的模块名,   如果用软件模拟I2C, 设为0
//    #define I2C0_CLK                    RCC_APB1Periph_I2C1                     ///< 设置该I2C的时钟模块, 如果用软件模拟I2C, 设为0
//
//    #define I2C0_SCL_PORT               GPIOB                                   ///< 设置该I2C的SCL引脚的PORT口
//    #define I2C0_SCL_CLK                RCC_APB2Periph_GPIOB                    ///< 设置该I2C的SCL引脚的时钟模块
//    #define I2C0_SCL_PIN                GPIO_Pin_6                              ///< 设置该I2C的SCL引脚的PIN口
//
//    #define I2C0_SDA_PORT               GPIOB                                   ///< 设置该I2C的SDA引脚的PORT口
//    #define I2C0_SDA_CLK                RCC_APB2Periph_GPIOB                    ///< 设置该I2C的SDA引脚的时钟模块
//    #define I2C0_SDA_PIN                GPIO_Pin_7                              ///< 设置该I2C的SDA引脚的PIN口
//
//    /**
//      * @brief 设置I2C引脚重映射功能. 更多信息请参考官方数据手册.
//      *        与I2C相关的配置如下:
//      *   @arg 0,                       不使用引脚重映射功能
//      *   @arg GPIO_Remap_I2C1,         I2C1 引脚重映射
//      */
//    #define I2C0_GPIO_REMAP             0
//#endif
//
//#if (I2C_NUM > 1)
//    #define I2C1_PERIPHERAL             I2C2
//    #define I2C1_CLK                    RCC_APB1Periph_I2C2
//
//    #define I2C1_SCL_PORT               GPIOB
//    #define I2C1_SCL_CLK                RCC_APB2Periph_GPIOB
//    #define I2C1_SCL_PIN                GPIO_Pin_10
//
//    #define I2C1_SDA_PORT               GPIOB
//    #define I2C1_SDA_CLK                RCC_APB2Periph_GPIOB
//    #define I2C1_SDA_PIN                GPIO_Pin_11
//
//    #define I2C1_GPIO_REMAP             0
//#endif
//
//#if (I2C_NUM > 2)
//    #define I2C2_PERIPHERAL             0
//    #define I2C2_CLK                    0
//
//    #define I2C2_SCL_PORT               GPIOB
//    #define I2C2_SCL_CLK                RCC_APB2Periph_GPIOB
//    #define I2C2_SCL_PIN                GPIO_Pin_6
//
//    #define I2C2_SDA_PORT               GPIOB
//    #define I2C2_SDA_CLK                RCC_APB2Periph_GPIOB
//    #define I2C2_SDA_PIN                GPIO_Pin_7
//
//    #define I2C2_GPIO_REMAP             0
//#endif
//
//#if (I2C_NUM > 3)
//    #define I2C3_PERIPHERAL             0
//    #define I2C3_CLK                    0
//
//    #define I2C3_SCL_PORT               GPIOB
//    #define I2C3_SCL_CLK                RCC_APB2Periph_GPIOB
//    #define I2C3_SCL_PIN                GPIO_Pin_6
//
//    #define I2C3_SDA_PORT               GPIOB
//    #define I2C3_SDA_CLK                RCC_APB2Periph_GPIOB
//    #define I2C3_SDA_PIN                GPIO_Pin_7
//
//    #define I2C3_GPIO_REMAP             0
//#endif
//
//#if (I2C_NUM > 4)
//    #define I2C4_PERIPHERAL             0
//    #define I2C4_CLK                    0
//
//    #define I2C4_SCL_PORT               GPIOB
//    #define I2C4_SCL_CLK                RCC_APB2Periph_GPIOB
//    #define I2C4_SCL_PIN                GPIO_Pin_6
//
//    #define I2C4_SDA_PORT               GPIOB
//    #define I2C4_SDA_CLK                RCC_APB2Periph_GPIOB
//    #define I2C4_SDA_PIN                GPIO_Pin_7
//
//    #define I2C4_GPIO_REMAP             0
//#endif
//
///// I2C名称类型定义
//typedef enum
//{
//    EEPROM_I2C = 0,                                                             ///< 命名为易于理解的I2C名称, 值0对应I2C0_xxx
//    USER1_I2C,                                                                  ///< 值1对应I2C1_xxx, 以下类推
//    USER2_I2C,
//    USER3_I2C,
//    USER4_I2C,
//} i2c_name_t;



#if (I2C_NUM > 5)
    #error "I2C_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_I2C_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
/// I2C应答类型定义
typedef enum
{
    I2C_ACK    = 0,                                                             ///< I2C 有应答信号
    I2C_NO_ACK = 1,                                                             ///< I2C 无应答信号
} i2c_ack_t;
/**
  * @}
  */

/** @defgroup DRV_I2C_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2C_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2C_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (I2C_NUM > 0)

void i2c_init(i2c_name_t i2c_name, uint16_t baud_rate_khz);                     // I2C初始化
void i2c_deinit(i2c_name_t i2c_name);                                           // 禁用I2C
void i2c_start(i2c_name_t i2c_name);                                            // 产生I2C起始信号
void i2c_stop(i2c_name_t i2c_name);                                             // 产生I2C结束信号
i2c_ack_t i2c_tx_addr_wr_slaver(i2c_name_t i2c_name, uint8_t addr);             // I2C准备写从设备
i2c_ack_t i2c_tx_addr_rd_slaver(i2c_name_t i2c_name, uint8_t addr);             // I2C准备读从设备
i2c_ack_t i2c_tx_byte(i2c_name_t i2c_name, uint8_t data);                       // 通过I2C发送一个字节
uint8_t i2c_rx_byte(i2c_name_t i2c_name, i2c_ack_t ack);                        // 通过I2C接收一个字节

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
