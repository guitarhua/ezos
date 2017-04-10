/**
  ******************************************************************************
  * @file    lcd_chip_ili9320.h
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2013.9.20
  * @brief   LCD 驱动芯片, 需提供 芯片初始化, 画点等函数.
  *          需要 lcd_bus_xxx 文件的支持.
  *          基本驱动仅需如下三个函数:
  *          lcd_chip_init(), lcd_chip_pixel_wr(), lcd_chip_canvas()
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_CHIP_ILI9320_H
#define __LCD_CHIP_ILI9320_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f10x.h"                                                          // __INLINE

#include "lcd_main.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup CPN_GUI
  * @{
  */



#include "drv_systick.h"
#define chip_delay_ms(time)             delay_ms(time)                          ///< 设置LCD驱动芯片毫秒延时函数



// LCD 驱动芯片初始化
__INLINE static void lcd_chip_init(void)
{
    lcd_bus_cmd_wr(0x00,0x0000);
    lcd_bus_cmd_wr(0x01,0x0100);        // Driver Output Contral.
    lcd_bus_cmd_wr(0x02,0x0700);        // LCD Driver Waveform Contral.
    // 刷屏方向1030-L2R&U2D, 1018-D2U&L2R
    lcd_bus_cmd_wr(0x03,0x1030);        // Entry Mode Set.

    lcd_bus_cmd_wr(0x04,0x0000);        // Scalling Contral.
    lcd_bus_cmd_wr(0x08,0x0202);        // Display Contral 2.(0x0207)
    lcd_bus_cmd_wr(0x09,0x0000);        // Display Contral 3.(0x0000)
    lcd_bus_cmd_wr(0x0a,0x0000);        // Frame Cycle Contal.(0x0000)
    lcd_bus_cmd_wr(0x0c,(1<<0));        // Extern Display Interface Contral 1.(0x0000)
    lcd_bus_cmd_wr(0x0d,0x0000);        // Frame Maker Position.
    lcd_bus_cmd_wr(0x0f,0x0000);        // Extern Display Interface Contral 2.

    chip_delay_ms(100);
    lcd_bus_cmd_wr(0x07,0x0101);        // Display Contral.
    chip_delay_ms(100);

    lcd_bus_cmd_wr(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));   // Power Control 1.(0x16b0)
    lcd_bus_cmd_wr(0x11,0x0007);        // Power Control 2.(0x0001)
    lcd_bus_cmd_wr(0x12,(1<<8)|(1<<4)|(0<<0));                  // Power Control 3.(0x0138)
    lcd_bus_cmd_wr(0x13,0x0b00);        // Power Control 4.
    lcd_bus_cmd_wr(0x29,0x0000);        // Power Control 7.

    lcd_bus_cmd_wr(0x2b,(1<<14)|(1<<4));

    lcd_bus_cmd_wr(0x50,0);             // Set X Start.
    lcd_bus_cmd_wr(0x51,239);           // Set X End.
    lcd_bus_cmd_wr(0x52,0);             // Set Y Start.
    lcd_bus_cmd_wr(0x53,319);           // Set Y End.

    lcd_bus_cmd_wr(0x60,0x2700);        // Driver Output Control.
    lcd_bus_cmd_wr(0x61,0x0001);        // Driver Output Control.
    lcd_bus_cmd_wr(0x6a,0x0000);        // Vertical Srcoll Control.

    lcd_bus_cmd_wr(0x80,0x0000);        // Display Position? Partial Display 1.
    lcd_bus_cmd_wr(0x81,0x0000);        // RAM Address Start? Partial Display 1.
    lcd_bus_cmd_wr(0x82,0x0000);        // RAM Address End-Partial Display 1.
    lcd_bus_cmd_wr(0x83,0x0000);        // Displsy Position? Partial Display 2.
    lcd_bus_cmd_wr(0x84,0x0000);        // RAM Address Start? Partial Display 2.
    lcd_bus_cmd_wr(0x85,0x0000);        // RAM Address End? Partial Display 2.

    lcd_bus_cmd_wr(0x90,(0<<7)|(16<<0));// Frame Cycle Contral.(0x0013)
    lcd_bus_cmd_wr(0x92,0x0000);        // Panel Interface Contral 2.(0x0000)
    lcd_bus_cmd_wr(0x93,0x0001);        // Panel Interface Contral 3.
    lcd_bus_cmd_wr(0x95,0x0110);        // Frame Cycle Contral.(0x0110)
    lcd_bus_cmd_wr(0x97,(0<<8));
    lcd_bus_cmd_wr(0x98,0x0000);        // Frame Cycle Contral.

    lcd_bus_cmd_wr(0x07,0x0173);

    chip_delay_ms(100);
}

// 写像素点函数, 必须支持
__INLINE static void lcd_chip_pixel_wr(uint16_t x, uint16_t y, uint16_t rgb565)
{
    lcd_bus_cmd_wr(0x50, x);
    lcd_bus_cmd_wr(0x51, x);
    lcd_bus_cmd_wr(0x52, y);
    lcd_bus_cmd_wr(0x53, y);            // 设置显示范围
    lcd_bus_cmd_wr(0x20, x);
    lcd_bus_cmd_wr(0x21, y);            // 设置起始点
    lcd_bus_cmd_wr(0x22, rgb565);
}

// 设置画布位置和大小, 必须支持. 起始点设置在左上角(x,y)出, 宽为w, 长为l,
__INLINE static void lcd_chip_canvas(uint16_t x, uint16_t y, uint16_t w, uint16_t l)
{
    lcd_bus_cmd_wr(0x50, x);
    lcd_bus_cmd_wr(0x51, x+w-1);
    lcd_bus_cmd_wr(0x52, y);
    lcd_bus_cmd_wr(0x53, y+l-1);
    lcd_bus_cmd_wr(0X20, x);
    lcd_bus_cmd_wr(0X21, y);
}

// 准备写入一串颜色数据
__INLINE static void lcd_chip_wr_start(void)
{
    lcd_bus_reg_wr(0x22);
    lcd_bus_datas_wr_start();
}

// 写入一串颜色数据
__INLINE static void lcd_chip_wring(uint16_t rgb565)
{
    lcd_bus_datas_wring(rgb565);
}

// 结束写入一串颜色数据
__INLINE static void lcd_chip_wr_stop(void)
{
    lcd_bus_datas_wr_stop();
}

// 准备读取一串颜色数据
__INLINE static void lcd_chip_rd_start(void)
{
    lcd_bus_reg_wr(0x22);
    lcd_bus_datas_rd_start();
}

// 读取一串颜色数据
__INLINE static uint16_t lcd_chip_rding()
{
    return lcd_bus_datas_rding();
}

// 结束读取一串颜色数据
__INLINE static void lcd_chip_rd_stop(void)
{
    lcd_bus_datas_rd_stop();
}

// 读像素点函数
__INLINE static uint16_t lcd_chip_pixel_rd(uint16_t x, uint16_t y)
{
    uint16_t val;

    lcd_bus_cmd_wr(0x20, x);
    lcd_bus_cmd_wr(0x21, y);            // 设置起始点
    lcd_chip_rd_start();
    lcd_chip_rding();                   // dummy
    lcd_chip_rding();                   // dummy
    val = lcd_chip_rding();             // 返回类型为BGR565
    lcd_chip_rd_stop();

    return ((val << 11) | (val & 0x07E0) | (val >> 11));
}



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
