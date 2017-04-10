/**
  ******************************************************************************
  * @file    drv_extint.c
  * @author  mousie
  * @version V1.0.0
  * @date    2011.12.22
  * @brief   �ⲿ�ж���������. ��Ҫ���崥���ص�����
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_extint.h"



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_ExtInt
  * @{
  */
#if (EXTINT_NUM > 0)



/**
  ******************************************************************************
  * @addtogroup DRV_ExtInt_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_ExtInt_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */
/// EXTINTӲ���������Ͷ���
typedef struct
{
    void (*callback)(void);                                                     ///< �ⲿ�жϴ����ص�����
    GPIO_TypeDef *const PORT;                                                   ///< �ⲿ�ж�PORT��
    const uint32_t CLK;                                                         ///< �ⲿ�ж�PORT��ʱ��Դ
    const uint32_t LINE;                                                        ///< �ⲿ�ж�LINE��
    const uint16_t PIN;                                                         ///< �ⲿ�ж�PIN��
    const uint8_t  PORT_SOURCE;                                                 ///< �ⲿ�ж�Դ����PORT����
    const uint8_t  PIN_SOURCE;                                                  ///< �ⲿ�ж�Դ����PIN ����
    const uint8_t  IRQ;                                                         ///< �ⲿ�ж��жϺ�
    const uint8_t  PRIORITY;                                                    ///< �ⲿ�ж��ж����ȼ�
} extint_hw_t;
/**
  * @}
  */

/** @defgroup DRV_ExtInt_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_ExtInt_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
/// EXTINTӲ������ӳ���
static const extint_hw_t extint_hw[EXTINT_NUM] = {
#if (EXTINT_NUM > 0)
    {EXTINT0_CALLBACK,
     EXTINT0_PORT,          EXTINT0_CLK,
     EXTINT0_LINE,          EXTINT0_PIN,
     EXTINT0_PORT_SOURCE,   EXTINT0_PIN_SOURCE,
     EXTINT0_IRQ,           EXTINT0_IRQ_PRIORITY},
#endif

#if (EXTINT_NUM > 1)
    {EXTINT1_CALLBACK,
     EXTINT1_PORT,          EXTINT1_CLK,
     EXTINT1_LINE,          EXTINT1_PIN,
     EXTINT1_PORT_SOURCE,   EXTINT1_PIN_SOURCE,
     EXTINT1_IRQ,           EXTINT1_IRQ_PRIORITY},
#endif

#if (EXTINT_NUM > 2)
    {EXTINT2_CALLBACK,
     EXTINT2_PORT,          EXTINT2_CLK,
     EXTINT2_LINE,          EXTINT2_PIN,
     EXTINT2_PORT_SOURCE,   EXTINT2_PIN_SOURCE,
     EXTINT2_IRQ,           EXTINT2_IRQ_PRIORITY},
#endif

#if (EXTINT_NUM > 3)
    {EXTINT3_CALLBACK,
     EXTINT3_PORT,          EXTINT3_CLK,
     EXTINT3_LINE,          EXTINT3_PIN,
     EXTINT3_PORT_SOURCE,   EXTINT3_PIN_SOURCE,
     EXTINT3_IRQ,           EXTINT3_IRQ_PRIORITY},
#endif

#if (EXTINT_NUM > 4)
    {EXTINT4_CALLBACK,
     EXTINT4_PORT,          EXTINT4_CLK,
     EXTINT4_LINE,          EXTINT4_PIN,
     EXTINT4_PORT_SOURCE,   EXTINT4_PIN_SOURCE,
     EXTINT4_IRQ,           EXTINT4_IRQ_PRIORITY},
#endif
};
/**
  * @}
  */

/** @defgroup DRV_ExtInt_Private_Function
  * @brief    ����˽�к���
  * @{
  */
__INLINE static void extint_irq_handler(uint8_t idx);
/**
  * @}
  */



