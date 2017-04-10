/**
  ******************************************************************************
  * @file    tcpc.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.8
  * @brief   提供 tcp 客户端示例
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "tcpc.h"
#include <string.h>



/** @addtogroup Components
  * @{
  */
/** @defgroup TCPClient
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup TCPClient_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup TCPClient_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Private_Function
  * @brief    定义私有函数
  * @{
  */
static SOCKET(socket_tcp_client);
/**
  * @}
  */



/** @defgroup TCPClient_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  tcp客户端
  * @param  ip, port, 服务器的ip地址和端口号
  * @retval none
  ******************************************************************************
  */
void example_tcp_client(uip_ipaddr_t *ip, uint16_t port)
{
    tcp_client_delete(socket_tcp_client);
    tcp_client_add(socket_tcp_client, UIP_NULL, ip, port);
}

/**
  ******************************************************************************
  * @brief  tcp客户端socket回调处理函数
  * @param  socket回调函数内置参数
  * @retval none
  ******************************************************************************
  */
SOCKET(socket_tcp_client)
{
    static char str[30];

    // 连接被关闭的情况随时可能发生, 因此要在 SOCKET 函数头部处理
    if (socket_closed() == SOCKET_CLOSED)
    {
        // 此处做一些任务结束的处理
        // 该范例无需做任何处理
        return;
    }

    switch (*state)
    {
    case 0: {
        // 此处为任务的入口, 可进行一些初始化工作
        strcpy(str, "Hello, who are you?");
        *state = 1;
        break; }

    case 1: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // 发送数据
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)  *state = 2;                                      // 发送成功
        else                   *state = 0xFF;                                   // 发送失败
        break; }

    case 2: {
        socket_t val;

        val = socket_rx_chr((uint8_t *)str, sizeof(str), '\r', 20000);          // 准备接收数据, 20s的输入时间
        if (val == SOCKET_WAIT) return;
        if ((val == SOCKET_OK) || (val == SOCKET_FULL))                         // 接收成功
        {
            str[socket_len()-1] = 0;
            *state = 3;
        }
        else                                                                    // 接收失败
        {
            strcpy(str, "Goodbye, indifference world!");                        // 错误处理
            *state = 4;
        }
        break; }

    case 3: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)
        {
            strcpy(str, "See you later!");
            *state = 4;
        }
        else                                                                    // 发送失败
        {
            *state = 0xFF;
        }
        break; }

    case 4: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);
        if (val == SOCKET_WAIT) return;
        socket_delay(10000);                                                    // 延时10S, 可看清发送的数据
        *state = 5;
        break; }

    case 0xFF:                                                                  // 错误处理
        assert_printf("tcp client tx_socket error!!!\r\n");
    default:
        socket_close();                                                         // 关闭此连接
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
