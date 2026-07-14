# Week 06 - Driver Module & Character Device Driver

## Overview

Trong tuần này, nội dung thực hành tập trung vào việc xây dựng **Linux Character Device Driver** dưới dạng **Kernel Module** (ngoài ra còn dạng **Built-in Driver**). Hai phương pháp triển khai character device được nghiên cứu và thực hành:

1. **Traditional Character Device Driver**
   - Sử dụng `alloc_chrdev_region()`, `cdev_init()`, `cdev_add()`, `class_create()`, `device_create()`
   - Tự quản lý major/minor number
   - Phù hợp để hiểu đầy đủ cơ chế hoạt động của character device trong Linux kernel

2. **Misc Character Device Driver**
   - Sử dụng `misc_register()`
   - Kernel tự cấp phát minor number
   - Đơn giản hóa quá trình đăng ký device
   - Thường được sử dụng cho các driver nhỏ và đơn giản

---

## Directory Structure

```text
week06_DriverModule_CharacterDeviceDriver
├── misc_character_device_driver
│   ├── knowledge
│   └── practice
└── traditional_character_device_driver
    ├── knowledge
    └── practice
```

### traditional_character_device_driver

Nghiên cứu và thực hành character device theo cách truyền thống.

#### knowledge

Chứa tài liệu lý thuyết liên quan

#### practice

Chứa bài thực hành: **Viết char device driver đọc/ghi buffer** (có sử dụng `ioctl` system call)

---

### misc_character_device_driver

Nghiên cứu và thực hành character device sử dụng Misc Framework.

#### knowledge

Chứa tài liệu lý thuyết:

#### practice

Chứa bài thực hành: **Viết char device driver echo data**

---
