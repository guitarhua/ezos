/**
  ******************************************************************************
  * @file    stm32f10x_conf.h
  * @author  mousie
  * @version V2.0.0
  * @date    2011.12.16
  * @brief   ���������ļ�, ����ģ��Ķ�����������ͳһ�ڴ˴�����
  *          ��Ŀ���ƣ� STM32����ģ��, ����ʾ������ģ��.
  *                     �ṩ�����ײ�������Ӧ�÷���, ʹ��ezos.
  *          ������ʹ�� RVMDK V4.20.03
  *          ������ʹ�� ��ţ������
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_CONF_H
#define __STM32F10x_CONF_H

/** @addtogroup Applications
  * @{
  */
/** @addtogroup Configure
  * @{
  */



// ��Ŀ�汾
#define PROJECT_VERSION_MAIN            2
#define PROJECT_VERSION_SUB             4
#define PROJECT_REVISION                0



// �Ƿ�ʹ�ܶ��Ե���, �����Ӵ�����, ��ʹ���򲻽��к궨��
#define ASSERT_ENABLE



// ���Ź�����
//#define IWATCHDOG_ENABLE                                                        ///< ʹ�ܶ������Ź�
#define IWATCHDOG_TIME_MS               5000                                    ///< �������Ź���λ����ʱ��, ��Чʱ��, 100-26000ms

/**
  * ���ڿ��Ź�ʱ��Ҫ�����ϸ�, �밴��ʽ���м��� WWATCHDOG_VALUE �� WWATCHDOG_COUNT ��ֵ.
  * ����ֵΪ WWATCHDOG_VALUE = 0x7F, WWATCHDOG_COUNT = 0x7F, ��Чι��ʱ�䴰��Ϊ0ms - 58ms֮��.
  * ����ϵͳĬ��ʱ�� PCLK1 = 36Mhzʱ, ʱ�䴰��(��λΪms)ֵ���㹫ʽ����:
  * WWDG_MAX_MS = 4096 * 8 * (WWATCHDOG_COUNT - 63) / 36000
  * WWDG_MIN_MS = 4096 * 8 * (WWATCHDOG_COUNT - WWATCHDOG_VALUE) / 36000
  * ��Чι��ʱ�䴰�ڼ�Ϊ WWDG_MIN_MS - WWDG_MAX_MS ֮��, ʱ�䵥λΪms
  */
//#define WWATCHDOG_ENABLE                                                        ///< ʹ�ܴ��ڿ��Ź�
#define WWATCHDOG_COUNT                 0x7F                                    ///< ���ڿ��Ź���������ʼֵ, ��Чֵ, 0x40 - 0x7F
#define WWATCHDOG_VALUE                 0x7F                                    ///< ���ڿ��Ź��������Ƚ�ֵ, ��Чֵ, 0x40 - WWATCHDOG_COUNT



// LEDӲ������
#define LED_NUM                         4                                       ///< ����LED������, �������������16��LED��
#define LED_LEVEL                       1                                       ///< 0, �͵�ƽ����LED. 1, �ߵ�ƽ����LED
#define LED_ADVANCED_ENABLE                                                     ///< ʹ��LED�ĸ߼�����, ������˸�ͽ���Ч��

#define LED0_PORT                       GPIOD                                   ///< ���ø�LED��PORT��
#define LED0_CLK                        RCC_APB2Periph_GPIOD                    ///< ���ø�LED��ʱ��ģ��
#define LED0_PIN                        GPIO_Pin_8                              ///< ���ø�LED��PIN��

#define LED1_PORT                       GPIOD
#define LED1_CLK                        RCC_APB2Periph_GPIOD
#define LED1_PIN                        GPIO_Pin_9

#define LED2_PORT                       GPIOD
#define LED2_CLK                        RCC_APB2Periph_GPIOD
#define LED2_PIN                        GPIO_Pin_10

#define LED3_PORT                       GPIOD
#define LED3_CLK                        RCC_APB2Periph_GPIOD
#define LED3_PIN                        GPIO_Pin_11

typedef enum
{
    LED1 = 0,
    LED2,
    LED3,
    LED4,
} led_name_t;



