/**
  ******************************************************************************
  * @file    drv_spi.h
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.15
  * @brief   提供 SPI 驱动程序. 主机模式. 同时支持IO口软件模拟和硬件SPI.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_SPI_H
#define __DRV_SPI_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_SPI
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_SPI_Configure
  * @brief    用户配置
  * @note     该驱动不控制SPI的SS引脚, SS引脚由调用SPI的函数控制
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define SPI_NUM                         5                                       ///< 设置使用的SPI数量.该驱动最多允许5个SPI数
//
//#if (SPI_NUM > 0)
//    /**
//      * @brief 设置该SPI的外设名称
//      *   @arg 0,                       使用IO口模拟SPI
//      *   @arg SPI1,                    使用SPI1外设, SPI1的CLK为 RCC_APB2Periph_SPI1
//      *   @arg SPI2,                    使用SPI2外设, 其余的CLK为 RCC_APB1Periph_XXX
//      *   @arg SPI3,                    使用SPI3外设
//      */
//    #define SPI0_PERIPHERAL             SPI1                                    ///< 设置该SPI的芯片外设名, 如果用软件模拟SPI, 设为0
//    #define SPI0_CLK                    RCC_APB2Periph_SPI1                     ///< 设置该SPI的时钟模块,   如果用软件模拟SPI, 设为0
//
//    #define SPI0_SCK_PORT               GPIOA                                   ///< 设置该SPI的SCK引脚PORT口
//    #define SPI0_SCK_CLK                RCC_APB2Periph_GPIOA                    ///< 设置该SPI的SCK引脚时钟模块
//    #define SPI0_SCK_PIN                GPIO_Pin_5                              ///< 设置该SPI的SCK引脚PIN口
//
//    #define SPI0_MISO_PORT              GPIOA                                   ///< 设置该SPI的MISO引脚PORT口
//    #define SPI0_MISO_CLK               RCC_APB2Periph_GPIOA                    ///< 设置该SPI的MISO引脚时钟模块
//    #define SPI0_MISO_PIN               GPIO_Pin_6                              ///< 设置该SPI的MISO引脚PIN口
//
//    #define SPI0_MOSI_PORT              GPIOA                                   ///< 设置该SPI的MOSI引脚PORT口
//    #define SPI0_MOSI_CLK               RCC_APB2Periph_GPIOA                    ///< 设置该SPI的MOSI引脚时钟模块
//    #define SPI0_MOSI_PIN               GPIO_Pin_7                              ///< 设置该SPI的MOSI引脚PIN口
//
//    /**
//      * @brief 设置该SPI的引脚重映射功能. 与使用的IO口有关.
//      *   @arg 0,                       不使用引脚重映射功能
//      *   @arg GPIO_Remap_SPI1,         SPI1 引脚重映射
//      *   @arg GPIO_Remap_SPI3,         SPI3 引脚重映射
//      */
//    #define SPI0_GPIO_REMAP             0
//#endif
//
//#if (SPI_NUM > 1)
//    #define SPI1_PERIPHERAL             SPI2
//    #define SPI1_CLK                    RCC_APB1Periph_SPI2
//
//    #define SPI1_SCK_PORT               GPIOB
//    #define SPI1_SCK_CLK                RCC_APB2Periph_GPIOB
//    #define SPI1_SCK_PIN                GPIO_Pin_13
//
//    #define SPI1_MISO_PORT              GPIOB
//    #define SPI1_MISO_CLK               RCC_APB2Periph_GPIOB
//    #define SPI1_MISO_PIN               GPIO_Pin_14
//
//    #define SPI1_MOSI_PORT              GPIOB
//    #define SPI1_MOSI_CLK               RCC_APB2Periph_GPIOB
//    #define SPI1_MOSI_PIN               GPIO_Pin_15
//
//    #define SPI1_GPIO_REMAP             0
//#endif
//
//#if (SPI_NUM > 2)
//    #define SPI2_PERIPHERAL             SPI2
//    #define SPI2_CLK                    RCC_APB1Periph_SPI2
//
//    #define SPI2_SCK_PORT               GPIOB
//    #define SPI2_SCK_CLK                RCC_APB2Periph_GPIOB
//    #define SPI2_SCK_PIN                GPIO_Pin_13
//
//    #define SPI2_MISO_PORT              GPIOB
//    #define SPI2_MISO_CLK               RCC_APB2Periph_GPIOB
//    #define SPI2_MISO_PIN               GPIO_Pin_14
//
//    #define SPI2_MOSI_PORT              GPIOB
//    #define SPI2_MOSI_CLK               RCC_APB2Periph_GPIOB
//    #define SPI2_MOSI_PIN               GPIO_Pin_15
//
//    #define SPI2_GPIO_REMAP             0
//#endif
//
//#if (SPI_NUM > 3)
//    #define SPI3_PERIPHERAL             SPI2
//    #define SPI3_CLK                    RCC_APB1Periph_SPI2
//
//    #define SPI3_SCK_PORT               GPIOB
//    #define SPI3_SCK_CLK                RCC_APB2Periph_GPIOB
//    #define SPI3_SCK_PIN                GPIO_Pin_13
//
//    #define SPI3_MISO_PORT              GPIOB
//    #define SPI3_MISO_CLK               RCC_APB2Periph_GPIOB
//    #define SPI3_MISO_PIN               GPIO_Pin_14
//
//    #define SPI3_MOSI_PORT              GPIOB
//    #define SPI3_MOSI_CLK               RCC_APB2Periph_GPIOB
//    #define SPI3_MOSI_PIN               GPIO_Pin_15
//
//    #define SPI3_GPIO_REMAP             0
//#endif
//
//#if (SPI_NUM > 4)
//    #define SPI4_PERIPHERAL             0
//    #define SPI4_CLK                    0
//
//    #define SPI4_SCK_PORT               GPIOB
//    #define SPI4_SCK_CLK                RCC_APB2Periph_GPIOB
//    #define SPI4_SCK_PIN                GPIO_Pin_13
//
//    #define SPI4_MISO_PORT              GPIOB
//    #define SPI4_MISO_CLK               RCC_APB2Periph_GPIOB
//    #define SPI4_MISO_PIN               GPIO_Pin_14
//
//    #define SPI4_MOSI_PORT              GPIOB
//    #define SPI4_MOSI_CLK               RCC_APB2Periph_GPIOB
//    #define SPI4_MOSI_PIN               GPIO_Pin_15
//
//    #define SPI4_GPIO_REMAP             0
//#endif
//
///// SPI名称类型定义
//typedef enum
//{
//    SD_SPI     = 0,                                                             ///< 命名为易于理解的SPI名称, 值0对应SPI0_xxx
//    SFLASH_SPI,                                                                 ///< 值1对应SPI1_xxx, 以下类推
//    USER0_SPI,
//    USER1_SPI,
//    USER2_SPI,
//} spi_name_t;



