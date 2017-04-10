/**
  ******************************************************************************
  * @file    tcp_udp.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.1.3
  * @brief   �ṩ tcp udp Ӧ�ò㺯��.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "tcpip.h"
#include "uip_arp.h"
#include "tapdev.h"



/** @addtogroup Components
  * @{
  */
/** @defgroup TCP_UDP
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup TCP_UDP_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup TCP_UDP_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup TCP_UDP_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup TCP_UDP_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
uip_task_t *app_sock;                                                           // ���ڼ�¼��ǰ��socket������ָ��
static uip_listen_t listenports[UIP_LISTENPORTS];                               // ������ʱ��¼���������Ķ˿ںź͹����Ļص�����
/**
  * @}
  */

/** @defgroup TCP_UDP_Private_Function
  * @brief    ����˽�к���
  * @{
  */
SOCKET(socket_tcp_close);

void uip_tcp_udp_init(void);
void uip_tcp_udp_time(void);
void uip_tcp_appcall(void);
void uip_udp_appcall(void);
/**
  * @}
  */



/** @defgroup TCP_UDP_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  ����һ��TCP������, ���������ж���ʹ��
  * @param  psock,       ��TCP��������SOCKET����
  * @param  pdata,       SOCKET���������ݲ���
  * @param  listen_port, �����˿ں�, ���� uip_htons �� UIP_HTONS ת��
  * @retval ����ֵ,      0-����ʧ��, !0-�����ɹ�
  ******************************************************************************
  */
uip_listen_t *tcp_server_add(void (*psock)(sock_s_t *, void **), void* pdata, uint16_t listen_port)
{
    uint8_t i;
    for (i=0; i < UIP_LISTENPORTS; i++)
    {
        if (listenports[i].port == uip_htons(listen_port))
        {
            if (listenports[i].psock) return 0;
            else listenports[i].port = 0;
        }
    }

    for (i=0; i < UIP_LISTENPORTS; i++)
    {
        if (listenports[i].port == 0)
        {
            listenports[i].psock    = psock;
            listenports[i].pdata    = pdata;
            listenports[i].port     = uip_htons(listen_port);
            listenports[i].conn_num = 0;
            uip_listen(listenports[i].port);
            return &listenports[i];
        }
    }

    return 0;
}

/**
  ******************************************************************************
  * @brief  ɾ��һ��TCP������, ���������ж���ʹ��
  * @param  psock,  ��TCP��������SOCKET����
  * @retval ����ֵ, 0-ɾ��ʧ��, !0-ɾ���ɹ�
  ******************************************************************************
  */
uip_listen_t *tcp_server_delete(void (*psock)(sock_s_t *, void **))
{
    uint8_t i;

    for (i=0; i < UIP_CONNS; i++)
    {
        if (uip_conns[i].appstate.psock == psock)
        {
            uip_conns[i].appstate.psock = socket_tcp_close;
            uip_conns[i].appstate.sock  = SOCK_NULL;
        }
    }

    for (i=0; i < UIP_LISTENPORTS; i++)
    {
        if (listenports[i].psock == psock)
        {
            uip_unlisten(listenports[i].port);
            listenports[i].psock    = UIP_NULL;
            listenports[i].port     = 0;
            listenports[i].conn_num = 0;
            return &listenports[i];
        }
    }

    return 0;
}

/**
  ******************************************************************************
  * @brief  ��ȡ�ص�������TCP��������Ϣ, ���������ж���ʹ��
  * @param  psock,  ��TCP��������SOCKET����
  * @retval ����ֵ, 0-��ȡʧ��, !0-��ȡ�ɹ�
  ******************************************************************************
  */
uip_listen_t *tcp_server_get(void (*psock)(sock_s_t *, void **))
{
    uint8_t i;

    for (i=0; i < UIP_LISTENPORTS; i++)
    {
        if (listenports[i].psock == psock)
        {
            return &listenports[i];
        }
    }

    return 0;
}

