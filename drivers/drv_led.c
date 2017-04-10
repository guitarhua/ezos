/**
  ******************************************************************************
  * @file    drv_led.c
  * @author  mousie
  * @version V2.1.0
  * @date    2012.2.25
  * @brief   LED灯驱动程序. 支持可调灯/呼吸灯/闪烁灯效果.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_led.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_Led
  * @{
  */
#if (LED_NUM > 0)



/**
  ******************************************************************************
  * @addtogroup DRV_Led_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_Led_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */
/// LED参数类型定义
typedef struct
{
    GPIO_TypeDef *const PORT;                                                   ///< LED的PORT口
    const uint32_t CLK;                                                         ///< LED的时钟模块
    const uint16_t PIN;                                                         ///< LED的PIN口
} led_hw_t;

#ifdef LED_ADVANCED_ENABLE
typedef struct
{
    volatile uint16_t counter;                                                  ///< LED PWM计数器/闪烁计数器
    volatile uint16_t dutyfactor;                                               ///< LED PWM占空比/闪烁频率
    volatile uint16_t destination;                                              ///< LED PWM占空比目标值/闪烁次数
} led_para_t;
#endif


/**
  * @}
  */

/** @defgroup DRV_Led_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
#define FLAG_MASK                       0xC000                                  ///< LED类型标记
#define FLAG_LED                        0x0000                                  ///< 普通灯标记
#define FLAG_BLED                       0x4000                                  ///< 呼吸灯标记
#define FLAG_FLED                       0x8000                                  ///< 闪烁灯标记

#define BLED_PWM_WIDTH                  20                                      ///< B=Breath, 呼吸灯PWM脉宽, 单位ms, <25避免闪烁感, 影响渐变时间
#define FLED_TIME_MIN                   2                                       ///< F=Flash,  闪烁周期最小值
#define FLED_TIME_MAX                   30000                                   ///< F=Flash,  闪烁周期最大值, 闪烁周期必须小于 0x4000 ms
/**
  * @}
  */

/** @defgroup DRV_Led_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
/// LED参数映射表
static const led_hw_t led_hw[LED_NUM] = {
#if (LED_NUM > 0)
    {LED0_PORT,  LED0_CLK,  LED0_PIN},
#endif
#if (LED_NUM > 1)
    {LED1_PORT,  LED1_CLK,  LED1_PIN},
#endif
#if (LED_NUM > 2)
    {LED2_PORT,  LED2_CLK,  LED2_PIN},
#endif
#if (LED_NUM > 3)
    {LED3_PORT,  LED3_CLK,  LED3_PIN},
#endif
#if (LED_NUM > 4)
    {LED4_PORT,  LED4_CLK,  LED4_PIN},
#endif
#if (LED_NUM > 5)
    {LED5_PORT,  LED5_CLK,  LED5_PIN},
#endif
#if (LED_NUM > 6)
    {LED6_PORT,  LED6_CLK,  LED6_PIN},
#endif
#if (LED_NUM > 7)
    {LED7_PORT,  LED7_CLK,  LED7_PIN},
#endif
#if (LED_NUM > 8)
    {LED8_PORT,  LED8_CLK,  LED8_PIN},
#endif
#if (LED_NUM > 9)
    {LED9_PORT,  LED9_CLK,  LED9_PIN},
#endif
#if (LED_NUM > 10)
    {LED10_PORT, LED10_CLK, LED10_PIN},
#endif
#if (LED_NUM > 11)
    {LED11_PORT, LED11_CLK, LED11_PIN},
#endif
#if (LED_NUM > 12)
    {LED12_PORT, LED12_CLK, LED12_PIN},
#endif
#if (LED_NUM > 13)
    {LED13_PORT, LED13_CLK, LED13_PIN},
#endif
#if (LED_NUM > 14)
    {LED14_PORT, LED14_CLK, LED14_PIN},
#endif
#if (LED_NUM > 15)
    {LED15_PORT, LED15_CLK, LED15_PIN},
#endif
};

#ifdef LED_ADVANCED_ENABLE
/// LED高级参数
static led_para_t led_para[LED_NUM];
#endif
/**
  * @}
  */

