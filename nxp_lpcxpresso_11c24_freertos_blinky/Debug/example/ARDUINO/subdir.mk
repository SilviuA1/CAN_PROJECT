################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../example/ARDUINO/arduino_code.c 

OBJS += \
./example/ARDUINO/arduino_code.o 

C_DEPS += \
./example/ARDUINO/arduino_code.d 


# Each subdirectory must supply rules for building sources it contributes
example/ARDUINO/%.o: ../example/ARDUINO/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M0 -DSDK_DEBUGCONSOLE=1 -D__NEWLIB__ -I"/home/ryu/Workspace/facultate/CAN/PROJECT/developing/nxp_lpcxpresso_11c24_freertos_blinky/example/inc" -I"/home/ryu/Workspace/facultate/CAN/PROJECT/developing/nxp_lpcxpresso_11c24_freertos_blinky/freertos/inc" -I"/home/ryu/Workspace/facultate/CAN/PROJECT/developing/lpc_chip_11cxx_lib/inc" -I"/home/ryu/Workspace/facultate/CAN/PROJECT/developing/nxp_lpcxpresso_11c24_board_lib/inc" -I"/home/ryu/Workspace/facultate/CAN/PROJECT/developing/nxp_lpcxpresso_11c24_freertos_blinky/example/CAN" -I"/home/ryu/Workspace/facultate/CAN/PROJECT/developing/nxp_lpcxpresso_11c24_freertos_blinky/example/UART" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


