/**
  ******************************************************************************
  * @file    sf_mx25l.c
  * @author  mousie-yu
  * @version V2.0.0
  * @date    2011.12.23
  * @brief   ���� MX25Lϵ�� SPI Flash ��������. ʹ�� 4DTRD ģʽ��, 4PPģʽд.
  *          ����������֧�� drv_spiflash. ����ֱ�ӵ���.
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

/// IO�ڲ����궨��
#define MX25L_SCLK_HIGH()               MX25L_SCLK_PORT->BSRR = MX25L_SCLK_PIN
#define MX25L_SCLK_LOW()                MX25L_SCLK_PORT->BRR  = MX25L_SCLK_PIN

#define MX25L_SIO0_HIGH()               MX25L_SIO_PORT->BSRR = MX25L_SIO0_PIN
#define MX25L_SIO0_LOW()                MX25L_SIO_PORT->BRR  = MX25L_SIO0_PIN

#define MX25L_SIO1_HIGH()               MX25L_SIO_PORT->BSRR = MX25L_SIO1_PIN
#define MX25L_SIO1_LEVEL()              (MX25L_SIO_PORT->IDR & MX25L_SIO1_PIN)

#define MX25L_SIO_IDR_GET()             (MX25L_SIO_PORT->IDR)
#define MX25L_SIO_ODR_GET()             (MX25L_SIO_PORT->ODR)
#define MX25L_SIO_ODR_SET(val)          MX25L_SIO_PORT->ODR = val;

/// MX25L SPI��ʼ������. �������50MHz
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

    // SIO0, ����Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Pin   = MX25L_SIO0_PIN;
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);

    // SIO1-3, ����Ϊ����ģʽ
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = (MX25L_SIO1_PIN | MX25L_SIO2_PIN | MX25L_SIO3_PIN);
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);
}

/// MX25L ͨ���շ�����, ���Ƶ�� 50MHz
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

/// MX25L 4PPģʽ��������, ���Ƶ�� 20MHz
__INLINE static void mx25l_4PP_tx_byte(uint8_t data)
{
    uint16_t port_val, temp;

    temp = (data >> 4) & 0x000F;                                                // ȡ�����ݸ�4bit
    port_val = MX25L_SIO_ODR_GET() & 0xFFF0;                                    // ȷ����4bitΪ0
    MX25L_SIO_ODR_SET(port_val | temp);                                         // �������
    MX25L_SCLK_HIGH();
    __NOP(); __NOP(); __NOP(); __NOP();                                         // �ʵ�����ʱ
    MX25L_SCLK_LOW();
    temp = data & 0x000F;                                                       // ȡ�����ݵ�4bit
    port_val = MX25L_SIO_ODR_GET() & 0xFFF0;                                    // ȷ����4bitΪ0
    MX25L_SIO_ODR_SET(port_val | temp);
    MX25L_SCLK_HIGH();
    __NOP(); __NOP(); __NOP(); __NOP();                                         // �ʵ�����ʱ
    MX25L_SCLK_LOW();
}

/// MX25L 4DTRDģʽ��������, ���Ƶ�� 50MHz
__INLINE static void mx25l_4DTRD_tx_byte(uint8_t data)
{
    volatile uint16_t port_val, temp;

    temp = (data >> 4) & 0x000F;                                                // ȡ�����ݸ�4bit
    port_val = MX25L_SIO_ODR_GET() & 0xFFF0;                                    // ȷ����4bitΪ0
    MX25L_SIO_ODR_SET(port_val | temp);                                         // �������

    MX25L_SCLK_HIGH();

    temp = data & 0x000F;                                                       // ȡ�����ݵ�4bit
    port_val = MX25L_SIO_ODR_GET() & 0xFFF0;                                    // ȷ����4bitΪ0
    MX25L_SIO_ODR_SET(port_val | temp);

    MX25L_SCLK_LOW();
}

/// MX25L 4DTRDģʽ��������, ���Ƶ�� 50MHz
__INLINE static uint8_t mx25l_4DTRD_rx_byte()
{
    static volatile uint8_t rec;                                                // ��ֹ���Ż��ȼ�����

    rec = MX25L_SIO_IDR_GET();
    MX25L_SCLK_HIGH();
    rec <<= 4;
    rec |= (MX25L_SIO_IDR_GET() & 0x000F);
    MX25L_SCLK_LOW();

    return rec;
}

/// MX25L Ƭѡ�ź�����ʹ��, ��IO�ڵ�ƽ�õ�
__INLINE static void mx25l_cs_enable(void)
{
    MX25L_CS_PORT->BRR = MX25L_CS_PIN;
}

/// MX25L Ƭѡ�ź����Ž�ֹ, ��IO�ڵ�ƽ�ø�
__INLINE static void mx25l_cs_disable(void)
{
    MX25L_CS_PORT->BSRR = MX25L_CS_PIN;
}

/**
  * @}
  */