// ����Ӳ������
#define KEY_NUM                         2                                       ///< ���ð�������, �������������16��ֱ��ʽ����
#define KEY_EXTPULL                                                             ///< �������ⲿ��������

#define KEY0_PORT                       GPIOC                                   ///< ���øð�����PORT��
#define KEY0_CLK                        RCC_APB2Periph_GPIOC                    ///< ���øð�����ʱ��ģ��
#define KEY0_PIN                        GPIO_Pin_13                             ///< ���øð�����PIN��

#define KEY1_PORT                       GPIOA
#define KEY1_CLK                        RCC_APB2Periph_GPIOA
#define KEY1_PIN                        GPIO_Pin_8

typedef enum
{
    S2_TAMPER = 0,
    S4_USER,
} key_name_t;



/**
  ******************************************************************************
  * �ⲿ�ж�����
  * ʹ������: 5���ж�Դ����ʹ�ò�ͬ�� PIN�� �Լ� �жϺ�!!!
  * Ӳ�����: ����ʹ�� PIN0, PIN1, PIN2, PIN3 �� PIN4.
  *           ���� PIN5-PIN9, ֻ��ȡ��һ. ����PIN10-PIN15, Ҳֻ��ȡ��һ.
  *           Ʃ�� PA0, PA1, PA2, PB3, PC4 ������Ϊ5����Ч���ж�Դ.
  *           ��   PA0, PB0, PC0, PD0, PE0 �ǲ���ȡ��, ����޷�������5���ⲿ�ж�.
  ******************************************************************************
  */
#define EXTINT_NUM                      1                                       ///< �����ⲿ�ж�����.�������������5���ⲿ�ж�

#if (EXTINT_NUM > 0)
    #define EXTINT0_PORT                GPIOA                                   ///< �����ⲿ�ж�PORT��
    #define EXTINT0_CLK                 RCC_APB2Periph_GPIOA                    ///< �����ⲿ�ж�PORT��ʱ��ģ��
    #define EXTINT0_PORT_SOURCE         GPIO_PortSourceGPIOA                    ///< �����ⲿ�ж�PORT��Դ
    #define EXTINT0_PIN                 GPIO_Pin_0                              ///< �����ⲿ�ж�PIN��
    #define EXTINT0_PIN_SOURCE          GPIO_PinSource0                         ///< GPIO_Pin_X, ������Ϊ GPIO_PinSourceX
    #define EXTINT0_LINE                EXTI_Line0                              ///< GPIO_Pin_X, ������Ϊ EXTI_LineX

    /**
      * @brief �����ⲿ�жϵ��жϺ�
      *   @arg EXTI0_IRQn,              GPIO_Pin_0 ʹ�ô��жϺ�
      *   @arg EXTI1_IRQn,              GPIO_Pin_1 ʹ�ô��жϺ�
      *   @arg EXTI2_IRQn,              GPIO_Pin_2 ʹ�ô��жϺ�
      *   @arg EXTI3_IRQn,              GPIO_Pin_3 ʹ�ô��жϺ�
      *   @arg EXTI4_IRQn,              GPIO_Pin_4 ʹ�ô��жϺ�
      *   @arg EXTI9_5_IRQn,            GPIO_Pin_5  - GPIO_Pin_9  ʹ�ô��жϺ�
      *   @arg EXTI15_10_IRQn,          GPIO_Pin_10 - GPIO_Pin_15 ʹ�ô��жϺ�
      */
    #define EXTINT0_IRQ                 EXTI0_IRQn
    #define EXTINT0_IRQ_HANDLER         EXTI0_IRQHandler                        ///< �ⲿ�жϺ�����

    /**
      * @brief �����ⲿ�жϻص�����
      *   @arg 0,                       ��ʹ�ûص�����, �����ⲿ�жϽ��д���
      *   @arg void fun(void),          �ص�������ʽ, �궨��fun�������Ƽ���
      */
    extern void int_key(void);
    #define EXTINT0_CALLBACK            int_key
    #define EXTINT0_IRQ_PRIORITY        0xF                                     ///< �����ж����ȼ�. ��2λΪ����ռ�����ȼ�, ��2λΪ����ռ�������ȼ�
