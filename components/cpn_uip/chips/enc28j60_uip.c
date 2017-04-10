/**
  ******************************************************************************
  * @file    enc28j60_uip.c
  * @author  mousie-yu
  * @version V1.1.1
  * @date    2013.5.5
  * @brief   ENC28J60底层驱动函数. 针对UIP组件书写.
  *          ENC28J60 为 Microchip Technology, Inc. 生产.
  *          其MAC位址高三位(OUI)为 00-04-A3, 低三位由芯片供应商提供.
  *          此芯片不支持自动检测网络, 通讯不稳定, 不建议使用!
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "enc28j60_uip.h"
#include "stm32f10x.h"                                                          // __INLINE



/** @addtogroup Drivers
  * @{
  */
/** @defgroup ENC28J60_UIP
  * @{
  */
#ifdef ENC28J60_UIP_ENABLE



/**
  ******************************************************************************
  * @addtogroup ENC28J60_UIP_Configure
  ******************************************************************************
  * @{
  */
#define ENC28J60_PRINTF(...)            //assert_printf(__VA_ARGS__)

#define WAIT_TIMEOUT                    0x2FFFF

#if !UIP_TASK_RX_SCAN_MS
/// ENC28J60 中断处理
extern void uip_chip_irq_rx(void);
void enc28j60_isr(void)
{
    uip_chip_irq_rx();
}

/// ENC28J60 INT初始化函数.
static __INLINE void enc28j60_int_init(void)
{
    extint_init(ENC28J60_INT_NAME, EXTI_Trigger_Falling);
}
#endif

/// ENC28J60 SPI初始化函数.
static __INLINE void enc28j60_spi_init(void)
{
    // 使用硬件SPI
    spi_init(ENC28J60_SPI_NAME, SPI_TYPE_LOW_EDGE1_MSB, SPI_BaudRatePrescaler_4);

    // 使用模拟SPI
//  spi_init(ENC28J60_SPI_NAME, SPI_TYPE_LOW_EDGE1_MSB, 500);
}

/// ENC28J60 SPI收发函数宏定义
static __INLINE uint8_t enc28j60_spi_tx_rx_byte(uint8_t data)
{
    return spi_tx_rx_byte(ENC28J60_SPI_NAME, data);
}

/// ENC28J60 片选信号引脚初始化, IO口设置为推挽输出.
static __INLINE void enc28j60_cs_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(ENC28J60_CS_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = ENC28J60_CS_PIN;
    GPIO_Init(ENC28J60_CS_PORT, &GPIO_InitStructure);
}

/// ENC28J60 片选信号引脚使能, 将IO口电平置低
static __INLINE void enc28j60_cs_enable(void)
{
    GPIO_ResetBits(ENC28J60_CS_PORT, ENC28J60_CS_PIN);
}

/// ENC28J60 片选信号引脚禁止, 将IO口电平置高
static __INLINE void enc28j60_cs_disable(void)
{
    GPIO_SetBits(ENC28J60_CS_PORT, ENC28J60_CS_PIN);
}
/**
  * @}
  */



/** @defgroup ENC28J60_UIP_Private_TypeDefine
  * @brief    私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup ENC28J60_UIP_Private_MacroDefine
  * @brief    私有宏定义
  * @{
  */
