################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Sources/Utilities/subdir.mk
-include Sources/Positioning/subdir.mk
-include Sources/KalmanFilter/subdir.mk
-include Sources/Display/subdir.mk
-include Sources/Common/subdir.mk
-include Sources/Board/subdir.mk
-include Sources/Accelerometer/subdir.mk
-include Sources/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
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
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
pos_estimator_frdm-kl25z.siz \


# All Target
all: pos_estimator_frdm-kl25z.elf secondary-outputs

# Tool invocations
pos_estimator_frdm-kl25z.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -T "MKL25Z128xxx4_flash.ld" -Xlinker --gc-sections -L"/home/eivind/Kinetis/pos_estimator-KL25Z4/Project_Settings/Linker_Files" -L/home/eivind/Kinetis/hello_world_frdm-kl25z/Sources/Board -L/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/lib/ksdk_hal_lib/kds/KL25Z4/debug -L/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/lib/ksdk_platform_lib/kds/KL25Z4/debug -L"/home/eivind/Documents/Cambridge/Classes/EmbeddedSystems/KSDK_1.3.0/lib/kdsk_platform_lib/kds/KL25Z4/inc" -Wl,-Map,"pos_estimator_frdm-kl25z.map" -specs=nosys.specs -specs=nano.specs -Xlinker -z -Xlinker muldefs -o "pos_estimator_frdm-kl25z.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

pos_estimator_frdm-kl25z.siz: pos_estimator_frdm-kl25z.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "pos_estimator_frdm-kl25z.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(C++_DEPS)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS)$(S_UPPER_DEPS) pos_estimator_frdm-kl25z.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
