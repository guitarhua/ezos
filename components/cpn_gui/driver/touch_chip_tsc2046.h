/**
  ******************************************************************************
  * @file    touch_chip_tsc2046.h
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2013.10.3
  * @brief   ����������оƬ TSC2046 �ײ�����.
  *          ��Ϊ���װ汾, δ�����˲����ܺ�У�鹦��
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



// IO�ڳ�ʼ��
__INLINE static void touch_port_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // SPI1 ʱ��ԴΪ72Mhz, ����SPI ʱ��ԴΪ36Mhz.
    spi_init(TOUCH_SPI_NAME, SPI_TYPE_LOW_EDGE1_MSB, SPI_BaudRatePrescaler_256);

    // ʹ��ģ��SPI.
//  spi_init(TOUCH_SPI_NAME, SPI_TYPE_LOW_EDGE1_MSB, 250);

    // �����ȳ�ʼ��SS����Ϊ�������, �ٳ�ʼ��CS����
    RCC_APB2PeriphClockCmd(TOUCH_CS_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = TOUCH_CS_PIN;
    GPIO_Init(TOUCH_CS_PORT, &GPIO_InitStructure);
    TOUCH_CS_PORT->BSRR = TOUCH_CS_PIN;
}

/// ��������оƬͨ���շ�����
__INLINE static uint8_t touch_tx_rx_byte(uint8_t data)
{
    return spi_tx_rx_byte(TOUCH_SPI_NAME, data);
}

// ��������оƬƬѡ�ź�����ʹ��, ��IO�ڵ�ƽ�õ�
__INLINE static void touch_cs_enable(void)
{
    TOUCH_CS_PORT->BRR = TOUCH_CS_PIN;
}

// ��������оƬƬѡ�ź����Ž�ֹ, ��IO�ڵ�ƽ�ø�
__INLINE static void touch_cs_disable(void)
{
    TOUCH_CS_PORT->BSRR = TOUCH_CS_PIN;
}

// ������оƬ��ʼ��
__INLINE static void touch_chip_init(void)
{
}

// ��ȡ2046�����Ĳ���ֵ
__INLINE static int tsc2046_read_adc(uint8_t channel)
{
    uint16_t val;

    touch_cs_enable();                                                          // ʹ��TSC2046Ƭѡ

    /*
        TSC2046 �����֣�8Bit��
        Bit7   = S     ��ʼλ, ������1
        Bit6:4 = A2-A0 ģ������ͨ��ѡ��A2-A0; ����6��ͨ����
        Bit3   = MODE  ADCλ��ѡ��, 0-12Bit; 1-8Bit
        Bit2   = SER/DFR ģ��������ʽ, 0-�������; 1-��������
        Bit1:0 = PD1-PD0 ����ģʽѡ��λ
    */
    touch_tx_rx_byte((1 << 7) | (channel << 4));                                // ѡ�����ͨ��

    // ��ADC���, 12λADCֵ�ĸ�λ�ȴ���ǰ12bit��Ч�����4bit��0
    val = touch_tx_rx_byte(0x00);
    val <<= 8;
    val += touch_tx_rx_byte(0x00);
    val >>= 3;

    touch_cs_disable();                                                         // ��ֹTSC2046Ƭѡ
    return (val);
}

#define TSC2046_CHANNEL_X                       1                               // ��һͨ��������Xλ��
#define TSC2046_CHANNEL_Y                       5                               // ����ͨ��������Yλ��
// ��ȡ������X����ֵ
__INLINE static int touch_read_x(void)
{
    // ʵ��ʹ��������˲�, ȥ���������ɵ�
    return (tsc2046_read_adc(TSC2046_CHANNEL_X));
}

// ��ȡ������Y����ֵ
__INLINE static int touch_read_y(void)
{
    // ʵ��ʹ��������˲�, ȥ���������ɵ�
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
