/**
  ******************************************************************************
  * @file    smtp.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.26
  * @brief   �ṩ SMTP CLIENT ����. �����ʼ�
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SMTP_H
#define __SMTP_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include "tcpip.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup SMTP
  * @{
  */



/**
  ******************************************************************************
  * @defgroup SMTP_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
///// ����SMTP�ͻ��˹���(�����ʼ�). 0-��ʹ��, 1-ʹ��
//#define UIP_SMTP                        0
/**
  * @}
  */



/** @defgroup SMTP_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */
typedef enum
{
    EMAIL_OK = 0,                       // EMAIL ���ͳɹ�
    EMAIL_SENDING,                      // EMAIL ������
    EMAIL_ERROR,                        // EMAIL ����ʧ��, ͨѶ����, ���ط�
    EMAIL_PSWERR,                       // EMAIL ����ʧ��, �û����������
} email_type_t;

typedef struct
{
    uip_ipaddr_t server;                // ������IP��ַ
    char *hostname;                     // ��������
    char *from;                         // ���Ͷ� mail
    char *to;                           // ���ն� mail
    char *username;                     // ��¼�û���
    char *password;                     // ��¼����
    char *subject;                      // �ʼ�����
    char *msg;                          // �ʼ�����
} email_t;
/**
  * @}
  */

/** @defgroup SMTP_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup SMTP_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup SMTP_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if UIP_SMTP

uint8_t      smtp_send(email_t *email);                                         // �����ʼ�
email_type_t smtp_check(void);                                                  // ����ʼ�

#endif
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
