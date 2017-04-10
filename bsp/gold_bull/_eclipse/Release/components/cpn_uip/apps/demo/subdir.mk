################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/httpd-cgi.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/httpd-fs.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/httpd.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/tcpc.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/tcps.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/udpc.c \
D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/udps.c 

OBJS += \
./components/cpn_uip/apps/demo/httpd-cgi.o \
./components/cpn_uip/apps/demo/httpd-fs.o \
./components/cpn_uip/apps/demo/httpd.o \
./components/cpn_uip/apps/demo/tcpc.o \
./components/cpn_uip/apps/demo/tcps.o \
./components/cpn_uip/apps/demo/udpc.o \
./components/cpn_uip/apps/demo/udps.o 

C_DEPS += \
./components/cpn_uip/apps/demo/httpd-cgi.d \
./components/cpn_uip/apps/demo/httpd-fs.d \
./components/cpn_uip/apps/demo/httpd.d \
./components/cpn_uip/apps/demo/tcpc.d \
./components/cpn_uip/apps/demo/tcps.d \
./components/cpn_uip/apps/demo/udpc.d \
./components/cpn_uip/apps/demo/udps.d 


# Each subdirectory must supply rules for building sources it contributes
components/cpn_uip/apps/demo/httpd-cgi.o: D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/httpd-cgi.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

components/cpn_uip/apps/demo/httpd-fs.o: D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/httpd-fs.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

components/cpn_uip/apps/demo/httpd.o: D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/httpd.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

components/cpn_uip/apps/demo/tcpc.o: D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/tcpc.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

components/cpn_uip/apps/demo/tcps.o: D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/tcps.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

components/cpn_uip/apps/demo/udpc.o: D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/udpc.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

components/cpn_uip/apps/demo/udps.o: D:/Users/draapho/Desktop/stm32_ezos_v231/components/cpn_uip/apps/demo/udps.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler (Sourcery Lite Bare)'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32F10X_CL -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\applications" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\bsp\gold_bull\configures" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_fatfs" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_tone" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\demo" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\config" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_gui\driver" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\apps" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\tcpip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\uip" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_uip\chips" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components\cpn_lib" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\components" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\ezos" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_sdcard" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\drv_spiflash" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\STM32F10x_StdPeriph_Driver\inc" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -I"D:\Users\draapho\Desktop\stm32_ezos_v231\drivers\libraries\CMSIS\CM3\CoreSupport" -Os -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


