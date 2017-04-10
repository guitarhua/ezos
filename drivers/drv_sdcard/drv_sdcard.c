/**
  ******************************************************************************
  * @file    drv_sdcard.c
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   SD����������, ֧�� SD_V1.x, SD_V2, SDHC��.
  *          ֧��SPIģʽ �� SDIOģʽ, ��Ҫ drv_sdspi �� drv_sdio ����.
  *          ���ڵ�ַʹ�õ��� uint32_t ��ʽ, �������֧�ֵ�4G�Ŀ�.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "drv_sdcard.h"
#include "stm32f10x.h"                                                          // __INLINE, __NOP

#ifdef SDCARD_POWER_NAME
    #include "drv_ctrlport.h"
#endif



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_SDCARD
  * @{
  */
#ifdef SDCARD_ENABLE



/**
  ******************************************************************************
  * @addtogroup DRV_SDCARD_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_SDCARD_Private_TypeDefine
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDCARD_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */
/// SDCARD_BLOCK_SIZE ֻ֧�� 512 byte, ����ᵼ�¶�д����. ԭ����.
#define SDCARD_BLOCK_SIZE                   512
/**
  * @}
  */

/** @defgroup DRV_SDCARD_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDCARD_Private_Function
  * @brief    ����˽�к���
  * @{
  */
/// SD����Դ�������ų�ʼ��
__INLINE static void sdcard_power_port_init(void)
{
#ifdef SDCARD_POWER_NAME
    ctrlport_init(SDCARD_POWER_NAME);
#endif
}

/// ��SD����Դ
__INLINE static void sdcard_power_port_on(void)
{
#ifdef SDCARD_POWER_NAME
    ctrlport_on(SDCARD_POWER_NAME);
#endif
}

/// �ر�SD����Դ
__INLINE static void sdcard_power_port_off()
{
#ifdef SDCARD_MODE_SDIO
    SD_PowerOFF();
#endif
#ifdef SDCARD_POWER_NAME
    ctrlport_off(SDCARD_POWER_NAME);
#endif
}

