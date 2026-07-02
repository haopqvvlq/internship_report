**Design Note - Bài tập "Viết chương trình quản lý danh sách liên kết"**

# 1. Ý tưởng

## Đề bài

> Viết chương trình quản lý danh sách liên kết.

Đề bài không quy định phải quản lý đối tượng gì, cũng không yêu cầu sử dụng singly hay doubly linked list.

---

## Ý tưởng ban đầu

Ban đầu mình nghĩ đến bài toán **quản lý sinh viên**.

Các chức năng dự kiến:

- Thêm sinh viên.
- Xóa sinh viên.
- Sửa thông tin.
- Hiển thị danh sách.

Tuy nhiên, sau khi phân tích, mình nhận thấy đây **không phải là bài toán phù hợp để thể hiện ưu điểm của linked list**.

Lý do:

- Truy cập theo vị trí vẫn phải duyệt từng node (`O(n)`).
- Hoàn toàn có thể sử dụng mảng động để quản lý.
- Việc sử dụng linked list không mang lại lợi ích nổi bật.

---

## Chuyển sang bài toán Playlist

Mình lựa chọn mô phỏng **playlist của trình phát nhạc**.

Mỗi bài hát gồm các thông tin:

- Tên bài hát.
- Ca sĩ.
- Ngày thêm vào playlist.

Mỗi node sẽ lưu:

- Dữ liệu bài hát.
- Con trỏ đến bài trước (`prev`).
- Con trỏ đến bài sau (`next`).

---

## Vì sao sử dụng Doubly Linked List?

Nếu chỉ cần:

- thêm,
- xóa,
- hiển thị,

thì **singly linked list cũng đủ**.

Để tận dụng ưu điểm của doubly linked list, playlist sẽ có thêm khái niệm:

> **Bài hát đang phát (playing).**

Playlist sẽ quản lý:

- `head`
- `tail`
- `playing`

Khi đó:

- Phát bài tiếp theo:

```text
playing = playing->next;
```

- Phát bài trước:

```text
playing = playing->prev;
```

Đây là thao tác đặc trưng của doubly linked list và là lý do chính để lựa chọn cấu trúc dữ liệu này.

---

## Menu chương trình

1. Hiển thị playlist.
2. Hiển thị bài hát thứ i.
3. Thêm bài hát vào vị trí i.
4. Xóa bài hát ở vị trí i.
5. Chọn bài hát thứ i để phát.
6. Phát bài hát tiếp theo.
7. Phát bài hát trước.
8. Thoát.

---

## Mục tiêu của bài tập

Thông qua bài tập này, mình muốn luyện tập:

- Thiết kế cấu trúc dữ liệu.
- Cấp phát và giải phóng bộ nhớ (`malloc`/`free`).
- Thao tác trên doubly linked list.
- Thiết kế API cho một module quản lý playlist.
- Viết mã theo hướng tách biệt dữ liệu và cấu trúc dữ liệu (`Song` và `Node`).

Bài tập không nhằm xây dựng một trình phát nhạc hoàn chỉnh, mà tập trung vào việc hiểu và vận dụng đúng cấu trúc dữ liệu danh sách liên kết đôi.

# 2. Triển khai code