################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/shared/clock.c \
../src/shared/generic.c 

OBJS += \
./src/shared/clock.o \
./src/shared/generic.o 

C_DEPS += \
./src/shared/clock.d \
./src/shared/generic.d 


# Each subdirectory must supply rules for building sources it contributes
src/shared/%.o: ../src/shared/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


