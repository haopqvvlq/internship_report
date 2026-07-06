# Local Stream Socket

## Giới thiệu

Chương trình minh họa giao tiếp giữa hai tiến trình trên cùng một máy bằng **UNIX Domain Stream Socket (AF_UNIX + SOCK_STREAM)**.

Cấu trúc project:

```
.
├── local_stream_client
│   └── main.c
├── local_stream_server
│   └── main.c
└── notes.md
```

Server và Client giao tiếp thông qua file socket:

```
/tmp/local_stream.sock
```

---

## Luồng hoạt động

```
Server:
socket()
    ↓
bind()
    ↓
listen()
    ↓
accept()
    ↓
read()
    ↓
write()
    ↓
close()

Client:
socket()
    ↓
connect()
    ↓
write()
    ↓
read()
    ↓
close()
```

---

## Biên dịch

**Server**

```bash
cd local_stream_server
gcc main.c -o server
```

**Client**

```bash
cd ../local_stream_client
gcc main.c -o client
```

---

## Chạy chương trình

Mở **Terminal 1**:

```bash
cd local_stream_server
./server
```

Mở **Terminal 2**:

```bash
cd local_stream_client
./client
```

Kết quả mong đợi:

**Server**

```
Server is listening...
Client connected.
Received: Hello from client
```

**Client**

```
Connected to server.
Server replied: Hello from server
```

---

## Kiến thức đạt được

- Hiểu cách tạo và sử dụng **UNIX Domain Stream Socket**.
- Nắm được quy trình giao tiếp Client–Server trên cùng một máy.
- Biết sử dụng các API `socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `read()`, `write()`, `close()` và `unlink()`.
