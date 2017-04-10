/**
  ******************************************************************************
  * @file    lcd_main.h
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2013.9.20
  * @brief   LCD 基础驱动程序. 包含LCD电源开关, 亮度调节, 驱动测试函数.
  *          并入 emwin, 内容做过简化!
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_MAIN_H
#define __LCD_MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "lcdgui_conf.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup LCD_MAIN
  * @{
  */

/** @defgroup LCD_MAIN_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Public_Function
  * @brief    定义公有函数
  * @{
  */
#ifdef LCD_ENABLE

// LCD控制函数
void lcd_init(void);                                                            // LCD IO口初始化
void lcd_power_on(void);                                                        // 打开电源
void lcd_power_off(void);                                                       // 关闭电源
void lcd_light_on(void);                                                        // 打开背光
void lcd_light_adjust(uint8_t level);                                           // 背光亮度调节
void lcd_light_off(void);                                                       // 关闭背光

// LCD基础函数
uint16_t lcd_rgb565(uint8_t red, uint8_t green, uint8_t blue);                  // 色彩转换
void     lcd_set_pixel(uint16_t x, uint16_t y, uint16_t rgb565);                // 设置一个点颜色
uint16_t lcd_get_pixel(uint16_t x, uint16_t y);                                 // 获取一个点颜色
void     lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t rgb565);

// LCD测试函数
void lcd_test_driver(void);                                                     // LCD底层驱动测试函数

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
