/**
  ******************************************************************************
  * @file    udps.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.16
  * @brief   提供 udp 服务器示例.
  *          此范例中, UDP服务器每次只能针对一个客户端进行服务.
  *          UDP的服务器客户端模型本质上并不明显.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "udps.h"
#include <string.h>



/** @addtogroup Components
  * @{
  */
/** @defgroup UDPServer
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup UDPServer_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup UDPServer_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Private_Function
  * @brief    定义私有函数
  * @{
  */
static SOCKET(socket_udp_server);
/**
  * @}
  */



/** @defgroup UDPServer_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  udp服务器
  * @param  port, 服务器的端口号
  * @retval none
  ******************************************************************************
  */
void example_udp_server(uint16_t port)
{
    udp_server_delete(socket_udp_server);
    udp_server_add(socket_udp_server, UIP_NULL, port);
}

/**
  ******************************************************************************
  * @brief  udp服务器socket回调处理函数
  * @param  socket回调函数内置参数
  * @retval none
  ******************************************************************************
  */
SOCKET(socket_udp_server)
{
    static char str[40];

    switch (*state)
    {
    case 0: {
        socket_t val;

        // 此处为任务的入口, 可进行一些初始化工作
        val = socket_rx_chr((uint8_t *)str, sizeof(str), '\r', SOCKET_FOREVER);
        if (val == SOCKET_WAIT) return;
        if ((val == SOCKET_OK) || (val == SOCKET_FULL))
        {
            if (strncmp(str, "link", 4) == 0)
            {
                // 接收到数据"link", 链接成功, 开始处理函数
                strcpy(str, "Hello, who are you?");
                *state = 1;
            }
            else
            {
                // 接收起始数据错误的处理, 关键是要调用socket_close()函数
                strcpy(str, "Please input \"link\" to start!");
                *state = 0xF0;
            }
        }
        break; }

    case 0xF0: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // 发送提示信息
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK) *state = 0xF1;                                    // 发送完成后, 调用socket_close()函数
        else *state = 0xFF;                                                     // 发送错误
        break; }

    case 0xF1: {
        socket_close();                                                         // 接收起始数据错误, 清空已有连接
        // *state = 0; 无需写此句, socket_close 会自动设置到初始状态
        break; }

    case 1: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // 发送数据
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK) *state = 2;
        else *state = 0xFF;                                                     // 发送错误
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

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // 发送数据
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)
        {
            strcpy(str, "See you later!");
            *state = 4;
        }
        else                                                                    // 发送错误
        {
            *state = 0xFF;
        }
        break; }

    case 4: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // 发送数据
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK) *state = 5;
        else *state = 0xFF;                                                     // 发送错误
        break; }

    case 0xFF:                                                                  // 错误处理
        assert_printf("udp server tx_socket error!!!\r\n");
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
