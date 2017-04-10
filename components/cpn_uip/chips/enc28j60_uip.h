/**
  ******************************************************************************
  * @file    enc28j60_uip.h
  * @author  mousie-yu
  * @version V1.1.1
  * @date    2013.5.5
  * @brief   ENC28J60�ײ���������. ���UIP�����д.
  *          ENC28J60 Ϊ Microchip Technology, Inc. ����.
  *          ��MACλַ����λ(OUI)Ϊ 00-04-A3, ����λ��оƬ��Ӧ���ṩ.
  *          ��оƬ��֧���Զ��������, ͨѶ���ȶ�, ������ʹ��!
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENC28J60_UIP_H
#define __ENC28J60_UIP_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "tapdev.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "drv_extint.h"
#include "drv_spi.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup ENC28J60_UIP
  * @{
  */



/**
  ******************************************************************************
  * @defgroup ENC28J60_UIP_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

/** @defgroup ENC28J60_UIP_Pin_Assignment
  * @brief    ENC28J60 ���Ŷ�Ӧ��ϵ, ����ͼ:
  *           +---------------------------------------------------------+
  *           |                       Pin assignment                    |
  *           +-------------------------+---------------+---------------+
  *           | STM32 Pins              |   ENC28J60    | Pin(SOIC/QFN) |
  *           +-------------------------+---------------+---------------+
  *           | ENC28J60_INT_PIN        |    _INT       |     4 / 28    |
  *           | ENC28J60_SPI_MISO_PIN   |    SO         |     6 / 2     |
  *           | ENC28J60_SPI_MOSI_PIN   |    SI         |     7 / 3     |
  *           | ENC28J60_SPI_SCK_PIN    |    SCK        |     8 / 4     |
  *           | ENC28J60_CS_PIN         |    _CS        |     9 / 5     |
  *           | ENC28J60_RESET_PIN      |    _RESET     |    10 / 6     |
  *           +-------------------------+---------------+---------------+
  * @{
  */
//#define ENC28J60_UIP_ENABLE                                                     ///< ʹ��enc28j60��ΪUIP����������оƬ
//
//#ifdef ENC28J60_UIP_ENABLE
//    #define ENC28J60_INT_NAME           ENC28J60_INT                            ///< ��uIPʹ���ж�ģʽ, ����һ�ⲿ�ж�, ��"drv_extint.h"������
//    #define ENC28J60_SPI_NAME           ENC28J60_SPI                            ///< ��������ʹ��һ��SPI, SPI��SCK, MISO, MOSI����"drv_spi.h"������
//    #define ENC28J60_CS_PORT            GPIOB                                   ///< ENC28J60Ƭѡ�źŵ�PORT��
//    #define ENC28J60_CS_CLK             RCC_APB2Periph_GPIOB                    ///< ENC28J60Ƭѡ�źŵ�ʱ��ģ��
//    #define ENC28J60_CS_PIN             GPIO_Pin_12                             ///< ENC28J60Ƭѡ�źŵ�PIN��
//#endif
/**
  * @}
  */

/**
  * @}
  */



/** @defgroup ENC28J60_UIP_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup ENC28J60_UIP_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup ENC28J60_UIP_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup ENC28J60_UIP_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#ifdef ENC28J60_UIP_ENABLE

void enc28j60_port_init(void);
void enc28j60_init(uip_macaddr_t mac);
void enc28j60_tx(uint8_t *pdata, uint16_t len);
uint16_t enc28j60_rx(uint8_t *pdata, uint16_t maxlen);
uint8_t  enc28j60_linked(void);

#if !UIP_TASK_RX_SCAN_MS
void enc28j60_isr(void);
#endif

#endif // ENC28J60_UIP_ENABLE
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
