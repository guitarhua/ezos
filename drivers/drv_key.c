/**
  ******************************************************************************
  * @file    drv_key.c
  * @author  mousie
  * @version V2.1.1
  * @date    2015.4.21
  * @brief   直连式按键驱动程序.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_key.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_Key
  * @{
  */
#if (KEY_NUM > 0)



/**
  ******************************************************************************
  * @addtogroup DRV_Key_Configure
  ******************************************************************************
  * @{
  */
#define LONG_PRESS_TIME                 130                                     ///< 长按触发时间设置, 单位10ms, 经测试使用1.3S较好
#define RUNNING_PRESS_TIME              20                                      ///< 连发时间间隔设置, 单位10ms, 经测试使用0.2s较好
/**
  * @}
  */



/** @defgroup DRV_Key_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */
/// 按键硬件参数类型定义
typedef struct
{
    GPIO_TypeDef *const PORT;                                                   ///< 按键的PORT口
    const uint32_t CLK;                                                         ///< 按键的时钟模块
    const uint16_t PIN;                                                         ///< 按键的PIN口
} key_hw_t;
/**
  * @}
  */

/** @defgroup DRV_Key_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Key_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
/// 按键硬件参数映射表
static const key_hw_t key_hw[KEY_NUM] = {
#if (KEY_NUM > 0)
    {KEY0_PORT,  KEY0_CLK,  KEY0_PIN},
#endif
#if (KEY_NUM > 1)
    {KEY1_PORT,  KEY1_CLK,  KEY1_PIN},
#endif
#if (KEY_NUM > 2)
    {KEY2_PORT,  KEY2_CLK,  KEY2_PIN},
#endif
#if (KEY_NUM > 3)
    {KEY3_PORT,  KEY3_CLK,  KEY3_PIN},
#endif
#if (KEY_NUM > 4)
    {KEY4_PORT,  KEY4_CLK,  KEY4_PIN},
#endif
#if (KEY_NUM > 5)
    {KEY5_PORT,  KEY5_CLK,  KEY5_PIN},
#endif
#if (KEY_NUM > 6)
    {KEY6_PORT,  KEY6_CLK,  KEY6_PIN},
#endif
#if (KEY_NUM > 7)
    {KEY7_PORT,  KEY7_CLK,  KEY7_PIN},
#endif
#if (KEY_NUM > 8)
    {KEY8_PORT,  KEY8_CLK,  KEY8_PIN},
#endif
#if (KEY_NUM > 9)
    {KEY9_PORT,  KEY9_CLK,  KEY9_PIN},
#endif
#if (KEY_NUM > 10)
    {KEY10_PORT, KEY10_CLK, KEY10_PIN},
#endif
#if (KEY_NUM > 11)
    {KEY11_PORT, KEY11_CLK, KEY11_PIN},
#endif
#if (KEY_NUM > 12)
    {KEY12_PORT, KEY12_CLK, KEY12_PIN},
#endif
#if (KEY_NUM > 13)
    {KEY13_PORT, KEY13_CLK, KEY13_PIN},
#endif
#if (KEY_NUM > 14)
    {KEY14_PORT, KEY14_CLK, KEY14_PIN},
#endif
#if (KEY_NUM > 15)
    {KEY15_PORT, KEY15_CLK, KEY15_PIN},
#endif
};

volatile key_status_t key_status = {0, 0, 0, 0, 0};                             ///< 按键状态变量
/**
  * @}
  */

/** @defgroup DRV_Key_Private_Function
  * @brief    定义私有函数
  * @{
  */
/// 按键IO口初始化, 配置成上拉输入 或 高阻输入
__INLINE static void key_port_init(key_name_t key_name)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(key_hw[key_name].CLK, ENABLE);                       // 使能按键端口的时钟
    GPIO_InitStructure.GPIO_Pin  = key_hw[key_name].PIN;                        // 设置按键端口的PIN
#ifdef KEY_EXTPULL
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                       // 有外部上拉电阻, IO口设为高阻输入
#else
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               // 无外部上拉电阻, IO口设为上拉输入
#endif
    GPIO_Init(key_hw[key_name].PORT, &GPIO_InitStructure);                      // 初始化按键IO口
}

/// 获取按键IO口当前电平值. 0, 低电平. !0, 高电平
__INLINE static uint32_t KEY_PORT_LEVEL(key_name_t key_name)
{
    return (key_hw[key_name].PORT->IDR & key_hw[key_name].PIN);
}
/**
  * @}
  */



/** @defgroup DRV_Key_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  初始化所有的按键端口
  * @param  none
  * @retval none
  ******************************************************************************
  */
void key_init_all(void)
{
    uint8_t i;

    for (i=0; i<KEY_NUM; i++)
        key_init((key_name_t)i);
}