// ENC28J60 Control Registers
// Control register definitions are a combination of address,
// bank number, and Ethernet/MAC/PHY indicator bits.
// - Register address                   (bits 0-4)
// - Bank number                        (bits 5-6)
// - MAC/PHY indicator                  (bit 7)
#define ADDR_MASK                       0x1F
#define BANK_MASK                       0x60
#define SPRD_MASK                       0x80
// All-bank registers
#define EIE                             0x1B
#define EIR                             0x1C
#define ESTAT                           0x1D
#define ECON2                           0x1E
#define ECON1                           0x1F
// Bank 0 registers
#define ERDPTL                          (0x00|0x00)
#define ERDPTH                          (0x01|0x00)
#define EWRPTL                          (0x02|0x00)
#define EWRPTH                          (0x03|0x00)
#define ETXSTL                          (0x04|0x00)
#define ETXSTH                          (0x05|0x00)
#define ETXNDL                          (0x06|0x00)
#define ETXNDH                          (0x07|0x00)
#define ERXSTL                          (0x08|0x00)
#define ERXSTH                          (0x09|0x00)
#define ERXNDL                          (0x0A|0x00)
#define ERXNDH                          (0x0B|0x00)
#define ERXRDPTL                        (0x0C|0x00)
#define ERXRDPTH                        (0x0D|0x00)
#define ERXWRPTL                        (0x0E|0x00)
#define ERXWRPTH                        (0x0F|0x00)
#define EDMASTL                         (0x10|0x00)
#define EDMASTH                         (0x11|0x00)
#define EDMANDL                         (0x12|0x00)
#define EDMANDH                         (0x13|0x00)
#define EDMADSTL                        (0x14|0x00)
#define EDMADSTH                        (0x15|0x00)
#define EDMACSL                         (0x16|0x00)
#define EDMACSH                         (0x17|0x00)
// Bank 1 registers
#define EHT0                            (0x00|0x20)
#define EHT1                            (0x01|0x20)
#define EHT2                            (0x02|0x20)
#define EHT3                            (0x03|0x20)
#define EHT4                            (0x04|0x20)
#define EHT5                            (0x05|0x20)
#define EHT6                            (0x06|0x20)
#define EHT7                            (0x07|0x20)
#define EPMM0                           (0x08|0x20)
#define EPMM1                           (0x09|0x20)
#define EPMM2                           (0x0A|0x20)
#define EPMM3                           (0x0B|0x20)
#define EPMM4                           (0x0C|0x20)
#define EPMM5                           (0x0D|0x20)
#define EPMM6                           (0x0E|0x20)
#define EPMM7                           (0x0F|0x20)
#define EPMCSL                          (0x10|0x20)
#define EPMCSH                          (0x11|0x20)
#define EPMOL                           (0x14|0x20)
#define EPMOH                           (0x15|0x20)
#define EWOLIE                          (0x16|0x20)
#define EWOLIR                          (0x17|0x20)
#define ERXFCON                         (0x18|0x20)
#define EPKTCNT                         (0x19|0x20)
// Bank 2 registers
#define MACON1                          (0x00|0x40|0x80)
#define MACON2                          (0x01|0x40|0x80)
#define MACON3                          (0x02|0x40|0x80)
#define MACON4                          (0x03|0x40|0x80)
#define MABBIPG                         (0x04|0x40|0x80)
#define MAIPGL                          (0x06|0x40|0x80)
#define MAIPGH                          (0x07|0x40|0x80)
#define MACLCON1                        (0x08|0x40|0x80)
#define MACLCON2                        (0x09|0x40|0x80)
#define MAMXFLL                         (0x0A|0x40|0x80)
#define MAMXFLH                         (0x0B|0x40|0x80)
#define MAPHSUP                         (0x0D|0x40|0x80)
#define MICON                           (0x11|0x40|0x80)
#define MICMD                           (0x12|0x40|0x80)
#define MIREGADR                        (0x14|0x40|0x80)
#define MIWRL                           (0x16|0x40|0x80)
#define MIWRH                           (0x17|0x40|0x80)
#define MIRDL                           (0x18|0x40|0x80)
#define MIRDH                           (0x19|0x40|0x80)
// Bank 3 registers
#define MAADR1                          (0x00|0x60|0x80)
#define MAADR0                          (0x01|0x60|0x80)
#define MAADR3                          (0x02|0x60|0x80)
#define MAADR2                          (0x03|0x60|0x80)
#define MAADR5                          (0x04|0x60|0x80)
#define MAADR4                          (0x05|0x60|0x80)
#define EBSTSD                          (0x06|0x60)
#define EBSTCON                         (0x07|0x60)
#define EBSTCSL                         (0x08|0x60)
#define EBSTCSH                         (0x09|0x60)
#define MISTAT                          (0x0A|0x60|0x80)
#define EREVID                          (0x12|0x60)
#define ECOCON                          (0x15|0x60)
#define EFLOCON                         (0x17|0x60)
#define EPAUSL                          (0x18|0x60)
#define EPAUSH                          (0x19|0x60)
// PHY registers
#define PHCON1                          0x00
#define PHSTAT1                         0x01
#define PHHID1                          0x02
#define PHHID2                          0x03
#define PHCON2                          0x10
#define PHSTAT2                         0x11
#define PHIE                            0x12
#define PHIR                            0x13
#define PHLCON                          0x14

