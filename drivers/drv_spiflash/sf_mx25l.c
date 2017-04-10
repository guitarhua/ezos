/**
  ******************************************************************************
  * @file    sf_mx25l.c
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2011.12.23
  * @brief   旺宏 MX25L系列 SPI Flash 驱动程序. 使用 4DTRD 模式读, 4PP模式写.
  *          此驱动用于支持 drv_spiflash. 请勿直接调用.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sf_mx25l.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Drivers
  * @{
  */
/** @defgroup SF_MX25L
  * @{
  */
#ifdef MX25L_4DTRD_4PP



/**
  ******************************************************************************
  * @addtogroup SF_MX25L_Configure
  ******************************************************************************
  * @{
  */
#include "drv_systick.h"
#define mx25l_delay_us(time)            delay_us(time)

/// IO口操作宏定义
#define MX25L_SCLK_HIGH()               MX25L_SCLK_PORT->BSRR = MX25L_SCLK_PIN
#define MX25L_SCLK_LOW()                MX25L_SCLK_PORT->BRR  = MX25L_SCLK_PIN

#define MX25L_SIO0_HIGH()               MX25L_SIO_PORT->BSRR = MX25L_SIO0_PIN
#define MX25L_SIO0_LOW()                MX25L_SIO_PORT->BRR  = MX25L_SIO0_PIN

#define MX25L_SIO1_HIGH()               MX25L_SIO_PORT->BSRR = MX25L_SIO1_PIN
#define MX25L_SIO1_LEVEL()              (MX25L_SIO_PORT->IDR & MX25L_SIO1_PIN)

#define MX25L_SIO_IDR_GET()             (MX25L_SIO_PORT->IDR)
#define MX25L_SIO_ODR_GET()             (MX25L_SIO_PORT->ODR)
#define MX25L_SIO_ODR_SET(val)          MX25L_SIO_PORT->ODR = val;

/// MX25L SPI初始化函数. 最高允许50MHz
__INLINE static void mx25l_port_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( MX25L_CS_CLK | MX25L_SCLK_CLK | MX25L_SIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = MX25L_CS_PIN;
    GPIO_Init(MX25L_CS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = MX25L_SCLK_PIN;
    GPIO_Init(MX25L_SCLK_PORT, &GPIO_InitStructure);

    // SIO0, 配置为推挽输出模式
    GPIO_InitStructure.GPIO_Pin   = MX25L_SIO0_PIN;
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);

    // SIO1-3, 配置为输入模式
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = (MX25L_SIO1_PIN | MX25L_SIO2_PIN | MX25L_SIO3_PIN);
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);
}

/// MX25L 通用收发函数, 最高频率 50MHz
__INLINE static uint8_t mx25l_tx_rx_byte(uint8_t data)
{
    uint8_t i, rec = 0;

    for (i=8; i>0; i--)
    {
        if (data & 0x80) MX25L_SIO0_HIGH();
        else             MX25L_SIO0_LOW();
        data <<= 1;
        MX25L_SCLK_HIGH();
        rec <<= 1;
        if (MX25L_SIO1_LEVEL()) rec |= 0x01;
        MX25L_SCLK_LOW();
    }

    return rec;
}

/// MX25L 4PP模式发送数据, 最高频率 20MHz
__INLINE static void mx25l_4PP_tx_byte(uint8_t data)
{
    uint16_t port_val, temp;

    temp = (data >> 4) & 0x000F;                                                // 取出数据高4bit
    port_val = MX25L_SIO_ODR_GET() & 0xFFF0;                                    // 确保低4bit为0
    MX25L_SIO_ODR_SET(port_val | temp);                                         // 输出数据
    MX25L_SCLK_HIGH();
    __NOP(); __NOP(); __NOP(); __NOP();                                         // 适当的延时
    MX25L_SCLK_LOW();
    temp = data & 0x000F;                                                       // 取出数据低4bit
    port_val = MX25L_SIO_ODR_GET() & 0xFFF0;                                    // 确保低4bit为0
    MX25L_SIO_ODR_SET(port_val | temp);
    MX25L_SCLK_HIGH();
    __NOP(); __NOP(); __NOP(); __NOP();                                         // 适当的延时
    MX25L_SCLK_LOW();
}

/// MX25L 4DTRD模式发送数据, 最高频率 50MHz
__INLINE static void mx25l_4DTRD_tx_byte(uint8_t data)
{
    volatile uint16_t port_val, temp;

    temp = (data >> 4) & 0x000F;                                                // 取出数据高4bit
    port_val = MX25L_SIO_ODR_GET() & 0xFFF0;                                    // 确保低4bit为0
    MX25L_SIO_ODR_SET(port_val | temp);                                         // 输出数据

    MX25L_SCLK_HIGH();

    temp = data & 0x000F;                                                       // 取出数据低4bit
    port_val = MX25L_SIO_ODR_GET() & 0xFFF0;                                    // 确保低4bit为0
    MX25L_SIO_ODR_SET(port_val | temp);

    MX25L_SCLK_LOW();
}

