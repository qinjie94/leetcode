################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hash/hashfunc.c \
../src/hash/hashsep.c 

OBJS += \
./src/hash/hashfunc.o \
./src/hash/hashsep.o 

C_DEPS += \
./src/hash/hashfunc.d \
./src/hash/hashsep.d 


# Each subdirectory must supply rules for building sources it contributes
src/hash/%.o: ../src/hash/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


