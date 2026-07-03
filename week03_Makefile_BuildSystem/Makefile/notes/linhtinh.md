# 1. Timestamp trong Make

`make` quyết định có build lại hay không dựa trên **timestamp** (thời gian sửa đổi cuối cùng của file), không dựa trên nội dung file.

Nguyên tắc:

- Nếu target chưa tồn tại → build.
- Nếu một dependency mới hơn target → build lại target.
- Nếu target mới hơn tất cả dependency → `target is up to date`.

Ví dụ:

main.c → main.o → main

Nếu `main.c` được sửa (hoặc `touch main.c`):

- `main.c` mới hơn `main.o` ⇒ build lại `main.o`
- `main.o` mới hơn `main` ⇒ build lại `main`

Lưu ý:

- `make` không biết nội dung có thay đổi hay không.
- Chỉ cần timestamp thay đổi (ví dụ dùng `touch`) là `make` có thể build lại.

# 2. Vì sao chương trình phải tạo ra file `.o` trước khi tạo file thực thi?

Nếu không có `.o`:

```make
main.c
lib.c
util.c
...
```

mỗi lần sửa 1 file:

```make
main.c thay đổi
↓
phải biên dịch lại toàn bộ project
```

Rất tốn thời gian khi project có hàng trăm hoặc hàng nghìn file.

Vì vậy quy trình được tách thành 2 bước:

```make
main.c ──► main.o
lib.c  ──► lib.o
util.c ──► util.o
               ↓
          linker
               ↓
             app
```

`.o` (object file) là kết quả biên dịch từng file nguồn riêng lẻ.

Khi main.c thay đổi:

```make
main.c ──► main.o   (build lại)
lib.o             (giữ nguyên)
util.o            (giữ nguyên)
↓
link lại app
```

nhanh hơn rất nhiều.

# 1. Phân phối thư viện dưới dạng `.a` để bảo vệ mã nguồn

Một lý do quan trọng khiến Static Library (`.a`) vẫn được sử dụng là để phân phối thư viện mà không cần công khai mã nguồn.

Thông thường nhà cung cấp chỉ phát hành:

```text
driver.h
libdriver.a
```

Trong đó:

- File `.h` chứa khai báo hàm để người dùng biết cách sử dụng.
- File `.a` chứa mã máy đã được biên dịch từ mã nguồn C/C++.

Người dùng có thể gọi các hàm trong thư viện và link vào chương trình của mình:

```c
Driver_Init();
Driver_Start();
```

nhưng không có quyền truy cập vào mã nguồn thực sự bên trong:

```c
void Driver_Start()
{
    // Thuật toán thực tế
}
```

Nhờ đó, khách hàng có thể sử dụng thư viện bình thường nhưng khó biết chính xác thuật toán được triển khai như thế nào.

Mô hình này thường được dùng khi thư viện chứa các công nghệ có giá trị như:

- Điều khiển động cơ
- Xử lý tín hiệu số (DSP)
- Mã hóa bảo mật
- Thuật toán AI
- Các driver độc quyền

Về lý thuyết, mã máy trong file `.a` vẫn có thể bị phân tích ngược (reverse engineering), nhưng việc đọc assembly khó khăn hơn rất nhiều so với việc xem trực tiếp mã nguồn C/C++.

Vì vậy, phát hành thư viện dưới dạng `.a` là một cách cân bằng giữa:

- Cho phép khách hàng sử dụng sản phẩm.
- Bảo vệ tài sản trí tuệ của nhà phát triển.

# 2. Linker và Dynamic Linker

Khi xây dựng và chạy một chương trình trên Linux, có hai thành phần khác nhau thường dễ bị nhầm lẫn: **Linker** và **Dynamic Linker**.

## 1. Linker là gì?

Linker là công cụ làm việc trong quá trình **build chương trình**.

Sau khi compiler dịch các file `.c` thành các file object (`.o`), linker sẽ:

- Ghép các file `.o` lại với nhau.
- Tìm các hàm và biến còn thiếu trong các thư viện (`.a`, `.so`).
- Tạo ra file thực thi cuối cùng (executable).

Ví dụ:

```bash
gcc main.o -ldriver -o app
```

Trong bước này, linker sẽ tìm thư viện `libdriver.a` hoặc `libdriver.so` và liên kết (link) chương trình với thư viện đó.

---

## 2. Dynamic Linker là gì?

Dynamic Linker (hay Dynamic Loader) làm việc khi **chương trình được chạy**.

Nếu executable phụ thuộc vào các Shared Library (`.so`), Dynamic Linker sẽ:

- Đọc danh sách các thư viện mà chương trình cần.
- Tìm các file `.so` tương ứng.
- Nạp chúng vào RAM.
- Kết nối chương trình với các thư viện trước khi bắt đầu thực thi.

Nếu không tìm thấy thư viện cần thiết, chương trình sẽ báo lỗi:

```text
error while loading shared libraries
```

---

## 3. Mối quan hệ với Static Library và Shared Library

### Static Library (`.a`)

```text
main.o + libdriver.a
        ↓
      Linker
        ↓
       app
```

Code của thư viện được copy trực tiếp vào executable.

Khi chạy chương trình, không cần tìm file `.a` nữa.

### Shared Library (`.so`)

```text
main.o + tham chiếu của libdriver.so
        ↓
      Linker
        ↓
       app
```

Khi chạy:

```text
app + libdriver.so
        ↓
  Dynamic Linker
```

Executable chỉ chứa thông tin tham chiếu đến thư viện. Khi chạy, Dynamic Linker phải tìm và nạp file `.so`.

---

## Tóm tắt

| Thành phần     | Thời điểm hoạt động | Nhiệm vụ                                           |
| -------------- | ------------------- | -------------------------------------------------- |
| Linker         | Khi build           | Ghép các file `.o`, tìm thư viện và tạo executable |
| Dynamic Linker | Khi chạy            | Tìm, nạp và kết nối các Shared Library (`.so`)     |

### Ghi nhớ

> **Linker ghép chương trình khi build, Dynamic Linker tìm và nạp `.so` khi chạy.**

# 3. Tại sao các thư viện Linux thường có cả: `libxxx.a`, `libxxx.so`, `libxxx.so.N`, `libxxx.so.N.M`

Linux thường lưu một thư viện dưới nhiều tên khác nhau để phục vụ các mục đích khác nhau trong quá trình **build** và **runtime**.

| File            | Vai trò                              |
| --------------- | ------------------------------------ |
| `libxxx.a`      | Static Library                       |
| `libxxx.so`     | Symlink dùng lúc build (`gcc -lxxx`) |
| `libxxx.so.N`   | Symlink theo major version           |
| `libxxx.so.N.M` | Shared Library thật sự               |

Ví dụ:

```text
libreadline.a
libreadline.so      -> libreadline.so.6.3
libreadline.so.6    -> libreadline.so.6.3
libreadline.so.6.3
```

Hiểu đơn giản thì Linux cung cấp một thư viện dưới 2 dạng `Static Lib` và `Shared Lib`.

Với Shared Library (`.so`) phải có cơ chế version:

- `Static Library` (`.a`) chỉ dùng lúc build. Sau khi link xong, mã thư viện đã được nhúng vào executable nên chương trình không còn phụ thuộc vào file `.a` nữa. Vì vậy nếu thư viện được cập nhật sau này thì executable cũ vẫn chạy như trước.

- `Shared Library` (`.so`) thì khác. Chương trình chỉ lưu tham chiếu đến thư viện và sẽ nạp thư viện mỗi khi chạy. Trong khi đó, thư viện có thể được nâng cấp hoặc thay đổi theo thời gian, thậm chí mất tính tương thích với các phiên bản cũ.

=> Vì vậy Shared Library phải đi kèm version (`.so.6`, `.so.8`, ...) để chương trình biết chính xác phiên bản tương thích nào cần được nạp khi chạy.
