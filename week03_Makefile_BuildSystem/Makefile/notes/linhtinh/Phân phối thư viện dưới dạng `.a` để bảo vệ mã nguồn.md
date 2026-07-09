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