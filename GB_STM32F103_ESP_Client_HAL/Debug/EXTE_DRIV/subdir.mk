################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EXTE_DRIV/GB_ESP8266_Basic.c 

OBJS += \
./EXTE_DRIV/GB_ESP8266_Basic.o 

C_DEPS += \
./EXTE_DRIV/GB_ESP8266_Basic.d 


# Each subdirectory must supply rules for building sources it contributes
EXTE_DRIV/%.o: ../EXTE_DRIV/%.c EXTE_DRIV/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../EXTE_DRIV -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-EXTE_DRIV

clean-EXTE_DRIV:
	-$(RM) ./EXTE_DRIV/GB_ESP8266_Basic.d ./EXTE_DRIV/GB_ESP8266_Basic.o

.PHONY: clean-EXTE_DRIV

