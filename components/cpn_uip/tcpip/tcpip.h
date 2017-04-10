/**
  ******************************************************************************
  * @file    tcpip.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.1.3
  * @brief   uIPϵͳ����. ���ϲ�Ӧ�õ���.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TCPIP_H
#define __TCPIP_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include "uip_types.h"
#include "uip.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Components
  * @{
  */
/** @addtogroup TCPIP
  * @{
  */



/**
  ******************************************************************************
  * @defgroup TCPIP_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup TCPIP_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
typedef enum
{
    UIPSYS_NULL       = 0x00,                                                   // ��ʼֵ, �޴�״̬
    UIPSYS_UNLINK     = 0x01,                                                   // uIP ���������Ѷ�

    UIPSYS_DHCP_BUSY  = 0x11,                                                   // uIP DHCP������
    UIPSYS_DHCP_ERROR = 0x12,                                                   // uIP DHCP����ʧ��

    UIPSYS_DHCP_OK    = 0xF1,                                                   // uIP DHCP���óɹ�
    UIPSYS_STATIC_OK  = 0xF2,                                                   // uIP ��̬IP���óɹ�

    UIPSYS_MASK_OK    = 0xF0,                                                   // uIP ���óɹ�����
} uipsys_t;
/**
  * @}
  */

/** @defgroup TCPIP_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */
/// socket ʱ��ֵ, ��ʾ���õȴ�
#define SOCKET_FOREVER                  (int32_t)-1

/// socket �ص������궨��
#define SOCKET(func)                    void func(sock_s_t *state, void **appdata)

/**
  * @}
  */

/** @defgroup TCPIP_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */
extern uip_ipaddr_t uip_dnsaddr;                                                // uIP DNS��������ַ
/**
  * @}
  */

/** @defgroup TCPIP_Public_Function
  * @brief    ���幫�к���
  * @{
  */
// ***************** uIP ϵͳ�������� ******************************************

void uipsys_init(uip_macaddr_t mac);                                            // uIP ϵͳ��ʼ������
void uipsys_scan_1ms(void);                                                     // uIP ϵͳ1ms��ʱɨ��
uipsys_t uipsys_check(void);                                                    // uIP ϵͳ״̬���



// ***************** uIPϵͳ TCP/UDP ���� (�ⲿ���񴴽�/ɾ������) **************
// TCP����������, ���������ж���ʹ��
uip_listen_t *tcp_server_add(void (*psock)(sock_s_t *, void **), void* pdata, uint16_t listen_port);
uip_listen_t *tcp_server_delete(void (*psock)(sock_s_t *, void **));
uip_listen_t *tcp_server_get(void (*psock)(sock_s_t *, void **));

// TCP�ͻ��˺���, ���������ж���ʹ��
uip_conn_t *tcp_client_add(void (*psock)(sock_s_t *, void **), void* pdata, uip_ipaddr_t *remote_ip, uint16_t remote_port);
uip_conn_t *tcp_client_delete(void (*psock)(sock_s_t *, void **));
uip_conn_t *tcp_client_get(void (*psock)(sock_s_t *, void **));

// UDP��������, ���������ж���ʹ��
uip_udp_conn_t *udp_add(void (*psock)(sock_s_t *, void **), void* pdata, uip_ipaddr_t *remote_ip, uint16_t remote_port, uint16_t local_port);
uip_udp_conn_t *udp_delete(void (*psock)(sock_s_t *, void **));
uip_udp_conn_t *udp_get(void (*psock)(sock_s_t *, void **));

