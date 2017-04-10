/**
  ******************************************************************************
  * @file    sd_sdspi.h
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   SD卡驱动程序, 使用SPI模式.
  *          支持MMC卡(未测), SD卡, SDHC卡. 最大支持4G的卡.
  *          此驱动用于支持 drv_sdcard 及 drv_sdflash. 请勿直接调用.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SD_SDSPI_H
#define __SD_SDSPI_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "drv_spi.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup SD_SDSPI
  * @{
  */



/**
  ******************************************************************************
  * @defgroup SD_SDSPI_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

/** @defgroup SDSPI_Pin_Assignment
  * @brief    SD SPI引脚对应关系, 如下图:
  *           +-------------------------------------------------------+
  *           |                     Pin assignment                    |
  *           +-------------------------+---------------+-------------+
  *           | STM32 SD SPI Pins       |     SD        |    Pin      |
  *           +-------------------------+---------------+-------------+
  *           | SD_CS_PIN               |   CD DAT3     |    1        |
  *           | SD_SPI_MOSI_PIN         |   CMD         |    2        |
  *           |                         |   GND         |    3 (GND)  |
  *           |                         |   VDD         |    4 (3.3 V)|
  *           | SD_SPI_SCK_PIN          |   CLK         |    5        |
  *           |                         |   GND         |    6 (GND)  |
  *           | SD_SPI_MISO_PIN         |   DAT0        |    7        |
  *           |                         |   DAT1        |    8        |
  *           |                         |   DAT2        |    9        |
  *           +-------------------------+---------------+-------------+
  * @{
  */
//#define SDCARD_MODE_SPI                                                         ///< drv_sdcard 使用SPI模式通讯
//#define SDFLASH_MODE_SPI                                                        ///< drv_sdflash使用SPI模式通讯
//
//#if (defined SDCARD_MODE_SPI) || (defined SDFLASH_MODE_SPI)
//    #define SD_SPI_NAME                 SD_SPI                                  ///< 该驱动需使用一个SPI, SPI的SCK, MISO, MOSI口在"drv_spi.h"中配置
//    #define SD_CS_PORT                  GPIOB                                   ///< SD卡片选信号的PORT口
//    #define SD_CS_CLK                   RCC_APB2Periph_GPIOB                    ///< SD卡片选信号的时钟模块
//    #define SD_CS_PIN                   GPIO_Pin_12                             ///< SD卡片选信号的PIN口
//#endif

#if ((defined SDCARD_MODE_SPI) && (defined SDFLASH_MODE_SPI))
    #error "SDxxx_MODE_SPI define error!"
#endif
/**
  * @}
  */

/**
  * @}
  */



/** @defgroup SD_SDSPI_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
/// SD卡类型定义
typedef enum
{
    SD_V1 = 0x00,                                                               ///< V1.x版本的SD卡
    SD_V2,                                                                      ///< V2.0版本的SD卡
    SDHC,                                                                       ///< SDHC卡
    MMC,                                                                        ///< MMC卡
    UNKNOW,                                                                     ///< 不明类型
} sd_type_t;

/// SD卡信息类型定义
typedef struct
{
    sd_type_t sd_type;                                                          ///< SD卡类型
    uint8_t  sd_manu;                                                           ///< 生产厂商编号, 由SD卡组织分配
    uint8_t  sd_oem[3];                                                         ///< OEM标示, 由SD卡组织分配
    uint8_t  sd_product[6];                                                     ///< 生产厂商名称
    uint8_t  sd_rev;                                                            ///< SD卡版本, 使用BCD码. 如0x32表示3.2
    uint32_t sd_sn;                                                             ///< 序列号
    uint8_t  sd_year;                                                           ///< 生产年份, 0表示2000年, 后类推
    uint8_t  sd_mon;                                                            ///< 生产月份
    uint64_t sd_cap;                                                            ///< SD卡容量, 单位为字节
} sd_info_t;
/**
  * @}
  */

/** @defgroup SD_SDSPI_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup SD_SDSPI_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup SD_SDSPI_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (defined SDCARD_MODE_SPI) || (defined SDFLASH_MODE_SPI)

// 不要直接调用以下这些函数, SD卡相关函数在 drv_sdcard 中
void sdspi_port_init(void);                                                     // SD卡SPI口初始化函数

uint8_t sdspi_init(uint16_t block_size);                                        // 初始化SD卡进入SPI模式. 0, 成功. 1, 失败
uint8_t sdspi_get_info(sd_info_t *info);                                        // SPI模式读取SD卡信息. 0, 成功. 1, 失败

uint8_t sdspi_read_block(uint8_t* pbuf, uint32_t sector);                                           // SD卡SPI模式读单个块
uint8_t sdspi_read_multi_blocks(uint8_t* pbuf, uint32_t sector, uint32_t block_num);                // SD卡SPI模式读多个块
uint8_t sdspi_write_block(const uint8_t* pbuf, uint32_t sector);                                    // SD卡SPI模式写单个块
uint8_t sdspi_write_multi_blocks(const uint8_t* pbuf, uint32_t sector, uint32_t block_num);         // SD卡SPI模式写多个块

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
