/**
  ******************************************************************************
  * @file    udpc.c
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.16
  * @brief   �ṩ udp �ͻ���ʾ��
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
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup UDPClient_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static SOCKET(socket_udp_client);
/**
  * @}
  */



/** @defgroup UDPClient_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  udp�ͻ���
  * @param  ip, port, ��������ip��ַ�Ͷ˿ں�
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
  * @brief  udp�ͻ���socket�ص�������
  * @param  socket�ص��������ò���
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
        // �˴�Ϊ��������, �ɽ���һЩ��ʼ������
        strcpy(str, "Hello, who are you? input \"bye\" to end...");
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

        val = socket_rx_chr((uint8_t *)str, sizeof(str), '\r', 30000);          // ׼����������, 30s������ʱ��
        if (val == SOCKET_WAIT) return;
        if ((val == SOCKET_OK) || (val == SOCKET_FULL))                         // ���ճɹ�
        {
            str[socket_len()-1] = 0;
            if (strncmp(str, "bye", 3) == 0)                                    // ������ bye
            {
                strcpy(str, "See you later!");
                linked = 1;                                                     // ��Ͽ�����
            }
            else
            {
                linked = 0;
            }
        }
        else                                                                    // ����ʧ��
        {
            strcpy(str, "Goodbye, indifference world!");
            linked = 2;                                                         // ���Ϊ���մ���
        }
        *state = 3;
        break; }

    case 3: {
        socket_t val;

        val = socket_tx((uint8_t *)str, strlen(str), SOCKET_FOREVER);           // ��������
        if (val == SOCKET_WAIT) return;
        if (val == SOCKET_OK)
        {
            if (linked) *state = 4;                                             // �Ͽ�����
            else        *state = 2;
        }
        else *state = 0xFF;                                                     // ����ʧ��
        break; }

    case 0xFF:                                                                  // ������
        assert_printf("udp client socket error!!!\r\n");
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
