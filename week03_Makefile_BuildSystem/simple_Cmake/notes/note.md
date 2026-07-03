# Đánh giá chung

Với mục tiêu "hiểu tổng quan và đủ dùng CMake cho Linux BSP", mình đánh giá mức độ ưu tiên của các Step như sau:

⭐⭐⭐⭐⭐ Phải nắm chắc: Step 0, Step 1, Step 3, Step 4.
⭐⭐⭐⭐ Nên hiểu: Step 2, Step 10.
⭐⭐⭐ Biết để đọc code: Step 5, Step 7, Step 9.
⭐⭐ Chỉ cần biết khái niệm: Step 6, Step 8.

Chỉ cần nắm tốt các Step ưu tiên cao, bạn đã có thể đọc và chỉnh sửa phần lớn các CMakeLists.txt trong các dự án Linux BSP thực tế.

# Step 0: Tổng quan về CMake

## Mục tiêu

Sau Step này cần hiểu:

- CMake là gì.
- Vai trò của CMake trong quá trình build.
- Workflow **Configure → Generate → Build**.
- Hai lệnh CMake cơ bản.
- Vì sao nên dùng **Out-of-Source Build**.

---

## 1. CMake là gì?

CMake là một **Build System Generator**.

**CMake không phải là:**

- Compiler (`gcc`, `clang`, ...)
- Build Tool (`make`, `ninja`, ...)

**Nhiệm vụ của CMake:**

- Đọc file `CMakeLists.txt`.
- Kiểm tra môi trường (compiler, thư viện, hệ điều hành...).
- Sinh ra Build System phù hợp.
- Gọi Build Tool để biên dịch chương trình.

Có thể hình dung:

```text
Source Code
      │
      ▼
CMakeLists.txt
      │
      ▼
    CMake
      │
      ▼
Build System
(Makefile/Ninja)
      │
      ▼
 Build Tool
(make/ninja)
      │
      ▼
 Compiler
(gcc/clang)
      │
      ▼
Executable / Library
```

> CMake **không trực tiếp biên dịch chương trình**, mà chuẩn bị và điều phối toàn bộ quá trình build.

---

## 2. Workflow của CMake

Một project CMake luôn trải qua 3 giai đoạn:

```text
Configure
      │
      ▼
Generate
      │
      ▼
Build
```

**Configure**

Đọc `CMakeLists.txt`, kiểm tra môi trường và tạo cache.

```bash
cmake -S . -B build
```

Kết quả:

- Xác định compiler.
- Kiểm tra dependency.
- Sinh `CMakeCache.txt`.

---

**Generate**

Sau khi cấu hình thành công, CMake sinh Build System.

Ví dụ:

- `Makefile`
- `build.ninja`
- `Visual Studio Solution`

> Generator quyết định loại Build System được tạo.

---

**Build**

Biên dịch chương trình.

```bash
cmake --build build
```

Lúc này CMake sẽ gọi Build Tool tương ứng.

Ví dụ với **Make**:

```text
Makefile
     │
     ▼
 make
     │
     ▼
 gcc
```

Hoặc với **Ninja**:

```text
build.ninja
      │
      ▼
 ninja
      │
      ▼
 clang
```

---

## 3. Generator

Generator quyết định **Build System** mà CMake tạo ra.

| Generator      | Build System  |
| -------------- | ------------- |
| Unix Makefiles | `Makefile`    |
| Ninja          | `build.ninja` |
| Visual Studio  | `*.sln`       |

**Lưu ý**

- Generator **không quyết định Compiler**.
- Compiler được phát hiện trong bước **Configure** hoặc được chỉ định bằng **Toolchain**.

---

## 4. Out-of-Source Build

Thay vì build ngay trong thư mục source:

```text
Project/
├── main.cpp
├── CMakeLists.txt
├── Makefile
└── ...
```

CMake khuyến nghị:

```text
Project/
├── main.cpp
├── CMakeLists.txt
└── build/
    ├── Makefile
    ├── CMakeCache.txt
    └── ...
```

**Ưu điểm**

- Source luôn sạch.
- Dễ xoá toàn bộ file build.
- Có thể tạo nhiều cấu hình:

```text
build-debug/
build-release/
build-arm/
```

Đây là cách build mà gần như mọi dự án CMake hiện đại đều sử dụng.

---

## 5. Hai lệnh quan trọng nhất

**Configure**

```bash
cmake -S . -B build
```

- `-S`: thư mục source.
- `-B`: thư mục build.

---

**Build**

```bash
cmake --build build
```

Không cần quan tâm project dùng **Make** hay **Ninja**, CMake sẽ tự gọi Build Tool phù hợp.

---

## 6. Liên hệ với Linux BSP

Trong Linux BSP, CMake thường được dùng để build:

- User-space Application.
- Static/Shared Library.
- Middleware.
- Demo Program.

Khác với lập trình desktop, compiler thường là **Cross Compiler**.

```text
Host PC
   │
   ▼
CMake
   │
   ▼
Build Tool
   │
   ▼
Cross Compiler
(aarch64-linux-gnu-gcc,
 arm-linux-gnueabihf-gcc)
   │
   ▼
Executable chạy trên board ARM
```

---

## Những điều cần ghi nhớ

- CMake **không phải Compiler**.
- CMake **không trực tiếp biên dịch chương trình**.
- CMake là **Build System Generator**, chịu trách nhiệm cấu hình project và sinh Build System.
- Workflow của CMake luôn gồm **Configure → Generate → Build**.
- Generator quyết định **Build System**, không quyết định **Compiler**.
- Nên sử dụng **Out-of-Source Build** để tách source và file build.
- Trong Linux BSP, CMake thường kết hợp với **Cross Compiler** để tạo chương trình cho board nhúng.

# Step 1: Khởi đầu với CMake

## Mục tiêu

Sau Step này cần hiểu:

- Khái niệm **Target**.
- Tạo **Executable** và **Library**.
- Liên kết các Target.
- Chia project thành nhiều module.

---

## 1. Target - Khái niệm quan trọng nhất

