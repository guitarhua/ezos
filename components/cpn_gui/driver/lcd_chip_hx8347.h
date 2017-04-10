/**
  ******************************************************************************
  * @file    lcd_chip_hx8347.h
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
#ifndef __LCD_CHIP_HX8347_H
#define __LCD_CHIP_HX8347_H

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
     //Driving ability Setting
    lcd_bus_cmd_wr(0xEA,0x00);          //PTBA[15:8]
    lcd_bus_cmd_wr(0xEB,0x20);          //PTBA[7:0]
    lcd_bus_cmd_wr(0xEC,0x3C);          //STBA[15:8]
    lcd_bus_cmd_wr(0xED,0xC4);          //STBA[7:0]
    lcd_bus_cmd_wr(0xE8,0x48);          //OPON[7:0]
    lcd_bus_cmd_wr(0xE9,0x38);          //OPON1[7:0]
    lcd_bus_cmd_wr(0xF1,0x01);          //OTPS1B
    lcd_bus_cmd_wr(0xF2,0x08);          //GEN
    //Gamma 2.2 Setting
    lcd_bus_cmd_wr(0x40,0x01);
    lcd_bus_cmd_wr(0x41,0x07);
    lcd_bus_cmd_wr(0x42,0x09);
    lcd_bus_cmd_wr(0x43,0x19);
    lcd_bus_cmd_wr(0x44,0x17);
    lcd_bus_cmd_wr(0x45,0x20);
    lcd_bus_cmd_wr(0x46,0x18);
    lcd_bus_cmd_wr(0x47,0x61);
    lcd_bus_cmd_wr(0x48,0x00);
    lcd_bus_cmd_wr(0x49,0x10);
    lcd_bus_cmd_wr(0x4A,0x17);
    lcd_bus_cmd_wr(0x4B,0x19);
    lcd_bus_cmd_wr(0x4C,0x14);
    lcd_bus_cmd_wr(0x50,0x1F);
    lcd_bus_cmd_wr(0x51,0x28);
    lcd_bus_cmd_wr(0x52,0x26);
    lcd_bus_cmd_wr(0x53,0x36);
    lcd_bus_cmd_wr(0x54,0x38);
    lcd_bus_cmd_wr(0x55,0x3E);
    lcd_bus_cmd_wr(0x56,0x1E);
    lcd_bus_cmd_wr(0x57,0x67);
    lcd_bus_cmd_wr(0x58,0x0B);
    lcd_bus_cmd_wr(0x59,0x06);
    lcd_bus_cmd_wr(0x5A,0x08);
    lcd_bus_cmd_wr(0x5B,0x0F);
    lcd_bus_cmd_wr(0x5C,0x1F);
    lcd_bus_cmd_wr(0x5D,0xCC);

    //Power Voltage Setting
    lcd_bus_cmd_wr(0x1B,0x1B);          //VRH=4.65V
    lcd_bus_cmd_wr(0x1A,0x01);          //BT (VGH~15V,VGL~-10V,DDVDH~5V)
    lcd_bus_cmd_wr(0x24,0x70);          //VMH(VCOM High voltage ~4.2V)
    lcd_bus_cmd_wr(0x25,0x58);          //VML(VCOM Low voltage -1.2V)

    //****VCOM offset**///
    lcd_bus_cmd_wr(0x23,0x6E);          //for Flicker adjust //can reload from OTP
    //Power on Setting
    lcd_bus_cmd_wr(0x18,0x36);          //I/P_RADJ,N/P_RADJ, Normal mode 70Hz
    lcd_bus_cmd_wr(0x19,0x01);          //OSC_EN='1', start Osc
    lcd_bus_cmd_wr(0x01,0x00);          //DP_STB='0', out deep sleep
    lcd_bus_cmd_wr(0x1F,0xD0);          // GAS=1, VOMG=10, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
    lcd_bus_cmd_wr(0x17,0x05);          //default 0x06 262k color // 0x05 65k color
    //SET PANEL
    lcd_bus_cmd_wr(0x36,0x09);          //SS_P, GS_P,REV_P,BGR_P
    lcd_bus_cmd_wr(0x28,0x3F);          //GON=1, DTE=1, D=1100
    lcd_bus_cmd_wr(0x16,0x10);          //左上到右下   竖屏
    lcd_bus_cmd_wr(0x02,0x00);
    lcd_bus_cmd_wr(0x03,0x00);          //Column Start
    lcd_bus_cmd_wr(0x04,0x00);
    lcd_bus_cmd_wr(0x05,0xEF);          //Column End

    lcd_bus_cmd_wr(0x06,0x00);
    lcd_bus_cmd_wr(0x07,0x00);          //Row Start
    lcd_bus_cmd_wr(0x08,0x01);
    lcd_bus_cmd_wr(0x09,0x3F);          //Row End

    chip_delay_ms(100);
}

// 写像素点函数, 必须支持
__INLINE static void lcd_chip_pixel_wr(uint16_t x, uint16_t y, uint16_t rgb565)
{
    lcd_bus_cmd_wr(0x02, x >> 8);
    lcd_bus_cmd_wr(0x03, x & 0xFF);
    lcd_bus_cmd_wr(0x06, y >> 8);
    lcd_bus_cmd_wr(0x07, y & 0xFF);

    lcd_bus_cmd_wr(0x04, x >> 8);
    lcd_bus_cmd_wr(0x05, x & 0xFF);
    lcd_bus_cmd_wr(0x08, y >> 8);
    lcd_bus_cmd_wr(0x09, y & 0xFF);

    lcd_bus_cmd_wr(0x22, rgb565);
}

// 设置画布位置和大小, 必须支持. 起始点设置在左上角(x,y)出, 宽为w, 长为l,
__INLINE static void lcd_chip_canvas(uint16_t x, uint16_t y, uint16_t w, uint16_t l)
{
    lcd_bus_cmd_wr(0x02, x >> 8);
    lcd_bus_cmd_wr(0x03, x & 0xFF);
    lcd_bus_cmd_wr(0x06, y >> 8);
    lcd_bus_cmd_wr(0x07, y & 0xFF);

    lcd_bus_cmd_wr(0x04, (x+w-1) >> 8);
    lcd_bus_cmd_wr(0x05, (x+w-1) & 0xFF);
    lcd_bus_cmd_wr(0x08, (y+l-1) >> 8);
    lcd_bus_cmd_wr(0x09, (y+l-1) & 0xFF);
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
    volatile uint16_t r, g, b;

    lcd_bus_cmd_wr(0x02, x >> 8);
    lcd_bus_cmd_wr(0x03, x & 0xFF);
    lcd_bus_cmd_wr(0x06, y >> 8);
    lcd_bus_cmd_wr(0x07, y & 0xFF);

    lcd_bus_cmd_wr(0x04, x >> 8);
    lcd_bus_cmd_wr(0x05, x & 0xFF);
    lcd_bus_cmd_wr(0x08, y >> 8);
    lcd_bus_cmd_wr(0x09, y & 0xFF);

    lcd_chip_rd_start();
    lcd_chip_rding();                   // dummy
    b = lcd_chip_rding();               // b is a temp
    r = (b & 0xF800);
    g = (b & 0xFC) << 3;
    b = lcd_chip_rding();
    b >>= 11;
    lcd_chip_rd_stop();

    return (r | g | b);
}



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
