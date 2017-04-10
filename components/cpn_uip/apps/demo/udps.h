/**
  ******************************************************************************
  * @file    udps.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.16
  * @brief   提供 udp 服务器示例.
  *          此范例中, UDP服务器每次只能针对一个客户端进行服务.
  *          UDP的服务器客户端模型本质上并不明显.
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
  * @brief    用户配置
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup UDPServer_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Public_Function
  * @brief    定义公有函数
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
