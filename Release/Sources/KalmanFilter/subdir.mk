################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/KalmanFilter/kalman_filter_small.cpp \
../Sources/KalmanFilter/predictor.cpp 

OBJS += \
./Sources/KalmanFilter/kalman_filter_small.o \
./Sources/KalmanFilter/predictor.o 

CPP_DEPS += \
./Sources/KalmanFilter/kalman_filter_small.d \
./Sources/KalmanFilter/predictor.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/KalmanFilter/%.o: ../Sources/KalmanFilter/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL25Z128VLK4" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/Positioning" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/Common" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/Utilities" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/Display" -I"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/platform/drivers/inc" -I"../Sources" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/KalmanFilter" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/Sources/Accelerometer" -I"/home/eivind/Kinetis/pos_estimator-KL25Z4/External/eigen" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MKL25Z4/include" -std=gnu++11 -fabi-version=8 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -Os -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


