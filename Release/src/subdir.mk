################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/allocate-vars.c \
../src/assignment.c \
../src/data.c \
../src/infer-b.c \
../src/infer-cmnt.c \
../src/infer-z.c \
../src/infer.c \
../src/inivars.c \
../src/params.c \
../src/sctm-main.c \
../src/utils.c 

OBJS += \
./src/allocate-vars.o \
./src/assignment.o \
./src/data.o \
./src/infer-b.o \
./src/infer-cmnt.o \
./src/infer-z.o \
./src/infer.o \
./src/inivars.o \
./src/params.o \
./src/sctm-main.o \
./src/utils.o 

C_DEPS += \
./src/allocate-vars.d \
./src/assignment.d \
./src/data.d \
./src/infer-b.d \
./src/infer-cmnt.d \
./src/infer-z.d \
./src/infer.d \
./src/inivars.d \
./src/params.d \
./src/sctm-main.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


