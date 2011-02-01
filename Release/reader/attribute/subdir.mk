################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../reader/attribute/attributeUtil.c 

OBJS += \
./reader/attribute/attributeUtil.o 

C_DEPS += \
./reader/attribute/attributeUtil.d 


# Each subdirectory must supply rules for building sources it contributes
reader/attribute/%.o: ../reader/attribute/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


