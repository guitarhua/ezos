################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_gui/config/GUIConf.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_gui/config/GUI_X.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_gui/config/LCDConf.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_gui/config/LCDDriver.c 

OBJS += \
./components/cpn_gui/config/GUIConf.o \
./components/cpn_gui/config/GUI_X.o \
./components/cpn_gui/config/LCDConf.o \
./components/cpn_gui/config/LCDDriver.o 

C_DEPS += \
./components/cpn_gui/config/GUIConf.d \
./components/cpn_gui/config/GUI_X.d \
./components/cpn_gui/config/LCDConf.d \
./components/cpn_gui/config/LCDDriver.d 


# Each subdirectory must supply rules for building sources it contributes
components/cpn_gui/config/GUIConf.o: D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_gui/config/GUIConf.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\fire_stm32\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\fire_stm32\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

components/cpn_gui/config/GUI_X.o: D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_gui/config/GUI_X.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\fire_stm32\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\fire_stm32\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

components/cpn_gui/config/LCDConf.o: D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_gui/config/LCDConf.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\fire_stm32\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\fire_stm32\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

components/cpn_gui/config/LCDDriver.o: D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_gui/config/LCDDriver.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\fire_stm32\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\fire_stm32\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


