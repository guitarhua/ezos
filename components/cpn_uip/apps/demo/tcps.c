/**
  ******************************************************************************
  * @file    tcps.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.8
  * @brief   提供 tcp 服务器示例.
  *          服务器的应用比客户端复杂, 服务器需要为各客户端独立划分出数据空间.
  *          此范例是使用 memb.c 文件实现的. 并将分配获得的空间通过 *appdata传递.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "tcps.h"
#include "memb.h"
#include <string.h>



/** @addtogroup Components
  * @{
  */
/** @defgroup TCPServer
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup TCPServer_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup TCPServer_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */
typedef struct{
    uint8_t state;
    char buf[30];
} tcps_app_t;

/**
  * @}
  */

/** @defgroup TCPServer_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPServer_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
// 设置memb的数据结构, 名称, 和可用数量.
// 可理解为在内存开辟了如下一块区域供分配使用,
// tcps_app_t tcps_app[UIP_CONF_MAX_CONNECTIONS];
MEMB(tcps_app_t, tcps_app, UIP_CONF_MAX_CONNECTIONS);
/**
  * @}
  */

/** @defgroup TCPServer_Private_Function
  * @brief    定义私有函数
  * @{
  */
static SOCKET(socket_tcp_server);
/**
  * @}
  */



/** @defgroup TCPServer_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  tcp服务器
  * @param  port, 服务器的端口号
  * @retval none
  ******************************************************************************
  */
void example_tcp_server(uint16_t port)
{
    memb_init(&tcps_app);
    tcp_server_delete(socket_tcp_server);
    tcp_server_add(socket_tcp_server, UIP_NULL, port);
}

/**
  ******************************************************************************
  * @brief  tcp服务器的socket回调处理函数
  * @param  socket回调函数内置参数
  * @retval none
  ******************************************************************************
  */
SOCKET(socket_tcp_server)
{
    if ((*state) && (*appdata == UIP_NULL))                                     // 避免异常情况导致内存泄露, 正常情况不会发生
    {
        socket_close();
        return;
    }

    // 连接被关闭的情况随时可能发生, 因此要在 SOCKET 函数头部处理
    if (socket_closed() == SOCKET_CLOSED)                                       // 客户端主动关闭了链接
    {
        memb_free(&tcps_app, *appdata);                                         // 需释放应用层数据结构空间
        return;
    }

    switch (*state)
    {
    case 0: {
        tcps_app_t *app;                                                        // 记录应用层数据结构指针

        // 此处为任务的入口, 可进行一些初始化工作
        *appdata = memb_alloc(&tcps_app);                                       // 获取应用层数据结构空间
        if (*appdata == UIP_NULL)                                               // 获取失败
        {
            assert_printf("tcp server memb_alloc error!\r\n");
            socket_close();
            return;
        }
        else
        {
            app = *appdata;
            strcpy(app->buf, "Hello, who are you?");
            app->state = 0;                                                     // 可用来标记应用层的处理状态, 用于演示
            *state = 1;
        }
        break; }

    case 1: {
        socket_t val;
        tcps_app_t *app = *appdata;                                             // 记录应用层数据结构指针

        val = socket_tx((uint8_t *)app->buf, strlen(app->buf), SOCKET_FOREVER); // 发送数据
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)                                                   // 发送成功
        {
            app->state = 0;
            *state = 2;
        }
        else                                                                    // 发送失败
        {
            app->state = 2;
            *state = 0xFF;
        }
        break; }

    case 2: {
        socket_t val;
        tcps_app_t *app = *appdata;                                             // 记录应用层数据结构指针

        val = socket_rx_chr((uint8_t *)app->buf, sizeof(app->buf), '\r', 20000);// 准备接收数据, 20s的输入时间
        if (val == SOCKET_WAIT) return;
        if ((val == SOCKET_OK) || (val == SOCKET_FULL))                         // 接收成功
        {
            app->buf[socket_len()-1] = 0;
            app->state = 0;
            *state = 3;
        }
        else                                                                    // 接收失败
        {
            strcpy(app->buf, "Goodbye, indifference world!");                   // 错误处理
            app->state = 1;
            *state = 4;
        }
        break; }

    case 3: {
        socket_t val;
        tcps_app_t *app = *appdata;                                             // 记录应用层数据结构指针

        val = socket_tx((uint8_t *)app->buf, strlen(app->buf), SOCKET_FOREVER);
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)
        {
            strcpy(app->buf, "See you later! Please close it...");
            app->state = 0;
            *state = 4;
        }
        else                                                                    // 发送失败
        {
            app->state = 2;
            *state = 0xFF;
        }
        break; }

    case 4: {
        socket_t val;
        tcps_app_t *app = *appdata;                                             // 记录应用层数据结构指针

        val = socket_tx((uint8_t *)app->buf, strlen(app->buf), SOCKET_FOREVER);
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)
        {
            if (app->state == 0) *state = 5;
            else                 *state = 6;
        }
        else                                                                    // 发送失败
        {
            app->state = 2;
            *state = 0xFF;
        }
        break; }

    case 5: {
        // 等待客户端主动关闭连接
        // 处理在此函数的 if (socket_closed() == SOCKET_CLOSED) 内
        socket_break();                                                         // 使socket跳出外部循环
        break; }

    case 0xFF:                                                                  // 错误处理
        assert_printf("tcp server tx_socket error!!!\r\n");
    default:
        socket_close();                                                         // 主动关闭连接
        memb_free(&tcps_app, *appdata);                                         // 释放应用层数据结构空间
        break;
    }
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
