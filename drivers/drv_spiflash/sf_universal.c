/**
  ******************************************************************************
  * @file    sf_universal.c
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2011.12.23
  * @brief   SPI Flash ͨ������. ʹ�ñ�׼��SPIͨѶЭ��. ���ݴ�������̵�SPI FLASH.
  *          ����������֧�� drv_spiflash. ����ֱ�ӵ���.
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

/// SpiFlash SPI�ڳ�ʼ������. �������50MHz, ����оƬ������33MHz. (READ_DATA 03H ������)
__INLINE static void sf_unvsl_spi_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    // SPI1 ʱ��ԴΪ72Mhz, ����SPI ʱ��ԴΪ36Mhz.
    spi_init(SPIFLASH_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, SPI_BaudRatePrescaler_2);

    // ʹ��ģ��SPI.
//  spi_init(SPIFLASH_SPI_NAME, SPI_TYPE_HIGH_EDGE2_MSB, 0);

    // �����ȳ�ʼ��SS����Ϊ�������, �ٳ�ʼ��CS����
    RCC_APB2PeriphClockCmd(SPIFLASH_CS_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = SPIFLASH_CS_PIN;
    GPIO_Init(SPIFLASH_CS_PORT, &GPIO_InitStructure);
}

/// SpiFlash ͨ���շ�����
__INLINE static uint8_t sf_unvsl_tx_rx_byte(uint8_t data)
{
    return spi_tx_rx_byte(SPIFLASH_SPI_NAME, data);
}

/// SpiFlash Ƭѡ�ź�����ʹ��, ��IO�ڵ�ƽ�õ�
__INLINE static void sf_unvsl_cs_enable(void)
{
    SPIFLASH_CS_PORT->BRR = SPIFLASH_CS_PIN;
}

/// SpiFlash Ƭѡ�ź����Ž�ֹ, ��IO�ڵ�ƽ�ø�
__INLINE static void sf_unvsl_cs_disable(void)
{
    SPIFLASH_CS_PORT->BSRR = SPIFLASH_CS_PIN;
}

/**
  * @}
  */



/** @defgroup SpiFlash_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */
// ָ��궨��
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

// ״̬�Ĵ������λ�궨��
#define BUSY                            0x01

// ��������ʱ, Ҫ���͵���������
#define DUMMY_BYTE                      0xFF
/**
  * @}
  */

/** @defgroup SF_Universal_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup SF_Universal_Private_Function
  * @brief    ����˽�к���
  * @{
  */
static void sf_unvsl_set_write_enable(void);
static void sf_unvsl_wait_write_finish(void);
/**
  * @}
  */



/** @defgroup SF_Universal_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  SpiFlash SPIͨѶ����Ӧ�Ķ˿ڳ�ʼ������
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
  * @brief  ����оƬ״̬�Ĵ���, ��������ֻ������
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
  * @brief  оƬ��������ģʽ, �ĵ����. ���޷����ж�д����.
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_power_down(void)
{
    sf_unvsl_cs_enable();
    sf_unvsl_tx_rx_byte(POWER_DOWN);
    sf_unvsl_cs_disable();
    delay_us(500);                                                              // �ȴ�����˯��ģʽ
}

/**
  ******************************************************************************
  * @brief  оƬ�˳�����ģʽ. �˳���ɽ��ж�д����
  * @param  none
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_power_on(void)
{
    sf_unvsl_cs_enable();
    sf_unvsl_tx_rx_byte(RELEASE_POWER_DOWN);
    sf_unvsl_cs_disable();
    delay_us(500);                                                              // �ȴ�������ͨģʽ
}

/**
  ******************************************************************************
  * @brief  ��ȡоƬID��Ϣ
  * @param  none
  * @retval ����ֵ, оƬID��Ϣ
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
  * @brief  ����һ����(sector),��СΪ4K byte,����ʱ�����Ҫ500ms
  * @param  addr, �ֽڵ�ַ. �����õ�ַ���ڵ�������
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_sector_erase(uint32_t addr)
{
    sf_unvsl_set_write_enable();
    sf_unvsl_cs_enable();

    sf_unvsl_tx_rx_byte(SECTOR_EARSE);                                          // ����������ָ��
    sf_unvsl_tx_rx_byte((addr & 0xFF0000) >> 16);                               // ����Ҫ�����ĵ�ַ
    sf_unvsl_tx_rx_byte((addr & 0xFF00) >> 8);
    sf_unvsl_tx_rx_byte(addr & 0xFF);

    sf_unvsl_cs_disable();
    sf_unvsl_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  ����һ����(block),��СΪ64K byte,����ʱ�����Ҫ1s
  * @param  addr, �ֽڵ�ַ. �����õ�ַ���ڵĿ�����
  * @retval none
  ******************************************************************************
  */
void sf_unvsl_block_erase(uint32_t addr)
{
    sf_unvsl_set_write_enable();
    sf_unvsl_cs_enable();

    sf_unvsl_tx_rx_byte(BLOCK_ERASE);                                           // ���Ϳ����ָ��
    sf_unvsl_tx_rx_byte((addr & 0xFF0000) >> 16);                               // ����Ҫ�����ĵ�ַ
    sf_unvsl_tx_rx_byte((addr & 0xFF00) >> 8);
    sf_unvsl_tx_rx_byte(addr & 0xFF);

    sf_unvsl_cs_disable();
    sf_unvsl_wait_write_finish();
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
void sf_unvsl_chip_erase(void)
{
    sf_unvsl_set_write_enable();
    sf_unvsl_cs_enable();

    sf_unvsl_tx_rx_byte(CHIP_EARSE);                                            // ���Ϳ����ָ��

    sf_unvsl_cs_disable();
    sf_unvsl_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  �����ȡģʽ
  * @param  addr, ��ȡ���ݵ���ʼ��ַ
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
  * @brief  ��ȡһ������
  * @param  pbuf, ��ȡ��ַָ��
  * @param  num,  ��ȡ���ݵ��ֽ���
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
  * @brief  �˳���ȡģʽ
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
  * @brief  дһ��ҳ
  * @param  pbuf, Ҫд�������ָ��
  * @param  addr, Ҫд��ĵ�ַ
  * @param  num,  Ҫд����ֽ���
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
    sf_unvsl_tx_rx_byte(addr & 0xFF);                                           // ��8λ��ʵ����Ч��

    while (num--)
    {
        sf_unvsl_tx_rx_byte(*pbuf++);
    }

    sf_unvsl_cs_disable();
    sf_unvsl_wait_write_finish();
}

/**
  ******************************************************************************
  * @brief  ����д����
  * @param  none
  * @retval ����ֵ. 1, ����д��. 0, ��ֹд��
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
  * @brief  �ȴ�д��������
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

