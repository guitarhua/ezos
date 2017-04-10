/**
 ******************************************************************************
 * @file    drv_adc1.c
 * @author  mousie
 * @version V1.0.0
 * @date    2014.4.22
 * @brief   ADC基础功能, 此驱动仅支持ADC1 DMA模式单通道采样
 ******************************************************************************
 */

/* Include -------------------------------------------------------------------*/
#include "drv_adc1.h"

/** @addtogroup Drivers
 * @{
 */
/** @defgroup DRV_ADC1
 * @{
 */
#if (ADC1_NUM > 0)



/**
 ******************************************************************************
 * @addtogroup DRV_ADC1_Configure
 ******************************************************************************
 * @{
 */
// ADC时钟设置, 最高允许18Mhz. 对PCLK2(最高72M)的分频, div2, 4, 6, 8
#define ADC_CLK_DIVISION        RCC_PCLK2_Div4

// 设置采样周期
#define ADC_SAMPLE_TIME         ADC_SampleTime_55Cycles5
/**
 * @}
 */



/** @defgroup DRV_ADC1_Private_TypeDefine
 * @brief    私有类型定义
 * @{
 */
/// ADC1硬件参数类型定义
typedef struct
{
    GPIO_TypeDef *const PORT;                                                   ///< 通道PORT口
    const uint32_t CLK;                                                         ///< 通道PORT口时钟源
    const uint16_t PIN;                                                         ///< 通道PIN口
    const uint8_t  CHANNEL;                                                     ///< 通道号
} adc1_hw_t;
/**
 * @}
 */

/** @defgroup DRV_ADC1_Private_MacroDefine
 * @brief    私有宏定义
 * @{
 */

/**
 * @}
 */

/** @defgroup DRV_ADC1_Variables
 * @brief    定义全局变量(私有/公有)
 * @{
 */
/// ADC1硬件参数映射表
static const adc1_hw_t adc1_hw[ADC1_NUM] = {
#if (ADC1_NUM > 0)
    {CHANNEL0_PORT, CHANNEL0_CLK, CHANNEL0_PIN, CHANNEL0_CHANNEL},
#endif
#if (ADC1_NUM > 1)
    {CHANNEL1_PORT, CHANNEL1_CLK, CHANNEL1_PIN, CHANNEL1_CHANNEL},
#endif
#if (ADC1_NUM > 2)
    {CHANNEL2_PORT, CHANNEL2_CLK, CHANNEL2_PIN, CHANNEL2_CHANNEL},
#endif
#if (ADC1_NUM > 3)
    {CHANNEL3_PORT, CHANNEL3_CLK, CHANNEL3_PIN, CHANNEL3_CHANNEL},
#endif
#if (ADC1_NUM > 4)
    {CHANNEL4_PORT, CHANNEL4_CLK, CHANNEL4_PIN, CHANNEL4_CHANNEL},
#endif
#if (ADC1_NUM > 5)
    {CHANNEL5_PORT, CHANNEL5_CLK, CHANNEL5_PIN, CHANNEL5_CHANNEL},
#endif
#if (ADC1_NUM > 6)
    {CHANNEL6_PORT, CHANNEL6_CLK, CHANNEL6_PIN, CHANNEL6_CHANNEL},
#endif
#if (ADC1_NUM > 7)
    {CHANNEL7_PORT, CHANNEL7_CLK, CHANNEL7_PIN, CHANNEL7_CHANNEL},
#endif
};

volatile uint16_t ADC1ConvertedValue[ADC1_NUM];
/**
 * @}
 */

/** @defgroup DRV_ADC1_Private_Function
 * @brief    定义私有函数
 * @{
 */

/**
 * @}
 */

/** @defgroup DRV_ADC1_Function
 * @brief    函数原文件
 * @{
 */

/**
 ******************************************************************************
 * @brief  ADC1 初始化
 * @param  none
 * @retval none
 ******************************************************************************
 */
void adc1_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t i;

    RCC_ADCCLKConfig(ADC_CLK_DIVISION); 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    for (i=0; i<ADC1_NUM; i++) {
        ADC1ConvertedValue[i] = 0xFFFF;
        if ((adc1_hw[i].CLK != 0) && (adc1_hw[i].PORT != 0)) {
            GPIO_InitStructure.GPIO_Pin  = adc1_hw[i].PIN;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
            GPIO_Init(adc1_hw[i].PORT, &GPIO_InitStructure);
        }
    }

    // DMA1 channel1 configuration
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC1ConvertedValue;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC1_NUM;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE);

    // ADC1 configuration
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = ADC1_NUM;
    ADC_Init(ADC1, &ADC_InitStructure);

    for (i=0; i<ADC1_NUM; i++) {
        ADC_RegularChannelConfig(ADC1, adc1_hw[i].CHANNEL, i+1, ADC_SAMPLE_TIME);
    }

    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));

    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


/**
 ******************************************************************************
 * @brief  ADC1 重置
 * @param  none
 * @retval none
 ******************************************************************************
 */
void adc1_deinit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t i;

    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    ADC_DeInit(ADC1);
    DMA_DeInit(DMA1_Channel1);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);

    for (i=0; i<ADC1_NUM; i++) {
        if ((adc1_hw[i].CLK != 0) && (adc1_hw[i].PORT != 0)) {
            RCC_APB2PeriphClockCmd(adc1_hw[i].CLK, ENABLE);
            GPIO_InitStructure.GPIO_Pin  = adc1_hw[i].PIN;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            GPIO_Init(adc1_hw[i].PORT, &GPIO_InitStructure);
        }
    }
}

/**
 ******************************************************************************
 * @brief  ADC1 启动
 * @param  none
 * @retval none
 ******************************************************************************
 */
void adc1_start(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**
 ******************************************************************************
 * @brief  ADC1 停止
 * @param  none
 * @retval none
 ******************************************************************************
 */
void adc1_stop(void)
{
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
}

/**
 ******************************************************************************
 * @brief  获取 ADC1 特定通道的采样值
 * @param  name, 通道名称
 * @retval 采样值
 ******************************************************************************
 */
uint16_t adc1_get_value(adc1_name_t name)
{
    return ADC1ConvertedValue[name];
}

/**
 ******************************************************************************
 * @brief  获取 ADC1 特定通道的采样值
 * @param  name, 通道名称
 * @retval 采样值
 ******************************************************************************
 */
void adc1_clear_value(void)
{
    uint8_t i;
    for (i=0; i<ADC1_NUM; i++)
        ADC1ConvertedValue[i] = 0xFFFF;
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
