/**
  ******************************************************************************
  * @file    stm32f10x_conf.h
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.16
  * @brief   工程配置文件, 各个模块的独立配置内容统一在此处配置
  *          项目名称： STM32工程模板, 属于示例工程模板.
  *                     提供各个底层驱动的应用范例, 使用ezos.
  *          编译器使用 RVMDK V4.20.03
  *          开发板使用 火牛开发板
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_CONF_H
#define __STM32F10x_CONF_H

/** @addtogroup Applications
  * @{
  */
/** @addtogroup Configure
  * @{
  */



// 项目版本
#define PROJECT_VERSION_MAIN            2
#define PROJECT_VERSION_SUB             4
#define PROJECT_REVISION                0



// 是否使能断言调试, 会增加代码量, 不使能则不进行宏定义
#define ASSERT_ENABLE



// 看门狗配置
//#define IWATCHDOG_ENABLE                                                        ///< 使能独立看门狗
#define IWATCHDOG_TIME_MS               5000                                    ///< 独立看门狗复位近似时间, 有效时间, 100-26000ms

/**
  * 窗口看门狗时间要求及其严格, 请按公式自行计算 WWATCHDOG_VALUE 和 WWATCHDOG_COUNT 的值.
  * 建议值为 WWATCHDOG_VALUE = 0x7F, WWATCHDOG_COUNT = 0x7F, 有效喂狗时间窗口为0ms - 58ms之间.
  * 采用系统默认时钟 PCLK1 = 36Mhz时, 时间窗口(单位为ms)值计算公式如下:
  * WWDG_MAX_MS = 4096 * 8 * (WWATCHDOG_COUNT - 63) / 36000
  * WWDG_MIN_MS = 4096 * 8 * (WWATCHDOG_COUNT - WWATCHDOG_VALUE) / 36000
  * 有效喂狗时间窗口即为 WWDG_MIN_MS - WWDG_MAX_MS 之间, 时间单位为ms
  */
//#define WWATCHDOG_ENABLE                                                        ///< 使能窗口看门狗
#define WWATCHDOG_COUNT                 0x7F                                    ///< 窗口看门狗计数器初始值, 有效值, 0x40 - 0x7F
#define WWATCHDOG_VALUE                 0x7F                                    ///< 窗口看门狗计数器比较值, 有效值, 0x40 - WWATCHDOG_COUNT



// LED硬件配置
#define LED_NUM                         4                                       ///< 设置LED的数量, 该驱动最多允许16个LED灯
#define LED_LEVEL                       1                                       ///< 0, 低电平点亮LED. 1, 高电平点亮LED
#define LED_ADVANCED_ENABLE                                                     ///< 使能LED的高级功能, 包括闪烁和渐变效果

#define LED0_PORT                       GPIOD                                   ///< 设置该LED的PORT口
#define LED0_CLK                        RCC_APB2Periph_GPIOD                    ///< 设置该LED的时钟模块
#define LED0_PIN                        GPIO_Pin_8                              ///< 设置该LED的PIN口

#define LED1_PORT                       GPIOD
#define LED1_CLK                        RCC_APB2Periph_GPIOD
#define LED1_PIN                        GPIO_Pin_9

#define LED2_PORT                       GPIOD
#define LED2_CLK                        RCC_APB2Periph_GPIOD
#define LED2_PIN                        GPIO_Pin_10

#define LED3_PORT                       GPIOD
#define LED3_CLK                        RCC_APB2Periph_GPIOD
#define LED3_PIN                        GPIO_Pin_11

typedef enum
{
    LED1 = 0,
    LED2,
    LED3,
    LED4,
} led_name_t;



// 按键硬件配置
#define KEY_NUM                         2                                       ///< 设置按键数量, 该驱动最多允许16个直按式按键
#define KEY_EXTPULL                                                             ///< 按键有外部上拉电阻

#define KEY0_PORT                       GPIOC                                   ///< 设置该按键的PORT口
#define KEY0_CLK                        RCC_APB2Periph_GPIOC                    ///< 设置该按键的时钟模块
#define KEY0_PIN                        GPIO_Pin_13                             ///< 设置该按键的PIN口

#define KEY1_PORT                       GPIOA
#define KEY1_CLK                        RCC_APB2Periph_GPIOA
#define KEY1_PIN                        GPIO_Pin_8

typedef enum
{
    S2_TAMPER = 0,
    S4_USER,
} key_name_t;



