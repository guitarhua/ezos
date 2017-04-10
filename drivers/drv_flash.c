/**
  ******************************************************************************
  * @file    drv_flash.c
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2015.1.9
  * @brief   STM32 内部 Flash 读写驱动程序.
  *          需要设定 Heap 至少为 FLASH_PAGE_SIZE+0x100.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "drv_flash.h"

#include <stdlib.h>
#include "stm32f10x_flash.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_rcc.h"



/** @addtogroup Drivers
  * @{
  */
/** @defgroup DRV_Flash
  * @{
  */
#if (FLASH_SIZE > 0)



/**
  ******************************************************************************
  * @addtogroup DRV_Flash_Configure
  ******************************************************************************
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_Flash_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Private_Function
  * @brief    定义私有函数
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_Flash_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  FLASH加锁, 禁止FLASH写入
  * @param  null
  * @retval null
  ******************************************************************************
  */
void flash_lock(void)
{
    FLASH_Lock();
}

/**
  ******************************************************************************
  * @brief  FLASH解锁, 使能FLASH写入, 使用flash_write前必须调用
  * @param  null
  * @retval null
  ******************************************************************************
  */
void flash_unlock(void)
{
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
}

/**
  ******************************************************************************
  * @brief  将一串数据写入内部 Flash, 完成后自动禁止写入功能
  * @param  pdata,   要写入数据的指针
  * @param  addr,    要写入数据的目标地址.
  * @param  num,     数据长度
  * @retval 返回值,  是否写入成功. 0,成功. 1,失败
  ******************************************************************************
  */
uint8_t flash_write(uint8_t *pdata, uint32_t addr, uint32_t num)
{
    uint8_t rval = 0;
    uint8_t *pbuf;
    uint32_t *pword;
    uint32_t i, addr_page, crc;

    // 芯片容量检测
    uint32_t flash_size;
    flash_size = (*(uint16_t *)0x1FFFF7E0);                                     // 读取FLASH容量寄存器
    flash_size <<= 10;                                                          // * 1024后为正确的容量值
    assert_param(flash_size >= FLASH_END_ADDRESS - FLASH_ADDRESS);
    if (flash_size < FLASH_END_ADDRESS - FLASH_ADDRESS) rval = 1;

    // 写入地址检测
    assert_param(addr >= FLASH_START_ADDRESS);
    assert_param(addr+num <= FLASH_END_ADDRESS);
    if (addr < FLASH_START_ADDRESS) rval = 1;
    if (addr+num > FLASH_END_ADDRESS) rval = 1;

    // 计算出所在页的地址
    addr_page = addr - addr % FLASH_PAGE_SIZE;
    pbuf = (uint8_t *)malloc(FLASH_PAGE_SIZE);
    if (pbuf == NULL) rval = 1;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
    while(rval == 0)
    {
        // 根据数据内容选择是否缓存FLASH
        for(i=0; i<FLASH_PAGE_SIZE; i++)
        {
            if ((addr_page < addr) || (addr_page >= addr + num))
                *(pbuf+i) = *(uint8_t *)addr_page;
            else
                *(pbuf+i) = *pdata++;
            addr_page++;
        }

        while(rval == 0)
        {
            addr_page -= FLASH_PAGE_SIZE;

            // 擦除
            if (FLASH_ErasePage(addr_page) != FLASH_COMPLETE)
            {
                RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,DISABLE);
                FLASH_Lock();
                free(pbuf);
                assert_printf("FLASH_ErasePage error@%lu\r\n", addr_page);
                rval = 1;
                break;
            }
            CRC_ResetDR();

            // 烧录
            pword = (uint32_t *)pbuf;
            for(i=0; i<FLASH_PAGE_SIZE; i+=4)
            {
                if (FLASH_ProgramWord(addr_page, *pword) != FLASH_COMPLETE)
                {
                    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,DISABLE);
                    FLASH_Lock();
                    free(pbuf);
                    assert_printf("FLASH_ProgramWord error@%lu\r\n", addr_page);
                    rval = 1;
                    break;
                }
                CRC_CalcCRC(*pword);
                addr_page += 4;
                pword++;
            }

            // 单页校验, 成功后烧写下一页, 否则重新烧写本页
            crc = CRC_GetCRC();
            CRC_ResetDR();
            pword = (uint32_t *)(addr_page-FLASH_PAGE_SIZE);
            if (crc == CRC_CalcBlockCRC(pword, FLASH_PAGE_SIZE >> 2))
                break;
        }

        // 所有数据烧录完成
        if (addr_page >= addr+num)
            break;
    }

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,DISABLE);
    FLASH_Lock();
    free(pbuf);

    return rval;
}

 /**
  ******************************************************************************
  * @brief  从 Flash 中读出一串数据.
  * @param  addr,   要读取的起始地址.
  * @param  pdata,  要读取数据的存放地址指针
  * @param  num,    需要读取的数据个数.
  * @retval 返回值, 是否读取成功. 0,成功. 1,失败
  ******************************************************************************
  */
uint8_t flash_read(uint8_t *pdata, uint32_t addr, uint32_t num)
{
    // 芯片容量检测
    uint32_t flash_size;
    flash_size = (*(uint16_t *)0x1FFFF7E0);                                     // 读取FLASH容量寄存器
    flash_size <<= 10;                                                          // * 1024后为正确的容量值
    assert_param(flash_size >= FLASH_END_ADDRESS - FLASH_ADDRESS);
    if (flash_size < FLASH_END_ADDRESS - FLASH_ADDRESS) return(1);

    // 读取地址检测
    assert_param(addr >= FLASH_START_ADDRESS);
    assert_param(addr+num <= FLASH_END_ADDRESS);
    if (addr < FLASH_START_ADDRESS) return(1);
    if (addr+num > FLASH_END_ADDRESS) return(1);

    for(; num>0; num--)
    {
        *pdata = *(uint8_t *)addr;
        pdata++;
        addr++;
    }
    return 0;
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
