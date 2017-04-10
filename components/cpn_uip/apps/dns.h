/**
  ******************************************************************************
  * @file    dns.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.26
  * @brief   �ṩ DNS CLIENT ����. ��������
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
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
///// ����DNS�ͻ��˹��ܼ�DNS���С(��������). 0-��ʹ��, >0-ʹ��, ֵΪDNS���С, ����ֵ4
///// �˹�����Ҫʹ�� UIP_CONF_UDP
//#define UIP_DNS                         4
/**
  * @}
  */



/** @defgroup DNS_Public_TypeDefine
  * @brief    �������Ͷ���
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
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DNS_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DNS_Public_Function
  * @brief    ���幫�к���
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
