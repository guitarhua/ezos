/**
  ******************************************************************************
  * @file    drv_i2c.c
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.16
  * @brief   提供 I2C 驱动程序. 主机模式, 7位地址. 同时支持IO口软件模拟和硬件I2C.
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
#define I2C_HW_ENABLE                                                           ///< 使能硬件I2C代码.
#define I2C_SW_ENABLE                                                           ///< 使能软件模拟I2C代码.

#include "drv_systick.h"
#define i2c_delay_us(time_us)           delay_us(time_us)                       ///< 设置I2C微秒延时函数, 用于产生软件模拟I2C的SCK信号波特率. 硬件I2C无需该函数.
/**
  * @}
  */



/** @defgroup DRV_I2C_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */
/// I2C硬件参数类型定义
typedef struct
{
#ifdef I2C_HW_ENABLE
    const uint32_t CLK;                                                         ///< 硬件I2C时钟模块
    const uint32_t REMAP;                                                       ///< 硬件I2C引脚重映射
#endif
    I2C_TypeDef *const PERIPHERAL;                                              ///< 硬件I2C模块名
    const uint32_t SCL_CLK;                                                     ///< SCL引脚时钟模块
    const uint32_t SDA_CLK;                                                     ///< SDA引脚时钟模块
    GPIO_TypeDef *const SCL_PORT;                                               ///< SCL引脚PORT口
    GPIO_TypeDef *const SDA_PORT;                                               ///< SDA引脚PORT口
    const uint16_t SCL_PIN;                                                     ///< SCL引脚PIN口
    const uint16_t SDA_PIN;                                                     ///< SDA引脚PIN口
} i2c_hw_t;

/// I2C参数类型定义
typedef struct
{
    uint16_t delay_us;                                                          ///< 软件模拟I2C用, 记录延时时间, 单位Us.
} i2c_para_t;
/**
  * @}
  */

/** @defgroup DRV_I2C_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2C_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
/// I2C硬件参数映射表
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

/// I2C参数表
#ifdef I2C_SW_ENABLE
static i2c_para_t i2c_para[I2C_NUM];
#endif
/**
  * @}
  */

/** @defgroup DRV_I2C_Private_Function
  * @brief    定义私有函数
  * @{
  */
#ifdef I2C_SW_ENABLE

static i2c_ack_t i2c_tx_byte_sw(i2c_name_t i2c_name, uint8_t val);              // I2C软件模拟发送字节
static uint8_t   i2c_rx_byte_sw(i2c_name_t i2c_name, uint8_t ack);              // I2C软件模拟接收字节

/// 软件模拟I2C PORT口初始化
__INLINE static void i2c_port_init(i2c_name_t i2c_name)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(i2c_hw[i2c_name].SCL_CLK | i2c_hw[i2c_name].SDA_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin   = i2c_hw[i2c_name].SCL_PIN;
    GPIO_Init(i2c_hw[i2c_name].SCL_PORT, &GPIO_InitStructure);                  // 设置I2C SCL引脚, 开漏输出
    GPIO_InitStructure.GPIO_Pin   = i2c_hw[i2c_name].SDA_PIN;
    GPIO_Init(i2c_hw[i2c_name].SDA_PORT, &GPIO_InitStructure);                  // 设置I2C SDA引脚, 开漏输出
}

/// I2C软件模拟SCL电平置高, 并等待真正拉高了SCL(从设备释放SCL信号), 返回1, 置高了. 0, 超时.
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

/// I2C软件模拟SCL电平置低
__INLINE static void I2C_SCL_LOW(i2c_name_t i2c_name)
{
    i2c_hw[i2c_name].SCL_PORT->BRR = i2c_hw[i2c_name].SCL_PIN;
}

/// I2C软件模拟SDA电平置高
__INLINE static void I2C_SDA_HIGH(i2c_name_t i2c_name)
{
    i2c_hw[i2c_name].SDA_PORT->BSRR = i2c_hw[i2c_name].SDA_PIN;
}

