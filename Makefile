COLOR_RED := $(shell echo "\033[1;31m")
COLOR_END := $(shell echo "\033[0m")

KERNEL ?= mm2
SUFFIX ?= 0
MAKE= make
KERNELDIR=$(CGRA_WORKBENCH)/kernels/$(KERNEL)
BUILDDIR=$(KERNELDIR)/build
OUTDIR=$(CGRA_WORKBENCH)/outputs/$(KERNEL)_$(SUFFIX)
BC = $(KERNELDIR)/build/kernel.bc
SO = $(MAPPER_HOME)/build/mapperPass-so
JSON = $(KERNELDIR)/param.json
RUNSH = $(CGRA_WORKBENCH)/scripts/run.sh
GDBSH= $(CGRA_WORKBENCH)/scripts/gdb.sh
DOTSH= $(CGRA_WORKBENCH)/scripts/dot.sh

ifeq ($(wildcard $(SO)),)
$(warning $(COLOR_RED)Warning: $(SO) does not exists!$(COLOR_END))
$(warning $(COLOR_RED)Build it in HIT-CGRA-Mapper first!$(COLOR_END))
endif

run : $(BC)
	@bash $(RUNSH) $(OUTDIR) $(KERNELDIR) $(JSON) $(SO) $(BC)
	@bash $(DOTSH) $(OUTDIR) $(KERNEL)_$(SUFFIX)
gdb : $(BC)
	@bash $(GDBSH) $(OUTDIR) $(KERNELDIR) $(JSON) $(SO) $(BC)
	@bash $(DOTSH) $(OUTDIR) $(KERNEL)_$(SUFFIX)
$(BC):
	@$(MAKE) -s  -C $(KERNELDIR)

clean:
	@$(MAKE) -s -C $(KERNELDIR) clean
