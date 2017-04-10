/**
  ******************************************************************************
  * @file    drv_i2c.c
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.16
  * @brief   �ṩ I2C ��������. ����ģʽ, 7λ��ַ. ͬʱ֧��IO�����ģ���Ӳ��I2C.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "drv_i2c.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_I2C
  * @{
  */
#if (I2C_NUM > 0)



/**
  ******************************************************************************
  * @addtogroup DRV_I2C_Configure
  ******************************************************************************
  * @{
  */
#define I2C_HW_ENABLE                                                           ///< ʹ��Ӳ��I2C����.
#define I2C_SW_ENABLE                                                           ///< ʹ�����ģ��I2C����.

#include "drv_systick.h"
#define i2c_delay_us(time_us)           delay_us(time_us)                       ///< ����I2C΢����ʱ����, ���ڲ������ģ��I2C��SCK�źŲ�����. Ӳ��I2C����ú���.
/**
  * @}
  */



/** @defgroup DRV_I2C_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */
/// I2CӲ���������Ͷ���
typedef struct
{
#ifdef I2C_HW_ENABLE
    const uint32_t CLK;                                                         ///< Ӳ��I2Cʱ��ģ��
    const uint32_t REMAP;                                                       ///< Ӳ��I2C������ӳ��
#endif
    I2C_TypeDef *const PERIPHERAL;                                              ///< Ӳ��I2Cģ����
    const uint32_t SCL_CLK;                                                     ///< SCL����ʱ��ģ��
    const uint32_t SDA_CLK;                                                     ///< SDA����ʱ��ģ��
    GPIO_TypeDef *const SCL_PORT;                                               ///< SCL����PORT��
    GPIO_TypeDef *const SDA_PORT;                                               ///< SDA����PORT��
    const uint16_t SCL_PIN;                                                     ///< SCL����PIN��
    const uint16_t SDA_PIN;                                                     ///< SDA����PIN��
} i2c_hw_t;

/// I2C�������Ͷ���
typedef struct
{
    uint16_t delay_us;                                                          ///< ���ģ��I2C��, ��¼��ʱʱ��, ��λUs.
} i2c_para_t;
/**
  * @}
  */

/** @defgroup DRV_I2C_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2C_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
/// I2CӲ������ӳ���
static const i2c_hw_t i2c_hw[I2C_NUM] = {
#if (I2C_NUM > 0)
    {
#ifdef I2C_HW_ENABLE
    I2C0_CLK,       I2C0_GPIO_REMAP,
#endif
    I2C0_PERIPHERAL,
    I2C0_SCL_CLK,   I2C0_SDA_CLK,
    I2C0_SCL_PORT,  I2C0_SDA_PORT,
    I2C0_SCL_PIN,   I2C0_SDA_PIN,   },
#endif

#if (I2C_NUM > 1)
    {
#ifdef I2C_HW_ENABLE
    I2C1_CLK,       I2C1_GPIO_REMAP,
#endif
    I2C1_PERIPHERAL,
    I2C1_SCL_CLK,   I2C1_SDA_CLK,
    I2C1_SCL_PORT,  I2C1_SDA_PORT,
    I2C1_SCL_PIN,   I2C1_SDA_PIN,   },
#endif

#if (I2C_NUM > 2)
    {
#ifdef I2C_HW_ENABLE
    I2C2_CLK,       I2C2_GPIO_REMAP,
#endif
    I2C2_PERIPHERAL,
    I2C2_SCL_CLK,   I2C2_SDA_CLK,
    I2C2_SCL_PORT,  I2C2_SDA_PORT,
    I2C2_SCL_PIN,   I2C2_SDA_PIN,   },
#endif

#if (I2C_NUM > 3)
    {
#ifdef I2C_HW_ENABLE
    I2C3_CLK,       I2C3_GPIO_REMAP,
#endif
    I2C3_PERIPHERAL,
    I2C3_SCL_CLK,   I2C3_SDA_CLK,
    I2C3_SCL_PORT,  I2C3_SDA_PORT,
    I2C3_SCL_PIN,   I2C3_SDA_PIN,   },
#endif

#if (I2C_NUM > 4)
    {
#ifdef I2C_HW_ENABLE
    I2C4_CLK,       I2C4_GPIO_REMAP,
#endif
    I2C4_PERIPHERAL,
    I2C4_SCL_CLK,   I2C4_SDA_CLK,
    I2C4_SCL_PORT,  I2C4_SDA_PORT,
    I2C4_SCL_PIN,   I2C4_SDA_PIN,   },
#endif
};

/// I2C������
#ifdef I2C_SW_ENABLE
static i2c_para_t i2c_para[I2C_NUM];
#endif
/**
  * @}
  */

