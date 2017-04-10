/**
  ******************************************************************************
  * @file    ezos_conf.h
  * @author  mousie
  * @version V2.3.0
  * @date    2014.3.28
  * @brief   Э��ʽRTOS�����ļ�
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EZOS_CONF_H
#define __EZOS_CONF_H

/** @addtogroup OS
  * @{
  */
/** @addtogroup ezOS
  * @{
  */



// ezos����
/**
  * ����ezosϵͳʱ��.
  * ��Чֵ1-1000. ����ֵ 1ms �� 10ms.
  * ��ͬʱ�޸� ezos_scan_tick() �ĵ���ʱ��.
  */
#define __EZOS_TICK_MS                  1

#define __EZOS_TASK_NUM                 16                                      ///< ����ezos�����������, ��Чֵ1-250
//#define __EZOS_DEBUG                                                            ///< ʹ��ezos���ܵ���

#ifdef __EZOS_DEBUG
    #include <stdio.h>
    #define ezos_printf(...)            assert_printf(__VA_ARGS__)              ///< ����ezos���Դ�ӡ����
#else
    #define ezos_printf(...)
#endif



#define __EZOS_SEM                                                              ///< ʹ��ezos�ź���
#define __EZOS_MUTEX                                                            ///< ʹ��ezos������
#define __EZOS_EVENT                                                            ///< ʹ��ezos�¼�
#define __EZOS_MAILBOX                                                          ///< ʹ��ezos����
#define __EZOS_MESSAGE                                                          ///< ʹ��ezos��Ϣ����



#endif
/* END OF FILE ---------------------------------------------------------------*/
