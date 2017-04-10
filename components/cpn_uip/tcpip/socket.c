/**
  ******************************************************************************
  * @file    socket.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.3
  * @brief   提供 socket 函数, 供uip应用层任务使用.
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
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup Socket_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
#define UDPBUF ((struct uip_udpip_hdr *)&uip_buf[UIP_LLH_LEN])
/**
  * @}
  */

/** @defgroup Socket_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
extern uip_task_t *app_sock;
/**
  * @}
  */

/** @defgroup Socket_Private_Function
  * @brief    定义私有函数
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
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  socket 回调函数不主动退出轮询, 立刻重调用
  * @param  none
  * @retval 返回值
  *   @arg  SOCKET_OK,    延时设置成功
  *   @arg  SOCKET_ERROR, 参数错误
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
  * @brief  socket 回调函数主动退出轮询
  * @param  none
  * @retval 返回值
  *   @arg  SOCKET_OK,    延时设置成功
  *   @arg  SOCKET_ERROR, 参数错误
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
  * @brief  socket 回调函数延时
  * @param  time_ms, 延时时间, 有效值为500ms的倍数
  * @retval 返回值
  *   @arg  SOCKET_OK,    延时设置成功
  *   @arg  SOCKET_ERROR, 参数错误
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
  * @brief  socket 关闭连接
  * @param  none
  * @retval 返回值
  *   @arg  SOCKET_OK,    关闭连接成功
  *   @arg  SOCKET_ERROR, 参数错误
  ******************************************************************************
  */
socket_t socket_close(void)
{
    if (app_sock == UIP_NULL) return SOCKET_ERROR;

    if ((app_sock->type == TCP_SERVER) || (app_sock->type == TCP_CLIENT))
    {
        // 重设回调函数, 用来关闭TCP连接
        app_sock->psock = socket_tcp_close;
        app_sock->sock  = SOCK_NULL;
        app_sock->state = 0;
        return SOCKET_OK;
    }
#if UIP_UDP
    else if (app_sock->type == UDP_SERVER)
    {
        uip_udp_conn_t *udp_conn = app_sock->conn;

        // 清空远程客户端的ip和port, 以便继续提供UDP服务
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

        // 关闭UDP的连接
        uip_udp_remove(udp_conn);
        app_sock->sock = SOCK_CLOSED;
        return SOCKET_OK;
    }
#endif

    // 类型错误, 正确调用时不会发生此错误!
    return SOCKET_ERROR;
}

/**
  ******************************************************************************
  * @brief  socket 检查连接状态是否已关闭
  * @param  none
  * @retval 返回值
  *   @arg  SOCKET_OK,     连接正常
  *   @arg  SOCKET_ERROR,  参数错误
  *   @arg  SOCKET_CLOSED, 连接已关闭
  ******************************************************************************
  */
socket_t socket_closed(void)
{
    if (app_sock == UIP_NULL) return SOCKET_ERROR;

    if ((app_sock->type == TCP_SERVER) || (app_sock->type == TCP_CLIENT))
    {
        // 判断TCP的连接情况
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

        // 判断UDP服务器的连接情况
        if ((udp_conn->rport) || uip_ipaddr_cmp(&udp_conn->ripaddr, &uip_all_zeroes_addr)) return SOCKET_OK;
        else                                                                               return SOCKET_CLOSED;
    }
    else if (app_sock->type == UDP_CLIENT)
    {
        // UDP客户端一定是连接状态, 否则不会调用回调函数. 判断依据为 lport 是否为0
        return SOCKET_OK;
    }
#endif

    return SOCKET_ERROR;
}

/**
  ******************************************************************************
  * @brief  socket 获取当前是否跳出循环
  * @param  none
  * @retval 返回值, 见 uip_sock_t 定义
  ******************************************************************************
  */
uip_sock_t socket_status_get(void)
{
    if (app_sock == UIP_NULL) return SOCK_ERROR;
    else                      return app_sock->sock;
}

/**
  ******************************************************************************
  * @brief  socket 通过TCP发送指定长度的数据
  * @param  buf,     发送数据的地址
  * @param  tx_len,  发送数据的长度
  * @param  time_ms, 发送数据的超时时间, 有效值为100ms的倍数
  * @retval 返回值
  *   @arg  SOCKET_OK,      发送成功
  *   @arg  SOCKET_ERROR,   参数错误
  *   @arg  SOCKET_TIMEOUT, 发送超时/发送失败
  *   @arg  SOCKET_WAIT,    等待状态
  ******************************************************************************
  */
