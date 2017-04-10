/**
  ******************************************************************************
  * @file    sd_sdspidma.c
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2014.4.28
  * @brief   SD����������, ʹ��SPI DMAģʽ. ����дû�гɹ�.
  *          ֧��MMC��(δ��), SD��, SDHC��. ���֧��4G�Ŀ�.
  *          ����������֧�� drv_sdcard �� drv_sdflash. ����ֱ�ӵ���.
  *
  *          RVMDK��size�Ż�ֻ���� level-0, ����дû�гɹ�. ԭ����.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sd_sdspidma.h"
#include "stm32f10x.h"                                                          // __INLINE
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"



/** @addtogroup Drivers
  * @{
  */
/** @defgroup SD_SDSPIDMA
  * @{
  */
#if (defined SDCARD_MODE_SPIDMA) || (defined SDFLASH_MODE_SPIDMA)



/**
  ******************************************************************************
  * @addtogroup SD_SDSPIDMA_Configure
  ******************************************************************************
  * @{
  */
#include "drv_systick.h"
#define sd_delay_1ms()                  delay_ms(1)                             ///< ����SD 1ms��ʱ����

/// SD SPI ��Ӳ����Դ����
#if (SD_SPI_X == 1)
    #define SD_SPI_PERIPHERAL           SPI1
    #define SD_SPI_CLK                  RCC_APB2Periph_SPI1

    #define SD_SPI_SCK_PORT             GPIOA
    #define SD_SPI_SCK_CLK              RCC_APB2Periph_GPIOA
    #define SD_SPI_SCK_PIN              GPIO_Pin_5

    #define SD_SPI_MISO_PORT            GPIOA
    #define SD_SPI_MISO_CLK             RCC_APB2Periph_GPIOA
    #define SD_SPI_MISO_PIN             GPIO_Pin_6

    #define SD_SPI_MOSI_PORT            GPIOA
    #define SD_SPI_MOSI_CLK             RCC_APB2Periph_GPIOA
    #define SD_SPI_MOSI_PIN             GPIO_Pin_7

    #define SD_SPI_DMA_CLK              RCC_AHBPeriph_DMA1
    #define SD_SPI_DMA_RX               DMA1_Channel2
    #define SD_SPI_DMATC_RX             DMA1_FLAG_TC2
    #define SD_SPI_DMA_TX               DMA1_Channel3
    #define SD_SPI_DMATC_TX             DMA1_FLAG_TC3

#elif (SD_SPI_X == 2)
    #define SD_SPI_PERIPHERAL           SPI2
    #define SD_SPI_CLK                  RCC_APB1Periph_SPI2

    #define SD_SPI_SCK_PORT             GPIOB
    #define SD_SPI_SCK_CLK              RCC_APB2Periph_GPIOB
    #define SD_SPI_SCK_PIN              GPIO_Pin_13

    #define SD_SPI_MISO_PORT            GPIOB
    #define SD_SPI_MISO_CLK             RCC_APB2Periph_GPIOB
    #define SD_SPI_MISO_PIN             GPIO_Pin_14

    #define SD_SPI_MOSI_PORT            GPIOB
    #define SD_SPI_MOSI_CLK             RCC_APB2Periph_GPIOB
    #define SD_SPI_MOSI_PIN             GPIO_Pin_15

    #define SD_SPI_DMA_CLK              RCC_AHBPeriph_DMA1
    #define SD_SPI_DMA_RX               DMA1_Channel4
    #define SD_SPI_DMATC_RX             DMA1_FLAG_TC4
    #define SD_SPI_DMA_TX               DMA1_Channel5
    #define SD_SPI_DMATC_TX             DMA1_FLAG_TC5

#else

    #error "NOT select valid SPI DMA hardware!!!"

#endif

