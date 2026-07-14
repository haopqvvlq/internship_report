# Traditional Character Device Driver - Quá trình khởi tạo

## Mục tiêu

Để một chương trình ở **user space** có thể:

```c
open("/dev/mychardev");
read();
write();
ioctl();
close();
```

thì kernel cần biết hai điều:

1. **Ai sẽ xử lý các system call này?**
2. **Làm sao user space tìm thấy thiết bị?**

Vì vậy, quá trình khởi tạo thường được chia thành hai phần:

```text
┌──────────────────────────────────────┐
│ 1. Đăng ký Character Device          │
└──────────────────────────────────────┘

alloc_chrdev_region()
        │
        ▼
cdev_init()
        │
        ▼
cdev_add()


┌──────────────────────────────────────┐
│ 2. Tạo Device cho User Space         │
└──────────────────────────────────────┘

class_create()
        │
        ▼
device_create()
```

---

# 1. alloc_chrdev_region()

## Mục đích

Xin kernel cấp phát **Device Number**.

Device Number gồm:

* Major Number
* Minor Number

Ví dụ:

```text
Major = 240
Minor = 0
```

## Prototype

```c
int alloc_chrdev_region(dev_t *dev,
                        unsigned baseminor,
                        unsigned count,
                        const char *name);
```

## Sau khi thành công

```text
dev_t
│
├── Major
└── Minor
```

Có thể lấy bằng:

```c
MAJOR(dev_num);
MINOR(dev_num);
```

Ví dụ:

```c
dev_t dev_num;

alloc_chrdev_region(&dev_num, 0, 1, "mychardev");

pr_info("Major=%d Minor=%d\n",
        MAJOR(dev_num),
        MINOR(dev_num));
```

## Hàm giải phóng

```c
unregister_chrdev_region(dev_num, count);
```

---

# 2. cdev_init()

## Mục đích

Khởi tạo một `struct cdev` và liên kết nó với `file_operations`.

```text
struct cdev
        │
        ▼
file_operations
        │
        ├── open()
        ├── read()
        ├── write()
        ├── ioctl()
        └── release()
```

## Prototype

```c
void cdev_init(struct cdev *cdev,
               const struct file_operations *fops);
```

Ví dụ:

```c
cdev_init(&my_cdev, &my_fops);
```

## Lưu ý

`cdev_init()` chỉ khởi tạo đối tượng trong bộ nhớ.

Kernel vẫn chưa biết character device này tồn tại.

---

## struct cdev là gì?

`struct cdev` là đối tượng đại diện cho **Character Device** trong kernel.

Bên trong nó chứa:

```text
struct cdev
        │
        ▼
file_operations
```

Khi user space gọi:

```c
open();
read();
write();
ioctl();
```

kernel sẽ thông qua `struct cdev` để tìm tới các callback tương ứng trong `file_operations`.

---

# 3. cdev_add()

## Mục đích

Đăng ký `struct cdev` với kernel.

Liên kết:

```text
dev_t (Major/Minor)
        │
        ▼
struct cdev
        │
        ▼
file_operations
```

## Prototype

```c
int cdev_add(struct cdev *cdev,
             dev_t dev,
             unsigned count);
```

Ví dụ:

```c
cdev_add(&my_cdev, dev_num, 1);
```

## Sau bước này

Kernel đã biết:

```text
Major/Minor
        │
        ▼
struct cdev
        │
        ▼
file_operations
```

Nói cách khác:

```text
dev_t → cdev → file_operations
```

đã được đăng ký vào kernel.

Lúc này kernel đã biết:

> Với Major/Minor này thì callback nào sẽ xử lý open(), read(), write(), ioctl(), release().

---

## Điểm quan trọng

Sau bước này:

```text
/dev/mychardev
```

vẫn chưa tồn tại.

Kernel chỉ mới biết:

```text
Major/Minor
        │
        ▼
Character Device
```

chứ user space vẫn chưa nhìn thấy thiết bị.

---

# 4. class_create()

## Mục đích

Tạo một **Device Class** trong Linux Device Model.

Ví dụ:

```text
/sys/class/

├── tty
├── leds
├── net
└── mychardev
```

## Prototype

```c
struct class *class_create(const char *name);
```

Ví dụ:

```c
my_class = class_create("mychardev");
```

## Sau bước này

Kernel tạo:

```text
/sys/class/mychardev/
```

---

## Linux Device Model

Linux tổ chức thiết bị theo mô hình:

```text
Class
  │
  ├── Device
  ├── Device
  └── Device
```

Ví dụ:

```text
Class: tty

├── tty0
├── tty1
├── ttyS0
└── ttyUSB0
```

Class giúp hệ thống quản lý các thiết bị cùng loại.

---

# 5. device_create()

## Mục đích

Tạo một Device Object thuộc class vừa tạo.

## Prototype

```c
struct device *device_create(struct class *class,
                             struct device *parent,
                             dev_t devt,
                             void *drvdata,
                             const char *fmt,
                             ...);
```

Ví dụ:

```c
device_create(my_class,
              NULL,
              dev_num,
              NULL,
              "mychardev");
```

## Sau bước này

