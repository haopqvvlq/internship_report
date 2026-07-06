# 0. Valgrind là gì?

Trong C/C++, lập trình viên phải tự quản lý bộ nhớ (`malloc()`, `free()`, ...), nên rất dễ gặp các lỗi như:

- Memory Leak
- Use After Free
- Đọc vùng nhớ chưa khởi tạo
- Buffer Overflow
- Double Free

Các lỗi này thường khó phát hiện vì chương trình vẫn có thể chạy bình thường hoặc chỉ crash ngẫu nhiên.

**Valgrind** là công cụ theo dõi chương trình khi chạy để phát hiện các lỗi quản lý bộ nhớ và chỉ ra vị trí gây lỗi.

> Valgrind không sửa lỗi, mà đóng vai trò như một **"camera giám sát bộ nhớ"**, ghi lại các thao tác bất thường để lập trình viên tìm nguyên nhân.

# 1. Cách Valgrind hoạt động (Dynamic Binary Instrumentation - DBI)

Thông thường:

```
Program → CPU → Memory
```

Khi dùng Valgrind:

```
Program → Valgrind → CPU → Memory
```

Valgrind đứng giữa chương trình và CPU. Nó phân tích từng lệnh máy, chèn thêm các bước kiểm tra (instrumentation), rồi mới cho CPU thực thi.

Nhờ đó, Valgrind có thể phát hiện các lỗi về bộ nhớ trong khi chương trình đang chạy.

> "CPU ảo" chỉ là cách hình dung. Valgrind không giả lập CPU mà tạo một môi trường thực thi trung gian.

**Đánh đổi:** Do phải kiểm tra từng lệnh, chương trình thường chạy chậm hơn khoảng **10–30 lần**.

# 2. Memcheck kiểm tra những gì?

## Memcheck là gì?

Valgrind là một framework gồm nhiều tool, mỗi tool phục vụ một mục đích khác nhau.

| Tool           | Chức năng                                             |
| -------------- | ----------------------------------------------------- |
| **Memcheck**   | Phát hiện lỗi liên quan đến bộ nhớ (phổ biến nhất)    |
| **Callgrind**  | Phân tích hiệu năng (profiling)                       |
| **Cachegrind** | Mô phỏng cache CPU và thống kê số lệnh thực thi       |
| **Helgrind**   | Phát hiện lỗi đồng bộ giữa các luồng                  |
| **DRD**        | Phát hiện _data race_ và các lỗi liên quan đến thread |

Trong đó, **Memcheck** là tool được sử dụng nhiều nhất. Nó theo dõi toàn bộ quá trình quản lý bộ nhớ của chương trình, bao gồm:

- Cấp phát bộ nhớ.
- Truy cập bộ nhớ (đọc/ghi).
- Giải phóng bộ nhớ.

## Memcheck phát hiện những lỗi nào?

Nhờ theo dõi toàn bộ vòng đời của bộ nhớ, Memcheck có thể phát hiện các lỗi như:

- **Memory Leak**
- **Invalid Read**
- **Invalid Write**
- **Use of Uninitialized Value**
- **Invalid Free / Double Free**
- **Mismatched Allocation** (`malloc/free`, `new/delete`)

## Thời điểm phát hiện lỗi

- Các lỗi truy cập bộ nhớ như **Invalid Read**, **Invalid Write**, **Invalid Free**, ... được phát hiện **ngay tại thời điểm chúng xảy ra**.
- Riêng **Memory Leak** được Memcheck tổng hợp và báo cáo **khi chương trình kết thúc**, sau khi kiểm tra toàn bộ bộ nhớ đã được cấp phát.

# 3. Valgrind kiểm tra khi nào?

## Trong lúc chương trình chạy (Runtime)

Memcheck theo dõi bộ nhớ trong suốt quá trình thực thi. Mỗi khi chương trình đọc, ghi, cấp phát hoặc giải phóng bộ nhớ, Memcheck sẽ kiểm tra ngay thao tác đó có hợp lệ hay không.

Các lỗi như:

- **Invalid Read**
- **Invalid Write**
- **Use After Free**
- **Invalid Free**
- **Use of Uninitialized Value**

được phát hiện ngay khi xảy ra. Đồng thời, Memcheck ghi lại **stack trace** để hỗ trợ quá trình debug.

## Khi chương trình kết thúc (Program Exit)

Sau khi chương trình kết thúc, Memcheck kiểm tra các vùng nhớ đã được cấp phát nhưng chưa được giải phóng và báo cáo **Memory Leak**, kèm theo thống kê chi tiết từng loại leak.

## Tóm tắt

