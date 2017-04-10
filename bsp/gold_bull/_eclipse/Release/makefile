################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := cs-rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include ezos/subdir.mk
-include drivers/libraries/STM32F10x_StdPeriph_Driver/src/subdir.mk
-include drivers/libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/subdir.mk
-include drivers/libraries/CMSIS/CM3/CoreSupport/subdir.mk
-include drivers/drv_spiflash/subdir.mk
-include drivers/drv_sdcard/subdir.mk
-include drivers/subdir.mk
-include components/cpn_uip/uip/subdir.mk
-include components/cpn_uip/tcpip/subdir.mk
-include components/cpn_uip/chips/subdir.mk
-include components/cpn_uip/apps/demo/subdir.mk
-include components/cpn_uip/apps/subdir.mk
-include components/cpn_tone/data/subdir.mk
-include components/cpn_tone/subdir.mk
-include components/cpn_lib/subdir.mk
-include components/cpn_gui/driver/subdir.mk
-include components/cpn_gui/demo/subdir.mk
-include components/cpn_gui/config/subdir.mk
-include components/cpn_fatfs/option/subdir.mk
-include components/cpn_fatfs/subdir.mk
-include applications/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
gold_bull.hex \

SECONDARY_LIST += \
gold_bull.lst \

SECONDARY_SIZE += \
gold_bull.siz \


# All Target
all: gold_bull.elf secondary-outputs

# Tool invocations
gold_bull.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Windows GCC C Linker (Sourcery Lite Bare)'
	arm-none-eabi-gcc -Xlinker --gc-sections -Wl,-Map,"gold_bull.map" -mcpu=cortex-m3 -mthumb -o "gold_bull.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

gold_bull.hex: gold_bull.elf
	@echo 'Invoking: ARM Windows GNU Create Flash Image (Sourcery Lite Bare)'
	arm-none-eabi-objcopy -O ihex "gold_bull.elf"  "gold_bull.hex"
	@echo 'Finished building: $@'
	@echo ' '

gold_bull.lst: gold_bull.elf
	@echo 'Invoking: ARM Windows GNU Create Listing (Sourcery Lite Bare)'
	arm-none-eabi-objdump -h -S "gold_bull.elf" > "gold_bull.lst"
	@echo 'Finished building: $@'
	@echo ' '

gold_bull.siz: gold_bull.elf
	@echo 'Invoking: ARM Windows GNU Print Size (Sourcery Lite Bare)'
	arm-none-eabi-size  --format=berkeley "gold_bull.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) gold_bull.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
