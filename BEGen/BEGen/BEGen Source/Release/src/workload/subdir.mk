################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/workload/generateWorkLoad.c \
../src/workload/workload.c 

OBJS += \
./src/workload/generateWorkLoad.o \
./src/workload/workload.o 

C_DEPS += \
./src/workload/generateWorkLoad.d \
./src/workload/workload.d 


# Each subdirectory must supply rules for building sources it contributes
src/workload/%.o: ../src/workload/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