// ENC28J60 ERXFCON Register Bit Definitions
#define ERXFCON_UCEN                    0x80
#define ERXFCON_ANDOR                   0x40
#define ERXFCON_CRCEN                   0x20
#define ERXFCON_PMEN                    0x10
#define ERXFCON_MPEN                    0x08
#define ERXFCON_HTEN                    0x04
#define ERXFCON_MCEN                    0x02
#define ERXFCON_BCEN                    0x01
// ENC28J60 EIE Register Bit Definitions
#define EIE_INTIE                       0x80
#define EIE_PKTIE                       0x40
#define EIE_DMAIE                       0x20
#define EIE_LINKIE                      0x10
#define EIE_TXIE                        0x08
#define EIE_WOLIE                       0x04
#define EIE_TXERIE                      0x02
#define EIE_RXERIE                      0x01
// ENC28J60 EIR Register Bit Definitions
#define EIR_PKTIF                       0x40
#define EIR_DMAIF                       0x20
#define EIR_LINKIF                      0x10
#define EIR_TXIF                        0x08
#define EIR_WOLIF                       0x04
#define EIR_TXERIF                      0x02
#define EIR_RXERIF                      0x01
// ENC28J60 ESTAT Register Bit Definitions
#define ESTAT_INT                       0x80
#define ESTAT_LATECOL                   0x10
#define ESTAT_RXBUSY                    0x04
#define ESTAT_TXABRT                    0x02
#define ESTAT_CLKRDY                    0x01
// ENC28J60 ECON2 Register Bit Definitions
#define ECON2_AUTOINC                   0x80
#define ECON2_PKTDEC                    0x40
#define ECON2_PWRSV                     0x20
#define ECON2_VRPS                      0x08
// ENC28J60 ECON1 Register Bit Definitions
#define ECON1_TXRST                     0x80
#define ECON1_RXRST                     0x40
#define ECON1_DMAST                     0x20
#define ECON1_CSUMEN                    0x10
#define ECON1_TXRTS                     0x08
#define ECON1_RXEN                      0x04
#define ECON1_BSEL1                     0x02
#define ECON1_BSEL0                     0x01
// ENC28J60 MACON1 Register Bit Definitions
#define MACON1_LOOPBK                   0x10
#define MACON1_TXPAUS                   0x08
#define MACON1_RXPAUS                   0x04
#define MACON1_PASSALL                  0x02
#define MACON1_MARXEN                   0x01
// ENC28J60 MACON2 Register Bit Definitions
#define MACON2_MARST                    0x80
#define MACON2_RNDRST                   0x40
#define MACON2_MARXRST                  0x08
#define MACON2_RFUNRST                  0x04
#define MACON2_MATXRST                  0x02
#define MACON2_TFUNRST                  0x01
// ENC28J60 MACON3 Register Bit Definitions
#define MACON3_PADCFG2                  0x80
#define MACON3_PADCFG1                  0x40
#define MACON3_PADCFG0                  0x20
#define MACON3_TXCRCEN                  0x10
#define MACON3_PHDRLEN                  0x08
#define MACON3_HFRMLEN                  0x04
#define MACON3_FRMLNEN                  0x02
#define MACON3_FULDPX                   0x01
// ENC28J60 MACON4 Register Bit Definitions
#define MACON4_DEFER                    0x40
#define MACON4_BPEN                     0x20
#define MACON4_NOBKOFF                  0x10
// ENC28J60 MICMD Register Bit Definitions
#define MICMD_MIISCAN                   0x02
#define MICMD_MIIRD                     0x01
// ENC28J60 MISTAT Register Bit Definitions
#define MISTAT_NVALID                   0x04
#define MISTAT_SCAN                     0x02
#define MISTAT_BUSY                     0x01
// ENC28J60 PHY PHCON1 Register Bit Definitions
#define PHCON1_PRST                     0x8000
#define PHCON1_PLOOPBK                  0x4000
#define PHCON1_PPWRSV                   0x0800
#define PHCON1_PDPXMD                   0x0100
// ENC28J60 PHY PHSTAT1 Register Bit Definitions
#define PHSTAT1_PFDPX                   0x1000
#define PHSTAT1_PHDPX                   0x0800
#define PHSTAT1_LLSTAT                  0x0004
#define PHSTAT1_JBSTAT                  0x0002
// ENC28J60 PHY PHSTAT2 Register Bit Definitions
#define PHSTAT2_TXSTAT                  0x2000
#define PHSTAT2_RXSTAT                  0x1000
#define PHSTAT2_COLSTAT                 0x0800
#define PHSTAT2_LSTAT                   0x0400
#define PHSTAT2_DPXSTAT                 0x0200
#define PHSTAT2_PLRITY                  0x0010
// ENC28J60 PHY PHCON2 Register Bit Definitions
#define PHCON2_FRCLINK                  0x4000
#define PHCON2_TXDIS                    0x2000
#define PHCON2_JABBER                   0x0400
#define PHCON2_HDLDIS                   0x0100

