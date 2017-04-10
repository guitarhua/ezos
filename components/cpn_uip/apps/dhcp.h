/**
  ******************************************************************************
  * @file    dhcp.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.26
  * @brief   �ṩ DHCP CLIENT ����. �Զ�����IP��ַ
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
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
///// ����DHCP�ͻ��˹���(�Զ�����IP��ַ). 0-��ʹ��, 1-ʹ��
///// �˹�����Ҫʹ�� UIP_CONF_UDP �� UIP_CONF_BROADCAST, ��ռ��һ��UDP����
//#define UIP_DHCP                        1
/**
  * @}
  */



/** @defgroup DHCP_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
typedef enum
{
    DHCP_NONE = 0,                      // DHCPδ����
    DHCP_OK,                            // DHCP����ɹ�
    DHCP_ERROR,                         // DHCP����ʧ��
    DHCP_BUSY,                          // DHCP������
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
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DHCP_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DHCP_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if UIP_DHCP

void dhcp_start(void);                                                          // DHCP����
void dhcp_stop(void);                                                           // DHCPֹͣ
dhcp_app_t *dhcp_get(void);                                                     // ��ȡDHCP״̬

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
