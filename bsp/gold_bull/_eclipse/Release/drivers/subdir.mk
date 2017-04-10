################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/assert.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_ctrlport.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_eeprom.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_extint.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_i2c.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_i2s.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_key.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_led.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_rtc_time.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_spi.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_systick.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_tcsbus.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_timer.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_uart.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_watchdog.c 

OBJS += \
./drivers/assert.o \
./drivers/drv_ctrlport.o \
./drivers/drv_eeprom.o \
./drivers/drv_extint.o \
./drivers/drv_i2c.o \
./drivers/drv_i2s.o \
./drivers/drv_key.o \
./drivers/drv_led.o \
./drivers/drv_rtc_time.o \
./drivers/drv_spi.o \
./drivers/drv_systick.o \
./drivers/drv_tcsbus.o \
./drivers/drv_timer.o \
./drivers/drv_uart.o \
./drivers/drv_watchdog.o 

C_DEPS += \
./drivers/assert.d \
./drivers/drv_ctrlport.d \
./drivers/drv_eeprom.d \
./drivers/drv_extint.d \
./drivers/drv_i2c.d \
./drivers/drv_i2s.d \
./drivers/drv_key.d \
./drivers/drv_led.d \
./drivers/drv_rtc_time.d \
./drivers/drv_spi.d \
./drivers/drv_systick.d \
./drivers/drv_tcsbus.d \
./drivers/drv_timer.d \
./drivers/drv_uart.d \
./drivers/drv_watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/assert.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/assert.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_ctrlport.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_ctrlport.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_eeprom.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_eeprom.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_extint.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_extint.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_i2c.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_i2s.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_i2s.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_key.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_key.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_led.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_led.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_rtc_time.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_rtc_time.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_spi.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_systick.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_systick.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_tcsbus.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_tcsbus.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_timer.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_uart.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/drv_watchdog.o: D:/Users/draapho/Desktop/stm32_ezos_v231/drivers/drv_watchdog.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