#endif

// �ⲿ�ж��������Ͷ���
typedef enum
{
    S3_WAKEUP = 0,
} extint_name_t;



// ��������Ӳ������
#define CTRLPORT_NUM                    2                                       ///< ���ÿ������ŵ�����. �������������16����������.

#define CTRLPORT0_PORT                  GPIOC                                   ///< ���øÿ������ŵ�PORT��
#define CTRLPORT0_CLK                   RCC_APB2Periph_GPIOC                    ///< ���øÿ������ŵ�ʱ��ģ��
#define CTRLPORT0_PIN                   GPIO_Pin_8                              ///< ���øÿ������ŵ�PIN��
#define CTRLPORT0_OFF                   1                                       ///< ���ùرտ������ŵĵ�ƽ

#define CTRLPORT1_PORT                  GPIOB                                   ///< ���øÿ������ŵ�PORT��
#define CTRLPORT1_CLK                   RCC_APB2Periph_GPIOB                    ///< ���øÿ������ŵ�ʱ��ģ��
#define CTRLPORT1_PIN                   GPIO_Pin_5                              ///< ���øÿ������ŵ�PIN��
#define CTRLPORT1_OFF                   0                                       ///< ���ùرտ������ŵĵ�ƽ

typedef enum
{
    SD_POWER = 0,
    BEEP,
} ctrlport_name_t;



// SPIӲ������
#define SPI_NUM                         2                                       ///< ����ʹ�õ�SPI����.�������������5��SPI��

#if  (SPI_NUM > 0)
    /**
      * @brief ���ø�SPI����������
      *   @arg 0,                       ʹ��IO��ģ��SPI
      *   @arg SPI1,                    ʹ��SPI1����, SPI1��CLKΪ RCC_APB2Periph_SPI1
      *   @arg SPI2,                    ʹ��SPI2����, �����CLKΪ RCC_APB1Periph_XXX
      *   @arg SPI3,                    ʹ��SPI3����
      */                                                                        ///< ���ø�SPI��SCK����PORT��
    #define SPI0_PERIPHERAL             SPI2                                    ///< ���ø�SPI��SCK����ʱ��ģ��
    #define SPI0_CLK                    RCC_APB1Periph_SPI2                     ///< ���ø�SPI��SCK����PIN��

    #define SPI0_SCK_PORT               GPIOB                                   ///< ���ø�SPI��MISO����PORT��
    #define SPI0_SCK_CLK                RCC_APB2Periph_GPIOB                    ///< ���ø�SPI��MISO����ʱ��ģ��
    #define SPI0_SCK_PIN                GPIO_Pin_13                             ///< ���ø�SPI��MISO����PIN��

    #define SPI0_MISO_PORT              GPIOB                                   ///< ���ø�SPI��MOSI����PORT��
    #define SPI0_MISO_CLK               RCC_APB2Periph_GPIOB                    ///< ���ø�SPI��MOSI����ʱ��ģ��
    #define SPI0_MISO_PIN               GPIO_Pin_14                             ///< ���ø�SPI��MOSI����PIN��

    #define SPI0_MOSI_PORT              GPIOB
    #define SPI0_MOSI_CLK               RCC_APB2Periph_GPIOB
    #define SPI0_MOSI_PIN               GPIO_Pin_15

    /**                                 0
      * @brief ���ø�SPI��������ӳ�书��. ��ʹ�õ�IO���й�.
      *   @arg 0,                       ��ʹ��������ӳ�书��
      *   @arg GPIO_Remap_SPI1,         SPI1 ������ӳ��
      *   @arg GPIO_Remap_SPI3,         SPI3 ������ӳ��
      */
    #define SPI0_GPIO_REMAP             0
#endif

