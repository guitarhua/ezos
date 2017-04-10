/**
  ******************************************************************************
  * @file    drv_sdcard.h
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   SD卡驱动程序, 支持 SD_V1.x, SD_V2, SDHC卡.
  *          支持SPI模式 和 SDIO模式, 需要 drv_sdspi 或 drv_sdio 驱动.
  *          由于地址使用的是 uint32_t 格式, 因此最大可支持到4G的卡.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_SDCARD_H
#define __DRV_SDCARD_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_SDCARD
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_SDCARD_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define SDCARD_ENABLE                                                           ///< 是否使能SD卡驱动

//#define SDCARD_POWER_NAME               SDCARD_POWER                            ///< SD卡电源控制引脚名称, 没有电源控制不要宏定义此项

/** @defgroup SDCARD_Pin_Assignment
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
  *           | SDCARD_DETECT_PIN       |   CD          |    10       |
  *           |                         |   GND         |    11 (0 V) |
  *           |                         |   WP          |    12       |
  *           +-------------------------+---------------+-------------+
  * @{
  */
//#define SDCARD_DETECT_ENABLE                                                    ///< 一般卡座都可检测SD卡是否存在
//#define SDCARD_DETECT_PORT              GPIOC                                   ///< SD卡座CD引脚, SD卡存在检测. PORT口
//#define SDCARD_DETECT_CLK               RCC_APB2Periph_GPIOC                    ///< SD卡座CD引脚, SD卡存在检测. 时钟模块
//#define SDCARD_DETECT_PIN               GPIO_Pin_11                             ///< SD卡座CD引脚, SD卡存在检测. PIN口
/**
  * @}
  */

#if   (defined SDCARD_MODE_SDIO)
    #include "sd_sdio.h"
#elif (defined SDCARD_MODE_SPIDMA)
    #include "sd_sdspidma.h"
#elif (defined SDCARD_MODE_SPI)
    #include "sd_sdspi.h"
#endif

#if  ((defined SDCARD_MODE_SDIO)   && (defined SDCARD_MODE_SPIDMA))
  || ((defined SDCARD_MODE_SDIO)   && (defined SDCARD_MODE_SPI))
  || ((defined SDCARD_MODE_SPIDMA) && (defined SDCARD_MODE_SPI))

    #error "SDCARD_MODE define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_SDCARD_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
/// SD卡类型定义
typedef enum
{
    SDCARD_V1     = 0x00,                                                       ///< V1.x版本的SD卡
    SDCARD_V2     = 0x01,                                                       ///< V2.0版本的SD卡
    SDCARD_SDHC   = 0x02,                                                       ///< SDHC卡
    SDCARD_MMC    = 0x03,                                                       ///< MMC卡
    SDCARD_UNKNOW = 0xFF,                                                       ///< 不明类型
} sdcard_type_t;

/// SD卡信息类型定义
typedef struct
{
    sdcard_type_t type;                                                         ///< SD卡类型
    uint8_t  revision;                                                          ///< SD卡版本, 使用BCD码. 如0x32表示3.2
    uint32_t serial;                                                            ///< 序列号
    uint8_t  year;                                                              ///< 生产年份, 0表示2000年, 后类推
    uint8_t  month;                                                             ///< 生产月份
    uint64_t capacity;                                                          ///< SD卡容量, 单位为字节
} sdcard_info_t;
/**
  * @}
  */

/** @defgroup DRV_SDCARD_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDCARD_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDCARD_Public_Function
  * @brief    定义公有函数
  * @{
  */
#ifdef SDCARD_ENABLE

void sdcard_port_init(void);                                                    // SD卡的电源口, SPI口初始化函数
void sdcard_power_on(void);                                                     // 打开SD卡电源
void sdcard_power_off(void);                                                    // 关闭SD卡电源

uint8_t sdcard_init(void);                                                      // 打开SD卡电源并初始化. 0, 成功. 1, 失败
uint8_t sdcard_present(void);                                                   // 检测SD卡是否存在. 0, 不存在. 1, 存在
uint8_t sdcard_get_info(sdcard_info_t *info);                                   // 读取SD卡信息. 0, 成功. 1, 失败

uint8_t sdcard_read_blocks(uint8_t* pbuf, uint32_t block, uint32_t block_num);          // SD卡读块
uint8_t sdcard_write_blocks(const uint8_t* pbuf, uint32_t block, uint32_t block_num);   // SD卡写块

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
