/**
  ******************************************************************************
  * @file    drv_flash.c
  * @author  mousie-yu
  * @version V1.0.0
  * @date    2015.1.9
  * @brief   STM32 �ڲ� Flash ��д��������.
  *          ��Ҫ�趨 Heap ����Ϊ FLASH_PAGE_SIZE+0x100.
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
  * @brief    ˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Private_MacroDefine
  * @brief    ˽�к궨��
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Variables
  * @brief    ����ȫ�ֱ���(˽��/����)
  * @{
  */

/**
  * @}
  */

/** @defgroup DRV_Flash_Private_Function
  * @brief    ����˽�к���
  * @{
  */

/**
  * @}
  */



/** @defgroup DRV_Flash_Function
  * @brief    ����ԭ�ļ�
  * @{
  */

/**
  ******************************************************************************
  * @brief  FLASH����, ��ֹFLASHд��
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
  * @brief  FLASH����, ʹ��FLASHд��, ʹ��flash_writeǰ�������
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
  * @brief  ��һ������д���ڲ� Flash, ��ɺ��Զ���ֹд�빦��
  * @param  pdata,   Ҫд�����ݵ�ָ��
  * @param  addr,    Ҫд�����ݵ�Ŀ���ַ.
  * @param  num,     ���ݳ���
  * @retval ����ֵ,  �Ƿ�д��ɹ�. 0,�ɹ�. 1,ʧ��
  ******************************************************************************
  */
uint8_t flash_write(uint8_t *pdata, uint32_t addr, uint32_t num)
{
    uint8_t rval = 0;
    uint8_t *pbuf;
    uint32_t *pword;
    uint32_t i, addr_page, crc;

    // оƬ�������
    uint32_t flash_size;
    flash_size = (*(uint16_t *)0x1FFFF7E0);                                     // ��ȡFLASH�����Ĵ���
    flash_size <<= 10;                                                          // * 1024��Ϊ��ȷ������ֵ
    assert_param(flash_size >= FLASH_END_ADDRESS - FLASH_ADDRESS);
    if (flash_size < FLASH_END_ADDRESS - FLASH_ADDRESS) rval = 1;

    // д���ַ���
    assert_param(addr >= FLASH_START_ADDRESS);
    assert_param(addr+num <= FLASH_END_ADDRESS);
    if (addr < FLASH_START_ADDRESS) rval = 1;
    if (addr+num > FLASH_END_ADDRESS) rval = 1;

    // ���������ҳ�ĵ�ַ
    addr_page = addr - addr % FLASH_PAGE_SIZE;
    pbuf = (uint8_t *)malloc(FLASH_PAGE_SIZE);
    if (pbuf == NULL) rval = 1;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
    while(rval == 0)
    {
        // ������������ѡ���Ƿ񻺴�FLASH
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

            // ����
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

            // ��¼
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

            // ��ҳУ��, �ɹ�����д��һҳ, ����������д��ҳ
            crc = CRC_GetCRC();
            CRC_ResetDR();
            pword = (uint32_t *)(addr_page-FLASH_PAGE_SIZE);
            if (crc == CRC_CalcBlockCRC(pword, FLASH_PAGE_SIZE >> 2))
                break;
        }

        // ����������¼���
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
  * @brief  �� Flash �ж���һ������.
  * @param  addr,   Ҫ��ȡ����ʼ��ַ.
  * @param  pdata,  Ҫ��ȡ���ݵĴ�ŵ�ַָ��
  * @param  num,    ��Ҫ��ȡ�����ݸ���.
  * @retval ����ֵ, �Ƿ��ȡ�ɹ�. 0,�ɹ�. 1,ʧ��
  ******************************************************************************
  */
uint8_t flash_read(uint8_t *pdata, uint32_t addr, uint32_t num)
{
    // оƬ�������
    uint32_t flash_size;
    flash_size = (*(uint16_t *)0x1FFFF7E0);                                     // ��ȡFLASH�����Ĵ���
    flash_size <<= 10;                                                          // * 1024��Ϊ��ȷ������ֵ
    assert_param(flash_size >= FLASH_END_ADDRESS - FLASH_ADDRESS);
    if (flash_size < FLASH_END_ADDRESS - FLASH_ADDRESS) return(1);

    // ��ȡ��ַ���
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
