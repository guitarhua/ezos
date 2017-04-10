/**
  ******************************************************************************
  * @file    drv_sdflash.c
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   ��SD����ΪFLASHʹ�õ���������.
  *          ֧�� SD_V1.x, SD_V2, SDHC��.
  *          ֧��SPIģʽ �� SDIOģʽ, ����ᵼ�¶�д����. ԭ����.
  *          ���ڵ�ַʹ�õ��� uint32_t ��ʽ, �������֧�ֵ�4G�Ŀ�.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "drv_sdflash.h"
#include "stm32f10x.h"                                                          // __INLINE, __NOP

#ifdef SDFLASH_POWER_NAME
    #include "drv_ctrlport.h"
#endif



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_SDFLASH
  * @{
  */
#ifdef SDFLASH_ENABLE



/**
  ******************************************************************************
  * @addtogroup DRV_SDFLASH_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_SDFLASH_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */
/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */
#define SDFLASH_BLOCK_SIZE              512                                     ///< SD�����С, ֻ֧�� 512 byte, ����ᵼ�¶�д����. ԭ����.
#define SDFLASH_SECTOR_SIZE             0x400                                   ///< ����SD��������С, ��ΪSECTOR����
/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
static uint8_t  data[SDFLASH_SECTOR_SIZE];                                      // ��¼������������
static uint32_t sector_addr_cur;                                                // ��¼��ǰ������ʼ��ַ
static uint8_t  data_changed;                                                   // ��¼����������Ƿ񱻸��Ĺ�, !0��ʾ���Ĺ�
/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Private_Function
  * @brief    ����˽�к���
  * @{
  */
/// SD����Դ�������ų�ʼ��
__INLINE static void sdflash_power_port_init(void)
{
#ifdef SDFLASH_POWER_NAME
    ctrlport_init(SDFLASH_POWER_NAME);
#endif
}

/// ��SD����Դ
__INLINE static void sdflash_power_port_on(void)
{
#ifdef SDFLASH_POWER_NAME
    ctrlport_on(SDFLASH_POWER_NAME);
#endif
}

/// �ر�SD����Դ
__INLINE static void sdflash_power_port_off()
{
#ifdef SDFLASH_MODE_SDIO
    SD_PowerOFF();
#endif
#ifdef SDFLASH_POWER_NAME
    ctrlport_off(SDFLASH_POWER_NAME);
#endif
}

/// SD ���ڼ���źų�ʼ��, IO������Ϊ��������
__INLINE static void sdflash_detect_port_init(void)
{
#ifdef SDFLASH_DETECT_ENABLE
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(SDFLASH_DETECT_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin  = SDFLASH_DETECT_PIN;
    GPIO_Init(SDFLASH_DETECT_PORT, &GPIO_InitStructure);
#endif
}

/// ��ȡSD�����IO�ڵ�ǰ��ƽֵ. 0, �͵�ƽ. !0, �ߵ�ƽ
__INLINE static uint32_t sdflash_detect_port_level(void)
{
#ifdef SDFLASH_DETECT_ENABLE
    return (SDFLASH_DETECT_PORT->IDR & SDFLASH_DETECT_PIN);
#else
    return 0;                                                                   // ʼ����Ϊ����
#endif
}
/**
  * @}
  */



/** @defgroup DRV_SDFLASH_Function
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
void sdflash_port_init(void)
{
    sdflash_power_port_init();                                                  // ��ʼ����Դ���ƿ�
    sdflash_detect_port_init();
#if (defined SDFLASH_MODE_SDIO)
    SD_Port_Init();
#elif (defined SDFLASH_MODE_SPIDMA)
    sdspidma_port_init();
#elif (defined SDFLASH_MODE_SPI)
    sdspi_port_init();
#endif
}

/**
  ******************************************************************************
 * @brief  ���SD���Ƿ����
 * @param  none
 * @retval ����ֵ, 0, ������. 1, ����
 *******************************************************************************
 */
uint8_t sdflash_present(void)
{
    if (sdflash_detect_port_level())
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
  ******************************************************************************
  * @brief  ��SD����Դ����ʼ��
  * @param  none
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdflash_init(void)
{
    uint8_t i;

    sdflash_port_init();                                                        // Ĭ�Ϲرյ�Դ
    for (i=0; i<100; i++) { __NOP(); }                                          // ��һ������ʱ
    sdflash_power_port_on();                                                    // ��SD����Դ

    sector_addr_cur = 0xFFFFFFFF;
    data_changed = 0;
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();                                // ��һ������ʱ
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();

    if (!sdflash_present()) return 1;                                           // SD��������, ֱ���˳�
#if (defined SDFLASH_MODE_SDIO)
    if (SD_Init(SDFLASH_BLOCK_SIZE) == SD_OK) return 0;
    else                                      return 1;
#elif (defined SDFLASH_MODE_SPIDMA)
    if (sdspidma_init(SDFLASH_BLOCK_SIZE) == 0) return 0;
    else                                        return 1;
#elif (defined SDFLASH_MODE_SPI)
    if (sdspi_init(SDFLASH_BLOCK_SIZE) == 0) return 0;
    else                                     return 1;
#else
    return 1;
#endif
}

/**
  ******************************************************************************
  * @brief  �ر�SD���ĵ�Դ
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sdflash_power_off(void)
{
    sdflash_power_port_off();
}

/**
  ******************************************************************************
  * @brief  ��ȡSD����Ϣ
  * @param  info,   SD����Ϣָ��
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdflash_get_info(sdflash_info_t *info)
{
#if (defined SDFLASH_MODE_SDIO)

    SD_CardInfo cardinfo;

    if (SD_GetCardInfo(&cardinfo) == SD_OK)
    {
        switch (cardinfo.CardType)                                              // SD������
        {
        case SDIO_STD_CAPACITY_SD_CARD_V1_1:
            info->type = SDFLASH_V1;
            break;
        case SDIO_STD_CAPACITY_SD_CARD_V2_0:
            info->type = SDFLASH_V2;
            break;
        case SDIO_HIGH_CAPACITY_SD_CARD:
            info->type = SDFLASH_SDHC;
            break;
        default:
            info->type = SDFLASH_UNKNOW;
            break;
        }
        info->revision = cardinfo.SD_cid.ProdRev;
        info->serial   = cardinfo.SD_cid.ProdSN;
        info->year     = cardinfo.SD_cid.ManufactDate >> 8;
        info->month    = cardinfo.SD_cid.ManufactDate & 0xFF;
        info->capacity = cardinfo.CardCapacity;
        return 0;
    }
    else
    {
        info->type = SDFLASH_UNKNOW;
        info->capacity = 0;
        return 1;
    }

#elif (defined SDFLASH_MODE_SPIDMA)

    sd_info_t cardinfo;

    if (sdspidma_get_info(&cardinfo) == 0)
    {
        switch (cardinfo.sd_type)                                               // SD������
        {
        case SD_V1:
            info->type = SDFLASH_V1;
            break;
        case SD_V2:
            info->type = SDFLASH_V2;
            break;
        case SDHC:
            info->type = SDFLASH_SDHC;
            break;
        case MMC:
            info->type = SDFLASH_MMC;
            break;
        default:
            info->type = SDFLASH_UNKNOW;
            break;
        }
        info->revision = cardinfo.sd_rev;
        info->serial   = cardinfo.sd_sn;
        info->year     = cardinfo.sd_year;
        info->month    = cardinfo.sd_mon;
        info->capacity = cardinfo.sd_cap;
        return 0;
    }
    else
    {
        info->type = SDFLASH_UNKNOW;
        info->capacity = 0;
        return 1;
    }

#elif (defined SDFLASH_MODE_SPI)

    sd_info_t cardinfo;

    if (sdspi_get_info(&cardinfo) == 0)
    {
        switch (cardinfo.sd_type)                                               // SD������
        {
        case SD_V1:
            info->type = SDFLASH_V1;
            break;
        case SD_V2:
            info->type = SDFLASH_V2;
            break;
        case SDHC:
            info->type = SDFLASH_SDHC;
            break;
        case MMC:
            info->type = SDFLASH_MMC;
            break;
        default:
            info->type = SDFLASH_UNKNOW;
            break;
        }
        info->revision = cardinfo.sd_rev;
        info->serial   = cardinfo.sd_sn;
        info->year     = cardinfo.sd_year;
        info->month    = cardinfo.sd_mon;
        info->capacity = cardinfo.sd_cap;
        return 0;
    }
    else
    {
        info->type = SDFLASH_UNKNOW;
        info->capacity = 0;
        return 1;
    }

#else

    return 1;

#endif
}

/**
  ******************************************************************************
  * @brief  ��ȡSD���Ŀ�,
  * @param  pbuf,   ��ȡ��ַָ��
  * @param  block,  ����ʼ��ַ, �������ַ. ���СĬ��Ϊ512byte
  * @param  block_num,  ��Ҫ��ȡ��SD������
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdflash_read_blocks(uint8_t* pbuf, uint32_t block, uint32_t block_num)
{
    if (block_num > 1)                                                          // ʹ�ö���д
    {
#if (defined SDFLASH_MODE_SDIO)
        if (SD_ReadMultiBlocks(pbuf, block, block_num)== SD_OK) return 0;
        else return 1;

#elif (defined SDFLASH_MODE_SPIDMA)
        uint8_t rtn = 0;
        do
        {
            rtn |= sdspidma_read_block(pbuf, block++);
            pbuf += SDFLASH_BLOCK_SIZE;
        } while (--block_num);
        return rtn;

#elif (defined SDFLASH_MODE_SPI)
        if (sdspi_read_multi_blocks(pbuf, block, block_num) == 0) return 0;
        else return 1;

#else
        return 1;
#endif
    }
    else if (block_num == 1)                                                    // ʹ�õ����д
    {
#if (defined SDFLASH_MODE_SDIO)
        if (SD_ReadBlock(pbuf, block) == SD_OK) return 0;
        else return 1;

#elif (defined SDFLASH_MODE_SPIDMA)
        if (sdspidma_read_block(pbuf, block) == 0) return 0;
        else return 1;

#elif (defined SDFLASH_MODE_SPI)
        if (sdspi_read_block(pbuf, block) == 0) return 0;
        else return 1;

#else
        return 1;
#endif
    }
    return 0;                                                                   // block_num = 0
}

/**
  ******************************************************************************
  * @brief  д��SD���Ŀ�
  * @param  pbuf,   д���ַָ��
  * @param  block,  ����ʼ��ַ, �������ַ. ���СĬ��Ϊ512byte
  * @param  block_num,  ��Ҫд���SD������
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdflash_write_blocks(const uint8_t* pbuf, uint32_t block, uint32_t block_num)
{
    if (block_num > 1)                                                          // ʹ�ö���д
    {
#if (defined SDFLASH_MODE_SDIO)
        if (SD_WriteMultiBlocks(pbuf, block, block_num) == SD_OK) return 0;
        else return 1;

#elif (defined SDFLASH_MODE_SPIDMA)
        uint8_t rtn = 0;
        do
        {
            rtn |= sdspidma_write_block(pbuf, block++);
            pbuf += SDFLASH_BLOCK_SIZE;
        } while (--block_num);
        return rtn;

#elif (defined SDFLASH_MODE_SPI)
        if (sdspi_write_multi_blocks(pbuf, block, block_num) == 0) return 0;
        else return 1;

#else
        return 1;
#endif
    }
    else if (block_num == 1)                                                    // ʹ�õ����д
    {
#if (defined SDFLASH_MODE_SDIO)
        if (SD_WriteBlock(pbuf, block) == SD_OK) return 0;
        else return 1;

#elif (defined SDFLASH_MODE_SPIDMA)
        if (sdspidma_write_block(pbuf, block) == 0) return 0;
        else return 1;

#elif (defined SDFLASH_MODE_SPI)
        if (sdspi_write_block(pbuf, block) == 0) return 0;
        else return 1;

#else
        return 1;
#endif
    }
    return 0;                                                                   // block_num = 0
}

/**
  ******************************************************************************
  * @brief  ��SD Flash�ж�ȡ����
  * @param  pbuf,      Ҫ��ȡ���ݵĴ�ŵ�ַ
  * @param  read_addr, Ҫ��ȡ�ĵ�ַ
  * @param  num,       Ҫ��ȡ�ĸ���
  * @retval ����ֵ,    �Ƿ��ȡ�ɹ�. 0,�ɹ�. 1,ʧ��
  ******************************************************************************
  */
uint8_t sdflash_read(uint8_t *pbuf, uint32_t read_addr, uint32_t num)
{
    uint32_t addr_offset, addr_sector, num_offset, read_start, read_num, i;

    if (num == 0) return 0;

    addr_offset = read_addr % SDFLASH_SECTOR_SIZE;                              // Sector��ַƫ����
    addr_sector = read_addr - addr_offset;                                      // ȡ��Sector��ʼ��ַ
    num_offset  = addr_offset + num;                                            // ��ƫ���������Ҫ��ȡ���ֽ���
    num_offset  = ((num_offset - 1) / SDFLASH_SECTOR_SIZE) + 1;                 // �������Ҫ��ȡ��Sector����
    read_start  = addr_offset;                                                  // һ��Sector�е����ݶ�ȡ��ʼ��ַ
    if (num_offset == 1)
        read_num = num;                                                         // һ��Sector�е����ݶ�ȡ����
    else
        read_num = SDFLASH_SECTOR_SIZE - addr_offset;

    while (num_offset--)
    {
        if (sector_addr_cur != addr_sector)                                     // ������������FLASH����һ��
        {
            sdflash_write_buf2flash();                                          // ���滺������
            if ( sdflash_read_blocks( data,
                    addr_sector         / SDFLASH_BLOCK_SIZE,
                    SDFLASH_SECTOR_SIZE / SDFLASH_BLOCK_SIZE)
                        != 0 )
            {
                sector_addr_cur = 0xFFFFFFFF;                                   // SD���ѳ���, ����������Ϊ��Ч
                data_changed = 0;
                return 1;
            }
            sector_addr_cur = addr_sector;
        }

        for (i=read_start; i<(read_num+read_start); i++) *pbuf++ = data[i];     // �ӻ�������ȡ����
        addr_sector += SDFLASH_SECTOR_SIZE;
        num -= read_num;
        read_start = 0;
        if (num <= SDFLASH_SECTOR_SIZE)
            read_num = num;
        else
            read_num = SDFLASH_SECTOR_SIZE;
    }

    return 0;
}

/**
  ******************************************************************************
  * @brief  д�뵽������, Ҫд������ݳ�����������, ���Զ���Դ����������д��Flash
  * @param  pbuf,       Ҫд�������ָ��
  * @param  write_addr, Ҫд�뵽��ַ
  * @param  num,        Ҫд��ĸ���
  * @retval ����ֵ,     �Ƿ�д��ɹ�. 0,�ɹ�. 1,ʧ��
  ******************************************************************************
  */
uint8_t sdflash_write_buf(uint8_t *pbuf, uint32_t write_addr, uint32_t num)
{
    uint32_t addr_offset, addr_sector, num_offset, write_start, write_num, i;

    if (num == 0) return 0;

    addr_offset = write_addr % SDFLASH_SECTOR_SIZE;                             // Sector��ַƫ����
    addr_sector = write_addr - addr_offset;                                     // ȡ��Sector��ʼ��ַ
    num_offset  = addr_offset + num;                                            // ��ƫ���������Ҫд����ֽ���
    num_offset  = ((num_offset - 1) / SDFLASH_SECTOR_SIZE) + 1;                 // �������Ҫд���Sector����
    write_start = addr_offset;                                                  // һ��Sector�е�����д����ʼ��ַ
    if (num_offset == 1)
        write_num = num;                                                        // һ��Sector�е�����д������
    else
        write_num = SDFLASH_SECTOR_SIZE - addr_offset;

    while (num_offset--)
    {
        if (sector_addr_cur != addr_sector)                                     // ������������FLASH����һ��
        {
            sdflash_write_buf2flash();                                          // ���滺������
            if ( sdflash_read_blocks( data,
                    addr_sector         / SDFLASH_BLOCK_SIZE,
                    SDFLASH_SECTOR_SIZE / SDFLASH_BLOCK_SIZE)
                        != 0 )
            {
                sector_addr_cur = 0xFFFFFFFF;                                   // SD���ѳ���, ����������Ϊ��Ч
                data_changed = 0;
                return 1;
            }
            sector_addr_cur = addr_sector;
        }

        for (i=write_start; i<(write_num+write_start); i++) data[i] = *pbuf++;  // ���µ�����д�뻺����
        addr_sector += SDFLASH_SECTOR_SIZE;
        data_changed = 1;
        num -= write_num;
        write_start = 0;
        if (num <= SDFLASH_SECTOR_SIZE)
            write_num = num;
        else
            write_num = SDFLASH_SECTOR_SIZE;
    }

    return 0;
}

/**
  ******************************************************************************
  * @brief  ����ǰ������������д�뵽Flash��
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sdflash_write_buf2flash(void)
{
    if (data_changed)
    {
        sdflash_write_blocks( data,
            sector_addr_cur     / SDFLASH_BLOCK_SIZE,
            SDFLASH_SECTOR_SIZE / SDFLASH_BLOCK_SIZE );
        data_changed = 0;
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
