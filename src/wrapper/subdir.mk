################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../wrapper/wrapperfacade_linux.o \
../wrapper/wrapperfacade_windows.o 

CPP_SRCS += \
../wrapper/wrapperfacade.cpp \
../wrapper/wrapperfacade_linux.cpp \
../wrapper/wrapperfacade_windows.cpp 

OBJS += \
./wrapper/wrapperfacade.o \
./wrapper/wrapperfacade_linux.o \
./wrapper/wrapperfacade_windows.o 

CPP_DEPS += \
./wrapper/wrapperfacade.d \
./wrapper/wrapperfacade_linux.d \
./wrapper/wrapperfacade_windows.d 


# Each subdirectory must supply rules for building sources it contributes
wrapper/%.o: ../wrapper/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_cplusplus=201103L -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


