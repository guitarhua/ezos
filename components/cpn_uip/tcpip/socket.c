/**
  ******************************************************************************
  * @file    socket.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.3
  * @brief   �ṩ socket ����, ��uipӦ�ò�����ʹ��.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "tcpip.h"
#include "uip_arp.h"
#include <string.h>



/** @addtogroup Components
  * @{
  */
/** @defgroup Socket
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup Socket_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup Socket_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup Socket_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */
#define UDPBUF ((struct uip_udpip_hdr *)&uip_buf[UIP_LLH_LEN])
/**
  * @}
  */

/** @defgroup Socket_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
extern uip_task_t *app_sock;
/**
  * @}
  */

/** @defgroup Socket_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static socket_t socket_tcp_tx(uint8_t *buf, uint16_t tx_len, int32_t time_ms);
extern SOCKET(socket_tcp_close);

#if UIP_UDP
static socket_t socket_udp_tx(uint8_t *buf, uint16_t tx_len, int32_t time_ms);
#endif
/**
  * @}
  */



/** @defgroup Socket_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  socket �ص������������˳���ѯ, �����ص���
  * @param  none
  * @retval ����ֵ
  *   @arg  SOCKET_OK,    ��ʱ���óɹ�
  *   @arg  SOCKET_ERROR, ��������
  ******************************************************************************
  */
socket_t socket_continue(void)
{
    if (app_sock == UIP_NULL) return SOCKET_ERROR;

    app_sock->time = 0;
    app_sock->sock = SOCK_NULL;
    return SOCKET_OK;
}

/**
  ******************************************************************************
  * @brief  socket �ص����������˳���ѯ
  * @param  none
  * @retval ����ֵ
  *   @arg  SOCKET_OK,    ��ʱ���óɹ�
  *   @arg  SOCKET_ERROR, ��������
  ******************************************************************************
  */
socket_t socket_break(void)
{
    if (app_sock == UIP_NULL) return SOCKET_ERROR;

    app_sock->time = 0;
    app_sock->sock = SOCK_DELAY;
    return SOCKET_OK;
}

/**
  ******************************************************************************
  * @brief  socket �ص�������ʱ
  * @param  time_ms, ��ʱʱ��, ��ЧֵΪ500ms�ı���
  * @retval ����ֵ
  *   @arg  SOCKET_OK,    ��ʱ���óɹ�
  *   @arg  SOCKET_ERROR, ��������
  ******************************************************************************
  */
socket_t socket_delay(int32_t time_ms)
{
    if (app_sock == UIP_NULL) return SOCKET_ERROR;

    app_sock->time = time_ms;
    app_sock->sock = SOCK_DELAY;
    return SOCKET_OK;
}

/**
  ******************************************************************************
  * @brief  socket �ر�����
  * @param  none
  * @retval ����ֵ
  *   @arg  SOCKET_OK,    �ر����ӳɹ�
  *   @arg  SOCKET_ERROR, ��������
  ******************************************************************************
  */
socket_t socket_close(void)
{
    if (app_sock == UIP_NULL) return SOCKET_ERROR;

    if ((app_sock->type == TCP_SERVER) || (app_sock->type == TCP_CLIENT))
    {
        // ����ص�����, �����ر�TCP����
        app_sock->psock = socket_tcp_close;
        app_sock->sock  = SOCK_NULL;
        app_sock->state = 0;
        return SOCKET_OK;
    }
#if UIP_UDP
    else if (app_sock->type == UDP_SERVER)
    {
        uip_udp_conn_t *udp_conn = app_sock->conn;

        // ���Զ�̿ͻ��˵�ip��port, �Ա�����ṩUDP����
        udp_conn->rport = 0;
        ip_copy(&udp_conn->ripaddr, &uip_all_zeroes_addr);
        app_sock->time  = 0;
        app_sock->sock  = SOCK_DELAY;
        app_sock->state = 0;
        return SOCKET_OK;
    }
    else if (app_sock->type == UDP_CLIENT)
    {
        uip_udp_conn_t *udp_conn = app_sock->conn;

        // �ر�UDP������
        uip_udp_remove(udp_conn);
        app_sock->sock = SOCK_CLOSED;
        return SOCKET_OK;
    }
#endif

    // ���ʹ���, ��ȷ����ʱ���ᷢ���˴���!
    return SOCKET_ERROR;
}

