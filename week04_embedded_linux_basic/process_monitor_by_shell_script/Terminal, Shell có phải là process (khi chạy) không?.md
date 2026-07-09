# 1. Terminal, Shell có phải là process (khi chạy) không?

- **Process**: một chương trình đang chạy trong bộ nhớ.
- **Executable**: file chương trình trên ổ đĩa. Khi chạy, kernel sẽ tạo một **process** từ executable đó.

## Terminal

- Terminal (GNOME Terminal, Konsole, xterm...) là **một chương trình** ⇒ cũng là **một process**.
- Nhiệm vụ: tạo cửa sổ terminal, nhận/phát dữ liệu và khởi chạy một shell.

## Shell

- Shell (bash, zsh, sh...) cũng là **một chương trình** ⇒ cũng là **một process**.
- Nhiệm vụ: đọc lệnh người dùng, phân tích và thực thi lệnh.

## Quan hệ

Khi mở Terminal:

```text
GNOME Terminal (process)
        │
        ▼
     bash (process)
```

Khi chạy:

```bash
ls
```

Shell tạo thêm một process:

```text
GNOME Terminal
        │
        ▼
      bash
        │
        └── ls
```

Sau khi `ls` kết thúc, chỉ còn:

```text
GNOME Terminal
        │
        ▼
      bash
```

## Tại sao `ps` chỉ thấy `bash` và `ps`?

```bash
hao@hao-X99:~/Desktop/Working_Space/internship_report/week04_embedded_linux_basic/process_monitor_by_shell_script$ ps
    PID TTY          TIME CMD
   2668 pts/1    00:00:00 bash
  25252 pts/1    00:00:00 ps
```

Lệnh `ps` mặc định chỉ hiển thị các process gắn với **terminal (TTY) hiện tại**, nên thường chỉ thấy:

```text
bash
ps
```

GNOME Terminal vẫn đang chạy, nhưng không được `ps` mặc định hiển thị. Muốn xem toàn bộ process của hệ thống, dùng:

```bash
ps -ef
```

hoặc

```bash
ps aux
```

## Ghi nhớ

- Terminal là **một process**.
- Shell là **một process**.
- Mỗi chương trình như `ls`, `gcc`, `vim`, `python`, `./app` khi chạy đều tạo **một process mới** (trừ một số lệnh built-in của shell như `cd`).
- Sau khi process kết thúc, shell (`bash`) vẫn tiếp tục chạy và chờ lệnh tiếp theo.
