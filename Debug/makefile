################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := E:/ti/ccsv5/tools/compiler/c6000_7.4.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./Insect_Detection_Radar_Platform/package/package_Insect_Detection_Radar_Platform.obj" \
"./Theads/SignalProcess.obj" \
"./Theads/NetworkSend.obj" \
"./Theads/NetworkRecv.obj" \
"./Theads/Hwi.obj" \
"./Theads/HostCmdResponse.obj" \
"./VectorProcessModule/C6678_Vec/invc_for3x3mat.obj" \
"./VectorProcessModule/C6678_Vec/invc_d.obj" \
"./VectorProcessModule/C6678_Vec/Vec_cplxmulmat.obj" \
"./VectorProcessModule/C6678_Vec/Vec_FloatToDouble.obj" \
"./c6678_driver/resourcemgr.obj" \
"./c6678_driver/platform_osal.obj" \
"./c6678_driver/emac.obj" \
"./utils_syn.obj" \
"./main.obj" \
"./hookfunc.obj" \
"./emif16_fpga.obj" \
"./ProcInit.obj" \
"./GlobalVar.obj" \
"./DSPInit.obj" \
-l"libc.a" \
-l"E:\ti\pdk_C6678_1_1_2_6\packages\ti\drv\pcie\lib\ti.drv.pcie.ae66" \
-l"E:\ti\dsplib_c66x_3_1_0_0\lib\dsplib.ae66" \
-l"E:\ti\pdk_C6678_1_1_2_6\packages\ti\csl\lib\ti.csl.intc.ae66" \
-l"E:\ti\pdk_C6678_1_1_2_6\packages\ti\csl\lib\ti.csl.ae66" \
"../BD_Bistatic_SAR.cmd" \
"../c6678_driver/c6678_2dsp_ddr3_vpx_lib.lib" \
"../VectorProcessModule/C6678_Vec/C6678_Vec_lib_ELF.lib" \
"../VectorProcessModule/C6678_Vec/Raco_Vec_C6678.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include c6678_driver/subdir_vars.mk
-include VectorProcessModule/C6678_Vec/subdir_vars.mk
-include Theads/subdir_vars.mk
-include Insect_Detection_Radar_Platform/package/subdir_vars.mk
-include subdir_rules.mk
-include c6678_driver/subdir_rules.mk
-include VectorProcessModule/C6678_Vec/subdir_rules.mk
-include Theads/subdir_rules.mk
-include Insect_Detection_Radar_Platform/package/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: BD_Bistatic_SAR.out

# Tool invocations
BD_Bistatic_SAR.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"E:/ti/ccsv5/tools/compiler/c6000_7.4.1/bin/cl6x" -mv6600 --abi=eabi -g --display_error_number --diag_warning=225 -z -m"BD_Bistatic_SAR.map" -i"E:/ti/ccsv5/tools/compiler/c6000_7.4.1/lib" -i"E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --reread_libs --warn_sections --display_error_number --rom_model -o "BD_Bistatic_SAR.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_SRCS__QUOTED)$(GEN_MISC_FILES__QUOTED)$(C6000_EXECUTABLE_OUTPUTS__QUOTED)$(GEN_OPTS__QUOTED)$(GEN_CMDS__QUOTED) "BD_Bistatic_SAR.out"
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-$(RM) "DSPInit.pp" "GlobalVar.pp" "ProcInit.pp" "emif16_fpga.pp" "hookfunc.pp" "main.pp" "utils_syn.pp" "c6678_driver\emac.pp" "c6678_driver\platform_osal.pp" "c6678_driver\resourcemgr.pp" "Theads\HostCmdResponse.pp" "Theads\Hwi.pp" "Theads\NetworkRecv.pp" "Theads\NetworkSend.pp" "Theads\SignalProcess.pp" "Insect_Detection_Radar_Platform\package\package_Insect_Detection_Radar_Platform.pp" 
	-$(RM) "DSPInit.obj" "GlobalVar.obj" "ProcInit.obj" "emif16_fpga.obj" "hookfunc.obj" "main.obj" "utils_syn.obj" "c6678_driver\emac.obj" "c6678_driver\platform_osal.obj" "c6678_driver\resourcemgr.obj" "VectorProcessModule\C6678_Vec\Vec_FloatToDouble.obj" "VectorProcessModule\C6678_Vec\Vec_cplxmulmat.obj" "VectorProcessModule\C6678_Vec\invc_d.obj" "VectorProcessModule\C6678_Vec\invc_for3x3mat.obj" "Theads\HostCmdResponse.obj" "Theads\Hwi.obj" "Theads\NetworkRecv.obj" "Theads\NetworkSend.obj" "Theads\SignalProcess.obj" "Insect_Detection_Radar_Platform\package\package_Insect_Detection_Radar_Platform.obj" 
	-$(RM) "VectorProcessModule\C6678_Vec\Vec_FloatToDouble.pp" "VectorProcessModule\C6678_Vec\Vec_cplxmulmat.pp" "VectorProcessModule\C6678_Vec\invc_d.pp" "VectorProcessModule\C6678_Vec\invc_for3x3mat.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