/// MX25L 4DTRD模式接收数据, 最高频率 50MHz
__INLINE static uint8_t mx25l_4DTRD_rx_byte()
{
    static volatile uint8_t rec;                                                // 防止高优化等级出错

    rec = MX25L_SIO_IDR_GET();
    MX25L_SCLK_HIGH();
    rec <<= 4;
    rec |= (MX25L_SIO_IDR_GET() & 0x000F);
    MX25L_SCLK_LOW();

    return rec;
}

/// MX25L 片选信号引脚使能, 将IO口电平置低
__INLINE static void mx25l_cs_enable(void)
{
    MX25L_CS_PORT->BRR = MX25L_CS_PIN;
}

/// MX25L 片选信号引脚禁止, 将IO口电平置高
__INLINE static void mx25l_cs_disable(void)
{
    MX25L_CS_PORT->BSRR = MX25L_CS_PIN;
}

/**
  * @}
  */



/** @defgroup SF_MX25L_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
// 指令宏定义
#define WREN                            0x06
#define WRDI                            0x04
#define RDID                            0x9F
#define RDSR                            0x05
#define WRSR                            0x01
#define READ                            0x03
#define SE                              0x20
#define BE                              0xD8
#define CE                              0xC7
#define PP                              0x02
#define DP                              0xB9
#define RDP                             0xAB
#define _4DTRD                          0xED
#define _4PP                            0x38

// 状态寄存器标记位宏定义
#define QE                              0x40
#define WIP                             0x01

// 接收数据时, 要发送的无用数据
#define DUMMY_BYTE                      0xFF
/**
  * @}
  */

/** @defgroup SF_MX25L_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */

/**
  * @}
  */

/** @defgroup SF_MX25L_Private_Function
  * @brief    定义私有函数
  * @{
  */
static void mx25l_set_write_enable(void);
static void mx25l_wait_write_finish(void);
/**
  * @}
  */



/** @defgroup SF_MX25L_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  MX25L SPI通讯和相应的端口初始化函数
  * @param  none
  * @retval none
  ******************************************************************************
  */
void mx25l_init(void)
{
    mx25l_port_init();

    mx25l_cs_disable();
    MX25L_SCLK_LOW();
    MX25L_SIO0_HIGH();

    mx25l_set_protect(MX25L_PROTECT_NONE);                                      // 设置为4线模式, 允许读写
}

/**
  ******************************************************************************
  * @brief  设置芯片状态寄存器, 用于设置只读区域
  * @param  none
  * @retval none
  ******************************************************************************
  */
void mx25l_set_protect(uint8_t status)
{
    mx25l_set_write_enable();
    mx25l_cs_enable();

    mx25l_tx_rx_byte(WRSR);
    mx25l_tx_rx_byte((status & 0x3C) | QE);

    mx25l_cs_disable();
    mx25l_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  芯片进入休眠模式, 耗电更少. 但无法进行读写操作.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void mx25l_power_down(void)
{
    mx25l_cs_enable();
    mx25l_tx_rx_byte(DP);
    mx25l_cs_disable();
    mx25l_delay_us(15);                                                         // 至少延时10us, 等待进入睡眠模式
}

/**
  ******************************************************************************
  * @brief  芯片退出休眠模式. 退出后可进行读写操作
  * @param  none
  * @retval none
  ******************************************************************************
  */
void mx25l_power_on(void)
{
    mx25l_cs_enable();
    mx25l_tx_rx_byte(RDP);
    mx25l_cs_disable();
    mx25l_delay_us(150);                                                        // 至少延时100us, 等待进入普通模式
}

/**
  ******************************************************************************
  * @brief  读取芯片ID信息
  * @param  none
  * @retval 返回值, 芯片ID信息
  ******************************************************************************
  */
uint32_t mx25l_read_id(void)
{
    uint32_t temp;
    mx25l_cs_enable();

    mx25l_tx_rx_byte(RDID);
    temp = mx25l_tx_rx_byte(DUMMY_BYTE);
    temp <<= 8;
    temp |= mx25l_tx_rx_byte(DUMMY_BYTE);
    temp <<= 8;
    temp |= mx25l_tx_rx_byte(DUMMY_BYTE);

    mx25l_cs_disable();
    return temp;
}

/**
  ******************************************************************************
  * @brief  擦除一个区(sector),大小为4K byte,擦除时间最长需要500ms
  * @param  addr, 字节地址. 擦除该地址所在的区内容
  * @retval none
  ******************************************************************************
  */
void mx25l_sector_erase(uint32_t addr)
{
    mx25l_set_write_enable();
    mx25l_cs_enable();

    mx25l_tx_rx_byte(SE);                                                       // 发送区擦除指令
    mx25l_tx_rx_byte((addr & 0xFF0000) >> 16);                                  // 发送要擦除的地址
    mx25l_tx_rx_byte((addr & 0xFF00) >> 8);
    mx25l_tx_rx_byte(addr & 0xFF);

    mx25l_cs_disable();
    mx25l_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  擦除一个块(block),大小为64K byte,擦除时间最长需要1s
  * @param  addr, 字节地址. 擦除该地址所在的块内容
  * @retval none
  ******************************************************************************
  */
void mx25l_block_erase(uint32_t addr)
{
    mx25l_set_write_enable();
    mx25l_cs_enable();

    mx25l_tx_rx_byte(BE);                                                       // 发送块擦除指令
    mx25l_tx_rx_byte((addr & 0xFF0000) >> 16);                                  // 发送要擦除的地址
    mx25l_tx_rx_byte((addr & 0xFF00) >> 8);
    mx25l_tx_rx_byte(addr & 0xFF);

    mx25l_cs_disable();
    mx25l_wait_write_finish();
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
void mx25l_chip_erase(void)
{
    mx25l_set_write_enable();
    mx25l_cs_enable();

    mx25l_tx_rx_byte(CE);                                                       // 发送块擦除指令

    mx25l_cs_disable();
    mx25l_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  进入读取模式
  * @param  addr, 读取数据的起始地址
  * @retval none
  ******************************************************************************
  */
void mx25l_read_start(uint32_t addr)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    mx25l_cs_enable();

    mx25l_tx_rx_byte(_4DTRD);

    // 将SIO0-3配置为推挽输出模式
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = (MX25L_SIO0_PIN | MX25L_SIO1_PIN | MX25L_SIO2_PIN | MX25L_SIO3_PIN);
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);

    mx25l_4DTRD_tx_byte((addr & 0xFF0000) >> 16);                               // 发送Address
    mx25l_4DTRD_tx_byte((addr & 0xFF00) >> 8);
    mx25l_4DTRD_tx_byte( addr & 0xFF);

    // 必须将IO口置为高, 等待输入信号. 普通模式, 8个Dummy cycles
    mx25l_4DTRD_tx_byte(0xFF);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);

    // 将SIO0-3配置为输入模式, 准备接受数据
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);
}

