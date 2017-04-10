/**
  ******************************************************************************
  * @file    sf_universal.c
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2011.12.23
  * @brief   SPI Flash 通用驱动. 使用标准的SPI通讯协议. 兼容大多数厂商的SPI FLASH.
  *          此驱动用于支持 drv_spiflash. 请勿直接调用.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sf_universal.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Drivers
  * @{
  */
/** @defgroup SF_Universal
  * @{
  */
#ifdef SPIFLASH_UNIVERSAL



/**
  ******************************************************************************
  * @addtogroup SF_Universal_Configure
  ******************************************************************************
  * @{
  */
#include "drv_systick.h"
#define sf_unvsl_delay_us(time)         delay_us(time)

/// SpiFlash SPI口初始化函数. 最高允许50MHz, 部分芯片仅允许33MHz. (READ_DATA 03H 的限制)
__INLINE static void sf_unvsl_spi_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    // SPI1 时钟源为72Mhz, 其它SPI 时钟源为36Mhz.
    spi_init(SPIFLASH_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, SPI_BaudRatePrescaler_2);

    // 使用模拟SPI.
//  spi_init(SPIFLASH_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, 0);

    // 必须先初始化SS引脚为软件控制, 再初始化CS引脚
    RCC_APB2PeriphClockCmd(SPIFLASH_CS_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = SPIFLASH_CS_PIN;
    GPIO_Init(SPIFLASH_CS_PORT, &GPIO_InitStructure);
}

/// SpiFlash 通用收发函数
__INLINE static uint8_t sf_unvsl_tx_rx_byte(uint8_t data)
{
    return spi_tx_rx_byte(SPIFLASH_SPI_NAME, data);
}

/// SpiFlash 片选信号引脚使能, 将IO口电平置低
__INLINE static void sf_unvsl_cs_enable(void)
{
    SPIFLASH_CS_PORT->BRR = SPIFLASH_CS_PIN;
}

/// SpiFlash 片选信号引脚禁止, 将IO口电平置高
__INLINE static void sf_unvsl_cs_disable(void)
{
    SPIFLASH_CS_PORT->BSRR = SPIFLASH_CS_PIN;
}

/**
  * @}
  */



/** @defgroup SpiFlash_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
// 指令宏定义
#define WRITE_ENABLE                    0x06
#define WRITE_DISABLE                   0x04
#define READ_STATUS_REG                 0x05
#define WRITE_STATUS_REG                0x01
#define READ_DATA                       0x03
#define PAGE_PROGRAM                    0x02
#define BLOCK_ERASE                     0xD8
#define SECTOR_EARSE                    0x20
#define CHIP_EARSE                      0xC7
#define POWER_DOWN                      0xB9
#define RELEASE_POWER_DOWN              0xAB
#define READ_ID                         0x9F

// 状态寄存器标记位宏定义
#define BUSY                            0x01

// 接收数据时, 要发送的无用数据
#define DUMMY_BYTE                      0xFF
/**
  * @}
  */

/** @defgroup SF_Universal_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup SF_Universal_Private_Function
  * @brief    定义私有函数
  * @{
  */
static void sf_unvsl_set_write_enable(void);
static void sf_unvsl_wait_write_finish(void);
/**
  * @}
  */



/** @defgroup SF_Universal_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  SpiFlash SPI通讯和相应的端口初始化函数
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_init(void)
{
    sf_unvsl_spi_init();
    sf_unvsl_cs_disable();
}

/**
  ******************************************************************************
  * @brief  设置芯片状态寄存器, 用于设置只读区域
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_set_protect(uint8_t status)
{
    sf_unvsl_set_write_enable();
    sf_unvsl_cs_enable();

    sf_unvsl_tx_rx_byte(WRITE_STATUS_REG & 0x3C);
    sf_unvsl_tx_rx_byte(status);

    sf_unvsl_cs_disable();
    sf_unvsl_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  芯片进入休眠模式, 耗电更少. 但无法进行读写操作.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_power_down(void)
{
    sf_unvsl_cs_enable();
    sf_unvsl_tx_rx_byte(POWER_DOWN);
    sf_unvsl_cs_disable();
    delay_us(500);                                                              // 等待进入睡眠模式
}

/**
  ******************************************************************************
  * @brief  芯片退出休眠模式. 退出后可进行读写操作
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_power_on(void)
{
    sf_unvsl_cs_enable();
    sf_unvsl_tx_rx_byte(RELEASE_POWER_DOWN);
    sf_unvsl_cs_disable();
    delay_us(500);                                                              // 等待进入普通模式
}

/**
  ******************************************************************************
  * @brief  读取芯片ID信息
  * @param  none
  * @retval 返回值, 芯片ID信息
  ******************************************************************************
  */
uint32_t sf_unvsl_read_id(void)
{
    uint32_t temp;
    sf_unvsl_cs_enable();

    sf_unvsl_tx_rx_byte(READ_ID);
    temp = sf_unvsl_tx_rx_byte(DUMMY_BYTE);
    temp <<= 8;
    temp |= sf_unvsl_tx_rx_byte(DUMMY_BYTE);
    temp <<= 8;
    temp |= sf_unvsl_tx_rx_byte(DUMMY_BYTE);

    sf_unvsl_cs_disable();
    return temp;
}

