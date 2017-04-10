/**
  ******************************************************************************
  * @file    lcd_bus_8080.h
  * @author  mousie-yu
  * @version V2.2.0
  * @date    2013.10.10
  * @brief   LCD 16位并行通讯总线, 8080模式(intel), 模拟IO口驱动.
  *          8080 是通过 "读使能(RE)" 和 "写使能(WE)" 控制线进行读写操作.
  *          并入 emwin, 增减若干函数.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_BUS_8080_H
#define __LCD_BUS_8080_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f10x.h"                                                          // __INLINE

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup CPN_GUI
  * @{
  */



// 并行INTEL 8080总线模式IO口宏定义
#define LCD_CS_HIGH()                   LCD_CS_PORT->BSRR = LCD_CS_PIN
#define LCD_CS_LOW()                    LCD_CS_PORT->BRR  = LCD_CS_PIN
#define LCD_RS_HIGH()                   LCD_RS_PORT->BSRR = LCD_RS_PIN
#define LCD_RS_LOW()                    LCD_RS_PORT->BRR  = LCD_RS_PIN
#define LCD_WR_HIGH()                   LCD_WR_PORT->BSRR = LCD_WR_PIN
#define LCD_WR_LOW()                    LCD_WR_PORT->BRR  = LCD_WR_PIN
#define LCD_RD_HIGH()                   LCD_RD_PORT->BSRR = LCD_RD_PIN
#define LCD_RD_LOW()                    LCD_RD_PORT->BRR  = LCD_RD_PIN
#define LCD_DATA_GET()                  (LCD_DATA_PORT->IDR)
#define LCD_DATA_SET(data)              LCD_DATA_PORT->ODR = data;
#define LCD_GPIO_Pin                    ((uint16_t)0x00FF)                      // LCD数据引脚Pin为0-7


// IO口初始化
__INLINE static void lcd_bus_port_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(LCD_DATA_CLK | LCD_CS_CLK | LCD_RS_CLK | LCD_WR_CLK | LCD_RD_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;                           // 设置为推挽输出
    GPIO_InitStructure.GPIO_Pin   = LCD_GPIO_Pin;
    GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure);                              // 16bit 数据线
    GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;                                   // 片选
    GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;                                   // 寄存器/数据
    GPIO_Init(LCD_RS_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LCD_WR_PIN;                                   // 写使能
    GPIO_Init(LCD_WR_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LCD_RD_PIN;                                   // 读使能
    GPIO_Init(LCD_RD_PORT, &GPIO_InitStructure);

    LCD_CS_HIGH();
    LCD_RS_HIGH();
    LCD_WR_HIGH();
    LCD_RD_HIGH();
}

// 写寄存器
__INLINE static void lcd_bus_reg_wr(uint16_t reg)
{
    LCD_CS_LOW();
    LCD_RS_LOW();

    LCD_DATA_SET(reg);
    LCD_WR_LOW();
    LCD_WR_HIGH();

    LCD_CS_HIGH();
}

// 写数据
__INLINE static void lcd_bus_data_wr(uint16_t data)
{
    LCD_CS_LOW();
    LCD_RS_HIGH();

    LCD_DATA_SET(data);
    LCD_WR_LOW();
    LCD_WR_HIGH();

    LCD_CS_HIGH();
}

// 写指令(寄存器+数据)
__INLINE static void lcd_bus_cmd_wr(uint16_t reg, uint16_t data)
{
    lcd_bus_reg_wr(reg);
    lcd_bus_data_wr(data);
}

// 准备写数据
__INLINE static void lcd_bus_datas_wr_start(void)
{
    LCD_RS_HIGH();
    LCD_RD_HIGH();
    LCD_WR_HIGH();
    LCD_CS_LOW();
}

// 连续写数据
__INLINE static void lcd_bus_datas_wring(uint16_t data)
{
    LCD_DATA_SET(data);
    LCD_WR_LOW();
    LCD_WR_HIGH();
}

// 结束写数据
__INLINE static void lcd_bus_datas_wr_stop(void)
{
    LCD_CS_HIGH();
}

// 准备读数据
__INLINE static void lcd_bus_datas_rd_start(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;                      // 设置为高阻输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = LCD_GPIO_Pin;
    GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure);

    LCD_RS_HIGH();
    LCD_RD_HIGH();
    LCD_WR_HIGH();
    LCD_CS_LOW();
}

// 连续读数据
__INLINE static uint16_t lcd_bus_datas_rding(void)
{
    uint16_t val;

    LCD_RD_LOW();
    LCD_RD_HIGH();
    val = LCD_DATA_GET();

    return val;
}

// 结束读数据
__INLINE static void lcd_bus_datas_rd_stop(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    LCD_CS_HIGH();

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;                           // 设置为推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = LCD_GPIO_Pin;
    GPIO_Init(LCD_DATA_PORT, &GPIO_InitStructure);
}



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
