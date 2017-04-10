/**
  ******************************************************************************
  * @file    enc28j60_uip.h
  * @author  mousie-yu
  * @version V1.1.1
  * @date    2013.5.5
  * @brief   ENC28J60底层驱动函数. 针对UIP组件书写.
  *          ENC28J60 为 Microchip Technology, Inc. 生产.
  *          其MAC位址高三位(OUI)为 00-04-A3, 低三位由芯片供应商提供.
  *          此芯片不支持自动检测网络, 通讯不稳定, 不建议使用!
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENC28J60_UIP_H
#define __ENC28J60_UIP_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "tapdev.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "drv_extint.h"
#include "drv_spi.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup ENC28J60_UIP
  * @{
  */



/**
  ******************************************************************************
  * @defgroup ENC28J60_UIP_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

/** @defgroup ENC28J60_UIP_Pin_Assignment
  * @brief    ENC28J60 引脚对应关系, 如下图:
  *           +---------------------------------------------------------+
  *           |                       Pin assignment                    |
  *           +-------------------------+---------------+---------------+
  *           | STM32 Pins              |   ENC28J60    | Pin(SOIC/QFN) |
  *           +-------------------------+---------------+---------------+
  *           | ENC28J60_INT_PIN        |    _INT       |     4 / 28    |
  *           | ENC28J60_SPI_MISO_PIN   |    SO         |     6 / 2     |
  *           | ENC28J60_SPI_MOSI_PIN   |    SI         |     7 / 3     |
  *           | ENC28J60_SPI_SCK_PIN    |    SCK        |     8 / 4     |
  *           | ENC28J60_CS_PIN         |    _CS        |     9 / 5     |
  *           | ENC28J60_RESET_PIN      |    _RESET     |    10 / 6     |
  *           +-------------------------+---------------+---------------+
  * @{
  */
//#define ENC28J60_UIP_ENABLE                                                     ///< 使用enc28j60作为UIP的网络驱动芯片
//
//#ifdef ENC28J60_UIP_ENABLE
//    #define ENC28J60_INT_NAME           ENC28J60_INT                            ///< 若uIP使用中断模式, 则需一外部中断, 在"drv_extint.h"中配置
//    #define ENC28J60_SPI_NAME           ENC28J60_SPI                            ///< 该驱动需使用一个SPI, SPI的SCK, MISO, MOSI口在"drv_spi.h"中配置
//    #define ENC28J60_CS_PORT            GPIOB                                   ///< ENC28J60片选信号的PORT口
//    #define ENC28J60_CS_CLK             RCC_APB2Periph_GPIOB                    ///< ENC28J60片选信号的时钟模块
//    #define ENC28J60_CS_PIN             GPIO_Pin_12                             ///< ENC28J60片选信号的PIN口
//#endif
/**
  * @}
  */

/**
  * @}
  */



/** @defgroup ENC28J60_UIP_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup ENC28J60_UIP_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup ENC28J60_UIP_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup ENC28J60_UIP_Public_Function
  * @brief    定义公有函数
  * @{
  */
#ifdef ENC28J60_UIP_ENABLE

void enc28j60_port_init(void);
void enc28j60_init(uip_macaddr_t mac);
void enc28j60_tx(uint8_t *pdata, uint16_t len);
uint16_t enc28j60_rx(uint8_t *pdata, uint16_t maxlen);
uint8_t  enc28j60_linked(void);

#if !UIP_TASK_RX_SCAN_MS
void enc28j60_isr(void);
#endif

#endif // ENC28J60_UIP_ENABLE
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
