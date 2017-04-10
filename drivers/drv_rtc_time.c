/**
  ******************************************************************************
  * @file    drv_rtc_time.c
  * @author  mousie
  * @version V2.0.1
  * @date    2015.04.13
  * @brief   RTC��ʼ��, TIME��غ���
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_rtc_time.h"

#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_RTC_TIME
  * @{
  */



/**
  ******************************************************************************
  * @addtogroup DRV_RTC_TIME_Configure
  ******************************************************************************
  * @{
  */
/// ��RTC��ʱ��ͨ��64��Ƶ����оƬ�����������������. �ɹ����ʱ�侫��ʹ��
//#define RTC_CLOCK_OUTPUT_ENABLE
/**
  * @}
  */



/** @defgroup DRV_RTC_TIME_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_RTC_TIME_Private_Function
  * @brief    ����˽�к���
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_RTC_TIME_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  RTC ʵʱʱ�ӳ�ʼ��
  * @param  none
  * @retval rvalue
  *   @arg  0, RTC��������, �����Ѿ���ʼ����, û�е���
  *   @arg  1, RTC�س�ʼ��, ����RTC֮ǰ������
  *   @arg  0xFF, RTC��ʼ��ʧ��, RTCδ������
  ******************************************************************************
  */
uint8_t rtc_init(void)
{
    uint8_t rvalue = 0xFF;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);                                                // RTC�ͺ������

    // ����ض��ı��Ĵ���RTC���ñ��(�üĴ���Ҳ��RTC��ع���), RTC���������Ҫ���³�ʼ��, ����RTC�Ѿ��ڹ�����
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        uint32_t i;
        BKP_DeInit();                                                           // ���ݼĴ���ģ�鸴λ
        RCC_LSEConfig(RCC_LSE_ON);                                              // ʹ��LSE(�����ⲿ����), 32.768KHz
        for (i=0; i<5000000; i++) {
            if (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == SET)                      // �ȴ�����
            {
                RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);                         // RTCʱ��Դ���ó�LSE
                RCC_RTCCLKCmd(ENABLE);
                RTC_WaitForSynchro();                                           // �ȴ�APB1ʱ�Ӻ�RTCʱ��ͬ��

                RTC_WaitForLastTask();                                          // ��д�Ĵ���ǰ, ȷ����һ�������Ѿ�����
                RTC_SetPrescaler(32767);                                        // ����RTC��Ƶ��, ʹRTCʱ��Ϊ1Hz
                RTC_WaitForLastTask();                                          // �ȴ��Ĵ���д�����

                BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);                       // ��󱸼Ĵ�����д��RTC���óɹ����
                rvalue = 1;
                break;
            }
        }
    }
    else
    {
        RTC_WaitForSynchro();                                                   // �ȴ�ʱ��ͬ��
        rvalue = 0;
    }

#ifdef RTC_CLOCK_OUTPUT_ENABLE
    BKP_TamperPinCmd(DISABLE);                                                  // ���������⹦��
    BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);                        // ��RTCʱ��64��Ƶ�������������������
#endif

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  ת��UNIXʱ���Ϊ����ʱ��
  * @param  unix_time, UNIXʱ���
  * @retval ����ֵ,   ����ʱ��ṹ��ָ��
  ******************************************************************************
  */
struct tm time_unix2calendar(time_t unix_time)
{
    struct tm *cal_time;
    cal_time = localtime(&unix_time);
    // time.h ת�������tm_year�����ֵ(����1900��), ��Ҫת���ճ�ֵ
    cal_time->tm_year += 1940;
    // time.h ת�������tm_mon 0��ʾ1�·�, ��Ҫ��һ�����ճ�ֵ
    cal_time->tm_mon++;

    return *cal_time;
}

/**
  ******************************************************************************
  * @brief  ת������ʱ��ΪUNIXʱ���
  * @param  cal_time, ����ʱ��
  * @retval ����ֵ,  UNIXʱ���
  ******************************************************************************
  */
time_t time_calendar2unix(struct tm cal_time)
{
    // time.h �ж������ݸ�ʽΪ1900�꿪ʼ�����, ʹ��time.h�еĺ���Ҫ�����תΪ����1900�꿪ʼ.
    cal_time.tm_year -= 1940;
    // time.h �ж���� tm_mon 0 ��Ӧ 1�·�, ��Ҫ��ʵ�ʵ��·ݼ�һ����.
    cal_time.tm_mon--;
    return mktime(&cal_time);
}

/**
  ******************************************************************************
  * @brief  ��RTCȡ��ǰʱ�������ʱ��(struct tm)
  * @param  none
  * @retval ����ֵ,   ����ʱ��ṹ��ָ��
  ******************************************************************************
  */
struct tm time_get_calendar(void)
{
    time_t unix_time;
    struct tm cal_time;

    unix_time = (time_t)RTC_GetCounter();
    cal_time  = time_unix2calendar(unix_time);
    return cal_time;
}

/**
  ******************************************************************************
  * @brief  ��RTCȡ��ǰʱ���Unixʱ���ֵ
  * @param  none
  * @retval ����ֵ,  UNIXʱ���
  ******************************************************************************
  */
time_t time_get_unix(void)
{
    return (time_t)RTC_GetCounter();
}

/**
  ******************************************************************************
  * @brief  ��������Calendar��ʽʱ��ת����UNIXʱ���д��RTC
  * @param  cal_time, ����ʱ��
  * @retval none
  ******************************************************************************
  */
void time_set_calendar(struct tm cal_time)
{
    time_set_unix(time_calendar2unix(cal_time));
    return;
}

/**
  ******************************************************************************
  * @brief  ��������Unixʱ���д��RTC
  * @param  unix_time, UNIXʱ���
  * @retval none
  ******************************************************************************
  */
void time_set_unix(time_t unix_time)
{
    RTC_WaitForLastTask();
    RTC_SetCounter((u32)unix_time);
    RTC_WaitForLastTask();
    return;
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
