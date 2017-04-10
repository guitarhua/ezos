/**
  ******************************************************************************
  * @file    tcps.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.8
  * @brief   �ṩ tcp ������ʾ��.
  *          ��������Ӧ�ñȿͻ��˸���, ��������ҪΪ���ͻ��˶������ֳ����ݿռ�.
  *          �˷�����ʹ�� memb.c �ļ�ʵ�ֵ�. ���������õĿռ�ͨ�� *appdata����.
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
  * @brief    ˽�����Ͷ���
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
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPServer_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
// ����memb�����ݽṹ, ����, �Ϳ�������.
// �����Ϊ���ڴ濪��������һ�����򹩷���ʹ��,
// tcps_app_t tcps_app[UIP_CONF_MAX_CONNECTIONS];
MEMB(tcps_app_t, tcps_app, UIP_CONF_MAX_CONNECTIONS);
/**
  * @}
  */

/** @defgroup TCPServer_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static SOCKET(socket_tcp_server);
/**
  * @}
  */



/** @defgroup TCPServer_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  tcp������
  * @param  port, �������Ķ˿ں�
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
  * @brief  tcp��������socket�ص�������
  * @param  socket�ص��������ò���
  * @retval none
  ******************************************************************************
  */
SOCKET(socket_tcp_server)
{
    if ((*state) && (*appdata == UIP_NULL))                                     // �����쳣��������ڴ�й¶, ����������ᷢ��
    {
        socket_close();
        return;
    }

    // ���ӱ��رյ������ʱ���ܷ���, ���Ҫ�� SOCKET ����ͷ������
    if (socket_closed() == SOCKET_CLOSED)                                       // �ͻ��������ر�������
    {
        memb_free(&tcps_app, *appdata);                                         // ���ͷ�Ӧ�ò����ݽṹ�ռ�
        return;
    }

    switch (*state)
    {
    case 0: {
        tcps_app_t *app;                                                        // ��¼Ӧ�ò����ݽṹָ��

        // �˴�Ϊ��������, �ɽ���һЩ��ʼ������
        *appdata = memb_alloc(&tcps_app);                                       // ��ȡӦ�ò����ݽṹ�ռ�
        if (*appdata == UIP_NULL)                                               // ��ȡʧ��
        {
            assert_printf("tcp server memb_alloc error!\r\n");
            socket_close();
            return;
        }
        else
        {
            app = *appdata;
            strcpy(app->buf, "Hello, who are you?");
            app->state = 0;                                                     // ���������Ӧ�ò�Ĵ���״̬, ������ʾ
            *state = 1;
        }
        break; }

    case 1: {
        socket_t val;
        tcps_app_t *app = *appdata;                                             // ��¼Ӧ�ò����ݽṹָ��

        val = socket_tx((uint8_t *)app->buf, strlen(app->buf), SOCKET_FOREVER); // ��������
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)                                                   // ���ͳɹ�
        {
            app->state = 0;
            *state = 2;
        }
        else                                                                    // ����ʧ��
        {
            app->state = 2;
            *state = 0xFF;
        }
        break; }

    case 2: {
        socket_t val;
        tcps_app_t *app = *appdata;                                             // ��¼Ӧ�ò����ݽṹָ��

        val = socket_rx_chr((uint8_t *)app->buf, sizeof(app->buf), '\r', 20000);// ׼����������, 20s������ʱ��
        if (val == SOCKET_WAIT) return;
        if ((val == SOCKET_OK) || (val == SOCKET_FULL))                         // ���ճɹ�
        {
            app->buf[socket_len()-1] = 0;
            app->state = 0;
            *state = 3;
        }
        else                                                                    // ����ʧ��
        {
            strcpy(app->buf, "Goodbye, indifference world!");                   // ������
            app->state = 1;
            *state = 4;
        }
        break; }

    case 3: {
        socket_t val;
        tcps_app_t *app = *appdata;                                             // ��¼Ӧ�ò����ݽṹָ��

        val = socket_tx((uint8_t *)app->buf, strlen(app->buf), SOCKET_FOREVER);
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)
        {
            strcpy(app->buf, "See you later! Please close it...");
            app->state = 0;
            *state = 4;
        }
        else                                                                    // ����ʧ��
        {
            app->state = 2;
            *state = 0xFF;
        }
        break; }

    case 4: {
        socket_t val;
        tcps_app_t *app = *appdata;                                             // ��¼Ӧ�ò����ݽṹָ��

        val = socket_tx((uint8_t *)app->buf, strlen(app->buf), SOCKET_FOREVER);
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)
        {
            if (app->state == 0) *state = 5;
            else                 *state = 6;
        }
        else                                                                    // ����ʧ��
        {
            app->state = 2;
            *state = 0xFF;
        }
        break; }

    case 5: {
        // �ȴ��ͻ��������ر�����
        // �����ڴ˺����� if (socket_closed() == SOCKET_CLOSED) ��
        socket_break();                                                         // ʹsocket�����ⲿѭ��
        break; }

    case 0xFF:                                                                  // ������
        assert_printf("tcp server tx_socket error!!!\r\n");
    default:
        socket_close();                                                         // �����ر�����
        memb_free(&tcps_app, *appdata);                                         // �ͷ�Ӧ�ò����ݽṹ�ռ�
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
