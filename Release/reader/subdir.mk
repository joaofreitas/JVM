################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../reader/classFileReader.c \
../reader/classLoader.c 

OBJS += \
./reader/classFileReader.o \
./reader/classLoader.o 

C_DEPS += \
./reader/classFileReader.d \
./reader/classLoader.d 


# Each subdirectory must supply rules for building sources it contributes
reader/%.o: ../reader/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


