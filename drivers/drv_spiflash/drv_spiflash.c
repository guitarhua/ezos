/**
  ******************************************************************************
  * @file    drv_spiflash.c
  * @author  mousie-yu
  * @version V2.0.1
  * @date    2013.2.10
  * @brief   SPI Flash ͨ����������. ��Ҫ��Ʒ�Ƶ�SPI FLASH�ײ�����֧��.
  *          ���ݲ�ͬ�ĵײ�����, ��֧�ֱ�׼SPIģʽ, ģ�����ģʽ, 4IOģʽ.
  *          v2.0.1, ����һ��������ȡʱ, ���ݴ��������bug.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "drv_spiflash.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_SpiFlash
  * @{
  */
#if (SPIFLASH_MBIT > 0)



/**
  ******************************************************************************
  * @addtogroup DRV_SpiFlash_Configure
  ******************************************************************************
  * @{
  */
/**
  * @}
  */



/** @defgroup DRV_SpiFlash_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */
#define SPIFLASH_PAGE_SIZE              0x100                                   ///< ����PAGE��С
#define SPIFLASH_SECTOR_SIZE            0x1000                                  ///< ����SECTOR��С

#if   (SPIFLASH_MBIT == 1  )
    #define SPIFLASH_CHIP_SIZE          0x00020000                              ///< SPI Flash���ֽ�����
#elif (SPIFLASH_MBIT == 2  )
    #define SPIFLASH_CHIP_SIZE          0x00040000
#elif (SPIFLASH_MBIT == 4  )
    #define SPIFLASH_CHIP_SIZE          0x00080000
#elif (SPIFLASH_MBIT == 8  )
    #define SPIFLASH_CHIP_SIZE          0x00100000
#elif (SPIFLASH_MBIT == 16 )
    #define SPIFLASH_CHIP_SIZE          0x00200000
#elif (SPIFLASH_MBIT == 32 )
    #define SPIFLASH_CHIP_SIZE          0x00400000
#elif (SPIFLASH_MBIT == 64 )
    #define SPIFLASH_CHIP_SIZE          0x00800000
#elif (SPIFLASH_MBIT == 128)
    #define SPIFLASH_CHIP_SIZE          0x01000000
#elif (SPIFLASH_MBIT == 256)
    #define SPIFLASH_CHIP_SIZE          0x02000000
#else
    #error "Set SPIFLASH_MBIT error!!!"
#endif
/**
  * @}
  */

/** @defgroup DRV_SpiFlash_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */
static uint8_t  data[SPIFLASH_SECTOR_SIZE];                                     // ��¼������������
static uint32_t sector_addr_cur;                                                // ��¼��ǰ������ʼ��ַ
static uint8_t  data_changed;                                                   // ��¼�����������Ƿ񱻸��Ĺ�, !0��ʾ���Ĺ�
/**
  * @}
  */

/** @defgroup DRV_SpiFlash_Private_Function
  * @brief    ����˽�к���
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_SpiFlash_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  Spi Flash SPIͨѶ����Ӧ�Ķ˿ڳ�ʼ������
  * @param  none
  * @retval none
  ******************************************************************************
  */
void spiflash_init(void)
{
#ifdef SPIFLASH_UNIVERSAL
    sf_unvsl_init();
#endif

#ifdef MX25L_4DTRD_4PP
    mx25l_init();
#endif

    sector_addr_cur = 0xFFFFFFFF;
    data_changed = 0;
}

/**
  ******************************************************************************
  * @brief  ����оƬ״̬�Ĵ���, ��������ֻ������
  * @param  none
  * @retval none
  ******************************************************************************
  */
void spiflash_set_protect(uint8_t status)
{
#ifdef SPIFLASH_UNIVERSAL
    sf_unvsl_set_protect(status);
#endif

#ifdef MX25L_4DTRD_4PP
    mx25l_set_protect(status);
#endif
}

/**
  ******************************************************************************
  * @brief  оƬ��������ģʽ, �ĵ����. ���޷����ж�д����.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void spiflash_power_down(void)
{
#ifdef SPIFLASH_UNIVERSAL
    sf_unvsl_power_down();
#endif

#ifdef MX25L_4DTRD_4PP
    mx25l_power_down();
#endif
}

/**
  ******************************************************************************
  * @brief  оƬ�˳�����ģʽ. �˳���ɽ��ж�д����
  * @param  none
  * @retval none
  ******************************************************************************
  */
void spiflash_power_on(void)
{
#ifdef SPIFLASH_UNIVERSAL
    sf_unvsl_power_on();
#endif

#ifdef MX25L_4DTRD_4PP
    mx25l_power_on();
#endif
}

/**
  ******************************************************************************
  * @brief  ��ȡоƬID��Ϣ
  * @param  none
  * @retval ����ֵ, оƬID��Ϣ
  ******************************************************************************
  */
