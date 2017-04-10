/**
  ******************************************************************************
  * @file    tcpip.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.1.3
  * @brief   uIP系统函数. 供上层应用调用.
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
  * @brief    用户配置
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup TCPIP_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */
typedef enum
{
    UIPSYS_NULL       = 0x00,                                                   // 初始值, 无此状态
    UIPSYS_UNLINK     = 0x01,                                                   // uIP 网络连接已断

    UIPSYS_DHCP_BUSY  = 0x11,                                                   // uIP DHCP设置中
    UIPSYS_DHCP_ERROR = 0x12,                                                   // uIP DHCP设置失败

    UIPSYS_DHCP_OK    = 0xF1,                                                   // uIP DHCP设置成功
    UIPSYS_STATIC_OK  = 0xF2,                                                   // uIP 静态IP设置成功

    UIPSYS_MASK_OK    = 0xF0,                                                   // uIP 设置成功掩码
} uipsys_t;
/**
  * @}
  */

/** @defgroup TCPIP_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */
/// socket 时间值, 表示永久等待
#define SOCKET_FOREVER                  (int32_t)-1

/// socket 回调函数宏定义
#define SOCKET(func)                    void func(sock_s_t *state, void **appdata)

/**
  * @}
  */

/** @defgroup TCPIP_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */
extern uip_ipaddr_t uip_dnsaddr;                                                // uIP DNS服务器地址
/**
  * @}
  */

/** @defgroup TCPIP_Public_Function
  * @brief    定义公有函数
  * @{
  */
// ***************** uIP 系统基础函数 ******************************************

void uipsys_init(uip_macaddr_t mac);                                            // uIP 系统初始化函数
void uipsys_scan_1ms(void);                                                     // uIP 系统1ms定时扫描
uipsys_t uipsys_check(void);                                                    // uIP 系统状态检查



// ***************** uIP系统 TCP/UDP 函数 (外部任务创建/删除连接) **************
// TCP服务器函数, 不允许在中断中使用
uip_listen_t *tcp_server_add(void (*psock)(sock_s_t *, void **), void* pdata, uint16_t listen_port);
uip_listen_t *tcp_server_delete(void (*psock)(sock_s_t *, void **));
uip_listen_t *tcp_server_get(void (*psock)(sock_s_t *, void **));

// TCP客户端函数, 不允许在中断中使用
uip_conn_t *tcp_client_add(void (*psock)(sock_s_t *, void **), void* pdata, uip_ipaddr_t *remote_ip, uint16_t remote_port);
uip_conn_t *tcp_client_delete(void (*psock)(sock_s_t *, void **));
uip_conn_t *tcp_client_get(void (*psock)(sock_s_t *, void **));

// UDP基础函数, 不允许在中断中使用
uip_udp_conn_t *udp_add(void (*psock)(sock_s_t *, void **), void* pdata, uip_ipaddr_t *remote_ip, uint16_t remote_port, uint16_t local_port);
uip_udp_conn_t *udp_delete(void (*psock)(sock_s_t *, void **));
uip_udp_conn_t *udp_get(void (*psock)(sock_s_t *, void **));

// UDP扩展函数, 基于UDP基础函数实现, 不允许在中断中使用
#define udp_server_add(psock, pdata, lport)         udp_add(psock, pdata, 0, 0, lport)                                  // 添加UDP服务器
#define udp_server_delete(psock)                    udp_delete(psock)                                                   // 删除UDP服务器
#define udp_server_get(psock)                       udp_get(psock)                                                      // 获取UDP服务器
#define udp_client_add(psock, pdata, rip, rport)    udp_add(psock, pdata, rip, rport, 0)                                // 添加UDP客户端
#define udp_client_delete(psock)                    udp_delete(psock)                                                   // 删除UDP客户端
#define udp_client_get(psock)                       udp_get(psock)                                                      // 获取UDP客户端
#define udp_broadcast_add(psock, pdata, rport)      udp_add(psock, pdata, (uip_ipaddr_t *)&uip_broadcast_addr, rport, 0)// 添加UDP广播
#define udp_broadcast_delete(psock)                 udp_delete(psock)                                                   // 删除UDP广播
#define udp_broadcast_get(psock)                    udp_get(psock)                                                      // 获取UDP广播



// ***************** uIP系统 SOCKET 函数 (socket回调函数用) ********************
socket_t socket_continue(void);                                                 // socket立刻重调用
socket_t socket_break(void);                                                    // socket主动退出轮询
socket_t socket_delay(int32_t time_ms);                                         // socket延时
socket_t socket_close(void);                                                    // socket关闭连接
socket_t socket_closed(void);                                                   // socket连接是否已关闭
uip_sock_t socket_status_get(void);                                             // socket获取当前状态

socket_t socket_tx(uint8_t *buf, uint16_t tx_len, int32_t time_ms);             // socket发送数据, 完成后socket_break()
socket_t socket_rx(uint8_t *buf, uint16_t max_len, int32_t time_ms);            // socket接收数据, 完成后socket_continue()
socket_t socket_rx_ptr(uint8_t **ptr, uint16_t *len, int32_t time_ms);          // socket接收数据, 有数据即返回数据起始位置和数据长度
socket_t socket_rx_len(uint8_t *buf, uint16_t rx_len, int32_t time_ms);         // socket接收指定长度数据
socket_t socket_rx_chr(uint8_t *buf, uint16_t max_len, uint8_t c, int32_t time_ms); // socket接收, 指定结束数据
socket_t socket_rx_str(uint8_t *buf, uint16_t max_len, char *s,   int32_t time_ms); // socket接收, 指定结束数据串
uint16_t socket_len(void);                                                      // socket获取数据长度(发送剩余数据/已收到数据)

