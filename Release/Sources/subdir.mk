################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/main.c 

OBJS += \
./Sources/main.o 

C_DEPS += \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL25Z128VLK4" -I"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/platform/utilities/src" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/Display" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/Common" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/Positioning" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/KalmanFilter" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/Accelerometer" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/Utilities" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/Board" -I"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/platform/utilities/inc" -I"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/platform/osa/inc" -I"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/platform/osa/src" -I"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/platform/drivers/inc" -I"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/platform/hal/inc" -I"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/platform/hal/src" -I"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/platform/system/inc" -I"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/platform/system/src" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MKL25Z4/include" -I"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/platform/drivers/src/i2c" -std=c99 -Os -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


