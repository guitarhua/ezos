/**
  ******************************************************************************
  * @file    drv_ctrlport.c
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.15
  * @brief   ����������������. ���Դ����, �źſ���.
  *          �������оƬ���п��ƿ��ص�����, �������Ӧ�ù���оƬ��������.
  *          �������Ŀ�������һ����ͨ��������, MOS�ܵȷ���Ԫ���ﵽ����Ŀ��.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_ctrlport.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_CtrlPort
  * @{
  */
#if (CTRLPORT_NUM > 0)



/**
  ******************************************************************************
  * @addtogroup DRV_CtrlPort_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_CtrlPort_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */
/// ��������Ӳ���������Ͷ���
typedef struct
{
    GPIO_TypeDef *const PORT;                                                   ///< �������ŵ�PORT��
    const uint32_t CLK;                                                         ///< �������ŵ�ʱ��ģ��
    const uint16_t PIN;                                                         ///< �������ŵ�PIN��
    const uint8_t  OFF;                                                         ///< �������ŵĿ��ص�ƽ
} ctrlport_hw_t;
/**
  * @}
  */

/** @defgroup DRV_CtrlPort_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_CtrlPort_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
/// ��������Ӳ������ӳ���
static const ctrlport_hw_t ctrlport_hw[CTRLPORT_NUM] = {
#if (CTRLPORT_NUM > 0)
    {CTRLPORT0_PORT,  CTRLPORT0_CLK,  CTRLPORT0_PIN,  CTRLPORT0_OFF },
#endif
#if (CTRLPORT_NUM > 1)
    {CTRLPORT1_PORT,  CTRLPORT1_CLK,  CTRLPORT1_PIN,  CTRLPORT1_OFF },
#endif
#if (CTRLPORT_NUM > 2)
    {CTRLPORT2_PORT,  CTRLPORT2_CLK,  CTRLPORT2_PIN,  CTRLPORT2_OFF },
#endif
#if (CTRLPORT_NUM > 3)
    {CTRLPORT3_PORT,  CTRLPORT3_CLK,  CTRLPORT3_PIN,  CTRLPORT3_OFF },
#endif
#if (CTRLPORT_NUM > 4)
    {CTRLPORT4_PORT,  CTRLPORT4_CLK,  CTRLPORT4_PIN,  CTRLPORT4_OFF },
#endif
#if (CTRLPORT_NUM > 5)
    {CTRLPORT5_PORT,  CTRLPORT5_CLK,  CTRLPORT5_PIN,  CTRLPORT5_OFF },
#endif
#if (CTRLPORT_NUM > 6)
    {CTRLPORT6_PORT,  CTRLPORT6_CLK,  CTRLPORT6_PIN,  CTRLPORT6_OFF },
#endif
#if (CTRLPORT_NUM > 7)
    {CTRLPORT7_PORT,  CTRLPORT7_CLK,  CTRLPORT7_PIN,  CTRLPORT7_OFF },
#endif
#if (CTRLPORT_NUM > 8)
    {CTRLPORT8_PORT,  CTRLPORT8_CLK,  CTRLPORT8_PIN,  CTRLPORT8_OFF },
#endif
#if (CTRLPORT_NUM > 9)
    {CTRLPORT9_PORT,  CTRLPORT9_CLK,  CTRLPORT9_PIN,  CTRLPORT9_OFF },
#endif
#if (CTRLPORT_NUM > 10)
    {CTRLPORT10_PORT, CTRLPORT10_CLK, CTRLPORT10_PIN, CTRLPORT10_OFF},
#endif
#if (CTRLPORT_NUM > 11)
    {CTRLPORT11_PORT, CTRLPORT11_CLK, CTRLPORT11_PIN, CTRLPORT11_OFF},
#endif
#if (CTRLPORT_NUM > 12)
    {CTRLPORT12_PORT, CTRLPORT12_CLK, CTRLPORT12_PIN, CTRLPORT12_OFF},
#endif
#if (CTRLPORT_NUM > 13)
    {CTRLPORT13_PORT, CTRLPORT13_CLK, CTRLPORT13_PIN, CTRLPORT13_OFF},
#endif
#if (CTRLPORT_NUM > 14)
    {CTRLPORT14_PORT, CTRLPORT14_CLK, CTRLPORT14_PIN, CTRLPORT14_OFF},
#endif
#if (CTRLPORT_NUM > 15)
    {CTRLPORT15_PORT, CTRLPORT15_CLK, CTRLPORT15_PIN, CTRLPORT15_OFF},
#endif
};
/**
  * @}
  */

/** @defgroup DRV_CtrlPort_Private_Function
  * @brief    ����˽�к���
  * @{
  */