| Thời điểm                 | Memcheck kiểm tra                                                      |
| ------------------------- | ---------------------------------------------------------------------- |
| Trong lúc chạy            | Phát hiện các thao tác truy cập bộ nhớ không hợp lệ và báo lỗi ngay.   |
| Khi chương trình kết thúc | Kiểm tra các vùng nhớ chưa được giải phóng và báo cáo **Memory Leak**. |

## Ý tưởng cốt lõi

Memcheck theo dõi bộ nhớ trong suốt vòng đời của chương trình.

- Các lỗi **truy cập bộ nhớ** được phát hiện ngay khi xảy ra.
- **Memory Leak** chỉ có thể kết luận sau khi chương trình kết thúc.

# 4. Quy trình sử dụng Valgrind

Quy trình sử dụng Memcheck thường gồm 4 bước.

## Bước 1. Build chương trình

Biên dịch với tùy chọn `-g` để Valgrind hiển thị tên file, hàm và số dòng khi báo lỗi.

**Build**

```bash
gcc -g main.c -o app
```

## Bước 2. Chạy bằng Valgrind

Thay vì chạy trực tiếp:

**Chạy trực tiếp**

```bash
./app
```

hãy chạy thông qua Valgrind (tool mặc định là Memcheck):

**Valgrind Memcheck**

```bash
valgrind --leak-check=full --show-leak-kinds=all ./app
```

Memcheck sẽ theo dõi toàn bộ quá trình thực thi của chương trình.

## Bước 3. Đọc báo cáo

Sau khi chương trình kết thúc, Valgrind sẽ hiển thị:

- Các lỗi truy cập bộ nhớ.
- Báo cáo **Memory Leak**.
- **Stack trace** chỉ ra vị trí xảy ra lỗi.

## Bước 4. Sửa lỗi và kiểm tra lại

Sửa các lỗi được báo cáo, sau đó chạy lại Valgrind để xác nhận lỗi đã được khắc phục.

Quá trình này được lặp lại cho đến khi chương trình không còn lỗi bộ nhớ.

## Tóm tắt

**Chu trình làm việc với Valgrind**

```text
Build (-g)
      ↓
Chạy bằng Valgrind
      ↓
Memcheck
      ↓
Đọc báo cáo
      ↓
Sửa lỗi
      ↓
Chạy lại Valgrind
      ↓
Không còn lỗi
```

## Điểm cần nhớ

- Tùy chọn `-g` giúp báo cáo dễ đọc bằng cách hiển thị tên file, hàm và số dòng.
- Khi debug **Memory Leak**, nên sử dụng mặc định cặp option:
  - `--leak-check=full`
  - `--show-leak-kinds=all`

# 5. Cách đọc báo cáo

Khi phân tích kết quả của Memcheck, thường cần quan tâm đến 3 phần sau.

## 1. Error Message

Đây là phần cho biết **loại lỗi** mà Memcheck phát hiện.

Ví dụ:

```text
Invalid Read of size 4
Invalid Write of size 1
Use of uninitialised value
Invalid free()
```

Đây là thông tin đầu tiên cần đọc để biết chương trình đang gặp lỗi gì.

## 2. Stack Trace

Ngay sau thông báo lỗi là **stack trace**, cho biết đường đi của lời gọi hàm dẫn đến vị trí xảy ra lỗi.

Ví dụ:

```text
==1234== Invalid Read of size 4
==1234==    at 0x401234: foo() (main.c:15)
==1234==    by 0x4012A0: bar() (main.c:30)
==1234==    by 0x401310: main() (main.c:45)
```

Thông tin quan trọng nhất là:

- Tên hàm.
- Tên file.
- Số dòng.

Đây là nơi nên bắt đầu kiểm tra mã nguồn.

## 3. Leak Summary

Khi chương trình kết thúc, Memcheck sẽ tổng hợp tình trạng bộ nhớ.

Ví dụ:

```text
LEAK SUMMARY:
    definitely lost: ...
    indirectly lost: ...
    possibly lost: ...
    still reachable: ...
```

Phần này cho biết chương trình có bị **Memory Leak** hay không và lượng bộ nhớ bị ảnh hưởng.

Chi tiết từng loại leak sẽ được trình bày ở phần sau.

## Thứ tự nên đọc báo cáo

Khi debug với Valgrind, nên đọc theo thứ tự:

1. **Error Message** – Xác định loại lỗi.
2. **Stack Trace** – Xác định vị trí trong mã nguồn.
3. **Leak Summary** – Kiểm tra tình trạng **Memory Leak**.

Không nên chỉ xem **Leak Summary**, vì nhiều lỗi truy cập bộ nhớ xảy ra trước đó có thể là nguyên nhân dẫn đến các báo cáo leak.

## Ý tưởng cốt lõi

Khi đọc báo cáo của Valgrind:

