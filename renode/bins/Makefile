BUILD_DIR = "../../build"
DRIVER_CODE_DIR = "../../driver-code"

drive.img:
	mkdir -p ../../shared; \
	truncate drive.img -s 128MB; \
	mkfs.ext4 -d ../../shared/ drive.img

clean_drive:
	rm -f drive.img

buildroot:
	$(MAKE) -C $(BUILD_DIR) buildroot

driver:
	$(MAKE) -C $(DRIVER_CODE_DIR) maximtemp_driver.ko

application:
	$(MAKE) -C $(DRIVER_CODE_DIR) maximtemp

rootfs.cpio: buildroot driver application
	mkdir -p $(BUILD_DIR)/rootfs && cd $(BUILD_DIR)/rootfs; \
	cat ../buildroot/output/images/rootfs.cpio | cpio -idmv 2> /dev/null; \
	cp ../../driver-code/maximtemp_driver.ko root/; \
	cp ../../driver-code/maximtemp root/; \
	find . | cpio -o -H newc -R root:root > ../../renode/bins/rootfs.cpio

linux:
	$(MAKE) -C $(BUILD_DIR) linux

Image: linux
	cp ../../build/linux/arch/riscv/boot/Image .

device_tree:
	$(MAKE) -C $(DRIVER_CODE_DIR) device_tree

rv32.dbt: device_tree
	cp $(DRIVER_CODE_DIR)/rv32.dtb .

all: drive.img Image rootfs.cpio rv32.dbt
