/**
  ******************************************************************************
  * @file    drv_sdflash.c
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   将SD卡作为FLASH使用的驱动程序.
  *          支持 SD_V1.x, SD_V2, SDHC卡.
  *          支持SPI模式 和 SDIO模式, 否则会导致读写错误. 原因不明.
  *          由于地址使用的是 uint32_t 格式, 因此最大可支持到4G的卡.
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
  * @brief    私有类型定义
  * @{
  */
/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
#define SDFLASH_BLOCK_SIZE              512                                     ///< SD卡块大小, 只支持 512 byte, 否则会导致读写错误. 原因不明.
#define SDFLASH_SECTOR_SIZE             0x400                                   ///< 定义SD缓存区大小, 作为SECTOR概念
/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
static uint8_t  data[SDFLASH_SECTOR_SIZE];                                      // 记录整个区的数据
static uint32_t sector_addr_cur;                                                // 记录当前区的起始地址
static uint8_t  data_changed;                                                   // 记录缓存块数据是否被更改过, !0表示更改过
/**
  * @}
  */

/** @defgroup DRV_SDFLASH_Private_Function
  * @brief    定义私有函数
  * @{
  */
/// SD卡电源控制引脚初始化
__INLINE static void sdflash_power_port_init(void)
{
#ifdef SDFLASH_POWER_NAME
    ctrlport_init(SDFLASH_POWER_NAME);
#endif
}

/// 打开SD卡电源
__INLINE static void sdflash_power_port_on(void)
{
#ifdef SDFLASH_POWER_NAME
    ctrlport_on(SDFLASH_POWER_NAME);
#endif
}

/// 关闭SD卡电源
__INLINE static void sdflash_power_port_off()
{
#ifdef SDFLASH_MODE_SDIO
    SD_PowerOFF();
#endif
#ifdef SDFLASH_POWER_NAME
    ctrlport_off(SDFLASH_POWER_NAME);
#endif
}

/// SD 存在检测信号初始化, IO口设置为上拉输入
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

/// 获取SD卡检测IO口当前电平值. 0, 低电平. !0, 高电平
__INLINE static uint32_t sdflash_detect_port_level(void)
{
#ifdef SDFLASH_DETECT_ENABLE
    return (SDFLASH_DETECT_PORT->IDR & SDFLASH_DETECT_PIN);
#else
    return 0;                                                                   // 始终认为存在
#endif
}
/**
  * @}
  */



/** @defgroup DRV_SDFLASH_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  SD卡的电源, SPI通讯和相应的端口初始化函数
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sdflash_port_init(void)
{
    sdflash_power_port_init();                                                  // 初始化电源控制口
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
 * @brief  检测SD卡是否存在
 * @param  none
 * @retval 返回值, 0, 不存在. 1, 存在
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
  * @brief  打开SD卡电源并初始化
  * @param  none
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdflash_init(void)
{
    uint8_t i;

    sdflash_port_init();                                                        // 默认关闭电源
    for (i=0; i<100; i++) { __NOP(); }                                          // 给一定的延时
    sdflash_power_port_on();                                                    // 打开SD卡电源

    sector_addr_cur = 0xFFFFFFFF;
    data_changed = 0;
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();                                // 给一定的延时
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();

    if (!sdflash_present()) return 1;                                           // SD卡不存在, 直接退出
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
  * @brief  关闭SD卡的电源
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
  * @brief  读取SD卡信息
  * @param  info,   SD卡信息指针
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdflash_get_info(sdflash_info_t *info)
{
#if (defined SDFLASH_MODE_SDIO)

    SD_CardInfo cardinfo;

    if (SD_GetCardInfo(&cardinfo) == SD_OK)
    {
        switch (cardinfo.CardType)                                              // SD卡类型
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
        switch (cardinfo.sd_type)                                               // SD卡类型
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
        switch (cardinfo.sd_type)                                               // SD卡类型
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
  * @brief  读取SD卡的块,
  * @param  pbuf,   读取地址指针
  * @param  block,  块起始地址, 非物理地址. 块大小默认为512byte
  * @param  block_num,  需要读取的SD块数量
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdflash_read_blocks(uint8_t* pbuf, uint32_t block, uint32_t block_num)
{
    if (block_num > 1)                                                          // 使用多块读写
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
    else if (block_num == 1)                                                    // 使用单块读写
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
  * @brief  写入SD卡的块
  * @param  pbuf,   写入地址指针
  * @param  block,  块起始地址, 非物理地址. 块大小默认为512byte
  * @param  block_num,  需要写入的SD块数量
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdflash_write_blocks(const uint8_t* pbuf, uint32_t block, uint32_t block_num)
{
    if (block_num > 1)                                                          // 使用多块读写
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
    else if (block_num == 1)                                                    // 使用单块读写
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
  * @brief  从SD Flash中读取数据
  * @param  pbuf,      要读取数据的存放地址
  * @param  read_addr, 要读取的地址
  * @param  num,       要读取的个数
  * @retval 返回值,    是否读取成功. 0,成功. 1,失败
  ******************************************************************************
  */