/** @defgroup SF_MX25L_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */
// ָ��궨��
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

// ״̬�Ĵ������λ�궨��
#define QE                              0x40
#define WIP                             0x01

// ��������ʱ, Ҫ���͵���������
#define DUMMY_BYTE                      0xFF
/**
  * @}
  */

/** @defgroup SF_MX25L_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup SF_MX25L_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static void mx25l_set_write_enable(void);
static void mx25l_wait_write_finish(void);
/**
  * @}
  */



/** @defgroup SF_MX25L_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  MX25L SPIͨѶ����Ӧ�Ķ˿ڳ�ʼ������
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

    mx25l_set_protect(MX25L_PROTECT_NONE);                                      // ����Ϊ4��ģʽ, �����д
}

/**
  ******************************************************************************
  * @brief  ����оƬ״̬�Ĵ���, ��������ֻ������
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
  * @brief  оƬ��������ģʽ, �ĵ����. ���޷����ж�д����.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void mx25l_power_down(void)
{
    mx25l_cs_enable();
    mx25l_tx_rx_byte(DP);
    mx25l_cs_disable();
    mx25l_delay_us(15);                                                         // ������ʱ10us, �ȴ�����˯��ģʽ
}

/**
  ******************************************************************************
  * @brief  оƬ�˳�����ģʽ. �˳���ɽ��ж�д����
  * @param  none
  * @retval none
  ******************************************************************************
  */
void mx25l_power_on(void)
{
    mx25l_cs_enable();
    mx25l_tx_rx_byte(RDP);
    mx25l_cs_disable();
    mx25l_delay_us(150);                                                        // ������ʱ100us, �ȴ�������ͨģʽ
}

/**
  ******************************************************************************
  * @brief  ��ȡоƬID��Ϣ
  * @param  none
  * @retval ����ֵ, оƬID��Ϣ
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
  * @brief  ����һ����(sector),��СΪ4K byte,����ʱ�����Ҫ500ms
  * @param  addr, �ֽڵ�ַ. �����õ�ַ���ڵ�������
  * @retval none
  ******************************************************************************
  */