// ENC28J60 Packet Control Byte Bit Definitions
#define PKTCTRL_PHUGEEN                 0x08
#define PKTCTRL_PPADEN                  0x04
#define PKTCTRL_PCRCEN                  0x02
#define PKTCTRL_POVERRIDE               0x01

// ENC28J60 Transmit Status Vector
#define TSV_TXBYTECNT                   0
#define TSV_TXCOLLISIONCNT              16
#define TSV_TXCRCERROR                  20
#define TSV_TXLENCHKERROR               21
#define TSV_TXLENOUTOFRANGE             22
#define TSV_TXDONE                      23
#define TSV_TXMULTICAST                 24
#define TSV_TXBROADCAST                 25
#define TSV_TXPACKETDEFER               26
#define TSV_TXEXDEFER                   27
#define TSV_TXEXCOLLISION               28
#define TSV_TXLATECOLLISION             29
#define TSV_TXGIANT                     30
#define TSV_TXUNDERRUN                  31
#define TSV_TOTBYTETXONWIRE             32
#define TSV_TXCONTROLFRAME              48
#define TSV_TXPAUSEFRAME                49
#define TSV_BACKPRESSUREAPP             50
#define TSV_TXVLANTAGFRAME              51

#define TSV_SIZE                        7
#define TSV_BYTEOF(x)                   ((x) / 8)
#define TSV_BITMASK(x)                  (1 << ((x) % 8))
#define TSV_GETBIT(x, y)                (((x)[TSV_BYTEOF(y)] & TSV_BITMASK(y)) ? 1 : 0)

// ENC28J60 Receive Status Vector
#define RSV_RXLONGEVDROPEV              16
#define RSV_CARRIEREV                   18
#define RSV_CRCERROR                    20
#define RSV_LENCHECKERR                 21
#define RSV_LENOUTOFRANGE               22
#define RSV_RXOK                        23
#define RSV_RXMULTICAST                 24
#define RSV_RXBROADCAST                 25
#define RSV_DRIBBLENIBBLE               26
#define RSV_RXCONTROLFRAME              27
#define RSV_RXPAUSEFRAME                28
#define RSV_RXUNKNOWNOPCODE             29
#define RSV_RXTYPEVLAN                  30

#define RSV_SIZE                        6
#define RSV_BITMASK(x)                  (1 << ((x) - 16))
#define RSV_GETBIT(x, y)                (((x) & RSV_BITMASK(y)) ? 1 : 0)

// SPI operation codes
#define ENC28J60_READ_CTRL_REG          0x00
#define ENC28J60_READ_BUF_MEM           0x3A
#define ENC28J60_WRITE_CTRL_REG         0x40
#define ENC28J60_WRITE_BUF_MEM          0x7A
#define ENC28J60_BIT_FIELD_SET          0x80
#define ENC28J60_BIT_FIELD_CLR          0xA0
#define ENC28J60_SOFT_RESET             0xFF

