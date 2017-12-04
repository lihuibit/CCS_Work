################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../BD_Bistatic_SAR.cmd 

CFG_SRCS += \
../app.cfg 

C_SRCS += \
../DSPInit.c \
../GlobalVar.c \
../ProcInit.c \
../emif16_fpga.c \
../hookfunc.c \
../main.c \
../utils_syn.c 

OBJS += \
./DSPInit.obj \
./GlobalVar.obj \
./ProcInit.obj \
./emif16_fpga.obj \
./hookfunc.obj \
./main.obj \
./utils_syn.obj 

GEN_SRCS += \
./configPkg/compiler.opt \
./configPkg/linker.cmd 

C_DEPS += \
./DSPInit.pp \
./GlobalVar.pp \
./ProcInit.pp \
./emif16_fpga.pp \
./hookfunc.pp \
./main.pp \
./utils_syn.pp 

GEN_MISC_DIRS += \
./configPkg/ 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_OPTS += \
./configPkg/compiler.opt 

GEN_SRCS__QUOTED += \
"configPkg\compiler.opt" \
"configPkg\linker.cmd" 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

C_DEPS__QUOTED += \
"DSPInit.pp" \
"GlobalVar.pp" \
"ProcInit.pp" \
"emif16_fpga.pp" \
"hookfunc.pp" \
"main.pp" \
"utils_syn.pp" 

OBJS__QUOTED += \
"DSPInit.obj" \
"GlobalVar.obj" \
"ProcInit.obj" \
"emif16_fpga.obj" \
"hookfunc.obj" \
"main.obj" \
"utils_syn.obj" 

C_SRCS__QUOTED += \
"../DSPInit.c" \
"../GlobalVar.c" \
"../emif16_fpga.c" \
"../hookfunc.c" \
"../main.c" \
"../utils_syn.c" 

GEN_OPTS__FLAG += \
--cmd_file="./configPkg/compiler.opt" 

GEN_CMDS__FLAG += \
-l"./configPkg/linker.cmd" 