// UDP��չ����, ����UDP��������ʵ��, ���������ж���ʹ��
#define udp_server_add(psock, pdata, lport)         udp_add(psock, pdata, 0, 0, lport)                                  // ���UDP������
#define udp_server_delete(psock)                    udp_delete(psock)                                                   // ɾ��UDP������
#define udp_server_get(psock)                       udp_get(psock)                                                      // ��ȡUDP������
#define udp_client_add(psock, pdata, rip, rport)    udp_add(psock, pdata, rip, rport, 0)                                // ���UDP�ͻ���
#define udp_client_delete(psock)                    udp_delete(psock)                                                   // ɾ��UDP�ͻ���
#define udp_client_get(psock)                       udp_get(psock)                                                      // ��ȡUDP�ͻ���
#define udp_broadcast_add(psock, pdata, rport)      udp_add(psock, pdata, (uip_ipaddr_t *)&uip_broadcast_addr, rport, 0)// ���UDP�㲥
#define udp_broadcast_delete(psock)                 udp_delete(psock)                                                   // ɾ��UDP�㲥
#define udp_broadcast_get(psock)                    udp_get(psock)                                                      // ��ȡUDP�㲥



// ***************** uIPϵͳ SOCKET ���� (socket�ص�������) ********************
socket_t socket_continue(void);                                                 // socket�����ص���
socket_t socket_break(void);                                                    // socket�����˳���ѯ
socket_t socket_delay(int32_t time_ms);                                         // socket��ʱ
socket_t socket_close(void);                                                    // socket�ر�����
socket_t socket_closed(void);                                                   // socket�����Ƿ��ѹر�
uip_sock_t socket_status_get(void);                                             // socket��ȡ��ǰ״̬

socket_t socket_tx(uint8_t *buf, uint16_t tx_len, int32_t time_ms);             // socket��������, ��ɺ�socket_break()
socket_t socket_rx(uint8_t *buf, uint16_t max_len, int32_t time_ms);            // socket��������, ��ɺ�socket_continue()
socket_t socket_rx_ptr(uint8_t **ptr, uint16_t *len, int32_t time_ms);          // socket��������, �����ݼ�����������ʼλ�ú����ݳ���
socket_t socket_rx_len(uint8_t *buf, uint16_t rx_len, int32_t time_ms);         // socket����ָ����������
socket_t socket_rx_chr(uint8_t *buf, uint16_t max_len, uint8_t c, int32_t time_ms); // socket����, ָ����������
socket_t socket_rx_str(uint8_t *buf, uint16_t max_len, char *s,   int32_t time_ms); // socket����, ָ���������ݴ�
uint16_t socket_len(void);                                                      // socket��ȡ���ݳ���(����ʣ������/���յ�����)

/**
  * SOCKET �����궨��
  * ʹ��SOCKET�궨�����, �������ڸú�����ʹ��switch���!!! ��Ҫʹ�ñ���ע��:
  * switch �ڲ���ʹ��SOCKET�궨�����, �� SOCKET_DELAY, SOCKET_WAIT_XXX, SOCKET_SUBFUN_XXX
  * �ŵ����Ķ�˳������, ��������������, ������չ�Ӻ���
  * ȱ�����﷨�������, ��������, ���нϸ��ӵķ�֧���ʱ, ����ʹ��!
  * �ۺ϶���, ������ʹ��SOCKET�궨�����, �궨������������û�ɬ�Ѷ�
  * ���������������ʹ��:
  * 1. ������̼�����
  * 2. ������������������, ����Ϊ�Ӻ�������ʽ
  * �Ӻ�������ֵ���ͱ���Ϊ socket_t, ������������
  * �Ӻ���������ʽΪ socket_t socket_sub_fun(...)
  * �Ӻ�����Ƕ��ʹ��, ��ʹ�ú궨��SOCKET_SUBFUN_XXX
  */
#define LC_SOCK_RESUME(s)               switch(s) { case 0:
#define LC_SOCK_SET(s)                  s = __LINE__; case __LINE__:
#define LC_SOCK_SET_EXECUTE(s, e)       s = __LINE__; e; case __LINE__:
#define LC_SOCK_END(s)                  s = 0; }

/// SOCKET ���ĺ궨�����
#define SOCKET_BEGIN(s)                 {uint16_t *_s = &s; LC_SOCK_RESUME(*_s) // SOCKET_BEGIN() �� SOCKET_END() ���ʹ��
#define SOCKET_END()                    LC_SOCK_END(*_s); return; }             // SOCKET_BEGIN() �� SOCKET_END() ���ʹ��

