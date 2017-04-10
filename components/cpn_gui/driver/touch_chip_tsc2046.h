/**
  ******************************************************************************
  * @file    touch_chip_tsc2046.h
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2013.10.3
  * @brief   电阻屏驱动芯片 TSC2046 底层驱动.
  *          此为简易版本, 未加入滤波功能和校验功能
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TOUCH_CHIP_TSC2046_H
#define __TOUCH_CHIP_TSC2046_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f10x.h"                                                          // __INLINE

#include "lcdgui_conf.h"
#include "drv_extint.h"
#include "drv_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup CPN_GUI
  * @{
  */



// IO口初始化
__INLINE static void touch_port_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // SPI1 时钟源为72Mhz, 其它SPI 时钟源为36Mhz.
    spi_init(TOUCH_SPI_NAME, SPI_TYPE_LOW_EDGE1_MSB, SPI_BaudRatePrescaler_256);

    // 使用模拟SPI.
//  spi_init(TOUCH_SPI_NAME, SPI_TYPE_LOW_EDGE1_MSB, 250);

    // 必须先初始化SS引脚为软件控制, 再初始化CS引脚
    RCC_APB2PeriphClockCmd(TOUCH_CS_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = TOUCH_CS_PIN;
    GPIO_Init(TOUCH_CS_PORT, &GPIO_InitStructure);
    TOUCH_CS_PORT->BSRR = TOUCH_CS_PIN;
}

/// 触摸驱动芯片通用收发函数
__INLINE static uint8_t touch_tx_rx_byte(uint8_t data)
{
    return spi_tx_rx_byte(TOUCH_SPI_NAME, data);
}

// 触摸驱动芯片片选信号引脚使能, 将IO口电平置低
__INLINE static void touch_cs_enable(void)
{
    TOUCH_CS_PORT->BRR = TOUCH_CS_PIN;
}

// 触摸驱动芯片片选信号引脚禁止, 将IO口电平置高
__INLINE static void touch_cs_disable(void)
{
    TOUCH_CS_PORT->BSRR = TOUCH_CS_PIN;
}

// 触摸屏芯片初始化
__INLINE static void touch_chip_init(void)
{
}

// 读取2046驱动的采样值
__INLINE static int tsc2046_read_adc(uint8_t channel)
{
    uint16_t val;

    touch_cs_enable();                                                          // 使能TSC2046片选

    /*
        TSC2046 控制字（8Bit）
        Bit7   = S     起始位, 必须是1
        Bit6:4 = A2-A0 模拟输入通道选择A2-A0; 共有6个通道。
        Bit3   = MODE  ADC位数选择, 0-12Bit; 1-8Bit
        Bit2   = SER/DFR 模拟输入形式, 0-差分输入; 1-单端输入
        Bit1:0 = PD1-PD0 掉电模式选择位
    */
    touch_tx_rx_byte((1 << 7) | (channel << 4));                                // 选择测量通道

    // 读ADC结果, 12位ADC值的高位先传，前12bit有效，最后4bit填0
    val = touch_tx_rx_byte(0x00);
    val <<= 8;
    val += touch_tx_rx_byte(0x00);
    val >>= 3;

    touch_cs_disable();                                                         // 禁止TSC2046片选
    return (val);
}

#define TSC2046_CHANNEL_X                       1                               // 第一通道，测量X位置
#define TSC2046_CHANNEL_Y                       5                               // 第五通道，测量Y位置
// 读取触摸屏X轴数值
__INLINE static int touch_read_x(void)
{
    // 实际使用需加入滤波, 去除电阻屏飞点
    return (tsc2046_read_adc(TSC2046_CHANNEL_X));
}

// 读取触摸屏Y轴数值
__INLINE static int touch_read_y(void)
{
    // 实际使用需加入滤波, 去除电阻屏飞点
    return (tsc2046_read_adc(TSC2046_CHANNEL_Y));
}



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
