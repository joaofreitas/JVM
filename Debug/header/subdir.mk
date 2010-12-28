################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../header/classLoader.c \
../header/formatoClass.c 

OBJS += \
./header/classLoader.o \
./header/formatoClass.o 

C_DEPS += \
./header/classLoader.d \
./header/formatoClass.d 


# Each subdirectory must supply rules for building sources it contributes
header/%.o: ../header/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


