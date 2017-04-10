/**
  ******************************************************************************
  * @file    udpc.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.16
  * @brief   提供 udp 客户端示例
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "udpc.h"
#include <string.h>



/** @addtogroup Components
  * @{
  */
/** @defgroup UDPClient
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup UDPClient_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup UDPClient_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Private_Function
  * @brief    定义私有函数
  * @{
  */
static SOCKET(socket_udp_client);
/**
  * @}
  */



/** @defgroup UDPClient_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  udp客户端
  * @param  ip, port, 服务器的ip地址和端口号
  * @retval none
  ******************************************************************************
  */
void example_udp_client(uip_ipaddr_t *ip, uint16_t port)
{
    udp_client_delete(socket_udp_client);
    udp_client_add(socket_udp_client, UIP_NULL, ip, port);
}

/**
  ******************************************************************************
  * @brief  udp客户端socket回调处理函数
  * @param  socket回调函数内置参数
  * @retval none
  ******************************************************************************
  */
SOCKET(socket_udp_client)
{
    static char str[40];
    static uint8_t linked;

    switch (*state)
    {
    case 0: {
        // 此处为任务的入口, 可进行一些初始化工作
        strcpy(str, "Hello, who are you? input \"bye\" to end...");
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

        val = socket_rx_chr((uint8_t *)str, sizeof(str), '\r', 30000);          // 准备接收数据, 30s的输入时间
        if (val == SOCKET_WAIT) return;
        if ((val == SOCKET_OK) || (val == SOCKET_FULL))                         // 接收成功
        {
            str[socket_len()-1] = 0;
            if (strncmp(str, "bye", 3) == 0)                                    // 输入了 bye
            {
                strcpy(str, "See you later!");
                linked = 1;                                                     // 需断开连接
            }
            else
            {
                linked = 0;
            }
        }
        else                                                                    // 接收失败
        {
            strcpy(str, "Goodbye, indifference world!");
            linked = 2;                                                         // 标记为接收错误
        }
        *state = 3;
        break; }

    case 3: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // 发送数据
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)
        {
            if (linked) *state = 4;                                             // 断开连接
            else        *state = 2;
        }
        else *state = 0xFF;                                                     // 发送失败
        break; }

    case 0xFF:                                                                  // 错误处理
        assert_printf("udp client socket error!!!\r\n");
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