uint32_t spiflash_read_id(void)
{
    uint32_t rtn = 0;

#ifdef SPIFLASH_UNIVERSAL
    rtn = sf_unvsl_read_id();
#endif

#ifdef MX25L_4DTRD_4PP
    rtn = mx25l_read_id();
#endif

    return rtn;
}

/**
  ******************************************************************************
  * @brief  ����һ����(sector),��СΪ4K byte.
  * @param  addr, �ֽڵ�ַ. �����õ�ַ���ڵ�������
  * @retval ����ֵ, �Ƿ�����ɹ�. 0,�ɹ�. 1,ʧ��
  ******************************************************************************
  */
uint8_t spiflash_sector_erase(uint32_t addr)
{
    assert_param(addr < SPIFLASH_CHIP_SIZE);
    if (addr >= SPIFLASH_CHIP_SIZE) return 1;

#ifdef SPIFLASH_UNIVERSAL
    sf_unvsl_sector_erase(addr);
#endif

#ifdef MX25L_4DTRD_4PP
    mx25l_sector_erase(addr);
#endif

    return 0;
}

/**
  ******************************************************************************
  * @brief  ����һ����(block),��СΪ64K byte.
  * @param  addr, �ֽڵ�ַ. �����õ�ַ���ڵĿ�����
  * @retval ����ֵ, �Ƿ�����ɹ�. 0,�ɹ�. 1,ʧ��
  ******************************************************************************
  */
uint8_t spiflash_block_erase(uint32_t addr)
{
    assert_param(addr < SPIFLASH_CHIP_SIZE);
    if (addr >= SPIFLASH_CHIP_SIZE) return 1;

#ifdef SPIFLASH_UNIVERSAL
    sf_unvsl_block_erase(addr);
#endif

#ifdef MX25L_4DTRD_4PP
    mx25l_block_erase(addr);
#endif

    return 0;
}

/**
  ******************************************************************************
  * @brief  ����оƬ����������(chip). ����ʱ���оƬ��С�й�.
  *         ����ʱ��Ϊ�뼶���. ��Ҫ������ʱ�ȴ������.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void spiflash_chip_erase(void)
{
#ifdef SPIFLASH_UNIVERSAL
    sf_unvsl_chip_erase();
#endif

#ifdef MX25L_4DTRD_4PP
    mx25l_chip_erase();
#endif
}

/**
  ******************************************************************************
  * @brief  �����ȡģʽ
  * @param  read_addr, ��ȡ���ݵ���ʼ��ַ
  * @retval none
  ******************************************************************************
  */
void spiflash_read_start(uint32_t read_addr)
{
#ifdef SPIFLASH_UNIVERSAL
    sf_unvsl_read_start(read_addr);
#endif

#ifdef MX25L_4DTRD_4PP
    mx25l_read_start(read_addr);
#endif
}

/**
  ******************************************************************************
  * @brief  ��ȡһ������
  * @param  pbuf, ��ȡ��ַָ��
  * @param  num,  ��ȡ���ݵ��ֽ���
  * @retval none
  ******************************************************************************
  */
void spiflash_read_bytes(uint8_t *pbuf, uint32_t num)
{
#ifdef SPIFLASH_UNIVERSAL
    sf_unvsl_read_bytes(pbuf, num);
#endif

#ifdef MX25L_4DTRD_4PP
    mx25l_read_bytes(pbuf, num);
#endif
}

/**
  ******************************************************************************
  * @brief  �˳���ȡģʽ
  * @param  none
  * @retval none
  ******************************************************************************
  */
void spiflash_read_end(void)
{
#ifdef SPIFLASH_UNIVERSAL
    sf_unvsl_read_end();
#endif

#ifdef MX25L_4DTRD_4PP
    mx25l_read_end();
#endif
}

/**
  ******************************************************************************
  * @brief  дһ��ҳ
  * @param  pbuf,       Ҫд�������ָ��
  * @param  write_addr, Ҫд�뵽ҳ��ַ
  * @param  num,        Ҫд��ĸ���, ���ܳ���ҳ��С.
  * @retval none
  ******************************************************************************
  */
__INLINE void spiflash_write_page(uint8_t *pbuf, uint32_t write_addr, uint32_t num)
{
    assert_param(num <= SPIFLASH_PAGE_SIZE);

#ifdef SPIFLASH_UNIVERSAL
    sf_unvsl_write_page(pbuf, write_addr, num);
#endif

#ifdef MX25L_4DTRD_4PP
    mx25l_write_page(pbuf, write_addr, num);
#endif
}

/**
  ******************************************************************************
  * @brief  ��SPI Flash�ж�ȡ����
  * @param  pbuf,      Ҫ��ȡ���ݵĴ�ŵ�ַ
  * @param  read_addr, Ҫ��ȡ�ĵ�ַ
  * @param  num,       Ҫ��ȡ�ĸ���
  * @retval ����ֵ,    �Ƿ��ȡ�ɹ�. 0,�ɹ�. 1,ʧ��
  ******************************************************************************
  */
