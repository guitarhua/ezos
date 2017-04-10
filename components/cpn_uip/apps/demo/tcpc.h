/**
  ******************************************************************************
  * @file    tcpc.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.8
  * @brief   �ṩ tcp �ͻ���ʾ��
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TCPC_H
#define __TCPC_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include "tcpip.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup TCPClient
  * @{
  */



/**
  ******************************************************************************
  * @defgroup TCPClient_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup TCPClient_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Public_Function
  * @brief    ���幫�к���
  * @{
  */
void example_tcp_client(uip_ipaddr_t *ip, uint16_t port);
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
