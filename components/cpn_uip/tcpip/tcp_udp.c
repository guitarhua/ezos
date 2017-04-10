/**
  ******************************************************************************
  * @file    tcp_udp.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.1.3
  * @brief   提供 tcp udp 应用层函数.
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
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TCP_UDP_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TCP_UDP_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
uip_task_t *app_sock;                                                           // 用于记录当前的socket参数的指针
static uip_listen_t listenports[UIP_LISTENPORTS];                               // 用于临时记录本机监听的端口号和关联的回调函数
/**
  * @}
  */

/** @defgroup TCP_UDP_Private_Function
  * @brief    定义私有函数
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
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  建立一个TCP服务器, 不允许在中断中使用
  * @param  psock,       该TCP服务器的SOCKET函数
  * @param  pdata,       SOCKET函数的数据参数
  * @param  listen_port, 监听端口号, 无需 uip_htons 或 UIP_HTONS 转换
  * @retval 返回值,      0-创建失败, !0-创建成功
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
  * @brief  删除一个TCP服务器, 不允许在中断中使用
  * @param  psock,  该TCP服务器的SOCKET函数
  * @retval 返回值, 0-删除失败, !0-删除成功
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
  * @brief  获取回调函数的TCP服务器信息, 不允许在中断中使用
  * @param  psock,  该TCP服务器的SOCKET函数
  * @retval 返回值, 0-获取失败, !0-获取成功
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
  * @brief  建立一个TCP客户端, 不允许在中断中使用
  * @param  psock,       该TCP客户端的SOCKET函数
  * @param  pdata,       SOCKET函数的数据参数
  * @param  remote_ip,   对方IP地址
  * @param  remote_port, 对方端口号, 无需 uip_htons 或 UIP_HTONS 转换
  * @retval 返回值,      0-创建失败, !0-创建成功并获得连接地址
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
  * @brief  删除指定TCP客户端, 不允许在中断中使用
  * @param  psock,  该TCP客户端的SOCKET函数
  * @retval 返回值, 0-删除失败, !0-删除成功并获得原连接地址
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
  * @brief  获取回调函数的TCP客户端连接, 不允许在中断中使用
  * @param  psock,  该TCP客户端的SOCKET函数
  * @retval 返回值, 0-获取失败, !0-获得连接地址
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
  * @brief  关闭TCP连接的回调函数
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
        app_sock->time = 500;                                                   // 500ms内等待应答信号
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
  * @brief  建立一个UDP连接, 不允许在中断中使用
  * @param  psock,       该UDP连接的SOCKET函数
  * @param  pdata,       SOCKET函数的数据参数
  * @param  remote_ip,   对方IP地址, 0表示不指定, 可接受任何IP地址发送的信息
  * @param  remote_port, 对方端口号, 0表示不指定, 可接受任何端口发送的信息, 无需 uip_htons 或 UIP_HTONS 转换
  * @param  local_port,  本地端口,   0表示不指定, 由系统分配本地端口, 无需 uip_htons 或 UIP_HTONS 转换
  * @retval 返回值,      0-创建失败, !0-创建成功并获得连接地址
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

        if (   (uip_ipaddr_cmp(&conn->ripaddr, &uip_all_zeroes_addr))           // IP地址为0 或
            || (remote_ip == 0) || (remote_port == 0) )                         // IP指针为0 或 端口号为0
        {
            if (local_port)                                                     // 指定了服务端口
            {
                conn->appstate.type = UDP_SERVER;                               // 可和多个机器端口通讯, 为UDP服务器
                uip_udp_bind(conn, uip_htons(local_port));                      // 绑定服务端口
            }
            else                                                                // 未指定服务端口, 不能作为UDP服务器
            {
                uip_udp_remove(conn);                                           // 取消连接
                conn->appstate.conn  = UIP_NULL;
                conn->appstate.psock = UIP_NULL;
                conn = UIP_NULL;
            }
        }
        else
        {
            conn->appstate.type = UDP_CLIENT;                                   // 只能和指定的IP地址端口号通讯, 为UDP客户机
            if (local_port) uip_udp_bind(conn, uip_htons(local_port));          // 绑定本地端口
        }
    }
#endif

    return conn;
}

/**
  ******************************************************************************
  * @brief  删除指定UDP连接, 不允许在中断中使用
  * @param  psock,  该UDP的SOCKET函数
  * @retval 返回值, 0-删除失败, !0-删除成功并获得原连接地址
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
  * @brief  获取回调函数的UDP连接, 不允许在中断中使用
  * @param  psock,  该UDP的SOCKET函数
  * @retval 返回值, 0-获取失败, !0-获得连接地址
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
  * @brief  TCP UDP应用初始化函数, 供 uipsys 调用
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
  * @brief  TCP UDP 100ms 任务定时函数, 100ms调用一次
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
  * @brief  UIP TCP 回调函数, 供 uip 调用
  * @param  none
  * @retval none
  ******************************************************************************
  */
void uip_tcp_appcall(void)
{
    uip_task_t *app = &uip_conn->appstate;
    uint8_t app_closed;

    // 本机监听的端口号被连接后, 对该连接进行正确赋值.
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
    // 回调函数有效, 且, 非延时状态 或 延时时间到 或 连接状态异常(uip_closed, uip_aborted, uip_timedout)
    if ((app->psock) && ((app->sock != SOCK_DELAY) || (app->time == 0) || app_closed))
    {
        if (app->sock == SOCK_DELAY) app->sock = SOCK_NULL;                     // 重设为初始状态

        // 调用该链接的回调函数
        app_sock = app;

        while(1)
        {
            app->psock(&app->state, &app->appdata);
            if (app_closed) app->sock = SOCK_CLOSED;                            // 连接被关闭, 必须跳出循环
            if (app->sock & SOCK_BREAK_MASK) break;                             // 不需要重调用回调函数, 直接跳出
        }

        if (app->sock == SOCK_CLOSED)
        {
            uip_conn->tcpstateflags = UIP_CLOSED;                               // 确保关闭TCP链接

            if (app->type == TCP_SERVER)                                        // TCP服务器类型
            {
                uint8_t i;

                for (i=0; i < UIP_LISTENPORTS; i++)
                {
                    if ((listenports[i].port == uip_conn->lport) && listenports[i].conn_num)
                    {
                        --listenports[i].conn_num;                              // 统计当前链接数
                    }
                }
            }

            app->conn    = UIP_NULL;
            app->psock   = UIP_NULL;                                            // 删除回调函数
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
  * @brief  UIP UDP 回调函数, 供 uip 调用
  * @param  none
  * @retval none
  ******************************************************************************
  */
void uip_udp_appcall(void)
{
#if UIP_UDP
    uip_task_t *app = &uip_udp_conn->appstate;

    // 回调函数有效, 且, 非延时状态, 或, 延时时间到
    if ((app->psock) && ((app->sock != SOCK_DELAY) || (app->time == 0)))
    {
        if (app->sock == SOCK_DELAY) app->sock = SOCK_NULL;                     // 重设为初始状态

        // 调用该链接的回调函数
        app_sock = app;

        while(1)
        {
            app->psock(&app->state, &app->appdata);
            if (app->sock & SOCK_BREAK_MASK) break;                             // 不需要重调用回调函数, 直接跳出
        }

        if (app->sock == SOCK_CLOSED)                                           // UDP客户端主动关闭链接
        {
            app->conn    = UIP_NULL;
            app->psock   = UIP_NULL;                                            // 删除回调函数
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
