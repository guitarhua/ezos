/**
  ******************************************************************************
  * @file    udpc.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.16
  * @brief   �ṩ udp �ͻ���ʾ��
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UDPC_H
#define __UDPC_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include "tcpip.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup UDPClient
  * @{
  */



/**
  ******************************************************************************
  * @defgroup UDPClient_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup UDPClient_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Public_Function
  * @brief    ���幫�к���
  * @{
  */
void example_udp_client(uip_ipaddr_t *ip, uint16_t port);
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
