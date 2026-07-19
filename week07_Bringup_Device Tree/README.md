# Bài thực hành Linux BSP: Device Tree → Platform Driver → GPIO Subsystem → User Space

---

# 🎯 Mục tiêu

Tự mô tả một thiết bị LED trong **Device Tree**, để kernel tạo **Platform Device**, sau đó viết **Platform Driver** bind với thiết bị.

Khi `probe()` được gọi, driver sẽ:

1. Đọc GPIO từ Device Tree
2. Sử dụng GPIO Subsystem để điều khiển LED
3. Tạo giao diện `/dev/myled`
4. Nhận lệnh từ User Space để bật/tắt LED

## Pipeline hoàn chỉnh

```text
Device Tree
     │
     ▼
Platform Device
     │
     │  compatible matching
     ▼
Platform Driver
     │
     ▼
probe()
     │
     ├──────────────► GPIO Subsystem
     │                      │
     │                      ▼
     │                GPIO Controller
     │                      │
     │                      ▼
     │                     LED
     │
     └──────────────► Character Device
                            │
                            ▼
                        /dev/myled
                            │
                            ▼
                        User Space
```

> **Lưu ý:** `Platform Driver`, `GPIO Subsystem` và `Character Device` là các thành phần khác nhau trong Kernel. Chúng phối hợp với nhau để hoàn thành bài thực hành.

---

# Giai đoạn 0 - Hiểu kiến trúc tổng thể

Trước khi viết code, cần hiểu **vai trò của từng thành phần**.

```text
                    Device Tree
                         │
                         ▼
                 Device Tree Node
                         │
                         ▼
                  Platform Device
                         │
                         │ compatible matching
                         ▼
                  Platform Driver
                         │
                       probe()
                         │
          ┌──────────────┴──────────────┐
          ▼                             ▼
    GPIO Subsystem                Character Device
          │                             │
          ▼                             ▼
    GPIO Controller                  /dev/myled
          │                             │
          ▼                             ▼
          LED                      User Space
```

## Cần phân biệt

| Thành phần           | Vai trò                            |
| -------------------- | ---------------------------------- |
| **Device Tree**      | Mô tả phần cứng                    |
| **Platform Device**  | Đại diện cho thiết bị trong Kernel |
| **Platform Driver**  | Driver điều khiển thiết bị         |
| **GPIO Subsystem**   | Abstraction để làm việc với GPIO   |
| **Character Device** | Giao diện cho User Space           |
| **`/dev/myled`**     | User-space device node             |

---

# Giai đoạn 1 - Khảo sát phần cứng

## Mục tiêu

Hiểu phần cứng trước khi viết Device Tree.

## Cần xác định

* LED nào được sử dụng?
* LED nối với GPIO nào?
* GPIO thuộc GPIO Controller nào?
* GPIO là `ACTIVE_HIGH` hay `ACTIVE_LOW`?
* LED có hoạt động theo logic đảo không?

Ví dụ:

```text
LED
 │
 └── GPIO1_21
```

## Kết quả cần đạt

Bạn phải trả lời được:

> **Nếu muốn bật LED, phần cứng cần tác động vào GPIO nào và theo logic nào?**

---

# Giai đoạn 2 - Mô tả thiết bị trong Device Tree

## Mục tiêu

Khai báo phần cứng cho Kernel.

Ví dụ:

```dts
my_led {
    compatible = "mycompany,my-led";
    gpios = <&gpio1 21 GPIO_ACTIVE_HIGH>;
};
```

## Cần hiểu từng property

### `compatible`

```dts
compatible = "mycompany,my-led";
```

Dùng để **matching Platform Device với Platform Driver**.

### `gpios`

```dts
gpios = <&gpio1 21 GPIO_ACTIVE_HIGH>;
```

Mô tả:

* GPIO Controller
* GPIO pin
* Active level

> Driver không tự biết GPIO là `GPIO1_21`. Driver sẽ đọc thông tin này từ Device Tree.

## Kết quả cần đạt

Device Tree đã mô tả được thiết bị `my_led`.

---

# Giai đoạn 3 - Build và Boot Device Tree

## Mục tiêu

Xác nhận Device Tree mới thực sự được Kernel sử dụng.

## Kiểm tra

### 1. Kiểm tra Device Tree

```bash
ls /proc/device-tree/
```

### 2. Kiểm tra node `my_led`

```bash
ls /proc/device-tree/my_led/
```

### 3. Kiểm tra `compatible`

```bash
cat /proc/device-tree/my_led/compatible
```

### 4. Kiểm tra DTB đang được sử dụng

Không được chỉ assume rằng:

> "Mình build xong thì board đã boot DTB mới."

## Kết quả cần đạt

Bạn phải chứng minh được:

> **Kernel đang chạy với Device Tree chứa node `my_led`.**

---

# Giai đoạn 4 - Tìm hiểu Platform Device

## Mục tiêu

Hiểu cách Device Tree node trở thành Kernel Device.

## Cơ chế tổng quát

