**NGÔN NGỮ C CƠ BẢN**
1. Kiểu dữ liệu, biến, toán tử
2. Hàm, đệ quy
3. Con trỏ, mảng
4. Struct/Union/Enum
5. Dynamic memory

# 1. Kiểu dữ liệu, biến, toán tử
## 1.1 Kiểu dữ liệu cơ bản (Basic Data Types)

Kiểu dữ liệu (Data Type) cho biết **biến sẽ lưu loại dữ liệu gì, chiếm bao nhiêu bộ nhớ và phạm vi giá trị có thể lưu.**

> **Lưu ý**: Kích thước của một số kiểu (int, long,...) phụ thuộc vào hệ điều hành và trình biên dịch. Phần sau sẽ học kỹ hơn với `sizeof`.

| Kiểu dữ liệu     | Dùng để lưu                                     | Phạm vi giá trị (thường gặp)                    | Ví dụ khai báo                         |
| ---------------- | ----------------------------------------------- | ----------------------------------------------- | -------------------------------------- |
| `char`           | Một ký tự hoặc số nguyên nhỏ                    | -128 → 127 (`signed`) hoặc 0 → 255 (`unsigned`) | `char c = 'A';`                        |
| `short`          | Số nguyên nhỏ                                   | -32,768 → 32,767                                | `short year = 2025;`                   |
| `int`            | Số nguyên                                       | -2,147,483,648 → 2,147,483,647                  | `int age = 20;`                        |
| `long`           | Số nguyên lớn                                   | Phụ thuộc hệ thống (ít nhất 32 bit)             | `long distance = 100000L;`             |
| `long long`      | Số nguyên rất lớn                               | ±9×10¹⁸ (64 bit)                                | `long long population = 8000000000LL;` |
| `float`          | Số thực, độ chính xác khoảng 6–7 chữ số         | ±3.4×10³⁸                                       | `float pi = 3.14f;`                    |
| `double`         | Số thực, chính xác hơn `float`                  | ±1.7×10³⁰⁸                                      | `double price = 19.99;`                |
| `long double`    | Số thực có độ chính xác cao hơn `double`        | Phụ thuộc hệ thống                              | `long double x = 3.1415926535L;`       |
| `_Bool` (`bool`) | Giá trị đúng/sai                                | `0` (false), `1` (true)                         | `bool flag = true;`                    |
| `void`           | Không có kiểu dữ liệu hoặc không trả về giá trị | Không áp dụng                                   | `void print(void);`                    |

**Ví dụ**
```c
#include <stdbool.h>

char c = 'A';
short year = 2025;
int age = 20;
long distance = 100000L;
long long money = 10000000000LL;

float pi = 3.14f;
double salary = 12345.67;
long double x = 3.141592653589793L;

bool flag = true;
```

>**Trong lập trình Embedded**, các kiểu được dùng nhiều nhất là `char`, `int`, `float` (ít hơn do tốn tài nguyên), `bool` và đặc biệt là các kiểu trong `stdint.h` (`uint8_t`, `uint16_t`, `uint32_t`...), sẽ học ở mục **1.5**.

## 1.2 Kích thước kiểu dữ liệu (`sizeof`)

Toán tử `sizeof` dùng để xác định kích thước (số byte) của một kiểu dữ liệu hoặc biến tại **thời điểm biên dịch**.

### Cú pháp

```c
sizeof(kiểu_dữ_liệu)
sizeof(biến)
```

> **Lưu ý:** Kích thước của các kiểu dữ liệu phụ thuộc vào **compiler** và **kiến trúc CPU**, vì vậy **không nên giả định `int` luôn có kích thước 4 byte**.

### Ví dụ

```c
#include <stdio.h>

int main() {
    printf("%zu\n", sizeof(char));
    printf("%zu\n", sizeof(short));
    printf("%zu\n", sizeof(int));
    printf("%zu\n", sizeof(long));
    printf("%zu\n", sizeof(double));

    return 0;
}
```

### Kích thước thường gặp

#### Trên Ubuntu 64-bit

| Kiểu dữ liệu | Kích thước |
|--------------|-----------:|
| `char` | 1 byte |
| `short` | 2 byte |
| `int` | 4 byte |
| `long` | 8 byte |
| `long long` | 8 byte |

#### Trên STM32 (ARM Cortex-M)

| Kiểu dữ liệu | Kích thước |
|--------------|-----------:|
| `char` | 1 byte |
| `short` | 2 byte |
| `int` | 4 byte |
| `long` | 4 byte |
| `long long` | 8 byte |

### Tại sao không nên giả định `int` luôn là 4 byte?

Mỗi **compiler** và **kiến trúc phần cứng** có thể quy định kích thước kiểu dữ liệu khác nhau để tối ưu hiệu năng và bộ nhớ. Vì vậy, viết chương trình dựa vào giả định kích thước của `int`, `long`,... có thể khiến chương trình hoạt động không đúng khi chuyển sang nền tảng khác.