/// ��������IO�ڳ�ʼ��, ���ó��������
__INLINE static void ctrlport_port_init(ctrlport_name_t ctrlport_name)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(ctrlport_hw[ctrlport_name].CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = ctrlport_hw[ctrlport_name].PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                            // IO����Ϊ�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;                            // ���Ƶ�����2MHz,���ؿ���Ӧ������
    GPIO_Init(ctrlport_hw[ctrlport_name].PORT, &GPIO_InitStructure);            // ��ʼ���ÿ�������
}

/// ��������IO�ڵ�ƽ�ø�
__INLINE static void CTRLPORT_HIGH(ctrlport_name_t ctrlport_name)
{
    ctrlport_hw[ctrlport_name].PORT->BSRR = ctrlport_hw[ctrlport_name].PIN;
}

/// ��������IO�ڵ�ƽ�õ�
__INLINE static void CTRLPORT_LOW(ctrlport_name_t ctrlport_name)
{
    ctrlport_hw[ctrlport_name].PORT->BRR  = ctrlport_hw[ctrlport_name].PIN;
}

/// ��������IO�ڵ�ƽ��ת
__INLINE static void CTRLPORT_TOGGLE(ctrlport_name_t ctrlport_name)
{
    ctrlport_hw[ctrlport_name].PORT->ODR ^= ctrlport_hw[ctrlport_name].PIN;
}

/// ��ȡ��������IO�ڵ�ƽֵ
__INLINE static uint32_t CTRLPORT_LEVEL(ctrlport_name_t ctrlport_name)
{
    return (ctrlport_hw[ctrlport_name].PORT->ODR & ctrlport_hw[ctrlport_name].PIN);
}
/**
  * @}
  */



/** @defgroup DRV_CtrlPort_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  ��ʼ�����еĿ�������, ��ʼ����ȫ�����ڹر�״̬.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void ctrlport_init_all(void)
{
    uint8_t i;

    for (i=0; i < CTRLPORT_NUM; i++)
        ctrlport_init((ctrlport_name_t)(i));
}

/**
  ******************************************************************************
  * @brief  �������ų�ʼ��, ��ʼ�����ڹر�״̬.
  * @param  ctrlport_name
  *   @arg  ����ֵ��ο� ctrlport_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void ctrlport_init(ctrlport_name_t ctrlport_name)
{
    assert_param(ctrlport_name < CTRLPORT_NUM);
    if (ctrlport_name >= CTRLPORT_NUM) return;

    ctrlport_port_init(ctrlport_name);
    ctrlport_off(ctrlport_name);                                                // ��ʼ���رոÿ�������
}

/**
  ******************************************************************************
  * @brief  �򿪿�������
  * @param  ctrlport_name
  *   @arg  ����ֵ��ο� ctrlport_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void ctrlport_on(ctrlport_name_t ctrlport_name)
{
    assert_param(ctrlport_name < CTRLPORT_NUM);
    if (ctrlport_name >= CTRLPORT_NUM) return;

    if (ctrlport_hw[ctrlport_name].OFF == 0)
        CTRLPORT_HIGH(ctrlport_name);
    else
        CTRLPORT_LOW(ctrlport_name);
}

/**
  ******************************************************************************
  * @brief  �رտ�������
  * @param  ctrlport_name
  *   @arg  ����ֵ��ο� ctrlport_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void ctrlport_off(ctrlport_name_t ctrlport_name)
{
    assert_param(ctrlport_name < CTRLPORT_NUM);
    if (ctrlport_name >= CTRLPORT_NUM) return;

    if (ctrlport_hw[ctrlport_name].OFF == 0)
        CTRLPORT_LOW(ctrlport_name);
    else
        CTRLPORT_HIGH(ctrlport_name);
}

/**
  ******************************************************************************
  * @brief  ��ת��������
  * @param  ctrlport_name
  *   @arg  ����ֵ��ο� ctrlport_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void ctrlport_toggle(ctrlport_name_t ctrlport_name)
{
    assert_param(ctrlport_name < CTRLPORT_NUM);
    if (ctrlport_name >= CTRLPORT_NUM) return;

    CTRLPORT_TOGGLE(ctrlport_name);
}

/**
  ******************************************************************************
  * @brief  ��ѯ���ƿڵ�ǰ״̬
  * @param  ctrlport_name
  *   @arg  ����ֵ��ο� ctrlport_name_t ���Ͷ����е�ö��ֵ
  * @retval status, ��������״̬
  *   @arg  0, ���ƿ��߼��ر�״̬
  *   @arg  1, ���ƿ��߼���״̬
  ******************************************************************************
  */
uint8_t ctrlport_get_status(ctrlport_name_t ctrlport_name)
{
    assert_param(ctrlport_name < CTRLPORT_NUM);
    if (ctrlport_name >= CTRLPORT_NUM) return 0;

    if (ctrlport_hw[ctrlport_name].OFF == 0)
        return (CTRLPORT_LEVEL(ctrlport_name) != 0);
    else
        return (CTRLPORT_LEVEL(ctrlport_name) == 0);
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
