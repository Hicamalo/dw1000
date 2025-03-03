################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/decadriver/platform/deca_mutex.c \
../Drivers/decadriver/platform/deca_range_tables.c \
../Drivers/decadriver/platform/deca_sleep.c \
../Drivers/decadriver/platform/deca_spi.c \
../Drivers/decadriver/platform/port.c 

OBJS += \
./Drivers/decadriver/platform/deca_mutex.o \
./Drivers/decadriver/platform/deca_range_tables.o \
./Drivers/decadriver/platform/deca_sleep.o \
./Drivers/decadriver/platform/deca_spi.o \
./Drivers/decadriver/platform/port.o 

C_DEPS += \
./Drivers/decadriver/platform/deca_mutex.d \
./Drivers/decadriver/platform/deca_range_tables.d \
./Drivers/decadriver/platform/deca_sleep.d \
./Drivers/decadriver/platform/deca_spi.d \
./Drivers/decadriver/platform/port.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/decadriver/platform/%.o Drivers/decadriver/platform/%.su Drivers/decadriver/platform/%.cyclo: ../Drivers/decadriver/platform/%.c Drivers/decadriver/platform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/terminal -I../Drivers/terminal/platform -I../Drivers/decadriver -I../Drivers/decadriver/platform -I../Drivers/tests -I../Core/Src/examples -I../Drivers -I../Drivers/dw1000/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-decadriver-2f-platform

clean-Drivers-2f-decadriver-2f-platform:
	-$(RM) ./Drivers/decadriver/platform/deca_mutex.cyclo ./Drivers/decadriver/platform/deca_mutex.d ./Drivers/decadriver/platform/deca_mutex.o ./Drivers/decadriver/platform/deca_mutex.su ./Drivers/decadriver/platform/deca_range_tables.cyclo ./Drivers/decadriver/platform/deca_range_tables.d ./Drivers/decadriver/platform/deca_range_tables.o ./Drivers/decadriver/platform/deca_range_tables.su ./Drivers/decadriver/platform/deca_sleep.cyclo ./Drivers/decadriver/platform/deca_sleep.d ./Drivers/decadriver/platform/deca_sleep.o ./Drivers/decadriver/platform/deca_sleep.su ./Drivers/decadriver/platform/deca_spi.cyclo ./Drivers/decadriver/platform/deca_spi.d ./Drivers/decadriver/platform/deca_spi.o ./Drivers/decadriver/platform/deca_spi.su ./Drivers/decadriver/platform/port.cyclo ./Drivers/decadriver/platform/port.d ./Drivers/decadriver/platform/port.o ./Drivers/decadriver/platform/port.su

.PHONY: clean-Drivers-2f-decadriver-2f-platform