#if (SPI_NUM > 5)
    #error "SPI_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_SPI_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @brief  SPI的传输类型定义
  *
  *   @arg  SPI_TYPE_HIGH_EDGE1_MSB, SCK默认高电平, 下降沿检测, MSB模式
  *         SCK　       ----↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑----
  *         MOSI / MISO --BIT7---XXX---XXX---XXX---XXX---XXX---XXX---BIT0-----
  *
  *   @arg  SPI_TYPE_HIGH_EDGE1_LSB, SCK默认高电平, 下降沿检测, LSB模式
  *         SCK　       ----↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑----
  *         MOSI / MISO --BIT0---XXX---XXX---XXX---XXX---XXX---XXX---BIT7-----
  *
  *   @arg  SPI_TYPE_HIGH_EDGE2_MSB, SCK默认高电平, 上升沿检测, MSB模式
  *         SCK　       ----↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑----
  *         MOSI / MISO -----BIT7---XXX---XXX---XXX---XXX---XXX---XXX---BIT0--
  *
  *   @arg  SPI_TYPE_HIGH_EDGE2_LSB, SCK默认高电平, 上升沿检测, LSB模式
  *         SCK　       ----↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑----
  *         MOSI / MISO -----BIT0---XXX---XXX---XXX---XXX---XXX---XXX---BIT7--
  *
  *   @arg  SPI_TYPE_LOW_EDGE1_MSB,  SCK默认低电平, 上升沿检测, MSB模式
  *         SCK　       ____↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓____
  *         MOSI / MISO --BIT7---XXX---XXX---XXX---XXX---XXX---XXX---BIT0-----
  *
  *   @arg  SPI_TYPE_LOW_EDGE1_LSB,  SCK默认低电平, 上升沿检测, LSB模式
  *         SCK　       ____↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓____
  *         MOSI / MISO --BIT0---XXX---XXX---XXX---XXX---XXX---XXX---BIT7-----
  *
  *   @arg  SPI_TYPE_LOW_EDGE2_MSB,  SCK默认低电平, 下降沿检测, MSB模式
  *         SCK　       ____↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓____
  *         MOSI / MISO -----BIT7---XXX---XXX---XXX---XXX---XXX---XXX---BIT0--
  *
  *   @arg  SPI_TYPE_LOW_EDGE2_LSB,  SCK默认低电平, 下降沿检测, LSB模式
  *         SCK　       ____↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓__↑--↓____
  *         MOSI / MISO -----BIT0---XXX---XXX---XXX---XXX---XXX---XXX---BIT7--
  */