Kernel tạo:

```text
/sys/class/mychardev/mychardev
```

và (nếu hệ thống có devtmpfs/udev):

```text
/dev/mychardev
```

Lúc này user space mới có thể:

```c
open("/dev/mychardev");
```

---

## Lưu ý quan trọng

Character Device vẫn hoạt động được nếu không dùng:

```c
class_create();
device_create();
```

Khi đó chỉ cần tạo node thủ công:

```bash
mknod /dev/mychardev c <major> <minor>
```

Ví dụ:

```bash
mknod /dev/mychardev c 240 0
```

User space vẫn có thể:

```c
open("/dev/mychardev");
```

Điều này cho thấy:

```text
cdev_add()
```

mới là phần cốt lõi của Character Device Driver.

`class_create()` và `device_create()` chủ yếu giúp tự động tạo device node và tích hợp với Linux Device Model.

---

# Luồng hoạt động tổng thể

```text
User Space

open("/dev/mychardev")
        │
        ▼
VFS
        │
        ▼
inode->i_rdev
        │
        ▼
Major + Minor
        │
        ▼
Tìm struct cdev
        │
        ▼
cdev->ops
        │
        ▼
my_open()
```

Các thao tác khác cũng tương tự:

```text
read()   → my_read()
write()  → my_write()
ioctl()  → my_ioctl()
close()  → my_release()
```

---

# Bức tranh tổng thể

```text
                  Traditional Character Device Driver

                    alloc_chrdev_region()
                              │
                              ▼
                   Device Number (Major/Minor)
                              │
                              ▼
                        cdev_init()
                              │
                              ▼
                  struct cdev + file_operations
                              │
                              ▼
                         cdev_add()
                              │
                              ▼
              dev_t → cdev → file_operations
                              │
                              ▼
                       class_create()
                              │
                              ▼
                 Linux Device Model Class
                              │
                              ▼
                      device_create()
                              │
                              ▼
                 Device Object + /dev node
                              │
──────────────────────────────────────────────────────
                              │
                              ▼
                         User Space
                              │
                              ▼
                  open("/dev/mychardev")
                              │
                              ▼
                             VFS
                              │
                              ▼
                       Major / Minor
                              │
                              ▼
                        struct cdev
                              │
                              ▼
                     file_operations
                              │
          ┌──────────┬─────────┬──────────┬──────────┐
          ▼          ▼         ▼          ▼          ▼
        open()     read()    write()    ioctl()   release()
```

---

# Thứ tự khởi tạo

```text
alloc_chrdev_region()
        │
        ▼
cdev_init()
        │
        ▼
cdev_add()
        │
        ▼
class_create()
        │
        ▼
device_create()
```

---

# Thứ tự giải phóng

Giải phóng theo thứ tự ngược lại:

```text
device_destroy()
        ▲
class_destroy()
        ▲
cdev_del()
        ▲
unregister_chrdev_region()
```

---

# Ý nghĩa của từng bước

| Hàm                     | Vai trò                                             |
| ----------------------- | --------------------------------------------------- |
| `alloc_chrdev_region()` | Xin cấp phát Major/Minor Number                     |
| `cdev_init()`           | Khởi tạo `struct cdev`, gắn `file_operations`       |
| `cdev_add()`            | Đăng ký `dev_t → cdev → file_operations` với kernel |
| `class_create()`        | Tạo Device Class trong Linux Device Model           |
| `device_create()`       | Tạo Device Object và `/dev` node cho user space     |

---

# Trong thực tế BSP

Mô hình trên thường được dùng khi tự xây dựng Character Device Driver từ đầu.

Tuy nhiên nhiều driver trong kernel thuộc các subsystem như:

* I2C
* SPI
* GPIO
* Input
* ALSA
* DRM
* Network
* IIO

thường không cần tự thực hiện toàn bộ quy trình này.

Subsystem sẽ hỗ trợ hoặc quản lý phần lớn việc đăng ký thiết bị.

Vì vậy khi đọc driver thực tế, bạn có thể không thấy:

```c
alloc_chrdev_region();
cdev_init();
cdev_add();
```

xuất hiện trực tiếp trong driver.

---

# Tóm tắt

Toàn bộ quá trình trên thực chất phục vụ hai mục tiêu.

## 1. Đăng ký khả năng xử lý của driver

```text
alloc_chrdev_region()
        │
        ▼
cdev_init()
        │
        ▼
cdev_add()
```

Sau ba bước này:

```text
dev_t
  │
  ▼
struct cdev
  │
  ▼
file_operations
```

đã được kernel đăng ký.

---

## 2. Đưa thiết bị ra cho User Space sử dụng

```text
class_create()
        │
        ▼
device_create()
```

Sau hai bước này hệ thống tạo:

* Device Object trong Linux Device Model
* `/dev/<device_name>`

để user space có thể:

```c
open();
read();
write();
ioctl();
```

và giao tiếp với driver.

---

## Một câu để nhớ

```text
Ba bước đầu tạo liên kết:

dev_t → cdev → file_operations

Hai bước cuối tạo Device Object và /dev node
để user space nhìn thấy và sử dụng thiết bị.
```
