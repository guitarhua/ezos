/**
  ******************************************************************************
  * @file    lcdgui_conf.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.9.21
  * @brief   LCD底层驱动 及 emWIN GUI 配置.
  ******************************************************************************
  */
#ifndef __LCDGUI_CONF_H__
#define __LCDGUI_CONF_H__



/*-----------------------------------------------------------------------------/
/ LCD GUI 基础功能
/-----------------------------------------------------------------------------*/
#define LCD_ENABLE                                                              ///< 使能LCD

#define GUI_SUPPORT_TOUCH               0                                       ///< 支持触摸屏
#define GUI_SUPPORT_MOUSE               0                                       ///< 支持鼠标
#define GUI_WINSUPPORT                  1                                       ///< 支持窗口管理器和widgets
#define GUI_SUPPORT_AA                  1                                       ///< 支持抗锯齿功能

#define GUI_SUPPORT_MEMDEV              1                                       ///< 支持存储设备
#define GUI_NUMBYTES                    (1024) * 40                             ///< emWIN内存堆容量, 至少4K以上
#define GUI_BLOCKSIZE                   0x80                                    ///< 内存块大小, 建议值32-1024. 若含有大量视图, 可选较小的值. 若用于图像解压, 则选大值.
#define GUI_DEFAULT_FONT                &GUI_Font6x8                            ///< 设定默认字体

/*-----------------------------------------------------------------------------/
/ LCD 及 触摸配置
/-----------------------------------------------------------------------------*/
#define XSIZE_PHYS                      240                                     ///< 视域X轴像素, 屏宽
#define YSIZE_PHYS                      320                                     ///< 视域Y轴像素, 屏高
#define VXSIZE_PHYS                     XSIZE_PHYS                              ///< 虚拟屏幕X轴, 一般和视域屏幕保持一致
#define VYSIZE_PHYS                     YSIZE_PHYS                              ///< 虚拟屏幕Y轴, 一般和视域屏幕保持一致

#define COLOR_CONVERSION                GUICC_565                               ///< 色彩转换模式, 屏幕16位色RGB为 GUICC_M565, BGR为 GUICC_565

// 开启以下功能会影响刷屏速度, 建议直接修改 LCD 底层驱动, 使视域XY轴和实际XY轴一致!!!
#define LCD_MIRROR_X                    0                                       ///< LCD 实际X轴镜像
#define LCD_MIRROR_Y                    0                                       ///< LCD 实际Y轴镜像
#define LCD_SWAP_XY                     0                                       ///< LCD 实际XY轴互换

#define TOUCH_MIRROR_X                  0                                       ///< 触摸屏实际X轴镜像
#define TOUCH_MIRROR_Y                  0                                       ///< 触摸屏实际Y轴镜像
#define TOUCH_SWAP_XY                   0                                       ///< 触摸屏实际XY轴互换



