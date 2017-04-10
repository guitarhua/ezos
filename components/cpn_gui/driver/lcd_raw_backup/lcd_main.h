/**
  ******************************************************************************
  * @file    lcd_main.h
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2012.2.22
  * @brief   LCD 驱动程序.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_MAIN_H
#define __LCD_MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup LCD_MAIN
  * @{
  */
/**
  ******************************************************************************
  * @defgroup LCD_MAIN_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//// LCD驱动使能
//#define LCD_ENABLE                                                              ///< 使能LCD
//#define LCD_FONT_ENABLE                                                         ///< 使能LCD文字显示功能
//
//// LCD 驱动芯片选择, 注意只能选一个
////#define LCD_CHIP_ILI9320
////#define LCD_CHIP_ILI9325
////#define LCD_CHIP_HX8347
//
//// LCD 分辨率配置
//#define LCD_X_MAX                       240                                     ///< X轴分辨率
//#define LCD_Y_MAX                       320                                     ///< Y轴分辨率
//
//// LCD 电源及背光控制引脚
////#define LCD_POWER_NAME                  LCD_POWER                               ///< 电源控制或接复位引脚. 不用则不定义, 使用需drv_ctrlport驱动
////#define LCD_LIGHT_NAME                  LCD_LIGHT                               ///< LCD背光控制. 不用则不定义, 使用需drv_ctrlport驱动
//
///**
//  * LCD通讯配置, 注意只能选一个! 引脚对应关系如下图:
//  * +------------------------------------------------------------------------+
//  * |                            Pin assignment                              |
//  * +---------------------+----------------+----------+----------------------|
//  * |  STM32 Pins         | STM32 FSMC     | LCD Pins |  describe            |
//  * +---------------------+----------------+----------+----------------------+
//  * |  LCD_DATA_PIN[15:0] | FSMC_D[15:0]   | DB[15:0] |  16bit Data          |
//  * |  LCD_CS_PIN         | FSMC_NE1       | CS       |  low, chip selection |
//  * |  LCD_RS_PIN         | FSMC_A16       | RS       |  low/high, reg/data  |
//  * |  LCD_WR_PIN         | FSMC_NWE       | WR       |  low, write enable   |
//  * |  LCD_RD_PIN         | FSMC_NOE       | RD       |  low, read enable    |
//  * +---------------------+----------------+----------+----------------------+
//  */
////#define LCD_BUS_FSMC                                                            ///< 使用FSMC接口, 将LCD作为RAM处理
////#define LCD_BUS_8080                                                            ///< 使用IO口模拟INTE 80800并行总线
//
//#ifdef LCD_BUS_8080
//    #define LCD_DATA_PORT               GPIOE                                   ///< LCD 16bit数据线的PORT口
//    #define LCD_DATA_CLK                RCC_APB2Periph_GPIOE                    ///< LCD 16bit数据线的时钟模块
//
//    #define LCD_CS_PORT                 GPIOD                                   ///< LCD片选信号的PORT口
//    #define LCD_CS_CLK                  RCC_APB2Periph_GPIOD                    ///< LCD片选信号的时钟模块
//    #define LCD_CS_PIN                  GPIO_Pin_12                             ///< LCD片选信号的PIN口
//
//    #define LCD_RS_PORT                 GPIOD                                   ///< LCD寄存器/数据信号的PORT口
//    #define LCD_RS_CLK                  RCC_APB2Periph_GPIOD                    ///< LCD寄存器/数据信号的时钟模块
//    #define LCD_RS_PIN                  GPIO_Pin_13                             ///< LCD寄存器/数据信号的PIN口
//
//    #define LCD_WR_PORT                 GPIOD                                   ///< LCD写信号的PORT口
//    #define LCD_WR_CLK                  RCC_APB2Periph_GPIOD                    ///< LCD写信号的时钟模块
//    #define LCD_WR_PIN                  GPIO_Pin_14                             ///< LCD写信号的PIN口
//
//    #define LCD_RD_PORT                 GPIOD                                   ///< LCD读信号的PORT口
//    #define LCD_RD_CLK                  RCC_APB2Periph_GPIOD                    ///< LCD读信号的时钟模块
//    #define LCD_RD_PIN                  GPIO_Pin_15                             ///< LCD读信号的PIN口
//#endif