/**
  ******************************************************************************
  * @brief  ����һ��TCP�ͻ���, ���������ж���ʹ��
  * @param  psock,       ��TCP�ͻ��˵�SOCKET����
  * @param  pdata,       SOCKET���������ݲ���
  * @param  remote_ip,   �Է�IP��ַ
  * @param  remote_port, �Է��˿ں�, ���� uip_htons �� UIP_HTONS ת��
  * @retval ����ֵ,      0-����ʧ��, !0-�����ɹ���������ӵ�ַ
  ******************************************************************************
  */
uip_conn_t *tcp_client_add(void (*psock)(sock_s_t *, void **), void* pdata, uip_ipaddr_t *remote_ip, uint16_t remote_port)
{
    uip_conn_t *conn;

    conn = uip_connect(remote_ip, uip_htons(remote_port));

    if (conn)
    {
        conn->appstate.conn    = conn;
        conn->appstate.psock   = psock;
        conn->appstate.appdata = pdata;
        conn->appstate.time    = 0;
        conn->appstate.sockptr = UIP_NULL;
        conn->appstate.socklen = 0;
        conn->appstate.type    = TCP_CLIENT;
        conn->appstate.sock    = SOCK_NULL;
        conn->appstate.state   = 0;

        uip_poll_conn(conn);
        if(uip_len > 0)
        {
            uip_arp_out();
            tapdev_send();
        }
    }

    return conn;
}

/**
  ******************************************************************************
  * @brief  ɾ��ָ��TCP�ͻ���, ���������ж���ʹ��
  * @param  psock,  ��TCP�ͻ��˵�SOCKET����
  * @retval ����ֵ, 0-ɾ��ʧ��, !0-ɾ���ɹ������ԭ���ӵ�ַ
  ******************************************************************************
  */
uip_conn_t *tcp_client_delete(void (*psock)(sock_s_t *, void **))
{
    uint8_t i;

    for (i=0; i < UIP_CONNS; i++)
    {
        if (uip_conns[i].appstate.psock == psock)
        {
            uip_conns[i].appstate.psock = socket_tcp_close;
            uip_conns[i].appstate.sock  = SOCK_NULL;

            return &uip_conns[i];
        }
    }

    return 0;
}

/**
  ******************************************************************************
  * @brief  ��ȡ�ص�������TCP�ͻ�������, ���������ж���ʹ��
  * @param  psock,  ��TCP�ͻ��˵�SOCKET����
  * @retval ����ֵ, 0-��ȡʧ��, !0-������ӵ�ַ
  ******************************************************************************
  */
uip_conn_t *tcp_client_get(void (*psock)(sock_s_t *, void **))
{
    uint8_t i;

    for (i=0; i < UIP_CONNS; i++)
    {
        if (uip_conns[i].appstate.psock == psock)
        {
            return &uip_conns[i];
        }
    }

    return 0;
}

/**
  ******************************************************************************
  * @brief  �ر�TCP���ӵĻص�����
  * @param  none
  * @retval none
  ******************************************************************************
  */
SOCKET(socket_tcp_close)
{
    if (app_sock == UIP_NULL) return;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    default:
        app_sock->time = 500;                                                   // 500ms�ڵȴ�Ӧ���ź�
        app_sock->sock = SOCK_NULL;
    case SOCK_CLOSING:
        if ((app_sock->sock == SOCK_NULL) || (app_sock->time == 0))
        {
            uip_close();
            app_sock->sock = SOCK_CLOSING;
        }
        if (uip_closed() || uip_aborted() || uip_timedout() || (app_sock->time == 0))
        {
            app_sock->sock = SOCK_CLOSED;
        }
        break;
    case SOCK_CLOSED:
        break;
    }
}

