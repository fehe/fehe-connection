################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../fehe_client.o \
../fehe_dllmain.o \
../fehe_sender.o \
../fehe_server.o 

CPP_SRCS += \
../fehe_client.cpp \
../fehe_dllmain.cpp \
../fehe_sender.cpp \
../fehe_server.cpp 

OBJS += \
./fehe_client.o \
./fehe_dllmain.o \
./fehe_sender.o \
./fehe_server.o 

CPP_DEPS += \
./fehe_client.d \
./fehe_dllmain.d \
./fehe_sender.d \
./fehe_server.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_cplusplus=201103L -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


