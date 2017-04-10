/**
  ******************************************************************************
  * @file    drv_key.c
  * @author  mousie
  * @version V2.1.1
  * @date    2015.4.21
  * @brief   ֱ��ʽ������������.
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
#define LONG_PRESS_TIME                 130                                     ///< ��������ʱ������, ��λ10ms, ������ʹ��1.3S�Ϻ�
#define RUNNING_PRESS_TIME              20                                      ///< ����ʱ��������, ��λ10ms, ������ʹ��0.2s�Ϻ�
/**
  * @}
  */



/** @defgroup DRV_Key_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */
/// ����Ӳ���������Ͷ���
typedef struct
{
    GPIO_TypeDef *const PORT;                                                   ///< ������PORT��
    const uint32_t CLK;                                                         ///< ������ʱ��ģ��
    const uint16_t PIN;                                                         ///< ������PIN��
} key_hw_t;
/**
  * @}
  */

/** @defgroup DRV_Key_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Key_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
/// ����Ӳ������ӳ���
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

volatile key_status_t key_status = {0, 0, 0, 0, 0};                             ///< ����״̬����
/**
  * @}
  */

/** @defgroup DRV_Key_Private_Function
  * @brief    ����˽�к���
  * @{
  */
/// ����IO�ڳ�ʼ��, ���ó��������� �� ��������
__INLINE static void key_port_init(key_name_t key_name)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(key_hw[key_name].CLK, ENABLE);                       // ʹ�ܰ����˿ڵ�ʱ��
    GPIO_InitStructure.GPIO_Pin  = key_hw[key_name].PIN;                        // ���ð����˿ڵ�PIN
#ifdef KEY_EXTPULL
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                       // ���ⲿ��������, IO����Ϊ��������
#else
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               // ���ⲿ��������, IO����Ϊ��������
#endif
    GPIO_Init(key_hw[key_name].PORT, &GPIO_InitStructure);                      // ��ʼ������IO��
}

/// ��ȡ����IO�ڵ�ǰ��ƽֵ. 0, �͵�ƽ. !0, �ߵ�ƽ
__INLINE static uint32_t KEY_PORT_LEVEL(key_name_t key_name)
{
    return (key_hw[key_name].PORT->IDR & key_hw[key_name].PIN);
}
/**
  * @}
  */



/** @defgroup DRV_Key_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  ��ʼ�����еİ����˿�
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
  * @brief  ������ʼ������
  * @param  key_name
  *     @arg ����ֵ��ο� key_name_t ���Ͷ����е�ö��ֵ
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
  * @brief  ����10msɨ�躯��, ����������Ҫ�ô˺���.
  *         10ms��ʱ���øú�������.ʱ��Ҫ���ϸ�, ������ڶ�ʱ�ж���.
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
        bit = !KEY_PORT_LEVEL((key_name_t)i);                                   // ��ȡ������ǰ��ƽֵ
        key_status.on_off |= (bit << i);                                        // ��ȡ��ǰ����״̬
    }

    switch(key_state)
    {
    case 0:                                                                     // �ް���״̬
        if (key_status.on_off) key_state = 1;                                   // �а���, ������ȷ��״̬
        key_status.on_off  = 0x00;                                              // ��������������Ϊ�����Ѿ�����
        key_status.touch   = 0x00;
        key_status.lclick  = 0x00;
        key_status.lpress  = 0x00;
        key_status.click = 0x00;
        key_status_old = 0x00;
        break;
    case 1:                                                                     // ����ȷ�ϰ���״̬
        if (key_status.on_off)                                                  // ȷ�������а���
        {
            // ��ֵ����ֵ
            key_status.touch |= (key_status.on_off ^ key_status_old) & key_status.on_off;
            key_status.touch &= ~(key_status.on_off ^ key_status_old) | key_status.on_off;
            key_status_old = key_status.on_off;
            temp_time = 0;
            key_state = 2;
        }
        else                                                                    // ���󴥷�
        {
            key_state = 0;
        }
        break;
    case 2:
        if (key_status.on_off == key_status_old)                                // ����û�иı�
        {
            if (++temp_time > LONG_PRESS_TIME)                                  // ���볤��״̬
            {
                key_status.lclick = key_status_old;
                key_status.lpress = key_status_old;
                temp_time = 0;
                key_state = 3;
            }
        }
        else                                                                    // �����ͷ�
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
            if (++temp_time > RUNNING_PRESS_TIME)                               // ����ʱ����
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
  * @brief  ��������������. ��ÿ10ms����һ��.
  *         �ú�����Ӧ�ò㺯��, ʹ��ʱ��ֱ�Ӹ��ƴ˴��뵽Ӧ�ò��ļ�.
  * @param  none
  * @retval none
  ******************************************************************************
  */
/*
void key_task_10ms(void)
{
    key_scan_10ms();

    switch(key_status.touch)                                                    // ��ⰴ������
    {
    case KEY_MASK(KEY2):
        do_something;                                                           // KEY2������, ����
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
    key_status.touch = 0;                                                       // touch̬�����Զ�����

    switch(key_status.click)                                                    // ���̰���
    {
    case KEY_MASK(KEY2):
        do_something;                                                           // KEY2������, ����
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

    switch(key_status.lclick)                                                   // ��ⳤ����
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

    // ����������, �˴������г�. ԭ����ͬ, ʹ��switch (key_status.press)�жϼ���
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
