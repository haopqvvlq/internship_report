# Implement Stack Using C++ Template

## Giới thiệu

Đây là bài tập xây dựng cấu trúc dữ liệu **Stack (LIFO - Last In, First Out)** bằng **Linked List** và **C++ Template**.

Thay vì chỉ hỗ trợ một kiểu dữ liệu cụ thể, chương trình sử dụng **template** để xây dựng một stack tổng quát, có thể lưu trữ bất kỳ kiểu dữ liệu nào như `int`, `float`, `char`, `std::string`,...

---

## Kiến thức sử dụng

- C++
- Template
- Linked List
- Dynamic Memory Allocation (`new`, `delete`)
- Generic Programming
- Stack (LIFO)

---

## Cấu trúc project

```
.
├── main.cpp
├── Makefile
└── README.md
```

---

## Cách build

Sử dụng Makefile:

```bash
make
```

Hoặc biên dịch thủ công:

```bash
g++ -Wall -Wextra main.cpp -o main
```

---

## Cách chạy

```bash
./main
```

---

## Kết quả mong đợi

```
30 20 10
20 10
```

---

## Cấu trúc dữ liệu

```
        Top
         │
         ▼
+------+     +------+     +------+
| 30   | --> | 20   | --> | 10   | --> nullptr
+------+     +------+     +------+
```

Sau khi gọi `pop()`:

```
        Top
         │
         ▼
+------+     +------+
| 20   | --> | 10   | --> nullptr
+------+     +------+
```

---

## Luồng hoạt động

```
main()
   │
   ▼
push(10)
   │
   ▼
push(20)
   │
   ▼
push(30)
   │
   ▼
display()
   │
   ▼
pop()
   │
   ▼
display()
```

---

## Giải thích chương trình

### 1. Node Template

```cpp
template <typename T>
struct Node
{
    T data;
    Node *next;
};
```

Mỗi node lưu:

- Giá trị (`data`)
- Con trỏ tới node tiếp theo (`next`)

Nhờ sử dụng template nên `data` có thể là bất kỳ kiểu dữ liệu nào.

---

### 2. Push

```cpp
push(head, value);
```

- Cấp phát một node mới.
- Gán dữ liệu.
- Cho node mới trỏ đến node đầu hiện tại.
- Cập nhật `head`.

Độ phức tạp:

```
O(1)
```

---

### 3. Pop

```cpp
pop(head);
```

- Kiểm tra stack có rỗng hay không.
- Xóa node đầu.
- Cập nhật `head`.

Độ phức tạp:

```
O(1)
```

---

### 4. Display

```cpp
display(head);
```

Duyệt toàn bộ Linked List từ `head` đến cuối danh sách và in ra các phần tử trong stack.

Độ phức tạp:

```
O(n)
```

---

## Kiến thức rút ra

- Cách xây dựng Stack bằng Linked List.
- Áp dụng **template** để tạo cấu trúc dữ liệu tổng quát.
- Quản lý bộ nhớ động với `new` và `delete`.
- Hiểu cách hoạt động của Stack theo nguyên lý **LIFO (Last In, First Out)**.
- Phân biệt ưu điểm của Stack cài đặt bằng Linked List so với mảng (không cần kích thước cố định).

---

## Hướng phát triển

Có thể mở rộng chương trình bằng cách:

- Thêm hàm `peek()` để xem phần tử trên cùng.
- Thêm hàm `isEmpty()`.
- Thêm hàm `size()`.
- Đóng gói Stack thành một class template.
- Bổ sung destructor để tự động giải phóng bộ nhớ.
- So sánh với cách cài đặt bằng `std::stack`.

---

## Tài liệu tham khảo

- C++ Primer (5th Edition)
- The C++ Programming Language – Bjarne Stroustrup
- cppreference.com (Template và Dynamic Memory)
