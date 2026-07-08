#include <stdio.h>
#include <string.h>

void (*isr)(void) = NULL;

typedef struct
{
    char name[20];
    char ver[20];
} driver_info;

typedef struct
{
    driver_info file_driver_info;
    void (*read)(void);
    void (*write)(void);
    void (*close)(void);
    void (*open)(void);

} file_operations;

typedef struct
{
    driver_info i2c_driver_info;
    void (*probe)(void);
    void (*suspend)(void);
    void (*remove)(void);
} i2c_driver;

// isr
void isr_register(void *callback)
{
    isr = callback;
}

// my_driver function
void my_driver_open(void)
{
    printf("LED_driver: OPEN\n");
}
void my_driver_close(void)
{
    printf("LED_driver: CLOSE\n");
}
void my_driver_read(void)
{
    printf("LED_STATE: ON\n");
}
void my_driver_write(void)
{
    printf("ON or OFF: \n");
}
void my_driver_isr(void)
{
    printf("off led immediately\n");
}

void kernel_test(file_operations *drv)
{
    isr();
    drv->open();
    drv->read();
    drv->write();
    drv->close();
}
int main(void)
{
    file_operations led_driver;
    led_driver.file_driver_info = (driver_info){"led_7_segment", "1.2.3"};
    led_driver.read = my_driver_read;
    led_driver.write = my_driver_write;
    led_driver.open = my_driver_open;
    led_driver.close = my_driver_close;
    isr_register(my_driver_isr);
    kernel_test(&led_driver);
    return 0;
}