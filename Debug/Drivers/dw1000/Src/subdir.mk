################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/dw1000/Src/calibration.c \
../Drivers/dw1000/Src/dw1000.c \
../Drivers/dw1000/Src/frame.c \
../Drivers/dw1000/Src/fsm.c 

OBJS += \
./Drivers/dw1000/Src/calibration.o \
./Drivers/dw1000/Src/dw1000.o \
./Drivers/dw1000/Src/frame.o \
./Drivers/dw1000/Src/fsm.o 

C_DEPS += \
./Drivers/dw1000/Src/calibration.d \
./Drivers/dw1000/Src/dw1000.d \
./Drivers/dw1000/Src/frame.d \
./Drivers/dw1000/Src/fsm.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/dw1000/Src/%.o Drivers/dw1000/Src/%.su Drivers/dw1000/Src/%.cyclo: ../Drivers/dw1000/Src/%.c Drivers/dw1000/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/terminal -I../Drivers/terminal/platform -I../Drivers/decadriver -I../Drivers/decadriver/platform -I../Drivers/tests -I../Core/Src/examples -I../Drivers -I../Drivers/dw1000/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-dw1000-2f-Src

clean-Drivers-2f-dw1000-2f-Src:
	-$(RM) ./Drivers/dw1000/Src/calibration.cyclo ./Drivers/dw1000/Src/calibration.d ./Drivers/dw1000/Src/calibration.o ./Drivers/dw1000/Src/calibration.su ./Drivers/dw1000/Src/dw1000.cyclo ./Drivers/dw1000/Src/dw1000.d ./Drivers/dw1000/Src/dw1000.o ./Drivers/dw1000/Src/dw1000.su ./Drivers/dw1000/Src/frame.cyclo ./Drivers/dw1000/Src/frame.d ./Drivers/dw1000/Src/frame.o ./Drivers/dw1000/Src/frame.su ./Drivers/dw1000/Src/fsm.cyclo ./Drivers/dw1000/Src/fsm.d ./Drivers/dw1000/Src/fsm.o ./Drivers/dw1000/Src/fsm.su

.PHONY: clean-Drivers-2f-dw1000-2f-Src

