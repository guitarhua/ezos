/**
  ******************************************************************************
  * @file    lcd_font.h
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2012.2.22
  * @brief   LCD 字体显示驱动程序.
  *          需要 LCD基础函数的 支持.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_FONT_H
#define __LCD_FONT_H

/* Includes ------------------------------------------------------------------*/
#include "lcd_main.h"
#include <stdint.h>



/** @addtogroup Components
  * @{
  */
/** @addtogroup LCD_FONT
  * @{
  */



/** @defgroup LCD_FONT_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
/**
  * @brief 字模取模类型定义
  *
  *   图示以12x12字模为例, 0.0 表示一个点, 含义为Byte0.Bit0, 即该点存放位置为字节0的Bit0上. xxx表示无关位
  *
  *   @arg TYPEHEAD_MODE_X_MSB, 逐行, 高位在前取模
  *            0.7 0.6 0.5 0.4 0.3 0.2 0.1 0.0   1.7 1.6 1.5 1.4 xxx xxx xxx xxx
  *            byte2  ----------------------->   byte3  ----------------------->
  *            byte4  ----------------------->   byte5  ----------------------->
  *            byte6  ----------------------->   byte7  ----------------------->
  *            byte8  ----------------------->   byte9  ----------------------->
  *            byte10 ----------------------->   byte11 ----------------------->
  *            byte12 ----------------------->   byte13 ----------------------->
  *            byte14 ----------------------->   byte15 ----------------------->
  *            byte16 ----------------------->   byte17 ----------------------->
  *            byte18 ----------------------->   byte19 ----------------------->
  *            byte20 ----------------------->   byte21 ----------------------->
  *            byte22 ----------------------->   byte23 ----------------------->
  *
  *   @arg TYPEHEAD_MODE_X_LSB, 逐行, 低位在前取模
  *            0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7   1.0 1.1 1.2 1.3 xxx xxx xxx xxx
  *            byte2  ----------------------->   byte3  ----------------------->
  *            byte4  ----------------------->   byte5  ----------------------->
  *            byte6  ----------------------->   byte7  ----------------------->
  *            byte8  ----------------------->   byte9  ----------------------->
  *            byte10 ----------------------->   byte11 ----------------------->
  *            byte12 ----------------------->   byte13 ----------------------->
  *            byte14 ----------------------->   byte15 ----------------------->
  *            byte16 ----------------------->   byte17 ----------------------->
  *            byte18 ----------------------->   byte19 ----------------------->
  *            byte20 ----------------------->   byte21 ----------------------->
  *            byte22 ----------------------->   byte23 ----------------------->
  *
  *   @arg TYPEHEAD_MODE_XY_MSB, 先行再列, 高位在前取模
  *            0.7 0.6 0.5 0.4 0.3 0.2 0.1 0.0   12.7 12.6 12.5 12.4 xxx xxx xxx
  *            byte1  -----------------------V   byte13 -----------------------V
  *            byte2  -----------------------V   byte14 -----------------------V
  *            byte3  -----------------------V   byte15 -----------------------V
  *            byte4  -----------------------V   byte16 -----------------------V
  *            byte5  -----------------------V   byte17 -----------------------V
  *            byte6  -----------------------V   byte18 -----------------------V
  *            byte7  -----------------------V   byte19 -----------------------V
  *            byte8  -----------------------V   byte20 -----------------------V
  *            byte9  -----------------------V   byte21 -----------------------V
  *            byte10 -----------------------V   byte22 -----------------------V
  *            byte11 -----------------------V   byte23 -----------------------V
  *
  *   @arg TYPEHEAD_MODE_XY_LSB, 先行再列, 低位在前取模
  *            0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7   12.0 12.1 12.2 12.3 xxx xxx xxx
  *            byte1  -----------------------V   byte13 -----------------------V
  *            byte2  -----------------------V   byte14 -----------------------V
  *            byte3  -----------------------V   byte15 -----------------------V
  *            byte4  -----------------------V   byte16 -----------------------V
  *            byte5  -----------------------V   byte17 -----------------------V
  *            byte6  -----------------------V   byte18 -----------------------V
  *            byte7  -----------------------V   byte19 -----------------------V
  *            byte8  -----------------------V   byte20 -----------------------V
  *            byte9  -----------------------V   byte21 -----------------------V
  *            byte10 -----------------------V   byte22 -----------------------V
  *            byte11 -----------------------V   byte23 -----------------------V
  *
  *   @arg TYPEHEAD_MODE_Y_MSB, 逐列, 高位在前取模
  *            0.7   b   b   b   b   b   b   b   b   b   b   b
  *            0.6   y   y   y   y   y   y   y   y   y   y   y
  *            0.5   t   t   t   t   t   t   t   t   t   t   t
  *            0.4   e   e   e   e   e   e   e   e   e   e   e
  *            0.3   2   4   6   8   1   1   1   1   1   2   2
  *            0.2                   0   2   4   6   8   0   2
  *            0.1   |   |   |   |
  *            0.0   V   V   V   V   V   V   V   V   V   V   V
  *
  *            1.7   b   b   b   b   b   b   b   b   b   b   b
  *            1.6   y   y   y   y   y   y   y   y   y   y   y
  *            1.5   t   t   t   t   t   t   t   t   t   t   t
  *            1.4   e   e   e   e   e   e   e   e   e   e   e
  *            xxx   3   5   7   9   1   1   1   1   1   2   2
  *            xxx                   1   3   5   7   9   1   3
  *            xxx   |   |   |   |
  *            xxx   V   V   V   V   V   V   V   V   V   V   V
  *
  *   @arg TYPEHEAD_MODE_Y_LSB, 逐列, 低位在前取模
  *            0.0   b   b   b   b   b   b   b   b   b   b   b
  *            0.1   y   y   y   y   y   y   y   y   y   y   y
  *            0.2   t   t   t   t   t   t   t   t   t   t   t
  *            0.3   e   e   e   e   e   e   e   e   e   e   e
  *            0.4   2   4   6   8   1   1   1   1   1   2   2
  *            0.5                   0   2   4   6   8   0   2
  *            0.6   |   |   |   |
  *            0.7   V   V   V   V   V   V   V   V   V   V   V
  *
  *            1.0   b   b   b   b   b   b   b   b   b   b   b
  *            1.1   y   y   y   y   y   y   y   y   y   y   y
  *            1.2   t   t   t   t   t   t   t   t   t   t   t
  *            1.3   e   e   e   e   e   e   e   e   e   e   e
  *            xxx   3   5   7   9   1   1   1   1   1   2   2
  *            xxx                   1   3   5   7   9   1   3
  *            xxx   |   |   |   |
  *            xxx   V   V   V   V   V   V   V   V   V   V   V
  *
  *   @arg TYPEHEAD_MODE_YX_MSB, 先列再行, 高位在前取模
  *            0.0   b   b   b   b   b   b   b   b   b   b   b
  *            0.1   y   y   y   y   y   y   y   y   y   y   y
  *            0.2   t   t   t   t   t   t   t   t   t   t   t
  *            0.3   e   e   e   e   e   e   e   e   e   e   e
  *            0.4   1   2   3   4   5   6   7   8   9   1   1
  *            0.5                                       0   1
  *            0.6   |   |   |   |   |   |   |   |   |
  *            0.7   >   >   >   >   >   >   >   >   >   >   >
  *
  *            12.0  b   b   b   b   b   b   b   b   b   b   b
  *            12.1  y   y   y   y   y   y   y   y   y   y   y
  *            12.2  t   t   t   t   t   t   t   t   t   t   t
  *            12.3  e   e   e   e   e   e   e   e   e   e   e
  *            xxx   1   1   1   1   1   1   1   2   2   2   2
  *            xxx   3   4   5   6   7   8   9   0   1   2   3
  *            xxx
  *            xxx   >   >   >   >   >   >   >   >   >   >   >
  *
  *   @arg TYPEHEAD_MODE_YX_LSB, 先列再行, 低位在前取模
  *            0.0   b   b   b   b   b   b   b   b   b   b   b
  *            0.1   y   y   y   y   y   y   y   y   y   y   y
  *            0.2   t   t   t   t   t   t   t   t   t   t   t
  *            0.3   e   e   e   e   e   e   e   e   e   e   e
  *            0.4   1   2   3   4   5   6   7   8   9   1   1
  *            0.5                                       0   1
  *            0.6   |   |   |   |   |   |   |   |   |
  *            0.7   >   >   >   >   >   >   >   >   >   >   >
  *
  *            12.0  b   b   b   b   b   b   b   b   b   b   b
  *            12.1  y   y   y   y   y   y   y   y   y   y   y
  *            12.2  t   t   t   t   t   t   t   t   t   t   t
  *            12.3  e   e   e   e   e   e   e   e   e   e   e
  *            xxx   1   1   1   1   1   1   1   2   2   2   2
  *            xxx   3   4   5   6   7   8   9   0   1   2   3
  *            xxx
  *            xxx   >   >   >   >   >   >   >   >   >   >   >
  */
