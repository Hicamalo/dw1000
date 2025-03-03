################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/terminal/platform/terminal.c 

OBJS += \
./Drivers/terminal/platform/terminal.o 

C_DEPS += \
./Drivers/terminal/platform/terminal.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/terminal/platform/%.o Drivers/terminal/platform/%.su Drivers/terminal/platform/%.cyclo: ../Drivers/terminal/platform/%.c Drivers/terminal/platform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/terminal -I../Drivers/terminal/platform -I../Drivers/decadriver -I../Drivers/decadriver/platform -I../Drivers/tests -I../Core/Src/examples -I../Drivers -I../Drivers/dw1000/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-terminal-2f-platform

clean-Drivers-2f-terminal-2f-platform:
	-$(RM) ./Drivers/terminal/platform/terminal.cyclo ./Drivers/terminal/platform/terminal.d ./Drivers/terminal/platform/terminal.o ./Drivers/terminal/platform/terminal.su

.PHONY: clean-Drivers-2f-terminal-2f-platform

