/**
  ******************************************************************************
  * @file    sd_sdspi.c
  * @author  mousie-yu
  * @version V2.1.0
  * @date    2015.2.13
  * @brief   SD卡驱动程序, 使用SPI模式.
  *          支持MMC卡(未测), SD卡, SDHC卡. 最大支持4G的卡.
  *          此驱动用于支持 drv_sdcard 及 drv_sdflash. 请勿直接调用.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sd_sdspi.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Drivers
  * @{
  */
/** @defgroup SD_SDSPI
  * @{
  */
#if (defined SDCARD_MODE_SPI) || (defined SDFLASH_MODE_SPI)



/**
  ******************************************************************************
  * @addtogroup SD_SDSPI_Configure
  ******************************************************************************
  * @{
  */
#include "drv_systick.h"
#define sd_delay_1ms()                  delay_ms(1)                             ///< 设置SD 1ms延时函数

/// SD卡SPI初始化函数, 低速率. 最高允许400KHz, 读写SD卡指令用.
__INLINE static void sd_spi_init_low_speed(void)
{
    // 使用硬件SPI
    spi_init(SD_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, SPI_BaudRatePrescaler_256);

    // 使用模拟SPI
//  spi_init(SD_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, 400);
}

/// SD卡SPI初始化函数, 高速率. 最高允许25MHz, 读写SD卡数据用.
__INLINE static void sd_spi_init_high_speed(void)
{
    // 使用硬件   SPI1 时钟源为72Mhz, 需要使用 SPI_BaudRatePrescaler_4.
    // 使用硬件其它SPI 时钟源为36Mhz, 可使用 SPI_BaudRatePrescaler_2.
    spi_init(SD_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, SPI_BaudRatePrescaler_4);

    // 使用模拟SPI.
//  spi_init(SD_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, 0);
}

/// SD卡SPI收发函数宏定义
__INLINE static uint8_t sd_spi_tx_rx_byte(uint8_t data)
{
    return spi_tx_rx_byte(SD_SPI_NAME, data);
}

/// SD 片选信号引脚初始化, IO口设置为推挽输出.
__INLINE static void sd_cs_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(SD_CS_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = SD_CS_PIN;
    GPIO_Init(SD_CS_PORT, &GPIO_InitStructure);
}

/// SD 片选信号引脚使能, 将IO口电平置低
__INLINE static void sd_cs_enable(void)
{
    GPIO_ResetBits(SD_CS_PORT, SD_CS_PIN);
}

/// SD 片选信号引脚禁止, 将IO口电平置高
__INLINE static void sd_cs_disable(void)
{
    GPIO_SetBits(SD_CS_PORT, SD_CS_PIN);
}
/**
  * @}
  */



/** @defgroup SD_SDSPI_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup SD_SDSPI_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
#define CMD0                            0                                       ///< 命令0 , 卡复位
#define CMD1                            1                                       ///< 命令1 , CMD_SEND_OP_COND
#define CMD8                            8                                       ///< 命令8 , CMD_SEND_IF_COND
#define CMD9                            9                                       ///< 命令9 , 读CSD数据
#define CMD10                           10                                      ///< 命令10, 读CID数据
#define CMD12                           12                                      ///< 命令12, 停止数据传输
#define CMD16                           16                                      ///< 命令16, 设置SectorSize 应返回0x00
#define CMD17                           17                                      ///< 命令17, 读sector
#define CMD18                           18                                      ///< 命令18, 读Multi sector
#define ACMD23                          23                                      ///< 命令23, 设置多sector写入前预先擦除N个block
#define CMD24                           24                                      ///< 命令24, 写sector
#define CMD25                           25                                      ///< 命令25, 写Multi sector
#define ACMD41                          41                                      ///< 命令41, 应返回0x00
#define CMD55                           55                                      ///< 命令55, 应返回0x01
#define CMD58                           58                                      ///< 命令58, 读OCR信息
/**
  * @}
  */

/** @defgroup SD_SDSPI_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
static sd_type_t sd_type = UNKNOW;                                              ///< SD卡类型
static uint16_t  sd_block_size = 512;                                           ///< SD卡块大小
static uint8_t   sd_power = 9;                                                  ///< SD卡块位移量
/**
  * @}
  */

/** @defgroup SD_SDSPI_Private_Function
  * @brief    定义私有函数
  * @{
  */
static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg);                          // SD指令发送函数
/**
  * @}
  */



