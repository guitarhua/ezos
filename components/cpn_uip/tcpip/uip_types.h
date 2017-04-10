/**
  ******************************************************************************
  * @file    uip_types.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.3.28
  * @brief   UIP 数据类型定义 及 底层宏定义
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
  * @brief    公有类型定义
  * @{
  */
typedef uint16_t                        uip_stats_t;
typedef struct uip_eth_addr             uip_macaddr_t;
typedef struct uip_conn                 uip_conn_t;
typedef struct uip_udp_conn             uip_udp_conn_t;
typedef uint16_t                        sock_s_t;

typedef enum
{
    UIP_TYPE_NULL   = 0,                                                        // 回调函数无类型
    TCP_SERVER      = 1,                                                        // TCP服务器类型
    TCP_CLIENT      = 2,                                                        // TCP客户端类型
    UDP_SERVER      = 3,                                                        // UDP服务器类型
    UDP_CLIENT      = 4,                                                        // UDP客户端类型
} uip_type_t;

typedef enum
{
    SOCKET_OK       = 0,                                                        // socket收发成功/连接正常
    SOCKET_TIMEOUT  = 1,                                                        // socket收发失败/任务超时
    SOCKET_WAIT     = 2,                                                        // socket收发等待状态
    SOCKET_FULL     = 3,                                                        // socket接收数据溢出
    SOCKET_CLOSED   = 4,                                                        // socket连接已关闭
    SOCKET_ERROR    = 0xFF,                                                     // socket参数错误
} socket_t;

typedef enum
{
    SOCK_NULL       = 0x00,                                                     // socket回调函数初始状态
    SOCK_DELAY      = 0x81,                                                     // socket回调函数延时状态
    SOCK_TX_ING     = 0x82,                                                     // socket回调函数发送数据状态
    SOCK_TX_ACK     = 0x83,                                                     // socket回调函数等待应答
    SOCK_RX_ING     = 0x85,                                                     // socket回调函数接收数据状态
    SOCK_RX_OK      = 0x06,                                                     // socket回调函数完成接收
    SOCK_CLOSING    = 0x87,                                                     // socket回调函数TCP关闭连接状态
    SOCK_CLOSED     = 0x88,                                                     // socket回调函数连接已关闭
    SOCK_BREAK_MASK = 0x80,                                                     // socket高位掩码, 用于跳出循环
    SOCK_ERROR      = 0xFF,                                                     // socket回调函数错误状态
} uip_sock_t;

typedef struct
{
    void *conn;                                                                 // socket所处的连接地址
    void (*psock)(sock_s_t *state, void **appdata);                             // socket回调函数指针
    void *appdata;                                                              // socket回调函数应用层数据指针
    int32_t time;                                                               // socket回调函数超时时间
    uint8_t *sockptr;                                                           // socket发送/接收区指针
    uint16_t socklen;                                                           // socket发送/接收区长度
    sock_s_t state;                                                             // socket处理函数状态
    uip_type_t type;                                                            // socket类型(TCP/UDP, SERVER/CLIENT)
    uip_sock_t sock;                                                            // socket协议状态
} uip_task_t;

typedef uip_task_t                      uip_tcp_appstate_t;
typedef uip_task_t                      uip_udp_appstate_t;

typedef struct
{
    void (*psock)(sock_s_t *state, void **appdata);                             // TCP服务器回调函数
    void *pdata;                                                                // 回调函数的数据参数
    uint16_t port;                                                              // 监听端口
    uint8_t  conn_num;                                                          // 处于连接状态的数目
} uip_listen_t;
/**
  * @}
  */

/** @defgroup UIP_Types_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */
// 定义UIP回调函数
extern void uip_tcp_appcall(void);
extern void uip_udp_appcall(void);
#define UIP_APPCALL                     uip_tcp_appcall
#define UIP_UDP_APPCALL                 uip_udp_appcall

/// UIP NULL 定义
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
