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
