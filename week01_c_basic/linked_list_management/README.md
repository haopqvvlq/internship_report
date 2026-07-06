# Linked List Management - Playlist Manager

## Giới thiệu

Đây là bài tập quản lý danh sách phát nhạc (Playlist) sử dụng **Linked List** trong ngôn ngữ C.

Mỗi bài hát được lưu trong một node của danh sách liên kết. Chương trình hỗ trợ thêm bài hát, phát nhạc, chuyển bài tiếp theo, quay lại bài trước, hiển thị danh sách và giải phóng bộ nhớ.

Thông qua bài tập này, người học làm quen với cách tổ chức dữ liệu bằng Linked List và cách xây dựng một thư viện (`lib`) tách biệt với chương trình chính.

---

## Kiến thức sử dụng

- C Programming
- Linked List
- Dynamic Memory Allocation (`malloc`, `free`)
- Modular Programming
- Header File (`.h`)
- Makefile

---

## Cấu trúc project

```
.
├── lib
│   ├── quanly.c      # Cài đặt các hàm quản lý playlist
│   └── quanly.h      # Khai báo cấu trúc dữ liệu và API
├── main.c            # Chương trình kiểm thử
├── Makefile
├── README.md
└── note.md
```

---

## Cách build

Biên dịch bằng Makefile:

```bash
make
```

Sau khi biên dịch thành công sẽ tạo file thực thi:

```
app
```

---

## Cách chạy

```bash
./app
```

---

## Chức năng

Chương trình hỗ trợ các thao tác sau:

- Tạo playlist
- Thêm bài hát vào vị trí bất kỳ
- Hiển thị toàn bộ playlist
- Phát bài hát theo vị trí
- Chuyển sang bài kế tiếp
- Quay lại bài trước
- Giải phóng toàn bộ bộ nhớ

---

## Luồng hoạt động

```
main()
   │
   ▼
playlist_create()
   │
   ▼
playlist_insert()
   │
   ▼
playlist_show()
   │
   ▼
playlist_play()
   │
   ├── playlist_next()
   └── playlist_prev()
   │
   ▼
playlist_destroy()
```

---

## Giải thích chương trình

### 1. Khởi tạo Playlist

```c
Playlist *playlist = playlist_create();
```

Khởi tạo một playlist rỗng để quản lý các bài hát.

---

### 2. Tạo dữ liệu bài hát

Mỗi bài hát gồm các thông tin:

- Tiêu đề
- Ca sĩ
- Thời lượng

Ví dụ:

```c
Song song1 = {
    .title = "Shape of You",
    .artist = "Ed Sheeran",
    .duration = 233
};
```

---

### 3. Thêm bài hát

```c
playlist_insert(playlist, position, &song);
```

Chèn bài hát vào vị trí mong muốn trong danh sách liên kết.

---

### 4. Hiển thị Playlist

```c
playlist_show(playlist);
```

Duyệt toàn bộ Linked List và hiển thị thông tin từng bài hát.

---

### 5. Phát nhạc

```c
playlist_play(playlist, index);
```

Đặt bài hát tại vị trí chỉ định làm bài hát hiện tại.

---

### 6. Chuyển bài

```c
playlist_next(playlist);
playlist_prev(playlist);
```

Di chuyển con trỏ bài hát hiện tại sang bài tiếp theo hoặc bài trước đó.

---

### 7. Giải phóng bộ nhớ

```c
playlist_destroy(playlist);
```

Giải phóng toàn bộ node và vùng nhớ đã cấp phát động.

---

## Ví dụ luồng sử dụng

```
Tạo playlist
        │
        ▼
Thêm 3 bài hát
        │
        ▼
Hiển thị playlist
        │
        ▼
Phát bài số 2
        │
        ▼
Next
        │
        ▼
Previous
        │
        ▼
Chèn thêm bài hát mới
        │
        ▼
Hiển thị playlist
        │
        ▼
Giải phóng bộ nhớ
```

---

## Kiến thức rút ra

- Cách xây dựng Linked List để quản lý dữ liệu động.
- Thao tác chèn node vào vị trí bất kỳ.
- Duyệt và cập nhật danh sách liên kết.
- Tách chương trình thành các module (`main.c`, `quanly.c`, `quanly.h`).
- Sử dụng Makefile để quản lý quá trình biên dịch.
- Quản lý bộ nhớ động và tránh rò rỉ bộ nhớ.

---

## Hướng phát triển

Có thể mở rộng chương trình bằng cách:

- Xóa bài hát.
- Tìm kiếm theo tên bài hát hoặc ca sĩ.
- Chỉnh sửa thông tin bài hát.
- Sắp xếp playlist theo tên hoặc thời lượng.
- Lưu playlist xuống file và đọc lại khi khởi động.
- Hỗ trợ chế độ phát ngẫu nhiên (Shuffle).
- Hỗ trợ phát lặp (Repeat).

---

## Tài liệu tham khảo

- The C Programming Language – Brian W. Kernighan & Dennis M. Ritchie
- Linux Manual Pages (`malloc`, `free`)
