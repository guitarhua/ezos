/**
  ******************************************************************************
  * @file    drv_led.h
  * @author  mousie
  * @version V2.1.0
  * @date    2012.2.25
  * @brief   LED灯驱动程序. 支持可调灯/呼吸灯/闪烁灯效果.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_LED_H
#define __DRV_LED_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_Led
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_Led_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define LED_NUM                         4                                       ///< 设置LED的数量, 该驱动最多允许16个LED灯.
//#define LED_LEVEL                       0                                       ///< 0, 低电平点亮LED. 1, 高电平点亮LED
//#define LED_ADVANCED_ENABLE                                                     ///< 使能LED的高级功能, 包括闪烁和渐变效果
//
//#define LED0_PORT                       GPIOD                                   ///< 设置该LED的PORT口
//#define LED0_CLK                        RCC_APB2Periph_GPIOD                    ///< 设置该LED的时钟模块
//#define LED0_PIN                        GPIO_Pin_8                              ///< 设置该LED的PIN口
//
//#define LED1_PORT                       GPIOD
//#define LED1_CLK                        RCC_APB2Periph_GPIOD
//#define LED1_PIN                        GPIO_Pin_9
//
//#define LED2_PORT                       GPIOD
//#define LED2_CLK                        RCC_APB2Periph_GPIOD
//#define LED2_PIN                        GPIO_Pin_10
//
//#define LED3_PORT                       GPIOD
//#define LED3_CLK                        RCC_APB2Periph_GPIOD
//#define LED3_PIN                        GPIO_Pin_11
//
///// LED名称类型定义
//typedef enum
//{
//    LED1 = 0,                                                                   ///< 命名为易于理解的LED名称, 值0对应LED0_xxx
//    LED2,                                                                       ///< 值1对应LED1_xxx, 以下类推
//    LED3,
//    LED4,
//} led_name_t;



#if (LED_NUM > 16)
    #error "LED_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_Led_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
/// LED灯状态类型定义
typedef enum
{
    LED_ON   = 0x01,                                                            ///< LED灯打开状态
    LED_OFF  = 0x00,                                                            ///< LED灯关闭状态
    BLED_ON  = 0x11,                                                            ///< 呼吸灯打开状态
    BLED_OFF = 0x10,                                                            ///< 呼吸灯关闭状态
    FLED_ON  = 0x21,                                                            ///< 闪烁灯高电平状态
    FLED_OFF = 0x20,                                                            ///< 闪烁灯低电平状态
} led_status_t;
/**
  * @}
  */

/** @defgroup DRV_Led_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Led_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Led_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (LED_NUM > 0)

// 基础功能函数
void led_init_all(void);                                                        // 初始化所有的LED
void led_init(led_name_t led_name);                                             // 初始化指定的LED
void led_on(led_name_t led_name);                                               // LED打开
void led_off(led_name_t led_name);                                              // LED关闭
void led_toggle(led_name_t led_name);                                           // LED翻转
led_status_t led_get_status(led_name_t led_name);                               // 读取当前LED状态

#ifdef LED_ADVANCED_ENABLE

// 高级功能函数
void led_scan_1ms(void);                                                        // LED 1ms 定时扫描, 在1ms定时中断函数中调用
void led_adjust(led_name_t led_name, uint8_t level);                            // LED调节亮度值. 0最亮, 0xFF熄灭.
void led_on_gradual(led_name_t led_name);                                       // LED渐变打开
void led_off_gradual(led_name_t led_name);                                      // LED渐变关闭
void led_toggle_gradual(led_name_t led_name);                                   // LED渐变翻转
void led_flash(led_name_t led_name, uint16_t time_ms, uint8_t counter);         // LED闪烁指定次数, counter=0 表示一直闪烁

#endif
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
