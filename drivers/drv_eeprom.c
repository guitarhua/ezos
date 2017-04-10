/**
 ******************************************************************************
 * @file    drv_eeprom.c
 * @author  mousie-yu
 * @version V2.0.0
 * @date    2011.12.16
 * @brief   24Cxxϵ�� EEPROM ��������.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "drv_eeprom.h"



/** @addtogroup Drivers
 * @{
 */
/** @defgroup DRV_EEPROM
 * @{
 */
#if (EEPROM_TYPE_24C > 0)



/**
 ******************************************************************************
 * @addtogroup DRV_EEPROM_Configure
 ******************************************************************************
 * @{
 */
#define eep_i2c_init()               i2c_init(EEPROM_I2C_NAME, 400)             ///< eeprom��ʼ�������궨��
#define eep_i2c_start()              i2c_start(EEPROM_I2C_NAME)                 ///< eeprom I2C ��ʼ�źź궨��
#define eep_i2c_stop()               i2c_stop(EEPROM_I2C_NAME)                  ///< eeprom I2C �����źź궨��
#define eep_i2c_tx_addr_wr_eep(addr) i2c_tx_addr_wr_slaver(EEPROM_I2C_NAME, addr) ///< eeprom I2C д�źź궨��
#define eep_i2c_tx_addr_rd_eep(addr) i2c_tx_addr_rd_slaver(EEPROM_I2C_NAME, addr) ///< eeprom I2C ���źź궨��
#define eep_i2c_tx_byte(data)        i2c_tx_byte(EEPROM_I2C_NAME, data)         ///< eeprom I2C �����ֽں궨��
#define eep_i2c_rx_byte(ack)         i2c_rx_byte(EEPROM_I2C_NAME, ack)          ///< eeprom I2C �����ֽں궨��

#include "drv_systick.h"
#define eep_delay_ms(time_ms)        delay_ms(time_ms)                          ///< ������ʱ����
/**
 * @}
 */



/** @defgroup DRV_EEPROM_Private_TypeDefine
 * @brief    ˽�����Ͷ���
 * @{
 */

/**
 * @}
 */

/** @defgroup DRV_EEPROM_Private_MacroDefine
 * @brief    ˽�к궨��
 * @{
 */
#if   (EEPROM_TYPE_24C == 1  )
    #define EEP_PAGE_SIZE 8                                                     ///< eeprom��ҳ��С
    #define EEP_CHIP_SIZE 0x0080                                                ///< eeprom������
#elif (EEPROM_TYPE_24C == 2  )
    #define EEP_PAGE_SIZE 8
    #define EEP_CHIP_SIZE 0x0100
#elif (EEPROM_TYPE_24C == 4  )
    #define EEP_PAGE_SIZE 16
    #define EEP_CHIP_SIZE 0x0200
#elif (EEPROM_TYPE_24C == 8  )
    #define EEP_PAGE_SIZE 16
    #define EEP_CHIP_SIZE 0x0400
#elif (EEPROM_TYPE_24C == 16 )
    #define EEP_PAGE_SIZE 16
    #define EEP_CHIP_SIZE 0x0800
#elif (EEPROM_TYPE_24C == 32 )
    #define EEP_PAGE_SIZE 32
    #define EEP_CHIP_SIZE 0x1000
#elif (EEPROM_TYPE_24C == 64 )
    #define EEP_PAGE_SIZE 32
    #define EEP_CHIP_SIZE 0x2000
#elif (EEPROM_TYPE_24C == 128)
    #define EEP_PAGE_SIZE 64
    #define EEP_CHIP_SIZE 0x4000
#elif (EEPROM_TYPE_24C == 256)
    #define EEP_PAGE_SIZE 64
    #define EEP_CHIP_SIZE 0x8000
#elif (EEPROM_TYPE_24C == 512)
    #define EEP_PAGE_SIZE 128
    #define EEP_CHIP_SIZE 0x10000
#elif (EEPROM_TYPE_24C == 1024)
    #define EEP_PAGE_SIZE 256
    #define EEP_CHIP_SIZE 0x20000
#else /* if   (EEPROM_TYPE_24C == 1  ) */
    #error "Set EEPROM_TYPE_24C error!!!"
#endif /* if   (EEPROM_TYPE_24C == 1  ) */
/**
 * @}
 */

/** @defgroup DRV_EEPROM_Variables
 * @brief    ����ȫ�ֱ���(˽��/����)
 * @{
 */