uint8_t spiflash_read(uint8_t *pbuf, uint32_t read_addr, uint32_t num)
{
    uint32_t addr_offset, addr_sector, num_offset, read_start, read_num, i;

    assert_param((read_addr + num) <= SPIFLASH_CHIP_SIZE);
    if ((read_addr + num ) > SPIFLASH_CHIP_SIZE) return(1);
    if (num == 0) return 0;

    addr_offset = read_addr % SPIFLASH_SECTOR_SIZE;                             // Sector��ַƫ����
    addr_sector = read_addr - addr_offset;                                      // ȡ��Sector��ʼ��ַ
    num_offset  = addr_offset + num;                                            // ��ƫ���������Ҫ��ȡ���ֽ���
    num_offset  = ((num_offset - 1) / SPIFLASH_SECTOR_SIZE) + 1;                // �������Ҫ��ȡ��Sector����
    read_start  = addr_offset;                                                  // һ��Sector�е����ݶ�ȡ��ʼ��ַ
    if (num_offset == 1)
        read_num = num;                                                         // һ��Sector�е����ݶ�ȡ����
    else
        read_num = SPIFLASH_SECTOR_SIZE - addr_offset;

    while (num_offset--)
    {
        if (sector_addr_cur != addr_sector)                                     // ������������FLASH����һ��
        {
            spiflash_read_start(read_start + addr_sector);
            spiflash_read_bytes(pbuf, read_num);
            spiflash_read_end();
            pbuf += read_num;                                                   // v201����, ȷ��������ָ���ַ����
        }
        else                                                                    // ֱ�Ӵӻ�������ȡ����
        {
            for (i=read_start; i<(read_num+read_start); i++)
                *pbuf++ = data[i];
        }

        addr_sector += SPIFLASH_SECTOR_SIZE;
        num -= read_num;
        read_start = 0;
        if (num <= SPIFLASH_SECTOR_SIZE)
            read_num = num;
        else
            read_num = SPIFLASH_SECTOR_SIZE;
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
uint8_t spiflash_write_buf(uint8_t *pbuf, uint32_t write_addr, uint32_t num)
{
    uint32_t addr_offset, addr_sector, num_offset, write_start, write_num, i;

    assert_param((write_addr + num) <= SPIFLASH_CHIP_SIZE);
    if ((write_addr + num ) > SPIFLASH_CHIP_SIZE) return(1);
    if (num == 0) return 0;

    addr_offset = write_addr % SPIFLASH_SECTOR_SIZE;                            // Sector��ַƫ����
    addr_sector = write_addr - addr_offset;                                     // ȡ��Sector��ʼ��ַ
    num_offset  = addr_offset + num;                                            // ��ƫ���������Ҫд����ֽ���
    num_offset  = ((num_offset - 1) / SPIFLASH_SECTOR_SIZE) + 1;                // �������Ҫд���Sector����
    write_start = addr_offset;                                                  // һ��Sector�е�����д����ʼ��ַ
    if (num_offset == 1)
        write_num = num;                                                        // һ��Sector�е�����д������
    else
        write_num = SPIFLASH_SECTOR_SIZE - addr_offset;

    while (num_offset--)
    {
        if (sector_addr_cur != addr_sector)                                     // ������������FLASH����һ��
        {
            if (data_changed)                                                   // �������ݸ��Ĺ�
            {
                spiflash_sector_erase(sector_addr_cur);
                for (i=0; i<SPIFLASH_SECTOR_SIZE ; i+=SPIFLASH_PAGE_SIZE)       // ����������д��FLASH��
                {
                    spiflash_write_page(data+i, sector_addr_cur, SPIFLASH_PAGE_SIZE);
                    sector_addr_cur += SPIFLASH_PAGE_SIZE;
                }
            }
            spiflash_read(data, addr_sector, SPIFLASH_SECTOR_SIZE);             // ��ȡ�µ�FLASH���ݵ�����
            sector_addr_cur = addr_sector;
        }

        for (i=write_start; i<(write_num+write_start); i++) data[i] = *pbuf++;  // ���µ�����д�뻺����
        addr_sector += SPIFLASH_SECTOR_SIZE;
        data_changed = 1;
        num -= write_num;
        write_start = 0;
        if (num <= SPIFLASH_SECTOR_SIZE)
            write_num = num;
        else
            write_num = SPIFLASH_SECTOR_SIZE;
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
void spiflash_write_buf2flash(void)
{
    uint32_t temp_addr, i;

    if (data_changed)
    {
        temp_addr = sector_addr_cur;
        spiflash_sector_erase(temp_addr);
        for (i=0; i<SPIFLASH_SECTOR_SIZE ; i+=SPIFLASH_PAGE_SIZE)               // ����������д��FLASH��
        {
            spiflash_write_page(data+i, temp_addr, SPIFLASH_PAGE_SIZE);
            temp_addr += SPIFLASH_PAGE_SIZE;
        }
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

