# Phân biệt Terminal, Shell, Bash và Kernel

Khi mới học Linux, rất dễ nhầm lẫn giữa **Terminal**, **Shell**, **Bash** và **Kernel** vì chúng luôn xuất hiện cùng nhau. Thực tế, mỗi thành phần có một vai trò riêng.

## 1. Mô hình tổng quan

Luồng làm việc của một câu lệnh trong Linux:

```text
Người dùng
    │
    ▼
Terminal
    │
    ▼
Shell (bash)
    │
    ▼
Kernel
    │
    ▼
CPU / RAM / File / Driver / Hardware
```

Mỗi thành phần chỉ làm đúng nhiệm vụ của mình.

---

# 2. Terminal là gì?

**Terminal** là chương trình cung cấp giao diện để người dùng nhập lệnh và xem kết quả.

Ví dụ:

- GNOME Terminal
- Konsole
- Terminal trong VS Code
- xterm

Terminal **không hiểu** lệnh Linux.

Nó chỉ có nhiệm vụ:

- nhận ký tự từ bàn phím
- hiển thị kết quả lên màn hình
- kết nối người dùng với Shell

Có thể xem Terminal giống như **một cửa sổ giao tiếp**.

---

# 3. Shell là gì?

**Shell** là chương trình cung cấp giao diện dòng lệnh, có nhiệm vụ phân tích câu lệnh, xử lý các cơ chế của shell (pipe, redirect, biến…), và khởi chạy các chương trình hoặc script tương ứng thông qua kernel.

Nếu muốn hiểu “một câu dễ nhớ” hơn:

> Shell là bộ điều phối lệnh (command dispatcher), không phải nơi thực thi ngôn ngữ lập trình.

Nó sẽ:

- đọc câu lệnh
- phân tích cú pháp
- quyết định phải làm gì
- hoặc tự thực hiện
- hoặc yêu cầu Kernel chạy chương trình

Ví dụ:

Người dùng gõ:

```bash
ls -l
```

Shell sẽ hiểu:

```text
Chương trình cần chạy: ls

Tham số:
    -l
```

Sau đó Shell tìm executable của `ls` rồi yêu cầu Kernel chạy nó.

---

# 4. Bash là gì?

**Bash** chỉ là **một loại Shell**.

Có nhiều Shell khác nhau:

```text
Shell
├── sh
├── bash
├── zsh
├── fish
└── tcsh
```

Trong phần lớn bản phân phối Linux, Shell mặc định thường là **bash**.

Nói cách khác:

> Bash là một Shell, giống như Chrome là một trình duyệt.

---

# 5. Kernel là gì?

Kernel là **nhân của hệ điều hành Linux**.

Nó quản lý:

- Process
- Bộ nhớ
- File
- Driver
- CPU
- Thiết bị phần cứng

Shell **không trực tiếp chạy chương trình**.

Shell chỉ yêu cầu Kernel tạo Process và thực thi executable.

---

# 6. Shell làm được những gì?

Shell có hai kiểu xử lý.

## Trường hợp 1: Chạy executable

Ví dụ:

```bash
ls
gcc
vim
python
```

Shell sẽ:

```text
Đọc lệnh
    │
    ▼
Tìm executable
    │
    ▼
Yêu cầu Kernel chạy
```

Kernel tạo Process và chương trình bắt đầu chạy.

---

## Trường hợp 2: Thực hiện lệnh nội bộ (Builtin)

Một số lệnh không phải executable.

Ví dụ:

```bash
cd
pwd
echo
export
alias
```

Đây là **builtin của Shell**.

Shell tự thực hiện chúng mà không cần chạy chương trình bên ngoài.

Ví dụ:

```bash
cd Desktop
```

Shell sẽ tự đổi thư mục làm việc của chính nó.

Nếu `cd` là một executable riêng thì sau khi chương trình kết thúc, Shell vẫn ở thư mục cũ. Vì vậy `cd` bắt buộc phải là builtin.

---

# 7. Ví dụ dễ nhớ: Nhà hàng

Có thể hình dung Linux như một nhà hàng.

```text
Bạn
    │
    ▼
Cửa sổ gọi món (Terminal)
    │
    ▼
Tiểu nhị (Shell / bash)
    │
    ▼
Đầu bếp (Kernel)
    │
    ▼
Bếp và dụng cụ (CPU, RAM, Driver, Hardware)
```

### Khi chạy executable

Bạn nói:

> "Cho tôi một tô phở."

Tiểu nhị:

- nghe yêu cầu
- hiểu bạn muốn món gì
- chuyển yêu cầu xuống bếp

Đầu bếp:

- nấu món
- mang kết quả lên

Trong Linux:

```bash
ls
gcc
vim
python
```

đều giống như việc **gọi đầu bếp nấu một món mới**.

---

### Khi dùng builtin

Bạn nói:

> "Em đứng sang bên trái giúp anh."

Tiểu nhị tự làm ngay.

Không cần gọi đầu bếp.

Trong Linux:

```bash
cd
export
pwd
echo
```

đều là những việc Shell tự xử lý.

---

# 8. TTY là gì?

Mỗi khi mở một Terminal, Linux tạo ra một **TTY (thường là `/dev/pts/x`)**.

TTY là **kênh giao tiếp giữa Terminal và các Process**.

Ví dụ:

```bash
tty
```

```
/dev/pts/2
```

Khi Shell chạy một chương trình, chương trình đó sẽ kế thừa TTY này.

Nhờ đó chương trình biết:

- đọc dữ liệu từ bàn phím nào
- in kết quả ra cửa sổ Terminal nào

Ví dụ:

```bash
sleep 100
```

Chương trình `sleep` sẽ gắn với cùng TTY của Shell.

Nếu chạy:

```bash
ps
```

thì mặc định chỉ thấy các Process đang sử dụng TTY hiện tại.

---

# 9. Tóm tắt

- **Terminal**: Cửa sổ để giao tiếp với Linux.
- **Shell**: Chương trình đọc và thông dịch lệnh.
- **Bash**: Một loại Shell phổ biến.
- **Kernel**: Nhân Linux, quản lý tài nguyên và thực thi chương trình.
- **Executable**: Chương trình độc lập được Kernel chạy.
- **Builtin**: Lệnh do chính Shell thực hiện.
- **TTY**: Kênh nhập/xuất mà Shell và các Process gắn vào để giao tiếp với Terminal.

Chỉ cần nhớ mô hình sau là đủ để hiểu hầu hết các kiến thức nền tảng của Linux:

```text
Người dùng
      │
      ▼
Terminal
      │
      ▼
Shell (bash)
      ├── Builtin → Shell tự xử lý
      └── Executable → Kernel tạo Process và chạy
                           │
                           ▼
                    CPU / RAM / Driver / Hardware
```
