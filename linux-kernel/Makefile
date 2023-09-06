obj-m += completions.o
obj-m += cryptosk.o
obj-m += example_atomic.o
obj-m += example_mutex.o
obj-m += example_rwlock.o
obj-m += example_spinlock.o
obj-m += example_tasklet.o
obj-m += hello-sysfs.o
obj-m += ioctl.o
obj-m += procfs1
obj-m += procfs2
obj-m += procfs3
obj-m += procfs4
obj-m += sched
obj-m += sleep
obj-m += print_string
obj-m += print_string
obj-m += print_string
obj-m += print_string
obj-m += print_string

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build CC=$(CC) M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build CC=$(CC) M=$(PWD) clean
	$(RM) other/cat_noblock *.plist


test: default
	sudo dmesg -c && sudo insmod ${TARGET}.ko && sudo dmesg -c \
		&& ls -al &&  sudo dmesg -c && sudo rmmod ${TARGET}ko && sudo dmesg -c