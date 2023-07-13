# Driver

obj-m+=maximtemp_driver.o
KBUILD_DIR?=../build/linux

# Kernel module build

WORKING_DIR = `pwd`

linux:
	$(MAKE) -C ../build linux

driver: maximtemp_driver.ko

maximtemp_driver.ko: maximtemp_driver.c linux
	export ARCH=riscv; \
	export CROSS_COMPILE=riscv64-linux-gnu-; \
	$(MAKE) -C $(KBUILD_DIR) M=$(WORKING_DIR) modules

clean_driver: linux
	$(MAKE) -C $(KBUILD_DIR) M=$(WORKING_DIR) clean

# Device tree

device_tree: rv32.dtb

rv32.dtb: rv32.dts
	dtc -I dts -O dtb -o rv32.dtb rv32.dts

clean_device_tree:
	rm -f rv32.dtb

# Test application

APPLICATION_CC = ./../build/buildroot/output/host/bin/riscv32-linux-gcc
APPLICATION_CFLAGS = -std=gnu17 -Wall -Wextra

BINARY_NAME := maximtemp
SRCS := maximtemp.c
OBJS := $(SRCS:%.c=%.o)

buildroot:
	$(MAKE) -C ../build buildroot

$(BINARY_NAME): $(OBJS) buildroot
	$(APPLICATION_CC) $(OBJS) -o $@

%.o: %.c buildroot
	$(APPLICATION_CC) -c $(APPLICATION_CFLAGS) $< -o $@

clean_maximtemp:
	rm -f $(OBJS) $(BINARY_NAME)

# General

.PHONY: all clean

all: driver device_tree maximtemp

clean: clean_driver clean_device_tree clean_maximtemp
