/**
  ******************************************************************************
  * @file    uip_types.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.3.28
  * @brief   UIP �������Ͷ��� �� �ײ�궨��
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UIP_TYPES_H
#define __UIP_TYPES_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>



/** @addtogroup Components
  * @{
  */
/** @addtogroup UIP_Types
  * @{
  */



/** @defgroup UIP_Types_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
typedef uint16_t                        uip_stats_t;
typedef struct uip_eth_addr             uip_macaddr_t;
typedef struct uip_conn                 uip_conn_t;
typedef struct uip_udp_conn             uip_udp_conn_t;
typedef uint16_t                        sock_s_t;

typedef enum
{
    UIP_TYPE_NULL   = 0,                                                        // �ص�����������
    TCP_SERVER      = 1,                                                        // TCP����������
    TCP_CLIENT      = 2,                                                        // TCP�ͻ�������
    UDP_SERVER      = 3,                                                        // UDP����������
    UDP_CLIENT      = 4,                                                        // UDP�ͻ�������
} uip_type_t;

typedef enum
{
    SOCKET_OK       = 0,                                                        // socket�շ��ɹ�/��������
    SOCKET_TIMEOUT  = 1,                                                        // socket�շ�ʧ��/����ʱ
    SOCKET_WAIT     = 2,                                                        // socket�շ��ȴ�״̬
    SOCKET_FULL     = 3,                                                        // socket�����������
    SOCKET_CLOSED   = 4,                                                        // socket�����ѹر�
    SOCKET_ERROR    = 0xFF,                                                     // socket��������
} socket_t;

typedef enum
{
    SOCK_NULL       = 0x00,                                                     // socket�ص�������ʼ״̬
    SOCK_DELAY      = 0x81,                                                     // socket�ص�������ʱ״̬
    SOCK_TX_ING     = 0x82,                                                     // socket�ص�������������״̬
    SOCK_TX_ACK     = 0x83,                                                     // socket�ص������ȴ�Ӧ��
    SOCK_RX_ING     = 0x85,                                                     // socket�ص�������������״̬
    SOCK_RX_OK      = 0x06,                                                     // socket�ص�������ɽ���
    SOCK_CLOSING    = 0x87,                                                     // socket�ص�����TCP�ر�����״̬
    SOCK_CLOSED     = 0x88,                                                     // socket�ص����������ѹر�
    SOCK_BREAK_MASK = 0x80,                                                     // socket��λ����, ��������ѭ��
    SOCK_ERROR      = 0xFF,                                                     // socket�ص���������״̬
} uip_sock_t;

typedef struct
{
    void *conn;                                                                 // socket���������ӵ�ַ
    void (*psock)(sock_s_t *state, void **appdata);                             // socket�ص�����ָ��
    void *appdata;                                                              // socket�ص�����Ӧ�ò�����ָ��
    int32_t time;                                                               // socket�ص�������ʱʱ��
    uint8_t *sockptr;                                                           // socket����/������ָ��
    uint16_t socklen;                                                           // socket����/����������
    sock_s_t state;                                                             // socket������״̬
    uip_type_t type;                                                            // socket����(TCP/UDP, SERVER/CLIENT)
    uip_sock_t sock;                                                            // socketЭ��״̬
} uip_task_t;

typedef uip_task_t                      uip_tcp_appstate_t;
typedef uip_task_t                      uip_udp_appstate_t;

typedef struct
{
    void (*psock)(sock_s_t *state, void **appdata);                             // TCP�������ص�����
    void *pdata;                                                                // �ص����������ݲ���
    uint16_t port;                                                              // �����˿�
    uint8_t  conn_num;                                                          // ��������״̬����Ŀ
} uip_listen_t;
/**
  * @}
  */

/** @defgroup UIP_Types_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */
// ����UIP�ص�����
extern void uip_tcp_appcall(void);
extern void uip_udp_appcall(void);
#define UIP_APPCALL                     uip_tcp_appcall
#define UIP_UDP_APPCALL                 uip_udp_appcall

/// UIP NULL ����
#define UIP_NULL                        0
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
