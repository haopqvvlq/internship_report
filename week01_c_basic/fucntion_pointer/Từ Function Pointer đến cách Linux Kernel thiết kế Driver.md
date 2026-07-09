# Từ Function Pointer đến cách Linux Kernel thiết kế Driver

Điều thú vị là ban đầu ta chỉ học về **function pointer** trong C, nhưng khi tìm hiểu sâu hơn sẽ thấy đây chính là nền tảng của rất nhiều cơ chế trong Linux Kernel, đặc biệt là cách xây dựng các **subsystem** và **device driver**.

---

# 1. Function Pointer trong C

Đầu tiên ta học:

```c
int add(int a, int b);

int (*func)(int, int) = add;
```

`func` là một biến lưu địa chỉ của hàm `add()`.

Ta có thể gọi:

```c
func(1, 2);
```

thay vì:

```c
add(1, 2);
```

Ở bước này, **function pointer** chỉ đơn giản là một tính năng của ngôn ngữ C.

---

# 2. Gom nhiều Function Pointer vào một struct

Ví dụ:

```c
struct calculator_ops {
    int (*add)(int, int);
    int (*sub)(int, int);
    int (*mul)(int, int);
};
```

Lúc này, `struct` không còn chỉ chứa dữ liệu mà còn mô tả một nhóm các thao tác liên quan.

Đây là bước chuyển từ:

- Một function pointer

sang

- Một tập các thao tác (**operations**).

---

# 3. Struct chứa Function Pointer đóng vai trò như một Interface

Ví dụ:

```c
struct storage_ops {
    int (*read)(...);
    int (*write)(...);
    int (*erase)(...);
};
```

`struct storage_ops` **không thực hiện** việc đọc, ghi hay xóa dữ liệu.

Nó chỉ định nghĩa rằng một thiết bị lưu trữ cần cung cấp các thao tác:

- `read()`
- `write()`
- `erase()`

Có thể xem đây là một **bản hợp đồng (contract)** hay **interface**: nó quy định các thao tác cần có, còn cách thực hiện sẽ do từng driver quyết định.

---

# 4. Mỗi Driver hiện thực Interface theo cách riêng

Ví dụ:

```c
struct storage_ops nor_ops = {
    .read  = nor_read,
    .write = nor_write,
    .erase = nor_erase,
};

struct storage_ops nand_ops = {
    .read  = nand_read,
    .write = nand_write,
    .erase = nand_erase,
};
```

Cả **NOR Flash** và **NAND Flash** đều cung cấp cùng ba thao tác:

- `read()`
- `write()`
- `erase()`

nhưng cách hiện thực bên trong hoàn toàn khác nhau.

---

# 5. Linux Kernel cũng áp dụng cách thiết kế này

Trong Linux Kernel có rất nhiều `struct` tương tự, chẳng hạn:

- `struct file_operations`
- `struct i2c_driver`
- `struct spi_driver`
- `struct usb_driver`

Các `struct` này định nghĩa những callback hoặc các thao tác mà driver cần cung cấp.

---

# 6. Driver hiện thực và đăng ký với Kernel

Ví dụ:

```c
static struct i2c_driver eeprom_driver = {
    .probe  = eeprom_probe,
    .remove = eeprom_remove,
};
```

Ở đây, driver cung cấp các hàm `probe()` và `remove()` theo yêu cầu của `struct i2c_driver`, sau đó đăng ký với **I2C Core**.

---

# 7. Kernel chỉ làm việc thông qua Interface

Khi một thiết bị I2C được phát hiện, **I2C Core** sẽ gọi:

```c
driver->probe(client);
```

Kernel không cần biết driver đó điều khiển:

- EEPROM
- LM75
- MPU6050
- hay bất kỳ thiết bị nào khác.

Kernel chỉ cần biết driver đã cung cấp hàm `probe()` và gọi hàm đó khi cần.

Nhờ vậy, cùng một đoạn mã của Kernel có thể làm việc với rất nhiều loại thiết bị khác nhau.

---

# 8. Device Driver chỉ sử dụng API của bus (I2C/SPI/USB...), còn việc điều khiển phần cứng bus ở phía CPU/SoC (như I2C Controller, SPI Controller...) là trách nhiệm của Controller Driver.

Ví dụ với I2C:

```text
Device Driver
        │
        ▼
I2C Core
        │
        ▼
I2C Controller Driver
        │
        ▼
I2C Hardware
```

`Device Driver` chỉ gọi các API như:

```c
i2c_smbus_read_byte_data(...);
```

Việc tạo tín hiệu **SDA**, **SCL** hay điều khiển bộ điều khiển I2C là trách nhiệm của **I2C Controller Driver**.

Nhờ sự phân lớp này, driver thiết bị không phụ thuộc vào phần cứng I2C cụ thể.

---

# 9. Tư tưởng thiết kế của Linux Kernel

Linux Kernel được chia thành nhiều lớp, mỗi lớp giao tiếp với lớp khác thông qua các interface.

```text
Application
      │
      ▼
System Call
      │
      ▼
Kernel Subsystem
      │
      ▼
Interface (struct + function pointer)
      │
      ▼
Device Driver
      │
      ▼
Bus/Controller Driver
      │
      ▼
Hardware
```

Mỗi thành phần chỉ cần biết các interface mà nó sử dụng, không cần biết chi tiết cách các interface đó được hiện thực.

---

# 10. Kết luận

Có thể tóm gọn ý tưởng như sau:

> Linux Kernel sử dụng các `struct` chứa **function pointer** để định nghĩa **interface** giữa các thành phần. Mỗi driver hiện thực các hàm theo interface đó rồi đăng ký với Kernel. Khi cần, Kernel chỉ gọi các hàm thông qua interface mà không cần biết chi tiết cách driver hay phần cứng được hiện thực.

Đây cũng chính là lý do **function pointer** là một trong những kiến thức quan trọng nhất khi học lập trình hệ thống và Linux Kernel.
