/**
  ******************************************************************************
  * @file    drv_sdcard.c
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   SD卡驱动程序, 支持 SD_V1.x, SD_V2, SDHC卡.
  *          支持SPI模式 和 SDIO模式, 需要 drv_sdspi 或 drv_sdio 驱动.
  *          由于地址使用的是 uint32_t 格式, 因此最大可支持到4G的卡.
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
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDCARD_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
/// SDCARD_BLOCK_SIZE 只支持 512 byte, 否则会导致读写错误. 原因不明.
#define SDCARD_BLOCK_SIZE                   512
/**
  * @}
  */

/** @defgroup DRV_SDCARD_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SDCARD_Private_Function
  * @brief    定义私有函数
  * @{
  */
/// SD卡电源控制引脚初始化
__INLINE static void sdcard_power_port_init(void)
{
#ifdef SDCARD_POWER_NAME
    ctrlport_init(SDCARD_POWER_NAME);
#endif
}

/// 打开SD卡电源
__INLINE static void sdcard_power_port_on(void)
{
#ifdef SDCARD_POWER_NAME
    ctrlport_on(SDCARD_POWER_NAME);
#endif
}

/// 关闭SD卡电源
__INLINE static void sdcard_power_port_off()
{
#ifdef SDCARD_MODE_SDIO
    SD_PowerOFF();
#endif
#ifdef SDCARD_POWER_NAME
    ctrlport_off(SDCARD_POWER_NAME);
#endif
}

/// SD 存在检测信号初始化, IO口设置为上拉输入
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

/// 获取SD卡检测IO口当前电平值. 0, 低电平. !0, 高电平
__INLINE static uint32_t sdcard_detect_port_level(void)
{
#ifdef SDCARD_DETECT_ENABLE
    return (SDCARD_DETECT_PORT->IDR & SDCARD_DETECT_PIN);
#else
    return 0;                                                                   // 始终认为存在
#endif
}
/**
  * @}
  */



/** @defgroup DRV_SDCARD_Function
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
void sdcard_port_init(void)
{
    sdcard_power_port_init();                                                   // 初始化电源控制口
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
  * @brief  打开SD卡的电源
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
  * @brief  关闭SD卡的电源
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
  * @brief  打开SD卡电源并初始化
  * @param  none
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdcard_init(void)
{
    sdcard_power_port_on();                                                     // 打开SD卡电源
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();                                // 给一定的延时
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    if (!sdcard_present()) return 1;                                            // SD卡不存在, 直接退出
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
 * @brief  检测SD卡是否存在
 * @param  none
 * @retval 返回值, 0, 不存在. 1, 存在
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
  * @brief  读取SD卡信息
  * @param  info,   SD卡信息指针
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdcard_get_info(sdcard_info_t *info)
{
#if (defined SDCARD_MODE_SDIO)

    SD_CardInfo cardinfo;

    if (SD_GetCardInfo(&cardinfo) == SD_OK)
    {
        switch (cardinfo.CardType)                                              // SD卡类型
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
        switch (cardinfo.sd_type)                                               // SD卡类型
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
        switch (cardinfo.sd_type)                                               // SD卡类型
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
  * @brief  读取SD卡的块,
  * @param  pbuf,   读取地址指针
  * @param  block,  块起始地址, 非物理地址. 块大小默认为512byte
  * @param  block_num,  需要读取的SD块数量
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdcard_read_blocks(uint8_t* pbuf, uint32_t block, uint32_t block_num)
{
    if (block_num > 1)                                                          // 使用多块读写
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
    else if (block_num == 1)                                                    // 使用单块读写
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
  * @brief  写入SD卡的块
  * @param  pbuf,   写入地址指针
  * @param  block,  块起始地址, 非物理地址. 块大小默认为512byte
  * @param  block_num,  需要写入的SD块数量
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdcard_write_blocks(const uint8_t* pbuf, uint32_t block, uint32_t block_num)
{
    if (block_num > 1)                                                          // 使用多块读写
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
    else if (block_num == 1)                                                    // 使用单块读写
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
