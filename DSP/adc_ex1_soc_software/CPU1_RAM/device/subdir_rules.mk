################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
device/%.obj: ../device/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="D:/Project/GuoSai/DSP/adc_ex1_soc_software" --include_path="D:/Project/GuoSai/DSP/adc_ex1_soc_software/device" --include_path="D:/C2000_WAVE/C2000Ware_4_00_00_00/driverlib/f28004x/driverlib" --include_path="D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/include" --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="D:/Project/GuoSai/DSP/adc_ex1_soc_software/CPU1_RAM/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/%.obj: ../device/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="D:/Project/GuoSai/DSP/adc_ex1_soc_software" --include_path="D:/Project/GuoSai/DSP/adc_ex1_soc_software/device" --include_path="D:/C2000_WAVE/C2000Ware_4_00_00_00/driverlib/f28004x/driverlib" --include_path="D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/include" --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="D:/Project/GuoSai/DSP/adc_ex1_soc_software/CPU1_RAM/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


