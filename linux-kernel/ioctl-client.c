#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/ioctl.h>

struct ioctl_arg {
    unsigned int val;
};

#define IOCTL_MAGIC 0x66
#define IOCTL_VALSET _IOW(IOCTL_MAGIC, 0, struct ioctl_arg)
#define IOCTL_VALGET _IOR(IOCTL_MAGIC, 1, struct ioctl_arg)
#define IOCTL_VALGET_NUM _IOR(IOCTL_MAGIC, 2, int)
#define IOCTL_VALSET_NUM _IOW(IOCTL_MAGIC, 3, int)

int main() {
    int fd = open("/dev/ioctltest", O_RDWR);

    if (fd == -1) {
        perror("Failed to open the device");
        return -1;
    }

    struct ioctl_arg arg;
    arg.val = 0x12345678;

    if (ioctl(fd, IOCTL_VALSET, &arg) == -1) {
        perror("IOCTL_VALSET failed");
    } else {
        printf("IOCTL_VALSET succeeded\n");
    }

    if (ioctl(fd, IOCTL_VALGET, &arg) == -1) {
        perror("IOCTL_VALGET failed");
    } else {
        printf("IOCTL_VALGET returned value: 0x%x\n", arg.val);
    }

    int num = 42;
    if (ioctl(fd, IOCTL_VALSET_NUM, &num) == -1) {
        perror("IOCTL_VALSET_NUM failed");
    } else {
        printf("IOCTL_VALSET_NUM succeeded\n");
    }

    if (ioctl(fd, IOCTL_VALGET_NUM, &num) == -1) {
        perror("IOCTL_VALGET_NUM failed");
    } else {
        printf("IOCTL_VALGET_NUM returned value: %d\n", num);
    }

    close(fd);

    return 0;
}