/**
  ******************************************************************************
  * @brief  socket �������״̬�Ƿ��ѹر�
  * @param  none
  * @retval ����ֵ
  *   @arg  SOCKET_OK,     ��������
  *   @arg  SOCKET_ERROR,  ��������
  *   @arg  SOCKET_CLOSED, �����ѹر�
  ******************************************************************************
  */
socket_t socket_closed(void)
{
    if (app_sock == UIP_NULL) return SOCKET_ERROR;

    if ((app_sock->type == TCP_SERVER) || (app_sock->type == TCP_CLIENT))
    {
        // �ж�TCP���������
        if (uip_closed() || uip_aborted() || uip_timedout())
        {
            app_sock->sock = SOCK_CLOSED;
            return SOCKET_CLOSED;
        }
        else
        {
            return SOCKET_OK;
        }
    }
#if UIP_UDP
    else if (app_sock->type == UDP_SERVER)
    {
        uip_udp_conn_t *udp_conn = app_sock->conn;

        // �ж�UDP���������������
        if ((udp_conn->rport) || uip_ipaddr_cmp(&udp_conn->ripaddr, &uip_all_zeroes_addr)) return SOCKET_OK;
        else                                                                               return SOCKET_CLOSED;
    }
    else if (app_sock->type == UDP_CLIENT)
    {
        // UDP�ͻ���һ��������״̬, ���򲻻���ûص�����. �ж�����Ϊ lport �Ƿ�Ϊ0
        return SOCKET_OK;
    }
#endif

    return SOCKET_ERROR;
}

/**
  ******************************************************************************
  * @brief  socket ��ȡ��ǰ�Ƿ�����ѭ��
  * @param  none
  * @retval ����ֵ, �� uip_sock_t ����
  ******************************************************************************
  */
uip_sock_t socket_status_get(void)
{
    if (app_sock == UIP_NULL) return SOCK_ERROR;
    else                      return app_sock->sock;
}

/**
  ******************************************************************************
  * @brief  socket ͨ��TCP����ָ�����ȵ�����
  * @param  buf,     �������ݵĵ�ַ
  * @param  tx_len,  �������ݵĳ���
  * @param  time_ms, �������ݵĳ�ʱʱ��, ��ЧֵΪ100ms�ı���
  * @retval ����ֵ
  *   @arg  SOCKET_OK,      ���ͳɹ�
  *   @arg  SOCKET_ERROR,   ��������
  *   @arg  SOCKET_TIMEOUT, ���ͳ�ʱ/����ʧ��
  *   @arg  SOCKET_WAIT,    �ȴ�״̬
  ******************************************************************************
  */
static socket_t socket_tcp_tx(uint8_t *buf, uint16_t tx_len, int32_t time_ms)
{
    if (app_sock == UIP_NULL) return SOCKET_ERROR;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:
    default:                                                                    // �Ƿ�״̬, �ϲ㺯�����÷����д���
        app_sock->sockptr = buf;
        app_sock->socklen = tx_len;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_TX_ING;
    case SOCK_TX_ING:
    case SOCK_TX_ACK:
        if (app_sock->socklen)                                                  // ������δ�������
        {
            if ((app_sock->sock == SOCK_TX_ACK) && uip_acked())                 // �ѷ������ݲ��յ�ȷ��
            {
                if (app_sock->socklen > uip_mss())                              // ��δ�����������
                {
                    app_sock->socklen -= uip_mss();
                    app_sock->sockptr += uip_mss();
                    app_sock->sock = SOCK_TX_ING;                               // ���ŷ���
                }
                else                                                            // ���ͳɹ�
                {
                    app_sock->time = 0;
                    app_sock->sock = SOCK_NULL;
                    return SOCKET_OK;
                }
            }

            if ((app_sock->sock == SOCK_TX_ING) || uip_rexmit())                // ��Ҫ�������� / ��Ҫ�ط�����
            {
                if (app_sock->socklen > uip_mss()) uip_send(app_sock->sockptr, uip_mss());
                else                               uip_send(app_sock->sockptr, app_sock->socklen);
                app_sock->sock = SOCK_TX_ACK;
            }

            // ��ʱ / �����쳣״̬
            if ((app_sock->time == 0) || uip_closed() || uip_aborted() || uip_timedout())
            {
                app_sock->time = 0;
                app_sock->sock = SOCK_NULL;
                return SOCKET_TIMEOUT;                                          // ���ͳ�ʱ / ����ʧ��
            }

            return SOCKET_WAIT;                                                 // ���͵ȴ�
        }
        else                                                                    // ������, �������
        {
            app_sock->time = 0;
            app_sock->sock = SOCK_NULL;
            return SOCKET_OK;
        }
    }
}

