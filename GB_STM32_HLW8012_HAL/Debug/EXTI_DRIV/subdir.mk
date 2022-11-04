################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EXTI_DRIV/gb_hlw8012.c \
../EXTI_DRIV/gb_timer_input_capture.c 

OBJS += \
./EXTI_DRIV/gb_hlw8012.o \
./EXTI_DRIV/gb_timer_input_capture.o 

C_DEPS += \
./EXTI_DRIV/gb_hlw8012.d \
./EXTI_DRIV/gb_timer_input_capture.d 


# Each subdirectory must supply rules for building sources it contributes
EXTI_DRIV/%.o: ../EXTI_DRIV/%.c EXTI_DRIV/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-EXTI_DRIV

clean-EXTI_DRIV:
	-$(RM) ./EXTI_DRIV/gb_hlw8012.d ./EXTI_DRIV/gb_hlw8012.o ./EXTI_DRIV/gb_timer_input_capture.d ./EXTI_DRIV/gb_timer_input_capture.o

.PHONY: clean-EXTI_DRIV

