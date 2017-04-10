/**
  ******************************************************************************
  * @file    drv_sdflash.h
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   将SD卡作为FLASH使用的驱动程序.
  *          支持 SD_V1.x, SD_V2, SDHC卡.
  *          支持SPI模式 和 SDIO模式, 否则会导致读写错误. 原因不明.
  *          由于地址使用的是 uint32_t 格式, 因此最大可支持到4G的卡.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_SDFLASH_H
#define __DRV_SDFLASH_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_SDFLASH
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_SDFLASH_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define SDFLASH_ENABLE                                                          ///< 是否使能SD FLASH驱动

//#define SDFLASH_POWER_NAME            SDFLASH_POWER                             ///< SD FLASH电源控制引脚名称, 没有电源控制不要宏定义此项

/** @defgroup SDFLASH_Pin_Assignment
  * @brief    SD引脚对应关系, 如下图:
  *           +-------------------------------------------------------+
  *           |                     Pin assignment                    |
  *           +-------------------------+---------------+-------------+
  *           | STM32        Pins       |     SD        |    Pin      |
  *           +-------------------------+---------------+-------------+
  *           |                         |   CD DAT3     |    1        |
  *           |                         |   CMD         |    2        |
  *           |                         |   GND         |    3 (0 V)  |
  *           |                         |   VDD         |    4 (3.3 V)|
  *           |                         |   CLK         |    5        |
  *           |                         |   GND         |    6 (0 V)  |
  *           |                         |   DAT0        |    7        |
  *           |                         |   DAT1        |    8        |
  *           |                         |   DAT2        |    9        |
  *           +-------------------------+---------------+-------------+
  *           |                         |    卡座       |             |
  *           +-------------------------+---------------+-------------+
  *           | SDFLASH_DETECT_PIN      |   CD          |    10       |
  *           |                         |   GND         |    11 (0 V) |
  *           |                         |   WP          |    12       |
  *           +-------------------------+---------------+-------------+
  * @{
  */
//#define SDFLASH_DETECT_ENABLE                                                   ///< 一般卡座都可检测SD卡是否存在
//#define SDFLASH_DETECT_PORT             GPIOC                                   ///< SD FLASH座CD引脚, SD卡存在检测. PORT口
//#define SDFLASH_DETECT_CLK              RCC_APB2Periph_GPIOC                    ///< SD FLASH座CD引脚, SD卡存在检测. 时钟模块
//#define SDFLASH_DETECT_PIN              GPIO_Pin_11                             ///< SD FLASH座CD引脚, SD卡存在检测. PIN口
/**
  * @}
  */

#if   (defined SDFLASH_MODE_SDIO)
    #include "sd_sdio.h"
#elif (defined SDFLASH_MODE_SPIDMA)
    #include "sd_sdspidma.h"
#elif (defined SDFLASH_MODE_SPI)
    #include "sd_sdspi.h"
#endif

#if  ((defined SDFLASH_MODE_SDIO)   && (defined SDFLASH_MODE_SPIDMA))
  || ((defined SDFLASH_MODE_SDIO)   && (defined SDFLASH_MODE_SPI))
  || ((defined SDFLASH_MODE_SPIDMA) && (defined SDFLASH_MODE_SPI))

    #error "SDFLASH_MODE define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_SDFLASH_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
/// SD卡类型定义
typedef enum
{
    SDFLASH_V1     = 0x00,                                                      ///< V1.x版本的SD卡
    SDFLASH_V2     = 0x01,                                                      ///< V2.0版本的SD卡
    SDFLASH_SDHC   = 0x02,                                                      ///< SDHC卡
    SDFLASH_MMC    = 0x03,                                                      ///< MMC卡
    SDFLASH_UNKNOW = 0xFF,                                                      ///< 不明类型
} sdflash_type_t;

/// SD卡信息类型定义
typedef struct
{
    sdflash_type_t type;                                                        ///< SD卡类型
    uint8_t  revision;                                                          ///< SD卡版本, 使用BCD码. 如0x32表示3.2
    uint32_t serial;                                                            ///< 序列号
    uint8_t  year;                                                              ///< 生产年份, 0表示2000年, 后类推
    uint8_t  month;                                                             ///< 生产月份
    uint64_t capacity;                                                          ///< SD卡容量, 单位为字节
} sdflash_info_t;
/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Public_Function
  * @brief    定义公有函数
  * @{
  */
#ifdef SDFLASH_ENABLE

uint8_t sdflash_init(void);                                                     // 打开SD FLASH电源并初始化. 0, 成功. 1, 失败
void    sdflash_power_off(void);                                                // 关闭SD FLASH电源
uint8_t sdflash_get_info(sdflash_info_t *info);                                 // 读取SD FLASH信息. 0, 成功. 1, 失败

// 以下函数直接调用即可. 无需考虑SD卡必须按块读写的特性
uint8_t sdflash_read(uint8_t *pbuf, uint32_t read_addr, uint32_t num);          // 从sd flash中读取数据
uint8_t sdflash_write_buf(uint8_t *pbuf, uint32_t write_addr, uint32_t num);    // 写入数据, 未满一个Sector的数据暂存在buf中
void    sdflash_write_buf2flash(void);                                          // 将buf中的数据写入到flash中

// 以下函数为SD卡的块读写函数, 可供特殊情况使用
uint8_t sdflash_read_blocks(uint8_t* pbuf, uint32_t block, uint32_t block_num);         // SD卡按块读取, block为块的起始数
uint8_t sdflash_write_blocks(const uint8_t* pbuf, uint32_t block, uint32_t block_num);  // SD卡按块写入, block为块的起始数

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
