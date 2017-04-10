/**
  ******************************************************************************
  * @file    drv_spiflash.h
  * @author  mousie-yu
  * @version V2.0.1
  * @date    2013.2.10
  * @brief   SPI Flash 通用驱动程序. 需要各品牌的SPI FLASH底层驱动支持.
  *          根据不同的底层驱动, 可支持标准SPI模式, 模拟快速模式, 4IO模式.
  *          v2.0.1, 修正一个跨区读取时, 数据错误的严重bug.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_SPIFLASH_H
#define __DRV_SPIFLASH_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>



/** @addtogroup Drivers
  * @{
  */
/** @addtogroup DRV_SpiFlash
  * @{
  */



/**
  ******************************************************************************
  * @defgroup DRV_SpiFlash_Configure
  * @brief    用户配置
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // 硬件集中配置文件

//#define SPIFLASH_MBIT                   16                                      ///< 选择SPI FLASH芯片大小单位为(M bit), 不使用可定义为0或不定义

#include "sf_mx25l.h"
#include "sf_universal.h"

#if ((defined SPIFLASH_UNIVERSAL) && (defined MX25L_4DTRD_4PP))
    #error "SPIFLASH_MODE define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_SpiFlash_Public_TypeDefine
  * @brief    公有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SpiFlash_Public_MacroDefine
  * @brief    公有宏定义
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SpiFlash_Public_Variable
  * @brief    声明公有全局变量
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SpiFlash_Public_Function
  * @brief    定义公有函数
  * @{
  */
#if (SPIFLASH_MBIT > 0)

void spiflash_init(void);                                                       // 通讯口初始化
/**
  ******************************************************************************
  * 设置写保护区域
  * 每块芯片可保护区域都不一样, 如有需要可参考数据手册自行定义
  * status需要调用不同芯片的特定宏定义.
  ******************************************************************************
  */
void spiflash_set_protect(uint8_t status);
void spiflash_power_down(void);                                                 // 芯片进入休眠模式
void spiflash_power_on(void);                                                   // 芯片进入退出休眠模式
uint32_t spiflash_read_id(void);                                                // 读取ID信息

uint8_t spiflash_sector_erase(uint32_t addr);                                   // 擦除一个区, 大小为4K byte
uint8_t spiflash_block_erase(uint32_t addr);                                    // 擦除一个块, 大小为64K byte
//void spiflash_chip_erase(void);                                               // 不建议使用. 擦除整个芯片内容, 需要自行延时等待其完成擦除.

// 以下函数直接调用即可. 写入前不需要预先调用擦除函数.
uint8_t spiflash_read(uint8_t *pbuf, uint32_t read_addr, uint32_t num);         // 从spi flash中读取数据
uint8_t spiflash_write_buf(uint8_t *pbuf, uint32_t write_addr, uint32_t num);   // 写入数据, 未满一个区的数据暂存在buf中
void spiflash_write_buf2flash(void);                                            // 将buf中的数据写入到flash中

/**
  ******************************************************************************
  * 以下函数为连续读取函数.
  * 注意连续读取函数直接从 SPI FLASH 中读取数据的, 不经过 buffer 区.
  * 建议用于大规模读取数据时使用, 如数据缓存区只有512字节, 需要读取10K的数据时, 可使用.
  *
  * 调用方法例程如下:
  *
  * // 从0地址开始连续读取数据. 期间不能对SPI FLASH有其它操作.
  * spiflash_read_start(0);
  * // 以512个字节为一组, 连续读取数据. 此步骤可以循环无数次,
  * spiflash_read_bytes(data, 512);     // 读取512个数据到data中
  * do_something(data);                 // 数据处理
  * spiflash_read_bytes(data, 512);     // 再读取512个数据到data中, 依次循环
  * do_something(data);                 // 数据处理
  * // 结束连续读取数据. 可对SPI FLASH进行其它操作.
  * spiflash_read_end();
  ******************************************************************************
  */
void spiflash_read_start(uint32_t read_addr);                                   // 准备连续读取
void spiflash_read_bytes(uint8_t *pbuf, uint32_t num);                          // 连续读取数据
void spiflash_read_end(void);                                                   // 结束连续读取

#endif
/**
  * @}
  */



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
