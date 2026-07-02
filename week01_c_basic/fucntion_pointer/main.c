/*Bài tập: Mô phỏng file_operations của Linux
Mục tiêu

Tạo một struct file_operations chứa các function pointer và mô phỏng cách Kernel gọi các callback của driver.*/

#include <stdio.h>
#include <string.h>

// Định nghĩa interface
typedef struct
{
    int (*open)(void);
    int (*read)(char *buf);
    int (*write)(const char *buf);
    int (*close)(void);
} file_operations;

// LED Driver
int led_open(void)
{
    printf("LED Driver: Open\n");
    return 0;
}

int led_read(char *buf)
{
    strcpy(buf, "LED State: OFF");
    return 0;
}

int led_write(const char *buf)
{
    printf("LED Driver: Write -> %s\n", buf);
    return 0;
}

// "Kernel" giả lập
void kernel_test(file_operations *fops)
{
    char buffer[100];

    fops->open();

    fops->read(buffer);
    printf("Kernel received: %s\n", buffer);

    fops->write("Turn LED ON");

    fops->close();
}
int led_close(void)
{
    printf("LED Driver: Close\n");
    return 0;
}

int main()
{
    // Đăng ký Driver
    file_operations led_fops = {
        .open = led_open,
        .read = led_read,
        .write = led_write,
        .close = led_close};

    kernel_test(&led_fops);
    return 0;
}