/** @defgroup DRV_Led_Private_Function
  * @brief    定义私有函数
  * @{
  */
/// LED IO口初始化, 配置成推挽输出
__INLINE static void led_port_init(led_name_t led_name)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(led_hw[led_name].CLK, ENABLE);                       // 使能LED的时钟
    GPIO_InitStructure.GPIO_Pin   = led_hw[led_name].PIN;                       // 设置LED的PIN
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;                           // IO口设为推拉输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;                            // 输出频率最大2MHz,LED应用足矣
    GPIO_Init(led_hw[led_name].PORT, &GPIO_InitStructure);                      // 初始化该LED的IO口
}

/// LED IO口电平置高
__INLINE static void LED_PORT_HIGH(led_name_t led_name)
{
    led_hw[led_name].PORT->BSRR = led_hw[led_name].PIN;
}

/// LED IO口电平置低
__INLINE static void LED_PORT_LOW(led_name_t led_name)
{
    led_hw[led_name].PORT->BRR  = led_hw[led_name].PIN;
}

/// LED IO口电平翻转
__INLINE static void LED_PORT_TOGGLE(led_name_t led_name)
{
    led_hw[led_name].PORT->ODR ^= led_hw[led_name].PIN;
}

/// 读取LED IO口电平值
__INLINE static uint32_t LED_PORT_LEVEL(led_name_t led_name)
{
    return (led_hw[led_name].PORT->ODR & led_hw[led_name].PIN);
}
/**
  * @}
  */



/** @defgroup DRV_Led_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  初始化所有的LED灯
  * @param  none
  * @retval none
  ******************************************************************************
  */
void led_init_all(void)
{
    uint8_t i;

    for (i=0; i<LED_NUM; i++)
        led_init((led_name_t)i);
}

/**
  ******************************************************************************
  * @brief  LED灯初始化
  * @param  led_name
  *   @arg  可用值请参考 led_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void led_init(led_name_t led_name)
{
    assert_param(led_name < LED_NUM);
    if (led_name >= LED_NUM) return;

    led_port_init(led_name);
    led_off(led_name);                                                          // 初始化关闭该LED
}

/**
  ******************************************************************************
  * @brief  LED灯打开
  * @param  led_name
  *   @arg  可用值请参考 led_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void led_on(led_name_t led_name)
{
    assert_param(led_name < LED_NUM);
    if (led_name >= LED_NUM) return;

#ifdef LED_ADVANCED_ENABLE
    led_para[led_name].counter     = FLAG_LED | 0;
    led_para[led_name].dutyfactor  = FLAG_LED | 0;
    led_para[led_name].destination = FLAG_LED | 0;
#endif

#if (LED_LEVEL > 0)
    LED_PORT_HIGH(led_name);
#else
    LED_PORT_LOW(led_name);
#endif
}

/**
  ******************************************************************************
  * @brief  LED灯关闭
  * @param  led_name
  *   @arg  可用值请参考 led_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void led_off(led_name_t led_name)
{
    assert_param(led_name < LED_NUM);
    if (led_name >= LED_NUM) return;

#ifdef LED_ADVANCED_ENABLE
    led_para[led_name].counter     = FLAG_LED | 0;
    led_para[led_name].dutyfactor  = FLAG_LED | 0;
    led_para[led_name].destination = FLAG_LED | 0;
#endif

#if (LED_LEVEL > 0)
    LED_PORT_LOW(led_name);
#else
    LED_PORT_HIGH(led_name);
#endif
}

/**
  ******************************************************************************
  * @brief  LED灯翻转
  * @param  led_name
  *   @arg  可用值请参考 led_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void led_toggle(led_name_t led_name)
{
    assert_param(led_name < LED_NUM);
    if (led_name >= LED_NUM) return;

#ifdef LED_ADVANCED_ENABLE
    led_para[led_name].counter     = FLAG_LED | 0;
    led_para[led_name].dutyfactor  = FLAG_LED | 0;
    led_para[led_name].destination = FLAG_LED | 0;
#endif

    LED_PORT_TOGGLE(led_name);
}

/**
  ******************************************************************************
  * @brief  查询LED当前状态
  * @param  led_name
  *   @arg  可用值请参考 led_name_t 类型定义中的枚举值
  * @retval status. LED状态, 高4bit代表LED类型, 低4bit代表LED状态
  *   @arg  LED_ON   = 0x01, LED灯打开状态
  *   @arg  LED_OFF  = 0x00, LED灯关闭状态
  *   @arg  BLED_ON  = 0x11, 呼吸灯打开状态
  *   @arg  BLED_OFF = 0x10, 呼吸灯关闭状态
  *   @arg  FLED_ON  = 0x21, 闪烁灯高电平状态
  *   @arg  FLED_OFF = 0x20, 闪烁灯低电平状态
  ******************************************************************************
  */
