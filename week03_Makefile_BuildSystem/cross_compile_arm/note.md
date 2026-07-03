# Native Compile và Cross Compile

## 1. Native Compile

Native compile là quá trình **biên dịch chương trình trên chính kiến trúc mà chương trình sẽ chạy**.

Nói đơn giản:

> **Máy nào biên dịch thì cũng chính máy đó chạy chương trình.**

### Ví dụ

Máy tính Ubuntu x86_64:

```bash
gcc hello.c -o hello
./hello
```

- Compiler chạy trên PC x86_64.
- File `hello` được tạo ra cũng dành cho x86_64.
- Chạy trực tiếp trên chính PC đó.

Sơ đồ:

```
PC (x86_64)
      │
      ├── gcc
      │
      ▼
hello (x86_64)
      │
      ▼
Chạy trên PC (x86_64)
```

---

## 2. Cross Compile

Cross compile là quá trình **biên dịch trên một kiến trúc nhưng tạo chương trình cho một kiến trúc khác**.

Nói đơn giản:

> **Biên dịch ở máy A nhưng chương trình chạy ở máy B.**

### Ví dụ

PC Ubuntu x86_64 dùng ARM GNU Toolchain:

```bash
arm-linux-gnueabihf-gcc hello.c -o hello
```

- Compiler chạy trên PC x86_64.
- File `hello` được tạo cho ARM.
- Không chạy được trên PC x86_64.
- Phải chép sang thiết bị ARM (ví dụ: BeagleBone Black, Raspberry Pi...) mới chạy được.

Sơ đồ:

```
PC (x86_64)
      │
      ├── arm-linux-gnueabihf-gcc
      │
      ▼
hello (ARM)
      │
      ▼
Copy sang Board ARM
      │
      ▼
Chạy trên Board ARM
```

---

# Vì sao cần Cross Compile?

Các board nhúng thường:

- CPU yếu hơn PC.
- RAM ít.
- Không cài sẵn compiler.
- Biên dịch trực tiếp rất chậm.

Do đó ta:

1. Viết code trên PC.
2. Cross compile trên PC.
3. Chép file thực thi sang board.
4. Chạy trên board.

Nhờ vậy việc phát triển phần mềm nhanh và thuận tiện hơn.

---

# So sánh Native Compile và Cross Compile

| Native Compile                               | Cross Compile                                                     |
| -------------------------------------------- | ----------------------------------------------------------------- |
| Biên dịch và chạy trên cùng kiến trúc        | Biên dịch và chạy trên hai kiến trúc khác nhau                    |
| Compiler tạo file cho chính máy đang dùng    | Compiler tạo file cho kiến trúc khác                              |
| Có thể chạy ngay sau khi build               | Phải chép sang thiết bị đích mới chạy được                        |
| Ví dụ: `gcc` trên PC tạo chương trình cho PC | Ví dụ: `arm-linux-gnueabihf-gcc` trên PC tạo chương trình cho ARM |

---

# Ví dụ thực tế trong Embedded Linux BSP

Giả sử:

- Host: Ubuntu PC (x86_64)
- Target: BeagleBone Black (ARM)

Có hai cách phát triển:

## Cách 1: Native Compile

```
Viết code
      │
      ▼
Copy source sang BBB
      │
      ▼
gcc hello.c
      │
      ▼
hello
      │
      ▼
Chạy trên BBB
```

Ưu điểm:

- Đơn giản.

Nhược điểm:

- Build chậm.
- Phải cài compiler trên board.

---

## Cách 2: Cross Compile (phổ biến)

```
Viết code trên PC
      │
      ▼
arm-linux-gnueabihf-gcc hello.c
      │
      ▼
hello (ARM)
      │
      ▼
Copy sang BBB
      │
      ▼
./hello
```

Ưu điểm:

- Build nhanh.
- Không cần compiler trên board.
- Là cách làm chuẩn trong phát triển Embedded Linux BSP.

---

# Tóm tắt

- **Native Compile**: Biên dịch và chạy trên cùng một kiến trúc.
- **Cross Compile**: Biên dịch trên host nhưng tạo chương trình cho target có kiến trúc khác.
- Trong Embedded Linux BSP, **cross compile là phương pháp được sử dụng phổ biến nhất**.