/// I2C软件模拟SDA电平置低
__INLINE static void I2C_SDA_LOW(i2c_name_t i2c_name)
{
    i2c_hw[i2c_name].SDA_PORT->BRR = i2c_hw[i2c_name].SDA_PIN;
}

/// 读取软件模拟I2C的SDA电平值
__INLINE static int32_t I2C_SDA_LEVEL(i2c_name_t i2c_name)
{
    return (i2c_hw[i2c_name].SDA_PORT->IDR & i2c_hw[i2c_name].SDA_PIN);
}
#endif

/// I2C PORT口禁用, 可不处理 或 配置成高阻输入 或 最省电的模式
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
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  I2C初始化函数
  * @param  i2c_name
  *   @arg  可用值请参考 i2c_name_t 类型定义中的枚举值
  * @param  baud_rate_khz
  *   @arg  1-400, I2C波特率有效值 1-400 Khz. 软件仿真误差非常大, 速率设置仅供参考.
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

    if (i2c != 0)                                                               // 使用I2C硬件
    {
    #ifdef I2C_HW_ENABLE
        if (i2c_hw[i2c_name].REMAP != 0)
        {
            GPIO_PinRemapConfig(i2c_hw[i2c_name].REMAP, ENABLE);                // 使能引脚重映射
            RCC_APB2PeriphClockCmd(i2c_hw[i2c_name].SCL_CLK |                   // 使能 I2C 的IO口时钟
                                   i2c_hw[i2c_name].SDA_CLK |
                                   RCC_APB2Periph_AFIO, ENABLE);                // 使能交替功能时钟
        }
        else
        {
            RCC_APB2PeriphClockCmd(i2c_hw[i2c_name].SCL_CLK  |                  // 使能 I2C 的IO口时钟
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
    else                                                                        // 软件IO口实现I2C
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
  * @brief  禁止某个I2C. 并将I2C寄存器恢复为默认值. 引脚设为悬空输入.
  * @param  i2c_name
  *   @arg  可用值请参考 i2c_name_t 类型定义中的枚举值
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
  * @brief  I2C起始信号
  * @param  i2c_name
  *   @arg  可用值请参考 i2c_name_t 类型定义中的枚举值
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
        i2c_delay_us(delay);                                                    // 此延时可提高兼容性
        I2C_SCL_LOW(i2c_name);
        I2C_SDA_HIGH(i2c_name);
        i2c_delay_us(delay);
    #endif
    }
}

/**
  ******************************************************************************
  * @brief  I2C结束信号
  * @param  i2c_name
  *   @arg  可用值请参考 i2c_name_t 类型定义中的枚举值
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
  * @brief  通过I2C发送一个7bit地址数据, 准备写从设备
  * @param  i2c_name
  *   @arg  可用值请参考 i2c_name_t 类型定义中的枚举值
  * @param  addr,   要发送的7bit地址, bit0无效.
  * @retval 返回值, I2C从设备的应答信号
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

    if (i2c != 0)                                                               // 使用I2C硬件
    {
    #ifdef I2C_HW_ENABLE
        I2C_Send7bitAddress(i2c, addr, I2C_Direction_Transmitter);
        ack = I2C_ACK;
        while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        {
            if ((--i == 0) || (I2C_GetFlagStatus(i2c, I2C_FLAG_AF) == SET))     // 超过时限或无应答
            {
                ack = I2C_NO_ACK;
                break;
            }
        }
    #endif
    }
    else                                                                        // 软件IO口实现I2C
    {
    #ifdef I2C_SW_ENABLE
        addr &= 0xFE;                                                           // 标记为写从设备
        ack = i2c_tx_byte_sw(i2c_name, addr);
    #endif
    }

    return ack;
}

/**
  ******************************************************************************
  * @brief  通过I2C发送一个7bit地址数据, 准备读从设备
  * @param  i2c_name
  *   @arg  可用值请参考 i2c_name_t 类型定义中的枚举值
  * @param  addr,   要发送的7bit地址, bit0无效.
  * @retval 返回值, I2C从设备的应答信号
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

    if (i2c != 0)                                                               // 使用I2C硬件
    {
    #ifdef I2C_HW_ENABLE
        I2C_Send7bitAddress(i2c, addr, I2C_Direction_Receiver);
        ack = I2C_ACK;
        while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        {
            if ((--i == 0) || (I2C_GetFlagStatus(i2c, I2C_FLAG_AF) == SET))     // 超过时限或无应答
            {
                ack = I2C_NO_ACK;
                break;
            }
        }
    #endif
    }
    else                                                                        // 软件IO口实现I2C
    {
    #ifdef I2C_SW_ENABLE
        addr |= 0x01;                                                           // 标记为读从设备
        ack = i2c_tx_byte_sw(i2c_name, addr);
    #endif
    }

    return ack;
}

/**
  ******************************************************************************
  * @brief  通过I2C发送一个字节的数据
  * @param  i2c_name
  *   @arg  可用值请参考 i2c_name_t 类型定义中的枚举值
  * @param  data,   要发送的数据
  * @retval 返回值, I2C从设备的应答信号
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

    if (i2c != 0)                                                               // 使用I2C硬件
    {
    #ifdef I2C_HW_ENABLE
        I2C_SendData(i2c, data);
        ack = I2C_ACK;
        while(!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            if ((--i == 0) || (I2C_GetFlagStatus(i2c, I2C_FLAG_AF) == SET))     // 超过时限或无应答
            {
                ack = I2C_NO_ACK;
                break;
            }
        }
    #endif
    }
    else                                                                        // 软件IO口实现I2C
    {
    #ifdef I2C_SW_ENABLE
        ack = i2c_tx_byte_sw(i2c_name, data);
    #endif
    }

    return ack;
}

/**
  ******************************************************************************
  * @brief  通过I2C接收一个字节的数据
  * @param  i2c_name
  *   @arg  可用值请参考 i2c_name_t 类型定义中的枚举值
  * @param  ack, 接收数据后的主机应答
  * @retval 返回值, 收到的数据
  ******************************************************************************
  */
uint8_t i2c_rx_byte(i2c_name_t i2c_name, i2c_ack_t ack)
{
    uint8_t data;
    I2C_TypeDef *i2c = i2c_hw[i2c_name].PERIPHERAL;

    assert_param(i2c_name < I2C_NUM);
    if (i2c_name >= I2C_NUM) return 0xFF;

    if (i2c != 0)                                                               // 使用I2C硬件
    {
    #ifdef I2C_HW_ENABLE
        if (ack == I2C_ACK) I2C_AcknowledgeConfig(i2c, ENABLE);                 // 先准备好ACK信号
        else                I2C_AcknowledgeConfig(i2c, DISABLE);
        while (!I2C_CheckEvent(i2c, I2C_EVENT_MASTER_BYTE_RECEIVED));
        data = I2C_ReceiveData(i2c);
    #endif
    }
    else                                                                        // 软件IO口实现I2C
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
  * @brief  模拟I2C 写一个字节
  * @param  val,    要写入的字节
  * @retval 返回值, 从设备应答情况
  ******************************************************************************
  */
i2c_ack_t i2c_tx_byte_sw(i2c_name_t i2c_name, uint8_t val)
{
    i2c_ack_t ack;
    uint8_t  i = 8;
    uint16_t delay = i2c_para[i2c_name].delay_us;

    while(i--)                                                                  // 发送一个字节的数据
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
    i2c_delay_us(delay+2);                                                      // 此处给从设备充分的时间来控制CLK信号
    ack = I2C_NO_ACK;
    if (I2C_SCL_HIGH(i2c_name) == 1)                                            // 从设备应答周期
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
  * @brief  模拟I2C 读一个字节
  * @param  ack,    应答值
  * @retval 返回值, 读到的字节
  ******************************************************************************
  */
uint8_t i2c_rx_byte_sw(i2c_name_t i2c_name, uint8_t ack)
{
    uint8_t  data = 0, i = 8;
    uint16_t delay = i2c_para[i2c_name].delay_us;

    I2C_SDA_HIGH(i2c_name);                                                     // 使SDA处于输入状态
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
