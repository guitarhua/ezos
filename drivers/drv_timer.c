/**
  ******************************************************************************
  * @file    drv_timer.c
  * @author  mousie
  * @version V1.0.0
  * @date    2012.2.18
  * @brief   Ӳ����ʱ������, ���ṩ������Ķ�ʱ����жϹ���.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_timer.h"



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_Timer
  * @{
  */
#if (TIMER_NUM > 0)



/**
  ******************************************************************************
  * @addtogroup DRV_Timer_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_Timer_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */
/// TimerӲ���������Ͷ���
typedef struct
{
    const uint32_t CLK;                                                         ///< Timerʱ��ģ��
    TIM_TypeDef *const PERIPHERAL;                                              ///< Timer������
    void (*overtime_callback)(void);                                            ///< Timer����жϻص�����
    const uint8_t  IRQ;                                                         ///< Timer�жϺ�
    const uint8_t  PRIORITY;                                                    ///< Timer�ж����ȼ�
} timer_hw_t;
/**
  * @}
  */

/** @defgroup DRV_Timer_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Timer_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
/// TimerӲ������ӳ���
static const timer_hw_t timer_hw[TIMER_NUM] = {
#if (TIMER_NUM > 0)
    {TIMER0_CLK, TIMER0_PERIPHERAL,
     TIMER0_OVERTIME_CALLBACK,
     TIMER0_IRQ, TIMER0_IRQ_PRIORITY},
#endif

#if (TIMER_NUM > 1)
    {TIMER1_CLK, TIMER1_PERIPHERAL,
     TIMER1_OVERTIME_CALLBACK,
     TIMER1_IRQ, TIMER1_IRQ_PRIORITY},
#endif

#if (TIMER_NUM > 2)
    {TIMER2_CLK, TIMER2_PERIPHERAL,
     TIMER2_OVERTIME_CALLBACK,
     TIMER2_IRQ, TIMER2_IRQ_PRIORITY},
#endif
};
/**
  * @}
  */

/** @defgroup DRV_Timer_Private_Function
  * @brief    ����˽�к���
  * @{
  */
__INLINE static void timer_irq_handler(uint8_t idx);
/**
  * @}
  */



/** @defgroup DRV_Timer_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  Timer��ʼ��. ��ʱʱ�� T(us) = prescaler * period / 72. ע��, T>2us��Ч
  * @param  timer_name
  *   @arg  ����ֵ��ο� timer_name_t ���Ͷ����е�ö��ֵ
  * @param  prescaler, ����timer��Ԥ��Ƶֵ.
  * @param  period,    ����timer�ķ�Ƶ��.
  * @retval none
  ******************************************************************************
  */
void timer_init(timer_name_t timer_name, uint16_t prescaler, uint16_t period)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    TIM_TypeDef *timer = timer_hw[timer_name].PERIPHERAL;

    assert_param(timer_name < TIMER_NUM);
    assert_param(prescaler > 0);
    assert_param(period > 0);
    if (timer_name >= TIMER_NUM) return;

    RCC_APB1PeriphClockCmd(timer_hw[timer_name].CLK, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = timer_hw[timer_name].IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (timer_hw[timer_name].PRIORITY & 0x0C) >> 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (timer_hw[timer_name].PRIORITY & 0x03);
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /**
      * ��ʱʱ��T���㹫ʽ:
      * T = (TIM_Period+1) * (TIM_Prescaler+1) / TIMxCLK
      * ����ϵͳĬ������ʱ TIMxCLK ��ֵΪ72Mhz.
      */
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.TIM_Period = period - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(timer, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(timer, DISABLE);                                       // �������͵�Ӱ�ӼĴ���
    TIM_ClearFlag(timer, TIM_FLAG_Update);                                      // ������±�־λ
    TIM_ITConfig(timer, TIM_IT_Update, ENABLE);                                 // ʹ������ж�
    TIM_Cmd(timer, ENABLE);
}

/**
  ******************************************************************************
  * @brief  ��ֹĳ��Timer. ����Timer�Ĵ����ָ�ΪĬ��ֵ.
  * @param  timer_name
  *   @arg  ����ֵ��ο� timer_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void timer_deinit(timer_name_t timer_name)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    TIM_TypeDef *timer = timer_hw[timer_name].PERIPHERAL;

    assert_param(timer_name < TIMER_NUM);
    if (timer_name >= TIMER_NUM) return;

    TIM_Cmd(timer, DISABLE);
    TIM_DeInit(timer);

    RCC_APB1PeriphClockCmd(timer_hw[timer_name].CLK, DISABLE);

    NVIC_InitStructure.NVIC_IRQChannel = timer_hw[timer_name].IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  ******************************************************************************
  * @brief  Timer������жϴ�����
  * @param  none
  * @retval none
  ******************************************************************************
*/
__INLINE static void timer_irq_handler(uint8_t idx)
{
    TIM_TypeDef *timer = timer_hw[idx].PERIPHERAL;

    if (TIM_GetITStatus(timer, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(timer, TIM_IT_Update);
        if (timer_hw[idx].overtime_callback != 0) timer_hw[idx].overtime_callback();
    }
}

#if (TIMER_NUM > 0)
void TIMER0_IRQ_HANDLER(void)
{
    timer_irq_handler(0);
}
#endif

#if (TIMER_NUM > 1)
void TIMER1_IRQ_HANDLER(void)
{
    timer_irq_handler(1);
}
#endif

#if (TIMER_NUM > 2)
void TIMER2_IRQ_HANDLER(void)
{
    timer_irq_handler(2);
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
