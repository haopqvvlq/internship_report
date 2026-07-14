# Character Device Driver - Đọc/Ghi Buffer & IOCTL

Project này là một bài tập thực hành viết một **Traditional Character Device Driver** (không dùng misc framework) cho Linux. Driver sẽ quản lý một bộ đệm (buffer) tĩnh, cho phép user space đọc, ghi dữ liệu và truy xuất thống kê thông qua system call `ioctl`.

---

## 1. Cấu trúc Project và Ý nghĩa

```text
practice/
├── app/
│   ├── Makefile
│   └── test.c          # Nơi thao tác của user space qua menu lựa chọn với buffer
├── driver/
│   ├── Makefile
│   └── rwbufdev.c      # Nơi chứa mã nguồn traditional character device driver
└── include/
    └── rwbuf_ioctl.h   # Nơi chứa data và ioctl command chung giữa user space và kernel space
```

- **`app/`**: Chứa chương trình ở không gian người dùng (user space). Ứng dụng này tương tác với driver thông qua các system call cơ bản để hiển thị menu cho người dùng thao tác.
- **`driver/`**: Chứa module kernel điều khiển thiết bị (kernel space). Nó cấp phát định danh thiết bị, tạo device node và thực thi các lời gọi hệ thống từ user space.
- **`include/`**: Chứa các khai báo được sử dụng chung (shared header) bởi cả ứng dụng (app) và trình điều khiển (driver), như các magic number của ioctl và cấu trúc dữ liệu truyền nhận.

---

## 2. Các Kiến Thức Sử Dụng

- Đăng ký và quản lý Traditional Character Device Driver:
  - Cấp phát Device Number (`alloc_chrdev_region`).
  - Khởi tạo và đăng ký `cdev` với kernel (`cdev_init`, `cdev_add`).
  - Tạo device node `/dev/rwbufdev` (`class_create`, `device_create`).
- Định nghĩa các hành vi của file (`struct file_operations`).
- Giao tiếp dữ liệu giữa User Space và Kernel Space (`copy_to_user`, `copy_from_user`).
- Triển khai custom system call bằng `ioctl` (`unlocked_ioctl`).
- Quản lý bộ đệm (buffer) cấp phát tĩnh trong kernel.
- Viết ứng dụng User space để gọi các system call (`open`, `read`, `write`, `ioctl`, `close`).

---

## 3. Cách Build

### Bắt đầu từ tạo Character Device Driver (Kernel Space)
1. Di chuyển vào thư mục driver:
   ```bash
   cd driver
   ```
2. Biên dịch module:
   ```bash
   make
   ```
3. Nạp module vào kernel:
   ```bash
   sudo insmod rwbufdev.ko
   ```
4. Kiểm tra xem module đã được nạp thành công chưa:
   ```bash
   lsmod | grep rwbufdev
   ```
5. Kiểm tra log của kernel để xem quá trình khởi tạo (cấp phát Major/Minor):
   ```bash
   sudo dmesg | tail -20
   ```

### Sang bên User Space
1. Di chuyển vào thư mục app:
   ```bash
   cd ../app
   ```
2. Biên dịch chương trình ứng dụng:
   ```bash
   make
   ```

---

## 4. Cách Chạy

Tại thư mục `app` sau khi đã build thành công, chạy ứng dụng bằng quyền sudo (vì thiết bị trong `/dev/` có thể yêu cầu quyền root):
```bash
sudo ./app
```

---

## 5. Cách Gỡ Module

### Bên Driver
1. Di chuyển vào thư mục driver:
   ```bash
   cd ../driver
   ```
2. Gỡ module khỏi kernel:
   ```bash
   sudo rmmod rwbufdev
   ```
3. Kiểm tra xem module đã được gỡ chưa:
   ```bash
   lsmod | grep rwbufdev
   ```
4. Dọn dẹp các file sinh ra trong quá trình build:
   ```bash
   make clean
   ```

### Bên App (User Space)
1. Di chuyển vào thư mục app:
   ```bash
   cd ../app
   ```
2. Dọn dẹp các file executable và object:
   ```bash
   make clean
   ```

---

## 6. Kết Quả Mong Đợi

Khi chạy chương trình ở user space (`sudo ./app`), bạn sẽ thấy một Menu hiển thị:
```text
===== RWBUF MENU =====
1. Write
2. Read
3. Get Stats
4. Exit
Choose: 
```

**Các trường hợp chạy ra:**
- **Chọn `1` (Write):** Nhập vào một chuỗi (vd: `hello_driver`). Chuỗi này sẽ được gửi xuống driver thông qua hàm `write()` và lưu vào bộ đệm của kernel.
- **Chọn `2` (Read):** Đọc dữ liệu từ driver thông qua hàm `read()`. Sẽ in ra: `Data: hello_driver`.
- **Chọn `3` (Get Stats):** Ứng dụng gọi hàm `ioctl()` với cờ `RWBUF_IOCTL_GET_STATS`. In ra số lần `read` và `write` đã gọi thành công:
  ```text
  Read count   : 1
  Write count  : 1
  ```