/**
  * SOCKET 操作宏定义
  * 使用SOCKET宏定义操作, 不建议在该函数中使用switch语句!!! 若要使用必须注意:
  * switch 内不可使用SOCKET宏定义操作, 如 SOCKET_DELAY, SOCKET_WAIT_XXX, SOCKET_SUBFUN_XXX
  * 优点是阅读顺序清晰, 易于理解软件流程, 易于扩展子函数
  * 缺点是语法理解困难, 调试困难, 当有较复杂的分支情况时, 难以使用!
  * 综合而言, 不建议使用SOCKET宏定义操作, 宏定义过多后语意会变得晦涩难懂
  * 但以下两种情况可使用:
  * 1. 软件流程简单明了
  * 2. 单个函数代码量过大, 需拆分为子函数的形式
  * 子函数返回值类型必须为 socket_t, 参数则无限制
  * 子函数定义形式为 socket_t socket_sub_fun(...)
  * 子函数可嵌套使用, 需使用宏定义SOCKET_SUBFUN_XXX
  */
#define LC_SOCK_RESUME(s)               switch(s) { case 0:
#define LC_SOCK_SET(s)                  s = __LINE__; case __LINE__:
#define LC_SOCK_SET_EXECUTE(s, e)       s = __LINE__; e; case __LINE__:
#define LC_SOCK_END(s)                  s = 0; }

/// SOCKET 主的宏定义操作
#define SOCKET_BEGIN(s)                 {uint16_t *_s = &s; LC_SOCK_RESUME(*_s) // SOCKET_BEGIN() 与 SOCKET_END() 配对使用
#define SOCKET_END()                    LC_SOCK_END(*_s); return; }             // SOCKET_BEGIN() 与 SOCKET_END() 配对使用

#define SOCKET_DELAY(time)              \
    do {                                \
        socket_delay(time);             \
        LC_SOCK_SET_EXECUTE(*_s, return); \
    } while(0)                                                                  // SOCKET 延时宏定义

#define SOCKET_WAIT_SUBFUN(sub)         \
    do {                                \
        LC_SOCK_SET(*_s);               \
        if ((sub) == SOCKET_WAIT) return; \
    } while(0)                                                                  // SOCKET 子函数宏定义

#define SOCKET_WAIT_UNTIL(cond)         \
    do {                                \
        LC_SOCK_SET(*_s);               \
        if (!(cond)) {                  \
            if (!(socket_status_get() & SOCK_BREAK_MASK)) socket_break(); \
            return; }                   \
    } while(0)                                                                  // SOCKET 条件判断宏定义

#define SOCKET_WAIT_WHILE(cond)         SOCKET_WAIT_UNTIL(!(cond))              // SOCKET 条件判断宏定义

/// SOCKET 子函数的宏定义操作
#define SOCKET_SUBFUN_BEGIN(s)          {uint16_t *_s = &s; LC_SOCK_RESUME(*_s)
#define SOCKET_SUBFUN_END()             LC_SOCK_END(*_s); return SOCKET_OK; }

#define SOCKET_SUBFUN_DELAY(time)       \
    do {                                \
        socket_delay(time);             \
        LC_SOCK_SET_EXECUTE(*_s, return SOCKET_WAIT); \
    } while(0)                                                                  // SOCKET 延时宏定义

#define SOCKET_SUBFUN_WAIT_SUBFUN(sub)  \
    do {                                \
        LC_SOCK_SET(*_s);               \
        if ((sub) == SOCKET_WAIT) return SOCKET_WAIT; \
    } while(0)                                                                  // SOCKET 子函数调用宏定义

#define SOCKET_SUBFUN_WAIT_UNTIL(cond)  \
    do {                                \
        LC_SOCK_SET(*_s);               \
        if (!(cond)) {                  \
            if (!(socket_status_get() & SOCK_BREAK_MASK)) socket_break(); \
            return SOCKET_WAIT; }       \
    } while(0)                                                                  // SOCKET 条件判断宏定义

#define SOCKET_SUBFUN_WAIT_WHILE(cond)  SOCKET_SUBFUN_WAIT_UNTIL(!(cond))       // SOCKET 条件判断宏定义



// ***************** uIP系统 lib 函数 (应用层或回调函数用) *********************
#define ip_set(ip, a0, a1, a2, a3)      uip_ipaddr(ip, a0, a1, a2, a3)          // 设置IP值, 如 ip_set(&ip, 192, 168, 1, 1);
#define ip_copy(ipdst, ipsrc)           uip_ipaddr_copy(ipdst, ipsrc)           // 拷贝IP值, 如 ip_copy(&ipdst, &ipsrc);
#define ip_set_host(ip)                 uip_sethostaddr(ip)                     // 设置本机IP地址
#define ip_set_mask(ip)                 uip_setnetmask(ip)                      // 设置子网掩码
#define ip_set_gateway(ip)              uip_setdraddr(ip)                       // 设置默认网关
#define ip_set_dns(ip)                  uip_ipaddr_copy(&uip_dnsaddr, (ip))     // 设置DNS服务器

#define ip_get_host(ip)                 uip_gethostaddr(ip)                     // 获取本机IP地址
#define ip_get_mask(ip)                 uip_getnetmask(ip)                      // 获取子网掩码
#define ip_get_gateway(ip)              uip_getdraddr(ip)                       // 获取默认网关
#define ip_get_dns(ip)                  uip_ipaddr_copy((ip), &uip_dnsaddr)     // 获取DNS服务器

uint8_t ip_str2ip(const char *ipstr, uip_ipaddr_t *ip);                         // 将ip字符串转换为ip地址
uint8_t ip_ip2str(uip_ipaddr_t *ip, char *ipstr);                               // 将ip地址转换为ip字符串
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
