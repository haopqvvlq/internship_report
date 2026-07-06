# Simple CMake Project

## Giới thiệu

Đây là bài tập làm quen với **CMake**, một hệ thống build phổ biến dùng để quản lý và sinh các file build cho nhiều nền tảng.

Chương trình sử dụng ví dụ quản lý Playlist (Linked List) để minh họa cách tổ chức source code thành nhiều thư mục và biên dịch bằng CMake.

Mục tiêu chính của project là tìm hiểu cách sử dụng `CMakeLists.txt` để quản lý quá trình build thay cho Makefile viết thủ công.

---

## Kiến thức sử dụng

- C Programming
- CMake
- Modular Programming
- Header File (`.h`)
- Multi-file Project

---

## Cấu trúc project

```
.
├── CMakeLists.txt
├── main.c
├── inc
│   └── quanly.h
├── src
│   └── quanly.c
└── README.md
```

---

## CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

project(MyProject C)

add_executable(main
    main.c
    src/quanly.c
)

target_include_directories(main PRIVATE
    inc
)
```

Ý nghĩa các lệnh:

| Lệnh                           | Chức năng                                |
| ------------------------------ | ---------------------------------------- |
| `cmake_minimum_required()`     | Yêu cầu phiên bản CMake tối thiểu        |
| `project()`                    | Khai báo tên project và ngôn ngữ sử dụng |
| `add_executable()`             | Tạo file thực thi từ các source          |
| `target_include_directories()` | Khai báo thư mục chứa file header        |

---

## Cách build

Tạo thư mục build:

```bash
mkdir build
```

Di chuyển vào thư mục build:

```bash
cd build
```

Sinh hệ thống build:

```bash
cmake ..
```

Biên dịch:

```bash
make
```

---

## Cách chạy

Trong thư mục `build`:

```bash
./main
```

---

## Luồng build

```
Project Source
       │
       ▼
CMakeLists.txt
       │
       ▼
cmake ..
       │
       ▼
Generate Makefile
       │
       ▼
make
       │
       ▼
Executable
```

---

## Giải thích

### 1. Khai báo Project

```cmake
project(MyProject C)
```

Đặt tên project và khai báo ngôn ngữ sử dụng là C.

---

### 2. Tạo chương trình

```cmake
add_executable(main
    main.c
    src/quanly.c
)
```

Biên dịch các source và tạo file thực thi có tên `main`.

---

### 3. Thêm đường dẫn header

```cmake
target_include_directories(main PRIVATE
    inc
)
```

Cho phép compiler tìm thấy các file header trong thư mục `inc`.

---

## Kiến thức rút ra

- Cách tổ chức project thành nhiều thư mục (`src`, `inc`).
- Viết file `CMakeLists.txt` cơ bản.
- Sinh Makefile tự động bằng CMake.
- Biên dịch project nhiều file mà không cần viết Makefile thủ công.
- Hiểu quy trình build của CMake.

---

## So sánh Makefile và CMake

| Makefile                      | CMake                            |
| ----------------------------- | -------------------------------- |
| Viết trực tiếp các lệnh build | Sinh Makefile hoặc Ninja tự động |
| Phù hợp project nhỏ           | Phù hợp project vừa và lớn       |
| Phụ thuộc hệ thống build      | Hỗ trợ đa nền tảng               |
| Quản lý thủ công              | Dễ mở rộng và bảo trì            |

---

## Hướng phát triển

Có thể mở rộng project bằng cách:

- Thêm nhiều source file.
- Tạo thư viện (`add_library()`).
- Chia project thành nhiều module.
- Sử dụng `target_link_libraries()`.
- Hỗ trợ Debug và Release Build.
- Tìm hiểu các lệnh `find_package()` và `install()`.

---

## Tài liệu tham khảo

- CMake Documentation
- Modern CMake
- Professional CMake: A Practical Guide