```text
Device Tree Node
       │
       ▼
Device Tree Population
       │
       ▼
Platform Device
```

> Không nên hiểu đơn giản rằng "cứ có một Device Tree node là kernel tự động tạo Platform Device". Device Tree infrastructure sẽ xử lý và populate các node phù hợp thành device tương ứng.

## Bài tập

Tìm Platform Device tương ứng với node `my_led`.

Có thể kiểm tra:

```bash
ls /sys/bus/platform/devices/
```

## Câu hỏi cần trả lời

> Node `my_led` trong Device Tree liên quan thế nào đến Platform Device trong `/sys/bus/platform/devices/`?

## Kết quả cần đạt

Hiểu được:

> **Device Tree mô tả phần cứng, còn Platform Device là đối tượng thiết bị được Kernel quản lý.**

---

# Giai đoạn 5 - Viết Platform Driver

## Mục tiêu

Tạo driver có thể bind với Platform Device.

## Công việc

1. Khai báo `struct platform_driver`
2. Khai báo `of_match_table`
3. Viết `probe()`
4. Viết `remove()`
5. Đăng ký Platform Driver

## Kiến trúc

```text
Platform Driver
      │
      ├── probe()
      ├── remove()
      └── of_match_table
```

## Kết quả cần đạt

Driver đã sẵn sàng tham gia vào cơ chế **Platform Bus / Driver Model**.

---

# Giai đoạn 6 - Hiểu cơ chế Driver Matching

## Mục tiêu

Hiểu **vì sao `probe()` được gọi**.

```text
Device Tree
     │
     ▼
Platform Device
     │
     │ compatible
     ▼
of_match_table
     │
     ▼
Match
     │
     ▼
probe()
```

Ví dụ:

```text
"mycompany,my-led"
        │
        │ match
        ▼
"mycompany,my-led"
```

## Câu hỏi quan trọng

> Nếu `compatible` không khớp thì chuyện gì xảy ra?

## Kết quả cần đạt

Có thể giải thích rõ:

> `probe()` không tự nhiên được gọi. Nó được gọi sau khi Device Model tìm thấy một Device và Driver phù hợp thông qua cơ chế matching.

---

# Giai đoạn 7 - Đọc Device Tree trong Driver

## Mục tiêu

Driver không hard-code thông tin phần cứng.

```text
Device Tree
     │
     ▼
GPIO Property
     │
     ▼
Platform Driver
```

## Driver cần đọc

* GPIO
* Active level
* Các property khác nếu có

## API GPIO nên học

```text
Device Tree
     │
     ▼
GPIO Descriptor
     │
     ▼
gpiod API
```

Ví dụ:

```c
devm_gpiod_get()
```

và:

```c
gpiod_set_value()
```

> **Ưu tiên học GPIO Descriptor API (`gpiod_*`) thay vì bắt đầu bằng Legacy GPIO Number API.**

## Kết quả cần đạt

Driver có thể lấy thông tin phần cứng từ Device Tree mà không cần hard-code.

---

# Giai đoạn 8 - Điều khiển GPIO

## Mục tiêu

Làm việc với GPIO Subsystem.

## Pipeline

```text
Platform Driver
       │
       ▼
GPIO Subsystem
       │
       ▼
GPIO Controller Driver
       │
       ▼
Hardware GPIO
```

## Công việc

* Lấy GPIO từ Device Tree
* Cấu hình GPIO output
* Set GPIO
* Clear GPIO

## Điều cần hiểu

Driver **không trực tiếp thao tác register của GPIO Controller**.

```text
Driver
   │
   ▼
GPIO Subsystem
   │
   ▼
GPIO Controller Driver
   │
   ▼
Hardware Register
```

## Kết quả cần đạt

Driver điều khiển được LED thông qua GPIO Subsystem.

---

# Giai đoạn 9 - Tạo Character Device

## Mục tiêu

Tạo giao diện cho User Space.

```text
Character Driver
       │
       ▼
   /dev/myled
```

## Công việc

* Đăng ký Character Device
* Tạo Device Number
* Tạo Class
* Tạo Device
* Cài đặt `file_operations`

## Ví dụ giao diện

```text
/dev/myled
     │
     ├── open()
     ├── write()
     └── release()
```

## Cần hiểu

> **Character Device không phải là Platform Driver.**

Trong bài này:

```text
Platform Driver
        │
        ├── GPIO Subsystem
        │
        └── Character Device
```

## Kết quả cần đạt

Driver có giao diện `/dev/myled` để User Space sử dụng.

---

# Giai đoạn 10 - Kết nối User Space với Driver

## Mục tiêu

Hiểu luồng User Space → Kernel Space.

## Pipeline

```text
echo 1 > /dev/myled
          │
          ▼
       write()
          │
          ▼
  Character Driver
          │
          ▼
   gpiod_set_value()
          │
          ▼
    GPIO Subsystem
          │
          ▼
          LED
```

## Kiểm thử

```bash
echo 1 > /dev/myled
```

