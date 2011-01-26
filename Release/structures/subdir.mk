################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../structures/frame.c \
../structures/heap.c \
../structures/methodArea.c \
../structures/mnemonics.c 

OBJS += \
./structures/frame.o \
./structures/heap.o \
./structures/methodArea.o \
./structures/mnemonics.o 

C_DEPS += \
./structures/frame.d \
./structures/heap.d \
./structures/methodArea.d \
./structures/mnemonics.d 


# Each subdirectory must supply rules for building sources it contributes
structures/%.o: ../structures/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


