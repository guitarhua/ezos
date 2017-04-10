/**
  ******************************************************************************
  * @file    cpn_volume.c
  * @author  mousie
  * @version V1.0.0
  * @date    2014.8.6
  * @brief   ���׵�������������. ������.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cpn_volume.h"



/** @addtogroup Components
  * @{
  */
/** @defgroup CPN_Volume
  * @{
  */
#ifdef VOLUME_ENABLE



/**
  ******************************************************************************
  * @addtogroup CPN_Volume_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup CPN_Volume_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Volume_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup CPN_Volume_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
/// ����������������Ӳ������ӳ���
static const ctrlport_name_t volume_io[VOLUME_ENABLE] = {
#if (VOLUME_ENABLE > 0)
    VOLUME_IO0,
#endif
#if (VOLUME_ENABLE > 1)
    VOLUME_IO1,
#endif
#if (VOLUME_ENABLE > 2)
    VOLUME_IO2,
#endif
#if (VOLUME_ENABLE > 3)
    VOLUME_IO3,
#endif
};

static uint8_t ringVolume;                                                      // ��������
/**
  * @}
  */

/** @defgroup CPN_Volume_Private_Function
  * @brief    ����˽�к���
  * @{
  */

/**
  * @}
  */



/** @defgroup CPN_Volume_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  ����������ʼ��
  * @param  none
  * @retval none
  ******************************************************************************
  */
void volume_init(void)
{
    int8_t i;

    for (i=0; i<RING_VOLUME_IDX; i++)
    {
        ctrlport_init(volume_io[i]);
        ctrlport_off(volume_io[i]);
    }
    ringVolume = 1;
}

/**
  ******************************************************************************
  * @brief  ������������
  * @param  volume, ����ֵ.
  * @retval none
  ******************************************************************************
  */
void volume_set(uint8_t volume)
{
    int8_t i;
    uint8_t max= 0x01 << RING_VOLUME_IDX;
    if (volume>max) volume = max;

    ringVolume = volume;
    if (volume)
    {
        volume--;
        for (i=0; i<RING_VOLUME_IDX; i++)
        {
            if (volume & (0x01 << i))
                ctrlport_on(volume_io[i]);
            else
                ctrlport_off(volume_io[i]);
        }
    }
    else
    {
        for (i=0; i<RING_VOLUME_IDX; i++)
            ctrlport_off(volume_io[i]);
    }
}

/**
  ******************************************************************************
  * @brief  ��ȡ��������
  * @param  none
  * @retval ����ֵ, ����ֵ.
  ******************************************************************************
  */
uint8_t ring_get_volume(void)
{
    return ringVolume;
}
/**
  * @}
  */



#endif
/**
  * @}
  */
/**
  * @}
  */

/* END OF FILE ---------------------------------------------------------------*/