/** @defgroup DRV_ExtInt_Function
  * @brief    ����ԭ�ļ�
  * @{
  */
/**
  ******************************************************************************
  * @brief  �ⲿ�жϳ�ʼ��
  * @param  extint_name
  *   @arg  ����ֵ��ο� extint_name_t ���Ͷ����е�ö��ֵ
  * @param  extint_trigger
  *   @arg  EXTI_Trigger_Rising,        �����ش���
  *   @arg  EXTI_Trigger_Falling,       �½��ش���
  *   @arg  EXTI_Trigger_Rising_Falling,�����ش���
  * @retval none
  ******************************************************************************
  */
void extint_init(extint_name_t extint_name, EXTITrigger_TypeDef extint_trigger)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    assert_param(extint_name < EXTINT_NUM);
    assert_param(IS_EXTI_TRIGGER(extint_trigger));
    if (extint_name >= EXTINT_NUM) return;

    // ʹ�� EXTINT ��IO��ʱ��
    RCC_APB2PeriphClockCmd(extint_hw[extint_name].CLK | RCC_APB2Periph_AFIO, ENABLE);

    // �����ⲿ�ж�����
    GPIO_InitStructure.GPIO_Pin = extint_hw[extint_name].PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(extint_hw[extint_name].PORT, &GPIO_InitStructure);

    // �����Ź������ж���
    GPIO_EXTILineConfig(extint_hw[extint_name].PORT_SOURCE, extint_hw[extint_name].PIN_SOURCE);

    // �����жϴ�������
    EXTI_InitStructure.EXTI_Line = extint_hw[extint_name].LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = extint_trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // ����NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = extint_hw[extint_name].IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (extint_hw[extint_name].PRIORITY & 0x0C) >> 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (extint_hw[extint_name].PRIORITY & 0x03);
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  ******************************************************************************
  * @brief  ��ֹĳ��extint.
  * @param  extint_name
  *   @arg  ����ֵ��ο� extint_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void extint_deinit(extint_name_t extint_name)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    assert_param(extint_name < EXTINT_NUM);
    if (extint_name >= EXTINT_NUM) return;

    // �����жϴ�������
    EXTI_InitStructure.EXTI_Line = extint_hw[extint_name].LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);

    // ����NVIC
    NVIC_InitStructure.NVIC_IRQChannel = extint_hw[extint_name].IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  ******************************************************************************
  * @brief  ʹ���������һ���ⲿ�ж��ź�
  * @param  extint_name
  *   @arg  ����ֵ��ο� extint_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void extint_software(extint_name_t extint_name)
{
    EXTI_GenerateSWInterrupt(extint_hw[extint_name].LINE);
}

/**
  ******************************************************************************
  * @brief  �ⲿ�ж��жϴ�����
  * @param  none
  * @retval none
  ******************************************************************************
*/
__INLINE static void extint_irq_handler(uint8_t idx)
{
    if(EXTI_GetITStatus(extint_hw[idx].LINE) != RESET)
    {
        if (extint_hw[idx].callback != 0) extint_hw[idx].callback();
        EXTI_ClearITPendingBit(extint_hw[idx].LINE);
    }
}

#if (EXTINT_NUM > 0)
void EXTINT0_IRQ_HANDLER(void)
{
    extint_irq_handler(0);
}
#endif

#if (EXTINT_NUM > 1)
void EXTINT1_IRQ_HANDLER(void)
{
    extint_irq_handler(1);
}
#endif

#if (EXTINT_NUM > 2)
void EXTINT2_IRQ_HANDLER(void)
{
    extint_irq_handler(2);
}
#endif

#if (EXTINT_NUM > 3)
void EXTINT3_IRQ_HANDLER(void)
{
    extint_irq_handler(3);
}
#endif

#if (EXTINT_NUM > 4)
void EXTINT4_IRQ_HANDLER(void)
{
    extint_irq_handler(4);
}
#endif

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