// The RXSTART_INIT should be zero. See Rev. B4 Silicon Errata
// buffer boundaries applied to internal 8K ram
// the entire available packet buffer space is allocated
// start with recbuf at 0/
#define RXSTART_INIT                    0x0
// receive buffer end
#define RXSTOP_INIT                     (0x1FFF-0x0600) - 1
// start TX buffer at 0x1FFF-0x0600, pace for one full ethernet frame (~1500 bytes)

#define TXSTART_INIT                    (0x1FFF-0x0600)
// stp TX buffer at end of mem
#define TXSTOP_INIT                     0x1FFF

// max frame length which the conroller will accept:
#define MAX_FRAMELEN                    UIP_BUFSIZE
/**
  * @}
  */

/** @defgroup ENC28J60_UIP_Variables
  * @brief    定义全局变量(私有/公有)
  * @{
  */
static uint8_t  enc28j60_bank;
static uint16_t next_packet;
/**
  * @}
  */

/** @defgroup ENC28J60_UIP_Private_Function
  * @brief    定义私有函数
  * @{
  */
static uint8_t  enc28j60_read_op(uint8_t op, uint8_t address);
static void     enc28j60_write_op(uint8_t op, uint8_t address, uint8_t data);
static void     enc28j60_set_bank(uint8_t address);

static uint8_t  enc28j60_read(uint8_t address);
static void     enc28j60_read_buffer(uint8_t* data, uint16_t len);
static void     enc28j60_write(uint8_t address, uint8_t data);
static void     enc28j60_write_buffer(uint8_t* data, uint16_t len);
static uint16_t enc28j60_phy_read(uint8_t address);
static void     enc28j60_phy_write(uint8_t address, uint16_t data);

static void enc28j60_info(void);
/**
  * @}
  */



/** @defgroup ENC28J60_UIP_Function
  * @brief    函数原文件
  * @{
  */

/**
  ******************************************************************************
  * @brief  ENC28J60 SPI通讯和相应的端口初始化函数
  * @param  none
  * @retval none
  ******************************************************************************
  */
void enc28j60_port_init(void)
{
#if !UIP_TASK_RX_SCAN_MS
    enc28j60_int_init();                                                        // 初始化外部中断引脚
#endif
    enc28j60_spi_init();                                                        // 必须先初始化SS引脚为软件控制
    enc28j60_cs_init();                                                         // 再初始化SS的GPIO口才有效
    enc28j60_cs_disable();
}

/**
  ******************************************************************************
  * @brief  初始化 ENC28J60
  * @param  none
  * @retval none
  ******************************************************************************
  */
