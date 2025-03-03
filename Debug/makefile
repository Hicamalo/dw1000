################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/tests/subdir.mk
-include Drivers/terminal/platform/subdir.mk
-include Drivers/terminal/subdir.mk
-include Drivers/dw1000/Src/subdir.mk
-include Drivers/decadriver/platform/subdir.mk
-include Drivers/decadriver/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Drivers/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/examples/ex_10a_gpio/subdir.mk
-include Core/Src/examples/ex_09b_bandwidth_power_comp/subdir.mk
-include Core/Src/examples/ex_09a_bandwidth_power_ref_meas/subdir.mk
-include Core/Src/examples/ex_08b_low_power_listen_tx/subdir.mk
-include Core/Src/examples/ex_08a_low_power_listen_rx/subdir.mk
-include Core/Src/examples/ex_07b_ack_data_rx/subdir.mk
-include Core/Src/examples/ex_07a_ack_data_tx/subdir.mk
-include Core/Src/examples/ex_06b_ss_twr_resp/subdir.mk
-include Core/Src/examples/ex_06a_ss_twr_init/subdir.mk
-include Core/Src/examples/ex_05b_ds_twr_resp/subdir.mk
-include Core/Src/examples/ex_05a_ds_twr_init/subdir.mk
-include Core/Src/examples/ex_04b_cont_frame/subdir.mk
-include Core/Src/examples/ex_04a_cont_wave/subdir.mk
-include Core/Src/examples/ex_03d_tx_wait_resp_interrupts/subdir.mk
-include Core/Src/examples/ex_03c_tx_wait_resp_leds/subdir.mk
-include Core/Src/examples/ex_03b_rx_send_resp/subdir.mk
-include Core/Src/examples/ex_03a_tx_wait_resp/subdir.mk
-include Core/Src/examples/ex_02f_rx_with_crystal_trim/subdir.mk
-include Core/Src/examples/ex_02e_rx_dbl_buff/subdir.mk
-include Core/Src/examples/ex_02d_rx_sniff/subdir.mk
-include Core/Src/examples/ex_02c_rx_diagnostics/subdir.mk
-include Core/Src/examples/ex_02b_rx_preamble_64/subdir.mk
-include Core/Src/examples/ex_02a_simple_rx/subdir.mk
-include Core/Src/examples/ex_01e_tx_with_cca/subdir.mk
-include Core/Src/examples/ex_01d_tx_timed_sleep/subdir.mk
-include Core/Src/examples/ex_01c_tx_sleep_auto/subdir.mk
-include Core/Src/examples/ex_01b_tx_sleep/subdir.mk
-include Core/Src/examples/ex_01a_simple_tx/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := test_dw1000
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
test_dw1000.elf \

MAP_FILES += \
test_dw1000.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
test_dw1000.list \


# All Target
all: main-build

# Main-build Target
main-build: test_dw1000.elf secondary-outputs

# Tool invocations
test_dw1000.elf test_dw1000.map: $(OBJS) $(USER_OBJS) C:\Users\SoloPCW\Desktop\UWB\workspace_1.17.0\test_dw1000\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "test_dw1000.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\SoloPCW\Desktop\UWB\workspace_1.17.0\test_dw1000\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="test_dw1000.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

test_dw1000.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "test_dw1000.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout test_dw1000.elf test_dw1000.list test_dw1000.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
