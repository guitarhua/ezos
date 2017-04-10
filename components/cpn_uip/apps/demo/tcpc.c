/**
  ******************************************************************************
  * @file    tcpc.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.8
  * @brief   �ṩ tcp �ͻ���ʾ��
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
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPClient_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static SOCKET(socket_tcp_client);
/**
  * @}
  */



/** @defgroup TCPClient_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  tcp�ͻ���
  * @param  ip, port, ��������ip��ַ�Ͷ˿ں�
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
  * @brief  tcp�ͻ���socket�ص�������
  * @param  socket�ص��������ò���
  * @retval none
  ******************************************************************************
  */
SOCKET(socket_tcp_client)
{
    static char str[30];

    // ���ӱ��رյ������ʱ���ܷ���, ���Ҫ�� SOCKET ����ͷ������
    if (socket_closed() == SOCKET_CLOSED)
    {
        // �˴���һЩ��������Ĵ���
        // �÷����������κδ���
        return;
    }

    switch (*state)
    {
    case 0: {
        // �˴�Ϊ��������, �ɽ���һЩ��ʼ������
        strcpy(str, "Hello, who are you?");
        *state = 1;
        break; }

    case 1: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // ��������
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)  *state = 2;                                      // ���ͳɹ�
        else                   *state = 0xFF;                                   // ����ʧ��
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

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)
        {
            strcpy(str, "See you later!");
            *state = 4;
        }
        else                                                                    // ����ʧ��
        {
            *state = 0xFF;
        }
        break; }

    case 4: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);
        if (val == SOCKET_WAIT) return;
        socket_delay(10000);                                                    // ��ʱ10S, �ɿ��巢�͵�����
        *state = 5;
        break; }

    case 0xFF:                                                                  // ������
        assert_printf("tcp client tx_socket error!!!\r\n");
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