/**
  ******************************************************************************
  * 外部中断配置
  * 使用限制: 5个中断源必须使用不同的 PIN口 以及 中断号!!!
  * 硬件设计: 建议使用 PIN0, PIN1, PIN2, PIN3 和 PIN4.
  *           对于 PIN5-PIN9, 只能取其一. 对于PIN10-PIN15, 也只能取其一.
  *           譬如 PA0, PA1, PA2, PB3, PC4 可以作为5个有效的中断源.
  *           而   PA0, PB0, PC0, PD0, PE0 是不可取的, 软件无法区分这5个外部中断.
  ******************************************************************************
  */
#define EXTINT_NUM                      1                                       ///< 设置外部中断数量.该驱动最多允许5个外部中断

#if (EXTINT_NUM > 0)
    #define EXTINT0_PORT                GPIOA                                   ///< 设置外部中断PORT口
    #define EXTINT0_CLK                 RCC_APB2Periph_GPIOA                    ///< 设置外部中断PORT口时钟模块
    #define EXTINT0_PORT_SOURCE         GPIO_PortSourceGPIOA                    ///< 设置外部中断PORT口源
    #define EXTINT0_PIN                 GPIO_Pin_0                              ///< 设置外部中断PIN口
    #define EXTINT0_PIN_SOURCE          GPIO_PinSource0                         ///< GPIO_Pin_X, 则设置为 GPIO_PinSourceX
    #define EXTINT0_LINE                EXTI_Line0                              ///< GPIO_Pin_X, 则设置为 EXTI_LineX

    /**
      * @brief 设置外部中断的中断号
      *   @arg EXTI0_IRQn,              GPIO_Pin_0 使用此中断号
      *   @arg EXTI1_IRQn,              GPIO_Pin_1 使用此中断号
      *   @arg EXTI2_IRQn,              GPIO_Pin_2 使用此中断号
      *   @arg EXTI3_IRQn,              GPIO_Pin_3 使用此中断号
      *   @arg EXTI4_IRQn,              GPIO_Pin_4 使用此中断号
      *   @arg EXTI9_5_IRQn,            GPIO_Pin_5  - GPIO_Pin_9  使用此中断号
      *   @arg EXTI15_10_IRQn,          GPIO_Pin_10 - GPIO_Pin_15 使用此中断号
      */
    #define EXTINT0_IRQ                 EXTI0_IRQn
    #define EXTINT0_IRQ_HANDLER         EXTI0_IRQHandler                        ///< 外部中断函数名

    /**
      * @brief 设置外部中断回调函数
      *   @arg 0,                       不使用回调函数, 不对外部中断进行处理
      *   @arg void fun(void),          回调函数格式, 宏定义fun函数名称即可
      */
    extern void int_key(void);
    #define EXTINT0_CALLBACK            int_key
    #define EXTINT0_IRQ_PRIORITY        0xF                                     ///< 设置中断优先级. 高2位为可抢占主优先级, 低2位为非抢占的子优先级
#endif

// 外部中断名称类型定义
typedef enum
{
    S3_WAKEUP = 0,
} extint_name_t;



// 控制引脚硬件配置
#define CTRLPORT_NUM                    2                                       ///< 设置控制引脚的数量. 该驱动最多允许16个控制引脚.

#define CTRLPORT0_PORT                  GPIOC                                   ///< 设置该控制引脚的PORT口
#define CTRLPORT0_CLK                   RCC_APB2Periph_GPIOC                    ///< 设置该控制引脚的时钟模块
#define CTRLPORT0_PIN                   GPIO_Pin_8                              ///< 设置该控制引脚的PIN口
#define CTRLPORT0_OFF                   1                                       ///< 设置关闭控制引脚的电平

#define CTRLPORT1_PORT                  GPIOB                                   ///< 设置该控制引脚的PORT口
#define CTRLPORT1_CLK                   RCC_APB2Periph_GPIOB                    ///< 设置该控制引脚的时钟模块
#define CTRLPORT1_PIN                   GPIO_Pin_5                              ///< 设置该控制引脚的PIN口
#define CTRLPORT1_OFF                   0                                       ///< 设置关闭控制引脚的电平

typedef enum
{
    SD_POWER = 0,
    BEEP,
} ctrlport_name_t;



// SPI硬件配置
#define SPI_NUM                         2                                       ///< 设置使用的SPI数量.该驱动最多允许5个SPI数

