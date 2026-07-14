#include <linux/init.h>    // module_init(), module_exit()
#include <linux/module.h>  // THIS_MODULE, MODULE_LICENSE
#include <linux/fs.h>      //struct file_operations
#include <linux/uaccess.h> //copy_to_user(), copy_from_user()
#include <linux/cdev.h>    // cdev_init()
#include <linux/device.h>  //class_create()
#include "rwbuf_ioctl.h"   // ioctl()

// #include "../include/rwbuf_ioctl.h"

#define BUF_SIZE 256
static dev_t dev_num;            // Bước 1. dev_t là kiểu dữ liệu chứa major number, minor number
static struct cdev rwbuf_cdev;   // Bước 3
static struct class *my_class;   // Bước 4
static struct device *rwbuf_dev; // Bước 5

/* cấu hình buffer*/
static char device_buffer[BUF_SIZE];
static size_t data_size;

/* Biến thống kê read()/write() toàn cục của driver */
static unsigned long read_count;  // đếm số lần read() được gọi
static unsigned long write_count; // đếm số lần write() được gọi

// character device function
static int my_open(struct inode *inode, struct file *file)
{
    pr_info("Device_opened\n");
    return 0;
}
static int my_release(struct inode *inode, struct file *file)
{
    pr_info("Device closed\n");
    return 0;
}
static ssize_t my_read(struct file *file, // thông tin file descriptor/device đang mở
                       char __user *buf,  // Buffer ở User Space để nhận dữ liệu
                       size_t count,      // Số bytes yêu cầu đọc
                       loff_t *ppos)      // vị trí đọc hiện tại (file offset)
{
    size_t bytes_to_read;

    /* đã đọc hết dữ liệu */
    if (*ppos >= data_size)
        return 0;

    /* chỉ đọc phần dữ liệu còn lại */
    bytes_to_read = min(count, data_size - *ppos);

    /* copy từ kernel space -> user space */
    if (copy_to_user(buf, device_buffer + *ppos, bytes_to_read))
        return -EFAULT;

    *ppos += bytes_to_read;
    read_count++; // tăng bộ đếm sau khi read() thành công
    pr_info("read %zu bytes\n", bytes_to_read);
    return bytes_to_read;
}

static ssize_t my_write(struct file *file,
                        const char __user *buf,
                        size_t count,
                        loff_t *ppos)
{
    size_t bytes_to_write;
    *ppos = 0; // overwrite

    /* Không cho ghi vượt buffer */
    bytes_to_write = min(count, (size_t)BUF_SIZE);

    /* copy từ user_space -> kernel space*/
    if (copy_from_user(device_buffer, buf, bytes_to_write))
        return -EFAULT;

    data_size = bytes_to_write;
    write_count++; // tăng bộ đếm sau mỗi lần write() thành công
    pr_info("written %zu bytes\n", bytes_to_write);
    return bytes_to_write;
}
static long my_ioctl(struct file *file,
                     unsigned int cmd,  // mã lệnh từ user gửi xuống
                     unsigned long arg) // tham số đi kèm(thường là con trỏ user space)
{
    /*struct tạm chứa dữ liệu thống kê */
    struct rwbuf_stats stats;

    /* kiểm tra magic number của ioctl */
    if (_IOC_TYPE(cmd) != RWBUF_MAGIC)
        return -ENOTTY;

    /* kiểm tra lệnh ioctl được gửi từ user*/
    switch (cmd)
    {
    case RWBUF_IOCTL_GET_STATS:
        /* copy giá trị thống kê hiên tại vào struct */
        stats.read_count = read_count;
        stats.write_count = write_count;

        /*
         * Sao chép dữ liệu từ kernel sang user
         * arg chứa địa chỉ &stats bên user space
         */
        if (copy_to_user((void __user *)arg, // địa  chỉ buffer bên user
                         &stats,             // dữ liệu nguồn trong kernel
                         sizeof(stats)))     // số bytes cần copy
            return -EFAULT;                  // lỗi truy cập bộ nhớ user
        return 0;
    default:
        /* không hỗ trợ ioctl này*/
        return -ENOTTY;
    }
}

// "Hợp đồng" giữa VFS và driver
// VFS nhìn vào đây để biết driver hỗ trợ syscall nào
static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,   // read() -> my_read()
    .write = my_write, // write() -> my_write()
    .release = my_release,
    .open = my_open,
    .unlocked_ioctl = my_ioctl, // khai báo ioctl()
};

// Hàm được gọi khi nạp module: sudo insmod .ko
static int __init rwbufdev_init(void)
{
    /*********Bước1: cấp phát major, minor cho device************/
    //
    int ret;
    // Hàm cấp phát Major Number + Minor Number cho device
    ret = alloc_chrdev_region(&dev_num,    // struct lưu major, minor number của device
                              0,           // minor number bắt đầu
                              1,           // số lượng minor muốn cấp
                              "rwdev_01"); // tên device dùng khi log, không phải tên trong /dev/
    if (ret < 0)
    {
        pr_err("Cannot allocate device number\n");
        return ret;
    }
    pr_info("Major = %d, Minor = %d\n", MAJOR(dev_num), MINOR(dev_num));

    /**********Bước2: Khởi tạo đối tượng struct cdev, gắn với file_operations(open,close,...)************/
    cdev_init(&rwbuf_cdev, &my_fops); //

    /**********Bước3: Đăng ký cdev với kernel, từ đây kernel biết rằng Major/Minor này được xử lý bởi driver của bạn************/
    ret = cdev_add(&rwbuf_cdev, dev_num, 1); // tham số 3 là số lượng minor mà my_cdev sẽ quản lý
    if (ret < 0)
    {
        pr_err("Cannot add cdev\n");
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    /******Bước 4: Tạo một device class trong /sys/class/. Đây chỉ là nơi để nhóm các thiết bị cùng loại, chưa tạo /dev node.****/
    my_class = class_create("rwbufdev"); // tạo class trong /sys/class/rwbufdev
    if (IS_ERR(my_class))
    {
        pr_err("Cannot create class\n");
        cdev_del(&rwbuf_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(my_class);
    }

    /******Bước 5: Tạo device trong class, sinh sysfs entry tương ứng và (thông qua udev/devtmpfs) tạo /dev/<device_name>. */
    rwbuf_dev = device_create(my_class,    // device này thuộc class my_class
                              NULL,        // parent device
                              dev_num,     // major + minor
                              NULL,        // driver private data
                              "rwbufdev"); // kernel sẽ tạo /dev/rwbufdev
    if (IS_ERR(rwbuf_dev))
    {
        pr_err("Cannot create device\n");
        class_destroy(my_class);
        cdev_del(&rwbuf_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(rwbuf_dev);
    }
    return 0;
}

// Hàm được gọi khi gỡ module: sudo rmmod .ko
static void __exit rwbufdev_exit(void)
{
    // giải phóng device_create()
    device_destroy(my_class, dev_num);

    // giải phóng class_create();
    class_destroy(my_class);

    // giải phóng cdev_add();
    cdev_del(&rwbuf_cdev);

    // giải phóng alloc_chrdev_region()
    unregister_chrdev_region(dev_num, 1);
    pr_info("Device number released\n");
}

// Khai báo entry và exit của module
module_init(rwbufdev_init);
module_exit(rwbufdev_exit);

// Khai báo License của module
MODULE_LICENSE("GPL");