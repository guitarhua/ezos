/**
  ******************************************************************************
  * @file    tcpc.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.8
  * @brief   提供 tcp 客户端示例
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
  * @brief    用户配置
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup TCPClient_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Public_Function
  * @brief    定义公有函数
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