#if UIP_UDP
/**
  ******************************************************************************
  * @brief  socket ͨ��UDP����ָ�����ȵ�����
  * @param  buf,     �������ݵĵ�ַ
  * @param  tx_len,  �������ݵĳ���
  * @param  time_ms, �������ݵĳ�ʱʱ��, ��ЧֵΪ100ms�ı���
  * @retval ����ֵ
  *   @arg  SOCKET_OK,      ���ͳɹ�
  *   @arg  SOCKET_ERROR,   ��������
  *   @arg  SOCKET_TIMEOUT, ���ͳ�ʱ
  *   @arg  SOCKET_WAIT,    �ȴ�״̬
  ******************************************************************************
  */
static socket_t socket_udp_tx(uint8_t *buf, uint16_t tx_len, int32_t time_ms)
{
    uip_udp_conn_t *udp_conn = app_sock->conn;
    extern uint16_t uip_slen;

    if (app_sock == UIP_NULL) return SOCKET_ERROR;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:
    default:                                                                    // �Ƿ�״̬, �ϲ㺯�����÷����д���
        app_sock->sockptr = buf;
        app_sock->socklen = tx_len;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_TX_ING;
    case SOCK_TX_ING:
        // ���������൱�ڵ��� uip_udp_send(), �����ݳ��ȿ��Ƹ���ȷ
        uip_slen = app_sock->socklen > (UIP_BUFSIZE - UIP_LLH_LEN - UIP_IPUDPH_LEN)? (UIP_BUFSIZE - UIP_LLH_LEN - UIP_IPUDPH_LEN): app_sock->socklen;
        memcpy(&uip_buf[UIP_LLH_LEN + UIP_IPUDPH_LEN], app_sock->sockptr, uip_slen);

        // ���ip��ַ�Ƿ���arp����, �ɱ�����ΪARP��δ׼���ö�����UDP����©��.
        if (uip_arp_check(&udp_conn->ripaddr))
        {
            app_sock->sockptr += uip_slen;
            app_sock->socklen -= uip_slen;

            if (app_sock->socklen == 0)                                         // ������, �������
            {
                app_sock->time = 0;
                app_sock->sock = SOCK_DELAY;
                return SOCKET_OK;
            }
        }

        if (app_sock->time == 0)
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;                                              // ���ͳ�ʱ / ����ʧ��
        }
        else
        {
            return SOCKET_WAIT;                                                 // ARPδ׼����/����δ������, �����ȴ�
        }
    }
}
#endif

/**
  ******************************************************************************
  * @brief  socket ����ָ�����ȵ�����
  *         ����UDP����, ���ͳɹ���(SOCKET_OK)     �൱���Զ�����socket_break()
  *         �������(SOCKET_ERROR��SOCKET_WAIT����)�൱���Զ�����socket_continue()
  * @param  buf,     �������ݵĵ�ַ
  * @param  tx_len,  �������ݵĳ���
  * @param  time_ms, �������ݵĳ�ʱʱ��, ��ЧֵΪ100ms�ı���
  * @retval ����ֵ
  *   @arg  SOCKET_OK,      ���ͳɹ�
  *   @arg  SOCKET_ERROR,   ��������
  *   @arg  SOCKET_TIMEOUT, ���ͳ�ʱ/����ʧ��
  *   @arg  SOCKET_WAIT,    �ȴ�״̬
  ******************************************************************************
  */
