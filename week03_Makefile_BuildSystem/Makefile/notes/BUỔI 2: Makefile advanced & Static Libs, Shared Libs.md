# BUỔI 2: Makefile advanced & Static Libs, Shared Libs

1. Giới thiệu về Makefile

2. Quá trình biên dịch một chương trình C

3. Static Lib và Shared Lib

## 1. Giới thiệu về Makefile (xem ở repo studies/Makefile)

## 2. Quá trình biên dịch một chương trình C

### 1. Giai đoạn tiền xử lý (Pre-processing)

- Loại bỏ comments
- Mở rộng các macros
- Mở rộng các include file
- Biên dịch các câu lệnh điều kiện
- Kết quả thu được sau bước này là một file `.i`

### 2. Giai đoạn dịch ngôn ngữ bậc cao sang asm (Compilation)

Ở giai đoạn này, mã nguồn sẽ tiếp tục thực hiện biên dịch từ file `.i` thu được ở bước trước thành một file `.s` (assembly).

### 3. Giai đoạn dịch asm sang mã máy (Assembly)

- File `.s` ở giai đoạn trước tiếp tục được sử dụng cho giai đoạn này.
- Thông qua assembler, output mà chúng ta thu được là một file `.o`. Đây là file chứa các chỉ lệnh cấp độ ngôn ngữ máy (machine language).

### 4. Giai đoạn Linker (Linking)

- Mỗi một file `.o` thu được ở gian đoạn Assembly là một phần của chương trình.
- Ở giai đoạn linking sẽ liên kết chúng để thu được một file thực thi hoàn chỉnh.
<p align="center">
  <strong>TÓM LẠI</strong>
</p>
Khi bạn chạy:

```make
gcc main.c -o app
```

GCC thực hiện ngầm:

```make
main.c
  ↓
main.i
  ↓
main.s
  ↓
main.o
  ↓
app
```

Nhưng sau khi dùng xong các file trung gian, GCC thường xóa chúng đi, nên bạn chỉ thấy:

```make
main.c
app
```

## 3. Static Lib and Shared Lib

Thực ra `static library` và `shared library` cũng giống như file `executable` là đều được tạo từ các file `.o`

Pipeline:

```make
.c
 ↓
.i
 ↓
.s
 ↓
.o
 ↓ linking
 |
 ├── executable
 │
 ├── static library (.a)
 │
 └── shared library (.so)
```

### 1. Static Library (.a)

Giả sử bạn có `uart.o`,`gpio.o`,`spi.o` và bạn đóng gói chúng thành `libdriver.a`

Bản chất:

```make
libdriver.a
= tập hợp nhiều file .o

libdriver.a
├── uart.o
├── gpio.o
└── spi.o
```

Khi link thì Linker sẽ lấy những đoạn mã cần thiết từ `libdriver.a` rồi **chép vào file thực thi**.

```make
gcc main.o -ldriver
```

Kết quả:

```make
app
├── code từ main.o
├── code từ uart.o
├── code từ gpio.o
└── ...
```

Sau khi build xong:

- Không cần `libdriver.a` nữa.
- Chương trình vẫn chạy bình thường.

### 2. Shared Library (.so)

Cũng được đóng gói từ những file `.o` y hệt `static lib`

> Nhưng thay vì chép mã vào executable thì linker chỉ **ghi tham chiếu**, còn lúc chạy hệ điều hành mới nạp thư viện vào bộ nhớ.

Khác biệt:
| Đặc điểm | Static Library (`.a`) | Shared Library (`.so`) |
|-----------|----------------------|------------------------|
| Cách liên kết | Code thư viện được chép vào file thực thi | Chỉ lưu tham chiếu tới thư viện |
| Kết quả | `app = code app + code thư viện` | `app + libxxx.so` |
| Kích thước file thực thi | Lớn hơn | Nhỏ hơn |
| Phụ thuộc khi chạy | Không | Có |
| Cần thư viện lúc chạy | Không | Có, phải có file `.so` |
| Chia sẻ giữa nhiều chương trình | Không | Có |
| Cập nhật thư viện | Phải build lại ứng dụng | Có thể thay thư viện mà không cần build lại (nếu tương thích ABI) |
| Lỗi thường gặp | Ít gặp lỗi thiếu thư viện khi chạy | Nếu thiếu `.so` có thể gặp lỗi: `error while loading shared libraries` |

### 3. Ưu nhược điểm của `Static Lib` và `Shared Lib`

Có vẻ `Shared Lib` hơn `Static Lib` về mọi mặt. Liệu điều này có thực sự luôn đúng?

#### Ưu điểm của Shared Lib

Có 3 file excecutable (`app1`,`app2`,`app3`) đều dùng `libc.so`, HĐH có thể nạp một bản `libc.so` vào RAM cho cả 3 chương trình dùng chung, giúp tiết kiệm RAM và dung lượng đĩa

#### Ưu điểm của Static Library

- Với các vi điều khiển như STM32, AVR, PIC; firmware được link hoàn chỉnh trước khi nạp vào Flash. Chúng thường không có hệ điều hành hỗ trợ:
  - Dynamic linker/loader (`ld.so`)
  - Cơ chế nạp thư viện lúc runtime
  - Hệ thống file chứa các thư viện `.so`

  Vì vậy gần như luôn sử dụng **Static Library**.

- Chỉ cần một file executable (hoặc firmware image) đã chứa toàn bộ mã cần thiết. Không cần các thư viện động như `libdriver.so`, `libutils.so` đi kèm khi chạy.

- Quá trình khởi động đơn giản hơn vì không cần:
  - Tìm kiếm thư viện
  - Kiểm tra dependency
  - Resolve symbol động

- Không gặp các vấn đề về phiên bản thư viện lúc runtime. Ví dụ chương trình được build với `libcrypto.so` phiên bản 1 nhưng máy đích chỉ có phiên bản 2, dẫn đến lỗi tương thích. Với Static Library, mã thư viện đã được nhúng vào executable ngay từ lúc build.

#### Kết luận

Với các hệ thống embedded truyền thống như STM32, Static Library có những lợi thế rất lớn:

- Triển khai đơn giản
- Dễ kiểm soát phiên bản
- Không phụ thuộc môi trường runtime
- Phù hợp với các hệ thống không có hệ điều hành hoặc tài nguyên hạn chế

### 4. Ví dụ một project đơn giản dùng `Static Lib` và `Shared Lib`

Cấu trúc của Project:
![Cấu trúc Project](./images/image1.png)

Makefile của Project:
![Makefile của Project](./images/image2.png)
Chi tiết việc tạo `Static Lib`, xem ở `1:40:00 Buổi 2 Vip1`