Trong CMake, gần như mọi thứ đều xoay quanh **Target**.

Target là một sản phẩm mà CMake quản lý, ví dụ:

- Executable
- Static Library
- Shared Library

Thay vì thao tác trực tiếp với các file `.c`, CMake thao tác trên **Target**.

Ví dụ:

```text
main.c
gpio.c
uart.c
    │
    ▼
   app   (Target)
```

Một Target có thể:

- Chứa source code.
- Liên kết với Target khác.
- Mang các thuộc tính như include path, compiler option,...

> **Tư duy quan trọng:** CMake quản lý **Target**, không quản lý từng file source riêng lẻ.

---

## 2. Tạo Target

**Executable**

Tạo chương trình thực thi:

```cmake
add_executable(app main.c)
```

Nếu có nhiều file source:

```cmake
add_executable(app)

target_sources(app PRIVATE
    main.c
    uart.c
    gpio.c
)
```

`target_sources()` giúp bổ sung source mà không cần sửa lại `add_executable()`.

---

**Library**

Tạo thư viện:

```cmake
add_library(gpio gpio.c)
```

Library có thể là:

- Static Library (`.a`)
- Shared Library (`.so`)

Việc tách thành Library giúp nhiều chương trình có thể dùng chung cùng một phần code.

---

## 3. Liên kết các Target

Một Target có thể sử dụng Target khác thông qua:

```cmake
target_link_libraries(app PRIVATE gpio)
```

Ví dụ:

```text
gpio.c
    │
    ▼
 libgpio.a
      │
      ▼
     app
```

CMake sẽ tự xử lý việc liên kết khi build.

> `target_link_libraries()` mô tả **quan hệ phụ thuộc** giữa các Target, không chỉ đơn thuần là "link thư viện".

---

## 4. Chia Project thành nhiều Module

Project nhỏ có thể chỉ cần một `CMakeLists.txt`.

Khi project lớn hơn, mỗi module nên có một `CMakeLists.txt` riêng.

Ví dụ:

```text
Project/
├── CMakeLists.txt
├── app/
├── drivers/
└── utils/
```

Root `CMakeLists.txt`:

```cmake
add_subdirectory(drivers)
add_subdirectory(utils)
add_subdirectory(app)
```

Ý tưởng:

```text
Root
 ├── drivers
 ├── utils
 └── app
```

Mỗi module tự quản lý source và Target của mình, còn Root chỉ có nhiệm vụ ghép các module lại.

Đây là cách tổ chức phổ biến trong các dự án CMake hiện đại.

---

## 5. Liên hệ với Linux BSP

Một project Linux BSP thường được tổ chức như sau:

```text
project/
├── app/
├── drivers/
├── middleware/
├── board/
└── CMakeLists.txt
```

Trong đó:

- `drivers/` → Library
- `middleware/` → Library
- `board/` → Library
- `app/` → Executable

Các Library sẽ được liên kết vào Executable:

```text
drivers ─┐
middleware├──► app
board ────┘
```

Root `CMakeLists.txt` chủ yếu dùng `add_subdirectory()` để ghép các module thành một project hoàn chỉnh.

---

## Các command cần nhớ

| Command                   | Chức năng               |
| ------------------------- | ----------------------- |
| `project()`               | Khai báo project        |
| `add_executable()`        | Tạo Executable          |
| `add_library()`           | Tạo Library             |
| `target_sources()`        | Thêm source cho Target  |
| `target_link_libraries()` | Liên kết các Target     |
| `add_subdirectory()`      | Thêm module vào project |

---

## Những điều cần ghi nhớ

- **Target** là khái niệm trung tâm của CMake.
- **Executable** và **Library** đều là **Target**.
- `add_executable()` và `add_library()` dùng để tạo Target.
- `target_sources()` thêm source cho Target.
- `target_link_libraries()` mô tả quan hệ phụ thuộc giữa các Target.
- `add_subdirectory()` giúp chia project thành các module độc lập.
- Trong các dự án lớn (đặc biệt là Linux BSP), mỗi module thường có `CMakeLists.txt` riêng, còn Root chỉ đóng vai trò ghép các module.

# Step 2: Ngôn ngữ CMake

## Mục tiêu

Sau bước này cần hiểu:

- `function()` - đóng gói logic có thể tái sử dụng.
- `list()` - quản lý danh sách (đặc biệt là source file).
- `if()` và `foreach()` - điều khiển quá trình build.
- `include()` - tách cấu hình sang file riêng.

Đây là các command xuất hiện rất thường xuyên trong các dự án Linux BSP.

---

## 1. `function()`

`function()` dùng để đóng gói và tái sử dụng một đoạn logic.

```cmake
function(add_driver name)
    add_library(${name} ${name}.c)
endfunction()

add_driver(gpio)
add_driver(uart)
```

Kết quả:

```text
gpio.c → libgpio.a
uart.c → libuart.a
```

**Đặc điểm**

- Có scope riêng, biến bên trong không ảnh hưởng bên ngoài.
- Phù hợp cho các thao tác lặp lại nhiều lần.
- Trong CMake hiện đại, nên ưu tiên `function()` hơn `macro()`.

---

## 2. `list()`

CMake lưu nhiều dữ liệu dưới dạng **List**, phổ biến nhất là danh sách source.

```cmake
set(DRIVER_SRCS
    gpio.c
    uart.c
)

list(APPEND DRIVER_SRCS spi.c)

add_library(drivers ${DRIVER_SRCS})
```

Kết quả:

```text
drivers
├── gpio.c
├── uart.c
└── spi.c
```

**Các thao tác thường gặp**

- `APPEND` : thêm phần tử.
- `REMOVE_ITEM` : xóa phần tử.
- `LENGTH` : lấy số lượng phần tử.

Trong project lớn, `list(APPEND ...)` thường được dùng để bổ sung source theo từng module.

---

## 3. Điều khiển quá trình build

**`if()`**

Build theo điều kiện.

```cmake
option(USE_UART "Enable UART" ON)

if(USE_UART)
    add_library(uart uart.c)
endif()
```

