################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/COM_ADAPTER/can_to_com.c \
../Core/COM_ADAPTER/com_adapter.c \
../Core/COM_ADAPTER/uart_to_com.c 

OBJS += \
./Core/COM_ADAPTER/can_to_com.o \
./Core/COM_ADAPTER/com_adapter.o \
./Core/COM_ADAPTER/uart_to_com.o 

C_DEPS += \
./Core/COM_ADAPTER/can_to_com.d \
./Core/COM_ADAPTER/com_adapter.d \
./Core/COM_ADAPTER/uart_to_com.d 


# Each subdirectory must supply rules for building sources it contributes
Core/COM_ADAPTER/%.o Core/COM_ADAPTER/%.su Core/COM_ADAPTER/%.cyclo: ../Core/COM_ADAPTER/%.c Core/COM_ADAPTER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Admin/OneDrive/Desktop/hieuvm/8.UAV_Project/CAN_ADAPTER/software/CAN_ADAPTER/Core/COM_ADAPTER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-COM_ADAPTER

clean-Core-2f-COM_ADAPTER:
	-$(RM) ./Core/COM_ADAPTER/can_to_com.cyclo ./Core/COM_ADAPTER/can_to_com.d ./Core/COM_ADAPTER/can_to_com.o ./Core/COM_ADAPTER/can_to_com.su ./Core/COM_ADAPTER/com_adapter.cyclo ./Core/COM_ADAPTER/com_adapter.d ./Core/COM_ADAPTER/com_adapter.o ./Core/COM_ADAPTER/com_adapter.su ./Core/COM_ADAPTER/uart_to_com.cyclo ./Core/COM_ADAPTER/uart_to_com.d ./Core/COM_ADAPTER/uart_to_com.o ./Core/COM_ADAPTER/uart_to_com.su

.PHONY: clean-Core-2f-COM_ADAPTER