/**
  ******************************************************************************
  * @brief  ����һ��UDP����, ���������ж���ʹ��
  * @param  psock,       ��UDP���ӵ�SOCKET����
  * @param  pdata,       SOCKET���������ݲ���
  * @param  remote_ip,   �Է�IP��ַ, 0��ʾ��ָ��, �ɽ����κ�IP��ַ���͵���Ϣ
  * @param  remote_port, �Է��˿ں�, 0��ʾ��ָ��, �ɽ����κζ˿ڷ��͵���Ϣ, ���� uip_htons �� UIP_HTONS ת��
  * @param  local_port,  ���ض˿�,   0��ʾ��ָ��, ��ϵͳ���䱾�ض˿�, ���� uip_htons �� UIP_HTONS ת��
  * @retval ����ֵ,      0-����ʧ��, !0-�����ɹ���������ӵ�ַ
  ******************************************************************************
  */
uip_udp_conn_t *udp_add(void (*psock)(sock_s_t *, void **), void* pdata, uip_ipaddr_t *remote_ip, uint16_t remote_port, uint16_t local_port)
{
    uip_udp_conn_t *conn = UIP_NULL;

#if UIP_UDP
    conn = uip_udp_new(remote_ip, uip_htons(remote_port));

    if (conn)
    {
        conn->appstate.conn    = conn;
        conn->appstate.psock   = psock;
        conn->appstate.appdata = pdata;
        conn->appstate.time    = 0;
        conn->appstate.sockptr = UIP_NULL;
        conn->appstate.socklen = 0;
        conn->appstate.sock    = SOCK_NULL;
        conn->appstate.state   = 0;

        if (   (uip_ipaddr_cmp(&conn->ripaddr, &uip_all_zeroes_addr))           // IP��ַΪ0 ��
            || (remote_ip == 0) || (remote_port == 0) )                         // IPָ��Ϊ0 �� �˿ں�Ϊ0
        {
            if (local_port)                                                     // ָ���˷���˿�
            {
                conn->appstate.type = UDP_SERVER;                               // �ɺͶ�������˿�ͨѶ, ΪUDP������
                uip_udp_bind(conn, uip_htons(local_port));                      // �󶨷���˿�
            }
            else                                                                // δָ������˿�, ������ΪUDP������
            {
                uip_udp_remove(conn);                                           // ȡ������
                conn->appstate.conn  = UIP_NULL;
                conn->appstate.psock = UIP_NULL;
                conn = UIP_NULL;
            }
        }
        else
        {
            conn->appstate.type = UDP_CLIENT;                                   // ֻ�ܺ�ָ����IP��ַ�˿ں�ͨѶ, ΪUDP�ͻ���
            if (local_port) uip_udp_bind(conn, uip_htons(local_port));          // �󶨱��ض˿�
        }
    }
#endif

    return conn;
}

/**
  ******************************************************************************
  * @brief  ɾ��ָ��UDP����, ���������ж���ʹ��
  * @param  psock,  ��UDP��SOCKET����
  * @retval ����ֵ, 0-ɾ��ʧ��, !0-ɾ���ɹ������ԭ���ӵ�ַ
  ******************************************************************************
  */
uip_udp_conn_t *udp_delete(void (*psock)(sock_s_t *, void **))
{
#if UIP_UDP
    uint8_t i;

    for (i=0; i < UIP_UDP_CONNS; i++)
    {
        if (uip_udp_conns[i].appstate.psock == psock)
        {
            uip_udp_remove(&uip_udp_conns[i]);
            uip_udp_conns[i].appstate.conn    = UIP_NULL;
            uip_udp_conns[i].appstate.psock   = UIP_NULL;
            uip_udp_conns[i].appstate.appdata = UIP_NULL;
            uip_udp_conns[i].appstate.time    = 0;
            uip_udp_conns[i].appstate.sockptr = UIP_NULL;
            uip_udp_conns[i].appstate.socklen = 0;
            uip_udp_conns[i].appstate.type    = UIP_TYPE_NULL;
            uip_udp_conns[i].appstate.sock    = SOCK_NULL;
            uip_udp_conns[i].appstate.state   = 0;
            return &uip_udp_conns[i];
        }
    }
#endif

    return 0;
}