led_status_t led_get_status(led_name_t led_name)
{
    assert_param(led_name < LED_NUM);
    if (led_name >= LED_NUM) return LED_OFF;

#ifdef LED_ADVANCED_ENABLE
    if ((led_para[led_name].destination & FLAG_MASK) == FLAG_BLED)              // 呼吸灯
    {
        if (led_para[led_name].destination < (FLAG_BLED | BLED_PWM_WIDTH)) return BLED_ON;
        else                                                               return BLED_OFF;
    }
    if ((led_para[led_name].destination & FLAG_MASK) == FLAG_FLED)              // 闪烁灯
    {
    #if (LED_LEVEL > 0)
        if (LED_PORT_LEVEL(led_name)) return FLED_ON;
        else                          return FLED_OFF;
    #else
        if (LED_PORT_LEVEL(led_name)) return FLED_OFF;
        else                          return FLED_ON;
    #endif
    }
#endif

#if (LED_LEVEL > 0)                                                             // 普通灯
    if (LED_PORT_LEVEL(led_name)) return LED_ON;
    else                          return LED_OFF;
#else
    if (LED_PORT_LEVEL(led_name)) return LED_OFF;
    else                          return LED_ON;
#endif
}

#ifdef LED_ADVANCED_ENABLE
/**
  ******************************************************************************
  * @brief  LED调节亮度值
  * @param  led_name
  *   @arg  可用值请参考 led_name_t 类型定义中的枚举值
  * @param  level, 0最亮, 0xFF熄灭.
  * @retval none
  ******************************************************************************
  */
void led_adjust(led_name_t led_name, uint8_t level)
{
    uint16_t value;
    led_status_t st;

    assert_param(led_name < LED_NUM);
    if (led_name >= LED_NUM) return;

    value = level * BLED_PWM_WIDTH;
    level = value / 0xFF;

    st = led_get_status(led_name);
    if ((st == LED_ON) || (st == FLED_ON))                                      // LED全亮状态
    {
        led_para[led_name].dutyfactor = FLAG_BLED | 0;                          // 占空比起始值
    }
    else if ((st == LED_OFF) || (st == FLED_OFF))                               // LED熄灭状态
    {
        led_para[led_name].dutyfactor = FLAG_BLED | BLED_PWM_WIDTH;
    }

    led_para[led_name].counter     = FLAG_BLED | 0;                             // 重新开始PWM计数
    led_para[led_name].destination = FLAG_BLED | level;                         // 设置目标亮度值
}

/**
  ******************************************************************************
  * @brief  LED灯渐变打开
  * @param  led_name
  *   @arg  可用值请参考 led_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void led_on_gradual(led_name_t led_name)
{
    led_adjust(led_name, 0);
}

/**
  ******************************************************************************
  * @brief  LED灯渐变关闭
  * @param  led_name
  *   @arg  可用值请参考 led_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void led_off_gradual(led_name_t led_name)
{
    led_adjust(led_name, 0xFF);
}

/**
  ******************************************************************************
  * @brief  LED灯渐变翻转
  * @param  led_name
  *   @arg  可用值请参考 led_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void led_toggle_gradual(led_name_t led_name)
{
    led_status_t st;

    assert_param(led_name < LED_NUM);
    if (led_name >= LED_NUM) return;

    st = led_get_status(led_name);
    if (st & 0x01) led_adjust(led_name, 0xFF);                                  // LED打开状态
    else           led_adjust(led_name, 0);                                     // LED关闭状态
}

/**
  ******************************************************************************
  * @brief  LED灯闪烁
  * @param  led_name
  *   @arg  可用值请参考 led_name_t 类型定义中的枚举值
  * @param  time_ms, 基于1ms的LED闪烁时间设定. 取值范围如下：
  *   @arg  FLED_TIME_MIN, 2ms,     闪烁频率最快
  *   @arg  FLED_TIME_MAX, 30000ms, 闪烁频率最慢
  * @param  counter, 闪烁次数. 0表示一直闪烁, 1-255闪烁指定次数
  * @retval none
  ******************************************************************************
  */
