/**
  ******************************************************************************
  * @file    drv_spiflash.h
  * @author  mousie-yu
  * @version V2.0.1
  * @date    2013.2.10
  * @brief   SPI Flash ͨ����������. ��Ҫ��Ʒ�Ƶ�SPI FLASH�ײ�����֧��.
  *          ���ݲ�ͬ�ĵײ�����, ��֧�ֱ�׼SPIģʽ, ģ�����ģʽ, 4IOģʽ.
  *          v2.0.1, ����һ��������ȡʱ, ���ݴ��������bug.
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
  * @brief    �û�����
  ******************************************************************************
  * @{
  */
#include "stm32f10x_conf.h"                                                     // Ӳ�����������ļ�

//#define SPIFLASH_MBIT                   16                                      ///< ѡ��SPI FLASHоƬ��С��λΪ(M bit), ��ʹ�ÿɶ���Ϊ0�򲻶���

#include "sf_mx25l.h"
#include "sf_universal.h"

#if ((defined SPIFLASH_UNIVERSAL) && (defined MX25L_4DTRD_4PP))
    #error "SPIFLASH_MODE define error!"
#endif
/**
  * @}
  */



/** @defgroup DRV_SpiFlash_Public_TypeDefine
  * @brief    �������Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SpiFlash_Public_MacroDefine
  * @brief    ���к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SpiFlash_Public_Variable
  * @brief    ��������ȫ�ֱ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_SpiFlash_Public_Function
  * @brief    ���幫�к���
  * @{
  */
#if (SPIFLASH_MBIT > 0)

void spiflash_init(void);                                                       // ͨѶ�ڳ�ʼ��
/**
  ******************************************************************************
  * ����д��������
  * ÿ��оƬ�ɱ������򶼲�һ��, ������Ҫ�ɲο������ֲ����ж���
  * status��Ҫ���ò�ͬоƬ���ض��궨��.
  ******************************************************************************
  */
void spiflash_set_protect(uint8_t status);
void spiflash_power_down(void);                                                 // оƬ��������ģʽ
void spiflash_power_on(void);                                                   // оƬ�����˳�����ģʽ
uint32_t spiflash_read_id(void);                                                // ��ȡID��Ϣ

uint8_t spiflash_sector_erase(uint32_t addr);                                   // ����һ����, ��СΪ4K byte
uint8_t spiflash_block_erase(uint32_t addr);                                    // ����һ����, ��СΪ64K byte
//void spiflash_chip_erase(void);                                               // ������ʹ��. ��������оƬ����, ��Ҫ������ʱ�ȴ�����ɲ���.

// ���º���ֱ�ӵ��ü���. д��ǰ����ҪԤ�ȵ��ò�������.
uint8_t spiflash_read(uint8_t *pbuf, uint32_t read_addr, uint32_t num);         // ��spi flash�ж�ȡ����
uint8_t spiflash_write_buf(uint8_t *pbuf, uint32_t write_addr, uint32_t num);   // д������, δ��һ�����������ݴ���buf��
void spiflash_write_buf2flash(void);                                            // ��buf�е�����д�뵽flash��

/**
  ******************************************************************************
  * ���º���Ϊ������ȡ����.
  * ע��������ȡ����ֱ�Ӵ� SPI FLASH �ж�ȡ���ݵ�, ������ buffer ��.
  * �������ڴ��ģ��ȡ����ʱʹ��, �����ݻ�����ֻ��512�ֽ�, ��Ҫ��ȡ10K������ʱ, ��ʹ��.
  *
  * ���÷�����������:
  *
  * // ��0��ַ��ʼ������ȡ����. �ڼ䲻�ܶ�SPI FLASH����������.
  * spiflash_read_start(0);
  * // ��512���ֽ�Ϊһ��, ������ȡ����. �˲������ѭ��������,
  * spiflash_read_bytes(data, 512);     // ��ȡ512�����ݵ�data��
  * do_something(data);                 // ���ݴ���
  * spiflash_read_bytes(data, 512);     // �ٶ�ȡ512�����ݵ�data��, ����ѭ��
  * do_something(data);                 // ���ݴ���
  * // ����������ȡ����. �ɶ�SPI FLASH������������.
  * spiflash_read_end();
  ******************************************************************************
  */
void spiflash_read_start(uint32_t read_addr);                                   // ׼��������ȡ
void spiflash_read_bytes(uint8_t *pbuf, uint32_t num);                          // ������ȡ����
void spiflash_read_end(void);                                                   // ����������ȡ

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