static socket_t socket_tcp_tx(uint8_t *buf, uint16_t tx_len, int32_t time_ms)
{
    if (app_sock == UIP_NULL) return SOCKET_ERROR;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:
    default:                                                                    // 非法状态, 上层函数调用方法有错误
        app_sock->sockptr = buf;
        app_sock->socklen = tx_len;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_TX_ING;
    case SOCK_TX_ING:
    case SOCK_TX_ACK:
        if (app_sock->socklen)                                                  // 有数据未发送完成
        {
            if ((app_sock->sock == SOCK_TX_ACK) && uip_acked())                 // 已发送数据并收到确认
            {
                if (app_sock->socklen > uip_mss())                              // 有未发送完的数据
                {
                    app_sock->socklen -= uip_mss();
                    app_sock->sockptr += uip_mss();
                    app_sock->sock = SOCK_TX_ING;                               // 接着发送
                }
                else                                                            // 发送成功
                {
                    app_sock->time = 0;
                    app_sock->sock = SOCK_NULL;
                    return SOCKET_OK;
                }
            }

            if ((app_sock->sock == SOCK_TX_ING) || uip_rexmit())                // 需要发送数据 / 需要重发数据
            {
                if (app_sock->socklen > uip_mss()) uip_send(app_sock->sockptr, uip_mss());
                else                               uip_send(app_sock->sockptr, app_sock->socklen);
                app_sock->sock = SOCK_TX_ACK;
            }

            // 超时 / 连接异常状态
            if ((app_sock->time == 0) || uip_closed() || uip_aborted() || uip_timedout())
            {
                app_sock->time = 0;
                app_sock->sock = SOCK_NULL;
                return SOCKET_TIMEOUT;                                          // 发送超时 / 发送失败
            }

            return SOCKET_WAIT;                                                 // 发送等待
        }
        else                                                                    // 无数据, 发送完成
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
  * @brief  socket 通过UDP发送指定长度的数据
  * @param  buf,     发送数据的地址
  * @param  tx_len,  发送数据的长度
  * @param  time_ms, 发送数据的超时时间, 有效值为100ms的倍数
  * @retval 返回值
  *   @arg  SOCKET_OK,      发送成功
  *   @arg  SOCKET_ERROR,   参数错误
  *   @arg  SOCKET_TIMEOUT, 发送超时
  *   @arg  SOCKET_WAIT,    等待状态
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
    default:                                                                    // 非法状态, 上层函数调用方法有错误
        app_sock->sockptr = buf;
        app_sock->socklen = tx_len;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_TX_ING;
    case SOCK_TX_ING:
        // 以下内容相当于调用 uip_udp_send(), 对数据长度控制更精确
        uip_slen = app_sock->socklen > (UIP_BUFSIZE - UIP_LLH_LEN - UIP_IPUDPH_LEN)? (UIP_BUFSIZE - UIP_LLH_LEN - UIP_IPUDPH_LEN): app_sock->socklen;
        memcpy(&uip_buf[UIP_LLH_LEN + UIP_IPUDPH_LEN], app_sock->sockptr, uip_slen);

        // 检查ip地址是否在arp表内, 可避免因为ARP表未准备好而导致UDP数据漏发.
        if (uip_arp_check(&udp_conn->ripaddr))
        {
            app_sock->sockptr += uip_slen;
            app_sock->socklen -= uip_slen;

            if (app_sock->socklen == 0)                                         // 无数据, 发送完成
            {
                app_sock->time = 0;
                app_sock->sock = SOCK_DELAY;
                return SOCKET_OK;
            }
        }

        if (app_sock->time == 0)
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;                                              // 发送超时 / 发送失败
        }
        else
        {
            return SOCKET_WAIT;                                                 // ARP未准备好/数据未发送完, 继续等待
        }
    }
}
#endif