/** @defgroup DRV_I2C_Private_Function
  * @brief    ����˽�к���
  * @{
  */
#ifdef I2C_SW_ENABLE

static i2c_ack_t i2c_tx_byte_sw(i2c_name_t i2c_name, uint8_t val);              // I2C���ģ�ⷢ���ֽ�
static uint8_t   i2c_rx_byte_sw(i2c_name_t i2c_name, uint8_t ack);              // I2C���ģ������ֽ�

/// ���ģ��I2C PORT�ڳ�ʼ��
__INLINE static void i2c_port_init(i2c_name_t i2c_name)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(i2c_hw[i2c_name].SCL_CLK | i2c_hw[i2c_name].SDA_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin   = i2c_hw[i2c_name].SCL_PIN;
    GPIO_Init(i2c_hw[i2c_name].SCL_PORT, &GPIO_InitStructure);                  // ����I2C SCL����, ��©���
    GPIO_InitStructure.GPIO_Pin   = i2c_hw[i2c_name].SDA_PIN;
    GPIO_Init(i2c_hw[i2c_name].SDA_PORT, &GPIO_InitStructure);                  // ����I2C SDA����, ��©���
}

/// I2C���ģ��SCL��ƽ�ø�, ���ȴ�����������SCL(���豸�ͷ�SCL�ź�), ����1, �ø���. 0, ��ʱ.
__INLINE static uint8_t I2C_SCL_HIGH(i2c_name_t i2c_name)
{
    uint8_t i=255;

    i2c_hw[i2c_name].SCL_PORT->BSRR  = i2c_hw[i2c_name].SCL_PIN;
    do
    {
        if (i2c_hw[i2c_name].SCL_PORT->IDR & i2c_hw[i2c_name].SCL_PIN) return 1;
    } while (i--);
    return 0;
}

/// I2C���ģ��SCL��ƽ�õ�
__INLINE static void I2C_SCL_LOW(i2c_name_t i2c_name)
{
    i2c_hw[i2c_name].SCL_PORT->BRR = i2c_hw[i2c_name].SCL_PIN;
}

/// I2C���ģ��SDA��ƽ�ø�
__INLINE static void I2C_SDA_HIGH(i2c_name_t i2c_name)
{
    i2c_hw[i2c_name].SDA_PORT->BSRR = i2c_hw[i2c_name].SDA_PIN;
}

/// I2C���ģ��SDA��ƽ�õ�
__INLINE static void I2C_SDA_LOW(i2c_name_t i2c_name)
{
    i2c_hw[i2c_name].SDA_PORT->BRR = i2c_hw[i2c_name].SDA_PIN;
}

/// ��ȡ���ģ��I2C��SDA��ƽֵ
__INLINE static int32_t I2C_SDA_LEVEL(i2c_name_t i2c_name)
{
    return (i2c_hw[i2c_name].SDA_PORT->IDR & i2c_hw[i2c_name].SDA_PIN);
}
#endif

/// I2C PORT�ڽ���, �ɲ����� �� ���óɸ������� �� ��ʡ���ģʽ
__INLINE static void i2c_port_deinit(i2c_name_t i2c_name)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin   = i2c_hw[i2c_name].SCL_PIN;
    GPIO_Init(i2c_hw[i2c_name].SCL_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = i2c_hw[i2c_name].SDA_PIN;
    GPIO_Init(i2c_hw[i2c_name].SDA_PORT, &GPIO_InitStructure);
}
/**
  * @}
  */



