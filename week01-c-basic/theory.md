**NGÔN NGỮ C CƠ BẢN**

1. Kiểu dữ liệu, biến, toán tử
2. Hàm, đệ quy
3. Con trỏ, mảng
4. Struct/Union/Enum
5. Dynamic memory

# 1. Kiểu dữ liệu, biến, toán tử

## 1. Data Types

### 1.1 Kiểu dữ liệu cơ bản (Basic Data Types)

**Định nghĩa**

Là các kiểu dữ liệu có sẵn trong ngôn ngữ C dùng để lưu trữ số, ký tự và các giá trị cơ bản.

**Mục đích**

Chọn kiểu dữ liệu phù hợp để biểu diễn dữ liệu và sử dụng bộ nhớ hiệu quả.

**Ví dụ**

```c
char c = 'A';
int a = 10;
float f = 3.14f;
double d = 3.1415926;
```

### 1.2 Kích thước dữ liệu (`sizeof`)

**Định nghĩa**

`sizeof` là toán tử dùng để lấy kích thước (tính bằng byte) của một kiểu dữ liệu hoặc một biến.

**Mục đích**

Biết lượng bộ nhớ cần sử dụng hoặc cấp phát.

**Ví dụ**

```c
sizeof(int);
sizeof(a);
```

### 1.3 `signed` / `unsigned`

**Định nghĩa**

Quy định kiểu số nguyên có dấu (`signed`) hoặc không có dấu (`unsigned`).

**Mục đích**

Lựa chọn phạm vi giá trị phù hợp với dữ liệu cần lưu.

**Ví dụ**

```c
signed int a = -10;
unsigned int b = 100;
```

### 1.4 Integer Promotion

**Định nghĩa**

Các kiểu số nguyên nhỏ như `char` và `short` sẽ được tự động chuyển thành `int` (hoặc `unsigned int` nếu cần) khi tham gia vào biểu thức.

**Mục đích**

Giúp các phép tính được thực hiện thống nhất và phù hợp với kiến trúc CPU.

**Ví dụ**

```c
char a = 10;
char b = 20;

int c = a + b;
```

### 1.5 `<stdint.h>`

**Định nghĩa**

Là thư viện cung cấp các kiểu số nguyên có kích thước cố định.

**Mục đích**

Đảm bảo kích thước dữ liệu nhất quán trên nhiều nền tảng.

**Ví dụ**

```c
#include <stdint.h>

uint8_t  a;
int32_t  b;
uint64_t c;
```

### 1.6 Type Casting

**Định nghĩa**

Là việc chuyển đổi giá trị từ kiểu dữ liệu này sang kiểu dữ liệu khác.

**Mục đích**

Điều khiển cách dữ liệu được xử lý hoặc tính toán.

**Ví dụ**

```c
int a = 5;
float b = (float)a;
```

### 1.7 Constants

**Định nghĩa**

Là các giá trị không thay đổi trong chương trình.

**Mục đích**

Giúp mã nguồn dễ đọc, dễ bảo trì và hạn chế sử dụng các "magic number".

**Ví dụ**

```c
const int MAX = 100;

#define PI 3.14
```

### 1.8 Type Qualifiers (`const`, `volatile`)

**Định nghĩa**

Là các từ khóa bổ sung thuộc tính cho một kiểu dữ liệu hoặc biến.

**Mục đích**

- `const`: Không cho phép thay đổi giá trị của đối tượng.
- `volatile`: Thông báo rằng giá trị có thể thay đổi ngoài sự kiểm soát của chương trình, vì vậy compiler phải luôn đọc/ghi trực tiếp từ bộ nhớ.

**Ví dụ**

```c
const int a = 10;

volatile int status;
```
