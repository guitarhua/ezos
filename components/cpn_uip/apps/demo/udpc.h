/**
  ******************************************************************************
  * @file    udpc.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.16
  * @brief   提供 udp 客户端示例
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
  * @brief    用户配置
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup UDPClient_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Public_Function
  * @brief    定义公有函数
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