/**
  ******************************************************************************
  * @brief  ��ȡ�ص�������UDP����, ���������ж���ʹ��
  * @param  psock,  ��UDP��SOCKET����
  * @retval ����ֵ, 0-��ȡʧ��, !0-������ӵ�ַ
  ******************************************************************************
  */
uip_udp_conn_t *udp_get(void (*psock)(sock_s_t *, void **))
{
#if UIP_UDP
    uint8_t i;

    for (i=0; i < UIP_UDP_CONNS; i++)
    {
        if (uip_udp_conns[i].appstate.psock == psock)
        {
            return &uip_udp_conns[i];
        }
    }
#endif

    return 0;
}

/**
  ******************************************************************************
  * @brief  TCP UDPӦ�ó�ʼ������, �� uipsys ����
  * @param  none
  * @retval none
  ******************************************************************************
  */
void uip_tcp_udp_init(void)
{
    uint8_t i;

    for (i=0; i < UIP_LISTENPORTS; i++)
    {
        listenports[i].psock    = UIP_NULL;
        listenports[i].port     = 0;
        listenports[i].conn_num = 0;
    }

    for (i=0; i < UIP_CONNS; i++)
    {
        uip_conns[i].appstate.conn    = UIP_NULL;
        uip_conns[i].appstate.psock   = UIP_NULL;
        uip_conns[i].appstate.appdata = UIP_NULL;
        uip_conns[i].appstate.time    = 0;
        uip_conns[i].appstate.sockptr = UIP_NULL;
        uip_conns[i].appstate.socklen = 0;
        uip_conns[i].appstate.type    = UIP_TYPE_NULL;
        uip_conns[i].appstate.sock    = SOCK_NULL;
        uip_conns[i].appstate.state   = 0;
    }

#if UIP_UDP
    for (i=0; i < UIP_UDP_CONNS; i++)
    {
        uip_udp_conns[i].appstate.conn    = UIP_NULL;
        uip_udp_conns[i].appstate.psock   = UIP_NULL;
        uip_udp_conns[i].appstate.appdata = UIP_NULL;
        uip_udp_conns[i].appstate.time    = 0;
        uip_udp_conns[i].appstate.sockptr = UIP_NULL;
        uip_udp_conns[i].appstate.socklen = 0;
        uip_udp_conns[i].appstate.type    = UIP_TYPE_NULL;
        uip_udp_conns[i].appstate.sock    = SOCK_NULL;
        uip_udp_conns[i].appstate.state   = 0;
    }
#endif
}
/**
  ******************************************************************************
  * @brief  TCP UDP 100ms ����ʱ����, 100ms����һ��
  * @param  none
  * @retval none
  ******************************************************************************
  */
void uip_tcp_udp_time(void)
{
    uint8_t i;

    for (i=0; i < UIP_CONNS; i++)
    {
        if (uip_conns[i].appstate.time > 0)
        {
            if (uip_conns[i].appstate.time > 100)
            {
                uip_conns[i].appstate.time -= 100;
            }
            else
            {
                uip_conns[i].appstate.time = 0;
            }
        }
    }

#if UIP_UDP
    for (i=0; i < UIP_UDP_CONNS; i++)
    {
        if (uip_udp_conns[i].appstate.time > 0)
        {
            if (uip_udp_conns[i].appstate.time > 100)
            {
                uip_udp_conns[i].appstate.time -= 100;
            }
            else
            {
                uip_udp_conns[i].appstate.time = 0;
            }
        }
    }
#endif
}

/**
  ******************************************************************************
  * @brief  UIP TCP �ص�����, �� uip ����
  * @param  none
  * @retval none
  ******************************************************************************
  */
