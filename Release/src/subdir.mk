################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/averagecluster.cpp \
../src/cluster.cpp \
../src/common.cpp \
../src/completecluster.cpp \
../src/inmatrix.cpp \
../src/main.cpp \
../src/matrix.cpp \
../src/singlecluster.cpp 

OBJS += \
./src/averagecluster.o \
./src/cluster.o \
./src/common.o \
./src/completecluster.o \
./src/inmatrix.o \
./src/main.o \
./src/matrix.o \
./src/singlecluster.o 

CPP_DEPS += \
./src/averagecluster.d \
./src/cluster.d \
./src/common.d \
./src/completecluster.d \
./src/inmatrix.d \
./src/main.d \
./src/matrix.d \
./src/singlecluster.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