**Ứng dụng**

- Bật/tắt driver.
- Bật/tắt feature.
- Chọn board.
- Chọn compiler.

---

**`foreach()`**

Lặp qua một danh sách.

```cmake
set(DRIVERS gpio uart spi)

foreach(driver ${DRIVERS})
    message(STATUS "Build ${driver}")
endforeach()
```

Kết quả:

```text
Build gpio
Build uart
Build spi
```

Thường dùng khi nhiều module cần xử lý giống nhau.

> `while()` cũng tồn tại nhưng rất ít được sử dụng trong các project CMake thực tế.

---

## 4. `include()`

`include()` dùng để nạp một file `.cmake`, giúp tách cấu hình khỏi `CMakeLists.txt`.

```text
Project/
├── CMakeLists.txt
└── cmake/
    ├── common.cmake
    └── config.cmake
```

```cmake
include(cmake/config.cmake)
include(cmake/common.cmake)
```

Ví dụ:

```cmake
# common.cmake
add_compile_options(-Wall -Wextra)

# config.cmake
set(BOARD stm32)
```

Sau khi `include()`, các biến và cấu hình trong file sẽ được sử dụng như đang viết trực tiếp trong `CMakeLists.txt`.

---

## 5. `include()` và `add_subdirectory()`

Đây là hai command dễ nhầm lẫn nhưng có mục đích khác nhau.

| `include()`                                         | `add_subdirectory()`                |
| --------------------------------------------------- | ----------------------------------- |
| Chia cấu hình                                       | Chia module                         |
| Nạp file `.cmake`                                   | Nạp thư mục có `CMakeLists.txt`     |
| Khai báo biến, option, compiler flags, toolchain... | Tạo hoặc quản lý Executable/Library |

Ví dụ cấu trúc Linux BSP:

```text
project/
├── CMakeLists.txt
├── app/
├── drivers/
└── cmake/
    ├── common.cmake
    ├── toolchain.cmake
    └── board.cmake
```

- `add_subdirectory(app)` → Build ứng dụng.
- `add_subdirectory(drivers)` → Build driver.
- `include(toolchain.cmake)` → Cấu hình compiler.
- `include(board.cmake)` → Cấu hình board.

---

## Tổng kết

| Command              | Chức năng                         |
| -------------------- | --------------------------------- |
| `function()`         | Đóng gói logic có thể tái sử dụng |
| `list()`             | Quản lý danh sách                 |
| `if()`               | Build theo điều kiện              |
| `foreach()`          | Lặp qua danh sách                 |
| `include()`          | Nạp file cấu hình `.cmake`        |
| `add_subdirectory()` | Thêm một module vào project       |

---

## Ghi nhớ

- `function()` giúp tái sử dụng logic và có scope riêng.
- `list()` chủ yếu dùng để quản lý danh sách source.
- `if()` dùng để bật/tắt feature hoặc driver theo điều kiện.
- `foreach()` xử lý nhiều phần tử trong một danh sách.
- `include()` chia nhỏ cấu hình.
- `add_subdirectory()` chia nhỏ module và thường tạo hoặc quản lý Target.

> **Tư duy quan trọng:** Trong các dự án Linux BSP, `include()` dùng để quản lý **cấu hình**, còn `add_subdirectory()` dùng để tổ chức **các thành phần của project** (app, driver, library...). Phân biệt rõ hai vai trò này sẽ giúp bạn đọc và hiểu cấu trúc của hầu hết các dự án CMake thực tế.

# Step 3: Cấu hình Project

## Mục tiêu

Sau bước này cần hiểu:

- Cấu hình project mà không cần sửa `CMakeLists.txt`.
- Sử dụng **Variable**, **Option** và **Cache Variable** để điều khiển quá trình build.
- Hiểu vai trò của `CMakeCache.txt`.
- Biết `CMakePresets.json` dùng để lưu và chia sẻ cấu hình build.

---

## 1. Variable

Biến trong CMake được khai báo bằng `set()`.

```cmake
set(BOARD stm32)
set(PROJECT_NAME BSP)

message(STATUS "Board = ${BOARD}")
```

Kết quả:

```text
-- Board = stm32
```

`message()` thường được dùng để kiểm tra giá trị biến trong quá trình **Configure**.

> **Lưu ý:** Variable thông thường chỉ tồn tại trong lần chạy CMake hiện tại.

---

## 2. Option

`option()` tạo một biến **ON/OFF**, thường dùng để bật hoặc tắt feature.

```cmake
option(ENABLE_UART "Enable UART Driver" ON)

if(ENABLE_UART)
    add_library(uart uart.c)
endif()
```

Có thể thay đổi khi Configure:

```bash
cmake -DENABLE_UART=OFF ..
```

Nếu `ENABLE_UART=OFF`, thư viện `uart` sẽ không được build.

> `option()` là cách phổ biến để bật/tắt driver hoặc tính năng trong project.

---

## 3. Cache Variable

**Cache Variable** là biến được lưu lại trong thư mục build, giúp giữ cấu hình giữa các lần chạy CMake.

```cmake
set(BOARD stm32 CACHE STRING "Target Board")
```

Có thể ghi đè bằng dòng lệnh:

```bash
cmake -DBOARD=imx6 ..
```

Các Cache Variable được lưu trong:

```text
build/
└── CMakeCache.txt
```

Ngoài các Cache Variable, `CMakeCache.txt` còn lưu nhiều thông tin như:

- Generator.
- Compiler.
- Build Type.
- Option.

> Không nên chỉnh sửa trực tiếp `CMakeCache.txt`; hãy thay đổi cấu hình bằng `-D` hoặc Preset.

---

## 4. `CMakePresets.json`

Khi project có nhiều cấu hình build, việc nhập nhiều tùy chọn `-D` sẽ khá bất tiện.

Ví dụ:

```bash
cmake -B build \
      -DCMAKE_BUILD_TYPE=Release \
      -DENABLE_UART=ON \
      -DBOARD=stm32
```

