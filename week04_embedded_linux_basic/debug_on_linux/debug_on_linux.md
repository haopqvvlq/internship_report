# Phân loại công cụ Debug trong Linux BSP

## Không có một cách phân loại duy nhất

Các công cụ debug trong Linux không được phân loại theo một tiêu chuẩn cố định. Tùy vào mục đích học tập hay công việc, có nhiều cách tiếp cận khác nhau.

Ba cách phân loại phổ biến là:

- **Theo tầng của hệ thống (System Layer):** giúp hiểu kiến trúc Linux và công cụ thường dùng ở từng tầng.
- **Theo mục đích debug (Debug Purpose):** giúp chọn công cụ dựa trên loại thông tin cần thu thập.
- **Theo đối tượng quan sát (Observation Target):** giúp xác định nhanh công cụ khi muốn theo dõi một thành phần cụ thể.

---

## 1. Phân loại theo tầng của hệ thống (System Layer)

Đây là cách tiếp cận phù hợp nhất khi mới học Linux BSP vì giúp hình dung toàn bộ kiến trúc hệ thống.

```text
                User Space
┌──────────────────────────────────────┐
│ Application                          │
│ Library                              │
│ System Call                          │
└──────────────────────────────────────┘
                  │
                  ▼
               Kernel Space
┌──────────────────────────────────────┐
│ Core Kernel                          │
│ Device Driver                        │
│ Architecture                         │
└──────────────────────────────────────┘
                  │
                  ▼
               Hardware
```

| Tầng          | Công cụ thường dùng                      |
| ------------- | ---------------------------------------- |
| Application   | GDB, Valgrind, ASAN, TSAN                |
| Library       | ltrace                                   |
| System Call   | strace                                   |
| Core Kernel   | printk, dmesg, ftrace, perf, KASAN, KGDB |
| Device Driver | printk, dev_dbg, dynamic debug, debugfs  |
| Hardware      | JTAG, Logic Analyzer, Oscilloscope       |

**Ưu điểm**

- Dễ xây dựng kiến thức tổng quan.
- Hiểu vai trò của từng tầng trong Linux.
- Biết công cụ nào thường được sử dụng ở mỗi tầng.
- Phù hợp cho người mới học Linux BSP.

---

## 2. Phân loại theo mục đích debug (Debug Purpose)

Đây là cách tiếp cận phổ biến khi xử lý lỗi trong thực tế.

| Mục đích                  | Công cụ                            |
| ------------------------- | ---------------------------------- |
| Ghi log                   | printk, dev_dbg                    |
| Xem log                   | dmesg                              |
| Theo dõi luồng thực thi   | GDB, KGDB, ftrace                  |
| Theo dõi System Call      | strace                             |
| Theo dõi lời gọi thư viện | ltrace                             |
| Debug bộ nhớ              | ASAN, KASAN, Valgrind              |
| Debug race condition      | TSAN, KCSAN                        |
| Phân tích hiệu năng       | perf                               |
| Debug phần cứng           | JTAG, Logic Analyzer, Oscilloscope |

Lúc này câu hỏi không còn là:

> "Mình đang ở tầng nào?"

mà là:

> "Mình cần thu thập thông tin gì?"

Ví dụ:

- Muốn biết chương trình gọi system call nào → **strace**
- Muốn biết kernel đang làm gì → **ftrace**
- Muốn biết driver đã chạy đến đâu → **printk**
- Muốn kiểm tra tín hiệu I2C → **Logic Analyzer**

---

## 3. Phân loại theo đối tượng quan sát (Observation Target)

Một cách tiếp cận khác là phân loại theo thành phần cần quan sát.

| Thành phần cần quan sát | Công cụ               |
| ----------------------- | --------------------- |
| User Program            | GDB                   |
| Shared Library          | ltrace                |
| System Call             | strace                |
| Kernel Log              | dmesg                 |
| Kernel Function         | ftrace                |
| Device Driver           | printk, dev_dbg       |
| Scheduler               | perf, trace-cmd       |
| Interrupt               | ftrace                |
| Memory                  | Valgrind, ASAN, KASAN |
| Bus (I2C, SPI, UART...) | Logic Analyzer        |
| Tín hiệu điện           | Oscilloscope          |
| CPU                     | JTAG                  |

Cách phân loại này đặc biệt hữu ích khi đã xác định được thành phần nghi ngờ gây lỗi.

---

## Một số lưu ý

**printk và dmesg không giống nhau**

Đây là điểm người mới rất dễ nhầm.

```text
printk()
      │
      ▼
Kernel Ring Buffer
      │
      ▼
dmesg
```

- `printk()` dùng để ghi log vào kernel.
- `dmesg` dùng để đọc các log đã được ghi.

Nói cách khác:

- `printk` là **nguồn tạo log**.
- `dmesg` là **công cụ xem log**.

---

## Tư duy debug của kỹ sư Linux BSP

Người mới thường suy nghĩ:

> "Tầng này thì dùng công cụ gì?"

Trong khi kỹ sư BSP thường suy nghĩ:

> "Lỗi có khả năng nằm ở đâu?"

Sau đó tiếp tục đặt câu hỏi:

- Thành phần nào đang gặp vấn đề?
- Cần thu thập thông tin gì?
- Công cụ nào cung cấp được thông tin đó?

Ví dụ:

```text
Camera không hoạt động
        │
        ▼
Có log lỗi không?
        │
        ├── dmesg
        │
        ▼
Driver đã probe chưa?
        │
        ├── printk
        ├── dev_dbg
        │
        ▼
Device Tree đúng chưa?
        │
        ▼
Clock / Reset đã được enable chưa?
        │
        ▼
I2C có giao tiếp không?
        │
        ├── Logic Analyzer
        │
        ▼
Sensor có phản hồi không?
        │
        ▼
Application gọi đúng chưa?
        │
        ├── strace
        └── GDB
```

Điểm quan trọng là **không chọn công cụ trước**, mà **xác định câu hỏi cần trả lời trước**, sau đó mới chọn công cụ phù hợp.

---

## Kết luận

Ba cách phân loại đều đúng, nhưng phục vụ các mục đích khác nhau.

- **Theo tầng của hệ thống:** giúp xây dựng bức tranh tổng thể về kiến trúc Linux và công cụ thường dùng ở từng tầng.
- **Theo mục đích debug:** giúp lựa chọn công cụ dựa trên loại thông tin cần thu thập khi xử lý lỗi.
- **Theo đối tượng quan sát:** giúp xác định nhanh công cụ khi cần theo dõi một thành phần cụ thể của hệ thống.

Đối với người mới học Linux BSP, nên bắt đầu bằng cách **phân loại theo tầng** để hiểu kiến trúc hệ thống. Khi có kinh nghiệm hơn, nên chuyển dần sang tư duy **đặt câu hỏi → khoanh vùng nguyên nhân → thu thập thông tin → chọn công cụ**, vì đây là cách làm việc phổ biến của các kỹ sư Linux BSP trong thực tế.
