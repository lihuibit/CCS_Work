################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Insect_Detection_Radar_Platform/package/package_Insect_Detection_Radar_Platform.obj: ../Insect_Detection_Radar_Platform/package/package_Insect_Detection_Radar_Platform.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"E:/ti/ccsv5/tools/compiler/c6000_7.4.1/bin/cl6x" -mv6600 --abi=eabi -g --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform/evmc6678l/platform_lib/include" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/platform" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/cppi" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/drv/qmss" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages" --include_path="E:/ti/mathlib_c66x_3_0_1_1/inc" --include_path="E:/ti/ti/dsplib_c66x_3_1_0_0/inc" --include_path="E:/ti/dsplib_c66x_3_1_0_0/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c66" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages/ti/csl/src/intc" --include_path="E:/ti/pdk_C6678_1_1_2_6/packages" --include_path="E:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --display_error_number --diag_warning=225 --preproc_with_compile --preproc_dependency="Insect_Detection_Radar_Platform/package/package_Insect_Detection_Radar_Platform.pp" --obj_directory="Insect_Detection_Radar_Platform/package" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


