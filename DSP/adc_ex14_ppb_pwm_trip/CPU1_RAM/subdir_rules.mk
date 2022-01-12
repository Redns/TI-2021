################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="D:/Project/GuoSai/DSP/adc_ex14_ppb_pwm_trip" --include_path="D:/Project/GuoSai/DSP/adc_ex14_ppb_pwm_trip/device" --include_path="D:/C2000_WAVE/C2000Ware_4_00_00_00/driverlib/driverlib/f28004x/driverlib" --include_path="D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/include" --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="D:/Project/GuoSai/DSP/adc_ex14_ppb_pwm_trip/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-2071550497: ../adc_ex14_ppb_pwm_trip.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"D:/CCS/ccs/utils/sysconfig_1.10.0/sysconfig_cli.bat" -s "D:/C2000_WAVE/C2000Ware_4_00_00_00/driverlib/.metadata/sdk.json" -d "F28004x" --script "D:/Project/GuoSai/DSP/adc_ex14_ppb_pwm_trip/adc_ex14_ppb_pwm_trip.syscfg" -o "syscfg" --package F28004x_100PZ --part F28004x_100PZ --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/error.h: build-2071550497 ../adc_ex14_ppb_pwm_trip.syscfg
syscfg/: build-2071550497