```bash
echo 0 > /dev/myled
```

## Luồng đầy đủ

```text
User Space
    │
    ▼
write()
    │
    ▼
file_operations
    │
    ▼
Character Driver
    │
    ▼
GPIO Subsystem
    │
    ▼
GPIO Controller
    │
    ▼
LED
```

## Kết quả cần đạt

Toàn bộ pipeline User Space → Kernel → Hardware hoạt động thông suốt.

---

# Giai đoạn 11 - Kiểm thử và Debug

## Mục tiêu

Hiểu cách debug một Linux Driver.

## Kiểm tra Device Tree

```bash
ls /proc/device-tree/
```

## Kiểm tra Platform Device

```bash
ls /sys/bus/platform/devices/
```

## Kiểm tra Platform Driver

```bash
ls /sys/bus/platform/drivers/
```

## Kiểm tra Kernel Log

```bash
dmesg
```

Hoặc:

```bash
dmesg | tail
```

## Kiểm tra User Space Interface

```bash
ls -l /dev/myled
```

## Các trường hợp cần test

* Load module
* Unload module
* `probe()` thành công
* `probe()` thất bại
* `compatible` sai
* GPIO không tồn tại
* `echo 1`
* `echo 0`
* Ghi dữ liệu không hợp lệ
* Mở device nhiều lần
* Đóng device

## Kết quả cần đạt

Có thể xác định lỗi nằm ở tầng nào:

```text
Device Tree
     │
     ▼
Platform Device
     │
     ▼
Driver Matching
     │
     ▼
probe()
     │
     ▼
GPIO
     │
     ▼
Character Device
     │
     ▼
User Space
```

---

# Giai đoạn 12 - Quản lý tài nguyên

## Mục tiêu

Hiểu Lifecycle của Driver.

```text
probe()
   │
   ▼
Allocate Resources
   │
   ▼
Driver Running
   │
   ▼
remove()
   │
   ▼
Release Resources
```

## Cần kiểm tra

* GPIO resource
* Character Device
* Device Number
* Class
* Device Node

## Ưu tiên sử dụng `devm_*`

Với GPIO, nên ưu tiên:

```text
devm_*
```

Ví dụ:

```c
devm_gpiod_get()
```

## Tư duy cần hình thành

> **Resource được acquire ở đâu thì phải có lifecycle tương ứng khi driver bị remove.**

## Kết quả cần đạt

Driver có thể load/unload mà không gây resource leak.

---

# Kiến trúc hoàn chỉnh

```text
                         DEVICE TREE
                              │
                              ▼
                      Device Tree Node
                              │
                              ▼
                   Device Tree Population
                              │
                              ▼
                      PLATFORM DEVICE
                              │
                              │ compatible matching
                              ▼
                      PLATFORM DRIVER
                              │
                            probe()
                              │
             ┌────────────────┴────────────────┐
             │                                 │
             ▼                                 ▼
      GPIO SUBSYSTEM                    CHARACTER DEVICE
             │                                 │
             ▼                                 ▼
      GPIO CONTROLLER                    /dev/myled
             │                                 │
             ▼                                 ▼
          HARDWARE                       USER SPACE
             │                                 │
             ▼                                 │
             LED ◄────── write() ─────────────┘
```

---

# Checkpoint cuối cùng

Sau khi hoàn thành bài thực hành, hãy tự trả lời được các câu hỏi sau mà không cần nhìn tài liệu:

* Device Tree dùng để làm gì?
* `compatible` dùng để làm gì?
* Device Tree node liên quan thế nào đến Platform Device?
* Platform Driver bind với Platform Device như thế nào?
* Vì sao `probe()` được gọi?
* Driver lấy GPIO từ đâu?
* Tại sao không hard-code GPIO trong driver?
* GPIO Subsystem đóng vai trò gì?
* Tại sao driver không trực tiếp thao tác GPIO register?
* Character Device đóng vai trò gì?
* Character Device có phải là Platform Driver không?
* `file_operations` kết nối User Space với Driver như thế nào?
* Dữ liệu từ `echo 1 > /dev/myled` đi qua những tầng nào trước khi LED sáng?
* Resource được acquire trong `probe()` thì được release ở đâu?

---

# 🧠 Kết luận

> **Device Tree mô tả phần cứng. Platform Driver bind với Device. Subsystem cung cấp abstraction. Character Device tạo User-space ABI.**

Sau khi nắm chắc bài `my_led`, có thể tiếp tục học các driver khác như:

```text
I2C
SPI
UART
PWM
RTC
```

Trong đó, **subsystem có thể thay đổi**, nhưng tư duy chung của Linux Device Model vẫn rất quan trọng:

```text
Device Description
        │
        ▼
Device
        │
        ▼
Driver
        │
        ▼
Subsystem
        │
        ▼
User Space Interface
```

> Nếu có thể giải thích rõ từng mũi tên trong sơ đồ kiến trúc, bạn đã nắm được phần cốt lõi của **Linux Platform Driver Architecture**.
