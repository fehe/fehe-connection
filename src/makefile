################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include wrapper/subdir.mk
-include protocol/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libFeheConnection.so

# Tool invocations
libFeheConnection.so: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -pthread -shared -o "libFeheConnection.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) libFeheConnection.so
	-@echo ' '


.PHONY: all clean dependents


install: libFeheConnection.so
	install -m 0755 libFeheConnection.so /usr/lib
	mkdir -p /usr/include/fehe_connection
	mkdir -p /usr/include/fehe_connection/wrapper
	mkdir -p /usr/include/fehe_connection/protocol
	install -m 0755 *.h /usr/include/fehe_connection
	install -m 0755 protocol/*.h /usr/include/fehe_connection/protocol
	install -m 0755 wrapper/*.h /usr/include/fehe_connection/wrapper
	ldconfig

.PHONY: install

.SECONDARY:

-include ../makefile.targets
