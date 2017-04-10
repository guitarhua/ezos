/**
  ******************************************************************************
  * @file    udps.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.16
  * @brief   �ṩ udp ������ʾ��.
  *          �˷�����, UDP������ÿ��ֻ�����һ���ͻ��˽��з���.
  *          UDP�ķ������ͻ���ģ�ͱ����ϲ�������.
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
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPServer_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static SOCKET(socket_udp_server);
/**
  * @}
  */



/** @defgroup UDPServer_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  udp������
  * @param  port, �������Ķ˿ں�
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
  * @brief  udp������socket�ص�������
  * @param  socket�ص��������ò���
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

        // �˴�Ϊ��������, �ɽ���һЩ��ʼ������
        val = socket_rx_chr((uint8_t *)str, sizeof(str), '\r', SOCKET_FOREVER);
        if (val == SOCKET_WAIT) return;
        if ((val == SOCKET_OK) || (val == SOCKET_FULL))
        {
            if (strncmp(str, "link", 4) == 0)
            {
                // ���յ�����"link", ���ӳɹ�, ��ʼ������
                strcpy(str, "Hello, who are you?");
                *state = 1;
            }
            else
            {
                // ������ʼ���ݴ���Ĵ���, �ؼ���Ҫ����socket_close()����
                strcpy(str, "Please input \"link\" to start!");
                *state = 0xF0;
            }
        }
        break; }

    case 0xF0: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // ������ʾ��Ϣ
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK) *state = 0xF1;                                    // ������ɺ�, ����socket_close()����
        else *state = 0xFF;                                                     // ���ʹ���
        break; }

    case 0xF1: {
        socket_close();                                                         // ������ʼ���ݴ���, �����������
        // *state = 0; ����д�˾�, socket_close ���Զ����õ���ʼ״̬
        break; }

    case 1: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // ��������
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK) *state = 2;
        else *state = 0xFF;                                                     // ���ʹ���
        break; }

    case 2: {
        socket_t val;

        val = socket_rx_chr((uint8_t *)str, sizeof(str), '\r', 20000);          // ׼����������, 20s������ʱ��
        if (val == SOCKET_WAIT) return;
        if ((val == SOCKET_OK) || (val == SOCKET_FULL))                         // ���ճɹ�
        {
            str[socket_len()-1] = 0;
            *state = 3;
        }
        else                                                                    // ����ʧ��
        {
            strcpy(str, "Goodbye, indifference world!");                        // ������
            *state = 4;
        }
        break; }

    case 3: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // ��������
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)
        {
            strcpy(str, "See you later!");
            *state = 4;
        }
        else                                                                    // ���ʹ���
        {
            *state = 0xFF;
        }
        break; }

    case 4: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // ��������
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK) *state = 5;
        else *state = 0xFF;                                                     // ���ʹ���
        break; }

    case 0xFF:                                                                  // ������
        assert_printf("udp server tx_socket error!!!\r\n");
    default:
        socket_close();                                                         // �رմ�����
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