void enc28j60_init(uip_macaddr_t mac)
{
    uint32_t i;

    enc28j60_cs_disable();

    // perform system reset
    enc28j60_write_op(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);

    // wait enc28j60 to ready
    for (i=0; i<50000; i++) __NOP();
    while (!(enc28j60_read(ESTAT) & ESTAT_CLKRDY))
    {
        if (++i > 0x4FFFF) return;                                              // 长时间错误, 直接返回.
    }

    next_packet = RXSTART_INIT;

    // Rx start
    enc28j60_write(ERXSTL, RXSTART_INIT&0xFF);
    enc28j60_write(ERXSTH, RXSTART_INIT>>8);
    // set receive pointer address
    enc28j60_write(ERXRDPTL, RXSTOP_INIT&0xFF);
    enc28j60_write(ERXRDPTH, RXSTOP_INIT>>8);
    // RX end
    enc28j60_write(ERXNDL, RXSTOP_INIT&0xFF);
    enc28j60_write(ERXNDH, RXSTOP_INIT>>8);

    // TX start
    enc28j60_write(ETXSTL, TXSTART_INIT&0xFF);
    enc28j60_write(ETXSTH, TXSTART_INIT>>8);
    // set transmission pointer address
    enc28j60_write(EWRPTL, TXSTART_INIT&0xFF);
    enc28j60_write(EWRPTH, TXSTART_INIT>>8);
    // TX end
    enc28j60_write(ETXNDL, TXSTOP_INIT&0xFF);
    enc28j60_write(ETXNDH, TXSTOP_INIT>>8);

    // do bank 1 stuff, packet filter:
    // For broadcast packets we allow only ARP packtets
    // All other packets should be unicast only for our mac (MAADR)
    //
    // The pattern to match on is therefore
    // Type     ETH.DST
    // ARP      BROADCAST
    // 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
    // in binary these poitions are:11 0000 0011 1111
    // This is hex 303F->EPMM0=0x3f,EPMM1=0x30
    enc28j60_write(ERXFCON, ERXFCON_UCEN|ERXFCON_CRCEN|ERXFCON_BCEN);

    // do bank 2 stuff
    // enable MAC receive
    enc28j60_write(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
    enc28j60_write(MACON2, 0x00);
    // enable automatic padding to 60bytes and CRC operations
    enc28j60_write_op(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN | MACON3_FULDPX);
    // bring MAC out of reset

    // set inter-frame gap
    enc28j60_write(MABBIPG, 0x15);
    enc28j60_write(MAIPGL, 0x12);
    enc28j60_write(MAIPGH, 0x0C);

    // Set the maximum packet size which the controller will accept
    // Do not send packets longer than MAX_FRAMELEN:
    enc28j60_write(MAMXFLL, MAX_FRAMELEN&0xFF);
    enc28j60_write(MAMXFLH, MAX_FRAMELEN>>8);

    // do bank 3 stuff
    // write MAC address
    // NOTE: MAC address in ENC28J60 is byte-backward
    enc28j60_write(MAADR0, mac.addr[5]);
    enc28j60_write(MAADR1, mac.addr[4]);
    enc28j60_write(MAADR2, mac.addr[3]);
    enc28j60_write(MAADR3, mac.addr[2]);
    enc28j60_write(MAADR4, mac.addr[1]);
    enc28j60_write(MAADR5, mac.addr[0]);

    // output off
    enc28j60_write(ECOCON, 0x00);

    // enc28j60_phy_write(PHCON1, 0x00);       // 半双工
    enc28j60_phy_write(PHCON1, PHCON1_PDPXMD); // 全双工
    // no loopback of transmitted frames
    enc28j60_phy_write(PHCON2, PHCON2_HDLDIS);

    // switch to bank 0
    enc28j60_set_bank(ECON1);
    // enable interrutps
    enc28j60_write_op(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
    // enable packet reception
    enc28j60_write_op(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);

    // setup clkout: 2 is 12.5MHz:
    //enc28j60_write(ECOCON, 2);

    enc28j60_phy_write(PHLCON, 0xD76);

    enc28j60_info();
}

/**
  ******************************************************************************
  * @brief  ENC28J60 发送数据包
  * @param  pdata, 数据包指针
  * @param  len,   数据包长度
  * @retval none
  ******************************************************************************
  */
void enc28j60_tx(uint8_t *pdata, uint16_t len)
{
    // Set the write pointer to start of transmit buffer area
    enc28j60_write(EWRPTL, TXSTART_INIT&0xFF);
    enc28j60_write(EWRPTH, TXSTART_INIT>>8);
    // Set the TXND pointer to correspond to the packet size given
    enc28j60_write(ETXNDL, (TXSTART_INIT+len) & 0xFF);
    enc28j60_write(ETXNDH, (TXSTART_INIT+len) >> 8);

    // write per-packet control byte (0x00 means use macon3 settings)
    enc28j60_write_op(ENC28J60_WRITE_BUF_MEM, 0, 0x00);
    enc28j60_write_buffer(pdata, len);

    // send the contents of the transmit buffer onto the network
    enc28j60_write_op(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
    // Reset the transmit logic problem. See Rev. B4 Silicon Errata point 12.
    if (enc28j60_read(EIR) & EIR_TXERIF)
    {
        ENC28J60_PRINTF("\r\n-- enc28j60 tx error:");
        enc28j60_info();
        enc28j60_set_bank(ECON1);
        enc28j60_write_op(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
        enc28j60_write_op(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
        enc28j60_set_bank(EIR);
        enc28j60_write_op(ENC28J60_BIT_FIELD_CLR, EIR, EIR_TXERIF);
    }
}

/**
  ******************************************************************************
  * @brief  ENC28J60 接收一个数据包
  * @param  pdata,  数据包指针
  * @param  maxlen, 应用程序允许的最大数据包长度
  * @retval 返回值. 收到的数据包长度
  ******************************************************************************
  */
uint16_t enc28j60_rx(uint8_t *pdata, uint16_t maxlen)
{
    uint32_t len;
    uint16_t rxstat;
    uint32_t pk_counter;

    pk_counter = enc28j60_read(EPKTCNT);
    if (pk_counter)
    {
        // Set the read pointer to the start of the received packet
        enc28j60_write(ERDPTL, (next_packet));
        enc28j60_write(ERDPTH, (next_packet)>>8);

        // read the next packet pointer
        next_packet  = enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0);
        next_packet |= enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0) << 8;

        // read the packet length (see datasheet page 43)
        len  = enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0);
        len |= enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0) << 8;

        len-=4; //remove the CRC count

        // read the receive status (see datasheet page 43)
        rxstat  = enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0);
        rxstat |= enc28j60_read_op(ENC28J60_READ_BUF_MEM, 0) << 8;

        // check CRC and symbol errors (see datasheet page 44, table 7-3):
        // The ERXFCON.CRCEN is set by default. Normally we should not
        // need to check this.
        if ((rxstat & 0x80)==0)
        {
            len=0;
        }
        else
        {
            if (len > maxlen) len = maxlen;
            enc28j60_read_buffer(pdata, len);
        }

        // Move the RX read pointer to the start of the next received packet
        // This frees the memory we just read out
        enc28j60_write(ERXRDPTL, (next_packet));
        enc28j60_write(ERXRDPTH, (next_packet)>>8);

        // decrement the packet counter indicate we are done with this packet
        enc28j60_write_op(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);
    }
    else
    {
        len = 0;
    }

    return len;
}

