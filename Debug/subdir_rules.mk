################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
DSPInit.obj: ../DSPInit.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"E:/ti/ccsv5/tools/compiler/c6000_7.4.1/bin/cl6x" -mv6600 --abi=eabi -g --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform/evmc6678l/platform_lib/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/cppi" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/qmss" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages" --include_path="E:/ti/mathlib_c66x_3_0_1_1/inc" --include_path="E:/ti/ti/dsplib_c66x_3_1_0_0/inc" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c66" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="DSPInit.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

GlobalVar.obj: ../GlobalVar.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"E:/ti/ccsv5/tools/compiler/c6000_7.4.1/bin/cl6x" -mv6600 --abi=eabi -g --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform/evmc6678l/platform_lib/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/cppi" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/qmss" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages" --include_path="E:/ti/mathlib_c66x_3_0_1_1/inc" --include_path="E:/ti/ti/dsplib_c66x_3_1_0_0/inc" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c66" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="GlobalVar.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ProcInit.obj: ../ProcInit.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"E:/ti/ccsv5/tools/compiler/c6000_7.4.1/bin/cl6x" -mv6600 --abi=eabi -g --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform/evmc6678l/platform_lib/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/cppi" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/qmss" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages" --include_path="E:/ti/mathlib_c66x_3_0_1_1/inc" --include_path="E:/ti/ti/dsplib_c66x_3_1_0_0/inc" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c66" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="ProcInit.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/compiler.opt: ../app.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"E:/ti/xdctools_3_23_04_60/xs" --xdcpath="E:/ti/bios_6_33_06_50/packages;E:/ti/ccsv5/ccs_base;C:/Users/Administrator.BIT-20160510ITO/myRepository/packages;E:/ti/mcsdk_2_01_02_06/demos;E:/ti/ndk_2_21_01_38/packages;D:/ccs_workspace_new/BD_Bistatic_SAR;E:/ti/pdk_C6678_1_1_2_6/packages;" xdc.tools.configuro -o configPkg -t ti.targets.elf.C66 -p Insect_Detection_Radar_Platform -r release -b "D:/ccs_workspace_new/BD_Bistatic_SAR/Insect_Detection_Radar_Platform/config.bld" -c "E:/ti/ccsv5/tools/compiler/c6000_7.4.1" --compileOptions "-g --optimize_with_debug" "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/compiler.opt: D:/ccs_workspace_new/BD_Bistatic_SAR/Insect_Detection_Radar_Platform/config.bld
configPkg/linker.cmd: configPkg/compiler.opt D:/ccs_workspace_new/BD_Bistatic_SAR/Insect_Detection_Radar_Platform/config.bld
configPkg/: configPkg/compiler.opt D:/ccs_workspace_new/BD_Bistatic_SAR/Insect_Detection_Radar_Platform/config.bld

emif16_fpga.obj: ../emif16_fpga.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"E:/ti/ccsv5/tools/compiler/c6000_7.4.1/bin/cl6x" -mv6600 --abi=eabi -g --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform/evmc6678l/platform_lib/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/cppi" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/qmss" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages" --include_path="E:/ti/mathlib_c66x_3_0_1_1/inc" --include_path="E:/ti/ti/dsplib_c66x_3_1_0_0/inc" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c66" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="emif16_fpga.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hookfunc.obj: ../hookfunc.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"E:/ti/ccsv5/tools/compiler/c6000_7.4.1/bin/cl6x" -mv6600 --abi=eabi -g --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform/evmc6678l/platform_lib/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/cppi" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/qmss" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages" --include_path="E:/ti/mathlib_c66x_3_0_1_1/inc" --include_path="E:/ti/ti/dsplib_c66x_3_1_0_0/inc" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c66" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="hookfunc.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"E:/ti/ccsv5/tools/compiler/c6000_7.4.1/bin/cl6x" -mv6600 --abi=eabi -g --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform/evmc6678l/platform_lib/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/cppi" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/qmss" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages" --include_path="E:/ti/mathlib_c66x_3_0_1_1/inc" --include_path="E:/ti/ti/dsplib_c66x_3_1_0_0/inc" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c66" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

utils_syn.obj: ../utils_syn.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"E:/ti/ccsv5/tools/compiler/c6000_7.4.1/bin/cl6x" -mv6600 --abi=eabi -g --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform/evmc6678l/platform_lib/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/cppi" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/qmss" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages" --include_path="E:/ti/mathlib_c66x_3_0_1_1/inc" --include_path="E:/ti/ti/dsplib_c66x_3_1_0_0/inc" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c66" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="utils_syn.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


