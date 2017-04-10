/**
  ******************************************************************************
  * @file    lcd_main.c
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2013.9.20
  * @brief   LCD 基础驱动程序. 包含LCD电源开关, 亮度调节, 驱动测试函数.
  *          并入 emwin, 内容做过简化!
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd_main.h"
#include "GUI.h"
#include "drv_systick.h"
#include "stm32f10x_rcc.h"

#ifdef LCD_POWER_NAME
    #include "drv_ctrlport.h"
#endif

#ifdef LCD_LIGHT_NAME
    #include "drv_ctrlport.h"
#endif



/** @addtogroup Components
  * @{
  */
/** @defgroup CPN_GUI
  * @{
  */
#ifdef LCD_ENABLE



/**
  ******************************************************************************
  * @addtogroup LCD_MAIN_Configure
  ******************************************************************************
  * @{
  */
#include "drv_systick.h"
#define lcd_delay_ms(time)              delay_ms(time)                          ///< 设置LCD毫秒延时函数
/**
  * @}
  */


/** @defgroup LCD_MAIN_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
// 常用颜色定义
#define LCD_TEST_BLACK                  ((uint16_t)0x0000)
#define LCD_TEST_RED                    ((uint16_t)0xF800)
#define LCD_TEST_GREEN                  ((uint16_t)0x07E0)
#define LCD_TEST_BLUE                   ((uint16_t)0x001F)
#define LCD_TEST_YELLOW                 (LCD_TEST_RED   | LCD_TEST_GREEN)
#define LCD_TEST_MAGENTA                (LCD_TEST_RED   | LCD_TEST_BLUE)
#define LCD_TEST_CYAN                   (LCD_TEST_GREEN | LCD_TEST_BLUE)
#define LCD_TEST_WHITE                  (LCD_TEST_RED   | LCD_TEST_GREEN | LCD_TEST_BLUE)
/**
  * @}
  */

/** @defgroup LCD_MAIN_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup LCD_MAIN_Private_Function
  * @brief    定义私有函数
  * @{
  */

/**
  * @}
  */



/** @defgroup LCD_MAIN_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  LCD IO口及驱动芯片初始化函数.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_init(void)
{
    // 用的是ST官网的emwin库, 其增加了使用CRC判断是否为ST芯片的代码, 因此必须使能CRC!
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    lcd_bus_port_init();

#ifdef LCD_POWER_NAME                                                           // 电源或复位引脚初始化
    ctrlport_init(LCD_POWER_NAME);
    ctrlport_off(LCD_POWER_NAME);
#endif

#ifdef LCD_LIGHT_NAME                                                           // 背光引脚初始化
    ctrlport_init(LCD_LIGHT_NAME);
    ctrlport_off(LCD_LIGHT_NAME);
#endif

    lcd_light_off();
    lcd_power_off();
    delay_ms(10);
    lcd_power_on();
    delay_ms(10);
    lcd_chip_init();
    lcd_light_on();
}

/**
  ******************************************************************************
  * @brief  LCD 打开电源.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_power_on(void)
{
#ifdef LCD_POWER_NAME
    ctrlport_on(LCD_POWER_NAME);
#endif
}

/**
  ******************************************************************************
  * @brief  LCD 关闭电源.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_power_off(void)
{
#ifdef LCD_POWER_NAME
    ctrlport_off(LCD_POWER_NAME);
#endif
}

/**
  ******************************************************************************
  * @brief  LCD 打开背光.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_light_on(void)
{
#ifdef LCD_LIGHT_NAME
    ctrlport_on(LCD_LIGHT_NAME);
#endif
}

/**
  ******************************************************************************
  * @brief  LCD 调节背光亮度.
  * @param  level, 亮度等级. 0-最亮, 0xFF-熄灭
  * @retval none
  ******************************************************************************
  */
void lcd_light_adjust(uint8_t level)
{
#ifdef LCD_LIGHT_NAME
    if (level < 0xFF) ctrlport_on(LCD_LIGHT_NAME);
    else              ctrlport_off(LCD_LIGHT_NAME);
#endif
}

