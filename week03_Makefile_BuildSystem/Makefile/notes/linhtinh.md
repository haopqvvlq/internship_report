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
