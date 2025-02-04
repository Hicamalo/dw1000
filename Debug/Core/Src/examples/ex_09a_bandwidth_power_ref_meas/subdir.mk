################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/examples/ex_09a_bandwidth_power_ref_meas/ex_09a_main.c 

OBJS += \
./Core/Src/examples/ex_09a_bandwidth_power_ref_meas/ex_09a_main.o 

C_DEPS += \
./Core/Src/examples/ex_09a_bandwidth_power_ref_meas/ex_09a_main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/examples/ex_09a_bandwidth_power_ref_meas/%.o Core/Src/examples/ex_09a_bandwidth_power_ref_meas/%.su Core/Src/examples/ex_09a_bandwidth_power_ref_meas/%.cyclo: ../Core/Src/examples/ex_09a_bandwidth_power_ref_meas/%.c Core/Src/examples/ex_09a_bandwidth_power_ref_meas/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/terminal -I../Drivers/terminal/platform -I../Drivers/decadriver -I../Drivers/decadriver/platform -I../Drivers/tests -I../Core/Src/examples -I../Drivers -I../Drivers/dw1000/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-examples-2f-ex_09a_bandwidth_power_ref_meas

clean-Core-2f-Src-2f-examples-2f-ex_09a_bandwidth_power_ref_meas:
	-$(RM) ./Core/Src/examples/ex_09a_bandwidth_power_ref_meas/ex_09a_main.cyclo ./Core/Src/examples/ex_09a_bandwidth_power_ref_meas/ex_09a_main.d ./Core/Src/examples/ex_09a_bandwidth_power_ref_meas/ex_09a_main.o ./Core/Src/examples/ex_09a_bandwidth_power_ref_meas/ex_09a_main.su

.PHONY: clean-Core-2f-Src-2f-examples-2f-ex_09a_bandwidth_power_ref_meas

