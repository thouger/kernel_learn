#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert W. Oliver II");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");
#define DEVICE_NAME "lkm_example"
#define EXAMPLE_MSG "Hello, World!\n"
#define MSG_BUFFER_LEN 15
/* Prototypes for device functions */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static int major_num;
static int device_open_count = 0;
static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr;
/* This structure points to all of the device functions */
static struct file_operations file_ops = {.read = device_read,
                                          .write = device_write,
                                          .open = device_open,
                                          .release = device_release};
/* When a process reads from our device, this gets called. */
static ssize_t device_read(struct file *flip, char *buffer, size_t len,
                           loff_t *offset) {
  int i, total_bytes_read = 0;
  printk(KERN_INFO "device_read called\n");
  for (i = 0; i < 10; i++) {
    int bytes_read = 0;
    /* If we’re at the end, loop back to the beginning */
    if (*msg_ptr == 0) {
      msg_ptr = msg_buffer;
    }
    /* Put data in the buffer */
    while (len && *msg_ptr) {
      /* Buffer is in user data, not kernel, so you can’t just reference
       * with a pointer. The function put_user handles this for us */
      put_user(*(msg_ptr++), buffer++);
      len--;
      bytes_read++;
    }
    total_bytes_read += bytes_read;
  }
  return total_bytes_read; // Return total bytes read for all repetitions
}

/* Called when a process tries to write to our device */
static ssize_t device_write(struct file *flip, const char *buffer, size_t len,
                            loff_t *offset) {
  /* This is a read-only device */
  printk(KERN_ALERT "This operation is not supported.\n");
  return -EINVAL;
}
/* Called when a process opens our device */
static int device_open(struct inode *inode, struct file *file) {
  /* If device is open, return busy */
  if (device_open_count) {
    return -EBUSY;
  }
  device_open_count++;
  try_module_get(THIS_MODULE);
  return 0;
}
/* Called when a process closes our device */
static int device_release(struct inode *inode, struct file *file) {
  /* Decrement the open counter and usage count. Without this, the module would
   * not unload. */
  device_open_count--;
  module_put(THIS_MODULE);
  return 0;
}
static int __init lkm_example_init(void) {
  /* Fill buffer with our message */
  strncpy(msg_buffer, EXAMPLE_MSG, MSG_BUFFER_LEN);
  /* Set the msg_ptr to the buffer */
  msg_ptr = msg_buffer;
  /* Try to register character device */
  major_num = register_chrdev(0, "lkm_example", &file_ops);
  if (major_num < 0) {
    printk(KERN_ALERT "Could not register device: %d\n", major_num);
    return major_num;
  } else {
    printk(KERN_INFO "lkm_example module loaded with device major number %d\n",
           major_num);
    return 0;
  }
}
static void __exit lkm_example_exit(void) {
  /* Remember — we have to clean up after ourselves. Unregister the character
   * device. */
  unregister_chrdev(major_num, DEVICE_NAME);
  printk(KERN_INFO "Goodbye, World!\n");
}
/* Register module functions */
module_init(lkm_example_init);
module_exit(lkm_example_exit);
