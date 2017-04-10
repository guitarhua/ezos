/**
  ******************************************************************************
  * @file    lcd_bus_fsmc.h
  * @author  mousie-yu
  * @version V2.2.0
  * @date    2013.10.10
  * @brief   LCD 16位并行通讯总线, intel模式, fsmc驱动
  *          并入 emwin, 增减若干函数.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_BUS_FSMC_H
#define __LCD_BUS_FSMC_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f10x.h"                                                          // __INLINE

#include "stm32f10x_fsmc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



/** @addtogroup Components
  * @{
  */
/** @addtogroup CPN_GUI
  * @{
  */



/**
  * FSMC LCD 的数据及寄存器地址宏定义
  * 选择BANK1-BORSRAM1 连接 LCD，地址范围为0X60000000~0X63FFFFFF
  * FSMC_A16 接LCD的DC(寄存器/数据选择)脚
  * 16 bit => FSMC[24:0]对应HADDR[25:1]
  * 寄存器基地址 = 0X60000000
  * RAM基地址 = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
  * 当选择不同的地址线时，地址要重新计算
  */
#define BANK1_LCD_RAM               ((uint32_t)0x60020000)
#define BANK1_LCD_REG               ((uint32_t)0x60000000)



// IO口初始化
__INLINE static void lcd_bus_port_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                           // FSMC相关引脚初始化
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = ( GPIO_Pin_0  | GPIO_Pin_1  |
                                    GPIO_Pin_4  | GPIO_Pin_5  |
                                    GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9 |
                                    GPIO_Pin_10 | GPIO_Pin_11 |
                                    GPIO_Pin_14 | GPIO_Pin_15 );
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ( GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  |
                                    GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
                                    GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 );
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOD, GPIO_Pin_3);                                          // CLK = 0
    GPIO_SetBits(GPIOD, GPIO_Pin_4);                                            // RD = 1
    GPIO_SetBits(GPIOD, GPIO_Pin_5);                                            // WR = 1
    GPIO_SetBits(GPIOD, GPIO_Pin_7);                                            // CS = 1

    FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 0x02;               //地址建立时间
    FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0x00;                //地址保持时间
    FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 0x05;                  //数据建立时间
    FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
    FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
    FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
    FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_B;        // 一般使用模式B来控制LCD

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

// 写寄存器
__INLINE static void lcd_bus_reg_wr(uint16_t reg)
{
    *(volatile uint16_t *)BANK1_LCD_REG = reg;
}

// 写数据
__INLINE static void lcd_bus_data_wr(uint16_t data)
{
    *(volatile uint16_t *)BANK1_LCD_RAM = data;
}

// 写指令(寄存器+数据)
__INLINE static void lcd_bus_cmd_wr(uint16_t reg, uint16_t data)
{
    lcd_bus_reg_wr(reg);
    lcd_bus_data_wr(data);
}

// 写数据
__INLINE static void lcd_bus_datas_wr_start(void)           {;}
__INLINE static void lcd_bus_datas_wring(uint16_t data)     { *(volatile uint16_t *)BANK1_LCD_RAM = data; }
__INLINE static void lcd_bus_datas_wr_stop(void)            {;}

// 读数据
__INLINE static void lcd_bus_datas_rd_start(void)           {;}
__INLINE static uint16_t lcd_bus_datas_rding(void)          { return *((volatile uint16_t *)BANK1_LCD_RAM); }
__INLINE static void lcd_bus_datas_rd_stop(void)            {;}



/**
  * @}
  */
/**
  * @}
  */

#endif
/* END OF FILE ---------------------------------------------------------------*/