static uint8_t chip_addr_a2a1a0 = 0;                                            ///< 24CxxƬѡ��ַ
/**
 * @}
 */

/** @defgroup DRV_EEPROM_Private_Function
 * @brief    ����˽�к���
 * @{
 */
// eeprom��ҳд
static uint8_t eeprom_write_page(uint8_t * psrc, uint32_t addr_dst, uint32_t num);
/**
 * @}
 */



/** @defgroup DRV_EEPROM_Function
 * @brief    ����ԭ�ļ�
 * @{
 */

/**
 ******************************************************************************
 * @brief  EEPROM��ʼ������, ������EEPROM��ַ
 * @param  none
 * @retval none
 ******************************************************************************
 */
void eeprom_init(uint8_t a2a1a0){
    eep_i2c_init();
    eeprom_set_chip_addr(a2a1a0);
}

/**
 ******************************************************************************
 * @brief  ����24Cxx EEPROM�ĵ�ַ(A2,A1,A0)
 * @param  a2a1a0,  EEPROM ��Χ���� A2A1A0 ��ֵ. ��Ӧbit2 bit1 bit0
 * @retval none
 ******************************************************************************
 */
void eeprom_set_chip_addr(uint8_t a2a1a0){
    chip_addr_a2a1a0 = ((a2a1a0 & 0x07) << 1);
}

/**
 ******************************************************************************
 * @brief  ������д�� 24Cxx ��ҳ��.
 * @param  psrc,     λ�ڵ�Ƭ�������ݵ�Դ��ַ.
 * @param  addr_dst, ������д��EEPROM�е�Ŀ����ַ.
 * @param  num,      ��Ҫд�������ݸ���.
 * @retval ����ֵ,   �Ƿ�д���ɹ�. 0,�ɹ�. 1,ʧ��
 ******************************************************************************
 */
uint8_t eeprom_write_page(uint8_t * psrc, uint32_t addr_dst, uint32_t num){
    uint8_t temp = 0;

    assert_param((addr_dst + num) <= EEP_CHIP_SIZE);

#if  ((EEPROM_TYPE_24C == 4) || (EEPROM_TYPE_24C == 8) || (EEPROM_TYPE_24C == 16))
    temp = addr_dst >> 8;
    temp <<= 1;
#elif (EEPROM_TYPE_24C == 1024)
    temp = addr_dst >> 16;
    temp <<= 1;
#endif

    eep_i2c_start();                                                            // ������ʼ�ź�

    // ����������ַ��д����
    if (eep_i2c_tx_addr_wr_eep(0xA0 | temp | chip_addr_a2a1a0) == I2C_ACK) {
    #if (EEPROM_TYPE_24C >= 32)                                                 // 24C32��������
        eep_i2c_tx_byte((addr_dst >> 8) & 0xFF);                                // ����Ҫд����ַ�ĸ�λ
    #endif
        eep_i2c_tx_byte(addr_dst & 0xFF);                                       // ����Ҫд����ַ�ĵ�λ
        temp = 0;                                                               // �����Ƿ��ɹ��ı���
        while (num--) {                                                         // ����ǰҳ��д��һ������
            if (eep_i2c_tx_byte(*psrc++) == I2C_NO_ACK) {
                temp = 1;                                                       // ����Ӧ������, д��ʧ��
                break;
            }
        }
    } else temp = 1;                                                            // ����ҳʧ�ܱ���
    eep_i2c_stop();                                                             // ���ͽ����ź�
    eep_delay_ms(10);                                                           // ��ʱ10ms

    return(temp);
} /* eeprom_write_page */

/**
 ******************************************************************************
 * @brief  ��һ������д�� 24Cxx
 * @param  pdata,   Ҫд�����ݵ�ָ��
 * @param  addr,    Ҫд�����ݵ�Ŀ����ַ.
 * @param  num,     ���ݳ���
 * @retval ����ֵ,  �Ƿ�д���ɹ�. 0,�ɹ�. 1,ʧ��
 ******************************************************************************
 */
