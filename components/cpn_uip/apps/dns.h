/**
  ******************************************************************************
  * @file    dns.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.26
  * @brief   提供 DNS CLIENT 功能. 域名解析
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DNS_H
#define __DNS_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include "tcpip.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup DNS
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DNS_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
///// 配置DNS客户端功能及DNS表大小(域名解析). 0-不使能, >0-使能, 值为DNS表大小, 建议值4
///// 此功能需要使能 UIP_CONF_UDP
//#define UIP_DNS                         4
/**
  * @}
  */



/** @defgroup DNS_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
typedef enum
{
    DNS_OK,
    DNS_ERROR,
    DNS_BUSY,
} dns_t;
/**
  * @}
  */

/** @defgroup DNS_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DNS_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DNS_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if UIP_DNS

dns_t dns_lookup(const char *name, uip_ipaddr_t *ip);
void dns_clear(void);

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
