
./build/kernel.ll:./build/kernel.bc
	@mkdir -p build
	@llvm-dis-12 $< -o $@
./build/kernel.bc:fft.c
	@mkdir -p build
	@clang-12 -emit-llvm -fno-unroll-loops -O3 -m32 -o $@ -c $<

clean:
	@rm -rf build