/** @defgroup DRV_I2C_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  I2C��ʼ������
  * @param  i2c_name
  *   @arg  ����ֵ��ο� i2c_name_t ���Ͷ����е�ö��ֵ
  * @param  baud_rate_khz
  *   @arg  1-400, I2C��������Чֵ 1-400 Khz. ����������ǳ���, �������ý����ο�.
  * @retval none
  ******************************************************************************
  */
void i2c_init(i2c_name_t i2c_name, uint16_t baud_rate_khz)
{
#ifdef I2C_HW_ENABLE
    I2C_InitTypeDef  I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
#endif
    I2C_TypeDef *i2c = i2c_hw[i2c_name].PERIPHERAL;

    assert_param(i2c_name < I2C_NUM);
    assert_param((baud_rate_khz >= 1) && (baud_rate_khz <=400));
    if (i2c_name >= I2C_NUM) return;

    if (i2c != 0)                                                               // ʹ��I2CӲ��
    {
    #ifdef I2C_HW_ENABLE
        if (i2c_hw[i2c_name].REMAP != 0)
        {
            GPIO_PinRemapConfig(i2c_hw[i2c_name].REMAP, ENABLE);                // ʹ��������ӳ��
            RCC_APB2PeriphClockCmd(i2c_hw[i2c_name].SCL_CLK |                   // ʹ�� I2C ��IO��ʱ��
                                   i2c_hw[i2c_name].SDA_CLK |
                                   RCC_APB2Periph_AFIO, ENABLE);                // ʹ�ܽ��湦��ʱ��
        }
        else
        {
            RCC_APB2PeriphClockCmd(i2c_hw[i2c_name].SCL_CLK  |                  // ʹ�� I2C ��IO��ʱ��
                                   i2c_hw[i2c_name].SDA_CLK, ENABLE);
        }
        RCC_APB1PeriphClockCmd(i2c_hw[i2c_name].CLK, ENABLE);

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
        GPIO_InitStructure.GPIO_Pin   = i2c_hw[i2c_name].SCL_PIN;
        GPIO_Init(i2c_hw[i2c_name].SCL_PORT, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = i2c_hw[i2c_name].SDA_PIN;
        GPIO_Init(i2c_hw[i2c_name].SDA_PORT, &GPIO_InitStructure);

        I2C_InitStructure.I2C_Mode        = I2C_Mode_I2C;
        I2C_InitStructure.I2C_DutyCycle   = I2C_DutyCycle_2;
        I2C_InitStructure.I2C_OwnAddress1 = 0;
        I2C_InitStructure.I2C_Ack         = I2C_Ack_Enable;
        I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_InitStructure.I2C_ClockSpeed  = baud_rate_khz * 1000;

        I2C_Init(i2c, &I2C_InitStructure);
        I2C_Cmd(i2c, ENABLE);
    #endif
    }
    else                                                                        // ���IO��ʵ��I2C
    {
    #ifdef I2C_SW_ENABLE
        i2c_port_init(i2c_name);
        if (baud_rate_khz < 250)
            i2c_para[i2c_name].delay_us = ((500 / baud_rate_khz) - 2);
        else
            i2c_para[i2c_name].delay_us = 0;
        i2c_stop(i2c_name);
    #endif
    }
}

/**
  ******************************************************************************
  * @brief  ��ֹĳ��I2C. ����I2C�Ĵ����ָ�ΪĬ��ֵ. ������Ϊ��������.
  * @param  i2c_name
  *   @arg  ����ֵ��ο� i2c_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void i2c_deinit(i2c_name_t i2c_name)
{
    assert_param(i2c_name < I2C_NUM);
    if (i2c_name >= I2C_NUM) return;

#ifdef I2C_HW_ENABLE
    if (i2c_hw[i2c_name].PERIPHERAL != 0)
    {
        I2C_Cmd(i2c_hw[i2c_name].PERIPHERAL, DISABLE);
        I2C_DeInit(i2c_hw[i2c_name].PERIPHERAL);
        RCC_APB1PeriphClockCmd(i2c_hw[i2c_name].CLK, DISABLE);
    }
#endif

    i2c_port_deinit(i2c_name);
}

/**
  ******************************************************************************
  * @brief  I2C��ʼ�ź�
  * @param  i2c_name
  *   @arg  ����ֵ��ο� i2c_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void i2c_start(i2c_name_t i2c_name)
{
#ifdef I2C_SW_ENABLE
    uint16_t delay = i2c_para[i2c_name].delay_us;
#endif
#ifdef I2C_HW_ENABLE
    uint8_t i=200;
#endif
    I2C_TypeDef *i2c = i2c_hw[i2c_name].PERIPHERAL;

    assert_param(i2c_name < I2C_NUM);
    if (i2c_name >= I2C_NUM) return;

    if (i2c != 0)
    {
    #ifdef I2C_HW_ENABLE
        I2C_GenerateSTART(i2c, ENABLE);
        while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_MODE_SELECT))
        {
            if (--i == 0) break;
        }
    #endif
    }
    else
    {
    #ifdef I2C_SW_ENABLE
        i2c_delay_us(delay);
        I2C_SDA_HIGH(i2c_name);
        I2C_SCL_HIGH(i2c_name);
        i2c_delay_us(delay);
        I2C_SDA_LOW(i2c_name);
        i2c_delay_us(delay);                                                    // ����ʱ����߼�����
        I2C_SCL_LOW(i2c_name);
        I2C_SDA_HIGH(i2c_name);
        i2c_delay_us(delay);
    #endif
    }
}

/**
  ******************************************************************************
  * @brief  I2C�����ź�
  * @param  i2c_name
  *   @arg  ����ֵ��ο� i2c_name_t ���Ͷ����е�ö��ֵ
  * @retval none
  ******************************************************************************
  */
void i2c_stop(i2c_name_t i2c_name)
{
#ifdef I2C_SW_ENABLE
    uint16_t delay = i2c_para[i2c_name].delay_us;
#endif
    I2C_TypeDef *i2c = i2c_hw[i2c_name].PERIPHERAL;

    assert_param(i2c_name < I2C_NUM);
    if (i2c_name >= I2C_NUM) return;

    if (i2c != 0)
    {
    #ifdef I2C_HW_ENABLE
        I2C_GenerateSTOP(i2c, ENABLE);
    #endif
    }
    else
    {
    #ifdef I2C_SW_ENABLE
        I2C_SDA_LOW(i2c_name);
        i2c_delay_us(delay);
        I2C_SCL_HIGH(i2c_name);
        i2c_delay_us(delay);
        I2C_SDA_HIGH(i2c_name);
        i2c_delay_us(delay);
    #endif
    }
}

/**
  ******************************************************************************
  * @brief  ͨ��I2C����һ��7bit��ַ����, ׼��д���豸
  * @param  i2c_name
  *   @arg  ����ֵ��ο� i2c_name_t ���Ͷ����е�ö��ֵ
  * @param  addr,   Ҫ���͵�7bit��ַ, bit0��Ч.
  * @retval ����ֵ, I2C���豸��Ӧ���ź�
  ******************************************************************************
  */
i2c_ack_t i2c_tx_addr_wr_slaver(i2c_name_t i2c_name, uint8_t addr)
{
#ifdef I2C_HW_ENABLE
    uint8_t i = 200;
#endif
    i2c_ack_t ack = I2C_NO_ACK;
    I2C_TypeDef *i2c = i2c_hw[i2c_name].PERIPHERAL;

    assert_param(i2c_name < I2C_NUM);
    if (i2c_name >= I2C_NUM) return ack;

    if (i2c != 0)                                                               // ʹ��I2CӲ��
    {
    #ifdef I2C_HW_ENABLE
        I2C_Send7bitAddress(i2c, addr, I2C_Direction_Transmitter);
        ack = I2C_ACK;
        while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        {
            if ((--i == 0) || (I2C_GetFlagStatus(i2c, I2C_FLAG_AF) == SET))     // ����ʱ�޻���Ӧ��
            {
                ack = I2C_NO_ACK;
                break;
            }
        }
    #endif
    }
    else                                                                        // ���IO��ʵ��I2C
    {
    #ifdef I2C_SW_ENABLE
        addr &= 0xFE;                                                           // ���Ϊд���豸
        ack = i2c_tx_byte_sw(i2c_name, addr);
    #endif
    }

    return ack;
}

/**
  ******************************************************************************
  * @brief  ͨ��I2C����һ��7bit��ַ����, ׼�������豸
  * @param  i2c_name
  *   @arg  ����ֵ��ο� i2c_name_t ���Ͷ����е�ö��ֵ
  * @param  addr,   Ҫ���͵�7bit��ַ, bit0��Ч.
  * @retval ����ֵ, I2C���豸��Ӧ���ź�
  ******************************************************************************
  */
i2c_ack_t i2c_tx_addr_rd_slaver(i2c_name_t i2c_name, uint8_t addr)
{
#ifdef I2C_HW_ENABLE
    uint8_t i = 200;
#endif
    i2c_ack_t ack = I2C_NO_ACK;
    I2C_TypeDef *i2c = i2c_hw[i2c_name].PERIPHERAL;

    assert_param(i2c_name < I2C_NUM);
    if (i2c_name >= I2C_NUM) return ack;

    if (i2c != 0)                                                               // ʹ��I2CӲ��
    {
    #ifdef I2C_HW_ENABLE
        I2C_Send7bitAddress(i2c, addr, I2C_Direction_Receiver);
        ack = I2C_ACK;
        while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        {
            if ((--i == 0) || (I2C_GetFlagStatus(i2c, I2C_FLAG_AF) == SET))     // ����ʱ�޻���Ӧ��
            {
                ack = I2C_NO_ACK;
                break;
            }
        }
    #endif
    }
    else                                                                        // ���IO��ʵ��I2C
    {
    #ifdef I2C_SW_ENABLE
        addr |= 0x01;                                                           // ���Ϊ�����豸
        ack = i2c_tx_byte_sw(i2c_name, addr);
    #endif
    }

    return ack;
}

/**
  ******************************************************************************
  * @brief  ͨ��I2C����һ���ֽڵ�����
  * @param  i2c_name
  *   @arg  ����ֵ��ο� i2c_name_t ���Ͷ����е�ö��ֵ
  * @param  data,   Ҫ���͵�����
  * @retval ����ֵ, I2C���豸��Ӧ���ź�
  ******************************************************************************
  */
i2c_ack_t i2c_tx_byte(i2c_name_t i2c_name, uint8_t data)
{
#ifdef I2C_HW_ENABLE
    uint8_t i = 200;
#endif
    i2c_ack_t ack = I2C_NO_ACK;
    I2C_TypeDef *i2c = i2c_hw[i2c_name].PERIPHERAL;

    assert_param(i2c_name < I2C_NUM);
    if (i2c_name >= I2C_NUM) return ack;

    if (i2c != 0)                                                               // ʹ��I2CӲ��
    {
    #ifdef I2C_HW_ENABLE
        I2C_SendData(i2c, data);
        ack = I2C_ACK;
        while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            if ((--i == 0) || (I2C_GetFlagStatus(i2c, I2C_FLAG_AF) == SET))     // ����ʱ�޻���Ӧ��
            {
                ack = I2C_NO_ACK;
                break;
            }
        }
    #endif
    }
    else                                                                        // ���IO��ʵ��I2C
    {
    #ifdef I2C_SW_ENABLE
        ack = i2c_tx_byte_sw(i2c_name, data);
    #endif
    }

    return ack;
}

