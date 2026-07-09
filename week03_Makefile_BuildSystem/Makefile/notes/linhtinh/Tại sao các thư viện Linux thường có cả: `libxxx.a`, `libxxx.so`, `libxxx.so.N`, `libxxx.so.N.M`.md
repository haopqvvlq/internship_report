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
