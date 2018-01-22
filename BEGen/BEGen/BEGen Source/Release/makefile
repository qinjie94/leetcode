################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/workload/subdir.mk
-include src/util/subdir.mk
-include src/sort/subdir.mk
-include src/shared/subdir.mk
-include src/randgen/subdir.mk
-include src/main/subdir.mk
-include src/linkedlist/subdir.mk
-include src/hashtable/subdir.mk
-include src/hash/subdir.mk
-include src/binaryheap/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: BEGen

# Tool invocations
BEGen: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "BEGen" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) BEGen
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
