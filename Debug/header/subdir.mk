################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../header/classFileReader.c \
../header/classLoader.c 

OBJS += \
./header/classFileReader.o \
./header/classLoader.o 

C_DEPS += \
./header/classFileReader.d \
./header/classLoader.d 


# Each subdirectory must supply rules for building sources it contributes
header/%.o: ../header/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


