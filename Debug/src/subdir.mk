################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/buildin.c \
../src/executeCommand.c \
../src/main.c \
../src/prompt.c \
../src/readCommand.c \
../src/shell_init.c 

OBJS += \
./src/buildin.o \
./src/executeCommand.o \
./src/main.o \
./src/prompt.o \
./src/readCommand.o \
./src/shell_init.o 

C_DEPS += \
./src/buildin.d \
./src/executeCommand.d \
./src/main.d \
./src/prompt.d \
./src/readCommand.d \
./src/shell_init.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


