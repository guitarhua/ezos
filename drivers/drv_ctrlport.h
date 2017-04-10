/**
  ******************************************************************************
  * @file    drv_ctrlport.h
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.15
  * @brief   ����������������. ���Դ����, �źſ���.
  *          �������оƬ���п��ƿ��ص�����, �������Ӧ�ù���оƬ��������.
  *          �������Ŀ�������һ����ͨ��������, MOS�ܵȷ���Ԫ���ﵽ����Ŀ��.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_CTRLPORT_H
#define __DRV_CTRLPORT_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_CtrlPort
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_CtrlPort_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define CTRLPORT_NUM                    5                                       ///< ���ÿ������ŵ�����. �������������16����������.
//
//#define CTRLPORT0_PORT                  GPIOC                                   ///< ���øÿ������ŵ�PORT��
//#define CTRLPORT0_CLK                   RCC_APB2Periph_GPIOC                    ///< ���øÿ������ŵ�ʱ��ģ��
//#define CTRLPORT0_PIN                   GPIO_Pin_8                              ///< ���øÿ������ŵ�PIN��
//#define CTRLPORT0_OFF                   1                                       ///< ���ùرտ������ŵĵ�ƽ
//
//#define CTRLPORT1_PORT                  GPIOD
//#define CTRLPORT1_CLK                   RCC_APB2Periph_GPIOD
//#define CTRLPORT1_PIN                   GPIO_Pin_9
//#define CTRLPORT1_OFF                   0
//
//#define CTRLPORT2_PORT                  GPIOD
//#define CTRLPORT2_CLK                   RCC_APB2Periph_GPIOD
//#define CTRLPORT2_PIN                   GPIO_Pin_10
//#define CTRLPORT2_OFF                   0
//
//#define CTRLPORT3_PORT                  GPIOD
//#define CTRLPORT3_CLK                   RCC_APB2Periph_GPIOD
//#define CTRLPORT3_PIN                   GPIO_Pin_11
//#define CTRLPORT3_OFF                   0
//
///// ���������������Ͷ���
//typedef enum
//{
//    SD_POWER = 0,                                                               ///< ����Ϊ�������Ŀ�������, ֵ0��ӦCTRLPORT0_xxx
//    SPK_POWER,                                                                  ///< ֵ1��ӦCTRLPORT1_xxx, ��������
//    SPK_SIGNAL,
//    LCD_BACKLIGHT,
//} ctrlport_name_t;



#if (CTRLPORT_NUM > 16)
    #error "CTRLPORT_NUM define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_CtrlPort_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_CtrlPort_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_CtrlPort_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_CtrlPort_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (CTRLPORT_NUM > 0)

void ctrlport_init_all(void);                                                     // ��ʼ�����еĿ�������
void ctrlport_init(ctrlport_name_t ctrlport_name);                                // ��ʼ��ָ���Ŀ�������
void ctrlport_on(ctrlport_name_t ctrlport_name);                                  // ���������߼���
void ctrlport_off(ctrlport_name_t ctrlport_name);                                 // ���������߼���
void ctrlport_toggle(ctrlport_name_t ctrlport_name);                              // �������ŷ�ת
uint8_t ctrlport_get_status(ctrlport_name_t ctrlport_name);                       // ��ȡ�������ŵ��߼�״̬. 0, �ر�. 1, ��

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