/** @defgroup SD_SDSPI_Function
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
void sdspi_port_init(void)
{
    sd_spi_init_low_speed();                                                    // 必须先初始化SS引脚为软件控制
    sd_cs_init();                                                               // 再初始化SS的GPIO口才有效
    sd_cs_disable();
}

/**
  ******************************************************************************
  * @brief  打开SD卡电源并初始化
  * @param  none
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdspi_init(uint16_t block_size)
{
    uint16_t i;
    uint8_t response;
    uint8_t rvalue = 1;

    sd_type = UNKNOW;

    if (((block_size % 512) != 0) || (block_size > 2048))
        return rvalue;

    sd_block_size = block_size;
    sd_power      = 8 + (block_size / 512);

    sd_spi_init_low_speed();                                                    // 使能SPI, 设为低速模式可提高SD卡识别率
    for (i=10; i>0; i--) sd_spi_tx_rx_byte(0xFF);                               // 先产生>74个脉冲，让SD卡自己初始化完成

    sd_cs_enable();                                                             // SD卡片选信号使能
    for (i=500; i>0; i--)                                                       // 实测300ms以下
    {
        sd_delay_1ms();
        if (sd_send_cmd(CMD0, 0) == 0x01) break;                                // CMD0, 让SD卡进入空闲模式
    }

    if (i)                                                                      // SD卡成功进入空闲模式
    {
        // CMD8指令响应是识别SD1.x和SD2.0规范的关键
        response = sd_send_cmd(8, 0x1AA);
        switch (response)
        {
        case 0x05: {                                                            // CMD8应答0x05, 即不支持CMD8指令, 是SD1.x或者MMC卡
            sd_spi_tx_rx_byte(0xFF);                                            // 发8个CLK, 让SD卡结束操作

            // 发送CMD55和ACMD41指令, 用于检测是SD卡还是MMC卡, MMC卡对ACMD41是没有应答的
            for (i=500; i>0; i--)
            {
                sd_delay_1ms();
                response = sd_send_cmd(CMD55, 0);
                if (response != 0x01) goto EXIT;                                // 应答错误, 直接退出
                response = sd_send_cmd(ACMD41, 0);
                if (response == 0x00) break;                                    // 正确应答, 跳出循环
            }
            if (i)
            {
                sd_type = SD_V1;                                                // 正确应答, 是SD卡V1.x
                rvalue = 0;                                                     // SD卡初始化成功标记
            }
            else                                                                // 无应答, 可能是MMC卡
            {
                for (i=500; i>0; i--)
                {
                    sd_delay_1ms();
                    response = sd_send_cmd(CMD1, 0);
                    if (response == 0x00) break;
                }
                if (i)                                                          // MMC卡初始化成功
                {
                    sd_type = MMC;                                              // 是MMC卡
                    rvalue = 0;                                                 // SD卡初始化成功标记
                }
            }

            if(sd_send_cmd(CMD16, sd_block_size) != 0x00)                       // 设置SD卡块大小
            {
                sd_type = UNKNOW;
                rvalue = 1;
            }
            break; }

        case 0x01: {                                                            // 应答0x01, 是V2.0的SD卡
            sd_spi_tx_rx_byte(0xFF);                                            // V2.0的卡在CMD8命令后会传回4字节的应答数据
            sd_spi_tx_rx_byte(0xFF);
            i   = sd_spi_tx_rx_byte(0xFF);
            i <<= 8;
            i  |= sd_spi_tx_rx_byte(0xFF);                                      // 4个字节应该是0x000001AA, 表示该卡是否支持2.7V-3.6V的电压范围

            if(i == 0x1AA)                                                      // SD卡支持2.7V-3.6V, 可以操作
            {
                for (i=500; i>0; i--)
                {
                    sd_delay_1ms();
                    response = sd_send_cmd(CMD55, 0);
                    if (response != 0x01) goto EXIT;                            // 应答错误, 直接退出
                    response = sd_send_cmd(ACMD41, 0x40000000);                 // SD V2.0要使用 0x40000000
                    if (response == 0x00) break;                                // 正确应答, 跳出循环
                }
                if (i)                                                          // CMD41正确应答
                {
                    // 通过获取OCR信息, 识别是普通的 SD V2.0 还是 SDHC 卡
                    if (sd_send_cmd(CMD58, 0) == 0x00)
                    {
                        i = sd_spi_tx_rx_byte(0xFF);                            // 应答的4字节OCR信息
                        sd_spi_tx_rx_byte(0xFF);
                        sd_spi_tx_rx_byte(0xFF);
                        sd_spi_tx_rx_byte(0xFF);

                        if (i & 0x40) sd_type = SDHC;                           // 通过检测CCS位确定SD卡类型
                        else          sd_type = SD_V2;
                        rvalue = 0;                                             // SD卡初始化成功标记
                    }
                }
                if(sd_send_cmd(CMD16, sd_block_size) != 0x00)                   // 设置SD卡块大小
                {
                    sd_type = UNKNOW;
                    rvalue = 1;
                }
            }
            break; }
        }
    }

EXIT:
    sd_cs_disable();
    sd_spi_tx_rx_byte(0xFF);                                                    // 8个CLK, 让SD结束后续操作
    sd_spi_init_high_speed();

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  读取SD卡信息
  * @param  info,   SD卡信息指针
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdspi_get_info(sd_info_t *info)
{
    uint8_t rvalue = 1;
    uint8_t data;
    uint8_t csd_read_bl_len = 0;
    uint8_t csd_c_size_mult = 0;
    uint32_t csd_c_size = 0;
    uint16_t i;

    if (sd_type == UNKNOW) return rvalue;                                       // 空指针 或 sd卡未初始化, 直接返回

    sd_cs_enable();
    if (sd_send_cmd(CMD10, 0) == 0x00)                                          // 读取CID寄存器
    {
        for (i=500; i>0; i--)                                                   // 实测400ms以下
        {
            sd_delay_1ms();
            if (sd_spi_tx_rx_byte(0xFF) == 0xFE) break;                         // 准备好传输数据
        }
        if (i)
        {
          rvalue = 0;                                                           // 读取CID寄存器成功
          for (i=0; i<18; i++)                                                  // 16个数据, 2个CRC校验
          {
                data = sd_spi_tx_rx_byte(0xFF);                                 // 接收CID数据
                switch(i)
                {
                case 0:
                    info->sd_manu = data;
                    break;
                case 1:
                case 2:
                    info->sd_oem[i - 1] = data;
                    break;
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    info->sd_product[i - 3] = data;
                    break;
                case 8:
                    info->sd_rev = data;
                    break;
                case 9:
                case 10:
                case 11:
                case 12:
                    info->sd_sn |= (uint32_t)data << ((12 - i) * 8);
                    break;
                case 13:
                    info->sd_year = data << 4;
                    break;
                case 14:
                    info->sd_year |= data >> 4;
                    info->sd_mon = data & 0x0f;
                    break;
                }
          }
        }
    }
    if (rvalue) return rvalue;

    rvalue = 1;
    if (sd_send_cmd(CMD9, 0) == 0x00)                                           // 读取CSD寄存器
    {
        for (i=500; i>0; i--)
        {
            sd_delay_1ms();
            if (sd_spi_tx_rx_byte(0xFF) == 0xFE) break;                         // 准备好传输数据
        }
        if (i)
        {
            rvalue = 0;                                                         // 读取CID寄存器成功
            for (i=0; i<18; i++)                                                // 16个数据, 2个CRC校验
            {
                data = sd_spi_tx_rx_byte(0xFF);

                if (sd_type == SDHC)                                            // SDHC卡
                {
                    switch(i)
                    {
                    case 7:
                        csd_c_size = (data & 0x3f);
                        csd_c_size <<= 8;
                        break;
                    case 8:
                        csd_c_size |= data;
                        csd_c_size <<= 8;
                        break;
                    case 9:
                        csd_c_size |= data;
                        csd_c_size++;
                        info->sd_cap = csd_c_size;
                        info->sd_cap <<= 19;                                    // *= (512 * 1024)
                        break;
                    }
                }
                else                                                            // 普通SD卡 或 MMC 卡
                {
                    switch(i)
                    {
                    case 5:
                        csd_read_bl_len = data & 0x0f;
                        break;
                    case 6:
                        csd_c_size = data & 0x03;
                        csd_c_size <<= 8;
                        break;
                    case 7:
                        csd_c_size |= data;
                        csd_c_size <<= 2;
                        break;
                    case 8:
                        csd_c_size |= data >> 6;
                        ++csd_c_size;
                        break;
                    case 9:
                        csd_c_size_mult = data & 0x03;
                        csd_c_size_mult <<= 1;
                        break;
                    case 10:
                        csd_c_size_mult |= data >> 7;
                        info->sd_cap = csd_c_size;
                        info->sd_cap <<= (csd_c_size_mult + csd_read_bl_len + 2);
                        break;
                    }
                }
            }
        }
    }

    info->sd_type = sd_type;
    return rvalue;
}

/**
  ******************************************************************************
  * @brief  读取SD卡的一个块,
  * @param  pbuf,   数据存储指针
  * @param  sector, 读取块内容, 非物理地址.
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdspi_read_block(uint8_t* pbuf, uint32_t sector)
{
    uint16_t i;
    uint8_t  response;
    uint8_t  rvalue = 1;

    if (sd_type == UNKNOW) return rvalue;
    if (sd_type != SDHC) sector <<= sd_power;                                   // 不是SDHC, 需要将块地址转为字节地址, sector *= sd_block_size

    sd_cs_enable();
    response = sd_send_cmd(CMD17, sector);                                      // 发送 CMD17, 读单个块
    if (response == 0x00)                                                       // 应答正确
    {
        for (i=20000; i>0; i--)                                                 // 实测5ms以内
        {
            if (sd_spi_tx_rx_byte(0xFF) == 0xFE) break;                         // 准备好传输数据
        }
        if (i)
        {
            rvalue = 0;
            for (i=sd_block_size; i>0; i--)
                *pbuf++ = sd_spi_tx_rx_byte(0xFF);                              // 读一个块
            sd_spi_tx_rx_byte(0xFF);                                            // 读CRC校验
            sd_spi_tx_rx_byte(0xFF);
        }
    }
    sd_cs_disable();
    sd_spi_tx_rx_byte(0xFF);

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  读取SD卡的多个块,
  * @param  pbuf,   读取地址指针
  * @param  sector, 读取块内容, 非物理地址.
  * @param  block_num,  需要读取的SD块数量
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdspi_read_multi_blocks(uint8_t* pbuf, uint32_t sector, uint32_t block_num)
{
    uint32_t i;
    uint8_t  response;
    uint8_t  rvalue = 1;

    if ((sd_type == UNKNOW) || (block_num <= 1)) return rvalue;
    if (sd_type != SDHC) sector <<= sd_power;                                   // 不是SDHC, 需要将块地址转为字节地址, sector *= sd_block_size

    sd_cs_enable();
    response = sd_send_cmd(CMD18, sector);                                      // 发送 CMD18, 读多个块
    if (response == 0x00)                                                       // 应答正确
    {
        rvalue = 0;
        while (block_num--)
        {
            for (i=20000; i>0; i--)
            {
                if (sd_spi_tx_rx_byte(0xFF) == 0xFE) break;                     // 准备好传输数据
            }
            if (i)
            {
                for (i=sd_block_size; i>0; i--)
                    *pbuf++ = sd_spi_tx_rx_byte(0xFF);                          // 读多个块
                sd_spi_tx_rx_byte(0xFF);                                        // 读CRC校验
                sd_spi_tx_rx_byte(0xFF);
            }
            else
            {
                rvalue = 1;
            }
        }
    }
    sd_send_cmd(CMD12, 0);                                                      // 发送停止命令
    sd_cs_disable();
    sd_spi_tx_rx_byte(0xFF);

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  写入SD卡的一个块
  * @param  pbuf,   写入数据指针
  * @param  sector, 写入块内容, 非物理地址.
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdspi_write_block(const uint8_t* pbuf, uint32_t sector)
{
    uint32_t i;
    uint8_t  response;
    uint8_t  rvalue = 1;

    if (sd_type == UNKNOW) return rvalue;
    if (sd_type != SDHC) sector <<= sd_power;                                   // 不是SDHC, 需要将块地址转为字节地址, sector *= sd_block_size

    sd_cs_enable();
    response = sd_send_cmd(CMD24, sector);                                      // 发送 CMD24, 写单个块
    if (response == 0x00)                                                       // 应答正确
    {
        sd_spi_tx_rx_byte(0xFF);                                                // 若干CLK, 等待SD卡准备好
        sd_spi_tx_rx_byte(0xFE);                                                // 起始令牌0xFE
        for (i = sd_block_size; i>0 ; i--) sd_spi_tx_rx_byte(*pbuf++);          // 写一个块
        sd_spi_tx_rx_byte(0xFF);                                                // 读CRC校验
        sd_spi_tx_rx_byte(0xFF);

        if ((sd_spi_tx_rx_byte(0xFF) & 0x1F) == 0x05)                           // 读数据应答
        {
            for (i=200000; i>0; i--)                                            // 实测500ms左右
            {
                if (sd_spi_tx_rx_byte(0xFF) == 0xFF)                            // 等待数据写入结束
                {
                    rvalue = 0;                                                 // 标记为写成功
                    break;
                }
            }
        }
    }
    sd_cs_disable();
    sd_spi_tx_rx_byte(0xFF);

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  写入SD卡的多个块
  * @param  pbuf,   写入地址指针
  * @param  sector, 写入块内容, 非物理地址.
  * @param  block_num,  需要写入的SD块数量
  * @retval 返回值, 0, 成功. 1, 失败
  ******************************************************************************
  */
