/**
  ******************************************************************************
  * @file    drv_i2s.c
  * @author  mousie
  * @version V1.0.0
  * @date    2012.3.1
  * @brief   ʹ����ģʽ, �жϷ�ʽ�� I2S ��������, ��������������ģʽ.
  *          ���� STM32F10x ϵ�е� I2S2 ʹ��. ������I2S�ӿڵ���Ƶ���.
  *          Ŀǰ�� I2S_DataFormat_8b ��ʽ����������, ԭ����.
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
#define i2s_enable_int(mask)            __set_PRIMASK(mask)                     ///< �궨��UART�жϴ򿪺���, ���������ֲ
#define i2s_disable_int()               __get_PRIMASK(); __disable_irq()        ///< �궨��UART�жϹرպ���, ���������ֲ

#define TX_BUF_SIZE                     256                                     ///< ���巢��������С, ����Χ 2-65535

#if ((TX_BUF_SIZE < 2) || (TX_BUF_SIZE > 65535))
    #error "TX_BUF_SIZE define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_I2S_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2S_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_I2S_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
static uint8_t  stereo_flag;                                                    ///< ��������ʶ
static uint16_t i2s_data_format;                                                ///< I2S ��ʽ
static uint16_t tx_wr_index;                                                    ///< ���ͻ�����д����
static uint16_t tx_rd_index;                                                    ///< ���ͻ�����������
static volatile uint16_t tx_counter;                                            ///< ���ͻ���������
static volatile uint16_t tx_buffer[TX_BUF_SIZE];                                ///< ���ͻ�����
/**
  * @}
  */

/** @defgroup DRV_I2S_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static void i2s_tx_data(uint16_t data);                                         // ͨ��I2S����һ������
/**
  * @}
  */



/** @defgroup DRV_I2S_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  I2S��ʼ��, ��ʼ��������, ��ʹ�ܸ�I2S
  * @param  data_format, ���� I2S ���ݸ�ʽ
  *   @arg  I2S_DataFormat_8b,          8λ���ݿ��,  ����16λ���ݿ�ȷ���
  *   @arg  I2S_DataFormat_16b,         16λ���ݿ��
  *   @arg  I2S_DataFormat_16bextended, 16λ���ݿ����չΪ32λ���ݿ�ȷ���
  *   @arg  I2S_DataFormat_24b,         24λ���ݿ����չΪ32λ���ݿ�ȷ���
  *   @arg  I2S_DataFormat_32b,         32λ���ݿ��
  * @param  audio_freq, ������Ƶ�ļ�������
  *   @arg  I2S_AudioFreq_8k,   8K������
  *   @arg  I2S_AudioFreq_11k,  11K������
  *   @arg  I2S_AudioFreq_16k,  16K������
  *   @arg  I2S_AudioFreq_22k,  22K������
  *   @arg  I2S_AudioFreq_32k,  32K������
  *   @arg  I2S_AudioFreq_44k,  44K������
  *   @arg  I2S_AudioFreq_48k,  48K������
  *   @arg  I2S_AudioFreq_96k,  96K������
  *   @arg  I2S_AudioFreq_192k, 192K������
  * @param  audio_track, ������Ƶ�ļ�����
  *   @arg  0,  ������
  *   @arg  !0, ˫����
  * @retval none
  ******************************************************************************
  */
void i2s_init(uint16_t data_format, uint32_t audio_freq, uint8_t audio_track)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    I2S_InitTypeDef I2S_InitStructure;

    I2S_Cmd(SPI2, DISABLE);                                                     // �Ƚ�ֹI2S����
    SPI_I2S_DeInit(SPI2);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                   // ֱ������Ϊ������ȼ�, �����Ĵ�������Ҫ��ϸ�
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
  * @brief  ��ֹĳ��I2S
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
  * @brief  ���i2s������
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
  * @brief  ����һ��С�˴洢������
  * @param  data, Ҫ���͵�����
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
  * @brief  ����һ������
  * @param  *pdata, Ҫ���͵����ݴ�ָ��
  * @param  length, Ҫ���͵����ݴ�����
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
            if (!stereo_flag) i2s_tx_data(data1);                               // ������, ��Ҫ������
            ++i;
        }
        else if ((i2s_data_format == I2S_DataFormat_16b) || (i2s_data_format == I2S_DataFormat_16bextended))
        {
            data1  = *pdata++;
            data1 |= (*pdata++ << 8);
            i2s_tx_data(data1);
            if (!stereo_flag) i2s_tx_data(data1);                               // ������, ��Ҫ������
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
            if (!stereo_flag)                                                   // ������, ��Ҫ������
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
            if (!stereo_flag)                                                   // ������, ��Ҫ������
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
  * @brief  i2s���жϴ�����
  * @param  none
  * @retval none
  ******************************************************************************
*/
void SPI2_IRQHandler(void)
{
    if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE) == SET)                        // ���ͼĴ������ж�
    {
        if (tx_counter)                                                         // ���ͻ�����������
        {
            SPI2->DR = tx_buffer[tx_rd_index];                                  // ��������
            if (++tx_rd_index >= TX_BUF_SIZE) tx_rd_index = 0;
            tx_counter--;
        }
        else                                                                    // û��������, �رշ����ж�
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
