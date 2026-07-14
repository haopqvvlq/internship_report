#include <linux/init.h>       //module_init(), module_exit()
#include <linux/module.h>     // THIS_MODULE, MODULE_LICENSE
#include <linux/miscdevice.h> // misc_register(), struct miscdevice
#include <linux/fs.h>         // struct file_operations
#include <linux/uaccess.h>    //copy_to_user(), copy_from_user()

// Bộ đệm nằm trong kernel space
#define BUF_SIZE 256

static char kbuf[BUF_SIZE]; // vùng nhớ lưu dữ liệu trong kernel
static size_t kbuf_len = 0; // số byte dữ liệu hiện đang có

// Hàm read()
// Được VFS gọi khi user gọi syscall: read(fd, user_buf, len)
static ssize_t echo_read(struct file *filep, char __user *buf, size_t len, loff_t *offset)
{
    /* Đã đọc hết dữ liệu */
    if (*offset >= kbuf_len)
        return 0;

    /* Chỉ đọc phần còn lại */
    if (len > kbuf_len - *offset)
        len = kbuf_len - *offset;

    // Copy dữ liệu từ Kernel Space -> User Space
    if (copy_to_user(buf, kbuf, len))
        return -EFAULT;

    *offset += len;

    // Trả về số byte đã đọc
    return len;
}

// Hàm write()
// Được VFS gọi khi user gọi syscall: write(fd, user_buf, len)
static ssize_t echo_write(struct file *filep, const char __user *buf, size_t len, loff_t *offset)
{
    // Không cho ghi vượt kích thước buffer
    if (len > BUF_SIZE)
        len = BUF_SIZE;

    // Copy dữ liệu từ User Space -> Kernel Space
    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    // Cập nhật số byte hiện có
    kbuf_len = len;

    // Trả về số byte đã ghi
    return len;
}

// "Hợp đồng" giữa VFS và driver
// VFS nhìn vào đây để biết driver hỗ trợ syscall nào
static struct file_operations echo_fops = {
    .owner = THIS_MODULE,
    .read = echo_read,   // read() -> echo_read()
    .write = echo_write, // write() -> echo_write()
};

// Khai báo một Misc Device
//  Kernel sẽ tự tạo: /dev/echo_demo
static struct miscdevice echo_misc = {
    .minor = MISC_DYNAMIC_MINOR, // kernel tự cấp minor number
    .name = "echo_demo",         // tên device trong /dev
    .fops = &echo_fops,          // file_operations của device
};

// Hàm được gọi khi nạp module: sudo insmod echo_demo.ko
static int __init echo_init(void)
{
    // Đăng ký misc device với kernel
    // Sau khi thành công sẽ có /dev/echo_demo
    return misc_register(&echo_misc);
}

// Hàm được gọi khi gỡ module: sudo rmmod echo_demo
static void __exit echo_exit(void)
{
    // Hủy đăng ký device
    misc_deregister(&echo_misc);
}

// Khai báo entry và exit của module
module_init(echo_init);
module_exit(echo_exit);

// Khai báo License của module
MODULE_LICENSE("GPL");