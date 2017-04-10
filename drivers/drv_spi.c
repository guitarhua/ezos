/**
  ******************************************************************************
  * @file    drv_spi.c
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.15
  * @brief   提供 SPI 驱动程序. 主机模式. 同时支持IO口软件模拟和硬件SPI.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "drv_spi.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_SPI
  * @{
  */
#if (SPI_NUM > 0)



/**
  ******************************************************************************
  * @addtogroup DRV_SPI_Configure
  ******************************************************************************
  * @{
  */
#define SPI_HW_ENABLE                                                           ///< 使能硬件SPI代码.
#define SPI_SW_ENABLE                                                           ///< 使能软件模拟SPI代码.

#include "drv_systick.h"
#define spi_delay_us(time)              delay_us(time)                          ///< 设置SPI微秒延时函数, 用于产生软件模拟SPI的SCK信号波特率. 硬件SPI无需该函数
/**
  * @}
  */



/** @defgroup DRV_SPI_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */
/// SPI硬件参数类型定义
typedef struct
{
#ifdef SPI_HW_ENABLE
    const uint32_t CLK;                                                         ///< SPI的时钟模块
    const uint32_t REMAP;                                                       ///< 引脚重映射
#endif
    SPI_TypeDef *const PERIPHERAL;                                              ///< 硬件SPI模块名
    const uint32_t SCK_CLK;                                                     ///< SCK 引脚的时钟模块
    const uint32_t MISO_CLK;                                                    ///< MISO引脚的时钟模块
    const uint32_t MOSI_CLK;                                                    ///< MOSI引脚的时钟模块
    GPIO_TypeDef *const SCK_PORT;                                               ///< SCK 引脚的PORT口
    GPIO_TypeDef *const MISO_PORT;                                              ///< MISO引脚的PORT口
    GPIO_TypeDef *const MOSI_PORT;                                              ///< MOSI引脚的PORT口
    const uint16_t SCK_PIN;                                                     ///< SCK 引脚的PIN口
    const uint16_t MISO_PIN;                                                    ///< MISO引脚的PIN口
    const uint16_t MOSI_PIN;                                                    ///< MOSI引脚的PIN口
} spi_hw_t;

/// SPI参数类型定义
typedef struct
{
    uint8_t  type;                                                              ///< 软件模拟SPI用, 记录SPI传输类型
    uint16_t delay_us;                                                          ///< 软件模拟SPI用, 记录延时时间, 单位Us.
} spi_para_t;
/**
  * @}
  */

/** @defgroup DRV_SPI_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SPI_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
/// SPI硬件参数映射表
static const spi_hw_t spi_hw[SPI_NUM] = {
#if (SPI_NUM > 0)
    {
#ifdef SPI_HW_ENABLE
    SPI0_CLK,           SPI0_GPIO_REMAP,
#endif
    SPI0_PERIPHERAL,
    SPI0_SCK_CLK,       SPI0_MISO_CLK,      SPI0_MOSI_CLK,
    SPI0_SCK_PORT,      SPI0_MISO_PORT,     SPI0_MOSI_PORT,
    SPI0_SCK_PIN,       SPI0_MISO_PIN,      SPI0_MOSI_PIN,  },
#endif

#if (SPI_NUM > 1)
    {
#ifdef SPI_HW_ENABLE
    SPI1_CLK,           SPI1_GPIO_REMAP,
#endif
    SPI1_PERIPHERAL,
    SPI1_SCK_CLK,       SPI1_MISO_CLK,      SPI1_MOSI_CLK,
    SPI1_SCK_PORT,      SPI1_MISO_PORT,     SPI1_MOSI_PORT,
    SPI1_SCK_PIN,       SPI1_MISO_PIN,      SPI1_MOSI_PIN,  },
#endif

#if (SPI_NUM > 2)
    {
#ifdef SPI_HW_ENABLE
    SPI2_CLK,           SPI2_GPIO_REMAP,
#endif
    SPI2_PERIPHERAL,
    SPI2_SCK_CLK,       SPI2_MISO_CLK,      SPI2_MOSI_CLK,
    SPI2_SCK_PORT,      SPI2_MISO_PORT,     SPI2_MOSI_PORT,
    SPI2_SCK_PIN,       SPI2_MISO_PIN,      SPI2_MOSI_PIN,  },
#endif

#if (SPI_NUM > 3)
    {
#ifdef SPI_HW_ENABLE
    SPI3_CLK,           SPI3_GPIO_REMAP,
#endif
    SPI3_PERIPHERAL,
    SPI3_SCK_CLK,       SPI3_MISO_CLK,      SPI3_MOSI_CLK,
    SPI3_SCK_PORT,      SPI3_MISO_PORT,     SPI3_MOSI_PORT,
    SPI3_SCK_PIN,       SPI3_MISO_PIN,      SPI3_MOSI_PIN,  },
#endif

#if (SPI_NUM > 4)
    {
#ifdef SPI_HW_ENABLE
    SPI4_CLK,           SPI4_GPIO_REMAP,
#endif
    SPI4_PERIPHERAL,
    SPI4_SCK_CLK,       SPI4_MISO_CLK,      SPI4_MOSI_CLK,
    SPI4_SCK_PORT,      SPI4_MISO_PORT,     SPI4_MOSI_PORT,
    SPI4_SCK_PIN,       SPI4_MISO_PIN,      SPI4_MOSI_PIN,  },
#endif
};

/// SPI参数表
#ifdef SPI_SW_ENABLE
static spi_para_t spi_para[SPI_NUM];
#endif
/**
  * @}
  */

