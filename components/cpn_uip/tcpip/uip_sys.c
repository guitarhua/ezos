/**
  ******************************************************************************
  * @file    uip_sys.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.1.3
  * @brief   uIP系统基础函数. 并屏蔽uIP底层函数.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "dhcp.h"
#include "dns.h"
#include "tcpip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "ezos.h"
#include "drv_systick.h"



/** @addtogroup Components
  * @{
  */
/** @defgroup UIP_SYS
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup UIP_SYS_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup UIP_SYS_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UIP_SYS_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup UIP_SYS_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
static ezos_sem_t uipsys_100ms_sem;                                             // uIP定时任务的信号量
static ezos_sem_t uipsys_rx_sem;                                                // uIP接收数据的信号量

uip_ipaddr_t uip_dnsaddr;                                                       // uIP DNS服务器地址, uIP核心代码未提供, 故写在此处
/**
  * @}
  */

/** @defgroup UIP_SYS_Private_Function
  * @brief    定义私有函数
  * @{
  */
extern void uip_tcp_udp_init(void);                                             // tcp udp 应用层初始化
extern void uip_tcp_udp_time(void);                                             // tcp udp 任务定时函数

static void uipsys_conn(void);                                                  // uIP定时回调函数

static ezos_task_t task_uipsys_100ms(ezos_state_t *state, ezos_para_t *para);   // uIP 100ms定时任务
static ezos_task_t task_uipsys_rx(ezos_state_t *state, ezos_para_t *para);      // uIP数据接收任务
/**
  * @}
  */



/** @defgroup UIP_SYS_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  uIP 系统初始化函数
  * @param  mac, 网络硬件地址(MAC)
  * @retval none
  ******************************************************************************
  */
void uipsys_init(uip_macaddr_t mac)
{
    // 初始化信号量
    ezos_sem_add(&uipsys_100ms_sem, 0);
    ezos_sem_add(&uipsys_rx_sem, 0);

    // 设置uIP mac地址
    uip_setethaddr(mac);
    // 网络驱动芯片初始化
    tapdev_init();
    delay_ms(20);
    // UIP协议栈初始化
    uip_init();

    // UIP TCP UDP 应用层初始化
    uip_tcp_udp_init();

#if UIP_DHCP
    dhcp_start();
#else
    {
    uip_ipaddr_t ip;

    // 设置本机IP地址
    ip_set(&ip, UIP_IPADDR0,  UIP_IPADDR1,  UIP_IPADDR2,  UIP_IPADDR3);
    ip_set_host(&ip);
    // 设置子网掩码
    ip_set(&ip, UIP_MSKADDR0, UIP_MSKADDR1, UIP_MSKADDR2, UIP_MSKADDR3);
    ip_set_mask(&ip);
    // 设置默认网关
    ip_set(&ip, UIP_GWADDR0,  UIP_GWADDR1,  UIP_GWADDR2,  UIP_GWADDR3);
    ip_set_gateway(&ip);
    // 设置DNS服务器
    ip_set(&ip, UIP_DNSADDR0, UIP_DNSADDR1, UIP_DNSADDR2, UIP_DNSADDR3);
    ip_set_dns(&ip);
    }
#endif

#if UIP_DNS
    dns_clear();
#endif

    quick_add(task_uipsys_100ms, TASK_UIPSYS_PRIORITY+1);
    quick_add(task_uipsys_rx,    TASK_UIPSYS_PRIORITY);
}

/**
  ******************************************************************************
  * @brief  uIP 系统1ms定时扫描
  * @param  none
  * @retval none
  ******************************************************************************
  */
void uipsys_scan_1ms(void)
{
    static uint8_t uip_timer = 0;

    if (++uip_timer >= 100)
    {
        ezos_sem_release(&uipsys_100ms_sem);
        uip_timer = 0;
    }

#if TASK_UIPSYS_RX_SCAN_MS
    if ((uip_timer % TASK_UIPSYS_RX_SCAN_MS) == 0)                              // 轮询模式下, 定时检测数据输入
    {
        ezos_sem_release(&uipsys_rx_sem);
    }
#endif
}