Thay vào đó, có thể lưu sẵn trong `CMakePresets.json`.

Ví dụ các Preset:

- Debug
- Release
- ARM
- x86

Sử dụng:

```bash
cmake --preset release
cmake --build --preset release
```

`CMakePresets.json` giúp mọi thành viên trong dự án sử dụng cùng một cấu hình build.

---

## 5. Liên hệ với Linux BSP

Trong các dự án Linux BSP, cấu hình thường bao gồm:

- Board mục tiêu.
- Toolchain.
- Build Type (`Debug`/`Release`).
- Driver hoặc Feature cần build.
- Đường dẫn SDK hoặc thư viện.

Thay vì sửa `CMakeLists.txt`, các giá trị này thường được truyền qua **Cache Variable** hoặc **Preset**.

---

## Tổng kết

| Thành phần          | Vai trò                         |
| ------------------- | ------------------------------- |
| `set()`             | Khai báo biến                   |
| `option()`          | Bật/tắt Feature                 |
| `message()`         | In thông tin khi Configure      |
| `CACHE`             | Lưu biến trong `CMakeCache.txt` |
| `CMakePresets.json` | Lưu và chia sẻ cấu hình build   |

---

## Ghi nhớ

- Variable dùng để lưu giá trị trong quá trình Configure.
- Option là một dạng biến **ON/OFF** để bật hoặc tắt Feature.
- Cache Variable giúp giữ cấu hình giữa các lần build và có thể ghi đè bằng `-D`.
- `CMakePresets.json` là cách hiện đại để quản lý nhiều cấu hình build.

> **Tư duy quan trọng**
>
> Một project CMake có thể nhìn theo hai góc độ:
>
> ```text
> Project
> │
> ├── Target
> │   ├── Executable
> │   └── Library
> │
> └── Configuration
>     ├── Build Type
>     ├── Board
>     ├── Toolchain
>     ├── Feature
>     └── Compiler Options
> ```
>
> - **Step 1:** Project gồm những gì? → **Target**
> - **Step 3:** Project được build như thế nào? → **Configuration**
>
> Khi đọc `CMakeLists.txt` trong các dự án Linux BSP, hãy luôn phân biệt rõ hai phần này.

# Step 4: Cấu hình Target

## Mục tiêu

Sau bước này cần hiểu:

- CMake hiện đại cấu hình theo **Target**, không cấu hình cho toàn bộ project.
- Mỗi `target_xxx()` chỉ là gắn thêm một thuộc tính cho Target.
- Đọc được phần lớn các `target_xxx()` trong các dự án Linux BSP.

---

## 1. Tư duy cốt lõi

Ở **Step 1**, ta đã biết Target có thể là:

- Executable
- Library

Step này bổ sung một ý quan trọng:

> Một **Target** không chỉ có source, mà còn có nhiều **thuộc tính (Properties)**.

Ví dụ:

```text
app
│
├── Source
├── C11
├── Include Directories
├── Compiler Options
├── Compile Definitions
└── Linked Libraries
```

Mỗi lệnh `target_xxx()` chỉ đang bổ sung hoặc thay đổi **một thuộc tính** của Target.

---

## 2. Các thuộc tính thường gặp của Target

| Thuộc tính          | Command                        | Vai trò                    |
| ------------------- | ------------------------------ | -------------------------- |
| Source              | `target_sources()`             | Thêm source file           |
| Language Standard   | `target_compile_features()`    | Chọn chuẩn C/C++           |
| Include Directories | `target_include_directories()` | Đường dẫn tìm file `.h`    |
| Compile Definitions | `target_compile_definitions()` | Thêm macro (`#define`)     |
| Compile Options     | `target_compile_options()`     | Thêm tùy chọn cho compiler |
| Linked Libraries    | `target_link_libraries()`      | Liên kết thư viện          |

Đây là các command xuất hiện thường xuyên nhất trong các dự án CMake hiện đại.

---

## 3. Ví dụ

```cmake
add_executable(app main.c)

target_compile_features(app PRIVATE c_std_11)

target_compile_definitions(app PRIVATE DEBUG)

target_compile_options(app PRIVATE -Wall)

target_include_directories(app PRIVATE drivers/include)

target_link_libraries(app PRIVATE gpio)
```

**Ý nghĩa**

- Dùng chuẩn **C11**.
- Định nghĩa macro `DEBUG`.
- Bật cảnh báo `-Wall`.
- Tìm header trong `drivers/include`.
- Liên kết với thư viện `gpio`.

Tất cả các cấu hình đều được gắn trực tiếp với Target `app`.

---

## 4. Liên hệ với Linux BSP

Trong các dự án Linux BSP, mỗi Executable hoặc Library thường được cấu hình theo mô hình:

```text
Target
│
├── Source
├── Include Directories
├── Compiler Options
├── Compile Definitions
├── Compile Features
└── Linked Libraries
```

Khi đọc `CMakeLists.txt`, hãy xem mỗi `target_xxx()` là đang bổ sung một thuộc tính cho Target.

---

## Tổng kết

| Command                        | Chức năng              |
| ------------------------------ | ---------------------- |
| `target_sources()`             | Thêm source            |
| `target_compile_features()`    | Chọn chuẩn C/C++       |
| `target_compile_definitions()` | Thêm macro (`#define`) |
| `target_compile_options()`     | Thêm tùy chọn compiler |
| `target_include_directories()` | Thêm đường dẫn header  |
| `target_link_libraries()`      | Liên kết thư viện      |

---

## Ghi nhớ

- CMake hiện đại cấu hình theo **Target**, không theo toàn bộ project.
- Hầu hết các lệnh đều có dạng:

```cmake
target_xxx(<target> ...)
```

Chỉ cần xác định `xxx`, bạn sẽ biết Target đang được cấu hình ở khía cạnh nào.

