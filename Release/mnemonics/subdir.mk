################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mnemonics/mnemonics.c 

OBJS += \
./mnemonics/mnemonics.o 

C_DEPS += \
./mnemonics/mnemonics.d 


# Each subdirectory must supply rules for building sources it contributes
mnemonics/%.o: ../mnemonics/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


