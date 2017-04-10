/**
  ******************************************************************************
  * @file    drv_spi.c
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.15
  * @brief   �ṩ SPI ��������. ����ģʽ. ͬʱ֧��IO�����ģ���Ӳ��SPI.
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
#define SPI_HW_ENABLE                                                           ///< ʹ��Ӳ��SPI����.
#define SPI_SW_ENABLE                                                           ///< ʹ�����ģ��SPI����.

#include "drv_systick.h"
#define spi_delay_us(time)              delay_us(time)                          ///< ����SPI΢����ʱ����, ���ڲ������ģ��SPI��SCK�źŲ�����. Ӳ��SPI����ú���
/**
  * @}
  */



/** @defgroup DRV_SPI_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */
/// SPIӲ���������Ͷ���
typedef struct
{
#ifdef SPI_HW_ENABLE
    const uint32_t CLK;                                                         ///< SPI��ʱ��ģ��
    const uint32_t REMAP;                                                       ///< ������ӳ��
#endif
    SPI_TypeDef *const PERIPHERAL;                                              ///< Ӳ��SPIģ����
    const uint32_t SCK_CLK;                                                     ///< SCK ���ŵ�ʱ��ģ��
    const uint32_t MISO_CLK;                                                    ///< MISO���ŵ�ʱ��ģ��
    const uint32_t MOSI_CLK;                                                    ///< MOSI���ŵ�ʱ��ģ��
    GPIO_TypeDef *const SCK_PORT;                                               ///< SCK ���ŵ�PORT��
    GPIO_TypeDef *const MISO_PORT;                                              ///< MISO���ŵ�PORT��
    GPIO_TypeDef *const MOSI_PORT;                                              ///< MOSI���ŵ�PORT��
    const uint16_t SCK_PIN;                                                     ///< SCK ���ŵ�PIN��
    const uint16_t MISO_PIN;                                                    ///< MISO���ŵ�PIN��
    const uint16_t MOSI_PIN;                                                    ///< MOSI���ŵ�PIN��
} spi_hw_t;

/// SPI�������Ͷ���
typedef struct
{
    uint8_t  type;                                                              ///< ���ģ��SPI��, ��¼SPI��������
    uint16_t delay_us;                                                          ///< ���ģ��SPI��, ��¼��ʱʱ��, ��λUs.
} spi_para_t;
/**
  * @}
  */

/** @defgroup DRV_SPI_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SPI_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
/// SPIӲ������ӳ���
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

/// SPI������
#ifdef SPI_SW_ENABLE
static spi_para_t spi_para[SPI_NUM];
#endif
/**
  * @}
  */

/** @defgroup DRV_SPI_Private_Function
  * @brief    ����˽�к���
  * @{
  */
#ifdef SPI_SW_ENABLE
/// ���ģ��SPI PORT�ڳ�ʼ��
__INLINE static void spi_port_init(spi_name_t spi_name)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(spi_hw[spi_name].SCK_CLK  |
                           spi_hw[spi_name].MISO_CLK |
                           spi_hw[spi_name].MOSI_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;                           // ����SPI SCK����, �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].SCK_PIN;
    GPIO_Init(spi_hw[spi_name].SCK_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].MOSI_PIN;                  // ����SPI MOSI����, �������
    GPIO_Init(spi_hw[spi_name].MOSI_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;                      // ����SPI MISO����, ��������
    GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].MISO_PIN;
    GPIO_Init(spi_hw[spi_name].MISO_PORT, &GPIO_InitStructure);
}

/// SPI���ģ��SCK��ƽ�ø�
__INLINE static void SPI_SCK_HIGH(spi_name_t spi_name)
{
    spi_hw[spi_name].SCK_PORT->BSRR  = spi_hw[spi_name].SCK_PIN;
}

/// SPI���ģ��SCK��ƽ�õ�
__INLINE static void SPI_SCK_LOW(spi_name_t spi_name)
{
    spi_hw[spi_name].SCK_PORT->BRR   = spi_hw[spi_name].SCK_PIN;
}