/**
  ******************************************************************************
  * @brief  uIP 系统状态检查
  * @param  none
  * @返回值, uIP系统状态, 见 uipsys_t 类型
  ******************************************************************************
  */
uipsys_t uipsys_check(void)
{
    if (tapdev_linked() == 0) return UIPSYS_UNLINK;                             // 断网

#if UIP_DHCP
    // DHCP 动态IP地址分配
    {
        dhcp_app_t *dhcp = dhcp_get();

        if (dhcp->state == DHCP_OK)
        {
            return UIPSYS_DHCP_OK;                      // DHCP申请成功
        }
        else if (dhcp->state == DHCP_ERROR)
        {
            dhcp_stop();                                // 载入默认静态IP地址
            return UIPSYS_DHCP_ERROR;                   // DHCP申请失败
        }
        else if (dhcp->state == DHCP_BUSY)
        {
            return UIPSYS_DHCP_BUSY;                    // DHCP申请中
        }
    }
#endif

    return UIPSYS_STATIC_OK;                            // 使用静态IP
}

/**
  ******************************************************************************
  * @brief  uIP定时回调函数, 间隔为500ms
  * @param  none
  * @retval none
  ******************************************************************************
  */
void uipsys_conn(void)
{
    uint8_t i;

    // 查询TCP连接状态
    for (i=0; i < UIP_CONNS; i++)
    {
        if (uip_conn_active(i))
        {
            uip_periodic(i);
            if(uip_len > 0)
            {
                uip_arp_out();
                tapdev_send();
            }
        }
    }

#if UIP_UDP
    // 查询UDP连接状态
    for (i=0; i < UIP_UDP_CONNS; i++)
    {
        uip_udp_periodic(i);
        if(uip_len > 0)
        {
            uip_arp_out();
            tapdev_send();
        }
    }
#endif
}

/**
  ******************************************************************************
  * @brief  uIP 100ms定时任务.
  * @param  none
  * @retval none
  ******************************************************************************
  */
ezos_task_t task_uipsys_100ms(ezos_state_t *state, ezos_para_t *para)
{
    static uint8_t timer = 0;
    ezos_t val;

    while (1)
    {
        val = ezos_sem_take(&uipsys_100ms_sem, EZOS_FOREVER);
        if (val == EZOS_WAIT) return;
        else ++timer;

        // 100ms定时
        uip_tcp_udp_time();                                                     // tcp udp 回调函数定时

        // 500ms定时
        if ((timer % 5) == 0) uipsys_conn();                                    // uIP任务定时回调

        // 10s定时
        if (timer >= 100)
        {
            uip_arp_timer();                                                    // uIP ARP定时
            timer = 0;
        }
    }
}

/**
  ******************************************************************************
  * @brief  uIP数据接收任务.
  * @param  none
  * @retval none
  ******************************************************************************
  */
ezos_task_t task_uipsys_rx(ezos_state_t *state, ezos_para_t *para)
{
    ezos_t val;

    // 该任务至少1S扫描一次, 在信号量机制失效时, 可自恢复.
    val = ezos_sem_take(&uipsys_rx_sem, 1000);
    if (val == EZOS_WAIT) return;

    uip_len = tapdev_read();

    if (uip_len)
    {
        struct uip_eth_hdr *phead = (struct uip_eth_hdr *)&uip_buf[0];

        if(phead->type == UIP_HTONS(UIP_ETHTYPE_IP))
        {
            uip_arp_ipin();
            uip_input();
            if(uip_len > 0)
            {
                uip_arp_out();
                tapdev_send();
            }
        }
        else if (phead->type == UIP_HTONS(UIP_ETHTYPE_ARP))
        {
            uip_arp_arpin();
            if(uip_len > 0) tapdev_send();
        }
    }

    ezos_delay(0);
}

/**
  ******************************************************************************
  * @brief  底层驱动中断接收到数据, 调用此函数来释放信号量.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void uip_chip_irq_rx(void)
{
#if !TASK_UIPSYS_RX_SCAN_MS
    ezos_sem_release(&uipsys_rx_sem);
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
