/**
  ******************************************************************************
  * @file    udps.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.16
  * @brief   �ṩ udp ������ʾ��.
  *          �˷�����, UDP������ÿ��ֻ�����һ���ͻ��˽��з���.
  *          UDP�ķ������ͻ���ģ�ͱ����ϲ�������.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UDPS_H
#define __UDPS_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include "tcpip.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup UDPServer
  * @{
  */



/**
  ******************************************************************************
  * @defgroup UDPServer_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup UDPServer_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Public_Function
  * @brief    ���幫�к���
  * @{
  */
void example_udp_server(uint16_t port);
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