/** @defgroup DRV_SPI_Private_Function
  * @brief    定义私有函数
  * @{
  */
#ifdef SPI_SW_ENABLE
/// 软件模拟SPI PORT口初始化
__INLINE static void spi_port_init(spi_name_t spi_name)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(spi_hw[spi_name].SCK_CLK  |
                           spi_hw[spi_name].MISO_CLK |
                           spi_hw[spi_name].MOSI_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;                           // 设置SPI SCK引脚, 推拉输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].SCK_PIN;
    GPIO_Init(spi_hw[spi_name].SCK_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].MOSI_PIN;                  // 设置SPI MOSI引脚, 推拉输出
    GPIO_Init(spi_hw[spi_name].MOSI_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;                      // 设置SPI MISO引脚, 高阻输入
    GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].MISO_PIN;
    GPIO_Init(spi_hw[spi_name].MISO_PORT, &GPIO_InitStructure);
}

/// SPI软件模拟SCK电平置高
__INLINE static void SPI_SCK_HIGH(spi_name_t spi_name)
{
    spi_hw[spi_name].SCK_PORT->BSRR  = spi_hw[spi_name].SCK_PIN;
}

/// SPI软件模拟SCK电平置低
__INLINE static void SPI_SCK_LOW(spi_name_t spi_name)
{
    spi_hw[spi_name].SCK_PORT->BRR   = spi_hw[spi_name].SCK_PIN;
}

/// SPI软件模拟MOSI电平置高
__INLINE static void SPI_MOSI_HIGH(spi_name_t spi_name)
{
    spi_hw[spi_name].MOSI_PORT->BSRR = spi_hw[spi_name].MOSI_PIN;
}

/// SPI软件模拟MOSI电平置低
__INLINE static void SPI_MOSI_LOW(spi_name_t spi_name)
{
    spi_hw[spi_name].MOSI_PORT->BRR  = spi_hw[spi_name].MOSI_PIN;
}

/// 读取软件模拟SPI的MISO电平值
__INLINE static int32_t SPI_MISO_LEVEL(spi_name_t spi_name)
{
    return (spi_hw[spi_name].MISO_PORT->IDR & spi_hw[spi_name].MISO_PIN);
}
#endif

/// SPI PORT口禁用, 可不处理 或 配置成高阻输入 或 最省电的模式
__INLINE static void spi_port_deinit(spi_name_t spi_name)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].SCK_PIN;
    GPIO_Init(spi_hw[spi_name].SCK_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].MOSI_PIN;
    GPIO_Init(spi_hw[spi_name].MOSI_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].MISO_PIN;
    GPIO_Init(spi_hw[spi_name].MISO_PORT, &GPIO_InitStructure);
}
/**
  * @}
  */