1. Xác định **loại lỗi** trước.
2. Tìm **vị trí lỗi** qua **Stack Trace**.
3. Cuối cùng mới đánh giá **Memory Leak** trong **Leak Summary**.

> **Lưu ý:** Trong thực tế, hãy ưu tiên sửa các lỗi **Invalid Read/Write**, **Invalid Free** và **Use of Uninitialised Value** trước. Sau khi các lỗi này được khắc phục, mới xử lý **Memory Leak**, vì nhiều báo cáo leak chỉ là hậu quả của các lỗi truy cập bộ nhớ xảy ra trước đó.

# 6. Các loại Memory Leak

Khi chương trình kết thúc, Memcheck sẽ phân tích các vùng nhớ đã cấp phát và phân loại chúng trong **LEAK SUMMARY**.

## 1. Definitely Lost

Vùng nhớ đã mất hoàn toàn con trỏ tham chiếu, chương trình không còn cách nào truy cập hoặc giải phóng.

Đây là **Memory Leak thực sự** và cần được sửa.

Ví dụ:

```c
int *p = malloc(100);
p = NULL;      // mất địa chỉ vùng nhớ
```

---

## 2. Indirectly Lost

Vùng nhớ vẫn còn được tham chiếu, nhưng chỉ thông qua một vùng nhớ đã bị **Definitely Lost**.

Thường gặp ở các cấu trúc dữ liệu như linked list hoặc tree.

Ví dụ:

```text
Head (lost)
   │
   ▼
Node A ──► Node B
```

Khi mất `Head`, các node phía sau cũng không còn cách truy cập.

---

## 3. Possibly Lost

Memcheck không thể khẳng định chắc chắn vùng nhớ có bị leak hay không.

Nguyên nhân thường là chương trình chỉ còn giữ **con trỏ vào giữa vùng nhớ** (_interior pointer_), thay vì con trỏ gốc.

Cần kiểm tra lại mã nguồn để xác định đây có phải là leak thực sự hay không.

---

## 4. Still Reachable

Vùng nhớ vẫn còn tham chiếu khi chương trình kết thúc, nên về lý thuyết vẫn có thể được giải phóng.

Điều này **không nhất thiết là lỗi**.

Thường gặp ở:

- Bộ nhớ được thư viện giữ lại đến khi chương trình kết thúc.
- Biến toàn cục hoặc singleton.
- Cache nội bộ.

---

## Mức độ ưu tiên

| Loại                | Mức độ                                              |
| ------------------- | --------------------------------------------------- |
| **Definitely Lost** | 🔴 Phải sửa                                         |
| **Indirectly Lost** | 🔴 Phải sửa (thường tự hết khi sửa Definitely Lost) |
| **Possibly Lost**   | 🟡 Kiểm tra thêm                                    |
| **Still Reachable** | 🟢 Thường có thể chấp nhận                          |

---

## Ý tưởng cốt lõi

> Không phải mọi **Memory Leak** đều nghiêm trọng như nhau. Trong thực tế, hãy **ưu tiên sửa `Definitely Lost` trước**, vì nhiều trường hợp **`Indirectly Lost` sẽ tự biến mất** sau đó. Tiếp theo kiểm tra `Possibly Lost`, còn `Still Reachable` thường có thể chấp nhận nếu đó là hành vi mong muốn của chương trình.

# 7. Hạn chế của Valgrind

Mặc dù Memcheck là công cụ rất hiệu quả để phát hiện lỗi bộ nhớ, nhưng nó vẫn có một số hạn chế.

## 1. Chạy chậm

Do phải phân tích và chèn các lệnh kiểm tra trong quá trình thực thi, chương trình thường chạy chậm hơn khoảng **10–30 lần**.

Vì vậy, Valgrind chủ yếu được dùng trong **debug và kiểm thử**, không dùng trong môi trường production.

---

## 2. Chỉ kiểm tra mã đã được thực thi

Valgrind chỉ phát hiện lỗi trên những đoạn code **đã chạy**.

Nếu một nhánh chương trình không được thực thi, Valgrind sẽ không thể phát hiện lỗi trong nhánh đó.

---

## 3. Chỉ phát hiện lỗi bộ nhớ

Memcheck chỉ tập trung vào **các lỗi quản lý bộ nhớ**.

Nó không phát hiện các lỗi như:

- Sai thuật toán hoặc logic chương trình.
- Lỗi nghiệp vụ (business logic).
- Hiệu năng của thuật toán.

---

## Ý tưởng cốt lõi

> Valgrind là công cụ **phát hiện lỗi bộ nhớ**, không phải công cụ **kiểm chứng tính đúng đắn của chương trình**. Vì vậy, hãy kết hợp Valgrind với test case, code review và các công cụ debug khác.
