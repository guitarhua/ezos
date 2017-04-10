/**
  ******************************************************************************
  * @file    drv_led.c
  * @author  mousie
  * @version V2.1.0
  * @date    2012.2.25
  * @brief   LED����������. ֧�ֿɵ���/������/��˸��Ч��.
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
  * @brief    ˽�����Ͷ���
  * @{
  */
/// LED�������Ͷ���
typedef struct
{
    GPIO_TypeDef *const PORT;                                                   ///< LED��PORT��
    const uint32_t CLK;                                                         ///< LED��ʱ��ģ��
    const uint16_t PIN;                                                         ///< LED��PIN��
} led_hw_t;

#ifdef LED_ADVANCED_ENABLE
typedef struct
{
    volatile uint16_t counter;                                                  ///< LED PWM������/��˸������
    volatile uint16_t dutyfactor;                                               ///< LED PWMռ�ձ�/��˸Ƶ��
    volatile uint16_t destination;                                              ///< LED PWMռ�ձ�Ŀ��ֵ/��˸����
} led_para_t;
#endif


/**
  * @}
  */

/** @defgroup DRV_Led_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */
#define FLAG_MASK                       0xC000                                  ///< LED���ͱ��
#define FLAG_LED                        0x0000                                  ///< ��ͨ�Ʊ��
#define FLAG_BLED                       0x4000                                  ///< �����Ʊ��
#define FLAG_FLED                       0x8000                                  ///< ��˸�Ʊ��

#define BLED_PWM_WIDTH                  20                                      ///< B=Breath, ������PWM����, ��λms, <25������˸��, Ӱ�콥��ʱ��
#define FLED_TIME_MIN                   2                                       ///< F=Flash,  ��˸������Сֵ
#define FLED_TIME_MAX                   30000                                   ///< F=Flash,  ��˸�������ֵ, ��˸���ڱ���С�� 0x4000 ms
/**
  * @}
  */

/** @defgroup DRV_Led_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
/// LED����ӳ���
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
/// LED�߼�����
static led_para_t led_para[LED_NUM];
#endif
/**
  * @}
  */

/** @defgroup DRV_Led_Private_Function
  * @brief    ����˽�к���
  * @{
  */
/// LED IO�ڳ�ʼ��, ���ó��������
__INLINE static void led_port_init(led_name_t led_name)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(led_hw[led_name].CLK, ENABLE);                       // ʹ��LED��ʱ��
    GPIO_InitStructure.GPIO_Pin   = led_hw[led_name].PIN;                       // ����LED��PIN
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;                           // IO����Ϊ�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;                            // ���Ƶ�����2MHz,LEDӦ������
    GPIO_Init(led_hw[led_name].PORT, &GPIO_InitStructure);                      // ��ʼ����LED��IO��
}

/// LED IO�ڵ�ƽ�ø�
__INLINE static void LED_PORT_HIGH(led_name_t led_name)
{
    led_hw[led_name].PORT->BSRR = led_hw[led_name].PIN;
}

/// LED IO�ڵ�ƽ�õ�
__INLINE static void LED_PORT_LOW(led_name_t led_name)
{
    led_hw[led_name].PORT->BRR  = led_hw[led_name].PIN;
}

/// LED IO�ڵ�ƽ��ת
__INLINE static void LED_PORT_TOGGLE(led_name_t led_name)
{
    led_hw[led_name].PORT->ODR ^= led_hw[led_name].PIN;
}

/// ��ȡLED IO�ڵ�ƽֵ
__INLINE static uint32_t LED_PORT_LEVEL(led_name_t led_name)
{
    return (led_hw[led_name].PORT->ODR & led_hw[led_name].PIN);
}
/**
  * @}
  */



/** @defgroup DRV_Led_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  ��ʼ�����е�LED��
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
  * @brief  LED�Ƴ�ʼ��
  * @param  led_name
  *   @arg  ����ֵ��ο� led_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void led_init(led_name_t led_name)
{
    assert_param(led_name < LED_NUM);
    if (led_name >= LED_NUM) return;

    led_port_init(led_name);
    led_off(led_name);                                                          // ��ʼ���رո�LED
}

/**
  ******************************************************************************
  * @brief  LED�ƴ�
  * @param  led_name
  *   @arg  ����ֵ��ο� led_name_t ���Ͷ����е�ö��ֵ
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
  * @brief  LED�ƹر�
  * @param  led_name
  *   @arg  ����ֵ��ο� led_name_t ���Ͷ����е�ö��ֵ
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
  * @brief  LED�Ʒ�ת
  * @param  led_name
  *   @arg  ����ֵ��ο� led_name_t ���Ͷ����е�ö��ֵ
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
  * @brief  ��ѯLED��ǰ״̬
  * @param  led_name
  *   @arg  ����ֵ��ο� led_name_t ���Ͷ����е�ö��ֵ
  * @retval status. LED״̬, ��4bit����LED����, ��4bit����LED״̬
  *   @arg  LED_ON   = 0x01, LED�ƴ�״̬
  *   @arg  LED_OFF  = 0x00, LED�ƹر�״̬
  *   @arg  BLED_ON  = 0x11, �����ƴ�״̬
  *   @arg  BLED_OFF = 0x10, �����ƹر�״̬
  *   @arg  FLED_ON  = 0x21, ��˸�Ƹߵ�ƽ״̬
  *   @arg  FLED_OFF = 0x20, ��˸�Ƶ͵�ƽ״̬
  ******************************************************************************
  */