Ví dụ:

- Trên Ubuntu 64-bit: `sizeof(long) = 8`
- Trên STM32: `sizeof(long) = 4`

Nếu chương trình giả định `long` luôn là **8 byte** thì khi chạy trên STM32 có thể xảy ra lỗi.

### Ghi nhớ

- `sizeof` dùng để lấy kích thước (byte) của kiểu dữ liệu hoặc biến.
- Kích thước các kiểu dữ liệu **không hoàn toàn giống nhau** trên mọi hệ thống.
- Không nên giả định `int`, `long`,... luôn có cùng kích thước.
- Đây là lý do trong lập trình **Embedded** thường sử dụng các kiểu dữ liệu có kích thước cố định trong `<stdint.h>` như `uint8_t`, `int16_t`, `uint32_t`,... để chương trình hoạt động nhất quán trên các vi điều khiển khác nhau.

## 1.3 Kiểu có dấu và không dấu (`signed` / `unsigned`)

Các kiểu số nguyên trong C có thể là **có dấu** (`signed`) hoặc **không dấu** (`unsigned`).

- **`signed`**: Lưu được cả số âm và số dương.
- **`unsigned`**: Chỉ lưu số không âm (từ `0` trở lên), đổi lại sẽ lưu được giá trị dương lớn hơn.

> **Lưu ý:** Nếu không ghi rõ, `int` mặc định là `signed int`.

### Phạm vi giá trị (với `int` 32 bit)

| Kiểu dữ liệu | Phạm vi giá trị |
|--------------|-----------------|
| `signed int` | `-2³¹` → `2³¹ - 1` |
| `unsigned int` | `0` → `2³² - 1` |

### Ví dụ

```c
signed int a = -10;
unsigned int b = 10;
unsigned char c = 255;
```

### Giải thích

- `signed int a = -10;` → Có thể lưu số âm.
- `unsigned int b = 10;` → Chỉ lưu các giá trị từ `0` trở lên.
- `unsigned char c = 255;` → Giá trị lớn nhất của `unsigned char` (8 bit).

### Khi nào dùng `unsigned`?

`unsigned` thường được dùng khi giá trị **không bao giờ âm**, chẳng hạn:

- Kích thước bộ nhớ.
- Số lượng phần tử.
- Giá trị của thanh ghi phần cứng.
- Giá trị đọc từ các chân GPIO.
- Các biến đếm chỉ tăng.

### Ghi nhớ

- `signed` → Có thể lưu cả số âm và số dương.
- `unsigned` → Không lưu được số âm, nhưng có phạm vi số dương lớn hơn.
- Nên chọn `unsigned` khi biết chắc giá trị không bao giờ âm để tận dụng tối đa phạm vi lưu trữ.
- Việc chọn đúng giữa `signed` và `unsigned` giúp tránh lỗi khi tính toán và làm chương trình rõ ràng hơn.

## 1.4 Integer Promotion (Nâng cấp kiểu số nguyên)

Trong C, khi thực hiện các phép toán như `+`, `-`, `*`, `/`,..., các kiểu số nguyên có kích thước **nhỏ hơn `int`** như `char`, `short`, `uint8_t`, `int8_t`... sẽ được **tự động chuyển thành `int`** (hoặc `unsigned int` nếu `int` không thể biểu diễn hết giá trị).

Quá trình này được gọi là **Integer Promotion**.

### Ví dụ 1

```c
#include <stdio.h>
#include <stdint.h>

int main() {
    uint8_t a = 200;
    uint8_t b = 100;

    printf("%d\n", a + b);

    return 0;
}
```

Mặc dù `a` và `b` có kiểu `uint8_t`, nhưng trước khi thực hiện phép cộng, chúng được chuyển thành `int`:

```text
a + b
↓
(int)a + (int)b
↓
300
```

Kết quả in ra:

```text
300
```

### Ví dụ 2

```c
#include <stdio.h>

int main() {
    char a = 120;
    char b = 120;

    printf("%d\n", a + b);

    return 0;
}
```

Kết quả:

```text
240
```

Mặc dù `char` thường chỉ có 8 bit, nhưng trước khi cộng, `a` và `b` đã được chuyển thành `int`. Vì vậy phép tính được thực hiện trên kiểu `int` và **không bị tràn trong quá trình cộng**.

> **Lưu ý:** Nếu sau đó gán kết quả `240` trở lại cho một biến `char`, lúc này mới có thể xảy ra hiện tượng tràn (overflow) vì `char` không đủ phạm vi để lưu giá trị đó.

### Ghi nhớ

- Các kiểu như `char`, `short`, `uint8_t`, `int8_t`... **không được tính toán trực tiếp**.
- Trước khi thực hiện phép toán, chúng sẽ được **tự động chuyển thành `int`** (hoặc `unsigned int` trong một số trường hợp).
- Hiểu rõ **Integer Promotion** rất quan trọng khi làm việc với phép toán bit (`&`, `|`, `^`, `<<`, `>>`) và lập trình **Embedded**, vì nếu không nắm quy tắc này, chương trình có thể cho kết quả ngoài mong đợi.