#if  (SPI_NUM > 0)
    /**
      * @brief 设置该SPI的外设名称
      *   @arg 0,                       使用IO口模拟SPI
      *   @arg SPI1,                    使用SPI1外设, SPI1的CLK为 RCC_APB2Periph_SPI1
      *   @arg SPI2,                    使用SPI2外设, 其余的CLK为 RCC_APB1Periph_XXX
      *   @arg SPI3,                    使用SPI3外设
      */                                                                        ///< 设置该SPI的SCK引脚PORT口
    #define SPI0_PERIPHERAL             SPI2                                    ///< 设置该SPI的SCK引脚时钟模块
    #define SPI0_CLK                    RCC_APB1Periph_SPI2                     ///< 设置该SPI的SCK引脚PIN口

    #define SPI0_SCK_PORT               GPIOB                                   ///< 设置该SPI的MISO引脚PORT口
    #define SPI0_SCK_CLK                RCC_APB2Periph_GPIOB                    ///< 设置该SPI的MISO引脚时钟模块
    #define SPI0_SCK_PIN                GPIO_Pin_13                             ///< 设置该SPI的MISO引脚PIN口

    #define SPI0_MISO_PORT              GPIOB                                   ///< 设置该SPI的MOSI引脚PORT口
    #define SPI0_MISO_CLK               RCC_APB2Periph_GPIOB                    ///< 设置该SPI的MOSI引脚时钟模块
    #define SPI0_MISO_PIN               GPIO_Pin_14                             ///< 设置该SPI的MOSI引脚PIN口

    #define SPI0_MOSI_PORT              GPIOB
    #define SPI0_MOSI_CLK               RCC_APB2Periph_GPIOB
    #define SPI0_MOSI_PIN               GPIO_Pin_15

    /**                                 0
      * @brief 设置该SPI的引脚重映射功能. 与使用的IO口有关.
      *   @arg 0,                       不使用引脚重映射功能
      *   @arg GPIO_Remap_SPI1,         SPI1 引脚重映射
      *   @arg GPIO_Remap_SPI3,         SPI3 引脚重映射
      */
    #define SPI0_GPIO_REMAP             0
#endif

#if  (SPI_NUM > 1)
    #define SPI1_PERIPHERAL             SPI1
    #define SPI1_CLK                    RCC_APB2Periph_SPI1

    #define SPI1_SCK_PORT               GPIOA
    #define SPI1_SCK_CLK                RCC_APB2Periph_GPIOA
    #define SPI1_SCK_PIN                GPIO_Pin_5

    #define SPI1_MISO_PORT              GPIOA
    #define SPI1_MISO_CLK               RCC_APB2Periph_GPIOA
    #define SPI1_MISO_PIN               GPIO_Pin_6

    #define SPI1_MOSI_PORT              GPIOA
    #define SPI1_MOSI_CLK               RCC_APB2Periph_GPIOA
    #define SPI1_MOSI_PIN               GPIO_Pin_7

    #define SPI1_GPIO_REMAP             0
#endif



// SPI名称类型定义
typedef enum
{
    SD_SPI = 0,
    TOUCH_SPI,
} spi_name_t;



// SD卡硬件配置
#define SDCARD_ENABLE                                                           ///< 是否使能SD卡驱动

#define SDCARD_POWER_NAME               SD_POWER                                ///< SD卡电源控制引脚名称, 没有电源控制不要宏定义此项
#define SDCARD_DETECT_ENABLE                                                    ///< 一般卡座都有, 最好使能此项, 否则检查SD卡较慢
#define SDCARD_DETECT_PORT              GPIOA                                   ///< SD卡座CD引脚, SD卡存在检测. PORT口
#define SDCARD_DETECT_CLK               RCC_APB2Periph_GPIOA                    ///< SD卡座CD引脚, SD卡存在检测. 时钟模块
#define SDCARD_DETECT_PIN               GPIO_Pin_4                              ///< SD卡座CD引脚, SD卡存在检测. PIN口

// SD 通讯模式宏定义, 注意只能选一个
//#define SDCARD_MODE_SDIO                                                        ///< SD卡使用SDIO模式通讯
//#define SDCARD_MODE_SPIDMA                                                      ///< SD卡使用SPI DMA模式通讯
#define SDCARD_MODE_SPI                                                         ///< SD卡使用SPI模式通讯

#ifdef SDCARD_MODE_SDIO
    #define SDIO_IRQ_PRIORITY           0x0                                     ///< 设置中断优先级. 高2位为可抢占主优先级, 低2位为非抢占的子优先级
