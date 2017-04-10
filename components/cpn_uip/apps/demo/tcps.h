/**
  ******************************************************************************
  * @file    tcps.h
  * @author  mousie
  * @version V1.0.0
  * @date    2013.4.8
  * @brief   �ṩ tcp ������ʾ��.
  *          ��������Ӧ�ñȿͻ��˸���, ��������ҪΪ���ͻ��˶������ֳ����ݿռ�.
  *          �˷�����ʹ�� memb.c �ļ�ʵ�ֵ�. ���������õĿռ�ͨ�� *appdata����.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TCPS_H
#define __TCPS_H

/* Include -------------------------------------------------------------------*/
#include <stdint.h>
#include "tcpip.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup TCPServer
  * @{
  */



/**
  ******************************************************************************
  * @defgroup TCPServer_Configure
  * @brief    �û�����
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup TCPServer_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPServer_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPServer_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup TCPServer_Public_Function
  * @brief    ���幫�к���
  * @{
  */
void example_tcp_server(uint16_t port);
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