socket_t socket_tx(uint8_t *buf, uint16_t tx_len, int32_t time_ms)
{
    if (app_sock == UIP_NULL) return SOCKET_ERROR;

    if ((app_sock->type == TCP_SERVER) || (app_sock->type == TCP_CLIENT))
    {
        return socket_tcp_tx(buf, tx_len, time_ms);
    }
#if UIP_UDP
    else if ((app_sock->type == UDP_SERVER) || (app_sock->type == UDP_CLIENT))
    {
        return socket_udp_tx(buf, tx_len, time_ms);
    }
#endif

    return SOCKET_ERROR;
}

/**
  ******************************************************************************
  * @brief  socket ��������, �����ݼ�����.
  *         ��ɺ�(SOCKET_ERROR��SOCKET_WAIT����)�൱���Զ�����socket_continue()
  * @param  buf,     �������ݵĵ�ַ. �ɵ���uip_appdata, �ӿ촦���ٶ�
  * @param  max_len, �������ݵĻ�������󳤶�
  * @param  time_ms, �������ݵĳ�ʱʱ��, ��ЧֵΪ100ms�ı���
  * @retval ����ֵ
  *   @arg  SOCKET_OK,      ���ճɹ�
  *   @arg  SOCKET_ERROR,   ��������
  *   @arg  SOCKET_TIMEOUT, ���ճ�ʱ
  *   @arg  SOCKET_WAIT,    �ȴ�״̬
  *   @arg  SOCKET_FULL,    ����������, ���������
  ******************************************************************************
  */
socket_t socket_rx(uint8_t *buf, uint16_t max_len, int32_t time_ms)
{
    if ((app_sock == UIP_NULL) || (buf == UIP_NULL)) return SOCKET_ERROR;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:                                                            // ֮ǰ�Գɹ���������, ���е�Ϊ��Ч����!
    default:                                                                    // �Ƿ�״̬, �ϲ㺯�����÷����д���
        app_sock->sockptr = buf;
        app_sock->socklen = 0;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_RX_ING;
        if (app_sock->sock == SOCK_RX_OK) return SOCKET_WAIT;
    case SOCK_RX_ING:
        if (uip_newdata())                                                      // �յ��µ�����
        {
            uint16_t cpylen;

            // ��������д����ջ�����
            if (app_sock->sockptr != uip_appdata)
            {
                if (uip_datalen() < max_len) cpylen = uip_datalen();
                else                         cpylen = max_len;
                memcpy(app_sock->sockptr, (uint8_t *)uip_appdata, cpylen);
            }

#if UIP_UDP
            // UDP����������, �յ���UDP�㲥���鲥Э��
            if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
            {
                uip_udp_conn_t *udp_conn = app_sock->conn;

                // ���Զ�̿ͻ��˵�ip��port
                ip_copy(&udp_conn->ripaddr, &UDPBUF->srcipaddr);
                udp_conn->rport = UDPBUF->srcport;
            }
#endif

            if (uip_datalen() > max_len)
            {
                app_sock->socklen = max_len;
                app_sock->time = 0;
                app_sock->sock = SOCK_RX_OK;
                return SOCKET_FULL;
            }
            else
            {
                app_sock->socklen = uip_datalen();
                app_sock->time = 0;
                app_sock->sock = SOCK_RX_OK;
                return SOCKET_OK;
            }
        }
        else if (app_sock->time == 0)                                           // ���ճ�ʱ
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;
        }
        return SOCKET_WAIT;                                                     // ���յȴ�
    }
}

/**
  ******************************************************************************
  * @brief  socket ��������, �����ݼ�����������ʼλ�ú����ݳ���.
  *         ��ɺ�(SOCKET_ERROR��SOCKET_WAIT����)�൱���Զ�����socket_continue()
  * @param  ptr,     �������ݻ������ʼ��ַ. ��uip_appdata
  * @param  len,     �������ݵ���Ч����ֵ�Ĵ�����ַ.
  * @param  time_ms, �������ݵĳ�ʱʱ��, ��ЧֵΪ100ms�ı���
  * @retval ����ֵ
  *   @arg  SOCKET_OK,      ���ճɹ�
  *   @arg  SOCKET_ERROR,   ��������
  *   @arg  SOCKET_TIMEOUT, ���ճ�ʱ
  *   @arg  SOCKET_WAIT,    �ȴ�״̬
  ******************************************************************************
  */