#endif

#ifdef SDCARD_MODE_SPIDMA                                                       ///< SPI DMA模式配置
    #define SD_SPI_X                    2                                       ///< 选择硬件SPI口, 1表示SPI1, 2表示SPI2
    #define SD_CS_PORT                  GPIOB                                   ///< SD卡片选信号的PORT口
    #define SD_CS_CLK                   RCC_APB2Periph_GPIOB                    ///< SD卡片选信号的时钟模块
    #define SD_CS_PIN                   GPIO_Pin_12                             ///< SD卡片选信号的PIN口
#endif

#ifdef SDCARD_MODE_SPI                                                          ///< SPI模式配置
    #define SD_SPI_NAME                 SD_SPI                                  ///< 该驱动需使用一个SPI, SPI的SCK, MISO, MOSI口在"drv_spi.h"中配置
    #define SD_CS_PORT                  GPIOB                                   ///< SD卡Pin1, 片选信号的PORT口
    #define SD_CS_CLK                   RCC_APB2Periph_GPIOB                    ///< SD卡Pin1, 片选信号的时钟模块
    #define SD_CS_PIN                   GPIO_Pin_12                             ///< SD卡Pin1, 片选信号的PIN口
#endif



/// SD FLASH 硬件配置
//#define SDFLASH_ENABLE                                                          ///< 是否使能SD FLASH驱动

//#define SDFLASH_POWER_NAME              SDFLASH_POWER                           ///< SD FLASH卡电源控制引脚名称, 没有电源控制不要宏定义此项
#define SDFLASH_DETECT_ENABLE                                                   ///< 一般卡座都可检测SD卡是否存在
#define SDFLASH_DETECT_PORT             GPIOC                                   ///< SD FLASH卡座CD引脚, SD卡存在检测. PORT口
#define SDFLASH_DETECT_CLK              RCC_APB2Periph_GPIOC                    ///< SD FLASH卡座CD引脚, SD卡存在检测. 时钟模块
#define SDFLASH_DETECT_PIN              GPIO_Pin_11                             ///< SD FLASH卡座CD引脚, SD卡存在检测. PIN口

// SD FLASH 通讯模式宏定义, 注意只能选一个
//#define SDFLASH_MODE_SDIO                                                       ///< SD FLASH使用SDIO模式通讯
//#define SDFLASH_MODE_SPIDMA                                                     ///< SD FLASH使用SPI DMA模式通讯
//#define SDFLASH_MODE_SPI                                                        ///< SD FLASH使用SPI模式通讯

#ifdef SDFLASH_MODE_SDIO
    #define SDIO_IRQ_PRIORITY           0x0                                     ///< 设置中断优先级. 高2位为可抢占主优先级, 低2位为非抢占的子优先级
#endif

#ifdef SDFLASH_MODE_SPIDMA                                                      ///< SPI DMA模式配置
    #define SD_SPI_X                    2                                       ///< 选择硬件SPI口, 1表示SPI1, 2表示SPI2
    #define SD_CS_PORT                  GPIOB                                   ///< SD卡片选信号的PORT口
    #define SD_CS_CLK                   RCC_APB2Periph_GPIOB                    ///< SD卡片选信号的时钟模块
    #define SD_CS_PIN                   GPIO_Pin_12                             ///< SD卡片选信号的PIN口
#endif

#ifdef SDFLASH_MODE_SPI                                                         ///< SPI模式配置
    #define SD_SPI_NAME                 SD_SPI                                  ///< 该驱动需使用一个SPI, SPI的SCK, MISO, MOSI口在"drv_spi.h"中配置
    #define SD_CS_PORT                  GPIOC                                   ///< SD卡片选信号的PORT口
    #define SD_CS_CLK                   RCC_APB2Periph_GPIOC                    ///< SD卡片选信号的时钟模块
    #define SD_CS_PIN                   GPIO_Pin_11                             ///< SD卡片选信号的PIN口
#endif



// I2C硬件配置
#define I2C_NUM                         2                                       ///< 设置使用的I2C数量.该驱动最多允许5个I2C数