/**
  ******************************************************************************
  * @brief  ͨ��I2C����һ���ֽڵ�����
  * @param  i2c_name
  *   @arg  ����ֵ��ο� i2c_name_t ���Ͷ����е�ö��ֵ
  * @param  ack, �������ݺ������Ӧ��
  * @retval ����ֵ, �յ�������
  ******************************************************************************
  */
uint8_t i2c_rx_byte(i2c_name_t i2c_name, i2c_ack_t ack)
{
    uint8_t data;
    I2C_TypeDef *i2c = i2c_hw[i2c_name].PERIPHERAL;

    assert_param(i2c_name < I2C_NUM);
    if (i2c_name >= I2C_NUM) return 0xFF;

    if (i2c != 0)                                                               // ʹ��I2CӲ��
    {
    #ifdef I2C_HW_ENABLE
        if (ack == I2C_ACK) I2C_AcknowledgeConfig(i2c, ENABLE);                 // ��׼����ACK�ź�
        else                I2C_AcknowledgeConfig(i2c, DISABLE);
        while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_RECEIVED));
        data = I2C_ReceiveData(i2c);
    #endif
    }
    else                                                                        // ���IO��ʵ��I2C
    {
    #ifdef I2C_SW_ENABLE
        data = i2c_rx_byte_sw(i2c_name, ack);
    #endif
    }

    return(data);
}

