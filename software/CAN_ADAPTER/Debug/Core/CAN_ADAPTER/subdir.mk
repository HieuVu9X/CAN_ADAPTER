################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/CAN_ADAPTER/adapter.c 

OBJS += \
./Core/CAN_ADAPTER/adapter.o 

C_DEPS += \
./Core/CAN_ADAPTER/adapter.d 


# Each subdirectory must supply rules for building sources it contributes
Core/CAN_ADAPTER/%.o Core/CAN_ADAPTER/%.su Core/CAN_ADAPTER/%.cyclo: ../Core/CAN_ADAPTER/%.c Core/CAN_ADAPTER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/Admin/OneDrive/Desktop/hieuvm/8.UAV_Project/CAN_ADAPTER/software/CAN_ADAPTER/Core/CAN_ADAPTER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-CAN_ADAPTER

clean-Core-2f-CAN_ADAPTER:
	-$(RM) ./Core/CAN_ADAPTER/adapter.cyclo ./Core/CAN_ADAPTER/adapter.d ./Core/CAN_ADAPTER/adapter.o ./Core/CAN_ADAPTER/adapter.su

.PHONY: clean-Core-2f-CAN_ADAPTER

