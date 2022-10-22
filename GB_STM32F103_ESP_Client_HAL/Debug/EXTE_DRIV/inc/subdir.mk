################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EXTE_DRIV/inc/GB_ESP8266_client.c 

OBJS += \
./EXTE_DRIV/inc/GB_ESP8266_client.o 

C_DEPS += \
./EXTE_DRIV/inc/GB_ESP8266_client.d 


# Each subdirectory must supply rules for building sources it contributes
EXTE_DRIV/inc/%.o: ../EXTE_DRIV/inc/%.c EXTE_DRIV/inc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../EXTE_DRIV/inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-EXTE_DRIV-2f-inc

clean-EXTE_DRIV-2f-inc:
	-$(RM) ./EXTE_DRIV/inc/GB_ESP8266_client.d ./EXTE_DRIV/inc/GB_ESP8266_client.o

.PHONY: clean-EXTE_DRIV-2f-inc

