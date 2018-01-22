################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/randgen/randgen.c 

OBJS += \
./src/randgen/randgen.o 

C_DEPS += \
./src/randgen/randgen.d 


# Each subdirectory must supply rules for building sources it contributes
src/randgen/%.o: ../src/randgen/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