/** @defgroup DRV_SPI_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  SPI初始化函数, 必须在初始化完成后再配置SPI的SS引脚
  * @param  spi_name
  *   @arg  可用值请参考 spi_name_t 类型定义中的枚举值
  * @param  spi_type, 设置SPI的传输类型
  *   @arg  SPI_TYPE_HIGH_EDGE1_MSB,    SCK默认高电平, 下降沿检测, MSB模式
  *   @arg  SPI_TYPE_HIGH_EDGE1_LSB,    SCK默认高电平, 下降沿检测, LSB模式
  *   @arg  SPI_TYPE_HIGH_EDGE2_MSB,    SCK默认高电平, 上升沿检测, MSB模式
  *   @arg  SPI_TYPE_HIGH_EDGE2_LSB,    SCK默认高电平, 上升沿检测, LSB模式
  *   @arg  SPI_TYPE_LOW_EDGE1_MSB,     SCK默认低电平, 上升沿检测, MSB模式
  *   @arg  SPI_TYPE_LOW_EDGE1_LSB,     SCK默认低电平, 上升沿检测, LSB模式
  *   @arg  SPI_TYPE_LOW_EDGE2_MSB,     SCK默认低电平, 下降沿检测, MSB模式
  *   @arg  SPI_TYPE_LOW_EDGE2_LSB,     SCK默认低电平, 下降沿检测, LSB模式
  * @param  baud_rate, 设置SPI波特率.
  *   对于硬件SPI, SPI波特率通过对APB总线预分频设置, 有效值如下:
  *   @arg   SPI_BaudRatePrescaler_2,    对APB总线2分频后作为SPI时钟信号
  *   @arg   SPI_BaudRatePrescaler_4,    对APB总线4分频后作为SPI时钟信号
  *   @arg   SPI_BaudRatePrescaler_8,    按上类推
  *   @arg   SPI_BaudRatePrescaler_16
  *   @arg   SPI_BaudRatePrescaler_32
  *   @arg   SPI_BaudRatePrescaler_64
  *   @arg   SPI_BaudRatePrescaler_128
  *   @arg   SPI_BaudRatePrescaler_256
  *   对于软件模拟SPI, 直接设置其波特率值, 单位为Khz.
  *   @arg   1-500,                      软件模拟SPI波特率有效值 1-500 Khz.
  *   @arg   0,                          表示不延时. 可达约2MHz
  * @retval none
  ******************************************************************************
  */
void spi_init(spi_name_t spi_name, spi_type_t spi_type, uint16_t baud_rate)
{
#ifdef SPI_HW_ENABLE
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
#endif
    SPI_TypeDef *spi = spi_hw[spi_name].PERIPHERAL;

    assert_param(spi_name < SPI_NUM);
    assert_param(IS_SPI_TYPE(spi_type));
    if (spi_name >= SPI_NUM) return;

    if (spi != 0)                                                               // 使用SPI硬件
    {
    #ifdef SPI_HW_ENABLE
        assert_param(IS_SPI_BAUDRATE_PRESCALER(baud_rate));

        if (spi_hw[spi_name].REMAP != 0)
        {
            GPIO_PinRemapConfig(spi_hw[spi_name].REMAP, ENABLE);                // 使能引脚重映射
            RCC_APB2PeriphClockCmd(spi_hw[spi_name].SCK_CLK  |                  // 使能 SPI 的IO口时钟
                                   spi_hw[spi_name].MISO_CLK |
                                   spi_hw[spi_name].MOSI_CLK |
                                   RCC_APB2Periph_AFIO, ENABLE);                // 使能交替功能时钟
        }
        else
        {
            RCC_APB2PeriphClockCmd(spi_hw[spi_name].SCK_CLK  |                  // 使能 SPI 的IO口时钟
                                   spi_hw[spi_name].MISO_CLK |
                                   spi_hw[spi_name].MOSI_CLK, ENABLE);
        }

        if (spi_hw[spi_name].CLK == RCC_APB2Periph_SPI1)                        // 使能 SPI 模块
            RCC_APB2PeriphClockCmd(spi_hw[spi_name].CLK, ENABLE);
        else
            RCC_APB1PeriphClockCmd(spi_hw[spi_name].CLK, ENABLE);

        SPI_Cmd(spi, DISABLE);                                                  // 禁用SPI模块
        SPI_I2S_DeInit(spi);                                                    // 重置SPI模块参数

        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].SCK_PIN;               // 设置SPI SCK引脚
        GPIO_Init(spi_hw[spi_name].SCK_PORT, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].MOSI_PIN;              // 设置SPI MOSI引脚
        GPIO_Init(spi_hw[spi_name].MOSI_PORT, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].MISO_PIN;              // 设置SPI MISO引脚
        GPIO_Init(spi_hw[spi_name].MISO_PORT, &GPIO_InitStructure);

        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      // 设置SPI
        SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
        SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
        SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
        SPI_InitStructure.SPI_CRCPolynomial = 7;
        SPI_InitStructure.SPI_BaudRatePrescaler = baud_rate;                    // 波特率设置
        if (spi_type & 0x04) SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;        // 设置SPI起始电平
        else                 SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        if (spi_type & 0x02) SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;       // 设置SPI检测边沿
        else                 SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
        if (spi_type & 0x01) SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_LSB;// 设置SPI位发送顺序
        else                 SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;

        SPI_Init(spi, &SPI_InitStructure);                                      // 使能SPI
        SPI_Cmd(spi, ENABLE);
    #endif
    }
    else                                                                        // 软件IO口实现SPI
    {
    #ifdef SPI_SW_ENABLE
        assert_param(baud_rate <= 500);

        spi_port_init(spi_name);
        SPI_MOSI_LOW(spi_name);
        if (spi_type & 0x04) SPI_SCK_HIGH(spi_name);
        else                 SPI_SCK_LOW(spi_name);

        spi_para[spi_name].type = (uint8_t)spi_type;                            // 记录SPI模式和波特率
        if (baud_rate) spi_para[spi_name].delay_us = (500 / baud_rate);
        else           spi_para[spi_name].delay_us = 0;
    #endif
    }
}