void led_flash(led_name_t led_name, uint16_t time_ms, uint8_t counter)
{
    assert_param(led_name < LED_NUM);
    if (led_name >= LED_NUM) return;

    if (time_ms < FLED_TIME_MIN) time_ms = FLED_TIME_MIN;
    if (time_ms > FLED_TIME_MAX) time_ms = FLED_TIME_MAX;

#ifdef LED_ADVANCED_ENABLE
    led_para[led_name].counter     = FLAG_FLED | 0;                             // 设置计数器
    led_para[led_name].dutyfactor  = FLAG_FLED | ((uint16_t)time_ms >> 1);      // 设置闪烁频率
    led_para[led_name].destination = FLAG_FLED | ((uint16_t)counter << 1);      // 设置闪烁次数
#endif

#if (LED_LEVEL > 0)
    LED_PORT_HIGH(led_name);
#else
    LED_PORT_LOW(led_name);
#endif
}

/**
  ******************************************************************************
  * @brief  LED灯1ms扫描函数, LED灯闪烁和呼吸效果必须要用此函数
  *         1ms定时调用该函数即可.时间要求不严格, 无需放在定时中断中
  * @param  none
  * @retval none
  ******************************************************************************
  */
void led_scan_1ms(void)
{
    uint8_t  i;
    uint16_t temp;

    for (i=0; i<LED_NUM; i++)
    {
        if ((led_para[i].destination & FLAG_MASK) == FLAG_LED)                  // 普通灯
        {
            continue;
        }
        else if ((led_para[i].destination & FLAG_MASK) == FLAG_BLED)            // 呼吸灯
        {
            if (++led_para[i].counter >= (FLAG_BLED | BLED_PWM_WIDTH))          // 新一轮PWM脉宽开始
            {
            #if (LED_LEVEL > 0)                                                 // 关灯
                LED_PORT_LOW((led_name_t)i);
            #else
                LED_PORT_HIGH((led_name_t)i);
            #endif

                led_para[i].counter = FLAG_BLED | 0;
                temp = led_para[i].destination;
                if (led_para[i].dutyfactor < temp)                              // 占空比逐渐接近目标值
                {
                    led_para[i].dutyfactor++;
                }
                else if (led_para[i].dutyfactor > temp)
                {
                    led_para[i].dutyfactor--;
                }
            }

            temp = led_para[i].dutyfactor;
            if (led_para[i].counter == temp)                                    // 占空比处翻转LED灯
            {
            #if (LED_LEVEL > 0)                                                 // 开灯
                LED_PORT_HIGH((led_name_t)i);
            #else
                LED_PORT_LOW((led_name_t)i);
            #endif
            }
        }
        else if ((led_para[i].destination & FLAG_MASK) == FLAG_FLED)            // 是闪烁灯
        {
            temp = led_para[i].dutyfactor;
            if (++led_para[i].counter >= temp)
            {
                led_para[i].counter = FLAG_FLED | 0;
                LED_PORT_TOGGLE((led_name_t)i);

                if (led_para[i].destination > (FLAG_FLED | 0))
                {
                    if (--led_para[i].destination <= (FLAG_FLED | 0))           // 闪烁次数到, 闪烁结束
                    {
                        led_para[(led_name_t)i].counter     = FLAG_LED | 0;
                        led_para[(led_name_t)i].dutyfactor  = FLAG_LED | 0;
                        led_para[(led_name_t)i].destination = FLAG_LED | 0;

                    #if (LED_LEVEL > 0)                                         // 关灯
                        LED_PORT_LOW((led_name_t)i);
                    #else
                        LED_PORT_HIGH((led_name_t)i);
                    #endif
                    }
                }
            }
        }
    }
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
