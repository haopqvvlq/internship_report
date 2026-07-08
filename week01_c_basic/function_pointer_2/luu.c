#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct
{
    char name[20];
    char version[20];
} driver_info;

typedef struct
{
    void (*open)(void);
    void (*close)(void);
    void (*read)(void);
    void (*write)(void);
} file_operations;

typedef struct
{
    driver_info i2c_driver_info;
    void (*probe)(void);
    void (*remove)(void);
    void (*suspend)(void);
} i2c_driver;

// hao_i2c function
void hao_probe(void)
{
    printf("I'm probing\n");
}
void hao_remove(void)
{
    printf("I'm removing\n");
}
void hao_suspend(void)
{
    printf("I'm going to suspend\n");
}

// Kernel test
void kernel_test(i2c_driver *drv)
{
    printf("device: %s\n", drv->i2c_driver_info.name);
    drv->probe();
    drv->remove();
    drv->suspend();
}
int main(void)
{
    i2c_driver hao_i2c_driver;
    hao_i2c_driver.i2c_driver_info = (driver_info){"DS3231", "3.2.4"};
    hao_i2c_driver.probe = hao_probe;
    hao_i2c_driver.remove = hao_remove;
    hao_i2c_driver.suspend = hao_suspend;
    kernel_test(&hao_i2c_driver);
    return 0;
}