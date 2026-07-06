# Function Pointer - Mô phỏng `file_operations` của Linux

## Giới thiệu

Đây là bài tập mô phỏng cơ chế `file_operations` trong Linux Kernel bằng ngôn ngữ C.

Trong Linux, mỗi driver sẽ đăng ký một tập các hàm callback (như `open`, `read`, `write`, `close`) thông qua cấu trúc `file_operations`. Khi người dùng gọi các system call tương ứng, Kernel sẽ thực thi callback của driver.

Project này tái hiện cơ chế đó bằng cách sử dụng **function pointer**.

---

## Kiến thức sử dụng

- C Programming
- Function Pointer
- Struct
- Callback Function
- Interface trong C
- Mô phỏng kiến trúc Linux Device Driver

---

## Cấu trúc project

```
.
├── main.c
├── Makefile
└── README.md
```

---

## Cách build

Sử dụng Makefile:

```bash
make
```

Hoặc biên dịch thủ công:

```bash
gcc -Wall -Wextra main.c -o main
```

---

## Cách chạy

```bash
./main
```

---

## Kết quả mong đợi

```
LED Driver: Open
Kernel received: LED State: OFF
LED Driver: Write -> Turn LED ON
LED Driver: Close
```

---

## Luồng hoạt động

```
main()
   │
   │ đăng ký callback
   ▼
file_operations
   │
   ▼
kernel_test()
   │
   ├── open()
   ├── read()
   ├── write()
   └── close()
```

---

## Giải thích chương trình

### 1. Định nghĩa interface

```c
typedef struct
{
    int (*open)(void);
    int (*read)(char *buf);
    int (*write)(const char *buf);
    int (*close)(void);
} file_operations;
```

Cấu trúc `file_operations` chứa các **function pointer**, đóng vai trò như một interface mô phỏng Linux Kernel.

---

### 2. Driver

Driver LED triển khai các callback:

- `led_open()`
- `led_read()`
- `led_write()`
- `led_close()`

Đây là các hàm sẽ được Kernel gọi thông qua function pointer.

---

### 3. Đăng ký Driver

```c
file_operations led_fops = {
    .open = led_open,
    .read = led_read,
    .write = led_write,
    .close = led_close
};
```

Quá trình này tương tự việc một Linux driver đăng ký các callback với Kernel.

---

### 4. Kernel gọi callback

```c
kernel_test(&led_fops);
```

Trong `kernel_test()`, chương trình không biết đang làm việc với driver nào mà chỉ gọi các callback thông qua interface:

```c
fops->open();
fops->read(buffer);
fops->write(...);
fops->close();
```

Đây chính là cơ chế **callback** được Linux Kernel sử dụng.

---

## Kiến thức rút ra

- Cách khai báo và sử dụng function pointer.
- Sử dụng struct để xây dựng interface trong C.
- Áp dụng callback nhằm tách biệt phần gọi hàm và phần cài đặt.
- Hiểu cách Linux Kernel giao tiếp với driver thông qua `file_operations`.
- Nắm được tư tưởng đa hình (polymorphism) trong C bằng function pointer.

---

## Hướng phát triển

Có thể mở rộng chương trình bằng cách:

- Thêm nhiều driver (UART, Button, Sensor,...).
- Cho Kernel lựa chọn driver tại runtime.
- Bổ sung các callback khác như:
  - `ioctl`
  - `poll`
  - `mmap`
  - `release`
- Mô phỏng quá trình đăng ký nhiều driver trong Kernel.

---

## Tài liệu tham khảo

- Linux Device Drivers, Third Edition (LDD3)
- `man gcc`
- Linux Kernel Documentation về `struct file_operations`