/**
  ******************************************************************************
  * @brief  读取一串数据
  * @param  pbuf, 读取地址指针
  * @param  num,  读取数据的字节数
  * @retval none
  ******************************************************************************
  */
void mx25l_read_bytes(uint8_t *pbuf, uint32_t num)
{
    while(num--)
    {
        *pbuf = mx25l_4DTRD_rx_byte();
        ++pbuf;
    }
}

/**
  ******************************************************************************
  * @brief  退出读取模式
  * @param  none
  * @retval none
  ******************************************************************************
  */
void mx25l_read_end(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    mx25l_cs_disable();

    // 将SIO0恢复为推挽输出模式
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = MX25L_SIO0_PIN;
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);

    // 将SIO1-3恢复为输入模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin  = (MX25L_SIO1_PIN | MX25L_SIO2_PIN | MX25L_SIO3_PIN);
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);
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
void mx25l_write_page(uint8_t *pbuf, uint32_t addr, uint32_t num)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    mx25l_set_write_enable();
    mx25l_cs_enable();

    mx25l_tx_rx_byte(_4PP);

    // 将SIO0-3配置为推挽输出模式
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = (MX25L_SIO0_PIN | MX25L_SIO1_PIN | MX25L_SIO2_PIN | MX25L_SIO3_PIN);
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);

    mx25l_4PP_tx_byte((addr & 0xFF0000) >> 16);
    mx25l_4PP_tx_byte((addr & 0xFF00) >> 8);
    mx25l_4PP_tx_byte(addr & 0xFF);

    while (num--)
    {
        mx25l_4PP_tx_byte(*pbuf);
        ++pbuf;
    }

    mx25l_cs_disable();

    // 将SIO1-3恢复为输入模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin  = (MX25L_SIO1_PIN | MX25L_SIO2_PIN | MX25L_SIO3_PIN);
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);

    mx25l_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  允许写操作
  * @param  none
  * @retval 返回值. 1, 允许写入. 0, 禁止写入
  ******************************************************************************
  */
void mx25l_set_write_enable(void)
{
    mx25l_cs_enable();
    mx25l_tx_rx_byte(WREN);
    mx25l_cs_disable();
}

/**
  ******************************************************************************
  * @brief  等待写操作结束
  * @param  none
  * @retval none
  ******************************************************************************
  */
void mx25l_wait_write_finish(void)
{
    uint32_t i;

    mx25l_cs_enable();
    mx25l_tx_rx_byte(RDSR);

    for (i=0; i<0xFFFFF; i++)
    {
        if ((mx25l_tx_rx_byte(DUMMY_BYTE) & WIP) == 0) break;
        mx25l_delay_us(1);
    }

    mx25l_cs_disable();
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