/**
  ******************************************************************************
  * @brief  按键初始化函数
  * @param  key_name
  *     @arg 可用值请参考 key_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void key_init(key_name_t key_name)
{
    assert_param(key_name < KEY_NUM);
    if (key_name >= KEY_NUM) return;

    key_port_init(key_name);
}

/**
  ******************************************************************************
  * @brief  按键10ms扫描函数, 按键检测必须要用此函数.
  *         10ms定时调用该函数即可.时间要求不严格, 无需放在定时中断中.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void key_scan_10ms(void)
{
    uint8_t bit, i;
    static uint32_t key_state = 0;
    static uint32_t key_status_old = 0;
    static uint32_t temp_time;

    key_status.on_off = 0x0000;
    for (i=0; i<KEY_NUM; i++)
    {
        bit = !KEY_PORT_LEVEL((key_name_t)i);                                   // 获取按键当前电平值
        key_status.on_off |= (bit << i);                                        // 提取当前按键状态
    }

    switch(key_state)
    {
    case 0:                                                                     // 无按键状态
        if (key_status.on_off) key_state = 1;                                   // 有按键, 到按键确认状态
        key_status.on_off  = 0x00;                                              // 不能让主程序认为按键已经按下
        key_status.touch   = 0x00;
        key_status.lclick  = 0x00;
        key_status.lpress  = 0x00;
        key_status.click = 0x00;
        key_status_old = 0x00;
        break;
    case 1:                                                                     // 按键确认按下状态
        if (key_status.on_off)                                                  // 确认了是有按键
        {
            // 赋值按键值
            key_status.touch |= (key_status.on_off ^ key_status_old) & key_status.on_off;
            key_status.touch &= ~(key_status.on_off ^ key_status_old) | key_status.on_off;
            key_status_old = key_status.on_off;
            temp_time = 0;
            key_state = 2;
        }
        else                                                                    // 是误触发
        {
            key_state = 0;
        }
        break;
    case 2:
        if (key_status.on_off == key_status_old)                                // 按键没有改变
        {
            if (++temp_time > LONG_PRESS_TIME)                                  // 进入长按状态
            {
                key_status.lclick = key_status_old;
                key_status.lpress = key_status_old;
                temp_time = 0;
                key_state = 3;
            }
        }
        else                                                                    // 按键释放
        {
            key_status.click = (key_status.on_off ^ key_status_old) & ~key_status.on_off;
            key_state = 1;
        }
        break;
    case 3:
        key_status.lclick = 0x00;
        key_status.lpress = 0x00;
        if (key_status.on_off == key_status_old)
        {
            if (++temp_time > RUNNING_PRESS_TIME)                               // 连发时间间隔
            {
                key_status.lpress = key_status_old;
                temp_time = 0;
            }
        }
        else
        {
            key_state = 1;
        }
        break;
    }
}

/**
  ******************************************************************************
  * @brief  按键任务函数范例. 需每10ms调用一次.
  *         该函数是应用层函数, 使用时可直接复制此代码到应用层文件.
  * @param  none
  * @retval none
  ******************************************************************************
  */
/*
void key_task_10ms(void)
{
    key_scan_10ms();

    switch(key_status.touch)                                                    // 检测按键触发
    {
    case KEY_MASK(KEY2):
        do_something;                                                           // KEY2被按下, 动作
        break;
    case KEY_MASK(KEY3):

        break;
    case KEY_MASK(KEY_UP):

        break;
    case KEY_MASK(KEY_DOWN):

        break;
    case KEY_MASK(KEY_LEFT):

        break;
    case KEY_MASK(KEY_RIGHT):

        break;
    case KEY_MASK(KEY_SEL):

        break;
    }
    key_status.touch = 0;                                                       // touch态不会自动清零

    switch(key_status.click)                                                    // 检测短按键
    {
    case KEY_MASK(KEY2):
        do_something;                                                           // KEY2被按下, 动作
        break;
    case KEY_MASK(KEY3):

        break;
    case KEY_MASK(KEY_UP):

        break;
    case KEY_MASK(KEY_DOWN):

        break;
    case KEY_MASK(KEY_LEFT):

        break;
    case KEY_MASK(KEY_RIGHT):

        break;
    case KEY_MASK(KEY_SEL):

        break;
    }

    switch(key_status.lclick)                                                   // 检测长按键
    {
    case KEY_MASK(KEY2):

        break;
    case KEY_MASK(KEY3):

        break;
    case KEY_MASK(KEY_UP):

        break;
    case KEY_MASK(KEY_DOWN):

        break;
    case KEY_MASK(KEY_LEFT):

        break;
    case KEY_MASK(KEY_RIGHT):

        break;
    case KEY_MASK(KEY_SEL):

        break;
    }

    // 连发不常用, 此处不再列出. 原理相同, 使用switch (key_status.press)判断即可
}
*/

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