#ifdef LCD_ENABLE
/*-----------------------------------------------------------------------------/
/ LCD底层驱动配置
/-----------------------------------------------------------------------------*/
    #include "stm32f10x_conf.h"                                                 // 硬件集中配置文件

    // LCD 驱动芯片选择, 注意只能选一个
    #define LCD_CHIP_ILI9320
    //#define LCD_CHIP_ILI9325
    //#define LCD_CHIP_HX8347

    // LCD 电源及背光控制引脚
    //#define LCD_POWER_NAME                  LCD_POWER                         ///< 电源控制或接复位引脚. 不用则不定义, 使用需drv_ctrlport驱动
    //#define LCD_LIGHT_NAME                  LCD_LIGHT                         ///< LCD背光控制. 不用则不定义, 使用需drv_ctrlport驱动

    /**
      * LCD通讯配置, 注意只能选一个! 引脚对应关系如下图:
      * +------------------------------------------------------------------------+
      * |                            Pin assignment                              |
      * +---------------------+----------------+----------+----------------------|
      * |  STM32 Pins         | STM32 FSMC     | LCD Pins |  describe            |
      * +---------------------+----------------+----------+----------------------+
      * |  LCD_DATA_PIN[15:0] | FSMC_D[15:0]   | DB[15:0] |  16bit Data          |
      * |  LCD_CS_PIN         | FSMC_NE1       | CS       |  low, chip selection |
      * |  LCD_RS_PIN         | FSMC_A16       | RS       |  low/high, reg/data  |
      * |  LCD_WR_PIN         | FSMC_NWE       | WR       |  low, write enable   |
      * |  LCD_RD_PIN         | FSMC_NOE       | RD       |  low, read enable    |
      * +---------------------+----------------+----------+----------------------+
      */
    #define LCD_BUS_8080                                                        ///< 使用IO口模拟INTE 80800并行总线
    //#define LCD_BUS_FSMC                                                        ///< 使用FSMC接口, 将LCD作为RAM处理

    #ifdef LCD_BUS_8080
        #define LCD_DATA_PORT           GPIOE                                   ///< LCD 16bit数据线的PORT口
        #define LCD_DATA_CLK            RCC_APB2Periph_GPIOE                    ///< LCD 16bit数据线的时钟模块

        #define LCD_CS_PORT             GPIOC                                   ///< LCD片选信号的PORT口
        #define LCD_CS_CLK              RCC_APB2Periph_GPIOC                    ///< LCD片选信号的时钟模块
        #define LCD_CS_PIN              GPIO_Pin_6                              ///< LCD片选信号的PIN口

        #define LCD_RS_PORT             GPIOD                                   ///< LCD寄存器/数据信号的PORT口
        #define LCD_RS_CLK              RCC_APB2Periph_GPIOD                    ///< LCD寄存器/数据信号的时钟模块
        #define LCD_RS_PIN              GPIO_Pin_13                             ///< LCD寄存器/数据信号的PIN口

        #define LCD_WR_PORT             GPIOD                                   ///< LCD写信号的PORT口
        #define LCD_WR_CLK              RCC_APB2Periph_GPIOD                    ///< LCD写信号的时钟模块
        #define LCD_WR_PIN              GPIO_Pin_14                             ///< LCD写信号的PIN口

        #define LCD_RD_PORT             GPIOD                                   ///< LCD读信号的PORT口
        #define LCD_RD_CLK              RCC_APB2Periph_GPIOD                    ///< LCD读信号的时钟模块
        #define LCD_RD_PIN              GPIO_Pin_15                             ///< LCD读信号的PIN口
    #endif

    // LCD驱动芯片通讯方式头文件
    #ifdef LCD_BUS_8080
        #include "lcd_bus_8080.h"
    #endif
    #ifdef LCD_BUS_FSMC
        #include "lcd_bus_fsmc.h"
    #endif

    // LCD驱动芯片预处理
    #ifdef LCD_CHIP_ILI9320
        #define LCD_QUICK                                                       ///< 使能LCD快速画图
        #include "lcd_chip_ili9320.h"
    #endif
    #ifdef LCD_CHIP_ILI9325
        #define LCD_QUICK                                                       ///< 使能LCD快速画图
        #include "lcd_chip_ili9325.h"
    #endif
    #ifdef LCD_CHIP_HX8347
        #define LCD_QUICK                                                       ///< 使能LCD快速画图
        #include "lcd_chip_hx8347.h"
    #endif
#endif // LCD_ENABLE



#if GUI_SUPPORT_TOUCH && (defined LCD_ENABLE)
/*-----------------------------------------------------------------------------/
/ 触摸底层驱动配置
/-----------------------------------------------------------------------------*/
    // 触摸芯片通讯设置, 多为 SPI / I2C
    #define TOUCH_SPI_NAME                  TOUCH_SPI                           ///< 该驱动需使用一个SPI, SPI的SCK, MISO, MOSI口在"drv_spi.h"中配置
    #define TOUCH_CS_PORT                   GPIOC                               ///< 触摸驱动芯片片选信号的PORT口
    #define TOUCH_CS_CLK                    RCC_APB2Periph_GPIOC                ///< 触摸驱动芯片片选信号的时钟模块
    #define TOUCH_CS_PIN                    GPIO_Pin_8                          ///< 触摸驱动芯片片选信号的PIN口

    // 触摸配置
    #define TOUCH_AD_LEFT                   400                                 ///< 触摸屏左边采样值
    #define TOUCH_AD_RIGHT                  4000                                ///< 触摸屏右边采样值
    #define TOUCH_AD_TOP                    400                                 ///< 触摸屏顶边采样值
    #define TOUCH_AD_BOTTOM                 4000                                ///< 触摸屏底边采样值

    // 选择触摸屏驱动
    #include "touch_chip_tsc2046.h"
#endif // LCD_ENABLE && GUI_SUPPORT_TOUCH



/*-----------------------------------------------------------------------------/
/ GUI总初始化函数
/-----------------------------------------------------------------------------*/
extern void gui_main_init(void);                                                ///< gui总初始化函数, 并准备好gui所需的任务.



#endif // __LCDGUI_CONF_H__

/** @} */
/** @} */
