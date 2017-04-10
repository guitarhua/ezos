/**
  ******************************************************************************
  * @file    drv_timer.c
  * @author  mousie
  * @version V1.0.0
  * @date    2012.2.18
  * @brief   硬件定时器驱动, 仅提供最基本的定时溢出中断功能.
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
  * @brief    私有类型定义
  * @{
  */
/// Timer硬件参数类型定义
typedef struct
{
    const uint32_t CLK;                                                         ///< Timer时钟模块
    TIM_TypeDef *const PERIPHERAL;                                              ///< Timer外设名
    void (*overtime_callback)(void);                                            ///< Timer溢出中断回调函数
    const uint8_t  IRQ;                                                         ///< Timer中断号
    const uint8_t  PRIORITY;                                                    ///< Timer中断优先级
} timer_hw_t;
/**
  * @}
  */

/** @defgroup DRV_Timer_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Timer_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
/// Timer硬件参数映射表
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
  * @brief    定义私有函数
  * @{
  */
__INLINE static void timer_irq_handler(uint8_t idx);
/**
  * @}
  */



/** @defgroup DRV_Timer_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  Timer初始化. 定时时间 T(us) = prescaler * period / 72. 注意, T>2us有效
  * @param  timer_name
  *   @arg  可用值请参考 timer_name_t 类型定义中的枚举值
  * @param  prescaler, 设置timer的预分频值.
  * @param  period,    设置timer的分频数.
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
      * 定时时间T计算公式:
      * T = (TIM_Period+1) * (TIM_Prescaler+1) / TIMxCLK
      * 采用系统默认配置时 TIMxCLK 的值为72Mhz.
      */
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.TIM_Period = period - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(timer, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(timer, DISABLE);                                       // 立即传送到影子寄存器
    TIM_ClearFlag(timer, TIM_FLAG_Update);                                      // 清除更新标志位
    TIM_ITConfig(timer, TIM_IT_Update, ENABLE);                                 // 使能溢出中断
    TIM_Cmd(timer, ENABLE);
}

/**
  ******************************************************************************
  * @brief  禁止某个Timer. 并将Timer寄存器恢复为默认值.
  * @param  timer_name
  *   @arg  可用值请参考 timer_name_t 类型定义中的枚举值
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
  * @brief  Timer的溢出中断处理函数
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