led_status_t led_get_status(led_name_t led_name)
{
    assert_param(led_name < LED_NUM);
    if (led_name >= LED_NUM) return LED_OFF;

#ifdef LED_ADVANCED_ENABLE
    if ((led_para[led_name].destination & FLAG_MASK) == FLAG_BLED)              // ������
    {
        if (led_para[led_name].destination < (FLAG_BLED | BLED_PWM_WIDTH)) return BLED_ON;
        else                                                               return BLED_OFF;
    }
    if ((led_para[led_name].destination & FLAG_MASK) == FLAG_FLED)              // ��˸��
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

#if (LED_LEVEL > 0)                                                             // ��ͨ��
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
  * @brief  LED��������ֵ
  * @param  led_name
  *   @arg  ����ֵ��ο� led_name_t ���Ͷ����е�ö��ֵ
  * @param  level, 0����, 0xFFϨ��.
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
    if ((st == LED_ON) || (st == FLED_ON))                                      // LEDȫ��״̬
    {
        led_para[led_name].dutyfactor = FLAG_BLED | 0;                          // ռ�ձ���ʼֵ
    }
    else if ((st == LED_OFF) || (st == FLED_OFF))                               // LEDϨ��״̬
    {
        led_para[led_name].dutyfactor = FLAG_BLED | BLED_PWM_WIDTH;
    }

    led_para[led_name].counter     = FLAG_BLED | 0;                             // ���¿�ʼPWM����
    led_para[led_name].destination = FLAG_BLED | level;                         // ����Ŀ������ֵ
}

/**
  ******************************************************************************
  * @brief  LED�ƽ����
  * @param  led_name
  *   @arg  ����ֵ��ο� led_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void led_on_gradual(led_name_t led_name)
{
    led_adjust(led_name, 0);
}

/**
  ******************************************************************************
  * @brief  LED�ƽ���ر�
  * @param  led_name
  *   @arg  ����ֵ��ο� led_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void led_off_gradual(led_name_t led_name)
{
    led_adjust(led_name, 0xFF);
}

/**
  ******************************************************************************
  * @brief  LED�ƽ��䷭ת
  * @param  led_name
  *   @arg  ����ֵ��ο� led_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void led_toggle_gradual(led_name_t led_name)
{
    led_status_t st;

    assert_param(led_name < LED_NUM);
    if (led_name >= LED_NUM) return;

    st = led_get_status(led_name);
    if (st & 0x01) led_adjust(led_name, 0xFF);                                  // LED��״̬
    else           led_adjust(led_name, 0);                                     // LED�ر�״̬
}

/**
  ******************************************************************************
  * @brief  LED����˸
  * @param  led_name
  *   @arg  ����ֵ��ο� led_name_t ���Ͷ����е�ö��ֵ
  * @param  time_ms, ����1ms��LED��˸ʱ���趨. ȡֵ��Χ���£�
  *   @arg  FLED_TIME_MIN, 2ms,     ��˸Ƶ�����
  *   @arg  FLED_TIME_MAX, 30000ms, ��˸Ƶ������
  * @param  counter, ��˸����. 0��ʾһֱ��˸, 1-255��˸ָ������
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
    led_para[led_name].counter     = FLAG_FLED | 0;                             // ���ü�����
    led_para[led_name].dutyfactor  = FLAG_FLED | ((uint16_t)time_ms >> 1);      // ������˸Ƶ��
    led_para[led_name].destination = FLAG_FLED | ((uint16_t)counter << 1);      // ������˸����
#endif

#if (LED_LEVEL > 0)
    LED_PORT_HIGH(led_name);
#else
    LED_PORT_LOW(led_name);
#endif
}

/**
  ******************************************************************************
  * @brief  LED��1msɨ�躯��, LED����˸�ͺ���Ч������Ҫ�ô˺���
  *         1ms��ʱ���øú�������.ʱ��Ҫ���ϸ�, ������ڶ�ʱ�ж���
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
        if ((led_para[i].destination & FLAG_MASK) == FLAG_LED)                  // ��ͨ��
        {
            continue;
        }
        else if ((led_para[i].destination & FLAG_MASK) == FLAG_BLED)            // ������
        {
            if (++led_para[i].counter >= (FLAG_BLED | BLED_PWM_WIDTH))          // ��һ��PWM����ʼ
            {
            #if (LED_LEVEL > 0)                                                 // �ص�
                LED_PORT_LOW((led_name_t)i);
            #else
                LED_PORT_HIGH((led_name_t)i);
            #endif

                led_para[i].counter = FLAG_BLED | 0;
                temp = led_para[i].destination;
                if (led_para[i].dutyfactor < temp)                              // ռ�ձ��𽥽ӽ�Ŀ��ֵ
                {
                    led_para[i].dutyfactor++;
                }
                else if (led_para[i].dutyfactor > temp)
                {
                    led_para[i].dutyfactor--;
                }
            }

            temp = led_para[i].dutyfactor;
            if (led_para[i].counter == temp)                                    // ռ�ձȴ���תLED��
            {
            #if (LED_LEVEL > 0)                                                 // ����
                LED_PORT_HIGH((led_name_t)i);
            #else
                LED_PORT_LOW((led_name_t)i);
            #endif
            }
        }
        else if ((led_para[i].destination & FLAG_MASK) == FLAG_FLED)            // ����˸��
        {
            temp = led_para[i].dutyfactor;
            if (++led_para[i].counter >= temp)
            {
                led_para[i].counter = FLAG_FLED | 0;
                LED_PORT_TOGGLE((led_name_t)i);

                if (led_para[i].destination > (FLAG_FLED | 0))
                {
                    if (--led_para[i].destination <= (FLAG_FLED | 0))           // ��˸������, ��˸����
                    {
                        led_para[(led_name_t)i].counter     = FLAG_LED | 0;
                        led_para[(led_name_t)i].dutyfactor  = FLAG_LED | 0;
                        led_para[(led_name_t)i].destination = FLAG_LED | 0;

                    #if (LED_LEVEL > 0)                                         // �ص�
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
