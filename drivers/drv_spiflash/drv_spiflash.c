/**
  ******************************************************************************
  * @file    drv_spiflash.c
  * @author  mousie-yu
  * @version V2.0.1
  * @date    2013.2.10
  * @brief   SPI Flash 通用驱动程序. 需要各品牌的SPI FLASH底层驱动支持.
  *          根据不同的底层驱动, 可支持标准SPI模式, 模拟快速模式, 4IO模式.
  *          v2.0.1, 修正一个跨区读取时, 数据错误的严重bug.
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
  * @brief    私有宏定义
  * @{
  */
#define SPIFLASH_PAGE_SIZE              0x100                                   ///< 定义PAGE大小
#define SPIFLASH_SECTOR_SIZE            0x1000                                  ///< 定义SECTOR大小

#if   (SPIFLASH_MBIT == 1  )
    #define SPIFLASH_CHIP_SIZE          0x00020000                              ///< SPI Flash的字节容量
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
  * @brief    定义全局变量(私有/公有)
  * @{
  */
static uint8_t  data[SPIFLASH_SECTOR_SIZE];                                     // 记录整个区的数据
static uint32_t sector_addr_cur;                                                // 记录当前区的起始地址
static uint8_t  data_changed;                                                   // 记录缓存区数据是否被更改过, !0表示更改过
/**
  * @}
  */

/** @defgroup DRV_SpiFlash_Private_Function
  * @brief    定义私有函数
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_SpiFlash_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  Spi Flash SPI通讯和相应的端口初始化函数
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
  * @brief  设置芯片状态寄存器, 用于设置只读区域
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
  * @brief  芯片进入休眠模式, 耗电更少. 但无法进行读写操作.
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
  * @brief  芯片退出休眠模式. 退出后可进行读写操作
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
  * @brief  读取芯片ID信息
  * @param  none
  * @retval 返回值, 芯片ID信息
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
  * @brief  擦除一个区(sector),大小为4K byte.
  * @param  addr, 字节地址. 擦除该地址所在的区内容
  * @retval 返回值, 是否擦除成功. 0,成功. 1,失败
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
  * @brief  擦除一个块(block),大小为64K byte.
  * @param  addr, 字节地址. 擦除该地址所在的块内容
  * @retval 返回值, 是否擦除成功. 0,成功. 1,失败
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
  * @brief  擦除芯片的所有内容(chip). 擦除时间和芯片大小有关.
  *         擦除时间为秒级别的. 需要自行延时等待其完成.
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
  * @brief  进入读取模式
  * @param  read_addr, 读取数据的起始地址
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
  * @brief  读取一串数据
  * @param  pbuf, 读取地址指针
  * @param  num,  读取数据的字节数
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
  * @brief  退出读取模式
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
  * @brief  写一整页
  * @param  pbuf,       要写入的数据指针
  * @param  write_addr, 要写入到页地址
  * @param  num,        要写入的个数, 不能超过页大小.
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
  * @brief  从SPI Flash中读取数据
  * @param  pbuf,      要读取数据的存放地址
  * @param  read_addr, 要读取的地址
  * @param  num,       要读取的个数
  * @retval 返回值,    是否读取成功. 0,成功. 1,失败
  ******************************************************************************
  */
uint8_t spiflash_read(uint8_t *pbuf, uint32_t read_addr, uint32_t num)
{
    uint32_t addr_offset, addr_sector, num_offset, read_start, read_num, i;

    assert_param((read_addr + num) <= SPIFLASH_CHIP_SIZE);
    if ((read_addr + num ) > SPIFLASH_CHIP_SIZE) return(1);
    if (num == 0) return 0;

    addr_offset = read_addr % SPIFLASH_SECTOR_SIZE;                             // Sector地址偏移量
    addr_sector = read_addr - addr_offset;                                      // 取出Sector起始地址
    num_offset  = addr_offset + num;                                            // 将偏移量计算进要读取的字节数
    num_offset  = ((num_offset - 1) / SPIFLASH_SECTOR_SIZE) + 1;                // 计算出需要读取的Sector数量
    read_start  = addr_offset;                                                  // 一个Sector中的数据读取起始地址
    if (num_offset == 1)
        read_num = num;                                                         // 一个Sector中的数据读取数量
    else
        read_num = SPIFLASH_SECTOR_SIZE - addr_offset;

    while (num_offset--)
    {
        if (sector_addr_cur != addr_sector)                                     // 缓存数据区与FLASH区不一致
        {
            spiflash_read_start(read_start + addr_sector);
            spiflash_read_bytes(pbuf, read_num);
            spiflash_read_end();
            pbuf += read_num;                                                   // v201新增, 确保缓冲区指针地址自增
        }
        else                                                                    // 直接从缓存区读取数据
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
  * @brief  写入到缓存区, 要写入的内容超过缓存区后, 会自动将源缓存区内容写入Flash
  * @param  pbuf,       要写入的数据指针
  * @param  write_addr, 要写入到地址
  * @param  num,        要写入的个数
  * @retval 返回值,     是否写入成功. 0,成功. 1,失败
  ******************************************************************************
  */
uint8_t spiflash_write_buf(uint8_t *pbuf, uint32_t write_addr, uint32_t num)
{
    uint32_t addr_offset, addr_sector, num_offset, write_start, write_num, i;

    assert_param((write_addr + num) <= SPIFLASH_CHIP_SIZE);
    if ((write_addr + num ) > SPIFLASH_CHIP_SIZE) return(1);
    if (num == 0) return 0;

    addr_offset = write_addr % SPIFLASH_SECTOR_SIZE;                            // Sector地址偏移量
    addr_sector = write_addr - addr_offset;                                     // 取出Sector起始地址
    num_offset  = addr_offset + num;                                            // 将偏移量计算进要写入的字节数
    num_offset  = ((num_offset - 1) / SPIFLASH_SECTOR_SIZE) + 1;                // 计算出需要写入的Sector数量
    write_start = addr_offset;                                                  // 一个Sector中的数据写入起始地址
    if (num_offset == 1)
        write_num = num;                                                        // 一个Sector中的数据写入数量
    else
        write_num = SPIFLASH_SECTOR_SIZE - addr_offset;

    while (num_offset--)
    {
        if (sector_addr_cur != addr_sector)                                     // 缓存数据区与FLASH区不一致
        {
            if (data_changed)                                                   // 缓存数据更改过
            {
                spiflash_sector_erase(sector_addr_cur);
                for (i=0; i<SPIFLASH_SECTOR_SIZE ; i+=SPIFLASH_PAGE_SIZE)       // 将缓存数据写入FLASH中
                {
                    spiflash_write_page(data+i, sector_addr_cur, SPIFLASH_PAGE_SIZE);
                    sector_addr_cur += SPIFLASH_PAGE_SIZE;
                }
            }
            spiflash_read(data, addr_sector, SPIFLASH_SECTOR_SIZE);             // 读取新的FLASH数据到缓存
            sector_addr_cur = addr_sector;
        }

        for (i=write_start; i<(write_num+write_start); i++) data[i] = *pbuf++;  // 将新的数据写入缓存区
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
  * @brief  将当前缓存区的内容写入到Flash中
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
        for (i=0; i<SPIFLASH_SECTOR_SIZE ; i+=SPIFLASH_PAGE_SIZE)               // 将缓存数据写入FLASH中
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