/**
  ******************************************************************************
  * @brief  ENC28J60 检查网络连接状态
  * @param  none
  * @retval 返回值.  0-未连接, 1-已连接.
  ******************************************************************************
  */
uint8_t enc28j60_linked(void)
{
    uint16_t reg = enc28j60_phy_read(PHSTAT2);

    // 1, on. 0, off
    if (reg & PHSTAT2_LSTAT) return 1;
    else                     return 0;
}

/**
  ******************************************************************************
  * @brief  ENC28J60 基础操作函数
  * @param  none
  * @retval none
  ******************************************************************************
  */
uint8_t enc28j60_read_op(uint8_t op, uint8_t address)
{
    int temp = 0;

    enc28j60_cs_enable();
    enc28j60_spi_tx_rx_byte(op | (address & ADDR_MASK));
    temp = enc28j60_spi_tx_rx_byte(0x00);

    // do dummy read if needed (for mac and mii, see datasheet page 29)
    if (address & 0x80)
    {
        temp = enc28j60_spi_tx_rx_byte(0x00);
    }
    enc28j60_cs_disable();
    return (temp);
}

void enc28j60_write_op(uint8_t op, uint8_t address, uint8_t data)
{
    enc28j60_cs_enable();

    enc28j60_spi_tx_rx_byte(op | (address & ADDR_MASK));
    enc28j60_spi_tx_rx_byte(data);

    enc28j60_cs_disable();
}

void enc28j60_set_bank(uint8_t address)
{
    // set the bank (if needed)
    if ((address & BANK_MASK) != enc28j60_bank)
    {
        // set the bank
        enc28j60_write_op(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
        enc28j60_write_op(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK)>>5);
        enc28j60_bank = (address & BANK_MASK);
    }
}

uint8_t enc28j60_read(uint8_t address)
{
    // set the bank
    enc28j60_set_bank(address);
    // do the read
    return enc28j60_read_op(ENC28J60_READ_CTRL_REG, address);
}

void enc28j60_read_buffer(uint8_t* data, uint16_t len)
{
    enc28j60_cs_enable();
    enc28j60_spi_tx_rx_byte(ENC28J60_READ_BUF_MEM);
    while (len)
    {
        len--;
        *data = enc28j60_spi_tx_rx_byte(0x00);
        data++;
    }
    enc28j60_cs_disable();
}