#if  (SPI_NUM > 1)
    #define SPI1_PERIPHERAL             SPI1
    #define SPI1_CLK                    RCC_APB2Periph_SPI1

    #define SPI1_SCK_PORT               GPIOA
    #define SPI1_SCK_CLK                RCC_APB2Periph_GPIOA
    #define SPI1_SCK_PIN                GPIO_Pin_5

    #define SPI1_MISO_PORT              GPIOA
    #define SPI1_MISO_CLK               RCC_APB2Periph_GPIOA
    #define SPI1_MISO_PIN               GPIO_Pin_6

    #define SPI1_MOSI_PORT              GPIOA
    #define SPI1_MOSI_CLK               RCC_APB2Periph_GPIOA
    #define SPI1_MOSI_PIN               GPIO_Pin_7

    #define SPI1_GPIO_REMAP             0
#endif



// SPI�������Ͷ���
typedef enum
{
    SD_SPI = 0,
    TOUCH_SPI,
} spi_name_t;



// SD��Ӳ������
#define SDCARD_ENABLE                                                           ///< �Ƿ�ʹ��SD������

#define SDCARD_POWER_NAME               SD_POWER                                ///< SD����Դ������������, û�е�Դ���Ʋ�Ҫ�궨�����
#define SDCARD_DETECT_ENABLE                                                    ///< һ�㿨������, ���ʹ�ܴ���, ������SD������
#define SDCARD_DETECT_PORT              GPIOA                                   ///< SD����CD����, SD�����ڼ��. PORT��
#define SDCARD_DETECT_CLK               RCC_APB2Periph_GPIOA                    ///< SD����CD����, SD�����ڼ��. ʱ��ģ��
#define SDCARD_DETECT_PIN               GPIO_Pin_4                              ///< SD����CD����, SD�����ڼ��. PIN��

// SD ͨѶģʽ�궨��, ע��ֻ��ѡһ��
//#define SDCARD_MODE_SDIO                                                        ///< SD��ʹ��SDIOģʽͨѶ
//#define SDCARD_MODE_SPIDMA                                                      ///< SD��ʹ��SPI DMAģʽͨѶ
#define SDCARD_MODE_SPI                                                         ///< SD��ʹ��SPIģʽͨѶ

#ifdef SDCARD_MODE_SDIO
    #define SDIO_IRQ_PRIORITY           0x0                                     ///< �����ж����ȼ�. ��2λΪ����ռ�����ȼ�, ��2λΪ����ռ�������ȼ�
#endif

#ifdef SDCARD_MODE_SPIDMA                                                       ///< SPI DMAģʽ����
    #define SD_SPI_X                    2                                       ///< ѡ��Ӳ��SPI��, 1��ʾSPI1, 2��ʾSPI2
    #define SD_CS_PORT                  GPIOB                                   ///< SD��Ƭѡ�źŵ�PORT��
    #define SD_CS_CLK                   RCC_APB2Periph_GPIOB                    ///< SD��Ƭѡ�źŵ�ʱ��ģ��
    #define SD_CS_PIN                   GPIO_Pin_12                             ///< SD��Ƭѡ�źŵ�PIN��
#endif

#ifdef SDCARD_MODE_SPI                                                          ///< SPIģʽ����
    #define SD_SPI_NAME                 SD_SPI                                  ///< ��������ʹ��һ��SPI, SPI��SCK, MISO, MOSI����"drv_spi.h"������
    #define SD_CS_PORT                  GPIOB                                   ///< SD��Pin1, Ƭѡ�źŵ�PORT��
    #define SD_CS_CLK                   RCC_APB2Periph_GPIOB                    ///< SD��Pin1, Ƭѡ�źŵ�ʱ��ģ��
    #define SD_CS_PIN                   GPIO_Pin_12                             ///< SD��Pin1, Ƭѡ�źŵ�PIN��
#endif



/// SD FLASH Ӳ������
//#define SDFLASH_ENABLE                                                          ///< �Ƿ�ʹ��SD FLASH����

//#define SDFLASH_POWER_NAME              SDFLASH_POWER                           ///< SD FLASH����Դ������������, û�е�Դ���Ʋ�Ҫ�궨�����
#define SDFLASH_DETECT_ENABLE                                                   ///< һ�㿨�����ɼ��SD���Ƿ����
#define SDFLASH_DETECT_PORT             GPIOC                                   ///< SD FLASH����CD����, SD�����ڼ��. PORT��
#define SDFLASH_DETECT_CLK              RCC_APB2Periph_GPIOC                    ///< SD FLASH����CD����, SD�����ڼ��. ʱ��ģ��
#define SDFLASH_DETECT_PIN              GPIO_Pin_11                             ///< SD FLASH����CD����, SD�����ڼ��. PIN��

