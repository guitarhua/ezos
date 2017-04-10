/**
  ******************************************************************************
  * @file    dhcp.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.26
  * @brief   提供 DHCP CLIENT 功能. 自动分配IP地址
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DHCP_H
#define __DHCP_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include "tcpip.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup DHCP
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DHCP_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
///// 配置DHCP客户端功能(自动分配IP地址). 0-不使能, 1-使能
///// 此功能需要使能 UIP_CONF_UDP 及 UIP_CONF_BROADCAST, 并占用一个UDP连接
//#define UIP_DHCP                        1
/**
  * @}
  */



/** @defgroup DHCP_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
typedef enum
{
    DHCP_NONE = 0,                      // DHCP未启动
    DHCP_OK,                            // DHCP申请成功
    DHCP_ERROR,                         // DHCP申请失败
    DHCP_BUSY,                          // DHCP申请中
} dhcp_state_t;

typedef struct {
    dhcp_state_t state;
    uint8_t  serverid[4];
    uint16_t lease_time[2];
    uip_ipaddr_t ipaddr;
    uip_ipaddr_t netmask;
    uip_ipaddr_t dnsaddr;
    uip_ipaddr_t router;
} dhcp_app_t;
/**
  * @}
  */

/** @defgroup DHCP_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DHCP_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DHCP_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if UIP_DHCP

void dhcp_start(void);                                                          // DHCP启动
void dhcp_stop(void);                                                           // DHCP停止
dhcp_app_t *dhcp_get(void);                                                     // 获取DHCP状态

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
