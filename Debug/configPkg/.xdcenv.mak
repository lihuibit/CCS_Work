#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = E:/ti/bios_6_33_06_50/packages;E:/ti/ccsv5/ccs_base;C:/Users/Administrator.BIT-20160510ITO/myRepository/packages;E:/ti/mcsdk_2_01_02_06/demos;E:/ti/ndk_2_21_01_38/packages;D:/ccs_workspace_new/BD_Bistatic_SAR;E:/ti/pdk_C6678_1_1_2_6/packages
override XDCROOT = E:/ti/xdctools_3_23_04_60
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = E:/ti/bios_6_33_06_50/packages;E:/ti/ccsv5/ccs_base;C:/Users/Administrator.BIT-20160510ITO/myRepository/packages;E:/ti/mcsdk_2_01_02_06/demos;E:/ti/ndk_2_21_01_38/packages;D:/ccs_workspace_new/BD_Bistatic_SAR;E:/ti/pdk_C6678_1_1_2_6/packages;E:/ti/xdctools_3_23_04_60/packages;..
HOSTOS = Windows
endif
