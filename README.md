## File Tree
```
.
├── kernels (the dir storing kernels)
│    └──fir (the dir to build kernel)
│       ├── build
│       │   ├── kernel.bc
│       │   └── kernel.ll
│       ├── fir.c
│       ├── Makefile
│       └── param.json
├── Makefile
├── outputs	(the outputs of the HIT-CGRA-Mapper)
│   └── fir_0 (NAME_SUFFIX)
│       ├── dfg.json
│       ├── fir (cp the workbench/kernels/NAME here to save)
│       │   ├── build
│       │   │   ├── kernel.bc
│       │   │   └── kernel.ll
│       │   ├── fir.c
│       │   ├── Makefile
│       │   └── param.json
│       ├── fir_0.png
│       ├── kernel.dot
│       └── param.json
├── README.md
└── scripts (scripts to help build)
    ├── dot.sh
    ├── gdb.sh
    └── run.sh
```

## Quick Start
1. make sure you have build HIT-CGRA-Mapper,If you haven't completed it yet, please read README of HIT-CGRA-Mapper
2. add CGRA\_WORKBENCH to ~/.bashrc  
	1. vim ~/.bashrc then add at the last line,for example "export CGRA\_WORKBENCH=/home/user/workbench"
	2. source ~/.bashrc
3. cd $CGRA\_WORKBENCH,run "make" to run the default kernel "fir"

## Add Your kernels
1. mkdir for your kernel, you can name the folder according to your preferences. **the name of the folder is the kernel's NAME** 
2. write the Makefile for your kernel,Reference ./kernels/fir/Makefile.Make sure if your run “make” in ./kernels/yourkernel, build dir is generated and kernel.bc is in it. and run "make clean" rm the build dir.

## Make  
1. make cmd include
	1. run:	build your kernel in ./kernels/NAME,and run the HIT-CGRA-Mapper to generate the result. default run fir
	2. gdb: build your kernel in ./kernel/NAME,and run HIT-CGRA-Mapper through GDB to find bugs in the HIT-CGRA-Mapper
	3. clean: rm the ./kernel/NAME/build
2. make param
	1. KERNEL : the kernel you want to build,run or clean
	2. SUFFIX : give a suffix to your kernel, to diff the different output of the same kernel.the default suffix is \_0
3. example
```
	make KERNEL=NAME SUFFIX=test gdb
	make KERNEL=NAME clean
```