/**
  ******************************************************************************
  * @brief  LCD 关闭背光.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_light_off(void)
{
#ifdef LCD_LIGHT_NAME
    ctrlport_off(LCD_LIGHT_NAME);
#endif
}

/**
  ******************************************************************************
  * @brief  转换颜色为RGB565函数
  * @param  red,   红色色值
  * @param  green, 绿色色值
  * @retval blue,  蓝色色值
  ******************************************************************************
  */
uint16_t lcd_rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
    return (((uint16_t)red << 11) | (((uint16_t)green << 5) & 0x07E0) | (blue & 0x1F));
}

/**
  ******************************************************************************
  * @brief  设置一个像素点颜色
  * @param  x,      像素点的X轴坐标
  * @param  y,      像素点的Y轴坐标
  * @param  rgb565, 像素点的颜色值
  * @retval none
  ******************************************************************************
  */
void lcd_set_pixel(uint16_t x, uint16_t y, uint16_t rgb565)
{
    lcd_chip_pixel_wr(x, y, rgb565);
}

/**
  ******************************************************************************
  * @brief  获取一个像素点颜色
  * @param  x,      像素点的X轴坐标
  * @param  y,      像素点的Y轴坐标
  * @retval none
  ******************************************************************************
  */
uint16_t lcd_get_pixel(uint16_t x, uint16_t y)
{
    return lcd_chip_pixel_rd(x, y);
}

/**
  ******************************************************************************
  * @brief  画实心矩形函数
  * @param  x, 矩形左上角X轴坐标
  * @param  y, 矩形左上角Y轴坐标
  * @param  w, 矩形X轴宽度
  * @param  l, 矩形Y轴宽度
  * @param  rgb565, 要显示的颜色
  * @retval none
  ******************************************************************************
  */
void lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t l, uint16_t rgb565)
{
#ifdef LCD_QUICK
    uint32_t i;

    lcd_chip_canvas(x, y, w, l);
    lcd_chip_wr_start();
    for (i = w*l; i>0; i--)
    {
        lcd_chip_wring(rgb565);
    }
    lcd_chip_wr_stop();

#else
    uint16_t i, j;

    for (i=y; i<=y+l; i++)
    {
        for (j=x; j<=x+w; j++)
        {
            lcd_chip_pixel_wr(j, i, rgb565);
        }
    }

#endif
}

/**
  ******************************************************************************
  * @brief  LCD 驱动测试函数.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void lcd_test_driver(void)
{
    uint16_t i, j;
    uint16_t rgb;

    // LCD初始化
    lcd_init();

    // lcd_main 驱动测试
    lcd_fill_rectangle(0, 0, 100, 100, LCD_TEST_MAGENTA);
    // 画点函数
    for (i=0; i<10; i++)
    {
        for (j=10; j<20; j++)
        {
            lcd_set_pixel(i+10,j,LCD_TEST_WHITE);
            lcd_set_pixel(i+20,j,LCD_TEST_BLACK);
            lcd_set_pixel(i+30,j,LCD_TEST_RED);
            lcd_set_pixel(i+40,j,LCD_TEST_GREEN);
            lcd_set_pixel(i+50,j,LCD_TEST_BLUE);
            lcd_set_pixel(i+60,j,LCD_TEST_WHITE);
            lcd_set_pixel(i+70,j,LCD_TEST_BLACK);
        }
    }
    // 颜色吸取并显示
    for (i=1; i<8; i++)
    {
        rgb = lcd_get_pixel(5+10*i, 10);
        lcd_fill_rectangle(10*i, 30, 10, 10, rgb);
    }

    delay_ms(2000);

    // emWIN测试
    GUI_Init();
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_GotoXY(10, 20);
    GUI_DispString("emWIN BLUE");
    // 注意, 画笔大小必须为1像素点, XOR模式才有效!
    GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
    GUI_FillCircle(120, 60, 40);
    GUI_SetDrawMode(GUI_DRAWMODE_XOR);
    GUI_FillCircle(140, 90, 40);
}



/**
  * @}
  */

#endif
/**
  * @}
  */
/**
  * @}
  */

/* END OF FILE ---------------------------------------------------------------*/