> **Tư duy quan trọng**
>
> Có thể hình dung một Target như một đối tượng gồm nhiều thuộc tính:
>
> ```text
> Target
> │
> ├── Source
> ├── Language Standard
> ├── Include Directories
> ├── Compile Options
> ├── Compile Definitions
> └── Linked Libraries
> ```
>
> Mỗi lệnh `target_xxx()` chỉ thay đổi **một thuộc tính** của Target.
>
> Ví dụ:
>
> - `target_sources()` → Source
> - `target_compile_features()` → Chuẩn ngôn ngữ
> - `target_compile_options()` → Compiler Options
> - `target_compile_definitions()` → Macro
> - `target_include_directories()` → Include Directories
> - `target_link_libraries()` → Linked Libraries
>
> Đây là tư duy cốt lõi của CMake hiện đại và cũng là mô hình bạn sẽ gặp thường xuyên trong các dự án Linux BSP.

---

## Tổng kết 4 Step đầu

Sau khi hoàn thành 4 Step đầu, bạn có thể hình dung một project CMake theo mô hình sau:

```text
Project
│
├── Target (Step 1)
│
├── Logic (Step 2)
│   ├── function()
│   ├── if()
│   └── list()
│
├── Configuration (Step 3)
│   ├── Option
│   ├── Cache
│   └── Preset
│
└── Target Properties (Step 4)
    ├── Sources
    ├── Includes
    ├── Compiler Options
    ├── Definitions
    └── Libraries
```

Đây là mô hình giúp đọc và hiểu hầu hết các dự án CMake trong Linux BSP.

Sau khi nắm vững 4 ý lớn này, bạn đã có nền tảng đủ tốt để đọc phần lớn các file `CMakeLists.txt`; những command khác thường chỉ là các biến thể hoặc mở rộng của các khái niệm trên.

# Step 5: Các loại Library trong CMake

## Mục tiêu

Sau bước này cần hiểu:

- CMake hỗ trợ những loại Library nào.
- Khi nào nên dùng từng loại.
- Đọc được các project CMake thực tế.

Trong Linux BSP, **Static Library** và **Shared Library** là hai loại phổ biến nhất. **Interface Library** và **Object Library** chủ yếu cần biết khái niệm.

---

## 1. Static Library

**Static Library** được gộp vào Executable khi link.

Trên Linux có phần mở rộng:

```text
.a
```

Ví dụ:

```text
gpio.c
uart.c
    │
    ▼
libdriver.a
    │
    ▼
app
```

Sau khi build:

- Mã của `libdriver.a` đã nằm trong `app`.
- Chạy `app` không cần file `.a`.

**Thường dùng khi**

- Embedded.
- Linux BSP.
- Muốn tạo một Executable độc lập.

---

## 2. Shared Library

**Shared Library** tồn tại riêng và được nạp khi chương trình chạy.

Trên Linux có phần mở rộng:

```text
.so
```

Ví dụ:

```text
app
 │
 └──► libdriver.so
```

Khi chạy, `app` phải tìm thấy `libdriver.so`.

**Thường dùng khi**

- Nhiều chương trình dùng chung một thư viện.
- Muốn cập nhật thư viện mà không cần build lại ứng dụng.

Ví dụ quen thuộc trên Linux:

- `libm.so`
- `libpthread.so`
- `libssl.so`

---

## 3. Interface Library

**Interface Library** không chứa source code và không tạo file thư viện.

Nó chỉ dùng để chia sẻ cấu hình giữa nhiều Target, như:

- Include Directories.
- Compile Options.
- Compile Definitions.

Có thể hiểu đơn giản:

> **Interface Library = một "gói cấu hình" dùng chung.**

Thường gặp trong:

- Header-only Library.
- Cấu hình dùng chung.

---

## 4. Object Library

**Object Library** chỉ biên dịch source thành các file object (`.o`), không tạo Library hay Executable.

Ví dụ:

```text
gpio.c
uart.c
    │
    ▼
gpio.o
uart.o
```

Các file `.o` này có thể được nhiều Target sử dụng.

**Mục đích**

- Tái sử dụng object file.
- Giảm thời gian build trong project lớn.

> Ở giai đoạn hiện tại, chỉ cần biết khái niệm này.

---

**So sánh nhanh**

| Loại      | Sinh ra         | Mục đích                           |
| --------- | --------------- | ---------------------------------- |
| Static    | `.a`            | Gộp vào Executable khi link        |
| Shared    | `.so`           | Dùng chung giữa nhiều chương trình |
| Interface | Không sinh file | Chia sẻ cấu hình                   |
| Object    | `.o`            | Tái sử dụng object file            |

---

## 5. Liên hệ với Linux BSP

- **Static Library:** Rất phổ biến.
- **Shared Library:** Thường gặp ở các ứng dụng chạy trên Linux.
- **Interface Library:** Chia sẻ cấu hình hoặc hỗ trợ thư viện header-only.
- **Object Library:** Ít gặp, chủ yếu trong các project lớn.

---

**Ghi nhớ**

- **Static Library (`.a`)**: Mã được gộp vào Executable.
- **Shared Library (`.so`)**: Tồn tại riêng và được nạp khi chương trình chạy.
- **Interface Library**: Không chứa code, không tạo file thư viện, chỉ chia sẻ cấu hình.
- **Object Library**: Chỉ tạo các file `.o` để tái sử dụng.

> **Tư duy quan trọng**
>
> Có thể hình dung một project CMake như sau:
>
> ```text
> Project
> │
> ├── Executable
> │
> └── Libraries
>     ├── Static (.a)
>     ├── Shared (.so)
>     ├── Interface (Configuration)
>     └── Object (.o)
> ```
>
> Đối với Linux BSP:
>
> - Nắm chắc **Static Library** và **Shared Library**.
> - Chỉ cần nhận biết **Interface Library** và **Object Library** khi đọc `CMakeLists.txt`.
>
> Đây là mức kiến thức đủ để đọc hiểu phần lớn các dự án CMake trong thực tế.

---

## Tổng kết 5 Step đầu

Sau khi hoàn thành 5 Step đầu, bạn có thể nhìn một project CMake theo mô hình sau:

