obj-m += reveng_rtkit.o

KERNEL_ROOT=/lib/modules/$(shell uname -r)/build

modules:
	@$(MAKE) -C $(KERNEL_ROOT) M=$(shell pwd) modules

clean:
	@$(MAKE) -C $(KERNEL_ROOT) M=$(shell pwd) clean

