/**
  ******************************************************************************
  * @file    tcps.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.8
  * @brief   提供 tcp 服务器示例.
  *          服务器的应用比客户端复杂, 服务器需要为各客户端独立划分出数据空间.
  *          此范例是使用 memb.c 文件实现的. 并将分配获得的空间通过 *appdata传递.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TCPS_H
#define __TCPS_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include "tcpip.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup TCPServer
  * @{
  */



/**
  ******************************************************************************
  * @defgroup TCPServer_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup TCPServer_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPServer_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPServer_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPServer_Public_Function
  * @brief    定义公有函数
  * @{
  */
void example_tcp_server(uint16_t port);
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