#ifdef LCD_ENABLE
            // LCD驱动芯片通讯方式头文件
    #ifdef LCD_BUS_8080
        #include "lcd_bus_8080.h"
    #endif
    #ifdef LCD_BUS_FSMC
        #include "lcd_bus_fsmc.h"
    #endif

    // LCD驱动芯片预处理
    #ifdef LCD_CHIP_ILI9320
        #define LCD_QUICK                                                           ///< 使能LCD快速画图
        #include "lcd_chip_ili9320.h"
    #endif
    #ifdef LCD_CHIP_ILI9325
        #define LCD_QUICK                                                           ///< 使能LCD快速画图
        #include "lcd_chip_ili9325.h"
    #endif
    #ifdef LCD_CHIP_HX8347
        #define LCD_QUICK                                                           ///< 使能LCD快速画图
        #include "lcd_chip_hx8347.h"
    #endif

#else
    #undef LCD_FONT_ENABLE

#endif
/**
  * @}
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
// 常用颜色定义
#define LCD_BLACK                       ((uint16_t)0x0000)
#define LCD_RED                         ((uint16_t)0xF800)
#define LCD_GREEN                       ((uint16_t)0x07E0)
#define LCD_BLUE                        ((uint16_t)0x001F)
#define LCD_YELLOW                      (LCD_RED   | LCD_GREEN)
#define LCD_MAGENTA                     (LCD_RED   | LCD_BLUE)
#define LCD_CYAN                        (LCD_GREEN | LCD_BLUE)
#define LCD_WHITE                       (LCD_RED   | LCD_GREEN | LCD_BLUE)

#define LCD_TRANSPARENT                 ((uint32_t)0xFFFFFFFF)                  // 字体背景为透明色
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
void lcd_init(void);                                                            // LCD初始化, 打开电源和背光

void lcd_power_on(void);                                                        // 打开电源
void lcd_power_off(void);                                                       // 关闭电源
void lcd_light_on(void);                                                        // 打开背光
void lcd_light_adjust(uint8_t level);                                           // 背光亮度调节
void lcd_light_off(void);                                                       // 关闭背光

// LCD辅助函数
uint16_t lcd_rgb565(uint8_t red, uint8_t green, uint8_t blue);                                      // 将颜色转为RGB565格式

// LCD画图函数
void lcd_clear_all(void);                                                                           // 清空LCD全屏, 填黑

void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t rgb565);                                       // 画一个像素点
void lcd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t rgb565);            // 画一条线
void lcd_draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t rgb565);           // 画空心矩形
void lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t rgb565);           // 画实心矩形
void lcd_draw_circle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t rgb565);                        // 画空心圆
void lcd_fill_circle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t rgb565);                        // 画实心圆

void lcd_paint_image(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t *rgb565);             // 画一幅图, rgb565
#ifdef LCD_QUICK
	void lcd_paint_canvas(uint16_t x, uint16_t y, uint16_t w, uint16_t l);                          // 指定画布区域
	void lcd_paint_rgb565(uint16_t *rgb565, uint32_t num);                                          // 显示颜色, rgb565
#endif

// LCD字体显示函数
#ifdef LCD_FONT_ENABLE
	void lcd_font_set_para(uint16_t x, uint16_t y, uint32_t back_color, uint16_t fore_color);       // 设置字体显示参数
    void lcd_put_font(uint16_t incode);                                                             // 显示一个字符
    void lcd_printf(char *pstring, ...);                                                            // LCD打印函数
#endif

// LCD颜色吸取函数, 暂未全部调通, 需测试后使用
uint16_t lcd_absorb_pixel(uint16_t x, uint16_t y);                                                  // 吸取一个像素点的颜色

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
