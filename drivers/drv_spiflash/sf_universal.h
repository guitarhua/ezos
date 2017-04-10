/**
  ******************************************************************************
  * @file    sf_universal.h
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2011.12.23
  * @brief   SPI Flash 通用驱动. 使用标准的SPI通讯协议. 兼容大多数厂商的SPI FLASH.
  *          此驱动用于支持 drv_spiflash. 请勿直接调用.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SF_UNIVERSAL_H
#define __SF_UNIVERSAL_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "drv_spi.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup SF_Universal
  * @{
  */



/**
  ******************************************************************************
  * @defgroup SF_Universal_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

/** @defgroup SF_Universal_Pin_Assignment
  * @brief    SPI Flash引脚对应关系, 如下图:
  *           +-------------------------------------------------------+
  *           |                     Pin assignment                    |
  *           +-------------------------+---------------+-------------+
  *           | STM32 SPI Pins          |   SPI Flash   |    Pin      |
  *           +-------------------------+---------------+-------------+
  *           | SPIFLASH_CS_PIN         |   /CS         |    1        |
  *           | SPIFLASH_MISO_PIN       |   DO          |    2        |
  *           |                         |   /WP         |    3 (GND)  |
  *           |                         |   GND         |    4 (GND)  |
  *           |                         |   VCC         |    5 (3.3V) |
  *           |                         |   /HOLD       |    6 (VCC)  |
  *           | SPIFLASH_CLK_PIN        |   CLK         |    7        |
  *           | SPIFLASH_MOSI_PIN       |   DIO         |    8        |
  *           +-------------------------+---------------+-------------+
  * @{
  */
//#define SPIFLASH_UNIVERSAL
//
//#ifdef SPIFLASH_UNIVERSAL                                                       ///< 选择SPI FLASH通用驱动
//    #define SPIFLASH_SPI_NAME           FLASH_SPI                               ///< 该驱动需使用一个SPI, SPI的SCK, MISO, MOSI口在"drv_spi.h"中配置
//
//    #define SPIFLASH_CS_PORT            GPIOA                                   ///< SpiFlash 片选信号的PORT口
//    #define SPIFLASH_CS_CLK             RCC_APB2Periph_GPIOA                    ///< SpiFlash 片选信号的时钟模块
//    #define SPIFLASH_CS_PIN             GPIO_Pin_4                              ///< SpiFlash 片选信号的PIN口
//#endif
/**
  * @}
  */

/**
  * @}
  */



/** @defgroup SF_Universal_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
/**
  * @}
  */

/** @defgroup SF_Universal_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */
#ifdef SPIFLASH_UNIVERSAL

// 写保护标记位宏定义(Status Register)
#define BP3                             0x20
#define BP2                             0x10
#define BP1                             0x08
#define BP0                             0x04

#endif
/**
  * @}
  */

/** @defgroup SF_Universal_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup SF_Universal_Public_Function
  * @brief    定义公有函数
  * @{
  */
#ifdef SPIFLASH_UNIVERSAL

// 以下函数供 drv_spiflash 使用. 请勿直接调用.
void sf_unvsl_init(void);                                                       // SpiFlash Universal 通讯口初始化
/**
  ******************************************************************************
  * 设置写保护区域
  * 每块芯片可保护区域都不一样, 如有需要可参考数据手册自行定义
  * 下面仅列出2种最常用的保护方式: 全部写保护, 全部可写
  ******************************************************************************
  */
#define SPIFLASH_PROTECT_ALL            (BP3 | BP2 | BP1 | BP0)
#define SPIFLASH_PROTECT_NONE           0
void sf_unvsl_set_protect(uint8_t status);
void sf_unvsl_power_down(void);                                                 // 芯片进入休眠模式
void sf_unvsl_power_on(void);                                                   // 芯片进入退出休眠模式
uint32_t sf_unvsl_read_id(void);                                                // 读取ID信息

void sf_unvsl_sector_erase(uint32_t addr);                                      // 擦除一个区, 大小为4K byte
void sf_unvsl_block_erase(uint32_t addr);                                       // 擦除一个块, 大小为64K byte
void sf_unvsl_chip_erase(void);                                                 // 擦除整个芯片内容, 请自行延时5-100s等待其完成擦除.

void sf_unvsl_write_page(uint8_t *pbuf, uint32_t addr, uint32_t num);           // 写入一个页

void sf_unvsl_read_start(uint32_t addr);                                        // 准备读取
void sf_unvsl_read_bytes(uint8_t *pbuf, uint32_t num);                          // 读取数据
void sf_unvsl_read_end(void);                                                   // 结束读取

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