// SD FLASH ͨѶģʽ�궨��, ע��ֻ��ѡһ��
//#define SDFLASH_MODE_SDIO                                                       ///< SD FLASHʹ��SDIOģʽͨѶ
//#define SDFLASH_MODE_SPIDMA                                                     ///< SD FLASHʹ��SPI DMAģʽͨѶ
//#define SDFLASH_MODE_SPI                                                        ///< SD FLASHʹ��SPIģʽͨѶ

#ifdef SDFLASH_MODE_SDIO
    #define SDIO_IRQ_PRIORITY           0x0                                     ///< �����ж����ȼ�. ��2λΪ����ռ�����ȼ�, ��2λΪ����ռ�������ȼ�
#endif

#ifdef SDFLASH_MODE_SPIDMA                                                      ///< SPI DMAģʽ����
    #define SD_SPI_X                    2                                       ///< ѡ��Ӳ��SPI��, 1��ʾSPI1, 2��ʾSPI2
    #define SD_CS_PORT                  GPIOB                                   ///< SD��Ƭѡ�źŵ�PORT��
    #define SD_CS_CLK                   RCC_APB2Periph_GPIOB                    ///< SD��Ƭѡ�źŵ�ʱ��ģ��
    #define SD_CS_PIN                   GPIO_Pin_12                             ///< SD��Ƭѡ�źŵ�PIN��
#endif

#ifdef SDFLASH_MODE_SPI                                                         ///< SPIģʽ����
    #define SD_SPI_NAME                 SD_SPI                                  ///< ��������ʹ��һ��SPI, SPI��SCK, MISO, MOSI����"drv_spi.h"������
    #define SD_CS_PORT                  GPIOC                                   ///< SD��Ƭѡ�źŵ�PORT��
    #define SD_CS_CLK                   RCC_APB2Periph_GPIOC                    ///< SD��Ƭѡ�źŵ�ʱ��ģ��
    #define SD_CS_PIN                   GPIO_Pin_11                             ///< SD��Ƭѡ�źŵ�PIN��
#endif



// I2CӲ������
#define I2C_NUM                         2                                       ///< ����ʹ�õ�I2C����.�������������5��I2C��

#if  (I2C_NUM > 0)
    /**
      * @brief ���ø�I2C����������
      *   @arg 0,                       ʹ��IO��ģ��SPI
      *   @arg I2C1,                    ʹ��I2C1����
      *   @arg I2C2,                    ʹ��I2C2����
      */
    #define I2C0_PERIPHERAL             I2C1                                    ///< ���ø�I2C��ģ����,   ��������ģ��I2C, ��Ϊ0
    #define I2C0_CLK                    RCC_APB1Periph_I2C1                     ///< ���ø�I2C��ʱ��ģ��, ��������ģ��I2C, ��Ϊ0

    #define I2C0_SCL_PORT               GPIOB                                   ///< ���ø�I2C��SCL���ŵ�PORT��
    #define I2C0_SCL_CLK                RCC_APB2Periph_GPIOB                    ///< ���ø�I2C��SCL���ŵ�ʱ��ģ��
    #define I2C0_SCL_PIN                GPIO_Pin_6                              ///< ���ø�I2C��SCL���ŵ�PIN��

    #define I2C0_SDA_PORT               GPIOB                                   ///< ���ø�I2C��SDA���ŵ�PORT��
    #define I2C0_SDA_CLK                RCC_APB2Periph_GPIOB                    ///< ���ø�I2C��SDA���ŵ�ʱ��ģ��
    #define I2C0_SDA_PIN                GPIO_Pin_7                              ///< ���ø�I2C��SDA���ŵ�PIN��

    /**
      * @brief ����I2C������ӳ�书��. ������Ϣ��ο��ٷ������ֲ�.
      *        ��I2C��ص���������:
      *   @arg 0,                       ��ʹ��������ӳ�书��
      *   @arg GPIO_Remap_I2C1,         I2C1 ������ӳ��
      */
    #define I2C0_GPIO_REMAP             0
