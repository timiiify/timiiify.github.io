obj-m += lxmem.o
KERNELDIR := /workspaces/cepheus-q-oss
TOOLCHAIN := /workspaces/cepheus-q-oss/toolchain/bin
CROSS_COMPILE := $(TOOLCHAIN)/aarch64-linux-android-
KBUILD_CFLAGS += -w
KBUILD_EXTRA_SYMBOLS+=/workspaces/cepheus-q-oss/out/Module.symvers # ??symbols?crc
all:
	$(MAKE) ARCH=arm64 SUBARCH=arm64 -C $(KERNELDIR) M=$(PWD) modules CROSS_COMPILE=$(CROSS_COMPILE)

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
mrproper:
	$(MAKE) -C $(KERNELDIR) O=$(KERNELDIR) mrproper
