OUTDIR=$1
KERNELDIR=$2
JSON=$3
SO=$4
BC=$5
mkdir -p $OUTDIR
cp -r $KERNELDIR $OUTDIR
cp $JSON $OUTDIR
cd $OUTDIR
cat > script.gdb << EOF
set breakpoint pending on
break mapperPass::runOnFunction
run -load $SO -mapperPass $BC
EOF
gdb opt-12 -x script.gdb