/// SPI���ģ��MOSI��ƽ�ø�
__INLINE static void SPI_MOSI_HIGH(spi_name_t spi_name)
{
    spi_hw[spi_name].MOSI_PORT->BSRR = spi_hw[spi_name].MOSI_PIN;
}

/// SPI���ģ��MOSI��ƽ�õ�
__INLINE static void SPI_MOSI_LOW(spi_name_t spi_name)
{
    spi_hw[spi_name].MOSI_PORT->BRR  = spi_hw[spi_name].MOSI_PIN;
}

/// ��ȡ���ģ��SPI��MISO��ƽֵ
__INLINE static int32_t SPI_MISO_LEVEL(spi_name_t spi_name)
{
    return (spi_hw[spi_name].MISO_PORT->IDR & spi_hw[spi_name].MISO_PIN);
}
#endif

/// SPI PORT�ڽ���, �ɲ����� �� ���óɸ������� �� ��ʡ���ģʽ
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
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  SPI��ʼ������, �����ڳ�ʼ����ɺ�������SPI��SS����
  * @param  spi_name
  *   @arg  ����ֵ��ο� spi_name_t ���Ͷ����е�ö��ֵ
  * @param  spi_type, ����SPI�Ĵ�������
  *   @arg  SPI_TYPE_HIGH_EDGE1_MSB,    SCKĬ�ϸߵ�ƽ, �½��ؼ��, MSBģʽ
  *   @arg  SPI_TYPE_HIGH_EDGE1_LSB,    SCKĬ�ϸߵ�ƽ, �½��ؼ��, LSBģʽ
  *   @arg  SPI_TYPE_HIGH_EDGE2_MSB,    SCKĬ�ϸߵ�ƽ, �����ؼ��, MSBģʽ
  *   @arg  SPI_TYPE_HIGH_EDGE2_LSB,    SCKĬ�ϸߵ�ƽ, �����ؼ��, LSBģʽ
  *   @arg  SPI_TYPE_LOW_EDGE1_MSB,     SCKĬ�ϵ͵�ƽ, �����ؼ��, MSBģʽ
  *   @arg  SPI_TYPE_LOW_EDGE1_LSB,     SCKĬ�ϵ͵�ƽ, �����ؼ��, LSBģʽ
  *   @arg  SPI_TYPE_LOW_EDGE2_MSB,     SCKĬ�ϵ͵�ƽ, �½��ؼ��, MSBģʽ
  *   @arg  SPI_TYPE_LOW_EDGE2_LSB,     SCKĬ�ϵ͵�ƽ, �½��ؼ��, LSBģʽ
  * @param  baud_rate, ����SPI������.
  *   ����Ӳ��SPI, SPI������ͨ����APB����Ԥ��Ƶ����, ��Чֵ����:
  *   @arg   SPI_BaudRatePrescaler_2,    ��APB����2��Ƶ����ΪSPIʱ���ź�
  *   @arg   SPI_BaudRatePrescaler_4,    ��APB����4��Ƶ����ΪSPIʱ���ź�
  *   @arg   SPI_BaudRatePrescaler_8,    ��������
  *   @arg   SPI_BaudRatePrescaler_16
  *   @arg   SPI_BaudRatePrescaler_32
  *   @arg   SPI_BaudRatePrescaler_64
  *   @arg   SPI_BaudRatePrescaler_128
  *   @arg   SPI_BaudRatePrescaler_256
  *   �������ģ��SPI, ֱ�������䲨����ֵ, ��λΪKhz.
  *   @arg   1-500,                      ���ģ��SPI��������Чֵ 1-500 Khz.
  *   @arg   0,                          ��ʾ����ʱ. �ɴ�Լ2MHz
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

    if (spi != 0)                                                               // ʹ��SPIӲ��
    {
    #ifdef SPI_HW_ENABLE
        assert_param(IS_SPI_BAUDRATE_PRESCALER(baud_rate));

        if (spi_hw[spi_name].REMAP != 0)
        {
            GPIO_PinRemapConfig(spi_hw[spi_name].REMAP, ENABLE);                // ʹ��������ӳ��
            RCC_APB2PeriphClockCmd(spi_hw[spi_name].SCK_CLK  |                  // ʹ�� SPI ��IO��ʱ��
                                   spi_hw[spi_name].MISO_CLK |
                                   spi_hw[spi_name].MOSI_CLK |
                                   RCC_APB2Periph_AFIO, ENABLE);                // ʹ�ܽ��湦��ʱ��
        }
        else
        {
            RCC_APB2PeriphClockCmd(spi_hw[spi_name].SCK_CLK  |                  // ʹ�� SPI ��IO��ʱ��
                                   spi_hw[spi_name].MISO_CLK |
                                   spi_hw[spi_name].MOSI_CLK, ENABLE);
        }

        if (spi_hw[spi_name].CLK == RCC_APB2Periph_SPI1)                        // ʹ�� SPI ģ��
            RCC_APB2PeriphClockCmd(spi_hw[spi_name].CLK, ENABLE);
        else
            RCC_APB1PeriphClockCmd(spi_hw[spi_name].CLK, ENABLE);

        SPI_Cmd(spi, DISABLE);                                                  // ����SPIģ��
        SPI_I2S_DeInit(spi);                                                    // ����SPIģ�����

        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].SCK_PIN;               // ����SPI SCK����
        GPIO_Init(spi_hw[spi_name].SCK_PORT, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].MOSI_PIN;              // ����SPI MOSI����
        GPIO_Init(spi_hw[spi_name].MOSI_PORT, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = spi_hw[spi_name].MISO_PIN;              // ����SPI MISO����
        GPIO_Init(spi_hw[spi_name].MISO_PORT, &GPIO_InitStructure);

        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      // ����SPI
        SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
        SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
        SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
        SPI_InitStructure.SPI_CRCPolynomial = 7;
        SPI_InitStructure.SPI_BaudRatePrescaler = baud_rate;                    // ����������
        if (spi_type & 0x04) SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;        // ����SPI��ʼ��ƽ
        else                 SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        if (spi_type & 0x02) SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;       // ����SPI������
        else                 SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
        if (spi_type & 0x01) SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_LSB;// ����SPIλ����˳��
        else                 SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;

        SPI_Init(spi, &SPI_InitStructure);                                      // ʹ��SPI
        SPI_Cmd(spi, ENABLE);
    #endif
    }
    else                                                                        // ���IO��ʵ��SPI
    {
    #ifdef SPI_SW_ENABLE
        assert_param(baud_rate <= 500);

        spi_port_init(spi_name);
        SPI_MOSI_LOW(spi_name);
        if (spi_type & 0x04) SPI_SCK_HIGH(spi_name);
        else                 SPI_SCK_LOW(spi_name);

        spi_para[spi_name].type = (uint8_t)spi_type;                            // ��¼SPIģʽ�Ͳ�����
        if (baud_rate) spi_para[spi_name].delay_us = (500 / baud_rate);
        else           spi_para[spi_name].delay_us = 0;
    #endif
    }
}

/**
  ******************************************************************************
  * @brief  ��ֹĳ��SPI. ����SPI�Ĵ����ָ�ΪĬ��ֵ. ������Ϊ��������.
  * @param  spi_name
  *   @arg  ����ֵ��ο� spi_name_t ���Ͷ����е�ö��ֵ
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
  * @brief  SPI�շ�һ���ֽ�, ������ɺ󷵻ر���ͨѶ��ȡ������.
  * @param  spi_name
  *   @arg  ����ֵ��ο� spi_name_t ���Ͷ����е�ö��ֵ
  * @param  send,   Ҫ���͵�����
  * @retval ����ֵ, �յ�������
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