/**
  ******************************************************************************
  * @brief  擦除一个区(sector),大小为4K byte,擦除时间最长需要500ms
  * @param  addr, 字节地址. 擦除该地址所在的区内容
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_sector_erase(uint32_t addr)
{
    sf_unvsl_set_write_enable();
    sf_unvsl_cs_enable();

    sf_unvsl_tx_rx_byte(SECTOR_EARSE);                                          // 发送区擦除指令
    sf_unvsl_tx_rx_byte((addr & 0xFF0000) >> 16);                               // 发送要擦除的地址
    sf_unvsl_tx_rx_byte((addr & 0xFF00) >> 8);
    sf_unvsl_tx_rx_byte(addr & 0xFF);

    sf_unvsl_cs_disable();
    sf_unvsl_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  擦除一个块(block),大小为64K byte,擦除时间最长需要1s
  * @param  addr, 字节地址. 擦除该地址所在的块内容
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_block_erase(uint32_t addr)
{
    sf_unvsl_set_write_enable();
    sf_unvsl_cs_enable();

    sf_unvsl_tx_rx_byte(BLOCK_ERASE);                                           // 发送块擦除指令
    sf_unvsl_tx_rx_byte((addr & 0xFF0000) >> 16);                               // 发送要擦除的地址
    sf_unvsl_tx_rx_byte((addr & 0xFF00) >> 8);
    sf_unvsl_tx_rx_byte(addr & 0xFF);

    sf_unvsl_cs_disable();
    sf_unvsl_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  擦除芯片的所有内容(chip). 擦除时间和芯片大小有关.
  *         擦除时间为秒级别的. 需要自行延时等待其完成.
  *         延时时间5-100s不等. 8M-bit需要10s, 64M-bit需要100s
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_chip_erase(void)
{
    sf_unvsl_set_write_enable();
    sf_unvsl_cs_enable();

    sf_unvsl_tx_rx_byte(CHIP_EARSE);                                            // 发送块擦除指令

    sf_unvsl_cs_disable();
    sf_unvsl_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  进入读取模式
  * @param  addr, 读取数据的起始地址
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_read_start(uint32_t addr)
{
    sf_unvsl_cs_enable();

    sf_unvsl_tx_rx_byte(READ_DATA);
    sf_unvsl_tx_rx_byte((addr & 0xFF0000) >> 16);
    sf_unvsl_tx_rx_byte((addr& 0xFF00) >> 8);
    sf_unvsl_tx_rx_byte(addr & 0xFF);
}

/**
  ******************************************************************************
  * @brief  读取一串数据
  * @param  pbuf, 读取地址指针
  * @param  num,  读取数据的字节数
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_read_bytes(uint8_t *pbuf, uint32_t num)
{
    while(num--)
    {
        *pbuf++ = sf_unvsl_tx_rx_byte(DUMMY_BYTE);
    }
}

/**
  ******************************************************************************
  * @brief  退出读取模式
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_read_end(void)
{
    sf_unvsl_cs_disable();
}

/**
  ******************************************************************************
  * @brief  写一整页
  * @param  pbuf, 要写入的数据指针
  * @param  addr, 要写入的地址
  * @param  num,  要写入的字节数
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_write_page(uint8_t *pbuf, uint32_t addr, uint32_t num)
{
    sf_unvsl_set_write_enable();
    sf_unvsl_cs_enable();

    sf_unvsl_tx_rx_byte(PAGE_PROGRAM);
    sf_unvsl_tx_rx_byte((addr & 0xFF0000) >> 16);
    sf_unvsl_tx_rx_byte((addr & 0xFF00) >> 8);
    sf_unvsl_tx_rx_byte(addr & 0xFF);                                           // 此8位其实是无效的

    while (num--)
    {
        sf_unvsl_tx_rx_byte(*pbuf++);
    }

    sf_unvsl_cs_disable();
    sf_unvsl_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  允许写操作
  * @param  none
  * @retval 返回值. 1, 允许写入. 0, 禁止写入
  ******************************************************************************
  */
void sf_unvsl_set_write_enable(void)
{
    sf_unvsl_cs_enable();
    sf_unvsl_tx_rx_byte(WRITE_ENABLE);
    sf_unvsl_cs_disable();
}

/**
  ******************************************************************************
  * @brief  等待写操作结束
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_wait_write_finish(void)
{
    uint32_t i;

    sf_unvsl_cs_enable();
    sf_unvsl_tx_rx_byte(READ_STATUS_REG);

    for (i=0; i<0xFFFFF; i++)
    {
        if ((sf_unvsl_tx_rx_byte(DUMMY_BYTE) & BUSY) == 0) break;
        sf_unvsl_delay_us(1);
    }

    sf_unvsl_cs_disable();
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