#if  (I2C_NUM > 0)
    /**
      * @brief 设置该I2C的外设名称
      *   @arg 0,                       使用IO口模拟SPI
      *   @arg I2C1,                    使用I2C1外设
      *   @arg I2C2,                    使用I2C2外设
      */
    #define I2C0_PERIPHERAL             I2C1                                    ///< 设置该I2C的模块名,   如果用软件模拟I2C, 设为0
    #define I2C0_CLK                    RCC_APB1Periph_I2C1                     ///< 设置该I2C的时钟模块, 如果用软件模拟I2C, 设为0

    #define I2C0_SCL_PORT               GPIOB                                   ///< 设置该I2C的SCL引脚的PORT口
    #define I2C0_SCL_CLK                RCC_APB2Periph_GPIOB                    ///< 设置该I2C的SCL引脚的时钟模块
    #define I2C0_SCL_PIN                GPIO_Pin_6                              ///< 设置该I2C的SCL引脚的PIN口

    #define I2C0_SDA_PORT               GPIOB                                   ///< 设置该I2C的SDA引脚的PORT口
    #define I2C0_SDA_CLK                RCC_APB2Periph_GPIOB                    ///< 设置该I2C的SDA引脚的时钟模块
    #define I2C0_SDA_PIN                GPIO_Pin_7                              ///< 设置该I2C的SDA引脚的PIN口

    /**
      * @brief 设置I2C引脚重映射功能. 更多信息请参考官方数据手册.
      *        与I2C相关的配置如下:
      *   @arg 0,                       不使用引脚重映射功能
      *   @arg GPIO_Remap_I2C1,         I2C1 引脚重映射
      */
    #define I2C0_GPIO_REMAP             0
#endif

#if  (I2C_NUM > 1)
    #define I2C1_PERIPHERAL             0
    #define I2C1_CLK                    0

    #define I2C1_SCL_PORT               GPIOB
    #define I2C1_SCL_CLK                RCC_APB2Periph_GPIOB
    #define I2C1_SCL_PIN                GPIO_Pin_6

    #define I2C1_SDA_PORT               GPIOB
    #define I2C1_SDA_CLK                RCC_APB2Periph_GPIOB
    #define I2C1_SDA_PIN                GPIO_Pin_7

    #define I2C1_GPIO_REMAP             0
#endif

typedef enum
{
  COMMON_I2C_HW = 0,
  COMMON_I2C_SW = 1,
} i2c_name_t;



// EEPROM硬件配置
#define EEPROM_I2C_NAME                 COMMON_I2C_HW                           ///< 该驱动需使用一个I2C, I2C的SCL, SDA. 在"drv_i2c.h"中配置
#define EEPROM_TYPE_24C                 2                                       ///< 设定EEPROM芯片型号. 1表示24C01, 64表示24C64/24C65, 1024表示24C1024, 类推



// UART硬件配置
#define UART_NUM                        2                                       ///< 设置使用的Uart数量.该驱动最多允许5个Uart数

//#define UART_TX_POLLING_MODE                                                    ///< UART 发送轮询模式, 此为默认值.
#define UART_TX_INT_BUF_MODE                                                    ///< UART 发送中断缓冲区模式

//#define UART_RX_POLLING_MODE                                                    ///< UART 接收轮询模式, 此为默认值.
#define UART_RX_INT_BUF_MODE                                                    ///< UART 接收中断缓冲区模式,  可与 UART_RX_INT_FUN_MODE 共用
//#define UART_RX_INT_FUN_MODE                                                    ///< UART 接收中断回调函数模式,可与 UART_RX_INT_BUF_MODE 共用