void mx25l_sector_erase(uint32_t addr)
{
    mx25l_set_write_enable();
    mx25l_cs_enable();

    mx25l_tx_rx_byte(SE);                                                       // ����������ָ��
    mx25l_tx_rx_byte((addr & 0xFF0000) >> 16);                                  // ����Ҫ�����ĵ�ַ
    mx25l_tx_rx_byte((addr & 0xFF00) >> 8);
    mx25l_tx_rx_byte(addr & 0xFF);

    mx25l_cs_disable();
    mx25l_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  ����һ����(block),��СΪ64K byte,����ʱ�����Ҫ1s
  * @param  addr, �ֽڵ�ַ. �����õ�ַ���ڵĿ�����
  * @retval none
  ******************************************************************************
  */
void mx25l_block_erase(uint32_t addr)
{
    mx25l_set_write_enable();
    mx25l_cs_enable();

    mx25l_tx_rx_byte(BE);                                                       // ���Ϳ����ָ��
    mx25l_tx_rx_byte((addr & 0xFF0000) >> 16);                                  // ����Ҫ�����ĵ�ַ
    mx25l_tx_rx_byte((addr & 0xFF00) >> 8);
    mx25l_tx_rx_byte(addr & 0xFF);

    mx25l_cs_disable();
    mx25l_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  ����оƬ����������(chip). ����ʱ���оƬ��С�й�.
  *         ����ʱ��Ϊ�뼶���. ��Ҫ������ʱ�ȴ������.
  *         ��ʱʱ��5-100s����. 8M-bit��Ҫ10s, 64M-bit��Ҫ100s
  * @param  none
  * @retval none
  ******************************************************************************
  */
void mx25l_chip_erase(void)
{
    mx25l_set_write_enable();
    mx25l_cs_enable();

    mx25l_tx_rx_byte(CE);                                                       // ���Ϳ����ָ��

    mx25l_cs_disable();
    mx25l_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  �����ȡģʽ
  * @param  addr, ��ȡ���ݵ���ʼ��ַ
  * @retval none
  ******************************************************************************
  */
void mx25l_read_start(uint32_t addr)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    mx25l_cs_enable();

    mx25l_tx_rx_byte(_4DTRD);

    // ��SIO0-3����Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = (MX25L_SIO0_PIN | MX25L_SIO1_PIN | MX25L_SIO2_PIN | MX25L_SIO3_PIN);
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);

    mx25l_4DTRD_tx_byte((addr & 0xFF0000) >> 16);                               // ����Address
    mx25l_4DTRD_tx_byte((addr & 0xFF00) >> 8);
    mx25l_4DTRD_tx_byte( addr & 0xFF);

    // ���뽫IO����Ϊ��, �ȴ������ź�. ��ͨģʽ, 8��Dummy cycles
    mx25l_4DTRD_tx_byte(0xFF);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);
    mx25l_4DTRD_tx_byte(DUMMY_BYTE);

    // ��SIO0-3����Ϊ����ģʽ, ׼����������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);
}

/**
  ******************************************************************************
  * @brief  ��ȡһ������
  * @param  pbuf, ��ȡ��ַָ��
  * @param  num,  ��ȡ���ݵ��ֽ���
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
  * @brief  �˳���ȡģʽ
  * @param  none
  * @retval none
  ******************************************************************************
  */
void mx25l_read_end(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    mx25l_cs_disable();

    // ��SIO0�ָ�Ϊ�������ģʽ
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = MX25L_SIO0_PIN;
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);

    // ��SIO1-3�ָ�Ϊ����ģʽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin  = (MX25L_SIO1_PIN | MX25L_SIO2_PIN | MX25L_SIO3_PIN);
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);
}

/**
  ******************************************************************************
  * @brief  дһ��ҳ
  * @param  pbuf, Ҫд�������ָ��
  * @param  addr, Ҫд��ĵ�ַ
  * @param  num,  Ҫд����ֽ���
  * @retval none
  ******************************************************************************
  */
void mx25l_write_page(uint8_t *pbuf, uint32_t addr, uint32_t num)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    mx25l_set_write_enable();
    mx25l_cs_enable();

    mx25l_tx_rx_byte(_4PP);

    // ��SIO0-3����Ϊ�������ģʽ
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

    // ��SIO1-3�ָ�Ϊ����ģʽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin  = (MX25L_SIO1_PIN | MX25L_SIO2_PIN | MX25L_SIO3_PIN);
    GPIO_Init(MX25L_SIO_PORT, &GPIO_InitStructure);

    mx25l_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  ����д����
  * @param  none
  * @retval ����ֵ. 1, ����д��. 0, ��ֹд��
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
  * @brief  �ȴ�д��������
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

