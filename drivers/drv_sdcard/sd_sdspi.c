/**
  ******************************************************************************
  * @file    sd_sdspi.c
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   SD����������, ʹ��SPIģʽ.
  *          ֧��MMC��(δ��), SD��, SDHC��. ���֧��4G�Ŀ�.
  *          ����������֧�� drv_sdcard �� drv_sdflash. ����ֱ�ӵ���.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sd_sdspi.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Drivers
  * @{
  */
/** @defgroup SD_SDSPI
  * @{
  */
#if (defined SDCARD_MODE_SPI) || (defined SDFLASH_MODE_SPI)



/**
  ******************************************************************************
  * @addtogroup SD_SDSPI_Configure
  ******************************************************************************
  * @{
  */
#include "drv_systick.h"
#define sd_delay_1ms()                  delay_ms(1)                             ///< ����SD 1ms��ʱ����

/// SD��SPI��ʼ������, ������. �������400KHz, ��дSD��ָ����.
__INLINE static void sd_spi_init_low_speed(void)
{
    // ʹ��Ӳ��SPI
    spi_init(SD_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, SPI_BaudRatePrescaler_256);

    // ʹ��ģ��SPI
//  spi_init(SD_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, 400);
}

/// SD��SPI��ʼ������, ������. �������25MHz, ��дSD��������.
__INLINE static void sd_spi_init_high_speed(void)
{
    // ʹ��Ӳ��   SPI1 ʱ��ԴΪ72Mhz, ��Ҫʹ�� SPI_BaudRatePrescaler_4.
    // ʹ��Ӳ������SPI ʱ��ԴΪ36Mhz, ��ʹ�� SPI_BaudRatePrescaler_2.
    spi_init(SD_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, SPI_BaudRatePrescaler_4);

    // ʹ��ģ��SPI.
//  spi_init(SD_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, 0);
}

