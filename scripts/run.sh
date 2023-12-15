OUTDIR=$1
KERNELDIR=$2
JSON=$3
SO=$4
BC=$5
mkdir -p $OUTDIR
cp -r $KERNELDIR $OUTDIR
cp $JSON $OUTDIR
cd $OUTDIR
opt-12 -load $SO -mapperPass $BC
