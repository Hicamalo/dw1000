################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/utils.c 

OBJS += \
./Drivers/utils.o 

C_DEPS += \
./Drivers/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o Drivers/%.su Drivers/%.cyclo: ../Drivers/%.c Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/terminal -I../Drivers/terminal/platform -I../Drivers/decadriver -I../Drivers/decadriver/platform -I../Drivers/tests -I../Core/Src/examples -I../Drivers -I../Drivers/dw1000/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers

clean-Drivers:
	-$(RM) ./Drivers/utils.cyclo ./Drivers/utils.d ./Drivers/utils.o ./Drivers/utils.su

.PHONY: clean-Drivers

