################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/binaryheap/binheap.c 

OBJS += \
./src/binaryheap/binheap.o 

C_DEPS += \
./src/binaryheap/binheap.d 


# Each subdirectory must supply rules for building sources it contributes
src/binaryheap/%.o: ../src/binaryheap/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


