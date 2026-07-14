# Kinh nghiệm khi viết Makefile cho Linux Kernel Module

## Không nên dùng `PWD`

Ví dụ:

```make
obj-m += rwbufdev.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

ccflags-y += -I$(PWD)/../include

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
```

### Vì sao?

Khi chạy:

```bash
make -C $(KDIR) M=$(PWD) modules
```

Kbuild sẽ gọi Makefile của module **lần thứ hai**.

Lúc này một số biến môi trường (đặc biệt là `PWD`) có thể bị thay đổi. Trong trường hợp mình gặp:

```text
PWD = /usr/src/linux-headers-6.8.0-124-generic
```

nên:

```make
ccflags-y += -I$(PWD)/../include
```

bị mở rộng thành:

```text
-I/usr/src/linux-headers-6.8.0-124-generic/../include
```

thay vì:

```text
-I/home/.../practice/include
```

Kết quả là compiler không tìm được header:

```text
fatal error: rwbuf_ioctl.h: No such file or directory
```

---

## Nên dùng `$(src)` và `$(CURDIR)`

Makefile khuyến nghị:

```make
obj-m := rwbufdev.o

KDIR := /lib/modules/$(shell uname -r)/build

# Thêm thư mục chứa header
ccflags-y := -I$(src)/../include

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
```

### Giải thích các biến

#### `$(src)`

- Là biến do **Kbuild** cung cấp.
- Luôn trỏ tới thư mục chứa source của module.
- Không bị thay đổi khi Kbuild gọi Makefile lần thứ hai.

Ví dụ:

```text
$(src)
/home/hao/.../practice/driver
```

nên:

```make
-I$(src)/../include
```

luôn trở thành:

```text
-I/home/hao/.../practice/include
```

#### `$(CURDIR)`

- Là biến chuẩn của GNU Make.
- Luôn là thư mục hiện tại chứa Makefile.
- Ổn định hơn `PWD`.

Ví dụ:

```make
M=$(CURDIR)
```

thay vì:

```make
M=$(PWD)
```

---

## Vì sao nên dùng `$(MAKE)` thay vì `make`

Nên viết:

```make
$(MAKE) -C $(KDIR) M=$(CURDIR) modules
```

không nên viết:

```make
make -C $(KDIR) M=$(PWD) modules
```

Lý do:

- Kế thừa các tùy chọn của lệnh `make` hiện tại (`-j`, `V=1`, `-k`, ...).
- Là cách được GNU Make và Kbuild khuyến nghị.

---

## Cách kiểm tra đường dẫn include

Khi nghi ngờ Makefile truyền sai đường dẫn, chạy:

```bash
make V=1
```

Quan sát dòng compile.

Nếu đúng sẽ thấy:

```text
-I/home/.../practice/include
```

Nếu thấy:

```text
-I/usr/src/linux-headers...
```

hoặc hoàn toàn không có `-I.../include`, thì gần như chắc chắn Makefile đang truyền sai đường dẫn.

---

## Kinh nghiệm rút ra

- ❌ Không dùng `PWD := $(shell pwd)` trong Makefile của Linux Kernel Module.
- ✅ Dùng `$(src)` để chỉ thư mục source của module.
- ✅ Dùng `$(CURDIR)` để truyền tham số `M=`.
- ✅ Dùng `$(MAKE)` thay vì gọi `make` trực tiếp.
- ✅ Khi gặp lỗi `No such file or directory`, luôn kiểm tra bằng:

```bash
make V=1
```

để xem compiler thực sự nhận được những tham số `-I` nào.

> **Quy tắc nhớ nhanh:** Khi viết Linux Kernel Module, hãy ưu tiên dùng các biến do Kbuild (`$(src)`) và GNU Make (`$(CURDIR)`, `$(MAKE)`) cung cấp, thay vì tự khai báo `PWD := $(shell pwd)`. Điều này giúp tránh các lỗi khó phát hiện khi Kbuild gọi Makefile nhiều lần.