void uip_tcp_appcall(void)
{
    uip_task_t *app = &uip_conn->appstate;
    uint8_t app_closed;

    // ���������Ķ˿ںű����Ӻ�, �Ը����ӽ�����ȷ��ֵ.
    if (uip_connected())
    {
        uint8_t i;

        for (i=0; i < UIP_LISTENPORTS; i++)
        {
            if (listenports[i].port == uip_conn->lport)
            {
                app->conn    = uip_conn;
                app->psock   = listenports[i].psock;
                app->appdata = listenports[i].pdata;
                app->time    = 0;
                app->sockptr = UIP_NULL;
                app->socklen = 0;
                app->type    = TCP_SERVER;
                app->sock    = SOCK_NULL;
                app->state   = 0;
                ++listenports[i].conn_num;
                break;
            }
        }
    }

    app_closed = uip_closed() | uip_aborted() | uip_timedout();
    // �ص�������Ч, ��, ����ʱ״̬ �� ��ʱʱ�䵽 �� ����״̬�쳣(uip_closed, uip_aborted, uip_timedout)
    if ((app->psock) && ((app->sock != SOCK_DELAY) || (app->time == 0) || app_closed))
    {
        if (app->sock == SOCK_DELAY) app->sock = SOCK_NULL;                     // ����Ϊ��ʼ״̬

        // ���ø����ӵĻص�����
        app_sock = app;

        while(1)
        {
            app->psock(&app->state, &app->appdata);
            if (app_closed) app->sock = SOCK_CLOSED;                            // ���ӱ��ر�, ��������ѭ��
            if (app->sock & SOCK_BREAK_MASK) break;                             // ����Ҫ�ص��ûص�����, ֱ������
        }

        if (app->sock == SOCK_CLOSED)
        {
            uip_conn->tcpstateflags = UIP_CLOSED;                               // ȷ���ر�TCP����

            if (app->type == TCP_SERVER)                                        // TCP����������
            {
                uint8_t i;

                for (i=0; i < UIP_LISTENPORTS; i++)
                {
                    if ((listenports[i].port == uip_conn->lport) && listenports[i].conn_num)
                    {
                        --listenports[i].conn_num;                              // ͳ�Ƶ�ǰ������
                    }
                }
            }

            app->conn    = UIP_NULL;
            app->psock   = UIP_NULL;                                            // ɾ���ص�����
            app->appdata = UIP_NULL;
            app->time    = 0;
            app->sockptr = UIP_NULL;
            app->socklen = 0;
            app->type    = UIP_TYPE_NULL;
            app->sock    = SOCK_NULL;
            app->state   = 0;
        }

        app_sock = UIP_NULL;
    }
}

/**
  ******************************************************************************
  * @brief  UIP UDP �ص�����, �� uip ����
  * @param  none
  * @retval none
  ******************************************************************************
  */
void uip_udp_appcall(void)
{
#if UIP_UDP
    uip_task_t *app = &uip_udp_conn->appstate;

    // �ص�������Ч, ��, ����ʱ״̬, ��, ��ʱʱ�䵽
    if ((app->psock) && ((app->sock != SOCK_DELAY) || (app->time == 0)))
    {
        if (app->sock == SOCK_DELAY) app->sock = SOCK_NULL;                     // ����Ϊ��ʼ״̬

        // ���ø����ӵĻص�����
        app_sock = app;

        while(1)
        {
            app->psock(&app->state, &app->appdata);
            if (app->sock & SOCK_BREAK_MASK) break;                             // ����Ҫ�ص��ûص�����, ֱ������
        }

        if (app->sock == SOCK_CLOSED)                                           // UDP�ͻ��������ر�����
        {
            app->conn    = UIP_NULL;
            app->psock   = UIP_NULL;                                            // ɾ���ص�����
            app->appdata = UIP_NULL;
            app->time    = 0;
            app->sockptr = UIP_NULL;
            app->socklen = 0;
            app->type    = UIP_TYPE_NULL;
            app->sock    = SOCK_NULL;
            app->state   = 0;
        }

        app_sock = UIP_NULL;
    }
#endif
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