typedef enum
{
    TYPEHEAD_MODE_X_MSB  = (0x00 | 0x00 | 0x00),
    TYPEHEAD_MODE_X_LSB  = (0x00 | 0x00 | 0x01),
    TYPEHEAD_MODE_Y_MSB  = (0x00 | 0x02 | 0x00),
    TYPEHEAD_MODE_Y_LSB  = (0x00 | 0x02 | 0x01),
    TYPEHEAD_MODE_XY_MSB = (0x04 | 0x00 | 0x00),
    TYPEHEAD_MODE_XY_LSB = (0x04 | 0x00 | 0x01),
    TYPEHEAD_MODE_YX_MSB = (0x04 | 0x02 | 0x00),
    TYPEHEAD_MODE_YX_LSB = (0x04 | 0x02 | 0x01),
} typehead_mode_t;
/**
  * @}
  */

/** @defgroup LCD_FONT_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */
#define TYPEHEAD_BYTE_MAX               200                                     // 可支持到 36*36 的点阵字库. 计算公式: 字模长 * 字模宽 / 8
/**
  * @}
  */

/** @defgroup LCD_FONT_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_FONT_Public_Function
  * @brief    定义公有函数
  * @{
  */
#ifdef LCD_FONT_ENABLE

void lcd_font_set_para(uint16_t x, uint16_t y, uint32_t back_color, uint16_t fore_color);
void lcd_set_typehead(uint8_t width, uint8_t heigth, typehead_mode_t mode);
void lcd_put_typehead(const uint8_t *pdata);
uint8_t lcd_get_typehead_byte(void);

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
