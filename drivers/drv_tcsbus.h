/**
  ******************************************************************************
  * @file    drv_tcsbus.h
  * @author  mousie
  * @version V2.0.0
  * @date    2012.10.26
  * @brief   TCS BUS �ײ�����. δ����
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_TCSBUS_H
#define __DRV_TCSBUS_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_TCSBUS
  * @{
  */
/**
  ******************************************************************************
  * @defgroup DRV_TCSBUS_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define TCSBUS_NUM                      3                                       ///< ����ʹ�õ�TCSBUS����.�������������3��TCSBUS����
//
//#if (TCSBUS_NUM > 0)
//    #define TCSBUS0_TX_PORT             GPIOA                                   ///< ���ø�TCSBUS���������PORT��
//    #define TCSBUS0_TX_CLK              RCC_APB2Periph_GPIOA                    ///< ���ø�TCSBUS���������ʱ��ģ��
//    #define TCSBUS0_TX_PIN              GPIO_Pin_0                              ///< ���ø�TCSBUS���������PIN��
//
//    #define TCSBUS0_RX_PORT             GPIOA                                   ///< ���ø�TCSBUS����������PORT��
//    #define TCSBUS0_RX_CLK              RCC_APB2Periph_GPIOA                    ///< ���ø�TCSBUS����������ʱ��ģ��
//    #define TCSBUS0_RX_PIN              GPIO_Pin_1                              ///< ���ø�TCSBUS����������PIN��
//
//    /**
//      * @brief ���ø�TCSBUS�Ľ��ջص�����
//      *   @arg 0,                                           ��ʹ�ûص�����, ���Խ������ݽ��д���
//      *   @arg void fun(uint8_t type, uint32_t data),       �ص�������ʽ, �궨��fun�������Ƽ���
//      */
//    #define TCSBUS0_RX_CALLBACK         0
//#endif
//
//#if (TCSBUS_NUM > 1)
//    #define TCSBUS1_TX_PORT             GPIOA
//    #define TCSBUS1_TX_CLK              RCC_APB2Periph_GPIOA
//    #define TCSBUS1_TX_PIN              GPIO_Pin_2
//
//    #define TCSBUS1_RX_PORT             GPIOA
//    #define TCSBUS1_RX_CLK              RCC_APB2Periph_GPIOA
//    #define TCSBUS1_RX_PIN              GPIO_Pin_3
//
//    #define TCSBUS1_RX_CALLBACK         0
//#endif
//
//#if (TCSBUS_NUM > 2)
//    #define TCSBUS2_TX_PORT             GPIOB
//    #define TCSBUS2_TX_CLK              RCC_APB2Periph_GPIOB
//    #define TCSBUS2_TX_PIN              GPIO_Pin_0
//
//    #define TCSBUS2_RX_PORT             GPIOB
//    #define TCSBUS2_RX_CLK              RCC_APB2Periph_GPIOB
//    #define TCSBUS2_RX_PIN              GPIO_Pin_1
//
//    #define TCSBUS2_RX_CALLBACK         0
//#endif
//
///// TCSBUS�����������Ͷ���
//typedef enum
//{
//    ETCSBUS = 0,                                                                ///< ����Ϊ��������TCSBUS����, ֵ0��ӦTCSBUS0_xxx
//    LTCSBUS,                                                                    ///< ֵ1��ӦTCSBUS1_xxx, ��������
//    STCSBUS,
//} tcsbus_name_t;



#if (TCSBUS_NUM > 3)
    #error "TCSBUS_NUM define error!"
#endif

/**
  * @}
  */



/** @defgroup DRV_TCSBUS_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
/// TCSBUS����Э�����Ͷ���
typedef enum
{
    A_LONG  = (0x00 | 0x01),                                                    ///< A��Э��, 32bit
    A_SHORT = (0x00 | 0x00),                                                    ///< A��Э��, 16bit
    Q_LONG  = (0x10 | 0x01),                                                    ///< Q��Э��, 32bit
    Q_SHORT = (0x10 | 0x00),                                                    ///< Q��Э��, 4bit
    Q_NONE  = (0x10 | 0x0F),                                                    ///< �ȴ�QЭ�鳬ʱ
} tcsbus_type_t;
/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_TCSBUS_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (TCSBUS_NUM > 0)

void tcsbus_init_all(void);                                                     // ��ʼ������ TCSBUS ����
void tcsbus_init(tcsbus_name_t tcsbus_name);                                    // ��ʼ��ָ�� TCSBUS ����
void tcsbus_scan_200us(tcsbus_name_t tcsbus_name);                              // TCSBUS 200us��ʱɨ�躯��

uint8_t tcsbus_tx(tcsbus_name_t tcsbus_name, tcsbus_type_t type, uint32_t data);// TCSBUS ����Э��, ����0��ʾ���ͳɹ�

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
