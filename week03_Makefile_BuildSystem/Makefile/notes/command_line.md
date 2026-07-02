# Linux Commands Cheat Sheet

## Điều hướng thư mục

| Lệnh              | Chức năng                 |
| ----------------- | ------------------------- |
| `pwd`             | Hiển thị thư mục hiện tại |
| `cd`              | Về thư mục Home           |
| `cd ..`           | Lùi 1 cấp thư mục         |
| `cd -`            | Quay về thư mục trước đó  |
| `cd /path/to/dir` | Đi tới thư mục chỉ định   |

## Liệt kê file/thư mục

| Lệnh      | Chức năng                  |
| --------- | -------------------------- |
| `ls`      | Liệt kê file/thư mục       |
| `ls -l`   | Hiển thị chi tiết          |
| `ls -a`   | Hiện cả file ẩn            |
| `ls -la`  | Kết hợp `-l` và `-a`       |
| `ls -lah` | Hiển thị dung lượng dễ đọc |

## Tạo file/thư mục

| Lệnh             | Chức năng             |
| ---------------- | --------------------- |
| `touch file.txt` | Tạo file rỗng         |
| `mkdir dir`      | Tạo thư mục           |
| `mkdir -p a/b/c` | Tạo nhiều cấp thư mục |

## Xóa

| Lệnh          | Chức năng                          |
| ------------- | ---------------------------------- |
| `rm file.txt` | Xóa file                           |
| `rm -r dir`   | Xóa thư mục                        |
| `rm -rf dir`  | Ép xóa thư mục và toàn bộ nội dung |

## Sao chép và di chuyển

| Lệnh            | Chức năng            |
| --------------- | -------------------- |
| `cp src dst`    | Copy file            |
| `cp -r src dst` | Copy thư mục         |
| `mv old new`    | Đổi tên file/thư mục |
| `mv file dir/`  | Di chuyển file       |

## Xem nội dung file

| Lệnh              | Chức năng                 |
| ----------------- | ------------------------- |
| `cat file.txt`    | Hiển thị toàn bộ nội dung |
| `cat -n file.txt` | Hiển thị kèm số dòng      |
| `less file.txt`   | Xem file có thể cuộn      |
| `head file.txt`   | 10 dòng đầu               |
| `tail file.txt`   | 10 dòng cuối              |
| `tail -f log.txt` | Theo dõi log realtime     |

## Tìm kiếm

| Lệnh                   | Chức năng                |
| ---------------------- | ------------------------ |
| `find . -name "*.c"`   | Tìm file `.c`            |
| `grep "text" file.txt` | Tìm chuỗi trong file     |
| `grep -rn "text" .`    | Tìm đệ quy trong thư mục |

## Quyền truy cập

| Lệnh                  | Chức năng               |
| --------------------- | ----------------------- |
| `chmod +x app.sh`     | Thêm quyền thực thi     |
| `chmod 755 app.sh`    | Thiết lập quyền bằng số |
| `chown user file.txt` | Đổi chủ sở hữu          |

## Quản lý tiến trình

| Lệnh          | Chức năng                    |
| ------------- | ---------------------------- |
| `ps`          | Liệt kê tiến trình           |
| `ps aux`      | Liệt kê chi tiết             |
| `top`         | Theo dõi tiến trình realtime |
| `htop`        | Giao diện đẹp hơn `top`      |
| `kill PID`    | Dừng tiến trình              |
| `kill -9 PID` | Buộc dừng tiến trình         |

## SSH và mạng

| Lệnh                     | Chức năng             |
| ------------------------ | --------------------- |
| `ping google.com`        | Kiểm tra kết nối mạng |
| `ip addr` hoặc `ip a`    | Xem địa chỉ IP        |
| `ssh user@ip`            | SSH tới máy khác      |
| `scp file user@ip:/path` | Copy file qua SSH     |

## Thông tin hệ thống

| Lệnh       | Chức năng        |
| ---------- | ---------------- |
| `uname -a` | Thông tin kernel |
| `df -h`    | Dung lượng ổ đĩa |
| `free -h`  | Dung lượng RAM   |
| `history`  | Lịch sử lệnh     |

## Trình soạn thảo

| Lệnh            | Chức năng         |
| --------------- | ----------------- |
| `vim file.txt`  | Mở file bằng Vim  |
| `nano file.txt` | Mở file bằng Nano |

## Phím tắt Terminal

| Phím       | Chức năng                   |
| ---------- | --------------------------- |
| `Ctrl + C` | Dừng chương trình đang chạy |
| `Ctrl + Z` | Tạm dừng chương trình       |
| `Ctrl + D` | Kết thúc input / logout     |
| `Ctrl + L` | Xóa màn hình terminal       |
| `Tab`      | Tự động hoàn thành          |
| `↑ ↓`      | Duyệt lịch sử lệnh          |
| `Ctrl + R` | Tìm kiếm trong history      |
