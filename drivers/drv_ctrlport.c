/**
  ******************************************************************************
  * @file    drv_ctrlport.c
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.15
  * @brief   控制引脚驱动程序. 如电源开关, 信号开关.
  *          如果外设芯片上有控制开关的引脚, 则该引脚应该归入芯片驱动代码.
  *          此驱动的控制引脚一般是通过三极管, MOS管等分立元件达到控制目的.
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
  * @brief    私有类型定义
  * @{
  */
/// 控制引脚硬件参数类型定义
typedef struct
{
    GPIO_TypeDef *const PORT;                                                   ///< 控制引脚的PORT口
    const uint32_t CLK;                                                         ///< 控制引脚的时钟模块
    const uint16_t PIN;                                                         ///< 控制引脚的PIN口
    const uint8_t  OFF;                                                         ///< 控制引脚的开关电平
} ctrlport_hw_t;
/**
  * @}
  */

/** @defgroup DRV_CtrlPort_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_CtrlPort_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
/// 控制引脚硬件参数映射表
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
  * @brief    定义私有函数
  * @{
  */
/// 控制引脚IO口初始化, 配置成推挽输出
__INLINE static void ctrlport_port_init(ctrlport_name_t ctrlport_name)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(ctrlport_hw[ctrlport_name].CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = ctrlport_hw[ctrlport_name].PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                            // IO口设为推拉输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;                            // 输出频率最大2MHz,开关控制应用足矣
    GPIO_Init(ctrlport_hw[ctrlport_name].PORT, &GPIO_InitStructure);            // 初始化该控制引脚
}

/// 控制引脚IO口电平置高
__INLINE static void CTRLPORT_HIGH(ctrlport_name_t ctrlport_name)
{
    ctrlport_hw[ctrlport_name].PORT->BSRR = ctrlport_hw[ctrlport_name].PIN;
}

/// 控制引脚IO口电平置低
__INLINE static void CTRLPORT_LOW(ctrlport_name_t ctrlport_name)
{
    ctrlport_hw[ctrlport_name].PORT->BRR  = ctrlport_hw[ctrlport_name].PIN;
}

/// 控制引脚IO口电平翻转
__INLINE static void CTRLPORT_TOGGLE(ctrlport_name_t ctrlport_name)
{
    ctrlport_hw[ctrlport_name].PORT->ODR ^= ctrlport_hw[ctrlport_name].PIN;
}

/// 读取控制引脚IO口电平值
__INLINE static uint32_t CTRLPORT_LEVEL(ctrlport_name_t ctrlport_name)
{
    return (ctrlport_hw[ctrlport_name].PORT->ODR & ctrlport_hw[ctrlport_name].PIN);
}
/**
  * @}
  */



/** @defgroup DRV_CtrlPort_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  初始化所有的控制引脚, 初始化后全部处于关闭状态.
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
  * @brief  控制引脚初始化, 初始化后处于关闭状态.
  * @param  ctrlport_name
  *   @arg  可用值请参考 ctrlport_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void ctrlport_init(ctrlport_name_t ctrlport_name)
{
    assert_param(ctrlport_name < CTRLPORT_NUM);
    if (ctrlport_name >= CTRLPORT_NUM) return;

    ctrlport_port_init(ctrlport_name);
    ctrlport_off(ctrlport_name);                                                // 初始化关闭该控制引脚
}

/**
  ******************************************************************************
  * @brief  打开控制引脚
  * @param  ctrlport_name
  *   @arg  可用值请参考 ctrlport_name_t 类型定义中的枚举值
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
  * @brief  关闭控制引脚
  * @param  ctrlport_name
  *   @arg  可用值请参考 ctrlport_name_t 类型定义中的枚举值
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
  * @brief  翻转控制引脚
  * @param  ctrlport_name
  *   @arg  可用值请参考 ctrlport_name_t 类型定义中的枚举值
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
  * @brief  查询控制口当前状态
  * @param  ctrlport_name
  *   @arg  可用值请参考 ctrlport_name_t 类型定义中的枚举值
  * @retval status, 控制引脚状态
  *   @arg  0, 控制口逻辑关闭状态
  *   @arg  1, 控制口逻辑打开状态
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
