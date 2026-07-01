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

## 2. Variables

### 2.1 Khai báo biến (Declaration)

**Định nghĩa**

Đặt tên và chỉ định kiểu dữ liệu cho biến.

**Mục đích**

Để compiler biết biến có kiểu dữ liệu gì.

**Ví dụ**

```c
int a;
float b;
```

### 2.2 Khởi tạo biến (Initialization)

**Định nghĩa**

Gán giá trị ban đầu cho biến ngay khi khai báo.

**Mục đích**

Giúp biến có giá trị xác định ngay từ đầu.

**Ví dụ**

```c
int a = 10;
float b = 3.14f;
```

### 2.3 Scope (Phạm vi)

**Định nghĩa**

Là phạm vi mà một biến có thể được truy cập.

**File scope**

Biến được khai báo ngoài mọi hàm, có thể được sử dụng trong phạm vi file.

**Block scope**

Biến được khai báo trong một khối `{ }`, chỉ sử dụng được trong khối đó.

**Phân loại biến theo phạm vi**

- Global variable
- Local variable

### 2.4 Lifetime (Thời gian tồn tại)

**Định nghĩa**

Là khoảng thời gian một biến tồn tại trong bộ nhớ.

**Automatic**

Được tạo khi vào block/hàm và bị hủy khi thoát block/hàm.

**Static**

Được tạo một lần và tồn tại đến khi chương trình kết thúc.

### 2.5 Storage Class

**Định nghĩa**

Quy định cách lưu trữ, phạm vi và thời gian tồn tại của biến.

- `auto`: Biến cục bộ mặc định.
- `static`: Giữ giá trị giữa các lần gọi hàm hoặc giới hạn phạm vi trong file.
- `extern`: Khai báo biến được định nghĩa ở nơi khác.
- `register`: Gợi ý compiler lưu biến vào thanh ghi CPU (compiler có thể bỏ qua).

### 2.6 Declaration vs Definition

**Declaration**

Thông báo tên và kiểu của biến để compiler biết biến tồn tại.

```c
extern int count;
```

**Definition**

Tạo biến và cấp phát bộ nhớ cho biến.

```c
int count = 0;
```

### 2.7 Memory Segments

**Stack**

Chứa các biến cục bộ thông thường.

**Data**

Chứa biến toàn cục hoặc `static` đã được khởi tạo.

**BSS**

Chứa biến toàn cục hoặc `static` chưa được khởi tạo hoặc được khởi tạo bằng `0`.

## 3. Operators

### 3.1 Toán tử số học (Arithmetic Operators)

**Định nghĩa**

Thực hiện các phép tính số học cơ bản.

**Các toán tử**

`+` `-` `*` `/` `%`

**Ví dụ**

```c
int c = a + b;
int r = a % b;
```

### 3.2 Toán tử gán (Assignment Operators)

**Định nghĩa**

Gán hoặc cập nhật giá trị cho biến.

**Các toán tử**

`=` `+=` `-=` `*=` `/=` `%=` `<<=` `>>=` `&=` `|=` `^=`

**Ví dụ**

```c
a += 5;
```

### 3.3 Toán tử tăng giảm (Increment / Decrement)

**Định nghĩa**

Tăng hoặc giảm giá trị của biến đi 1.

**Prefix (`++a`)**

Tăng trước rồi mới sử dụng.

**Postfix (`a++`)**

Sử dụng trước rồi mới tăng.

**Ví dụ**

```c
++a;
b = a++;
```

### 3.4 Toán tử so sánh (Relational Operators)

**Định nghĩa**

So sánh hai giá trị, kết quả trả về là `0` hoặc `1`.

**Các toán tử**

`==` `!=` `>` `<` `>=` `<=`

**Ví dụ**

```c
if (a >= b)
```

### 3.5 Toán tử logic (Logical Operators)

**Định nghĩa**

Kết hợp hoặc phủ định các điều kiện logic, kết quả trả về là `0` hoặc `1`.

**Các toán tử**

`&&` `||` `!`

**Short-circuit**

Nếu kết quả đã được xác định thì các biểu thức còn lại sẽ không được đánh giá.

**Ví dụ**

```c
if (a > 0 && b > 0)
```

### 3.6 Toán tử thao tác bit (Bitwise Operators)

**Định nghĩa**

Thực hiện các phép toán trực tiếp trên từng bit của số nguyên.

**Các toán tử**

`&` `|` `^` `~` `<<` `>>`

**Ứng dụng**

- Set bit
- Clear bit
- Toggle bit
- Check bit

**Ví dụ**

```c
reg |= (1 << 3);
```

### 3.7 Toán tử điều kiện (Conditional Operator)

**Định nghĩa**

Viết gọn câu lệnh `if...else`.

**Cú pháp**

```c
condition ? expr1 : expr2;
```

**Ví dụ**

```c
max = (a > b) ? a : b;
```

### 3.8 Toán tử địa chỉ và giải tham chiếu

**Định nghĩa**

Làm việc với địa chỉ bộ nhớ và con trỏ.

- `&`: Lấy địa chỉ của biến.
- `*`: Truy cập giá trị tại địa chỉ mà con trỏ đang trỏ tới.

**Ví dụ**

```c
int *p = &a;
printf("%d", *p);
```

### 3.9 Toán tử dấu phẩy (Comma Operator)

**Định nghĩa**

Đánh giá nhiều biểu thức từ trái sang phải và trả về giá trị của biểu thức cuối cùng.

**Ví dụ**

```c
a = (x = 3, x + 2);
```

### 3.10 Độ ưu tiên và tính kết hợp (Operator Precedence & Associativity)

**Định nghĩa**

Quy định thứ tự thực hiện và hướng kết hợp của các toán tử trong một biểu thức.

**Ví dụ**

```c
a + b * c;
x = y = z = 0;
```

**Lưu ý**

Có thể dùng dấu `()` để làm rõ thứ tự tính toán.

### 3.11 Hành vi không xác định (Undefined Behavior)

**Định nghĩa**

Là những trường hợp mà chuẩn C không quy định kết quả. Chương trình có thể hoạt động khác nhau tùy compiler hoặc môi trường.

**Ví dụ**

```c
i = i++;
```

**Lưu ý**

Không nên thay đổi cùng một biến nhiều lần trong một biểu thức.