```text
CMake
│
├── Step 1: Target
│
├── Step 2: Logic
│   ├── function()
│   ├── if()
│   ├── list()
│   └── include()
│
├── Step 3: Configuration
│   ├── Variable
│   ├── Option
│   ├── Cache
│   └── Preset
│
├── Step 4: Target Properties
│   ├── Sources
│   ├── Includes
│   ├── Compiler Options
│   ├── Definitions
│   └── Libraries
│
└── Step 5: Library Types
    ├── Static
    ├── Shared
    ├── Interface
    └── Object
```

Đây là một cách tổ chức giúp hiểu CMake một cách tổng quan và đủ dùng. Với 5 bước này, bạn đã có nền tảng để đọc được phần lớn các file `CMakeLists.txt` trong các dự án Linux BSP.

Khi gặp một command mới, hãy tự hỏi:

- Nó đang tạo **Target**?
- Nó đang cấu hình **Target**?
- Nó đang điều khiển **logic**?
- Hay nó đang cấu hình **quá trình build**?

Nếu trả lời được câu hỏi đó, bạn sẽ hiếm khi bị "lạc" khi đọc một dự án CMake mới. Đây cũng là cách nhiều kỹ sư tiếp cận các codebase CMake lớn như **Qt**, **OpenCV** hay **LLVM**.

# Step 6 - Kiểm tra môi trường Build

**Mục tiêu**

Sau bước này cần hiểu:

- Trong giai đoạn **Configure**, CMake có thể tự kiểm tra môi trường build.
- Dựa trên kết quả kiểm tra, CMake sẽ quyết định cách cấu hình project.
- Mục đích là giúp một project có thể build trên nhiều hệ điều hành, compiler và toolchain khác nhau.

---

## 1. CMake kiểm tra những gì?

Trong quá trình **Configure**, CMake có thể kiểm tra:

- Header có tồn tại không?
- Đoạn code có biên dịch được không?
- Compiler có hỗ trợ tính năng cần thiết không?

**Ví dụ**

**Header**

- `pthread.h`
- `unistd.h`
- `sys/socket.h`

**Compile**

- ✓ Thread
- ✓ Socket
- ✓ API mới

**Compiler**

- ✓ C11/C17
- ✓ C++17/C++20
- ✓ LTO/IPO

---

## 2. Dựa vào kết quả kiểm tra

Sau khi kiểm tra, CMake sẽ quyết định:

- Bật hoặc tắt một feature.
- Chọn cách triển khai phù hợp.
- Báo lỗi nếu thiếu thành phần bắt buộc.

**Ví dụ**

```text
Có pthread.h
        │
        ▼
Bật hỗ trợ Thread

Không có pthread.h
        │
        ▼
Tắt feature hoặc báo lỗi
```

Nhờ đó, cùng một project có thể hoạt động trên nhiều nền tảng mà không cần sửa mã nguồn.

---

## 3. Liên hệ với Linux BSP

Trong các dự án Linux BSP, việc kiểm tra môi trường thường dùng để xác định:

- Header hoặc thư viện có sẵn.
- Compiler hoặc toolchain có hỗ trợ tính năng cần thiết.
- Một đoạn code có thể biên dịch thành công hay không.

Mục tiêu là giúp project tương thích với nhiều:

- Hệ điều hành.
- Compiler.
- Toolchain.
- Board.

Khi đọc các dự án BSP, bạn sẽ thường gặp các đoạn kiểm tra này, nhưng hiếm khi phải tự viết.

---

**Ghi nhớ**

- CMake có thể tự kiểm tra môi trường trước khi build.
- Các kiểm tra phổ biến gồm:
  - Header
  - Compile
  - Compiler
- Kết quả kiểm tra sẽ ảnh hưởng đến cấu hình và tính năng được build.

---

**Tư duy quan trọng**

Có thể hình dung giai đoạn **Configure** như sau:

```text
Configure
│
├── Tìm Compiler
├── Tìm Library
├── Kiểm tra môi trường
│   ├── Header
│   ├── Compile
│   └── Compiler
└── Sinh Build System
```

Điều này giải thích vì sao bước **Configure** đôi khi mất vài giây: trước khi sinh **Makefile** hoặc **Ninja**, CMake cần "thăm dò" môi trường để biết hệ thống hiện tại hỗ trợ những gì.

Đối với mục tiêu hiểu CMake tổng quan và đủ dùng, bạn không cần nhớ tên các module hay command cụ thể. Chỉ cần nhớ tư duy:

> **CMake luôn kiểm tra môi trường trước, sau đó mới quyết định cách build project.**

Đây là cơ chế bạn sẽ thường xuyên bắt gặp khi đọc các dự án Linux BSP.

# Step 7 - Custom Command và Generated Files

**Mục tiêu**

Sau Step này cần hiểu:

- CMake có thể chạy một chương trình hoặc script trước khi build.
- Chương trình đó có thể tự động sinh ra các file nguồn hoặc file cấu hình.
- Các file được tạo trong quá trình build gọi là **Generated Files**.

---

## 1. Generated Files

Thông thường, lập trình viên tự viết mã nguồn:

```text
main.c
gpio.c
    │
    ▼
Compiler
```

Nhưng trong nhiều dự án, một phần mã nguồn được tạo tự động:

```text
File cấu hình
      │
      ▼
Script / Generator
      │
      ▼
generated.c
generated.h
      │
      ▼
Compiler
```

Các file như `generated.c`, `generated.h`, `version.h`... được gọi là **Generated Files**.

---

## 2. Custom Command

Để tạo các file này, CMake có thể chạy một chương trình hoặc script trước khi biên dịch.

Ví dụ:

```text
config.json
      │
      ▼
generate.py
      │
      ▼
config.c
config.h
      │
      ▼
Compiler
```

Nếu `config.json` thay đổi, chỉ cần build lại, các file sinh ra sẽ được cập nhật tự động.

---

## 3. Một số ví dụ thực tế

**Sinh file phiên bản**

```text
version.txt
      │
      ▼
Generator
      │
      ▼
version.h
```

