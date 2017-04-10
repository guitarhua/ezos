/**
  ******************************************************************************
  * @file    drv_i2s.c
  * @author  mousie
  * @version V1.0.0
  * @date    2012.3.1
  * @brief   使用主模式, 中断方式的 I2S 驱动程序, 工作于主机发送模式.
  *          仅限 STM32F10x 系列的 I2S2 使用. 可用于I2S接口的音频输出.
  *          目前仅 I2S_DataFormat_8b 格式可正常播放, 原因不明.
  ******************************************************************************
  */

/* Include -------------------------------------------------------------------*/
#include "drv_i2s.h"
#include <stdarg.h>



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_I2S
  * @{
  */
#ifdef I2S_ENABLE



/**
  ******************************************************************************
  * @addtogroup DRV_I2S_Configure
  ******************************************************************************
  * @{
  */
#include "stm32f10x.h"
#define i2s_enable_int(mask)            __set_PRIMASK(mask)                     ///< 宏定义UART中断打开函数, 方便代码移植
#define i2s_disable_int()               __get_PRIMASK(); __disable_irq()        ///< 宏定义UART中断关闭函数, 方便代码移植

#define TX_BUF_SIZE                     256                                     ///< 定义发缓冲区大小, 允许范围 2-65535

#if ((TX_BUF_SIZE < 2) || (TX_BUF_SIZE > 65535))
    #error "TX_BUF_SIZE define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_I2S_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2S_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2S_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
static uint8_t  stereo_flag;                                                    ///< 立体声标识
static uint16_t i2s_data_format;                                                ///< I2S 格式
static uint16_t tx_wr_index;                                                    ///< 发送缓冲区写索引
static uint16_t tx_rd_index;                                                    ///< 发送缓冲区读索引
static volatile uint16_t tx_counter;                                            ///< 发送缓冲区计数
static volatile uint16_t tx_buffer[TX_BUF_SIZE];                                ///< 发送缓存区
/**
  * @}
  */

/** @defgroup DRV_I2S_Private_Function
  * @brief    定义私有函数
  * @{
  */
static void i2s_tx_data(uint16_t data);                                         // 通过I2S发送一个数据
/**
  * @}
  */



/** @defgroup DRV_I2S_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  I2S初始化, 初始化缓存区, 并使能该I2S
  * @param  data_format, 设置 I2S 数据格式
  *   @arg  I2S_DataFormat_8b,          8位数据宽度,  采用16位数据宽度发送
  *   @arg  I2S_DataFormat_16b,         16位数据宽度
  *   @arg  I2S_DataFormat_16bextended, 16位数据宽度扩展为32位数据宽度发送
  *   @arg  I2S_DataFormat_24b,         24位数据宽度扩展为32位数据宽度发送
  *   @arg  I2S_DataFormat_32b,         32位数据宽度
  * @param  audio_freq, 设置音频文件采样率
  *   @arg  I2S_AudioFreq_8k,   8K采样率
  *   @arg  I2S_AudioFreq_11k,  11K采样率
  *   @arg  I2S_AudioFreq_16k,  16K采样率
  *   @arg  I2S_AudioFreq_22k,  22K采样率
  *   @arg  I2S_AudioFreq_32k,  32K采样率
  *   @arg  I2S_AudioFreq_44k,  44K采样率
  *   @arg  I2S_AudioFreq_48k,  48K采样率
  *   @arg  I2S_AudioFreq_96k,  96K采样率
  *   @arg  I2S_AudioFreq_192k, 192K采样率
  * @param  audio_track, 设置音频文件声道
  *   @arg  0,  单声道
  *   @arg  !0, 双声道
  * @retval none
  ******************************************************************************
  */
void i2s_init(uint16_t data_format, uint32_t audio_freq, uint8_t audio_track)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    I2S_InitTypeDef I2S_InitStructure;

    I2S_Cmd(SPI2, DISABLE);                                                     // 先禁止I2S功能
    SPI_I2S_DeInit(SPI2);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                   // 直接设置为最高优先级, 声音的传输速率要求较高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    stereo_flag = audio_track;
    i2s_data_format = data_format;
    if (data_format == I2S_DataFormat_8b) data_format = I2S_DataFormat_16b;

    I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
    I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
    I2S_InitStructure.I2S_DataFormat = data_format;
    I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
    I2S_InitStructure.I2S_AudioFreq = audio_freq;
    I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
    I2S_Init(SPI2, &I2S_InitStructure);

    i2s_buf_clear();
    I2S_Cmd(SPI2, ENABLE);
}