uint8_t sdspi_write_multi_blocks(const uint8_t* pbuf, uint32_t sector, uint32_t block_num)
{
    uint32_t i;
    uint8_t  response;
    uint8_t  rvalue = 1;

    if ((sd_type == UNKNOW) || (block_num <= 1)) return rvalue;
    if (sd_type != SDHC) sector <<= sd_power;                                   // 不是SDHC, 需要将块地址转为字节地址, sector *= sd_block_size

    sd_cs_enable();
    if(sd_type != MMC) sd_send_cmd(ACMD23, block_num);                          // 使用ACMD23预擦除SD卡内容
    response = sd_send_cmd(CMD25, sector);                                      // 发送 CMD25, 写多个块
    if (response == 0x00)                                                       // 应答正确
    {
        rvalue = 0;
        for (; block_num>0; block_num--)
        {
            sd_spi_tx_rx_byte(0xFF);                                            // 若干CLK, 等待SD卡准备好
            sd_spi_tx_rx_byte(0xFC);                                            // 放起始令牌0xFC, 表明是多块写入

            for(i=sd_block_size; i>0; i--)
                sd_spi_tx_rx_byte(*pbuf++);                                     // 写入一个块
            sd_spi_tx_rx_byte(0xFF);                                            // 读取CRC信息
            sd_spi_tx_rx_byte(0xFF);

            if ((sd_spi_tx_rx_byte(0xFF) & 0x1F) == 0x05)                       // 读取SD卡应答信息
            {
                for (i=200000; i>0; i--)                                        // 写入需要时间, V1卡较慢, 延时不能太小.
                {
                    if (sd_spi_tx_rx_byte(0xFF) == 0xFF) break;                 // 等待数据写入结束
                }
            }
            else
            {
                i = 0;
            }
            if (i == 0) rvalue = 1;                                             // 有一个块写入错误, 就标记为错误
        }
    }
    sd_spi_tx_rx_byte(0xFD);                                                    // 结束SD卡多块写
    for (i=100; i>0; i--)
    {
        if (sd_spi_tx_rx_byte(0xFF) == 0x00) break;
    }
    if (i == 0) rvalue = 1;
    for (i=200000; i>0; i--)                                                    // 等待多块写入结束
    {
        if (sd_spi_tx_rx_byte(0xFF) == 0xFF) break;
    }
    if (i == 0) rvalue = 1;
    sd_cs_disable();
    sd_spi_tx_rx_byte(0xFF);

    return rvalue;
}

/**
  ******************************************************************************
  * @brief  发送5字节的SD卡指令
  * @param  Cmd, 要发送的指令
  * @param  Arg, 指令参数
  * @retval 返回值, 指令应答
  ******************************************************************************
  */
uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg)
{
    uint8_t response;
    uint8_t i;

    sd_spi_tx_rx_byte(0xFF);

    sd_spi_tx_rx_byte(0x40 | cmd);                                              // 发送指令
    sd_spi_tx_rx_byte(arg >> 24);
    sd_spi_tx_rx_byte(arg >> 16);
    sd_spi_tx_rx_byte(arg >> 8);
    sd_spi_tx_rx_byte(arg >> 0);
    switch(cmd)
    {
    case CMD0:
        sd_spi_tx_rx_byte(0x95);                                                // CMD0的校验为0x95
        break;
    case CMD8:
        sd_spi_tx_rx_byte(0x87);                                                // CMD8的校验为0x87
        break;
    default:
        sd_spi_tx_rx_byte(0xff);                                                // 其它指令在SPI模式下无需校验
        break;
    }

    for (i=10; i>0; i--)                                                        // 接收应答
    {
        response = sd_spi_tx_rx_byte(0xFF);
        if(response != 0xFF) break;
    }

    return response;
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
