COLOR_RED := $(shell echo "\033[1;31m")
COLOR_END := $(shell echo "\033[0m")

KERNEL ?= conv3
SUFFIX ?= 0
MAKE= make
KERNELDIR=$(CGRA_WORKBENCH)/kernels/$(KERNEL)
BUILDDIR=$(KERNELDIR)/build
OUTDIR=$(CGRA_WORKBENCH)/outputs/$(KERNEL)_$(SUFFIX)
BC = $(KERNELDIR)/build/kernel.bc
SO = $(MAPPER_HOME)/build/mapperPass-so
BITSTREAM = $(OUTDIR)/bitstream.bin
PARAMJSON = $(KERNELDIR)/param.json
MAPCONSTRAINTJSON = $(KERNELDIR)/mapconstraint.json
RUNSH = $(CGRA_WORKBENCH)/scripts/run.sh
GDBSH= $(CGRA_WORKBENCH)/scripts/gdb.sh
DOTSH= $(CGRA_WORKBENCH)/scripts/dot.sh

ifeq ($(wildcard $(SO)),)
$(warning $(COLOR_RED)Warning: $(SO) does not exists!$(COLOR_END))
$(warning $(COLOR_RED)Build it in HIT-CGRA-Mapper first!$(COLOR_END))
endif

compile : $(BC)
	make -C $(MAPPER_HOME) -j 16
	@bash $(RUNSH) $(OUTDIR) $(KERNELDIR) $(PARAMJSON) $(MAPCONSTRAINTJSON) $(SO) $(BC)
	@bash $(DOTSH) $(OUTDIR) $(KERNEL)_$(SUFFIX)
gdbmapper : $(BC)
	make -C $(MAPPER_HOME)
	@bash $(GDBSH) $(OUTDIR) $(KERNELDIR) $(PARAMJSON) $(MAPCONSTRAINTJSON) $(SO) $(BC)
	@bash $(DOTSH) $(OUTDIR) $(KERNEL)_$(SUFFIX)
emu :
	@$(MAKE) -s -C $(CGRA_EMU_HOME) clean
	ln -sf $(KERNELDIR)/kernel.h $(CGRA_EMU_HOME)/include/kernel.h
	make -C $(CGRA_EMU_HOME) run BITSTREAM=$(BITSTREAM) PARAM=$(PARAMJSON) -j 16
gdbemu :
	@$(MAKE) -s -C $(CGRA_EMU_HOME) clean
	ln -sf $(KERNELDIR)/kernel.h $(CGRA_EMU_HOME)/include/kernel.h
	make -C $(CGRA_EMU_HOME) gdb BITSTREAM=$(BITSTREAM) PARAM=$(PARAMJSON)
$(BC):
	@$(MAKE) -s  -C $(KERNELDIR)

cleanmapper:
	@$(MAKE) -s -C $(MAPPER_HOME) clean
cleanemu:
	@$(MAKE) -s -C $(CGRA_EMU_HOME) clean
cleanIR:
	@$(MAKE) -s -C $(KERNELDIR) clean
cleanallIR:
	@$(MAKE) -s -C $(CGRA_WORKBENCH)/kernels cleanall
cleanall:
	@$(MAKE) -s -C $(CGRA_EMU_HOME) clean
	@$(MAKE) -s -C $(CGRA_WORKBENCH)/kernels cleanall
	@$(MAKE) -s -C $(MAPPER_HOME) clean

