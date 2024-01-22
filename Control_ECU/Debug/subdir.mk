################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP.c \
../DC_motor.c \
../External_eeprom.c \
../buzzer.c \
../gpio.c \
../timer1.c \
../twi.c \
../uart.c 

OBJS += \
./APP.o \
./DC_motor.o \
./External_eeprom.o \
./buzzer.o \
./gpio.o \
./timer1.o \
./twi.o \
./uart.o 

C_DEPS += \
./APP.d \
./DC_motor.d \
./External_eeprom.d \
./buzzer.d \
./gpio.d \
./timer1.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