- **Kiểm tra log của kernel:** Chạy `sudo dmesg` ở một terminal khác, bạn sẽ thấy log khi device mở, ghi (số lượng bytes), đọc (số lượng bytes), và đóng:
  ```text
  [  ...] Device_opened
  [  ...] written 12 bytes
  [  ...] read 12 bytes
  [  ...] Device closed
  ```

---

## 7. Luồng Hoạt Động

Dưới đây là sơ đồ tương tác giữa các thành phần của Project:
```
+----------------------+
| User Application     |
| read/write/ioctl()   |
+----------+-----------+
           |
           v
+----------------------+
| Virtual File System  |
+----------+-----------+
           |
           v
+----------------------+
| rwbufdev Driver      |
| file_operations      |
+----------+-----------+
           |
           v
+----------------------+
| Kernel Buffer        |
+----------------------+
```
---

## 8. Giải Thích Mã Nguồn Chi Tiết

Project được chia thành 3 phần chính tương ứng với các file mã nguồn:

### 8.1. Kernel Space (`driver/rwbufdev.c`)
Đây là module cốt lõi chạy trong Kernel Space, đóng vai trò quản lý thiết bị ký tự:
- **Quản lý dữ liệu**: Sử dụng một mảng tĩnh `device_buffer[256]` làm bộ đệm mô phỏng thiết bị thực. Module cũng duy trì 2 biến `read_count` và `write_count` để lưu trữ số liệu thống kê.
- **Khởi tạo và Hủy (Init & Exit)**: Hàm `rwbufdev_init()` tuần tự thực hiện các bước chuẩn bị: xin cấp phát bộ định danh Major/Minor (`alloc_chrdev_region`), khởi tạo thiết bị ký tự (`cdev_add`), và yêu cầu kernel tự động sinh ra file thiết bị `/dev/rwbufdev` thông qua `class_create` và `device_create`.
- **Cấu trúc File Operations (`my_fops`)**: 
  - `my_open()` / `my_release()`: Bắt sự kiện khi user mở hoặc đóng thiết bị, in ra log hệ thống để theo dõi.
  - `my_read()` / `my_write()`: Đảm nhiệm việc truyền tải dữ liệu một cách an toàn giữa bộ nhớ ảo của User Space và Kernel Space bằng cách sử dụng `copy_to_user()` và `copy_from_user()`. Đồng thời tự động cập nhật biến thống kê.
  - `my_ioctl()`: Hàm xử lý lệnh tùy chỉnh. Khi nhận được lệnh `RWBUF_IOCTL_GET_STATS`, driver sẽ copy giá trị đếm hiện tại vào biến struct rồi gửi qua mạng lưới Kernel-User về cho phía ứng dụng gọi.

### 8.2. User Space (`app/test.c`)
Chương trình giao diện dòng lệnh (CLI) chạy ở tầng người dùng, đóng vai trò ứng dụng tương tác:
- **Kết nối thiết bị**: Gọi API `open("/dev/rwbufdev", O_RDWR)` để mở file đại diện thiết bị và nhận về File Descriptor (fd).
- **Vòng lặp tương tác (Menu)**:
  - Khi chọn mục 1 (Write) hoặc 2 (Read), ứng dụng sử dụng các system call `write()` hoặc `read()` cơ bản trên fd để truyền chuỗi văn bản.
  - Khi chọn mục 3 (Get Stats), chương trình thực thi system call `ioctl(fd, RWBUF_IOCTL_GET_STATS, &stats)` yêu cầu module phía dưới đẩy gói dữ liệu (`struct rwbuf_stats`) lên để hiển thị.

### 8.3. Shared Header (`include/rwbuf_ioctl.h`)
File header đóng vai trò như bản "hợp đồng giao thức" chung giữa Kernel Driver và User App:
- **Cấu trúc dữ liệu (`struct rwbuf_stats`)**: Quy định dạng dữ liệu đóng gói bao gồm số lượt read/write.
- **Định nghĩa Lệnh IOCTL**: Định nghĩa Macro `RWBUF_IOCTL_GET_STATS` bằng cách sử dụng `_IOR()`. Macro này sử dụng `RWBUF_MAGIC` ('R') để tạo ra một mã lệnh duy nhất (đảm bảo không đụng hàng với các ioctl khác trong hệ thống) và báo cho VFS biết đây là lệnh yêu cầu "đọc dữ liệu từ driver".
