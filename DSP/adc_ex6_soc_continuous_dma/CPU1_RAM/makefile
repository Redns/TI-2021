################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./adc_ex6_soc_continuous_dma.obj" \
"./syscfg/board.obj" \
"./device/device.obj" \
"./device/f28004x_codestartbranch.obj" \
"../28004x_generic_ram_lnk.cmd" \
"D:/C2000_WAVE/C2000Ware_4_00_00_00/driverlib/f28004x/driverlib/ccs/Debug/driverlib.lib" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include device/subdir_vars.mk
-include subdir_rules.mk
-include device/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
adc_ex6_soc_continuous_dma.out \

EXE_OUTPUTS__QUOTED += \
"adc_ex6_soc_continuous_dma.out" \

BIN_OUTPUTS += \
adc_ex6_soc_continuous_dma.hex \

BIN_OUTPUTS__QUOTED += \
"adc_ex6_soc_continuous_dma.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "adc_ex6_soc_continuous_dma.out"

# Tool invocations
adc_ex6_soc_continuous_dma.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi -z -m"adc_ex6_soc_continuous_dma.map" --heap_size=0x200 --stack_size=0x3F8 --warn_sections -i"D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/lib" -i"D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="adc_ex6_soc_continuous_dma_linkInfo.xml" --entry_point=code_start --rom_model -o "adc_ex6_soc_continuous_dma.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

adc_ex6_soc_continuous_dma.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: C2000 Hex Utility'
	"D:/CCS/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/hex2000" --diag_wrap=off -o "adc_ex6_soc_continuous_dma.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "adc_ex6_soc_continuous_dma.obj" "syscfg\board.obj" "device\device.obj" "device\f28004x_codestartbranch.obj" 
	-$(RM) "adc_ex6_soc_continuous_dma.d" "syscfg\board.d" "device\device.d" 
	-$(RM) "device\f28004x_codestartbranch.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-if 0 == 1  mkdir "D:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma/CPU1_RAM/simulation"
	-if 0 == 1  C:/TDM-GCC-64/bin/g++  -c -DCLB_SIM -ID:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc-2.3.3/src -ID:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc/include/type2 -ID:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma -IC:/TDM-GCC-64/bin/include -Og -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MFD:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma/CPU1_RAM/simulation/clb_sim.d -MTD:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma/CPU1_RAM/simulation/clb_sim.o -ID:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma/CPU1_RAM/syscfg   -fno-threadsafe-statics  -oD:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma/CPU1_RAM/simulation/clb_sim.o D:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma/CPU1_RAM/syscfg/clb_sim.cpp
	-if 0 == 1  C:/TDM-GCC-64/bin/g++ -DCLB_SIM -Og -g -gdwarf-3 -gstrict-dwarf -Wall -Wl,-Map,D:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma/CPU1_RAM/simulation/simulation_output.map -LD:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc-2.3.3/build/src -oD:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma/CPU1_RAM/simulation/simulation_output.exe D:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma/CPU1_RAM/simulation/clb_sim.o D:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type2/CLB_FSM_SC_model.o D:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type2/CLB_HLC_SC_model.o D:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type2/CLB_LUT4_SC_model.o D:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type2/CLB_OutputLUT_SC_model.o D:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type2/CLB_counter_SC_model.o D:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type2/CLB_counter_top_SC_model.o D:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type2/CLB_FSM_top_SC_model.o D:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type2/CLB_LUT4_top_SC_model.o D:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/systemc/src/type2/CLB_OutputLUT_top_SC_model.o  -Wl,--start-group -lsystemc -Wl,--end-group
	-if 0 == 1  "./simulation/simulation_output.exe"
	-if 0 == 1  D:/CCS/ccs/tools/node/node.exe "D:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/dot_file_libraries/clbDotUtility.js"  "D:/C2000_WAVE/C2000Ware_4_00_00_00/utilities/clb_tool/clb_syscfg/" "D:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma/CPU1_RAM/syscfg" "D:/Project/GuoSai/DSP/adc_ex6_soc_continuous_dma/CPU1_RAM/syscfg/clb.dot"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