## 1.5 Kiểu số nguyên có kích thước cố định (`<stdint.h>`)

Thư viện **`<stdint.h>`** cung cấp các kiểu số nguyên có **kích thước cố định**, giúp chương trình hoạt động nhất quán trên mọi nền tảng.

### Các kiểu dữ liệu thường dùng

| Kiểu dữ liệu | Kích thước | Phạm vi giá trị |
|--------------|-----------:|-----------------|
| `int8_t` | 8 bit | `-128` → `127` |
| `uint8_t` | 8 bit | `0` → `255` |
| `int16_t` | 16 bit | `-32,768` → `32,767` |
| `uint16_t` | 16 bit | `0` → `65,535` |
| `int32_t` | 32 bit | `-2³¹` → `2³¹ - 1` |
| `uint32_t` | 32 bit | `0` → `2³² - 1` |
| `int64_t` | 64 bit | `-2⁶³` → `2⁶³ - 1` |
| `uint64_t` | 64 bit | `0` → `2⁶⁴ - 1` |

### Ví dụ

```c
#include <stdint.h>

uint8_t led = 1;
int16_t temperature = -20;
uint32_t counter = 1000;
```

### Giải thích

- `uint8_t` luôn có kích thước **8 bit (1 byte)**.
- `uint16_t` luôn có kích thước **16 bit**.
- `uint32_t` luôn có kích thước **32 bit**.
- Kích thước của các kiểu này **không phụ thuộc** vào compiler hay kiến trúc CPU.

Ví dụ:

- `uint8_t` luôn có kích thước **8 bit**, dù chương trình chạy trên **Ubuntu**, **STM32** hay **ESP32**.

### Tại sao nên dùng `<stdint.h>`?

Các kiểu dữ liệu như `int`, `long` có thể có kích thước khác nhau trên từng nền tảng. Trong khi đó, các kiểu trong `<stdint.h>` luôn có kích thước cố định, giúp chương trình:

- Hoạt động nhất quán trên nhiều hệ thống.
- Dễ đọc và dễ bảo trì.
- Tránh lỗi do khác biệt về kích thước kiểu dữ liệu.

### Ghi nhớ

- Dùng các kiểu trong **`<stdint.h>`** khi cần biết **chính xác kích thước dữ liệu**.
- Đây là các kiểu dữ liệu được sử dụng rất nhiều trong **Embedded**, đặc biệt khi làm việc với thanh ghi, giao tiếp ngoại vi và các giao thức truyền dữ liệu.

## 1.6 Ép kiểu (Type Casting)

Ép kiểu (**Type Casting**) là quá trình chuyển dữ liệu từ kiểu này sang kiểu khác.

Có hai loại ép kiểu:

- **Ép kiểu ngầm (Implicit Casting):** Trình biên dịch tự động chuyển kiểu khi cần.
- **Ép kiểu tường minh (Explicit Casting):** Lập trình viên chủ động chuyển kiểu bằng toán tử `(type)`.

### Ép kiểu ngầm (Implicit Casting)

```c
int a = 5;
float b = a;
```

Ở đây, `a` có kiểu `int` nhưng được tự động chuyển thành `float`, nên `b` có giá trị:

```text
5.0
```

### Ép kiểu tường minh (Explicit Casting)

```c
float a = 5.8;
int b = (int)a;
```

Kết quả:

```text
b = 5
```

Phần thập phân bị **loại bỏ** khi chuyển từ `float` sang `int` (không làm tròn).

### Ví dụ cần lưu ý

```c
int a = 5;
int b = 2;

float c = a / b;
```

Kết quả:

```text
c = 2.0
```

### Giải thích

Mặc dù `c` là `float`, nhưng:

- `a` và `b` đều có kiểu `int`.
- Phép chia `a / b` được thực hiện theo **chia số nguyên**.
- `5 / 2 = 2`, sau đó kết quả mới được chuyển sang `float`.

Do đó:

```text
c = 2.0
```

Muốn phép chia cho ra số thực, cần ép một trong hai toán hạng sang `float` trước khi chia:

```c
float c = (float)a / b;
```

Kết quả:

```text
c = 2.5
```

### Ghi nhớ

- **Ép kiểu ngầm:** Trình biên dịch tự động chuyển kiểu khi cần.
- **Ép kiểu tường minh:** Dùng `(type)` để chủ động chuyển kiểu.
- Khi chia **hai số nguyên**, kết quả luôn là **số nguyên**, kể cả khi gán vào biến `float`.
- Muốn phép chia cho ra **số thực**, hãy ép **ít nhất một toán hạng** sang `float` trước khi thực hiện phép chia.