/**
  ******************************************************************************
  * @brief  socket 发送指定长度的数据
  *         对于UDP连接, 发送成功后(SOCKET_OK)     相当于自动调用socket_break()
  *         其它情况(SOCKET_ERROR和SOCKET_WAIT除外)相当于自动调用socket_continue()
  * @param  buf,     发送数据的地址
  * @param  tx_len,  发送数据的长度
  * @param  time_ms, 发送数据的超时时间, 有效值为100ms的倍数
  * @retval 返回值
  *   @arg  SOCKET_OK,      发送成功
  *   @arg  SOCKET_ERROR,   参数错误
  *   @arg  SOCKET_TIMEOUT, 发送超时/发送失败
  *   @arg  SOCKET_WAIT,    等待状态
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
  * @brief  socket 接收数据, 有数据即返回.
  *         完成后(SOCKET_ERROR和SOCKET_WAIT除外)相当于自动调用socket_continue()
  * @param  buf,     接收数据的地址. 可等于uip_appdata, 加快处理速度
  * @param  max_len, 接收数据的缓冲区最大长度
  * @param  time_ms, 接收数据的超时时间, 有效值为100ms的倍数
  * @retval 返回值
  *   @arg  SOCKET_OK,      接收成功
  *   @arg  SOCKET_ERROR,   参数错误
  *   @arg  SOCKET_TIMEOUT, 接收超时
  *   @arg  SOCKET_WAIT,    等待状态
  *   @arg  SOCKET_FULL,    接收数据满, 缓冲区溢出
  ******************************************************************************
  */
socket_t socket_rx(uint8_t *buf, uint16_t max_len, int32_t time_ms)
{
    if ((app_sock == UIP_NULL) || (buf == UIP_NULL)) return SOCKET_ERROR;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:                                                            // 之前以成功接收数据, 现有的为无效数据!
    default:                                                                    // 非法状态, 上层函数调用方法有错误
        app_sock->sockptr = buf;
        app_sock->socklen = 0;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_RX_ING;
        if (app_sock->sock == SOCK_RX_OK) return SOCKET_WAIT;
    case SOCK_RX_ING:
        if (uip_newdata())                                                      // 收到新的数据
        {
            uint16_t cpylen;

            // 将新数据写入接收缓冲区
            if (app_sock->sockptr != uip_appdata)
            {
                if (uip_datalen() < max_len) cpylen = uip_datalen();
                else                         cpylen = max_len;
                memcpy(app_sock->sockptr, (uint8_t *)uip_appdata, cpylen);
            }

#if UIP_UDP
            // UDP服务器类型, 收到非UDP广播或组播协议
            if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
            {
                uip_udp_conn_t *udp_conn = app_sock->conn;

                // 需绑定远程客户端的ip和port
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
        else if (app_sock->time == 0)                                           // 接收超时
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;
        }
        return SOCKET_WAIT;                                                     // 接收等待
    }
}

/**
  ******************************************************************************
  * @brief  socket 接收数据, 有数据即返回数据起始位置和数据长度.
  *         完成后(SOCKET_ERROR和SOCKET_WAIT除外)相当于自动调用socket_continue()
  * @param  ptr,     接收数据缓存的起始地址. 即uip_appdata
  * @param  len,     接收数据的有效长度值寄存器地址.
  * @param  time_ms, 接收数据的超时时间, 有效值为100ms的倍数
  * @retval 返回值
  *   @arg  SOCKET_OK,      接收成功
  *   @arg  SOCKET_ERROR,   参数错误
  *   @arg  SOCKET_TIMEOUT, 接收超时
  *   @arg  SOCKET_WAIT,    等待状态
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
    case SOCK_RX_OK:                                                            // 之前以成功接收数据, 现有的为无效数据!
    default:                                                                    // 非法状态, 上层函数调用方法有错误
        app_sock->sockptr = (uint8_t *)uip_appdata;
        app_sock->socklen = 0;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_RX_ING;
        if (app_sock->sock == SOCK_RX_OK) return SOCKET_WAIT;
    case SOCK_RX_ING:
        if (uip_newdata())                                                      // 收到新的数据
        {
#if UIP_UDP
            // UDP服务器类型, 收到非UDP广播或组播协议
            if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
            {
                uip_udp_conn_t *udp_conn = app_sock->conn;

                // 需绑定远程客户端的ip和port
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
        else if (app_sock->time == 0)                                           // 接收超时
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;
        }
        return SOCKET_WAIT;                                                     // 接收等待
    }
}

/**
  ******************************************************************************
  * @brief  socket 接收指定长度的数据
  *         完成后(SOCKET_ERROR和SOCKET_WAIT除外)相当于自动调用socket_continue()
  * @param  buf,     接收数据的地址
  * @param  rx_len,  接收数据的长度
  * @param  time_ms, 接收数据的超时时间, 有效值为100ms的倍数
  * @retval 返回值
  *   @arg  SOCKET_OK,      接收成功
  *   @arg  SOCKET_ERROR,   参数错误
  *   @arg  SOCKET_TIMEOUT, 接收超时
  *   @arg  SOCKET_WAIT,    等待状态
  ******************************************************************************
  */