uint8_t sdflash_read(uint8_t *pbuf, uint32_t read_addr, uint32_t num)
{
    uint32_t addr_offset, addr_sector, num_offset, read_start, read_num, i;

    if (num == 0) return 0;

    addr_offset = read_addr % SDFLASH_SECTOR_SIZE;                              // Sector地址偏移量
    addr_sector = read_addr - addr_offset;                                      // 取出Sector起始地址
    num_offset  = addr_offset + num;                                            // 将偏移量计算进要读取的字节数
    num_offset  = ((num_offset - 1) / SDFLASH_SECTOR_SIZE) + 1;                 // 计算出需要读取的Sector数量
    read_start  = addr_offset;                                                  // 一个Sector中的数据读取起始地址
    if (num_offset == 1)
        read_num = num;                                                         // 一个Sector中的数据读取数量
    else
        read_num = SDFLASH_SECTOR_SIZE - addr_offset;

    while (num_offset--)
    {
        if (sector_addr_cur != addr_sector)                                     // 缓存数据区与FLASH区不一致
        {
            sdflash_write_buf2flash();                                          // 保存缓存数据
            if ( sdflash_read_blocks( data,
                    addr_sector         / SDFLASH_BLOCK_SIZE,
                    SDFLASH_SECTOR_SIZE / SDFLASH_BLOCK_SIZE)
                        != 0 )
            {
                sector_addr_cur = 0xFFFFFFFF;                                   // SD卡已出错, 缓存数据置为无效
                data_changed = 0;
                return 1;
            }
            sector_addr_cur = addr_sector;
        }

        for (i=read_start; i<(read_num+read_start); i++) *pbuf++ = data[i];     // 从缓存区读取数据
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
  * @brief  写入到缓存区, 要写入的内容超过缓存区后, 会自动将源缓存区内容写入Flash
  * @param  pbuf,       要写入的数据指针
  * @param  write_addr, 要写入到地址
  * @param  num,        要写入的个数
  * @retval 返回值,     是否写入成功. 0,成功. 1,失败
  ******************************************************************************
  */
uint8_t sdflash_write_buf(uint8_t *pbuf, uint32_t write_addr, uint32_t num)
{
    uint32_t addr_offset, addr_sector, num_offset, write_start, write_num, i;

    if (num == 0) return 0;

    addr_offset = write_addr % SDFLASH_SECTOR_SIZE;                             // Sector地址偏移量
    addr_sector = write_addr - addr_offset;                                     // 取出Sector起始地址
    num_offset  = addr_offset + num;                                            // 将偏移量计算进要写入的字节数
    num_offset  = ((num_offset - 1) / SDFLASH_SECTOR_SIZE) + 1;                 // 计算出需要写入的Sector数量
    write_start = addr_offset;                                                  // 一个Sector中的数据写入起始地址
    if (num_offset == 1)
        write_num = num;                                                        // 一个Sector中的数据写入数量
    else
        write_num = SDFLASH_SECTOR_SIZE - addr_offset;

    while (num_offset--)
    {
        if (sector_addr_cur != addr_sector)                                     // 缓存数据区与FLASH区不一致
        {
            sdflash_write_buf2flash();                                          // 保存缓存数据
            if ( sdflash_read_blocks( data,
                    addr_sector         / SDFLASH_BLOCK_SIZE,
                    SDFLASH_SECTOR_SIZE / SDFLASH_BLOCK_SIZE)
                        != 0 )
            {
                sector_addr_cur = 0xFFFFFFFF;                                   // SD卡已出错, 缓存数据置为无效
                data_changed = 0;
                return 1;
            }
            sector_addr_cur = addr_sector;
        }

        for (i=write_start; i<(write_num+write_start); i++) data[i] = *pbuf++;  // 将新的数据写入缓存区
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
  * @brief  将当前缓存区的内容写入到Flash中
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