/**
  ******************************************************************************
  * @brief  禁止某个SPI. 并将SPI寄存器恢复为默认值. 引脚设为悬空输入.
  * @param  spi_name
  *   @arg  可用值请参考 spi_name_t 类型定义中的枚举值
  * @retval none
  ******************************************************************************
  */
void spi_deinit(spi_name_t spi_name)
{
    assert_param(spi_name < SPI_NUM);
    if (spi_name >= SPI_NUM) return;

#ifdef SPI_HW_ENABLE
    if (spi_hw[spi_name].PERIPHERAL != 0)
    {
        SPI_Cmd(spi_hw[spi_name].PERIPHERAL, DISABLE);
        SPI_I2S_DeInit(spi_hw[spi_name].PERIPHERAL);

        if (spi_hw[spi_name].CLK == RCC_APB2Periph_SPI1)
            RCC_APB2PeriphClockCmd(spi_hw[spi_name].CLK, DISABLE);
        else
            RCC_APB1PeriphClockCmd(spi_hw[spi_name].CLK, DISABLE);
    }
#endif

    spi_port_deinit(spi_name);
}

/**
  ******************************************************************************
  * @brief  SPI收发一个字节, 发送完成后返回本次通讯读取的数据.
  * @param  spi_name
  *   @arg  可用值请参考 spi_name_t 类型定义中的枚举值
  * @param  send,   要发送的数据
  * @retval 返回值, 收到的数据
  ******************************************************************************
  */