socket_t socket_rx_ptr(uint8_t **ptr, uint16_t *len, int32_t time_ms)
{
    if (app_sock == UIP_NULL) return SOCKET_ERROR;
    *ptr = (uint8_t *)uip_appdata;
    *len = 0;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:                                                            // ֮ǰ�Գɹ���������, ���е�Ϊ��Ч����!
    default:                                                                    // �Ƿ�״̬, �ϲ㺯�����÷����д���
        app_sock->sockptr = (uint8_t *)uip_appdata;
        app_sock->socklen = 0;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_RX_ING;
        if (app_sock->sock == SOCK_RX_OK) return SOCKET_WAIT;
    case SOCK_RX_ING:
        if (uip_newdata())                                                      // �յ��µ�����
        {
#if UIP_UDP
            // UDP����������, �յ���UDP�㲥���鲥Э��
            if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
            {
                uip_udp_conn_t *udp_conn = app_sock->conn;

                // ���Զ�̿ͻ��˵�ip��port
                ip_copy(&udp_conn->ripaddr, &UDPBUF->srcipaddr);
                udp_conn->rport = UDPBUF->srcport;
            }
#endif

            *len = uip_datalen();
            app_sock->socklen = uip_datalen();
            app_sock->time = 0;
            app_sock->sock = SOCK_RX_OK;
            return SOCKET_OK;
        }
        else if (app_sock->time == 0)                                           // ���ճ�ʱ
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;
        }
        return SOCKET_WAIT;                                                     // ���յȴ�
    }
}

/**
  ******************************************************************************
  * @brief  socket ����ָ�����ȵ�����
  *         ��ɺ�(SOCKET_ERROR��SOCKET_WAIT����)�൱���Զ�����socket_continue()
  * @param  buf,     �������ݵĵ�ַ
  * @param  rx_len,  �������ݵĳ���
  * @param  time_ms, �������ݵĳ�ʱʱ��, ��ЧֵΪ100ms�ı���
  * @retval ����ֵ
  *   @arg  SOCKET_OK,      ���ճɹ�
  *   @arg  SOCKET_ERROR,   ��������
  *   @arg  SOCKET_TIMEOUT, ���ճ�ʱ
  *   @arg  SOCKET_WAIT,    �ȴ�״̬
  ******************************************************************************
  */
socket_t socket_rx_len(uint8_t *buf, uint16_t rx_len, int32_t time_ms)
{
    if ((app_sock == UIP_NULL) || (buf == UIP_NULL)) return SOCKET_ERROR;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:                                                            // ֮ǰ�Գɹ���������, ���е�Ϊ��Ч����!
    default:                                                                    // �Ƿ�״̬, �ϲ㺯�����÷����д���
        app_sock->sockptr = buf;
        app_sock->socklen = 0;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_RX_ING;
        if (app_sock->sock == SOCK_RX_OK) return SOCKET_WAIT;
    case SOCK_RX_ING:
        if ((app_sock->socklen < rx_len) && uip_newdata())                      // �յ��µ�����
        {
            uint16_t cpylen = rx_len - app_sock->socklen;                       // ��Ҫ���յ�ʣ��������

            if (uip_datalen() < cpylen) cpylen = uip_datalen();
            memcpy(app_sock->sockptr, (uint8_t *)uip_appdata, cpylen);          // ��������д����ջ�����
            app_sock->sockptr += cpylen;
            app_sock->socklen += cpylen;
        }

        if (app_sock->socklen >= rx_len)                                        // �������
        {
#if UIP_UDP
            // UDP����������, �յ���UDP�㲥���鲥Э��
            if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
            {
                uip_udp_conn_t *udp_conn = app_sock->conn;

                // ���Զ�̿ͻ��˵�ip��port
                ip_copy(&udp_conn->ripaddr, &UDPBUF->srcipaddr);
                udp_conn->rport = UDPBUF->srcport;
            }
#endif
            app_sock->time = 0;
            app_sock->sock = SOCK_RX_OK;
            return SOCKET_OK;
        }
        else if (app_sock->time == 0)                                           // ���ճ�ʱ
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;
        }
        return SOCKET_WAIT;                                                     // ���յȴ�
    }
}