/// SD ���ڼ���źų�ʼ��, IO������Ϊ��������
__INLINE static void sdcard_detect_port_init(void)
{
#ifdef SDCARD_DETECT_ENABLE
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(SDCARD_DETECT_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin  = SDCARD_DETECT_PIN;
    GPIO_Init(SDCARD_DETECT_PORT, &GPIO_InitStructure);
#endif
}

/// ��ȡSD�����IO�ڵ�ǰ��ƽֵ. 0, �͵�ƽ. !0, �ߵ�ƽ
__INLINE static uint32_t sdcard_detect_port_level(void)
{
#ifdef SDCARD_DETECT_ENABLE
    return (SDCARD_DETECT_PORT->IDR & SDCARD_DETECT_PIN);
#else
    return 0;                                                                   // ʼ����Ϊ����
#endif
}
/**
  * @}
  */



/** @defgroup DRV_SDCARD_Function
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
void sdcard_port_init(void)
{
    sdcard_power_port_init();                                                   // ��ʼ����Դ���ƿ�
    sdcard_detect_port_init();
#if (defined SDCARD_MODE_SDIO)
    SD_Port_Init();
#elif (defined SDCARD_MODE_SPIDMA)
    sdspidma_port_init();
#elif (defined SDCARD_MODE_SPI)
    sdspi_port_init();
#endif
}

/**
  ******************************************************************************
  * @brief  ��SD���ĵ�Դ
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sdcard_power_on(void)
{
    sdcard_power_port_on();
}

/**
  ******************************************************************************
  * @brief  �ر�SD���ĵ�Դ
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sdcard_power_off(void)
{
    sdcard_power_port_off();
}

/**
  ******************************************************************************
  * @brief  ��SD����Դ����ʼ��
  * @param  none
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdcard_init(void)
{
    sdcard_power_port_on();                                                     // ��SD����Դ
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();                                // ��һ������ʱ
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    if (!sdcard_present()) return 1;                                            // SD��������, ֱ���˳�
#if (defined SDCARD_MODE_SDIO)
    if (SD_Init(SDCARD_BLOCK_SIZE) == SD_OK) return 0;
    else                                     return 1;
#elif (defined SDCARD_MODE_SPIDMA)
    if (sdspidma_init(SDCARD_BLOCK_SIZE) == 0) return 0;
    else                                       return 1;
#elif (defined SDCARD_MODE_SPI)
    if (sdspi_init(SDCARD_BLOCK_SIZE) == 0) return 0;
    else                                    return 1;
#else
    return 1;
#endif
}

/**
  ******************************************************************************
 * @brief  ���SD���Ƿ����
 * @param  none
 * @retval ����ֵ, 0, ������. 1, ����
 *******************************************************************************
 */
uint8_t sdcard_present(void)
{
    if (sdcard_detect_port_level())
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
  * @brief  ��ȡSD����Ϣ
  * @param  info,   SD����Ϣָ��
  * @retval ����ֵ, 0, �ɹ�. 1, ʧ��
  ******************************************************************************
  */
uint8_t sdcard_get_info(sdcard_info_t *info)
{
#if (defined SDCARD_MODE_SDIO)

    SD_CardInfo cardinfo;

    if (SD_GetCardInfo(&cardinfo) == SD_OK)
    {
        switch (cardinfo.CardType)                                              // SD������
        {
        case SDIO_STD_CAPACITY_SD_CARD_V1_1:
            info->type = SDCARD_V1;
            break;
        case SDIO_STD_CAPACITY_SD_CARD_V2_0:
            info->type = SDCARD_V2;
            break;
        case SDIO_HIGH_CAPACITY_SD_CARD:
            info->type = SDCARD_SDHC;
            break;
        default:
            info->type = SDCARD_UNKNOW;
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
        info->type = SDCARD_UNKNOW;
        info->capacity = 0;
        return 1;
    }

#elif (defined SDCARD_MODE_SPIDMA)

    sd_info_t cardinfo;

    if (sdspidma_get_info(&cardinfo) == 0)
    {
        switch (cardinfo.sd_type)                                               // SD������
        {
        case SD_V1:
            info->type = SDCARD_V1;
            break;
        case SD_V2:
            info->type = SDCARD_V2;
            break;
        case SDHC:
            info->type = SDCARD_SDHC;
            break;
        case MMC:
            info->type = SDCARD_MMC;
            break;
        default:
            info->type = SDCARD_UNKNOW;
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
        info->type = SDCARD_UNKNOW;
        info->capacity = 0;
        return 1;
    }

#elif (defined SDCARD_MODE_SPI)

    sd_info_t cardinfo;

    if (sdspi_get_info(&cardinfo) == 0)
    {
        switch (cardinfo.sd_type)                                               // SD������
        {
        case SD_V1:
            info->type = SDCARD_V1;
            break;
        case SD_V2:
            info->type = SDCARD_V2;
            break;
        case SDHC:
            info->type = SDCARD_SDHC;
            break;
        case MMC:
            info->type = SDCARD_MMC;
            break;
        default:
            info->type = SDCARD_UNKNOW;
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
        info->type = SDCARD_UNKNOW;
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
uint8_t sdcard_read_blocks(uint8_t* pbuf, uint32_t block, uint32_t block_num)
{
    if (block_num > 1)                                                          // ʹ�ö���д
    {
#if (defined SDCARD_MODE_SDIO)
        if (SD_ReadMultiBlocks(pbuf, block, block_num) == SD_OK) return 0;
        else return 1;

#elif (defined SDCARD_MODE_SPIDMA)
        uint8_t rtn = 0;
        do
        {
            rtn |= sdspidma_read_block(pbuf, block++);
            pbuf += SDCARD_BLOCK_SIZE;
        } while (--block_num);
        return rtn;

#elif (defined SDCARD_MODE_SPI)
        if (sdspi_read_multi_blocks(pbuf, block, block_num) == 0) return 0;
        else return 1;

#else
        return 1;
#endif
    }
    else if (block_num == 1)                                                    // ʹ�õ����д
    {
#if (defined SDCARD_MODE_SDIO)
        if (SD_ReadBlock(pbuf, block) == SD_OK) return 0;
        else return 1;

#elif (defined SDCARD_MODE_SPIDMA)
        if (sdspidma_read_block(pbuf, block) == 0) return 0;
        else return 1;

#elif (defined SDCARD_MODE_SPI)
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
uint8_t sdcard_write_blocks(const uint8_t* pbuf, uint32_t block, uint32_t block_num)
{
    if (block_num > 1)                                                          // ʹ�ö���д
    {
#if (defined SDCARD_MODE_SDIO)
        if (SD_WriteMultiBlocks(pbuf, block, block_num) == SD_OK) return 0;
        else return 1;

#elif (defined SDCARD_MODE_SPIDMA)
        uint8_t rtn = 0;
        do
        {
            rtn |= sdspidma_write_block(pbuf, block++);
            pbuf += SDCARD_BLOCK_SIZE;
        } while (--block_num);
        return val;

#elif (defined SDCARD_MODE_SPI)
        if (sdspi_write_multi_blocks(pbuf, block, block_num) == 0) return 0;
        else return 1;

#else
        return 1;
#endif
    }
    else if (block_num == 1)                                                    // ʹ�õ����д
    {
#if (defined SDCARD_MODE_SDIO)
        if (SD_WriteBlock(pbuf, block) == SD_OK) return 0;
        else return 1;

#elif (defined SDCARD_MODE_SPIDMA)
        if (sdspidma_write_block(pbuf, block) == 0) return 0;
        else return 1;

#elif (defined SDCARD_MODE_SPI)
        if (sdspi_write_block(pbuf, block) == 0) return 0;
        else return 1;

#else
        return 1;
#endif
    }
    return 0;                                                                   // block_num = 0
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