typedef enum
{
    SPI_TYPE_HIGH_EDGE1_MSB = (0x80 | 0x04 | 0x00 | 0x00),
    SPI_TYPE_HIGH_EDGE1_LSB = (0x80 | 0x04 | 0x00 | 0x01),
    SPI_TYPE_HIGH_EDGE2_MSB = (0x80 | 0x04 | 0x02 | 0x00),
    SPI_TYPE_HIGH_EDGE2_LSB = (0x80 | 0x04 | 0x02 | 0x01),
    SPI_TYPE_LOW_EDGE1_MSB  = (0x80 | 0x00 | 0x00 | 0x00),
    SPI_TYPE_LOW_EDGE1_LSB  = (0x80 | 0x00 | 0x00 | 0x01),
    SPI_TYPE_LOW_EDGE2_MSB  = (0x80 | 0x00 | 0x02 | 0x00),
    SPI_TYPE_LOW_EDGE2_LSB  = (0x80 | 0x00 | 0x02 | 0x01),
} spi_type_t;

#define IS_SPI_TYPE(spi_type)         ( ((spi_type) == SPI_TYPE_HIGH_EDGE1_MSB) || \
                                        ((spi_type) == SPI_TYPE_HIGH_EDGE1_LSB) || \
                                        ((spi_type) == SPI_TYPE_HIGH_EDGE2_MSB) || \
                                        ((spi_type) == SPI_TYPE_HIGH_EDGE2_LSB) || \
                                        ((spi_type) == SPI_TYPE_LOW_EDGE1_MSB ) || \
                                        ((spi_type) == SPI_TYPE_LOW_EDGE1_LSB ) || \
                                        ((spi_type) == SPI_TYPE_LOW_EDGE2_MSB ) || \
                                        ((spi_type) == SPI_TYPE_LOW_EDGE2_LSB ) )
/**
  * @}
  */

/** @defgroup DRV_SPI_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SPI_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SPI_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (SPI_NUM > 0)

/**
  ******************************************************************************
  * @brief  SPI初始化函数, 必须在初始化完成后再配置SPI的SS引脚
  * @param  spi_name
  *   @arg  可用值请参考 spi_name_t 类型定义中的枚举值
  * @param  spi_type, 设置SPI的传输类型
  *   @arg  SPI_TYPE_HIGH_EDGE1_MSB,    SCK默认高电平, 下降沿检测, MSB模式
  *   @arg  SPI_TYPE_HIGH_EDGE1_LSB,    SCK默认高电平, 下降沿检测, LSB模式
  *   @arg  SPI_TYPE_HIGH_EDGE2_MSB,    SCK默认高电平, 上升沿检测, MSB模式
  *   @arg  SPI_TYPE_HIGH_EDGE2_LSB,    SCK默认高电平, 上升沿检测, LSB模式
  *   @arg  SPI_TYPE_LOW_EDGE1_MSB,     SCK默认低电平, 上升沿检测, MSB模式
  *   @arg  SPI_TYPE_LOW_EDGE1_LSB,     SCK默认低电平, 上升沿检测, LSB模式
  *   @arg  SPI_TYPE_LOW_EDGE2_MSB,     SCK默认低电平, 下降沿检测, MSB模式
  *   @arg  SPI_TYPE_LOW_EDGE2_LSB,     SCK默认低电平, 下降沿检测, LSB模式
  * @param  baud_rate, 设置SPI波特率.
  *   对于硬件SPI, SPI波特率通过对APB总线预分频设置, 有效值如下:
  *   @arg   SPI_BaudRatePrescaler_2,   对APB总线2分频后作为SPI时钟信号
  *   @arg   SPI_BaudRatePrescaler_4,   对APB总线4分频后作为SPI时钟信号
  *   @arg   SPI_BaudRatePrescaler_8,   按上类推
  *   @arg   SPI_BaudRatePrescaler_16
  *   @arg   SPI_BaudRatePrescaler_32
  *   @arg   SPI_BaudRatePrescaler_64
  *   @arg   SPI_BaudRatePrescaler_128
  *   @arg   SPI_BaudRatePrescaler_256
  *   对于软件模拟SPI, 直接设置其波特率值, 单位为Khz.
  *   @arg   1-500,                      软件模拟SPI波特率有效值 1-500 Khz.
  *   @arg   0,                          表示不延时. 可达约2MHz
  * @retval none
  ******************************************************************************
  */
void spi_init(spi_name_t spi_name, spi_type_t spi_type, uint16_t baud_rate);
void spi_deinit(spi_name_t spi_name);                                           // SPI禁用函数
uint8_t spi_tx_rx_byte(spi_name_t spi_name, uint8_t data);                      // SPI收发一个字节

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
