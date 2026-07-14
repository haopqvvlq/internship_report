# Misc Character Device Driver (Echo Driver)

Một ví dụ đơn giản về **Linux Misc Character Device Driver**.

Driver tạo ra một device:

```
/dev/echo_demo
```

Cho phép:

- `write()` dữ liệu từ User Space vào Kernel Space.
- `read()` đọc lại chính dữ liệu đó (echo).

Đây là ví dụ phù hợp để học:

- Linux Kernel Module
- Character Device Driver
- Misc Device
- `struct file_operations`
- `copy_to_user()`
- `copy_from_user()`

---

# Kiến trúc

```
User Application
        │
        │ read() / write()
        ▼
+----------------------+
|      System Call     |
+----------------------+
        │
        ▼
+----------------------+
|         VFS          |
+----------------------+
        │
        ▼
struct file_operations
        │
        ▼
+----------------------+
|   echo_demo Driver   |
|                      |
|  echo_read()         |
|  echo_write()        |
+----------------------+
        │
        ▼
Kernel Buffer (kbuf[])
```

---

# Cấu trúc driver

Driver gồm các thành phần chính:

## 1. Kernel Buffer

```c
#define BUF_SIZE 256

static char kbuf[BUF_SIZE];
static size_t kbuf_len;
```

Lưu dữ liệu bên trong **Kernel Space**.

- `kbuf` : vùng nhớ lưu dữ liệu.
- `kbuf_len` : số byte hiện đang chứa.

---

## 2. Hàm `write()`

```c
echo_write()
```

Được VFS gọi khi User Space thực hiện:

```c
write(fd, buf, len);
```

Quy trình:

```
User Buffer
      │
      ▼
copy_from_user()
      │
      ▼
Kernel Buffer
```

Các bước:

1. Giới hạn kích thước ghi không vượt quá `BUF_SIZE`.
2. Copy dữ liệu từ User Space sang Kernel Space.
3. Cập nhật `kbuf_len`.
4. Trả về số byte đã ghi.

---

## 3. Hàm `read()`

```c
echo_read()
```

Được gọi khi User Space thực hiện:

```c
read(fd, buf, len);
```

Quy trình:

```
Kernel Buffer
      │
      ▼
copy_to_user()
      │
      ▼
User Buffer
```

Các bước:

1. Kiểm tra số byte yêu cầu đọc.
2. Không đọc quá dữ liệu đang có.
3. Copy dữ liệu sang User Space.
4. Trả về số byte đã đọc.

---

## 4. `struct file_operations`

```c
static struct file_operations echo_fops = {
    .owner = THIS_MODULE,
    .read  = echo_read,
    .write = echo_write,
};
```

Đây là **cầu nối giữa VFS và Driver**.

Khi chương trình gọi:

```
read()
```

VFS sẽ gọi:

```
echo_read()
```

Khi chương trình gọi:

```
write()
```

VFS sẽ gọi:

```
echo_write()
```

---

## 5. Misc Device

```c
static struct miscdevice echo_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = "echo_demo",
    .fops  = &echo_fops,
};
```

Driver sử dụng **Misc Device Framework**.

Ưu điểm:

- Không cần tự cấp Major Number.
- Kernel tự quản lý Minor Number.
- Code ngắn gọn hơn so với Character Driver truyền thống.

Sau khi đăng ký thành công:

```
/dev/echo_demo
```

sẽ được tạo.

---

## 6. Module Initialization

```c
echo_init()
```

Được gọi khi:

```
sudo insmod echo_demo.ko
```

Thực hiện:

```
misc_register(&echo_misc);
```

Kernel sẽ:

- đăng ký device
- cấp Minor Number
- tạo `/dev/echo_demo`

---

## 7. Module Exit

```c
echo_exit()
```

Được gọi khi:

```
sudo rmmod echo_demo
```

Thực hiện:

```
misc_deregister()
```

để hủy device.

---

# Luồng hoạt động

## Ghi dữ liệu

```
echo "Hello" > /dev/echo_demo
```

```
User Space
     │
write()
     │
     ▼
VFS
     │
     ▼
echo_write()
     │
copy_from_user()
     │
     ▼
Kernel Buffer
```

---

## Đọc dữ liệu

```
cat /dev/echo_demo
```

```
Kernel Buffer
      │
copy_to_user()
      │
      ▼
echo_read()
      │
      ▼
VFS
      │
      ▼
User Space
```

---

# Build

```bash
make
```

Sinh ra:

```
echo_demo.ko
```

---

# Nạp module

```bash
sudo insmod echo_demo.ko
```

Kiểm tra:

```bash
lsmod | grep echo_demo
```

---

# Kiểm tra device

```bash
ls -l /dev/echo_demo
```

---

# Ghi dữ liệu

```bash
sudo sh -c 'echo "Linux Kernel" > /dev/echo_demo'
```

---

# Đọc dữ liệu

```bash
sudo cat /dev/echo_demo
```

Kết quả:

```
Linux Kernel
```

---

# Gỡ module

```bash
sudo rmmod echo_demo
```

---

# Kiến thức rút ra

Qua ví dụ này có thể hiểu được:

- Cách viết một Linux Kernel Module.
- Cách tạo Character Device bằng Misc Framework.
- Vai trò của `struct file_operations`.
- Quan hệ giữa User Space và Kernel Space.
- Cách sử dụng `copy_to_user()` và `copy_from_user()`.
- Luồng hoạt động của `read()` và `write()` thông qua VFS.
- Cách đăng ký và hủy một Misc Device với `misc_register()` và `misc_deregister()`.