#endif

#if  (I2C_NUM > 1)
    #define I2C1_PERIPHERAL             0
    #define I2C1_CLK                    0

    #define I2C1_SCL_PORT               GPIOB
    #define I2C1_SCL_CLK                RCC_APB2Periph_GPIOB
    #define I2C1_SCL_PIN                GPIO_Pin_6

    #define I2C1_SDA_PORT               GPIOB
    #define I2C1_SDA_CLK                RCC_APB2Periph_GPIOB
    #define I2C1_SDA_PIN                GPIO_Pin_7

    #define I2C1_GPIO_REMAP             0
#endif

typedef enum
{
  COMMON_I2C_HW = 0,
  COMMON_I2C_SW = 1,
} i2c_name_t;



// EEPROMӲ������
#define EEPROM_I2C_NAME                 COMMON_I2C_HW                           ///< ��������ʹ��һ��I2C, I2C��SCL, SDA. ��"drv_i2c.h"������
#define EEPROM_TYPE_24C                 2                                       ///< �趨EEPROMоƬ�ͺ�. 1��ʾ24C01, 64��ʾ24C64/24C65, 1024��ʾ24C1024, ����



// UARTӲ������
#define UART_NUM                        2                                       ///< ����ʹ�õ�Uart����.�������������5��Uart��

//#define UART_TX_POLLING_MODE                                                    ///< UART ������ѯģʽ, ��ΪĬ��ֵ.
#define UART_TX_INT_BUF_MODE                                                    ///< UART �����жϻ�����ģʽ

//#define UART_RX_POLLING_MODE                                                    ///< UART ������ѯģʽ, ��ΪĬ��ֵ.
#define UART_RX_INT_BUF_MODE                                                    ///< UART �����жϻ�����ģʽ,  ���� UART_RX_INT_FUN_MODE ����
//#define UART_RX_INT_FUN_MODE                                                    ///< UART �����жϻص�����ģʽ,���� UART_RX_INT_BUF_MODE ����

#if (UART_NUM > 0)
    /**
      * @brief ���øô��ڵ���������
      *   @arg USART1,                  ʹ��USART1����, USART1��CLKΪ RCC_APB2Periph_USART1
      *   @arg USART2,                  ʹ��USART2����,  ���� ��CLKΪ RCC_APB1Periph_XXX
      *   @arg USART3,                  ʹ��USART3����
      *   @arg UART4,                   ʹ��UART4 ����
      *   @arg UART5,                   ʹ��UART5 ����
      */
    #define UART0_PERIPHERAL            USART1
    #define UART0_IRQ                   USART1_IRQn                             ///< ���øô��ڵ��ж�
    #define UART0_IRQ_HANDLER           USART1_IRQHandler                       ///< ���øô��ڵ��жϺ���
    #define UART0_CLK                   RCC_APB2Periph_USART1                   ///< ���øô��ڵ�ʱ��ģ��

    #define UART0_TX_PORT               GPIOA                                   ///< ���øô��ڵ��������PORT��
    #define UART0_TX_CLK                RCC_APB2Periph_GPIOA                    ///< ���øô��ڵ��������ʱ��ģ��
    #define UART0_TX_PIN                GPIO_Pin_9                              ///< ���øô��ڵ��������PIN��

    #define UART0_RX_PORT               GPIOA                                   ///< ���øô��ڵ���������PORT��
    #define UART0_RX_CLK                RCC_APB2Periph_GPIOA                    ///< ���øô��ڵ���������ʱ��ģ��
    #define UART0_RX_PIN                GPIO_Pin_10                             ///< ���øô��ڵ���������PIN��

    /**
      * @brief ���øô��ڵ�������ӳ�书��. ��ʹ�õ�IO���й�.
      *   @arg 0,                       ��ʹ��������ӳ�书��
      *   @arg GPIO_Remap_USART1,       USART1 ������ӳ��
      *   @arg GPIO_Remap_USART2,       USART2 ������ӳ��
      *   @arg GPIO_PartialRemap_USART3,USART3 ����������ӳ��
      *   @arg GPIO_FullRemap_USART3,   USART3 ȫ��������ӳ��
      */
    #define UART0_GPIO_REMAP            0

    /**
      * @brief ���øô��ڵĽ����жϻص�����
      *   @arg 0,                       ��ʹ�ûص�����, ���Խ������ݽ��д���
      *   @arg void fun(uint8_t data),  �ص�������ʽ, �궨��fun�������Ƽ���
      */
    #define UART0_RX_CALLBACK           0
    #define UART0_IRQ_PRIORITY          0xF                                     ///< �����ж����ȼ�. ��2λΪ����ռ�����ȼ�, ��2λΪ����ռ�������ȼ�
