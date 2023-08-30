obj-m += lkm_example.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	sudo rm -fr /dev/lkm_example && sudo rmmod lkm_example.ko
	sudo insmod lkm_example.ko
	$(eval MAJOR_NUM := $(shell dmesg | grep "lkm_example module loaded with device major number" | head -n 1 | awk '{print $$9}'))
	sudo mknod /dev/lkm_example c $(MAJOR_NUM) 0
	sudo chmod 777 /dev/lkm_example
	cat /dev/lkm_example