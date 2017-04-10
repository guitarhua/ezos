/**
  ******************************************************************************
  * @file    sf_mx25l.h
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2011.12.23
  * @brief   旺宏 MX25L系列 SPI Flash 驱动程序. 使用 4DTRD 模式读, 4PP模式写.
  *          此驱动用于支持 drv_spiflash. 请勿直接调用.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SF_MX25L_H
#define __SF_MX25L_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup SF_MX25L
  * @{
  */



/**
  ******************************************************************************
  * @defgroup SF_MX25L_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

/** @defgroup SF_MX25L_Pin_Assignment
  * @brief    为追求最快的读取速度, 硬件有严格要求. 不能有上拉电阻.
  *           SIO0-SIO3 必须依次分配在同一PORT口的PIN0-PIN3引脚上.
  *           引脚对应关系, 如下图:
  *           +-----------------------------------------------------+
  *           |                   Pin assignment                    |
  *           +------------------------------+----------------------+
  *           | STM32 SPI Pins               |      MX25L芯片       |
  *           +------------------------------+----------------------+
  *           | MX25L_CS_PIN                 |      /CS             |
  *           | MX25L_SCLK_PIN               |      SCLK            |
  *           | MX25L_SIO0_PIN(必须在PIN0脚) |      SI/SIO0         |
  *           | MX25L_SIO1_PIN(必须在PIN1脚) |      SO/SIO1         |
  *           | MX25L_SIO2_PIN(必须在PIN2脚) |      /WP/SIO2        |
  *           | MX25L_SIO3_PIN(必须在PIN3脚) |      SIO3            |
  *           |                              |      GND             |
  *           |                              |      VCC(3.3V)       |
  *           +------------------------------+----------------------+
  * @{
  */
//#define MX25L_4DTRD_4PP                                                         ///< 选择 MX25L 双边沿4IO读 及 4IO 写的驱动

#ifdef MX25L_4DTRD_4PP
//    #define MX25L_CS_PORT               GPIOD                                   ///< MX25L片选信号的PORT口
//    #define MX25L_CS_CLK                RCC_APB2Periph_GPIOD                    ///< MX25L片选信号的时钟模块
//    #define MX25L_CS_PIN                GPIO_Pin_5                              ///< MX25L片选信号的PIN口
//
//    #define MX25L_SCLK_PORT             GPIOD                                   ///< MX25L时钟信号的PORT口
//    #define MX25L_SCLK_CLK              RCC_APB2Periph_GPIOD                    ///< MX25L时钟信号的时钟模块
//    #define MX25L_SCLK_PIN              GPIO_Pin_4                              ///< MX25L时钟信号的PIN口
//
//    #define MX25L_SIO_PORT              GPIOD                                   ///< MX25L数据线0的PORT口
//    #define MX25L_SIO_CLK               RCC_APB2Periph_GPIOD                    ///< MX25L数据线0的时钟模块

    #define MX25L_SIO0_PIN              GPIO_Pin_0                              ///< MX25L数据线0的PIN口, 必须分配在GPIO_Pin_0脚
    #define MX25L_SIO1_PIN              GPIO_Pin_1                              ///< MX25L数据线1的PIN口, 必须分配在GPIO_Pin_1脚
    #define MX25L_SIO2_PIN              GPIO_Pin_2                              ///< MX25L数据线2的PIN口, 必须分配在GPIO_Pin_2脚
    #define MX25L_SIO3_PIN              GPIO_Pin_3                              ///< MX25L数据线3的PIN口, 必须分配在GPIO_Pin_3脚
#endif
/**
  * @}
  */

/**
  * @}
  */



/** @defgroup SF_MX25L_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
/**
  * @}
  */

/** @defgroup SF_MX25L_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */
#ifdef MX25L_4DTRD_4PP

// 写保护标记位宏定义(Status Register)
#define BP3                             0x20
#define BP2                             0x10
#define BP1                             0x08
#define BP0                             0x04

#endif
/**
  * @}
  */

/** @defgroup SF_MX25L_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup SF_MX25L_Public_Function
  * @brief    定义公有函数
  * @{
  */
#ifdef MX25L_4DTRD_4PP

// 以下函数供 drv_spiflash 使用. 请勿直接调用.
void mx25l_init(void);                                                           // 通讯口初始化
/**
  ******************************************************************************
  * 设置写保护区域
  * 每块芯片可保护区域都不一样, 如有需要可参考数据手册自行定义
  * 下面仅列出2种最常用的保护方式: 全部写保护, 全部可写
  ******************************************************************************
  */
#define MX25L_PROTECT_ALL                (BP3 | BP2 | BP1 | BP0)
#define MX25L_PROTECT_NONE               0
void mx25l_set_protect(uint8_t status);
void mx25l_power_down(void);                                                     // 芯片进入休眠模式
void mx25l_power_on(void);                                                       // 芯片进入退出休眠模式
uint32_t mx25l_read_id(void);                                                    // 读取ID信息

void mx25l_sector_erase(uint32_t addr);                                          // 擦除一个区, 大小为4K byte
void mx25l_block_erase(uint32_t addr);                                           // 擦除一个块, 大小为64K byte
void mx25l_chip_erase(void);                                                     // 擦除整个芯片内容, 请自行延时5-100s等待其完成擦除.

void mx25l_write_page(uint8_t *pbuf, uint32_t addr, uint32_t num);               // 写入一个页

void mx25l_read_start(uint32_t addr);                                            // 准备读取
void mx25l_read_bytes(uint8_t *pbuf, uint32_t num);                              // 读取数据
void mx25l_read_end(void);                                                       // 结束读取

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
