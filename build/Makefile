buildroot:
	git clone https://github.com/buildroot/buildroot; \
	$(MAKE) -C buildroot/ defconfig BR2_DEFCONFIG=../../defconfig/buildroot; \
	$(MAKE) -C buildroot/ -j$$((`nproc`>1 ? `nproc`-1 : 1))

linux:
	git clone --depth=1 https://github.com/antmicro/linux -b litex-rebase; \
	export ARCH=riscv; \
	export CROSS_COMPILE=riscv64-linux-gnu-; \
	$(MAKE) -C linux/ defconfig KBUILD_DEFCONFIG=../../../../../defconfig/linux; \
	$(MAKE) -C linux/ -j$$((`nproc`>1 ? `nproc`-1 : 1))
