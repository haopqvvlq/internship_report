# Valgrind Memory Leak Demo

## Giới thiệu

Project này minh họa cách **Valgrind Memcheck** phát hiện và phân loại các loại memory leak phổ biến trong C.

Các trường hợp được mô phỏng gồm:

- `still reachable`
- `definitely lost`
- `indirectly lost`
- `possibly lost`

Ngoài ra, mã nguồn còn có phần (đã comment) để thử nghiệm các lỗi như **Invalid Write**.

---

## Build

```bash
gcc -g -O0 main.c -o app
```

---

## Chạy

```bash
valgrind --leak-check=full --show-leak-kinds=all ./app
```

Chương trình chạy vòng lặp để mô phỏng một tiến trình đang hoạt động. Để kết thúc chương trình, mở một terminal khác và gửi tín hiệu `SIGINT`:

```bash
ps -ef | grep app
kill -SIGINT <PID>
```

---

## Kết quả mong đợi

Valgrind sẽ báo cáo tương tự:

```text
LEAK SUMMARY:
definitely lost: ...
indirectly lost: ...
possibly lost: ...
still reachable: ...
```

Ý nghĩa:

- **still reachable**: Vẫn còn con trỏ hợp lệ tới block nhớ.
- **definitely lost**: Mất hoàn toàn mọi con trỏ tới block nhớ.
- **indirectly lost**: Block bị mất do block cha đã bị mất.
- **possibly lost**: Chỉ còn con trỏ trỏ vào bên trong block (interior pointer).

Project này được dùng để thực hành đọc báo cáo của Valgrind và hiểu sự khác nhau giữa các loại memory leak.