#define SOCKET_DELAY(time)              \
    do {                                \
        socket_delay(time);             \
        LC_SOCK_SET_EXECUTE(*_s, return); \
    } while(0)                                                                  // SOCKET ��ʱ�궨��

#define SOCKET_WAIT_SUBFUN(sub)         \
    do {                                \
        LC_SOCK_SET(*_s);               \
        if ((sub) == SOCKET_WAIT) return; \
    } while(0)                                                                  // SOCKET �Ӻ����궨��

#define SOCKET_WAIT_UNTIL(cond)         \
    do {                                \
        LC_SOCK_SET(*_s);               \
        if (!(cond)) {                  \
            if (!(socket_status_get() & SOCK_BREAK_MASK)) socket_break(); \
            return; }                   \
    } while(0)                                                                  // SOCKET �����жϺ궨��

#define SOCKET_WAIT_WHILE(cond)         SOCKET_WAIT_UNTIL(!(cond))              // SOCKET �����жϺ궨��

/// SOCKET �Ӻ����ĺ궨�����
#define SOCKET_SUBFUN_BEGIN(s)          {uint16_t *_s = &s; LC_SOCK_RESUME(*_s)
#define SOCKET_SUBFUN_END()             LC_SOCK_END(*_s); return SOCKET_OK; }

#define SOCKET_SUBFUN_DELAY(time)       \
    do {                                \
        socket_delay(time);             \
        LC_SOCK_SET_EXECUTE(*_s, return SOCKET_WAIT); \
    } while(0)                                                                  // SOCKET ��ʱ�궨��

#define SOCKET_SUBFUN_WAIT_SUBFUN(sub)  \
    do {                                \
        LC_SOCK_SET(*_s);               \
        if ((sub) == SOCKET_WAIT) return SOCKET_WAIT; \
    } while(0)                                                                  // SOCKET �Ӻ������ú궨��

#define SOCKET_SUBFUN_WAIT_UNTIL(cond)  \
    do {                                \
        LC_SOCK_SET(*_s);               \
        if (!(cond)) {                  \
            if (!(socket_status_get() & SOCK_BREAK_MASK)) socket_break(); \
            return SOCKET_WAIT; }       \
    } while(0)                                                                  // SOCKET �����жϺ궨��

#define SOCKET_SUBFUN_WAIT_WHILE(cond)  SOCKET_SUBFUN_WAIT_UNTIL(!(cond))       // SOCKET �����жϺ궨��



// ***************** uIPϵͳ lib ���� (Ӧ�ò��ص�������) *********************
#define ip_set(ip, a0, a1, a2, a3)      uip_ipaddr(ip, a0, a1, a2, a3)          // ����IPֵ, �� ip_set(&ip, 192, 168, 1, 1);
#define ip_copy(ipdst, ipsrc)           uip_ipaddr_copy(ipdst, ipsrc)           // ����IPֵ, �� ip_copy(&ipdst, &ipsrc);
#define ip_set_host(ip)                 uip_sethostaddr(ip)                     // ���ñ���IP��ַ
#define ip_set_mask(ip)                 uip_setnetmask(ip)                      // ������������
#define ip_set_gateway(ip)              uip_setdraddr(ip)                       // ����Ĭ������
#define ip_set_dns(ip)                  uip_ipaddr_copy(&uip_dnsaddr, (ip))     // ����DNS������

#define ip_get_host(ip)                 uip_gethostaddr(ip)                     // ��ȡ����IP��ַ
#define ip_get_mask(ip)                 uip_getnetmask(ip)                      // ��ȡ��������
#define ip_get_gateway(ip)              uip_getdraddr(ip)                       // ��ȡĬ������
#define ip_get_dns(ip)                  uip_ipaddr_copy((ip), &uip_dnsaddr)     // ��ȡDNS������

uint8_t ip_str2ip(const char *ipstr, uip_ipaddr_t *ip);                         // ��ip�ַ���ת��Ϊip��ַ
uint8_t ip_ip2str(uip_ipaddr_t *ip, char *ipstr);                               // ��ip��ַת��Ϊip�ַ���
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