Khi đổi phiên bản (ví dụ `1.2.3 → 1.2.4`), chỉ cần build lại, `version.h` sẽ được cập nhật.

**Code Generator**

Một số công cụ sẽ sinh mã nguồn trước khi build, chẳng hạn:

```text
message.proto
      │
      ▼
Protobuf
      │
      ▼
message.pb.c
message.pb.h
```

```text
parser.y
    │
    ▼
Bison
    │
    ▼
parser.c
```

Bạn chưa cần học các công cụ này, chỉ cần biết chúng đều là **Code Generator**.

---

## 4. Liên hệ với Linux BSP

Trong Linux BSP, bạn có thể gặp các quy trình như:

```text
JSON
 │
 ▼
Python
 │
 ▼
config.c
```

```text
Excel
 │
 ▼
Python
 │
 ▼
pin_config.h
```

```text
Device Tree
 │
 ▼
Tool
 │
 ▼
device_config.h
```

Điểm chung:

- Lập trình viên chỉnh sửa file đầu vào.
- CMake chạy công cụ để sinh mã nguồn.
- Sau đó mới tiến hành build.

---

**Những điều cần ghi nhớ**

- CMake không chỉ biên dịch mã nguồn có sẵn.
- CMake còn có thể chạy script hoặc chương trình trước khi build.
- Các file được tạo trong quá trình build gọi là **Generated Files**.

Khi gặp các file như:

- `version.h`
- `config.h`
- `pinmux.c`
- `*_generated.c`

thì nhiều khả năng chúng được sinh tự động, không phải viết tay.

---

**Tư duy cần nhớ**

Quy trình build của CMake không phải lúc nào cũng là:

```text
Source Code
    │
    ▼
Compiler
    │
    ▼
Executable
```

Nhiều dự án sẽ có thêm bước sinh mã nguồn:

```text
Source / Config
      │
      ▼
Code Generator
      │
      ▼
Generated Files
      │
      ▼
Compiler
      │
      ▼
Executable
```

Đây là quy trình rất phổ biến trong các dự án Linux BSP và Embedded.

> Ở giai đoạn hiện tại, chưa cần biết cách viết **Custom Command**. Chỉ cần hiểu rằng CMake có thể tạo ra mã nguồn trước khi biên dịch và nhận biết các **Generated Files** khi đọc project là đủ.

# Step 8 - Testing với CTest

**Mục tiêu**

Sau Step này cần hiểu:

- CMake hỗ trợ hệ thống kiểm thử (Testing).
- **CTest** là công cụ dùng để chạy các bài kiểm thử sau khi build.

---

## 1. CTest là gì?

Sau khi build, **CTest** sẽ chạy các chương trình test và báo kết quả **PASS** hoặc **FAIL**.

```text
Source Code
      │
      ▼
Build
      │
      ▼
Executable + Test
      │
      ▼
CTest
      │
      ▼
PASS / FAIL
```

---

## 2. Cách sử dụng

Build project:

```bash
cmake --build build
```

Chạy toàn bộ test:

```bash
ctest
```

Nếu tất cả đều đúng:

```text
Test #1 : PASS
Test #2 : PASS
```

---

## 3. Trong dự án thực tế

Bạn có thể gặp cấu trúc:

```text
project/
├── app/
├── drivers/
└── tests/
```

hoặc các file như:

```text
test_gpio
test_uart
test_spi
```

Sau khi build, chỉ cần chạy:

```bash
ctest
```

để kiểm tra toàn bộ chương trình.

---

## Những điều cần ghi nhớ

- **CTest** là công cụ đi kèm với CMake để chạy test.
- Sau khi build, dùng:

```bash
ctest
```

để chạy tất cả các bài kiểm thử đã đăng ký.

- Ở giai đoạn này, chưa cần học cách viết test với CTest, chỉ cần biết khi gặp `ctest` hoặc thư mục `tests/` thì đó là bước kiểm tra chương trình sau khi build.

# Step 9 - Installation (Cài đặt sau khi Build)

**Mục tiêu**

Sau Step này cần hiểu:

- Sau khi build, CMake có thể cài đặt (**install**) chương trình hoặc thư viện vào hệ thống.
- Sau khi được cài đặt, các project khác có thể sử dụng chúng.

---

## 1. Install là gì?

Sau khi build, các file thường nằm trong thư mục `build/`.

Ví dụ:

```text
build/
├── app
├── libgpio.a
└── gpio.h
```

Khi chạy:

```bash
cmake --install build
```

CMake sẽ copy các file cần thiết đến những vị trí chuẩn của hệ thống, ví dụ:

```text
/usr/bin/app
/usr/lib/libgpio.a
/usr/include/gpio.h
```

Lúc này, chương trình hoặc thư viện không còn chỉ nằm trong thư mục `build/` mà đã sẵn sàng để người dùng hoặc project khác sử dụng.

---

## 2. Build Artifacts là gì?

**Build Artifacts** là các file được tạo ra sau quá trình build.

Ví dụ:

```text
Source Code
     │
     ▼
Build
     │
     ▼
app
libgpio.a
libgpio.so
```

Các Build Artifacts thường gặp gồm:

- Executable (`app`)
- Static Library (`.a`)
- Shared Library (`.so`)
- Generated Files (nếu có)

Ngoài ra, khi install, CMake cũng có thể copy các file header (`.h`) cần thiết để project khác sử dụng.

---

## 3. Liên hệ với Linux BSP

Ví dụ build một thư viện:

```text
libcamera.so
camera.h
```

Sau khi cài đặt:

```text
/usr/lib/libcamera.so
/usr/include/camera.h
```

Các ứng dụng hoặc project khác trên hệ thống có thể sử dụng thư viện này mà không cần copy thủ công.

---

## 4. Export Package _(Biết để đọc)_

Các thư viện lớn thường cho phép project khác sử dụng rất dễ dàng.

Ví dụ:

```cmake
find_package(MyLib REQUIRED)

target_link_libraries(app PRIVATE MyLib)
```

Có thể hình dung:

```text
find_package(MyLib)
        │
        ▼
CMake tìm thư viện đã được install
```

