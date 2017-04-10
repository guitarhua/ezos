/**
  ******************************************************************************
  * @file    uip_conf.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.1.3
  * @brief   uIP常用配置. uIP更多的配置内容见"uip_popt.h"
  ******************************************************************************
  */
#ifndef __UIP_CONF_H__
#define __UIP_CONF_H__

#include <stdint.h>
#include "assert.h"                                                             // assert_printf



/*-----------------------------------------------------------------------------/
/ uIP APP功能配置
/-----------------------------------------------------------------------------*/
/// 配置DHCP客户端功能(自动分配IP地址). 0-不使能, 1-使能
/// 此功能需要使能 UIP_CONF_UDP 及 UIP_CONF_BROADCAST, 并占用一个UDP连接
#define UIP_DHCP                        1

/// 配置DNS客户端功能及DNS表大小(域名解析). 0-不使能, >0-使能, 值为DNS表大小, 建议值4
/// 此功能需要使能 UIP_CONF_UDP
#define UIP_DNS                         4

/// 配置SMTP客户端功能(发送邮件). 0-不使能, 1-使能
#define UIP_SMTP                        0



/*-----------------------------------------------------------------------------/
/ IP地址初始设置. (应用层可通过函数修改)
/-----------------------------------------------------------------------------*/
/// 设置本机IP地址
#define UIP_IPADDR0                     192
#define UIP_IPADDR1                     168
#define UIP_IPADDR2                     1
#define UIP_IPADDR3                     10

/// 设置子网掩码
#define UIP_MSKADDR0                    255
#define UIP_MSKADDR1                    255
#define UIP_MSKADDR2                    255
#define UIP_MSKADDR3                    0

/// 设置默认网关
#define UIP_GWADDR0                     192
#define UIP_GWADDR1                     168
#define UIP_GWADDR2                     1
#define UIP_GWADDR3                     1

/// 设置DNS服务器, 仅使能DNS功能时会用到
#define UIP_DNSADDR0                    192
#define UIP_DNSADDR1                    168
#define UIP_DNSADDR2                    1
#define UIP_DNSADDR3                    1



/*-----------------------------------------------------------------------------/
/ uIP 基础功能配置
/-----------------------------------------------------------------------------*/
/// 配置uIP系统任务的优先级, 范围0-250, 最高优先级为0. 建议使用高优先级
#define TASK_UIPSYS_PRIORITY            1

/// 配置uIP接收任务的工作方式, 0-中断模式, !0-轮询模式, 其值为扫描间隔时间.
/// 中断模式可提供最大的网络性能. 建议值 0.
/// 轮询模式较容易实现, 允许值 1-20ms, 建议值10ms.
#define TASK_UIPSYS_RX_SCAN_MS          0

/// 配置uIP调试功能. 0-不使能, 1-使能
#define UIP_CONF_LOGGING                0

/// 配置uIP统计功能. 0-不使能, 1-使能
#define UIP_CONF_STATISTICS             1

/// 配置CPU数据格式. UIP_LITTLE_ENDIAN-小端模式, UIP_BIG_ENDIAN-大端模式
#define UIP_CONF_BYTE_ORDER             UIP_LITTLE_ENDIAN



/*-----------------------------------------------------------------------------/
/ uIP TCP相关配置
/-----------------------------------------------------------------------------*/
/// 配置TCP连接的最大数量, 范围 1-200, 建议值 10.
#define UIP_CONF_MAX_CONNECTIONS        10

/// 配置监听端口的最大数量, 相当于TCP服务器应用的数量. 范围 1-100, 建议值5.
#define UIP_CONF_MAX_LISTENPORTS        5



/*-----------------------------------------------------------------------------/
/ uIP UDP相关配置
/-----------------------------------------------------------------------------*/
/// 配置UDP功能. 0-不使能, 1-使能. 建议使能, 否则与UDP相关的上层应用无法使用.
#define UIP_CONF_UDP                    1

/// 配置广播功能. 0-不使能, 1-使能, 且必须使能UDP功能!
#if UIP_CONF_UDP
    #define UIP_CONF_BROADCAST          1
#endif

/// 配置UDP连接的最大数量. 范围 1-200, 建议值 10.
#define UIP_CONF_UDP_CONNS              10

/// 配置UDP校验功能. 仅有调试意义. 因为uIP会直接丢弃校验错误的包, 且不通知应用层!
#define UIP_CONF_UDP_CHECKSUMS          UIP_CONF_LOGGING



/*-----------------------------------------------------------------------------/
/ uIP 其它配置
/-----------------------------------------------------------------------------*/
/// 配置 uIP packet 缓冲区大小. 范围 60-1500. 建议使用1500, 可提高稳定性.
#define UIP_CONF_BUFFER_SIZE            1500

/**
  * 配置ARP表的大小. 可以不进行宏定义. 系统默认值 8
  * 如果本地网络中有许多到这个uIP节点的连接，那么这个选项应该设置为一个比较大的值.
  */
#define UIP_CONF_ARPTAB_SIZE            8



#endif /* __UIP_CONF_H__ */

/** @} */
/** @} */
