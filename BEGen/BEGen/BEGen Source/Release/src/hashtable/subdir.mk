################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hashtable/hashtable.c \
../src/hashtable/hashtable_itr.c \
../src/hashtable/hashtable_utility.c 

OBJS += \
./src/hashtable/hashtable.o \
./src/hashtable/hashtable_itr.o \
./src/hashtable/hashtable_utility.o 

C_DEPS += \
./src/hashtable/hashtable.d \
./src/hashtable/hashtable_itr.d \
./src/hashtable/hashtable_utility.d 


# Each subdirectory must supply rules for building sources it contributes
src/hashtable/%.o: ../src/hashtable/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


