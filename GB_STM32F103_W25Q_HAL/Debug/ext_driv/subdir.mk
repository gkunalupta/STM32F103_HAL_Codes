################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ext_driv/w25q128jv.c 

OBJS += \
./ext_driv/w25q128jv.o 

C_DEPS += \
./ext_driv/w25q128jv.d 


# Each subdirectory must supply rules for building sources it contributes
ext_driv/%.o: ../ext_driv/%.c ext_driv/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../ext_driv -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ext_driv

clean-ext_driv:
	-$(RM) ./ext_driv/w25q128jv.d ./ext_driv/w25q128jv.o

.PHONY: clean-ext_driv

