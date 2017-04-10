/**
  ******************************************************************************
  * @file    drv_extint.c
  * @author  mousie
  * @version V1.0.0
  * @date    2011.12.22
  * @brief   外部中断驱动函数. 需要定义触发回调函数
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
  * @brief    私有类型定义
  * @{
  */
/// EXTINT硬件参数类型定义
typedef struct
{
    void (*callback)(void);                                                     ///< 外部中断触发回调函数
    GPIO_TypeDef *const PORT;                                                   ///< 外部中断PORT口
    const uint32_t CLK;                                                         ///< 外部中断PORT口时钟源
    const uint32_t LINE;                                                        ///< 外部中断LINE号
    const uint16_t PIN;                                                         ///< 外部中断PIN口
    const uint8_t  PORT_SOURCE;                                                 ///< 外部中断源引脚PORT配置
    const uint8_t  PIN_SOURCE;                                                  ///< 外部中断源引脚PIN 配置
    const uint8_t  IRQ;                                                         ///< 外部中断中断号
    const uint8_t  PRIORITY;                                                    ///< 外部中断中断优先级
} extint_hw_t;
/**
  * @}
  */

/** @defgroup DRV_ExtInt_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_ExtInt_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
/// EXTINT硬件参数映射表
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
  * @brief    定义私有函数
  * @{
  */
__INLINE static void extint_irq_handler(uint8_t idx);
/**
  * @}
  */



/** @defgroup DRV_ExtInt_Function
  * @brief    函数原文件
  * @{
  */
/**
  ******************************************************************************
  * @brief  外部中断初始化
  * @param  extint_name
  *   @arg  可用值请参考 extint_name_t 类型定义中的枚举值
  * @param  extint_trigger
  *   @arg  EXTI_Trigger_Rising,        上升沿触发
  *   @arg  EXTI_Trigger_Falling,       下降沿触发
  *   @arg  EXTI_Trigger_Rising_Falling,跳变沿触发
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

    // 使能 EXTINT 的IO口时钟
    RCC_APB2PeriphClockCmd(extint_hw[extint_name].CLK | RCC_APB2Periph_AFIO, ENABLE);

    // 配置外部中断引脚
    GPIO_InitStructure.GPIO_Pin = extint_hw[extint_name].PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(extint_hw[extint_name].PORT, &GPIO_InitStructure);

    // 将引脚关联到中断线
    GPIO_EXTILineConfig(extint_hw[extint_name].PORT_SOURCE, extint_hw[extint_name].PIN_SOURCE);

    // 配置中断触发类型
    EXTI_InitStructure.EXTI_Line = extint_hw[extint_name].LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = extint_trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = extint_hw[extint_name].IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (extint_hw[extint_name].PRIORITY & 0x0C) >> 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (extint_hw[extint_name].PRIORITY & 0x03);
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  ******************************************************************************
  * @brief  禁止某个extint.
  * @param  extint_name
  *   @arg  可用值请参考 extint_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void extint_deinit(extint_name_t extint_name)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    assert_param(extint_name < EXTINT_NUM);
    if (extint_name >= EXTINT_NUM) return;

    // 配置中断触发类型
    EXTI_InitStructure.EXTI_Line = extint_hw[extint_name].LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = extint_hw[extint_name].IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  ******************************************************************************
  * @brief  使用软件产生一次外部中断信号
  * @param  extint_name
  *   @arg  可用值请参考 extint_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void extint_software(extint_name_t extint_name)
{
    EXTI_GenerateSWInterrupt(extint_hw[extint_name].LINE);
}

/**
  ******************************************************************************
  * @brief  外部中断中断处理函数
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