#ifdef I2C_SW_ENABLE
/**
  ******************************************************************************
  * @brief  ģ��I2C дһ���ֽ�
  * @param  val,    Ҫд����ֽ�
  * @retval ����ֵ, ���豸Ӧ�����
  ******************************************************************************
  */
i2c_ack_t i2c_tx_byte_sw(i2c_name_t i2c_name, uint8_t val)
{
    i2c_ack_t ack;
    uint8_t  i = 8;
    uint16_t delay = i2c_para[i2c_name].delay_us;

    while(i--)                                                                  // ����һ���ֽڵ�����
    {
        if (val & 0x80) I2C_SDA_HIGH(i2c_name);
        else            I2C_SDA_LOW(i2c_name);
        i2c_delay_us(delay+1);
        I2C_SCL_HIGH(i2c_name);
        val <<= 1;
        i2c_delay_us(delay);
        I2C_SCL_LOW(i2c_name);
    }

    I2C_SDA_HIGH(i2c_name);
    i2c_delay_us(delay+2);                                                      // �˴������豸��ֵ�ʱ��������CLK�ź�
    ack = I2C_NO_ACK;
    if (I2C_SCL_HIGH(i2c_name) == 1)                                            // ���豸Ӧ������
    {
        if (I2C_SDA_LEVEL(i2c_name) == 0) ack = I2C_ACK;
    }
    i2c_delay_us(delay);
    I2C_SCL_LOW(i2c_name);
    i2c_delay_us(delay);

    return ack;
}

/**
  ******************************************************************************
  * @brief  ģ��I2C ��һ���ֽ�
  * @param  ack,    Ӧ��ֵ
  * @retval ����ֵ, �������ֽ�
  ******************************************************************************
  */
uint8_t i2c_rx_byte_sw(i2c_name_t i2c_name, uint8_t ack)
{
    uint8_t  data = 0, i = 8;
    uint16_t delay = i2c_para[i2c_name].delay_us;

    I2C_SDA_HIGH(i2c_name);                                                     // ʹSDA��������״̬
    while (i--)
    {
        i2c_delay_us(delay+1);
        I2C_SCL_HIGH(i2c_name);
        data <<= 1;
        if (I2C_SDA_LEVEL(i2c_name)) data++;
        i2c_delay_us(delay);
        I2C_SCL_LOW(i2c_name);
    }
    if (ack == I2C_ACK) I2C_SDA_LOW(i2c_name);
    else                I2C_SDA_HIGH(i2c_name);
    i2c_delay_us(delay+1);
    I2C_SCL_HIGH(i2c_name);
    i2c_delay_us(delay);
    I2C_SCL_LOW(i2c_name);
    i2c_delay_us(delay);

    return data;
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