void enc28j60_write(uint8_t address, uint8_t data)
{
    // set the bank
    enc28j60_set_bank(address);
    // do the write
    enc28j60_write_op(ENC28J60_WRITE_CTRL_REG, address, data);
}

void enc28j60_write_buffer(uint8_t* data, uint16_t len)
{
    enc28j60_cs_enable();
    enc28j60_spi_tx_rx_byte(ENC28J60_WRITE_BUF_MEM);
    while (len)
    {
        len--;
        enc28j60_spi_tx_rx_byte(*data);
        data++;
    }
    enc28j60_cs_disable();
}

uint16_t enc28j60_phy_read(uint8_t address)
{
    uint16_t reg;

    // Set the right address and start the register read operation
    enc28j60_write(MIREGADR, address);
    enc28j60_write(MICMD, MICMD_MIIRD);

    // wait until the PHY read completes
    while (enc28j60_read(MISTAT) & MISTAT_BUSY) __NOP();

    // reset reading bit
    enc28j60_write(MICMD, 0x00);

    reg   = enc28j60_read(MIRDH);
    reg <<= 8;
    reg  |= enc28j60_read(MIRDL);
    return reg;
}

void enc28j60_phy_write(uint8_t address, uint16_t data)
{
    // set the PHY register address
    enc28j60_write(MIREGADR, address);

    // write the PHY data
    enc28j60_write(MIWRL, data);
    enc28j60_write(MIWRH, data>>8);

    // wait until the PHY write completes
    while (enc28j60_read(MISTAT) & MISTAT_BUSY) __NOP();
}

/**
  ******************************************************************************
  * @brief  ENC28J60 读取并打印芯片信息. 用于调试.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void enc28j60_info(void)
{
    ENC28J60_PRINTF("\r\n-- enc28j60 registers:\r\n");
    ENC28J60_PRINTF("   HwRevID: 0x%02x\r\n", enc28j60_read(EREVID));
    ENC28J60_PRINTF("   Cntrl: ECON1 ECON2 ESTAT  EIR  EIE\r\n");
    ENC28J60_PRINTF("          0x%02x  0x%02x  0x%02x  0x%02x  0x%02x\r\n",enc28j60_read(ECON1), enc28j60_read(ECON2), enc28j60_read(ESTAT), enc28j60_read(EIR), enc28j60_read(EIE));
    ENC28J60_PRINTF("   MAC  : MACON1 MACON3 MACON4\r\n");
    ENC28J60_PRINTF("          0x%02x   0x%02x   0x%02x\r\n", enc28j60_read(MACON1), enc28j60_read(MACON3), enc28j60_read(MACON4));
    ENC28J60_PRINTF("   Rx   : ERXST  ERXND  ERXWRPT ERXRDPT ERXFCON EPKTCNT MAMXFL\r\n");
    ENC28J60_PRINTF("          0x%04x 0x%04x 0x%04x  0x%04x  ",
        (enc28j60_read(ERXSTH) << 8) | enc28j60_read(ERXSTL),
        (enc28j60_read(ERXNDH) << 8) | enc28j60_read(ERXNDL),
        (enc28j60_read(ERXWRPTH) << 8) | enc28j60_read(ERXWRPTL),
        (enc28j60_read(ERXRDPTH) << 8) | enc28j60_read(ERXRDPTL));
    ENC28J60_PRINTF("0x%02x    0x%02x    0x%04x\r\n", enc28j60_read(ERXFCON), enc28j60_read(EPKTCNT),
        (enc28j60_read(MAMXFLH) << 8) | enc28j60_read(MAMXFLL));
    ENC28J60_PRINTF("   Tx   : ETXST  ETXND  MACLCON1 MACLCON2 MAPHSUP\r\n");
    ENC28J60_PRINTF("          0x%04x 0x%04x 0x%02x     0x%02x     0x%02x\r\n\r\n",
        (enc28j60_read(ETXSTH) << 8) | enc28j60_read(ETXSTL),
        (enc28j60_read(ETXNDH) << 8) | enc28j60_read(ETXNDL),
        enc28j60_read(MACLCON1), enc28j60_read(MACLCON2), enc28j60_read(MAPHSUP));
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
