# 6 Milestones — Linux BSP: Device Tree → Platform Driver → GPIO → User Space

1. Device Tree → Platform Device
2. Platform Driver → GPIO Subsystem
3. Create /dev/myled
4. User Space → write()
5. User Space C Application
6. Debug & Verify End-to-End Flow

## Milestone 1 — Device Tree → Platform Device

**Chưa có GPIO.**

Tạo một node đơn giản trong Device Tree:

```dts
myled {
    compatible = "mycompany,myled";
    status = "okay";
};
```

Driver:

```c
static int myled_probe(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "myled probe\n");
    return 0;
}
```

### Mục tiêu

Hiểu flow:

```text
Device Tree
     ↓
Kernel tạo Platform Device
     ↓
Platform Driver match
     ↓
probe()
```

### Câu hỏi cần tự trả lời

> Tại sao `probe()` được gọi?

Nếu chưa giải thích được flow này thì **chưa nên sang GPIO**.

---

## Milestone 2 — Platform Driver → GPIO Subsystem

Thêm GPIO vào Device Tree:

```dts
led-gpios = <...>;
```

Trong driver:

```c
struct gpio_desc *led_gpio;

led_gpio = devm_gpiod_get(&pdev->dev, "led", GPIOD_OUT_LOW);
```

Sau đó điều khiển GPIO:

```c
gpiod_set_value(led_gpio, 1);
```

### Flow

```text
led-gpios
     ↓
devm_gpiod_get(..., "led", ...)
     ↓
GPIO Subsystem
     ↓
GPIO Hardware
```

### Mục tiêu

Khi `probe()` được gọi:

1. Đọc GPIO từ Device Tree
2. Request GPIO thông qua GPIO Subsystem
3. Điều khiển GPIO
4. Bật LED

**Chưa tạo `/dev`.**

---

## Milestone 3 — Tạo `/dev/myled`

Bắt đầu thêm User Space Interface.

Với bài tập học tập, nên ưu tiên sử dụng:

```c
miscdevice
```

Thay vì bắt đầu ngay với:

```text
alloc_chrdev_region()
     ↓
cdev_init()
     ↓
cdev_add()
     ↓
class_create()
     ↓
device_create()
```

### Concept flow

```text
probe()
     ↓
misc_register()
     ↓
/dev/myled
```

### Mục tiêu

Sau khi driver bind thành công:

```bash
ls -l /dev/myled
```

phải thấy device node:

```text
/dev/myled
```

---

## Milestone 4 — User Space → `write()`

Test bằng:

```bash
echo 1 > /dev/myled
```

### Flow

```text
echo 1
   ↓
write()
   ↓
myled_write()
   ↓
gpiod_set_value()
   ↓
LED ON
```

Test tắt LED:

```bash
echo 0 > /dev/myled
```

### Mục tiêu

Hiểu được:

> User Space không điều khiển GPIO trực tiếp.

Thay vào đó:

```text
User Space
     ↓
/dev/myled
     ↓
Driver
     ↓
GPIO Subsystem
     ↓
Hardware
```

---

## Milestone 5 — Test bằng C User Space

Thay vì chỉ sử dụng:

```bash
echo 1 > /dev/myled
```

Viết một chương trình User Space:

```c
int fd = open("/dev/myled", O_WRONLY);

write(fd, "1", 1);
```

### Flow

```text
User Space
     ↓
open()
     ↓
write()
     ↓
system call
     ↓
VFS
     ↓
file_operations
     ↓
myled_write()
     ↓
GPIO Subsystem
     ↓
Hardware
```

### Mục tiêu

Hiểu rõ đường đi của một lệnh từ **User Space đến Hardware**.

---

## Milestone 6 — Debug và kiểm chứng toàn bộ Flow

Kiểm tra log kernel:

```bash
dmesg
```

Kiểm tra Platform Device:

```bash
ls /sys/bus/platform/devices/
```

Kiểm tra Platform Driver:

```bash
ls /sys/bus/platform/drivers/
```

Kiểm tra Device Tree:

```bash
ls /proc/device-tree/
```

Kiểm tra Device Node:

```bash
ls -l /dev/myled
```

### Mục tiêu cuối cùng

Phải chứng minh được toàn bộ flow:

```text
Device Tree Node
        ↓
Platform Device
        ↓
Platform Driver
        ↓
Driver bind
        ↓
probe()
        ↓
Read GPIO from Device Tree
        ↓
GPIO Subsystem
        ↓
misc_register()
        ↓
/dev/myled
        ↓
User Space write()
        ↓
Driver write()
        ↓
GPIO
        ↓
LED
```

### Câu hỏi cuối cùng cần tự trả lời

> Khi chạy `echo 1 > /dev/myled`, chính xác kernel đã làm gì từ đầu đến cuối?