Ở giai đoạn hiện tại, chỉ cần biết đây là cơ chế giúp các project chia sẻ và sử dụng thư viện với nhau.

---

## 5. Version File _(Biết để đọc)_

Ví dụ:

```cmake
find_package(MyLib 2.0 REQUIRED)
```

Ý nghĩa:

- Chỉ chấp nhận thư viện từ phiên bản **2.0** trở lên.
- Nếu phiên bản không phù hợp, CMake sẽ báo lỗi.

**Version File** giúp `find_package()` kiểm tra tính tương thích giữa các project.

---

**Liên hệ với quy trình Build**

Đến Step 9, quy trình của một project CMake có thể hình dung như sau:

```text
Source Code
      │
      ▼
Build
      │
      ▼
Executable / Library
      │
      ▼
Install
      │
      ▼
/usr/bin
/usr/lib
/usr/include
```

Build tạo ra sản phẩm, còn **Install** đưa sản phẩm đến vị trí chuẩn để người dùng hoặc project khác có thể sử dụng.

---

**Những điều cần ghi nhớ**

- Sau khi build, có thể dùng:

```bash
cmake --install build
```

để cài đặt chương trình hoặc thư viện.

- Install giúp người dùng hoặc project khác sử dụng sản phẩm mà không cần lấy trực tiếp từ thư mục `build/`.
- `find_package()` giúp tìm và sử dụng các thư viện đã được cài đặt.
- **Version File** giúp kiểm tra phiên bản thư viện khi sử dụng.

---

**Một tư duy quan trọng**

Nếu các Step trước tập trung vào cách build project, thì Step 9 giới thiệu bước cuối cùng:

```text
Source Code
      │
      ▼
Build
      │
      ▼
Executable / Library
      │
      ▼
Install
      │
      ▼
Người dùng / Project khác
```

Đây là lý do nhiều dự án không chỉ có lệnh:

```bash
cmake --build build
```

mà còn có:

```bash
cmake --install build
```

> Với mục tiêu hiểu CMake ở mức tổng quan và đủ dùng, bạn chưa cần học cách viết các lệnh `install()`, `find_package()` hay tạo Version File. Chỉ cần hiểu vai trò của **Install** và nhận biết các cơ chế này khi đọc project là đủ.

# Step 10 - Finding Dependencies

**Mục tiêu**

Sau Step này cần hiểu:

- CMake có thể tự tìm các thư viện hoặc công cụ cần thiết trước khi build.
- CMake tự quản lý các dependency giữa các library.

---

## 1. `find_package()`

`find_package()` dùng để tìm các thư viện hoặc package đã được cài trên hệ thống.

Ví dụ:

```cmake
find_package(OpenSSL REQUIRED)
```

Ý nghĩa:

- Nếu tìm thấy **OpenSSL** → tiếp tục build.
- Nếu không tìm thấy → CMake báo lỗi.

Sau đó có thể sử dụng thư viện:

```cmake
target_link_libraries(app PRIVATE OpenSSL::SSL)
```

Thay vì tự tìm đường dẫn đến thư viện và header, CMake sẽ thực hiện việc đó nếu package được cấu hình đúng.

---

## 2. Ví dụ thực tế

Một project cần các thư viện:

- OpenSSL
- Threads
- SQLite

Chỉ cần:

```cmake
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)
```

CMake sẽ kiểm tra chúng đã được cài đặt hay chưa trước khi build.

---

## 3. Transitive Dependency

Đây là một trong những ưu điểm quan trọng của CMake hiện đại.

Giả sử:

```text
app
 │
 ▼
network
 │
 ▼
crypto
```

- `app` sử dụng `network`.
- `network` sử dụng `crypto`.

Nếu `network` đã khai báo phụ thuộc vào `crypto`, thì:

```cmake
target_link_libraries(app PRIVATE network)
```

là đủ.

CMake sẽ tự liên kết thêm `crypto`.

Có thể hiểu đơn giản:

> Mỗi library tự khai báo dependency của mình, còn CMake sẽ tự kết nối chúng.

---

## 4. Tìm các công cụ khác

CMake không chỉ tìm thư viện mà còn có thể kiểm tra các công cụ cần thiết cho quá trình build, ví dụ:

- Python
- Git
- Protobuf (`protoc`)
- Device Tree Compiler (`dtc`)

Nếu thiếu công cụ cần thiết, CMake sẽ báo lỗi ngay từ bước **Configure**.

---

**Liên hệ với Linux BSP**

Trong các dự án Linux BSP, bạn thường gặp:

```cmake
find_package(...)
```

để tìm:

- Thư viện của hệ thống.
- Công cụ phục vụ build.
- Package do project khác cung cấp.

Điều này giúp project có thể build trên nhiều máy mà không cần cấu hình thủ công.

---

**Những điều cần ghi nhớ**

- `find_package()` là cách chuẩn để sử dụng thư viện hoặc package bên ngoài.
- CMake tự tìm các thư viện và công cụ cần thiết trước khi build.
- Một library có thể tự mang theo các dependency của nó (**Transitive Dependency**), giúp project không phải link lặp lại.

---

**Một tư duy quan trọng**

Trước khi build, CMake thường phải kiểm tra các dependency của project:

```text
Project
    │
    ▼
Tìm Dependency
    │
    ├── Library
    ├── Tool
    └── Package khác
    │
    ▼
Build
```

Điểm mạnh của CMake là quản lý dependency. Thay vì tự tìm thư viện, tự cấu hình đường dẫn và tự link từng dependency, bạn chỉ cần khai báo những gì project cần, phần còn lại CMake sẽ xử lý nếu project được cấu hình đúng.

> Với mục tiêu hiểu CMake ở mức tổng quan và đủ dùng, bạn chỉ cần nhớ:
>
> - `find_package()` giúp tìm và sử dụng thư viện hoặc package bên ngoài.
> - **Transitive Dependency** giúp một library tự mang theo các dependency của nó, nên project chỉ cần link trực tiếp những library mình sử dụng.
