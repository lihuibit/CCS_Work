## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd \
  package/cfg/app_pe66.oe66 \

linker.cmd: package/cfg/app_pe66.xdl
	$(SED) 's"^\"\(package/cfg/app_pe66cfg.cmd\)\"$""\"D:/ccs_workspace_new/BD_Bistatic_SAR/Debug/configPkg/\1\""' package/cfg/app_pe66.xdl > $@