socket_t socket_rx_len(uint8_t *buf, uint16_t rx_len, int32_t time_ms)
{
    if ((app_sock == UIP_NULL) || (buf == UIP_NULL)) return SOCKET_ERROR;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:                                                            // 之前以成功接收数据, 现有的为无效数据!
    default:                                                                    // 非法状态, 上层函数调用方法有错误
        app_sock->sockptr = buf;
        app_sock->socklen = 0;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_RX_ING;
        if (app_sock->sock == SOCK_RX_OK) return SOCKET_WAIT;
    case SOCK_RX_ING:
        if ((app_sock->socklen < rx_len) && uip_newdata())                      // 收到新的数据
        {
            uint16_t cpylen = rx_len - app_sock->socklen;                       // 需要接收的剩余数据数

            if (uip_datalen() < cpylen) cpylen = uip_datalen();
            memcpy(app_sock->sockptr, (uint8_t *)uip_appdata, cpylen);          // 将新数据写入接收缓冲区
            app_sock->sockptr += cpylen;
            app_sock->socklen += cpylen;
        }

        if (app_sock->socklen >= rx_len)                                        // 接收完成
        {
#if UIP_UDP
            // UDP服务器类型, 收到非UDP广播或组播协议
            if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
            {
                uip_udp_conn_t *udp_conn = app_sock->conn;

                // 需绑定远程客户端的ip和port
                ip_copy(&udp_conn->ripaddr, &UDPBUF->srcipaddr);
                udp_conn->rport = UDPBUF->srcport;
            }
#endif
            app_sock->time = 0;
            app_sock->sock = SOCK_RX_OK;
            return SOCKET_OK;
        }
        else if (app_sock->time == 0)                                           // 接收超时
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;
        }
        return SOCKET_WAIT;                                                     // 接收等待
    }
}

/**
  ******************************************************************************
  * @brief  socket 接收数据, 接收到指定的数据后结束
  *         完成后(SOCKET_ERROR和SOCKET_WAIT除外)相当于自动调用socket_continue()
  * @param  buf,     接收数据的地址
  * @param  max_len, 接收数据的缓冲区最大长度
  * @param  c,       指定的结束数据
  * @param  time_ms, 接收数据的超时时间, 有效值为100ms的倍数
  * @retval 返回值
  *   @arg  SOCKET_OK,      接收成功
  *   @arg  SOCKET_ERROR,   参数错误
  *   @arg  SOCKET_TIMEOUT, 接收超时
  *   @arg  SOCKET_WAIT,    等待状态
  *   @arg  SOCKET_FULL,    接收数据满, 缓冲区溢出
  ******************************************************************************
  */