/**
  ******************************************************************************
  * @brief  禁止某个I2S
  * @param  none
  * @retval none
  ******************************************************************************
  */
void i2s_deinit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    I2S_Cmd(SPI2, DISABLE);
    SPI_I2S_DeInit(SPI2);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE);

    i2s_buf_clear();

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  ******************************************************************************
  * @brief  清空i2s缓存区
  * @param  none
  * @retval none
  ******************************************************************************
  */
void i2s_buf_clear(void)
{
    uint32_t mask;

    mask = i2s_disable_int();
    tx_wr_index = 0;
    tx_rd_index = 0;
    tx_counter  = 0;
    i2s_enable_int(mask);
}

/**
  ******************************************************************************
  * @brief  发送一个小端存储的数据
  * @param  data, 要发送的数据
  * @retval none
  ******************************************************************************
  */
void i2s_tx_data(uint16_t data)
{
    uint32_t mask;

    do
    {
        mask = i2s_disable_int();
        if (tx_counter < TX_BUF_SIZE) break;
        i2s_enable_int(mask);
    } while (1);

    tx_buffer[tx_wr_index] = data;
    tx_counter++;
    if (++tx_wr_index >= TX_BUF_SIZE) tx_wr_index = 0;
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, ENABLE);
    i2s_enable_int(mask);
}

/**
  ******************************************************************************
  * @brief  发送一串数据
  * @param  *pdata, 要发送的数据串指针
  * @param  length, 要发送的数据串长度
  * @retval none
  ******************************************************************************
*/
void i2s_tx(uint8_t *pdata, uint16_t length)
{
    uint16_t i, data1, data2;

    for (i=0; i < length;)
    {
        if (i2s_data_format == I2S_DataFormat_8b)
        {
            data1 = *pdata++;
            i2s_tx_data(data1);
            if (!stereo_flag) i2s_tx_data(data1);                               // 单声道, 需要放两遍
            ++i;
        }
        else if ((i2s_data_format == I2S_DataFormat_16b) || (i2s_data_format == I2S_DataFormat_16bextended))
        {
            data1  = *pdata++;
            data1 |= (*pdata++ << 8);
            i2s_tx_data(data1);
            if (!stereo_flag) i2s_tx_data(data1);                               // 单声道, 需要放两遍
            i += 2;
        }
        else if (i2s_data_format == I2S_DataFormat_24b)
        {
            data1 = *pdata++;
            data1 <<= 8;
            data2 = *pdata++;
            data2 |= (*pdata++ << 8);
            i2s_tx_data(data2);
            i2s_tx_data(data1);
            if (!stereo_flag)                                                   // 单声道, 需要放两遍
            {
                i2s_tx_data(data2);
                i2s_tx_data(data1);
            }
            i += 3;
        }
        else if (i2s_data_format == I2S_DataFormat_32b)
        {
            data1 = *pdata++;
            data1 |= (*pdata++ << 8);
            data2 = *pdata++;
            data2 |= (*pdata++ << 8);
            i2s_tx_data(data2);
            i2s_tx_data(data1);
            if (!stereo_flag)                                                   // 单声道, 需要放两遍
            {
                i2s_tx_data(data2);
                i2s_tx_data(data1);
            }
            i += 4;
        }
    }
}

/**
  ******************************************************************************
  * @brief  i2s的中断处理函数
  * @param  none
  * @retval none
  ******************************************************************************
*/
void SPI2_IRQHandler(void)
{
    if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE) == SET)                        // 发送寄存器空中断
    {
        if (tx_counter)                                                         // 发送缓存区有数据
        {
            SPI2->DR = tx_buffer[tx_rd_index];                                  // 发送数据
            if (++tx_rd_index >= TX_BUF_SIZE) tx_rd_index = 0;
            tx_counter--;
        }
        else                                                                    // 没有数据了, 关闭发送中断
        {
            SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);
        }
    }
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
