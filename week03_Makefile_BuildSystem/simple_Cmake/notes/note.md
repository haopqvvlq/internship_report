# 1. Cấu trúc cơ bản của `CMakeLists.txt`

Một `CMakeLists.txt` trong project C/C++ cơ bản thường gồm các bước sau:

```cmake
# 1. Yêu cầu phiên bản CMake
cmake_minimum_required(VERSION 3.16)

# 2. Khai báo project
project(MyProject C)

# 3. Thiết lập chung (tùy chọn)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 4. Tạo các target
add_library(...)
add_executable(...)

# 5. Cấu hình cho từng target
target_include_directories(...)
target_link_libraries(...)
target_compile_options(...)
target_compile_definitions(...)

# 6. Cài đặt / Kiểm thử (tùy chọn)
install(...)
enable_testing()
add_test(...)
```

---

## Ý nghĩa từng phần

**1. `cmake_minimum_required()`**

Khai báo phiên bản CMake tối thiểu cần có để build project.

```cmake
cmake_minimum_required(VERSION 3.16)
```

Có thể hiểu là:

> "Muốn build project này thì phải dùng CMake phiên bản 3.16 trở lên."

---

**2. `project()`**

Khai báo tên project và ngôn ngữ sử dụng.

```cmake
project(MyProject C)
```

Hoặc:

```cmake
project(MyProject C CXX)
```

Nếu project dùng cả C và C++.

---

**3. Thiết lập chung (`set()`)**

Thiết lập các biến cấu hình cho project hoặc giá trị mặc định.

Ví dụ:

```cmake
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
```

**Lưu ý**

`set()` chỉ là lệnh gán giá trị cho biến trong CMake. Một số biến đặc biệt (ví dụ `CMAKE_C_STANDARD`) sẽ ảnh hưởng đến quá trình build.

---

**4. Tạo target**

Đây là phần trung tâm của CMake.

Mỗi lệnh `add_library()` hoặc `add_executable()` sẽ tạo ra một **target**, và hầu hết các lệnh phía sau đều dùng để cấu hình target này.

Ví dụ:

```cmake
add_library(math STATIC
    math.c
)

add_executable(app
    main.c
)
```

Sau bước này, CMake biết project sẽ tạo ra:

- Thư viện `math`
- Chương trình thực thi `app`

---

**5. Cấu hình target**

Sau khi tạo target, sử dụng các lệnh `target_*` để cấu hình riêng cho từng target.

| Lệnh                           | Mục đích                     |
| ------------------------------ | ---------------------------- |
| `target_include_directories()` | Khai báo thư mục chứa header |
| `target_link_libraries()`      | Liên kết với thư viện khác   |
| `target_compile_options()`     | Thêm tùy chọn compiler       |
| `target_compile_definitions()` | Thêm macro (`-D...`)         |

Ví dụ:

```cmake
target_include_directories(app PRIVATE include)

target_link_libraries(app PRIVATE math)

target_compile_options(app PRIVATE
    -Wall
    -Wextra
)
```

Đây là cách cấu hình được khuyến khích trong CMake hiện đại vì mỗi thiết lập chỉ ảnh hưởng đến target được chỉ định.

---

**6. Cài đặt / Kiểm thử (tùy chọn)**

Nếu cần cài đặt hoặc viết test:

```cmake
install(TARGETS app DESTINATION bin)
```

Hoặc:

```cmake
enable_testing()

add_test(NAME mytest COMMAND app)
```

---

## Ví dụ một project hoàn chỉnh

```
project/
│
├── CMakeLists.txt
├── include/
│   └── math.h
├── main.c
└── math.c
```

`CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.16)

project(MyProject C)

set(CMAKE_C_STANDARD 11)

add_library(math
    math.c
)

add_executable(app
    main.c
)

target_include_directories(app PRIVATE
    include
)

target_link_libraries(app PRIVATE
    math
)
```

---

## Quy trình CMake

```text
Đọc CMakeLists.txt
        │
        ▼
Thực thi từng lệnh từ trên xuống dưới
        │
        ▼
Tạo các target
        │
        ▼
Cấu hình các target
        │
        ▼
Sinh hệ thống build
(Makefile, Ninja, ...)
        │
        ▼
Build bằng công cụ tương ứng
```