socket_t socket_rx_chr(uint8_t *buf, uint16_t max_len, uint8_t c, int32_t time_ms)
{
    if ((app_sock == UIP_NULL) || (buf == UIP_NULL)) return SOCKET_ERROR;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:                                                            // 之前以成功接收数据, 现有的为无效数据!
    default:                                                                    // 非法状态, 上层函数调用方法有错误
        app_sock->sockptr = buf;
        app_sock->socklen = 0;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_RX_ING;
        if (app_sock->sock == SOCK_RX_OK) return SOCKET_WAIT;
    case SOCK_RX_ING:
        if ((app_sock->socklen < max_len) && uip_newdata())                     // 收到新的数据
        {
            uint16_t cpylen = max_len - app_sock->socklen;                      // 需要接收的剩余数据数
            uint8_t *pdata = app_sock->sockptr;
            uint16_t i;

            if (uip_datalen() < cpylen) cpylen = uip_datalen();
            memcpy(app_sock->sockptr, (uint8_t *)uip_appdata, cpylen);          // 将新数据写入接收缓冲区

            for (i=1; i <= cpylen; i++)
            {
                if (*pdata++ == c)                                              // 找出结束数据
                {
#if UIP_UDP
                    // UDP服务器类型, 收到非UDP广播或组播协议
                    if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
                    {
                        uip_udp_conn_t *udp_conn = app_sock->conn;

                        // 需绑定远程客户端的ip和port
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

        if (app_sock->socklen >= max_len)                                       // 接收完成
        {
#if UIP_UDP
            // UDP服务器类型, 收到非UDP广播或组播协议
            if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
            {
                uip_udp_conn_t *udp_conn = app_sock->conn;

                // 需绑定远程客户端的ip和port
                ip_copy(&udp_conn->ripaddr, &UDPBUF->srcipaddr);
                udp_conn->rport = UDPBUF->srcport;
            }
#endif
            app_sock->time = 0;
            app_sock->sock = SOCK_RX_OK;
            return SOCKET_FULL;
        }
        else if (app_sock->time == 0)                                           // 接收超时
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;
        }
        return SOCKET_WAIT;                                                     // 接收等待
    }
}

/**
  ******************************************************************************
  * @brief  socket 接收数据, 接收到指定的字符串后结束
  *         完成后(SOCKET_ERROR和SOCKET_WAIT除外)相当于自动调用socket_continue()
  * @param  buf,     接收数据的地址
  * @param  max_len, 接收数据的缓冲区最大长度
  * @param  s,       指定的结束字符串
  * @param  time_ms, 接收数据的超时时间, 有效值为100ms的倍数
  * @retval 返回值
  *   @arg  SOCKET_OK,      接收成功
  *   @arg  SOCKET_ERROR,   参数错误
  *   @arg  SOCKET_TIMEOUT, 接收超时
  *   @arg  SOCKET_WAIT,    等待状态
  *   @arg  SOCKET_FULL,    接收数据满, 缓冲区溢出
  ******************************************************************************
  */
socket_t socket_rx_str(uint8_t *buf, uint16_t max_len, char *s, int32_t time_ms)
{
    if ((app_sock == UIP_NULL) || (buf == UIP_NULL)) return SOCKET_ERROR;

    switch (app_sock->sock)
    {
    case SOCK_NULL:
    case SOCK_RX_OK:                                                            // 之前以成功接收数据, 现有的为无效数据!
    default:                                                                    // 非法状态, 上层函数调用方法有错误
        app_sock->sockptr = buf;
        app_sock->socklen = 0;
        app_sock->time = time_ms;
        app_sock->sock = SOCK_RX_ING;
        if (app_sock->sock == SOCK_RX_OK) return SOCKET_WAIT;
    case SOCK_RX_ING:
        if ((app_sock->socklen < max_len) && uip_newdata())                     // 收到新的数据
        {
            uint16_t cpylen = max_len - app_sock->socklen;                      // 需要接收的剩余数据数
            uint8_t  cmplen = strlen(s);                                        // 需要进行比较的数据长度

            if (uip_datalen() < cpylen) cpylen = uip_datalen();
            memcpy(app_sock->sockptr, (uint8_t *)uip_appdata, cpylen);          // 将新数据写入接收缓冲区

            if ((app_sock->socklen + cpylen) >= cmplen)
            {
                uint8_t *pmem = app_sock->sockptr - cmplen + 1;                 // 此次需要比较的数据起始指针
                uint8_t i;

                if (pmem < buf) pmem = buf;
                for (i=1; i<=cpylen; i++)
                {
                    if (memcmp(pmem++, s, cmplen) == 0)                         // 找出结束字符串
                    {
#if UIP_UDP
                        // UDP服务器类型, 收到非UDP广播或组播协议
                        if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
                        {
                            uip_udp_conn_t *udp_conn = app_sock->conn;

                            // 需绑定远程客户端的ip和port
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

        if (app_sock->socklen >= max_len)                                       // 接收完成
        {
#if UIP_UDP
            // UDP服务器类型, 收到非UDP广播或组播协议
            if ((app_sock->type == UDP_SERVER) && ((UDPBUF->destipaddr.u8[0] & 224) < 224))
            {
                uip_udp_conn_t *udp_conn = app_sock->conn;

                // 需绑定远程客户端的ip和port
                ip_copy(&udp_conn->ripaddr, &UDPBUF->srcipaddr);
                udp_conn->rport = UDPBUF->srcport;
            }
#endif
            app_sock->time = 0;
            app_sock->sock = SOCK_RX_OK;
            return SOCKET_FULL;
        }
        else if (app_sock->time == 0)                                           // 接收超时
        {
            app_sock->sock = SOCK_NULL;
            return SOCKET_TIMEOUT;
        }
        return SOCKET_WAIT;                                                     // 接收等待
    }
}

/**
  ******************************************************************************
  * @brief  socket 获取接收数据的长度 或 发送数据的剩余长度
  * @param  none
  * @retval 返回值, 返回socket数据长度
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