uint8_t spi_tx_rx_byte(spi_name_t spi_name, uint8_t send)
{
#ifdef SPI_SW_ENABLE
    uint8_t  i;
    uint16_t delay;
#endif
    uint8_t receive = 0;
    SPI_TypeDef *spi = spi_hw[spi_name].PERIPHERAL;

    assert_param(spi_name < SPI_NUM);
    if (spi_name >= SPI_NUM) return 0xFF;

    if (spi != 0)
    {
    #ifdef SPI_HW_ENABLE
        while (SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(spi, send);

        while (SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_RXNE) == RESET);
        receive = SPI_I2S_ReceiveData(spi);
    #endif
    }
    else
    {
    #ifdef SPI_SW_ENABLE
        delay = spi_para[spi_name].delay_us;

        switch (spi_para[spi_name].type)
        {
        case SPI_TYPE_HIGH_EDGE1_MSB: {
            SPI_SCK_HIGH(spi_name);
            for (i=8; i>0; i--)
            {
                if (send & 0x80) SPI_MOSI_HIGH(spi_name);
                else             SPI_MOSI_LOW(spi_name);
                send <<= 1;
                spi_delay_us(delay);
                SPI_SCK_LOW(spi_name);
                receive <<= 1;
                if (SPI_MISO_LEVEL(spi_name)) receive |= 0x01;
                spi_delay_us(delay);
                SPI_SCK_HIGH(spi_name);
            }
            break; }
        case SPI_TYPE_HIGH_EDGE1_LSB: {
            SPI_SCK_HIGH(spi_name);
            for (i=8; i>0; i--)
            {
                if (send & 0x01) SPI_MOSI_HIGH(spi_name);
                else             SPI_MOSI_LOW(spi_name);
                send >>= 1;
                spi_delay_us(delay);
                SPI_SCK_LOW(spi_name);
                receive >>= 1;
                if (SPI_MISO_LEVEL(spi_name)) receive |= 0x80;
                spi_delay_us(delay);
                SPI_SCK_HIGH(spi_name);
            }
            break; }
        case SPI_TYPE_HIGH_EDGE2_MSB: {
            SPI_SCK_HIGH(spi_name);
            for (i=8; i>0; i--)
            {
                SPI_SCK_LOW(spi_name);
                if (send & 0x80) SPI_MOSI_HIGH(spi_name);
                else             SPI_MOSI_LOW(spi_name);
                send <<= 1;
                spi_delay_us(delay);
                SPI_SCK_HIGH(spi_name);
                receive <<= 1;
                if (SPI_MISO_LEVEL(spi_name)) receive |= 0x01;
                spi_delay_us(delay);
            }
            break; }
        case SPI_TYPE_HIGH_EDGE2_LSB: {
            SPI_SCK_HIGH(spi_name);
            for (i=8; i>0; i--)
            {
                SPI_SCK_LOW(spi_name);
                if (send & 0x01) SPI_MOSI_HIGH(spi_name);
                else             SPI_MOSI_LOW(spi_name);
                send >>= 1;
                spi_delay_us(delay);
                SPI_SCK_HIGH(spi_name);
                receive >>= 1;
                if (SPI_MISO_LEVEL(spi_name)) receive |= 0x80;
                spi_delay_us(delay);
            }
            break; }
        case SPI_TYPE_LOW_EDGE1_MSB: {
            SPI_SCK_LOW(spi_name);
            for (i=8; i>0; i--)
            {
                if (send & 0x80) SPI_MOSI_HIGH(spi_name);
                else             SPI_MOSI_LOW(spi_name);
                send <<= 1;
                spi_delay_us(delay);
                SPI_SCK_HIGH(spi_name);
                receive <<= 1;
                if (SPI_MISO_LEVEL(spi_name)) receive |= 0x01;
                spi_delay_us(delay);
                SPI_SCK_LOW(spi_name);
            }
            break; }
        case SPI_TYPE_LOW_EDGE1_LSB: {
            SPI_SCK_LOW(spi_name);
            for (i=8; i>0; i--)
            {
                if (send & 0x01) SPI_MOSI_HIGH(spi_name);
                else             SPI_MOSI_LOW(spi_name);
                send >>= 1;
                spi_delay_us(delay);
                SPI_SCK_HIGH(spi_name);
                receive >>= 1;
                if (SPI_MISO_LEVEL(spi_name)) receive |= 0x80;
                spi_delay_us(delay);
                SPI_SCK_LOW(spi_name);
            }
            break; }
        case SPI_TYPE_LOW_EDGE2_MSB: {
            SPI_SCK_LOW(spi_name);
            for (i=8; i>0; i--)
            {
                SPI_SCK_HIGH(spi_name);
                if (send & 0x80) SPI_MOSI_HIGH(spi_name);
                else             SPI_MOSI_LOW(spi_name);
                send <<= 1;
                spi_delay_us(delay);
                SPI_SCK_LOW(spi_name);
                receive <<= 1;
                if (SPI_MISO_LEVEL(spi_name)) receive |= 0x01;
                spi_delay_us(delay);
            }
            break; }
        case SPI_TYPE_LOW_EDGE2_LSB: {
            SPI_SCK_LOW(spi_name);
            for (i=8; i>0; i--)
            {
                SPI_SCK_HIGH(spi_name);
                if (send & 0x01) SPI_MOSI_HIGH(spi_name);
                else             SPI_MOSI_LOW(spi_name);
                send >>= 1;
                spi_delay_us(delay);
                SPI_SCK_LOW(spi_name);
                receive >>= 1;
                if (SPI_MISO_LEVEL(spi_name)) receive |= 0x80;
                spi_delay_us(delay);
            }
            break; }
        }
        spi_delay_us(delay);
        spi_delay_us(delay);
    #endif
    }

    return receive;
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