uint8_t eeprom_write(uint8_t * pdata, uint32_t addr, uint32_t num){
    uint32_t temp = 0;

    // д�������ݵ�ַ���ܳ�����ǰ EEPROM ��Ч��Χ
    assert_param((addr + num) <= EEP_CHIP_SIZE);
    if ((addr + num) > EEP_CHIP_SIZE) return(1);

    temp = addr / EEP_PAGE_SIZE;                                                // ȷ��Ŀ����ַ������ַ�Ĳ�
    if (temp) temp = (uint32_t)EEP_PAGE_SIZE * (temp + 1) - addr;
    else      temp = EEP_PAGE_SIZE - addr;

    // �������ݿ鶼��һ��ҳ��, ��ֱ��д��num������
    if (temp >= num) {
        if (eeprom_write_page(pdata, addr, num)) return(1);                     // д���ݳ����򷵻�
    }
    // ���ݿ���ɢ�ڶ���ҳ��, ��Ҫ��ҳд������
    else {
        // ��д�� addr ���ڵ����ݿ�β��ַ�� addr ֮��������
        if (eeprom_write_page(pdata, addr, temp)) return(1);                    // д���ݳ����򷵻�
        pdata += temp;                                                          // ����ʣ�����ݵ�Դ��ַ
        addr  += temp;                                                          // ����ʣ�����ݵ�Ŀ����ַ
        num   -= temp;                                                          // ����ʣ������
        // Ȼ������ҳ������д�� EEPROM ��
        while ( num >= EEP_PAGE_SIZE) {
            if (eeprom_write_page(pdata, addr, EEP_PAGE_SIZE)) return(1);
            pdata += EEP_PAGE_SIZE;                                             // ����ʣ�����ݵ�Դ��ַ
            addr  += EEP_PAGE_SIZE;                                             // ����ʣ�����ݵ�Ŀ����ַ
            num   -= EEP_PAGE_SIZE;                                             // ����ʣ������
        }
        // ������δ��һҳ������д�� EEPROM ��
        if (num) {
            if (eeprom_write_page(pdata, addr, num)) return(1);                 // д���ݳ����򷵻�
        }
    }

    return(0);                                                                  // ȫ��д���ɹ�, ����ֵ 0
} /* eeprom_write */

/**
 ******************************************************************************
 * @brief  �� 24Cxx �ж���һ������.
 * @param  addr,   Ҫ��ȡ���ݵ���ʼ��ַ.
 * @param  pdata,  Ҫ��ȡ���ݵĴ��ŵ�ַָ��
 * @param  num,    ��Ҫ��ȡ�����ݸ���.
 * @retval ����ֵ, �Ƿ���ȡ�ɹ�. 0,�ɹ�. 1,ʧ��
 ******************************************************************************
 */
uint8_t eeprom_read(uint8_t * pdata, uint32_t addr, uint32_t num){
    uint8_t temp = 0;

    // Ҫ���������ݵ�ַ���ܳ�����ǰ EEPROM ��Ч��Χ
    assert_param((addr + num) <= EEP_CHIP_SIZE);
    if ((addr + num ) > EEP_CHIP_SIZE) return(1);

#if ((EEPROM_TYPE_24C == 4) || (EEPROM_TYPE_24C == 8) || (EEPROM_TYPE_24C == 16))
    temp = addr >> 8;
    temp <<= 1;
#elif (EEPROM_TYPE_24C == 1024)
    temp = addr >> 16;
    temp <<= 1;
#endif

    eep_i2c_start();                                                            // ������ʼ�ź�
    // ����������ַ��д����
    if (eep_i2c_tx_addr_wr_eep(0xA0 | temp | chip_addr_a2a1a0) == I2C_ACK) {
    #if (EEPROM_TYPE_24C >= 32)                                                 // 24C32��������
        eep_i2c_tx_byte((addr >> 8) & 0xFF);                                    // ����Ҫ�������ݵĸ�λ��ַ
    #endif
        eep_i2c_tx_byte(addr & 0xFF);                                           // ����Ҫ�������ݵĵ�λ��ַ
        eep_i2c_start();                                                        // �ٴ���ʼ�ź�
        eep_i2c_tx_addr_rd_eep(0xA0 | temp | chip_addr_a2a1a0);                 // ����������ַ��������
        while (num-- > 1)                                                       // ����һ������

            *pdata++ = eep_i2c_rx_byte(I2C_ACK);
        *pdata = eep_i2c_rx_byte(I2C_NO_ACK);                                   // ����һ���ֽڲ�Ҫ��Ӧ���ź�
        temp = 0;                                                               // ��ȡ�ɹ�����
    } else temp = 1;                                                            // ��ȡʧ�ܱ���
    eep_i2c_stop();                                                             // ���ͽ����ź�

    return(temp);
} /* eeprom_read */

/**
 * @}
 */



#endif /* if (EEPROM_TYPE_24C > 0) */
/**
 * @}
 */
/**
 * @}
 */

/* END OF FILE ---------------------------------------------------------------*/
