OUTDIR=$1
KERNELDIR=$2
PARAMJSON=$3
CONSTRAINTJSON=$4
SO=$5
BC=$6
mkdir -p $OUTDIR
cp -r $KERNELDIR $OUTDIR
cp $PARAMJSON $OUTDIR
cp $CONSTRAINTJSON $OUTDIR
cd $OUTDIR
cat > script.gdb << EOF
set breakpoint pending on
break mapperPass::runOnFunction
run -load $SO -mapperPass $BC
EOF
gdb opt-12 -x script.gdb