#endif

#if (UART_NUM > 1)
    #define UART1_PERIPHERAL            USART2
    #define UART1_IRQ                   USART2_IRQn
    #define UART1_IRQ_HANDLER           USART2_IRQHandler
    #define UART1_CLK                   RCC_APB1Periph_USART2

    #define UART1_TX_PORT               GPIOA
    #define UART1_TX_CLK                RCC_APB2Periph_GPIOA
    #define UART1_TX_PIN                GPIO_Pin_2

    #define UART1_RX_PORT               GPIOA
    #define UART1_RX_CLK                RCC_APB2Periph_GPIOA
    #define UART1_RX_PIN                GPIO_Pin_3

    #define UART1_GPIO_REMAP            0

    #define UART1_RX_CALLBACK           0
    #define UART1_IRQ_PRIORITY          0xF
#endif

// �����������Ͷ���
typedef enum
{
    COM1 = 0,
    COM2 = 1,
} uart_name_t;



/// ��ʱ������
#define TIMER_NUM                       1                                      ///< ����ʹ�õ�Timer����.�������������3��Timer��

#if (TIMER_NUM > 0)
    /**
      * @brief ���ø�Timer����������. TIM1Ϊ�߼���ʱ��, ��������֧��ʹ��TIM1!!!
      *   @arg TIM2,                    ʹ��TIM2����, CLKΪ RCC_APB1Periph_TIMx
      *   @arg TIM3,                    ʹ��TIM3����
      *   @arg TIM4,                    ʹ��TIM4����
      *   @arg TIM5,                    ʹ��TIM5����
      *   @arg TIM6,                    ʹ��TIM6����
      *   @arg TIM7,                    ʹ��TIM7����
      */
    #define TIMER0_PERIPHERAL           TIM2
    #define TIMER0_IRQ                  TIM2_IRQn                               ///< ���ø�Timer���ж�
    #define TIMER0_IRQ_HANDLER          TIM2_IRQHandler                         ///< ���ø�Timer���жϺ���
    #define TIMER0_CLK                  RCC_APB1Periph_TIM2                     ///< ���ø�Timer��ʱ��ģ��

    /**
      * @brief ���ø�Timer������жϻص�����
      *   @arg 0,                       ��ʹ�ûص�����, ���Խ������ݽ��д���
      *   @arg void fun(void),          �ص�������ʽ, �궨��fun�������Ƽ���
      */
    extern void tone_scan_10us(void);
    #define TIMER0_OVERTIME_CALLBACK    tone_scan_10us
    #define TIMER0_IRQ_PRIORITY         0xF                                     ///< �����ж����ȼ�. ��2λΪ����ռ�����ȼ�, ��2λΪ����ռ�������ȼ�
#endif

/// ��ʱ���������Ͷ���
typedef enum
{
    TIMER_10US = 0,                                                             ///< ����Ϊ��������Timer����, ֵ0��ӦTIMER0_xxx
} timer_name_t;



/// �������ʹ��
#define TONE_ENABLE
#define TONE_OUT                        BEEP                                    ///< ��������IO��, ��"drv_ctrlport"�ṩ����



#include "assert.h"                                                             ///< ����ͷ�ļ�, ������ڸ��ļ���ĩβ

/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
