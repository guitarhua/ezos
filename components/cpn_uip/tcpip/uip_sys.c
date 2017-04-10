/**
  ******************************************************************************
  * @file    uip_sys.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.1.3
  * @brief   uIPϵͳ��������. ������uIP�ײ㺯��.
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
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup UIP_SYS_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup UIP_SYS_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
static ezos_sem_t uipsys_100ms_sem;                                             // uIP��ʱ������ź���
static ezos_sem_t uipsys_rx_sem;                                                // uIP�������ݵ��ź���

uip_ipaddr_t uip_dnsaddr;                                                       // uIP DNS��������ַ, uIP���Ĵ���δ�ṩ, ��д�ڴ˴�
/**
  * @}
  */

/** @defgroup UIP_SYS_Private_Function
  * @brief    ����˽�к���
  * @{
  */
extern void uip_tcp_udp_init(void);                                             // tcp udp Ӧ�ò��ʼ��
extern void uip_tcp_udp_time(void);                                             // tcp udp ����ʱ����

static void uipsys_conn(void);                                                  // uIP��ʱ�ص�����

static ezos_task_t task_uipsys_100ms(ezos_state_t *state, ezos_para_t *para);   // uIP 100ms��ʱ����
static ezos_task_t task_uipsys_rx(ezos_state_t *state, ezos_para_t *para);      // uIP���ݽ�������
/**
  * @}
  */



/** @defgroup UIP_SYS_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  uIP ϵͳ��ʼ������
  * @param  mac, ����Ӳ����ַ(MAC)
  * @retval none
  ******************************************************************************
  */
void uipsys_init(uip_macaddr_t mac)
{
    // ��ʼ���ź���
    ezos_sem_add(&uipsys_100ms_sem, 0);
    ezos_sem_add(&uipsys_rx_sem, 0);

    // ����uIP mac��ַ
    uip_setethaddr(mac);
    // ��������оƬ��ʼ��
    tapdev_init();
    delay_ms(20);
    // UIPЭ��ջ��ʼ��
    uip_init();

    // UIP TCP UDP Ӧ�ò��ʼ��
    uip_tcp_udp_init();

#if UIP_DHCP
    dhcp_start();
#else
    {
    uip_ipaddr_t ip;

    // ���ñ���IP��ַ
    ip_set(&ip, UIP_IPADDR0,  UIP_IPADDR1,  UIP_IPADDR2,  UIP_IPADDR3);
    ip_set_host(&ip);
    // ������������
    ip_set(&ip, UIP_MSKADDR0, UIP_MSKADDR1, UIP_MSKADDR2, UIP_MSKADDR3);
    ip_set_mask(&ip);
    // ����Ĭ������
    ip_set(&ip, UIP_GWADDR0,  UIP_GWADDR1,  UIP_GWADDR2,  UIP_GWADDR3);
    ip_set_gateway(&ip);
    // ����DNS������
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
  * @brief  uIP ϵͳ1ms��ʱɨ��
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
    if ((uip_timer % TASK_UIPSYS_RX_SCAN_MS) == 0)                              // ��ѯģʽ��, ��ʱ�����������
    {
        ezos_sem_release(&uipsys_rx_sem);
    }
#endif
}

/**
  ******************************************************************************
  * @brief  uIP ϵͳ״̬���
  * @param  none
  * @����ֵ, uIPϵͳ״̬, �� uipsys_t ����
  ******************************************************************************
  */
uipsys_t uipsys_check(void)
{
    if (tapdev_linked() == 0) return UIPSYS_UNLINK;                             // ����

#if UIP_DHCP
    // DHCP ��̬IP��ַ����
    {
        dhcp_app_t *dhcp = dhcp_get();

        if (dhcp->state == DHCP_OK)
        {
            return UIPSYS_DHCP_OK;                      // DHCP����ɹ�
        }
        else if (dhcp->state == DHCP_ERROR)
        {
            dhcp_stop();                                // ����Ĭ�Ͼ�̬IP��ַ
            return UIPSYS_DHCP_ERROR;                   // DHCP����ʧ��
        }
        else if (dhcp->state == DHCP_BUSY)
        {
            return UIPSYS_DHCP_BUSY;                    // DHCP������
        }
    }
#endif

    return UIPSYS_STATIC_OK;                            // ʹ�þ�̬IP
}

/**
  ******************************************************************************
  * @brief  uIP��ʱ�ص�����, ���Ϊ500ms
  * @param  none
  * @retval none
  ******************************************************************************
  */
void uipsys_conn(void)
{
    uint8_t i;

    // ��ѯTCP����״̬
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
    // ��ѯUDP����״̬
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
  * @brief  uIP 100ms��ʱ����.
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

        // 100ms��ʱ
        uip_tcp_udp_time();                                                     // tcp udp �ص�������ʱ

        // 500ms��ʱ
        if ((timer % 5) == 0) uipsys_conn();                                    // uIP����ʱ�ص�

        // 10s��ʱ
        if (timer >= 100)
        {
            uip_arp_timer();                                                    // uIP ARP��ʱ
            timer = 0;
        }
    }
}

/**
  ******************************************************************************
  * @brief  uIP���ݽ�������.
  * @param  none
  * @retval none
  ******************************************************************************
  */
ezos_task_t task_uipsys_rx(ezos_state_t *state, ezos_para_t *para)
{
    ezos_t val;

    // ����������1Sɨ��һ��, ���ź�������ʧЧʱ, ���Իָ�.
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
  * @brief  �ײ������жϽ��յ�����, ���ô˺������ͷ��ź���.
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
