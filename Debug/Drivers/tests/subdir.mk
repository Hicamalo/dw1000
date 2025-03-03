################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/tests/tests.c 

OBJS += \
./Drivers/tests/tests.o 

C_DEPS += \
./Drivers/tests/tests.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/tests/%.o Drivers/tests/%.su Drivers/tests/%.cyclo: ../Drivers/tests/%.c Drivers/tests/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/terminal -I../Drivers/terminal/platform -I../Drivers/decadriver -I../Drivers/decadriver/platform -I../Drivers/tests -I../Core/Src/examples -I../Drivers -I../Drivers/dw1000/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-tests

clean-Drivers-2f-tests:
	-$(RM) ./Drivers/tests/tests.cyclo ./Drivers/tests/tests.d ./Drivers/tests/tests.o ./Drivers/tests/tests.su

.PHONY: clean-Drivers-2f-tests