/**
  ******************************************************************************
  * @brief  socket ��������, ���յ�ָ�������ݺ����
  *         ��ɺ�(SOCKET_ERROR��SOCKET_WAIT����)�൱���Զ�����socket_continue()
  * @param  buf,     �������ݵĵ�ַ
  * @param  max_len, �������ݵĻ�������󳤶�
  * @param  c,       ָ���Ľ�������
  * @param  time_ms, �������ݵĳ�ʱʱ��, ��ЧֵΪ100ms�ı���
  * @retval ����ֵ
  *   @arg  SOCKET_OK,      ���ճɹ�
  *   @arg  SOCKET_ERROR,   ��������
  *   @arg  SOCKET_TIMEOUT, ���ճ�ʱ
  *   @arg  SOCKET_WAIT,    �ȴ�״̬
  *   @arg  SOCKET_FULL,    ����������, ���������
  ******************************************************************************
  */
socket_t socket_rx_chr(uint8_t *buf, uint16_t max_len, uint8_t c, int32_t time_ms)
{
    if ((app_sock == UIP_NULL) || (buf == UIP_NULL)) return SOCKET_ERROR;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:                                                            // ֮ǰ�Գɹ���������, ���е�Ϊ��Ч����!
    default:                                                                    // �Ƿ�״̬, �ϲ㺯�����÷����д���
        app_sock->sockptr = buf;
        app_sock->socklen = 0;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_RX_ING;
        if (app_sock->sock == SOCK_RX_OK) return SOCKET_WAIT;
    case SOCK_RX_ING:
        if ((app_sock->socklen < max_len) && uip_newdata())                     // �յ��µ�����
        {
            uint16_t cpylen = max_len - app_sock->socklen;                      // ��Ҫ���յ�ʣ��������
            uint8_t *pdata = app_sock->sockptr;
            uint16_t i;

            if (uip_datalen() < cpylen) cpylen = uip_datalen();
            memcpy(app_sock->sockptr, (uint8_t *)uip_appdata, cpylen);          // ��������д����ջ�����

            for (i=1; i <= cpylen; i++)
            {
                if (*pdata++ == c)                                              // �ҳ���������
                {
#if UIP_UDP
                    // UDP����������, �յ���UDP�㲥���鲥Э��
                    if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
                    {
                        uip_udp_conn_t *udp_conn = app_sock->conn;

                        // ���Զ�̿ͻ��˵�ip��port
                        ip_copy(&udp_conn->ripaddr, &UDPBUF->srcipaddr);
                        udp_conn->rport = UDPBUF->srcport;
                    }
#endif
                    app_sock->socklen += i;
                    app_sock->time = 0;
                    app_sock->sock = SOCK_RX_OK;
                    return SOCKET_OK;
                }
            }

            app_sock->sockptr += cpylen;
            app_sock->socklen += cpylen;
        }

        if (app_sock->socklen >= max_len)                                       // �������
        {
#if UIP_UDP
            // UDP����������, �յ���UDP�㲥���鲥Э��
            if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
            {
                uip_udp_conn_t *udp_conn = app_sock->conn;

                // ���Զ�̿ͻ��˵�ip��port
                ip_copy(&udp_conn->ripaddr, &UDPBUF->srcipaddr);
                udp_conn->rport = UDPBUF->srcport;
            }
#endif
            app_sock->time = 0;
            app_sock->sock = SOCK_RX_OK;
            return SOCKET_FULL;
        }
        else if (app_sock->time == 0)                                           // ���ճ�ʱ
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;
        }
        return SOCKET_WAIT;                                                     // ���յȴ�
    }
}