/// SD��SPI��ʼ������, ������. �������400KHz, ��дSD��ָ����.
__INLINE static void sd_spi_init_speed(uint8_t isHigh)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    if (isHigh)
    {
        // �������18Mhz
    #if (SD_SPI_X == 1)
        // ʹ��Ӳ�� SPI1 ʱ��ԴΪ72Mhz, ��Ҫʹ�� SPI_BaudRatePrescaler_4.
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    #elif (SD_SPI_X == 2)
        // ʹ��Ӳ�� SPI2 ʱ��ԴΪ36Mhz, ��ʹ�� SPI_BaudRatePrescaler_2.
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    #endif
    }
    else
    {
        // ���ʲ��ɸ���400K
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    }

    RCC_APB2PeriphClockCmd(SD_SPI_SCK_CLK  |                                    // ʹ�� SPI ��IO��ʱ��
                           SD_SPI_MISO_CLK |
                           SD_SPI_MOSI_CLK, ENABLE);

    #if (SD_SPI_X == 1)
        RCC_APB2PeriphClockCmd(SD_SPI_CLK, ENABLE);
    #elif (SD_SPI_X == 2)
        RCC_APB1PeriphClockCmd(SD_SPI_CLK, ENABLE);
    #endif

    RCC_AHBPeriphClockCmd(SD_SPI_DMA_CLK, ENABLE);

    SPI_Cmd(SD_SPI_PERIPHERAL, DISABLE);
    SPI_I2S_DeInit(SD_SPI_PERIPHERAL);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = SD_SPI_SCK_PIN;                             // ����SPI SCK����
    GPIO_Init(SD_SPI_SCK_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = SD_SPI_MOSI_PIN;                            // ����SPI MOSI����
    GPIO_Init(SD_SPI_MOSI_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = SD_SPI_MISO_PIN;                            // ����SPI MISO����
    GPIO_Init(SD_SPI_MISO_PORT, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;

    SPI_Init(SD_SPI_PERIPHERAL, &SPI_InitStructure);
    SPI_Cmd(SD_SPI_PERIPHERAL, ENABLE);
}

/// SD��SPI�շ������궨��
__INLINE static uint8_t sd_spi_tx_rx_byte(uint8_t data)
{
    uint8_t receive = 0;

    while (SPI_I2S_GetFlagStatus(SD_SPI_PERIPHERAL, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SD_SPI_PERIPHERAL, data);
    while (SPI_I2S_GetFlagStatus(SD_SPI_PERIPHERAL, SPI_I2S_FLAG_RXNE) == RESET);
    receive = SPI_I2S_ReceiveData(SD_SPI_PERIPHERAL);
    return receive;
}

/// SD Ƭѡ�ź����ų�ʼ��, IO������Ϊ�������.
__INLINE static void sd_cs_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(SD_CS_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = SD_CS_PIN;
    GPIO_Init(SD_CS_PORT, &GPIO_InitStructure);
}

/// SD Ƭѡ�ź�����ʹ��, ��IO�ڵ�ƽ�õ�
__INLINE static void sd_cs_enable(void)
{
    GPIO_ResetBits(SD_CS_PORT, SD_CS_PIN);
}

/// SD Ƭѡ�ź����Ž�ֹ, ��IO�ڵ�ƽ�ø�
__INLINE static void sd_cs_disable(void)
{
    GPIO_SetBits(SD_CS_PORT, SD_CS_PIN);
}
/**
  * @}
  */



/** @defgroup SD_SDSPIDMA_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup SD_SDSPIDMA_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */
#define CMD0                            0                                       ///< ����0 , ����λ
#define CMD1                            1                                       ///< ����1 , CMD_SEND_OP_COND
#define CMD8                            8                                       ///< ����8 , CMD_SEND_IF_COND
#define CMD9                            9                                       ///< ����9 , ��CSD����
#define CMD10                           10                                      ///< ����10, ��CID����
#define CMD12                           12                                      ///< ����12, ֹͣ���ݴ���
#define CMD16                           16                                      ///< ����16, ����SectorSize Ӧ����0x00
#define CMD17                           17                                      ///< ����17, ��sector
#define CMD18                           18                                      ///< ����18, ��Multi sector
#define ACMD23                          23                                      ///< ����23, ���ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24                           24                                      ///< ����24, дsector
#define CMD25                           25                                      ///< ����25, дMulti sector
#define ACMD41                          41                                      ///< ����41, Ӧ����0x00
#define CMD55                           55                                      ///< ����55, Ӧ����0x01
#define CMD58                           58                                      ///< ����58, ��OCR��Ϣ
/**
  * @}
  */

/** @defgroup SD_SDSPIDMA_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
static sd_type_t sd_type = UNKNOW;                                              ///< SD������
static uint16_t  sd_block_size = 512;                                           ///< SD�����С
static uint8_t   sd_power = 9;                                                  ///< SD����λ����
/**
  * @}
  */

/** @defgroup SD_SDSPIDMA_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg);                          // SDָ��ͺ���
static void sdspi_dma_read_init(uint8_t* pbuf);                                 // SPI����ǰ��ʼ��DMA
static void sdspi_dma_write_init(const uint8_t* pbuf);                          // SPI����ǰ��ʼ��DMA
/**
  * @}
  */



/** @defgroup SD_SDSPIDMA_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  SD���ĵ�Դ, SPIͨѶ����Ӧ�Ķ˿ڳ�ʼ������
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sdspidma_port_init(void)
{
    sd_spi_init_speed(0);                                                       // �����ȳ�ʼ��SS����Ϊ�������
    sd_cs_init();                                                               // �ٳ�ʼ��SS��GPIO�ڲ���Ч
    sd_cs_disable();
}

/**
  ******************************************************************************
  * @brief  ��SD����Դ����ʼ��
  * @param  none
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdspidma_init(uint16_t block_size)
{
    uint16_t i;
    uint8_t response;
    uint8_t rvalue = 1;

    sd_type = UNKNOW;

    if (((block_size % 512) != 0) || (block_size > 2048))
        return rvalue;

    sd_block_size = block_size;
    sd_power      = 8 + (block_size / 512);

    sd_spi_init_speed(0);                                                       // ʹ��SPI, ��Ϊ����ģʽ�����SD��ʶ����
    for (i=10; i>0; i--) sd_spi_tx_rx_byte(0xFF);                               // �Ȳ���>74�����壬��SD���Լ���ʼ�����

    sd_cs_enable();                                                             // SD��Ƭѡ�ź�ʹ��
    for (i=500; i>0; i--)                                                       // ʵ��300ms����
    {
        sd_delay_1ms();
        if (sd_send_cmd(CMD0, 0) == 0x01) break;                                // CMD0, ��SD���������ģʽ
    }

    if (i)                                                                      // SD���ɹ��������ģʽ
    {
        // CMD8ָ����Ӧ��ʶ��SD1.x��SD2.0�淶�Ĺؼ�
        response = sd_send_cmd(8, 0x1AA);
        switch (response)
        {
        case 0x05: {                                                            // CMD8Ӧ��0x05, ����֧��CMD8ָ��, ��SD1.x����MMC��
            sd_spi_tx_rx_byte(0xFF);                                            // ��8��CLK, ��SD����������

            // ����CMD55��ACMD41ָ��, ���ڼ����SD������MMC��, MMC����ACMD41��û��Ӧ���
            for (i=500; i>0; i--)
            {
                sd_delay_1ms();
                response = sd_send_cmd(CMD55, 0);
                if (response != 0x01) goto EXIT;                                // Ӧ�����, ֱ���˳�
                response = sd_send_cmd(ACMD41, 0);
                if (response == 0x00) break;                                    // ��ȷӦ��, ����ѭ��
            }
            if (i)
            {
                sd_type = SD_V1;                                                // ��ȷӦ��, ��SD��V1.x
                rvalue = 0;                                                     // SD����ʼ���ɹ����
            }
            else                                                                // ��Ӧ��, ������MMC��
            {
                for (i=500; i>0; i--)
                {
                    sd_delay_1ms();
                    response = sd_send_cmd(CMD1, 0);
                    if (response == 0x00) break;
                }
                if (i)                                                          // MMC����ʼ���ɹ�
                {
                    sd_type = MMC;                                              // ��MMC��
                    rvalue = 0;                                                 // SD����ʼ���ɹ����
                }
            }

            if(sd_send_cmd(CMD16, sd_block_size) != 0x00)                       // ����SD�����С
            {
                sd_type = UNKNOW;
                rvalue = 1;
            }
            break; }

        case 0x01: {                                                            // Ӧ��0x01, ��V2.0��SD��
            sd_spi_tx_rx_byte(0xFF);                                            // V2.0�Ŀ���CMD8�����ᴫ��4�ֽڵ�Ӧ������
            sd_spi_tx_rx_byte(0xFF);
            i   = sd_spi_tx_rx_byte(0xFF);
            i <<= 8;
            i  |= sd_spi_tx_rx_byte(0xFF);                                      // 4���ֽ�Ӧ����0x000001AA, ��ʾ�ÿ��Ƿ�֧��2.7V-3.6V�ĵ�ѹ��Χ

            if(i == 0x1AA)                                                      // SD��֧��2.7V-3.6V, ���Բ���
            {
                for (i=500; i>0; i--)
                {
                    sd_delay_1ms();
                    response = sd_send_cmd(CMD55, 0);
                    if (response != 0x01) goto EXIT;                            // Ӧ�����, ֱ���˳�
                    response = sd_send_cmd(ACMD41, 0x40000000);                 // SD V2.0Ҫʹ�� 0x40000000
                    if (response == 0x00) break;                                // ��ȷӦ��, ����ѭ��
                }
                if (i)                                                          // CMD41��ȷӦ��
                {
                    // ͨ����ȡOCR��Ϣ, ʶ������ͨ�� SD V2.0 ���� SDHC ��
                    if (sd_send_cmd(CMD58, 0) == 0x00)
                    {
                        i = sd_spi_tx_rx_byte(0xFF);                            // Ӧ���4�ֽ�OCR��Ϣ
                        sd_spi_tx_rx_byte(0xFF);
                        sd_spi_tx_rx_byte(0xFF);
                        sd_spi_tx_rx_byte(0xFF);

                        if (i & 0x40) sd_type = SDHC;                           // ͨ�����CCSλȷ��SD������
                        else          sd_type = SD_V2;
                        rvalue = 0;                                             // SD����ʼ���ɹ����
                    }
                }
                if(sd_send_cmd(CMD16, sd_block_size) != 0x00)                   // ����SD�����С
                {
                    sd_type = UNKNOW;
                    rvalue = 1;
                }
            }
            break; }
        }
    }

EXIT:
    sd_cs_disable();
    sd_spi_tx_rx_byte(0xFF);                                                    // 8��CLK, ��SD������������
    sd_spi_init_speed(1);

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  ��ȡSD����Ϣ
  * @param  info,   SD����Ϣָ��
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdspidma_get_info(sd_info_t *info)
{
    uint8_t rvalue = 1;
    uint8_t data;
    uint8_t csd_read_bl_len = 0;
    uint8_t csd_c_size_mult = 0;
    uint32_t csd_c_size = 0;
    uint16_t i;

    if (sd_type == UNKNOW) return rvalue;                                       // ��ָ�� �� sd��δ��ʼ��, ֱ�ӷ���

    sd_cs_enable();
    if (sd_send_cmd(CMD10, 0) == 0x00)                                          // ��ȡCID�Ĵ���
    {
        for (i=500; i>0; i--)                                                   // ʵ��400ms����
        {
            sd_delay_1ms();
            if (sd_spi_tx_rx_byte(0xFF) == 0xFE) break;                         // ׼���ô�������
        }
        if (i)
        {
          rvalue = 0;                                                           // ��ȡCID�Ĵ����ɹ�
          for (i=0; i<18; i++)                                                  // 16������, 2��CRCУ��
          {
                data = sd_spi_tx_rx_byte(0xFF);                                 // ����CID����
                switch(i)
                {
                case 0:
                    info->sd_manu = data;
                    break;
                case 1:
                case 2:
                    info->sd_oem[i - 1] = data;
                    break;
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    info->sd_product[i - 3] = data;
                    break;
                case 8:
                    info->sd_rev = data;
                    break;
                case 9:
                case 10:
                case 11:
                case 12:
                    info->sd_sn |= (uint32_t)data << ((12 - i) * 8);
                    break;
                case 13:
                    info->sd_year = data << 4;
                    break;
                case 14:
                    info->sd_year |= data >> 4;
                    info->sd_mon = data & 0x0f;
                    break;
                }
          }
        }
    }
    if (rvalue) return rvalue;

    rvalue = 1;
    if (sd_send_cmd(CMD9, 0) == 0x00)                                           // ��ȡCSD�Ĵ���
    {
        for (i=500; i>0; i--)
        {
            sd_delay_1ms();
            if (sd_spi_tx_rx_byte(0xFF) == 0xFE) break;                         // ׼���ô�������
        }
        if (i)
        {
            rvalue = 0;                                                         // ��ȡCID�Ĵ����ɹ�
            for (i=0; i<18; i++)                                                // 16������, 2��CRCУ��
            {
                data = sd_spi_tx_rx_byte(0xFF);

                if (sd_type == SDHC)                                            // SDHC��
                {
                    switch(i)
                    {
                    case 7:
                        csd_c_size = (data & 0x3f);
                        csd_c_size <<= 8;
                        break;
                    case 8:
                        csd_c_size |= data;
                        csd_c_size <<= 8;
                        break;
                    case 9:
                        csd_c_size |= data;
                        csd_c_size++;
                        info->sd_cap = csd_c_size;
                        info->sd_cap <<= 19;                                    // *= (512 * 1024)
                        break;
                    }
                }
                else                                                            // ��ͨSD�� �� MMC ��
                {
                    switch(i)
                    {
                    case 5:
                        csd_read_bl_len = data & 0x0f;
                        break;
                    case 6:
                        csd_c_size = data & 0x03;
                        csd_c_size <<= 8;
                        break;
                    case 7:
                        csd_c_size |= data;
                        csd_c_size <<= 2;
                        break;
                    case 8:
                        csd_c_size |= data >> 6;
                        ++csd_c_size;
                        break;
                    case 9:
                        csd_c_size_mult = data & 0x03;
                        csd_c_size_mult <<= 1;
                        break;
                    case 10:
                        csd_c_size_mult |= data >> 7;
                        info->sd_cap = csd_c_size;
                        info->sd_cap <<= (csd_c_size_mult + csd_read_bl_len + 2);
                        break;
                    }
                }
            }
        }
    }

    info->sd_type = sd_type;
    return rvalue;
}

/**
  ******************************************************************************
  * @brief  spi ��ȡǰ dma ��ʼ��
  * @param  pbuf,   ���ݴ洢ָ��
  * @retval none
  ******************************************************************************
  */
void sdspi_dma_read_init(uint8_t* pbuf)
{
    static uint8_t dummy_tx = 0xFF;
    DMA_InitTypeDef DMA_InitStructure;

    DMA_DeInit(SD_SPI_DMA_RX);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SD_SPI_PERIPHERAL->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)pbuf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = sd_block_size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(SD_SPI_DMA_RX, &DMA_InitStructure);

    DMA_DeInit(SD_SPI_DMA_TX);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&dummy_tx;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(SD_SPI_DMA_TX, &DMA_InitStructure);
}

/**
  ******************************************************************************
  * @brief  ��ȡSD����һ����,
  * @param  pbuf,   ���ݴ洢ָ��
  * @param  sector, ��ȡ������, �������ַ.
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdspidma_read_block(uint8_t* pbuf, uint32_t sector)
{
    uint16_t i;
    uint8_t  response;
    uint8_t  rvalue = 1;

    if (sd_type == UNKNOW) return rvalue;
    if (sd_type != SDHC) sector <<= sd_power;                                   // ����SDHC, ��Ҫ�����ַתΪ�ֽڵ�ַ, sector *= sd_block_size

    sd_cs_enable();
    response = sd_send_cmd(CMD17, sector);                                      // ���� CMD17, ��������
    if (response == 0x00)                                                       // Ӧ����ȷ
    {
        sdspi_dma_read_init(pbuf);                                              // DMA��ʼ��
        for (i=20000; i>0; i--)                                                 // ʵ��5ms����
        {
            if (sd_spi_tx_rx_byte(0xFF) == 0xFE) break;                         // ׼���ô�������
        }
        if (i)
        {
            rvalue = 0;

            // DMA��һ����
            SPI_I2S_DMACmd(SD_SPI_PERIPHERAL, SPI_I2S_DMAReq_Rx, ENABLE);
            SPI_I2S_DMACmd(SD_SPI_PERIPHERAL, SPI_I2S_DMAReq_Tx, ENABLE);
            DMA_Cmd(SD_SPI_DMA_RX, ENABLE);
            DMA_Cmd(SD_SPI_DMA_TX, ENABLE);
            while(!DMA_GetFlagStatus(SD_SPI_DMATC_RX));
            while(!DMA_GetFlagStatus(SD_SPI_DMATC_TX));

            DMA_Cmd(SD_SPI_DMA_TX, DISABLE);
            DMA_Cmd(SD_SPI_DMA_RX, DISABLE);
            SPI_I2S_DMACmd(SD_SPI_PERIPHERAL, SPI_I2S_DMAReq_Tx, DISABLE);
            SPI_I2S_DMACmd(SD_SPI_PERIPHERAL, SPI_I2S_DMAReq_Rx, DISABLE);
		  
//            for (i=sd_block_size; i>0; i--)
//                *pbuf++ = sd_spi_tx_rx_byte(0xFF);                              // ��һ����

            sd_spi_tx_rx_byte(0xFF);                                            // ��CRCУ��
            sd_spi_tx_rx_byte(0xFF);
        }
    }
    sd_cs_disable();
    sd_spi_tx_rx_byte(0xFF);

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  spi д��ǰ dma ��ʼ��
  * @param  pbuf,   ���ݴ洢ָ��
  * @retval none
  ******************************************************************************
  */
void sdspi_dma_write_init(const uint8_t* pbuf)
{
    DMA_InitTypeDef DMA_InitStructure;

    DMA_DeInit(SD_SPI_DMA_TX);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SD_SPI_PERIPHERAL->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pbuf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = sd_block_size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(SD_SPI_DMA_TX, &DMA_InitStructure);
}

/**
  ******************************************************************************
  * @brief  д��SD����һ����
  * @param  pbuf,   д������ָ��
  * @param  sector, д�������, �������ַ.
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdspidma_write_block(const uint8_t* pbuf, uint32_t sector)
{
    uint32_t i;
    uint8_t  response;
    uint8_t  rvalue = 1;

    if (sd_type == UNKNOW) return rvalue;
    if (sd_type != SDHC) sector <<= sd_power;                                   // ����SDHC, ��Ҫ�����ַתΪ�ֽڵ�ַ, sector *= sd_block_size

    sd_cs_enable();
    response = sd_send_cmd(CMD24, sector);                                      // ���� CMD24, д������
    if (response == 0x00)                                                       // Ӧ����ȷ
    {
        sd_spi_tx_rx_byte(0xFF);                                                // ����CLK, �ȴ�SD��׼����
        sd_spi_tx_rx_byte(0xFE);                                                // ��ʼ����0xFE

//		for (i = sd_block_size; i>0 ; i--) sd_spi_tx_rx_byte(*pbuf++); 
        // дһ����, �˲��� RVMDK �Ż��ȼ�����Ϊ level-0, ԭ����
        sdspi_dma_write_init(pbuf);
        SPI_I2S_DMACmd(SD_SPI_PERIPHERAL, SPI_I2S_DMAReq_Tx, ENABLE);
        DMA_Cmd(SD_SPI_DMA_TX, ENABLE);
        while(!DMA_GetFlagStatus(SD_SPI_DMATC_TX));

        DMA_Cmd(SD_SPI_DMA_TX, DISABLE);
        SPI_I2S_DMACmd(SD_SPI_PERIPHERAL, SPI_I2S_DMAReq_Tx, DISABLE);

        sd_spi_tx_rx_byte(0xFF);                                                // ��CRCУ��
        sd_spi_tx_rx_byte(0xFF);

        if ((sd_spi_tx_rx_byte(0xFF) & 0x1F) == 0x05)                           // ������Ӧ��
        {
            for (i=200000; i>0; i--)                                            // ʵ��500ms����
            {
                if (sd_spi_tx_rx_byte(0xFF) == 0xFF)                            // �ȴ�����д�����
                {
                    rvalue = 0;                                                 // ���Ϊд�ɹ�
                    break;
                }
            }
        }
    }
    sd_cs_disable();
    sd_spi_tx_rx_byte(0xFF);

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  ����5�ֽڵ�SD��ָ��
  * @param  Cmd, Ҫ���͵�ָ��
  * @param  Arg, ָ�����
  * @retval ����ֵ, ָ��Ӧ��
  ******************************************************************************
  */
uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg)
{
    uint8_t response;
    uint8_t i;

    sd_spi_tx_rx_byte(0xFF);

    sd_spi_tx_rx_byte(0x40 | cmd);                                              // ����ָ��
    sd_spi_tx_rx_byte(arg >> 24);
    sd_spi_tx_rx_byte(arg >> 16);
    sd_spi_tx_rx_byte(arg >> 8);
    sd_spi_tx_rx_byte(arg >> 0);
    switch(cmd)
    {
    case CMD0:
        sd_spi_tx_rx_byte(0x95);                                                // CMD0��У��Ϊ0x95
        break;
    case CMD8:
        sd_spi_tx_rx_byte(0x87);                                                // CMD8��У��Ϊ0x87
        break;
    default:
        sd_spi_tx_rx_byte(0xff);                                                // ����ָ����SPIģʽ������У��
        break;
    }

    for (i=10; i>0; i--)                                                        // ����Ӧ��
    {
        response = sd_spi_tx_rx_byte(0xFF);
        if(response != 0xFF) break;
    }

    return response;
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