#if (UART_NUM > 0)
    /**
      * @brief 设置该串口的外设名称
      *   @arg USART1,                  使用USART1外设, USART1的CLK为 RCC_APB2Periph_USART1
      *   @arg USART2,                  使用USART2外设,  其余 的CLK为 RCC_APB1Periph_XXX
      *   @arg USART3,                  使用USART3外设
      *   @arg UART4,                   使用UART4 外设
      *   @arg UART5,                   使用UART5 外设
      */
    #define UART0_PERIPHERAL            USART1
    #define UART0_IRQ                   USART1_IRQn                             ///< 设置该串口的中断
    #define UART0_IRQ_HANDLER           USART1_IRQHandler                       ///< 设置该串口的中断函数
    #define UART0_CLK                   RCC_APB2Periph_USART1                   ///< 设置该串口的时钟模块

    #define UART0_TX_PORT               GPIOA                                   ///< 设置该串口的输出引脚PORT口
    #define UART0_TX_CLK                RCC_APB2Periph_GPIOA                    ///< 设置该串口的输出引脚时钟模块
    #define UART0_TX_PIN                GPIO_Pin_9                              ///< 设置该串口的输出引脚PIN口

    #define UART0_RX_PORT               GPIOA                                   ///< 设置该串口的输入引脚PORT口
    #define UART0_RX_CLK                RCC_APB2Periph_GPIOA                    ///< 设置该串口的输入引脚时钟模块
    #define UART0_RX_PIN                GPIO_Pin_10                             ///< 设置该串口的输入引脚PIN口

    /**
      * @brief 设置该串口的引脚重映射功能. 与使用的IO口有关.
      *   @arg 0,                       不使用引脚重映射功能
      *   @arg GPIO_Remap_USART1,       USART1 引脚重映射
      *   @arg GPIO_Remap_USART2,       USART2 引脚重映射
      *   @arg GPIO_PartialRemap_USART3,USART3 部分引脚重映射
      *   @arg GPIO_FullRemap_USART3,   USART3 全部引脚重映射
      */
    #define UART0_GPIO_REMAP            0

    /**
      * @brief 设置该串口的接收中断回调函数
      *   @arg 0,                       不使用回调函数, 不对接收数据进行处理
      *   @arg void fun(uint8_t data),  回调函数格式, 宏定义fun函数名称即可
      */
    #define UART0_RX_CALLBACK           0
    #define UART0_IRQ_PRIORITY          0xF                                     ///< 设置中断优先级. 高2位为可抢占主优先级, 低2位为非抢占的子优先级
#endif

#if (UART_NUM > 1)
    #define UART1_PERIPHERAL            USART2
    #define UART1_IRQ                   USART2_IRQn
    #define UART1_IRQ_HANDLER           USART2_IRQHandler
    #define UART1_CLK                   RCC_APB1Periph_USART2

    #define UART1_TX_PORT               GPIOA
    #define UART1_TX_CLK                RCC_APB2Periph_GPIOA
    #define UART1_TX_PIN                GPIO_Pin_2

    #define UART1_RX_PORT               GPIOA
    #define UART1_RX_CLK                RCC_APB2Periph_GPIOA
    #define UART1_RX_PIN                GPIO_Pin_3

    #define UART1_GPIO_REMAP            0

    #define UART1_RX_CALLBACK           0
    #define UART1_IRQ_PRIORITY          0xF
#endif

// 串口名称类型定义
typedef enum
{
    COM1 = 0,
    COM2 = 1,
} uart_name_t;



/// 定时器配置
#define TIMER_NUM                       1                                      ///< 设置使用的Timer数量.该驱动最多允许3个Timer数

#if (TIMER_NUM > 0)
    /**
      * @brief 设置该Timer的外设名称. TIM1为高级定时器, 此驱动不支持使用TIM1!!!
      *   @arg TIM2,                    使用TIM2外设, CLK为 RCC_APB1Periph_TIMx
      *   @arg TIM3,                    使用TIM3外设
      *   @arg TIM4,                    使用TIM4外设
      *   @arg TIM5,                    使用TIM5外设
      *   @arg TIM6,                    使用TIM6外设
      *   @arg TIM7,                    使用TIM7外设
      */
    #define TIMER0_PERIPHERAL           TIM2
    #define TIMER0_IRQ                  TIM2_IRQn                               ///< 设置该Timer的中断
    #define TIMER0_IRQ_HANDLER          TIM2_IRQHandler                         ///< 设置该Timer的中断函数
    #define TIMER0_CLK                  RCC_APB1Periph_TIM2                     ///< 设置该Timer的时钟模块

    /**
      * @brief 设置该Timer的溢出中断回调函数
      *   @arg 0,                       不使用回调函数, 不对接收数据进行处理
      *   @arg void fun(void),          回调函数格式, 宏定义fun函数名称即可
      */
    extern void tone_scan_10us(void);
    #define TIMER0_OVERTIME_CALLBACK    tone_scan_10us
    #define TIMER0_IRQ_PRIORITY         0xF                                     ///< 设置中断优先级. 高2位为可抢占主优先级, 低2位为非抢占的子优先级
#endif

/// 定时器名称类型定义
typedef enum
{
    TIMER_10US = 0,                                                             ///< 命名为易于理解的Timer名称, 值0对应TIMER0_xxx
} timer_name_t;



/// 曲调组件使能
#define TONE_ENABLE
#define TONE_OUT                        BEEP                                    ///< 设置振铃IO口, 由"drv_ctrlport"提供功能



#include "assert.h"                                                             ///< 调试头文件, 必须放在该文件的末尾

/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