/**
  ******************************************************************************
  * @brief  socket ��������, ���յ�ָ�����ַ��������
  *         ��ɺ�(SOCKET_ERROR��SOCKET_WAIT����)�൱���Զ�����socket_continue()
  * @param  buf,     �������ݵĵ�ַ
  * @param  max_len, �������ݵĻ�������󳤶�
  * @param  s,       ָ���Ľ����ַ���
  * @param  time_ms, �������ݵĳ�ʱʱ��, ��ЧֵΪ100ms�ı���
  * @retval ����ֵ
  *   @arg  SOCKET_OK,      ���ճɹ�
  *   @arg  SOCKET_ERROR,   ��������
  *   @arg  SOCKET_TIMEOUT, ���ճ�ʱ
  *   @arg  SOCKET_WAIT,    �ȴ�״̬
  *   @arg  SOCKET_FULL,    ����������, ���������
  ******************************************************************************
  */
socket_t socket_rx_str(uint8_t *buf, uint16_t max_len, char *s, int32_t time_ms)
{
    if ((app_sock == UIP_NULL) || (buf == UIP_NULL)) return SOCKET_ERROR;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:                                                            // ֮ǰ�Գɹ���������, ���е�Ϊ��Ч����!
    default:                                                                    // �Ƿ�״̬, �ϲ㺯�����÷����д���
        app_sock->sockptr = buf;
        app_sock->socklen = 0;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_RX_ING;
        if (app_sock->sock == SOCK_RX_OK) return SOCKET_WAIT;
    case SOCK_RX_ING:
        if ((app_sock->socklen < max_len) && uip_newdata())                     // �յ��µ�����
        {
            uint16_t cpylen = max_len - app_sock->socklen;                      // ��Ҫ���յ�ʣ��������
            uint8_t  cmplen = strlen(s);                                        // ��Ҫ���бȽϵ����ݳ���

            if (uip_datalen() < cpylen) cpylen = uip_datalen();
            memcpy(app_sock->sockptr, (uint8_t *)uip_appdata, cpylen);          // ��������д����ջ�����

            if ((app_sock->socklen + cpylen) >= cmplen)
            {
                uint8_t *pmem = app_sock->sockptr - cmplen + 1;                 // �˴���Ҫ�Ƚϵ�������ʼָ��
                uint8_t i;

                if (pmem < buf) pmem = buf;
                for (i=1; i<=cpylen; i++)
                {
                    if (memcmp(pmem++, s, cmplen) == 0)                         // �ҳ������ַ���
                    {
#if UIP_UDP
                        // UDP����������, �յ���UDP�㲥���鲥Э��
                        if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
                        {
                            uip_udp_conn_t *udp_conn = app_sock->conn;

                            // ���Զ�̿ͻ��˵�ip��port
                            ip_copy(&udp_conn->ripaddr, &UDPBUF->srcipaddr);
                            udp_conn->rport = UDPBUF->srcport;
                        }
#endif
                        app_sock->socklen += (i + cmplen);
                        app_sock->time = 0;
                        app_sock->sock = SOCK_RX_OK;
                        return SOCKET_OK;
                    }
                }
            }

            app_sock->sockptr += cpylen;
            app_sock->socklen += cpylen;
        }

        if (app_sock->socklen >= max_len)                                       // �������
        {
#if UIP_UDP
            // UDP����������, �յ���UDP�㲥���鲥Э��
            if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
            {
                uip_udp_conn_t *udp_conn = app_sock->conn;

                // ���Զ�̿ͻ��˵�ip��port
                ip_copy(&udp_conn->ripaddr, &UDPBUF->srcipaddr);
                udp_conn->rport = UDPBUF->srcport;
            }
#endif
            app_sock->time = 0;
            app_sock->sock = SOCK_RX_OK;
            return SOCKET_FULL;
        }
        else if (app_sock->time == 0)                                           // ���ճ�ʱ
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;
        }
        return SOCKET_WAIT;                                                     // ���յȴ�
    }
}

/**
  ******************************************************************************
  * @brief  socket ��ȡ�������ݵĳ��� �� �������ݵ�ʣ�೤��
  * @param  none
  * @retval ����ֵ, ����socket���ݳ���
  ******************************************************************************
  */
uint16_t socket_len(void)
{
    return app_sock->socklen;
}
/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

/* END OF FILE ---------------------------------------------------------------*/