/// SD��SPI�շ������궨��
__INLINE static uint8_t sd_spi_tx_rx_byte(uint8_t data)
{
    return spi_tx_rx_byte(SD_SPI_NAME, data);
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



/** @defgroup SD_SDSPI_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup SD_SDSPI_Private_MacroDefine
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

/** @defgroup SD_SDSPI_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
static sd_type_t sd_type = UNKNOW;                                              ///< SD������
static uint16_t  sd_block_size = 512;                                           ///< SD�����С
static uint8_t   sd_power = 9;                                                  ///< SD����λ����
/**
  * @}
  */

/** @defgroup SD_SDSPI_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg);                          // SDָ��ͺ���
/**
  * @}
  */



/** @defgroup SD_SDSPI_Function
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
void sdspi_port_init(void)
{
    sd_spi_init_low_speed();                                                    // �����ȳ�ʼ��SS����Ϊ�������
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
uint8_t sdspi_init(uint16_t block_size)
{
    uint16_t i;
    uint8_t response;
    uint8_t rvalue = 1;

    sd_type = UNKNOW;

    if (((block_size % 512) != 0) || (block_size > 2048))
        return rvalue;

    sd_block_size = block_size;
    sd_power      = 8 + (block_size / 512);

    sd_spi_init_low_speed();                                                    // ʹ��SPI, ��Ϊ����ģʽ�����SD��ʶ����
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
    sd_spi_init_high_speed();

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  ��ȡSD����Ϣ
  * @param  info,   SD����Ϣָ��
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdspi_get_info(sd_info_t *info)
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
  * @brief  ��ȡSD����һ����,
  * @param  pbuf,   ���ݴ洢ָ��
  * @param  sector, ��ȡ������, �������ַ.
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdspi_read_block(uint8_t* pbuf, uint32_t sector)
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
        for (i=20000; i>0; i--)                                                 // ʵ��5ms����
        {
            if (sd_spi_tx_rx_byte(0xFF) == 0xFE) break;                         // ׼���ô�������
        }
        if (i)
        {
            rvalue = 0;
            for (i=sd_block_size; i>0; i--)
                *pbuf++ = sd_spi_tx_rx_byte(0xFF);                              // ��һ����
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
  * @brief  ��ȡSD���Ķ����,
  * @param  pbuf,   ��ȡ��ַָ��
  * @param  sector, ��ȡ������, �������ַ.
  * @param  block_num,  ��Ҫ��ȡ��SD������
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdspi_read_multi_blocks(uint8_t* pbuf, uint32_t sector, uint32_t block_num)
{
    uint32_t i;
    uint8_t  response;
    uint8_t  rvalue = 1;

    if ((sd_type == UNKNOW) || (block_num <= 1)) return rvalue;
    if (sd_type != SDHC) sector <<= sd_power;                                   // ����SDHC, ��Ҫ�����ַתΪ�ֽڵ�ַ, sector *= sd_block_size

    sd_cs_enable();
    response = sd_send_cmd(CMD18, sector);                                      // ���� CMD18, �������
    if (response == 0x00)                                                       // Ӧ����ȷ
    {
        rvalue = 0;
        while (block_num--)
        {
            for (i=20000; i>0; i--)
            {
                if (sd_spi_tx_rx_byte(0xFF) == 0xFE) break;                     // ׼���ô�������
            }
            if (i)
            {
                for (i=sd_block_size; i>0; i--)
                    *pbuf++ = sd_spi_tx_rx_byte(0xFF);                          // �������
                sd_spi_tx_rx_byte(0xFF);                                        // ��CRCУ��
                sd_spi_tx_rx_byte(0xFF);
            }
            else
            {
                rvalue = 1;
            }
        }
    }
    sd_send_cmd(CMD12, 0);                                                      // ����ֹͣ����
    sd_cs_disable();
    sd_spi_tx_rx_byte(0xFF);

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  д��SD����һ����
  * @param  pbuf,   д������ָ��
  * @param  sector, д�������, �������ַ.
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdspi_write_block(const uint8_t* pbuf, uint32_t sector)
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
        for (i = sd_block_size; i>0 ; i--) sd_spi_tx_rx_byte(*pbuf++);          // дһ����
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
  * @brief  д��SD���Ķ����
  * @param  pbuf,   д���ַָ��
  * @param  sector, д�������, �������ַ.
  * @param  block_num,  ��Ҫд���SD������
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdspi_write_multi_blocks(const uint8_t* pbuf, uint32_t sector, uint32_t block_num)
{
    uint32_t i;
    uint8_t  response;
    uint8_t  rvalue = 1;

    if ((sd_type == UNKNOW) || (block_num <= 1)) return rvalue;
    if (sd_type != SDHC) sector <<= sd_power;                                   // ����SDHC, ��Ҫ�����ַתΪ�ֽڵ�ַ, sector *= sd_block_size

    sd_cs_enable();
    if(sd_type != MMC) sd_send_cmd(ACMD23, block_num);                          // ʹ��ACMD23Ԥ����SD������
    response = sd_send_cmd(CMD25, sector);                                      // ���� CMD25, д�����
    if (response == 0x00)                                                       // Ӧ����ȷ
    {
        rvalue = 0;
        for (; block_num>0; block_num--)
        {
            sd_spi_tx_rx_byte(0xFF);                                            // ����CLK, �ȴ�SD��׼����
            sd_spi_tx_rx_byte(0xFC);                                            // ����ʼ����0xFC, �����Ƕ��д��

            for(i=sd_block_size; i>0; i--)
                sd_spi_tx_rx_byte(*pbuf++);                                     // д��һ����
            sd_spi_tx_rx_byte(0xFF);                                            // ��ȡCRC��Ϣ
            sd_spi_tx_rx_byte(0xFF);

            if ((sd_spi_tx_rx_byte(0xFF) & 0x1F) == 0x05)                       // ��ȡSD��Ӧ����Ϣ
            {
                for (i=200000; i>0; i--)                                        // д����Ҫʱ��, V1������, ��ʱ����̫С.
                {
                    if (sd_spi_tx_rx_byte(0xFF) == 0xFF) break;                 // �ȴ�����д�����
                }
            }
            else
            {
                i = 0;
            }
            if (i == 0) rvalue = 1;                                             // ��һ����д�����, �ͱ��Ϊ����
        }
    }
    sd_spi_tx_rx_byte(0xFD);                                                    // ����SD�����д
    for (i=100; i>0; i--)
    {
        if (sd_spi_tx_rx_byte(0xFF) == 0x00) break;
    }
    if (i == 0) rvalue = 1;
    for (i=200000; i>0; i--)                                                    // �ȴ����д�����
    {
        if (sd_spi_tx_rx_byte(0xFF) == 0xFF) break;
    }
    if (i == 0) rvalue = 1;
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
