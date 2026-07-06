# Process Monitor by Shell Script

## Giới thiệu

Đây là bài tập sử dụng **Shell Script** để kiểm tra trạng thái của một tiến trình đang chạy trên hệ thống Linux.

Chương trình sử dụng các lệnh `pgrep` và `ps` để tìm tiến trình theo tên, lấy PID và hiển thị trạng thái hiện tại của tiến trình.

Mục tiêu của project là làm quen với việc tự động hóa các tác vụ quản trị hệ thống bằng Bash.

---

## Kiến thức sử dụng

- Bash Shell Script
- Linux Process Management
- `pgrep`
- `ps`
- Biến trong Bash
- Điều kiện (`if`)
- Command Substitution

---

## Cấu trúc project

```
.
├── process_monitor.sh
└── README.md
```

---

## Cách cấp quyền thực thi

```bash
chmod +x monitor.sh
```

---

## Cách chạy

```bash
./monitor.sh
```

---

## Kết quả mong đợi

Khi tiến trình đang chạy:

```text
Process 'bash' đang chạy.
PID: 12345
State: S
```

Nếu tiến trình không tồn tại:

```text
Process 'bash' không chạy.
```

---

## Luồng hoạt động

```
Khai báo tên tiến trình
          │
          ▼
       pgrep
          │
          ▼
 Có tìm thấy PID?
      │          │
     Có         Không
      │          │
      ▼          ▼
Lấy trạng thái   Thông báo
 bằng ps        không tồn tại
      │
      ▼
 Hiển thị kết quả
```

---

## Giải thích chương trình

### 1. Khai báo tên tiến trình

```bash
PROCESS_NAME="bash"
```

Tên tiến trình cần kiểm tra.

Có thể thay đổi thành các tiến trình khác như:

```bash
PROCESS_NAME="ssh"
PROCESS_NAME="systemd"
PROCESS_NAME="nginx"
```

---

### 2. Tìm PID

```bash
PID=$(pgrep "$PROCESS_NAME")
```

Lệnh `pgrep` tìm PID của tiến trình theo tên.

Nếu tìm thấy:

```
1234
```

Nếu không tìm thấy:

```
(empty)
```

---

### 3. Kiểm tra kết quả

```bash
if [ -n "$PID" ]; then
```

`-n` kiểm tra chuỗi có khác rỗng hay không.

Nếu có PID, chương trình tiếp tục lấy trạng thái tiến trình.

---

### 4. Lấy trạng thái

```bash
STATE=$(ps -o stat= -p "$PID")
```

Lệnh `ps` hiển thị trạng thái của tiến trình.

Ví dụ:

| State | Ý nghĩa               |
| ----- | --------------------- |
| R     | Running               |
| S     | Sleeping              |
| D     | Uninterruptible Sleep |
| T     | Stopped               |
| Z     | Zombie                |

---

### 5. Hiển thị kết quả

```bash
echo "PID: $PID"
echo "State: $STATE"
```

In ra PID và trạng thái của tiến trình.

---

## Kiến thức rút ra

- Sử dụng Bash để tự động hóa tác vụ trên Linux.
- Tìm tiến trình bằng `pgrep`.
- Lấy thông tin tiến trình bằng `ps`.
- Sử dụng command substitution (`$(...)`).
- Kiểm tra điều kiện trong Bash.
- Hiểu các trạng thái cơ bản của tiến trình trên Linux.

---

## Hướng phát triển

Có thể mở rộng chương trình bằng cách:

- Nhận tên tiến trình từ dòng lệnh.

```bash
./process_monitor.sh nginx
```

- Theo dõi nhiều tiến trình cùng lúc.
- Hiển thị mức sử dụng CPU và RAM.
- Cập nhật trạng thái theo thời gian thực.
- Ghi kết quả vào file log.
- Tự động gửi cảnh báo khi tiến trình dừng hoạt động.

---

## Tài liệu tham khảo

- Bash Reference Manual
- `man pgrep`
- `man ps`
- `man bash`
