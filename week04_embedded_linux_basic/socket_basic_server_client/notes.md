**SOCKET**

1. Giới thiệu
2. Flow hoạt động
3. Sockets: Internet Domain socket
4. Sockets: Unix Domain

# 1. Giới thiệu

## 1. Định nghĩa

Socket là cơ chế IPC (Inter-Process Communication) cho phép các tiến trình trao đổi dữ liệu với nhau.

Các tiến trình có thể:

- Chạy trên cùng một máy.
- Chạy trên các máy khác nhau thông qua mạng.

Mỗi socket được đặc tưng bởi:

```c
socket(domain, type, protocol);
```

1. Domain (họ địa chỉ) = Giao tiếp ở đâu (cùng máy/qua mạng)

```make
AF_UNIX  -> cùng máy
AF_INET  -> qua mạng IPv4
AF_INET6 -> qua mạng IPv6
```

2. Type (kiểu truyền dữ liệu) = Truyền kiểu gì (luồng liên tục/ gói tin độc lập)

```make
SOCK_STREAM -> luồng liên tục (TCP)
SOCK_DGRAM  -> gói tin độc lập (UDP)
```

3. Protocol (giao thức) = Dùng giao thức nào

```
TCP
UDP
...
```

**Liên hệ với File thường**

Như đã học ở phần file thường, các tài nguyên I/O được kernel quản lý và process thao tác với chúng thông qua file descriptor (fd) cùng các system call. Socket cũng hoạt động theo cơ chế tương tự. Trong Linux, nhiều đối tượng I/O như file thường, pipe, FIFO, socket, eventfd, timerfd hay epoll đều được truy cập thông qua file descriptor. Mỗi process sở hữu một **File Descriptor Table** để quản lý các file descriptor mà nó đang sử dụng.

[Kernel quản lý file thường](../BUỔI%203%20_%20File%20system%20&%20Tương%20tác%20cơ%20bản/notes.md#quanlyfile)

```make
Process
   │
   ▼
File Descriptor Table
   │
   ▼
Open File Table
   │
   ▼
Socket Object (Socket Table)
```

## 2. Domain (Address Family)

Domain xác định môi trường giao tiếp và cách định danh hai đầu kết nối.
![domain](./images/image1.png)
Hai domain phổ biến nhất:

### 1. UNIX Domain Socket (`AF_UNIX`)

Dùng cho các tiến trình trên cùng một máy.

Ví dụ:

```make
Process A <----> Process B
```

Socket được định danh bằng một đường dẫn file:

```make
/tmp/my_socket
```

Ưu điểm:

- Nhanh hơn socket mạng.
- Không cần IP.
- Chỉ dùng trong cùng hệ điều hành.

### 2. Internet Domain Socket (`AF_INET`, `AF_INET6`)

Dùng cho giao tiếp qua mạng.

Ví dụ:

```make
PC A <---- mạng ----> PC B
```

Socket được định danh bằng:

```make
IP + Port
```

Ví dụ:

```make
192.168.1.10:8080
```

## 3. Type

Type xác định cách dữ liệu được truyền.

Hai loại phổ biến:

### 1. Stream Socket (`SOCK_STREAM`)

Đặc điểm:

- Có kết nối (connection-oriented)
- Dữ liệu đến đúng thứ tự
- Không mất dữ liệu

Giống như cuộc gọi điện thoại:

```make
A ======== B
```

Thường dùng với:

```make
TCP
```

### 2. Datagram Socket (`SOCK_DGRAM`)

Đặc điểm:

- Không cần kết nối
- Mỗi lần gửi là một gói độc lập
- Có thể mất gói
- Có thể đến sai thứ tự

Giống gửi thư:

```make
A ---> Packet
A ---> Packet
A ---> Packet
```

Thường dùng với:

```make
UDP
```

### 3. So sánh Stream Socket và Datagram Socket

| Stream Socket (TCP)                                                                   | Datagram Socket (UDP)                                                    |
| ------------------------------------------------------------------------------------- | ------------------------------------------------------------------------ |
| Tin cậy: đảm bảo dữ liệu đến nơi, đúng thứ tự, tự phát hiện và truyền lại khi mất gói | Không đảm bảo dữ liệu đến nơi, không đảm bảo thứ tự, không tự truyền lại |
| Phải thiết lập kết nối trước (`connect()`, `accept()`)                                | Không cần thiết lập kết nối trước                                        |
| Dữ liệu được nhìn như một **luồng byte liên tục (byte stream)**                       | Dữ liệu được gửi dưới dạng **từng datagram/gói tin riêng biệt**          |
| Không giữ ranh giới giữa các lần `send()`                                             | Giữ nguyên ranh giới giữa các datagram                                   |
| Thường dùng cho HTTP, SSH, FTP, Database...                                           | Thường dùng cho DNS, VoIP, Video streaming, Game online...               |

## 4. Protocol

Protocol là giao thức thực sự được dùng để đóng gói và truyền dữ liệu.

Thông thường với một Domain và Type đã chọn chỉ có 1 giao thức có thể dùng nên protocol thường có giá trị 0.

Ví dụ:
| Domain | Type | Protocol |
| ------- | ----------- | -------------- |
| AF_INET | SOCK_STREAM | TCP |
| AF_INET | SOCK_DGRAM | UDP |
| AF_UNIX | SOCK_STREAM | Local Stream |
| AF_UNIX | SOCK_DGRAM | Local Datagram |

Khi tạo socket:

```c
socket(AF_INET, SOCK_STREAM, 0);
```

Số 0 nghĩa là:

```make
Kernel tự chọn protocol mặc định phù hợp.
```

Ví dụ:

```make
AF_INET + SOCK_STREAM
          ↓
         TCP
```

# 2. Flow hoạt động

## 2.1 Flow hoạt động của Stream Socket (TCP Socket)

### Cái nhìn tổng thể

Có hai vai trò:

**Server (Passive Socket)**

"Passive" nghĩa là:

```make
Tôi mở cửa và chờ người khác kết nối tới.
```

**Client (Active Socket)**

"Active" nghĩa là:

```make
Tôi chủ động đi kết nối.
```

![flow_tcp](./images/image2.png)

### Bước 1: Server tạo socket

```c
int listenfd = socket(AF_INET, SOCK_STREAM, 0);
```

Kernel tạo một socket descriptor.

Ví dụ:

```make
listenfd = 3
```

Lúc này socket chưa có địa chỉ.

### Bước 2: `bind()`: server gắn socket với IP + Port

```c
bind(listenfd, ...);
```

Gắn socket với:

```make
IP + Port
```

Ví dụ:

```make
0.0.0.0:8080
```

Ý nghĩa:

```make
"Tôi sẽ lắng nghe tại cổng 8080"
```

### Bước 3: `listen()`: Chuyển socket sang trạng thái chờ kết nối

```c
listen(listenfd, backlog);
```

Socket chuyển sang trạng thái:

```make
LISTEN
```

Từ đây kernel bắt đầu nhận các yêu cầu kết nối TCP.

### Bước 4: `accept()`: Lấy một kết nối cụ thể từ hàng đợi kết nối

```c
int connfd = accept(listenfd, NULL, NULL);
```

Đây là điểm cực kỳ quan trọng.

Nếu chưa có client:

```make
Server ngủ
(chặn tại accept)
listenfd
|
+--> accept() đang chờ
```

### Bước 5: Client connect()

Ở phía client:

```c
int sockfd = socket(AF_INET, SOCK_STREAM, 0);

connect(sockfd, ...);
```

Client gửi yêu cầu kết nối tới:

```make
Server_IP:8080
```

### Bước 6: TCP bắt tay 3 bước

Kernel hai bên tự thực hiện:

```make
Client         Server

SYN ----------->
<----------- SYN+ACK

ACK ----------->
```

Kết nối TCP được thiết lập.

### Bước 7: `accept()` thức dậy

Sau khi bắt tay thành công:

```c
connfd = accept(...);
```

trả về một fd mới.

Ví dụ:

```make
listenfd = 3
connfd = 4
```

Lúc này:

```make
fd 3 --> tiếp tục nghe kết nối mới

fd 4 --> giao tiếp với client vừa kết nối
```

Server thường:

```c
while (1)
{
    int connfd = accept(...);

    xử lý connfd;
}
```

### Bước 8: Trao đổi dữ liệu

Sau khi kết nối thành công:

Client:

```c
write(sockfd, ...);
read(sockfd, ...);
```

Server:

```c
read(connfd, ...);
write(connfd, ...);
```

TCP là:

```make
Full Duplex
```

Nghĩa là:

```make
A gửi cho B
đồng thời
B gửi cho A
```

không cần chờ nhau.

### Bước 9: `close()`

Client:

```c
close(sockfd);
```

hoặc Server:

```c
close(connfd);
```

Kernel thực hiện quá trình đóng TCP.

Sau khi đóng:

```make
Không thể gửi nhận dữ liệu nữa.
```

### Tóm tắt

```make
**SERVER**

socket()
   ↓
bind()
   ↓
listen()
   ↓
accept()
   ↓
read()/write()
   ↓
close()
```

```make
**CLIENT**

socket()
   ↓
connect()
   ↓
read()/write()
   ↓
close()
```

Nếu chỉ được nhớ một câu duy nhất về TCP Stream Socket thì nên nhớ:

> Server tạo một **listening socket** để chờ kết nối. Mỗi lần `accept()` thành công, kernel tạo ra một **connected socket** mới dùng riêng cho client đó; việc `read()` và `write()` diễn ra trên socket mới này, không phải trên **listening socket**.

Đây là ý quan trọng nhất của toàn bộ flow hoạt động của TCP socket.

## 2.2 Flow hoạt động của Datagram Socket (UDP Socket)

![flow_udp](./images/image3.png)

### Ý tưởng cốt lõi của Datagram Socket

Hãy tưởng tượng:

**TCP**

Giống gọi điện

```make
A gọi B
↓
B bắt máy
↓
Hai bên nói chuyện
↓
Cúp máy
```

Phải thiết lập kết nối trước.

**UDP**

Giống gửi thư

```make
A gửi thư cho B
```

B nhận được thì tốt.

Không nhận được cũng không ai biết.

Không cần gọi điện trước.

### Bước 1: Server tạo socket

```c
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
```

Kernel tạo UDP socket.

### Bước 2: `bind()`

```c
bind(sockfd, ...);
```

Ví dụ:

```make
0.0.0.0:8888
```

Ý nghĩa:

```make
"Tôi sẽ nhận các UDP packet gửi tới cổng 8888"
```

### Bước 3: Client tạo socket

```c
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
```

Khác TCP ở chỗ:

```make
Không cần connect()
```

### Bước 4: Client gửi dữ liệu

```c
sendto(
    sockfd,
    buffer,
    len,
    0,
    &server_addr,
    sizeof(server_addr)
);
```

Client phải chỉ rõ:

```make
IP đích
Port đích
```

Ví dụ:

```make
192.168.1.10:8888
```

### Bước 5: Server nhận dữ liệu

```c
recvfrom(
    sockfd,
    buffer,
    sizeof(buffer),
    0,
    &client_addr,
    &addrlen
);
```

Ngoài dữ liệu nhận được:

```make
"Hello"
```

Server còn biết:

```make
Gói tin này đến từ ai
```

Ví dụ:

```make
192.168.1.20:50000
```

### Bước 6: Server phản hồi

Server dùng chính địa chỉ vừa nhận được:

```c
sendto(
    sockfd,
    reply,
    len,
    0,
    &client_addr,
    sizeof(client_addr)
);
```

### Sự khác nhau mấu chốt giữa Datagram Socket và Stream Socket

**TCP**:

```make
1 client
↓
1 connection
↓
1 socket riêng
```

Ví dụ:

```make
listenfd
    ↓
accept()
    ↓
connfd_A

listenfd
    ↓
accept()
    ↓
connfd_B
```

**UDP**:

```make
1 socket duy nhất
        Client A
            |
            |
Client B ---+--- Server Socket
            |
            |
        Client C
```

Server không tạo thêm socket mới.

Mọi packet đều đi qua:

```c
sockfd
```

duy nhất.

Vì UDP không có khái niệm "connection", nên kernel không cần tạo một socket riêng cho từng client.
Đây là khác biệt lớn nhất giữa TCP và UDP.

# 3. Internet Domain Socket

```make
Internet Domain Socket
│
├── 1. IP Address
│   ├── IPv4
│   └── IPv6
│
├── 2. Port
│
├── 3. Socket Address
│   └── (IP + Port)
│
├── 4. sockaddr_in
├── 5. sockaddr_in6
│
├── 6. Byte Order
│   ├── htons()
│   ├── htonl()
│   ├── ntohs()
│   └── ntohl()
│
├── 7. inet_pton()
├── 8. inet_ntop()
│
└── 9. bind()
```

## 1. IP Address

**IP Address (Internet Protocol Address)** là địa chỉ dùng để định danh một thiết bị trên mạng IP (LAN, Internet, v.v.).

Hãy tưởng tượng mạng là một thành phố:

- Địa chỉ IP giống như địa chỉ nhà.

- Port (sẽ học sau) giống như số phòng trong ngôi nhà đó.

### 1. IPv4

![ipv4](./images/image4.png)
IPv4 là phiên bản địa chỉ IP phổ biến nhất hiện nay.

Địa chỉ gồm 32 bit, mỗi octet có giá trị từ 0–255, thường được viết dưới dạng 4 số thập phân ngăn cách bởi dấu chấm`.`

### 2. IPv6

![ipv6](./images/image5.png)
IPv6 được tạo ra để thay thế IPv4 đã gần cạn kiệt địa chỉ.

Địa chỉ gồm 128 bit, viết bằng 8 nhóm hexadecimal (0–9, a–f) ngăn cách bởi dấu `:`

### 3. Kernel hiểu IPv4, IPv6 như nào?

Khi lập trình socket, bạn không làm việc trực tiếp với chuỗi IP, mà thường chuyển nó thành dạng nhị phân bằng `inet_pton()` rồi lưu vào:

| Giao thức | Cấu trúc địa chỉ      |
| --------- | --------------------- |
| IPv4      | `struct sockaddr_in`  |
| IPv6      | `struct sockaddr_in6` |

## 2. Port là gì?

### 1. Định nghĩa

**Port (cổng)** là một số nguyên **16-bit** dùng để xác định **dịch vụ hoặc ứng dụng** cụ thể đang chạy trên một máy.

Nếu:

- IP Address xác định máy nào
- Port xác định chương trình nào trên máy đó

thì:

```make
IP + Port
```

sẽ xác định chính xác nơi dữ liệu cần được gửi đến.

```make
Máy chủ: 192.168.1.10

├── Web Server      → Port 80
├── HTTPS Server    → Port 443
├── SSH Server      → Port 22
└── Database        → Port 3306
```

### 2. Các dải Port

#### 1. Well-Known Ports

```make
0 - 1023
```

Các cổng chuẩn được gán cho những dịch vụ phổ biến.

| Port  | Dịch vụ |
| ----- | ------- |
| 20,21 | FTP     |
| 22    | SSH     |
| 25    | SMTP    |
| 53    | DNS     |
| 80    | HTTP    |
| 443   | HTTPS   |

Ví dụ:

```make
google.com:443
```

Trình duyệt biết phải dùng HTTPS.

#### 2. Registered Ports

```
1024 - 49151
```

Dành cho các ứng dụng đã đăng ký.

Ví dụ:

| Port | Dịch vụ       |
| ---- | ------------- |
| 3306 | MySQL         |
| 5432 | PostgreSQL    |
| 6379 | Redis         |
| 8080 | HTTP thay thế |

#### 3. Dynamic / Ephemeral Ports

```
49152 - 65535
```

Kernel thường tự động cấp phát cho client.

Ví dụ:

```
Client:
192.168.1.100:52341

Server:
8.8.8.8:53
```

Ở đây:

```
52341
```

là port tạm thời do kernel chọn.

### 3. Server Port và Client Port

Khi kết nối TCP:

```
Client                         Server
------                         ------
192.168.1.100:52341  --->  192.168.1.10:80
```

Server thường lắng nghe trên một port cố định:

```
80
443
22
...
```

Client thường dùng một port ngẫu nhiên:

```
52341
```

để phân biệt các kết nối khác nhau.

## 3. Socket Address

**Socket Address** là địa chỉ đầy đủ của một endpoint trên mạng.

Sau khi đã biết:

- **IP Address** → xác định máy nào trên mạng.
- **Port** → xác định ứng dụng/dịch vụ nào trên máy đó.
  thì ta ghép chúng lại thành:

```
Socket Address = IP Address + Port
```

## 4. `sockaddr_in` và `sockaddr_in6`

Sau khi có:

```
Socket Address = IP + Port
```

thì Linux cần một cấu trúc để lưu địa chỉ đó trong bộ nhớ.

| Giao thức | Cấu trúc              |
| --------- | --------------------- |
| IPv4      | `struct sockaddr_in`  |
| IPv6      | `struct sockaddr_in6` |

### 1. `struct sockaddr_in` (IPv4)

Dùng để lưu địa chỉ IPv4.

```c
struct sockaddr_in {
    sa_family_t    sin_family;
    in_port_t      sin_port;
    struct in_addr sin_addr;
};
```

Các thành phần chính:

| Thành viên   | Ý nghĩa                  |
| ------------ | ------------------------ |
| `sin_family` | Loại địa chỉ (`AF_INET`) |
| `sin_port`   | Port                     |
| `sin_addr`   | Địa chỉ IPv4             |

Ví dụ

```c
struct sockaddr_in addr;

addr.sin_family = AF_INET;
addr.sin_port   = htons(8080);

inet_pton(AF_INET,
          "192.168.1.10",
          &addr.sin_addr);
```

Biểu diễn:

```
192.168.1.10:8080
```

### 2. `struct sockaddr_in6` (IPv6)

Dùng để lưu địa chỉ IPv6.

```c
struct sockaddr_in6 {
    sa_family_t     sin6_family;
    in_port_t       sin6_port;
    struct in6_addr sin6_addr;
};
```

Các thành phần chính

| Thành viên    | Ý nghĩa                   |
| ------------- | ------------------------- |
| `sin6_family` | Loại địa chỉ (`AF_INET6`) |
| `sin6_port`   | Port                      |
| `sin6_addr`   | Địa chỉ IPv6              |

Ví dụ

```c
struct sockaddr_in6 addr;

addr.sin6_family = AF_INET6;
addr.sin6_port   = htons(8080);

inet_pton(AF_INET6,
          "2001:db8::1",
          &addr.sin6_addr);
```

Biểu diễn:

```
[2001:db8::1]:8080
```

## 5. Byte Order

### 1. Byte Order là gì?

**Byte Order (Endianness)** là cách sắp xếp các byte của một số trong bộ nhớ.

Có 2 kiểu phổ biến:

| Kiểu          | Ý nghĩa                              |
| ------------- | ------------------------------------ |
| Big Endian    | Byte có trọng số lớn nhất đứng trước |
| Little Endian | Byte có trọng số nhỏ nhất đứng trước |

Ví dụ số:

```
0x1234
```

gồm 2 byte:

```
0x12 0x34
```

Trong bộ nhớ:

| Kiểu          | Thứ tự lưu |
| ------------- | ---------- |
| Big Endian    | 12 34      |
| Little Endian | 34 12      |

### 2. Tại sao cần quan tâm?

Các máy tính có thể dùng kiến trúc khác nhau:

```
Máy A → Little Endian
Máy B → Big Endian
```

Nếu gửi dữ liệu trực tiếp qua mạng:

```
0x1234
```

thì hai bên có thể hiểu khác nhau.

Để tránh điều đó, giao thức TCP/IP quy định:

```
Network Byte Order = Big Endian
```

### 3. Host Byte Order và Network Byte Order

| Thuật ngữ          | Ý nghĩa                           |
| ------------------ | --------------------------------- |
| Host Byte Order    | Cách lưu của máy hiện tại         |
| Network Byte Order | Chuẩn dùng trên mạng (Big Endian) |

Trên Linux x86/x86-64:

```
Host Byte Order = Little Endian
```

nên thường phải chuyển đổi trước khi gửi dữ liệu lên mạng.

### 4. Các hàm chuyển đổi

Các hàm sau dùng để chuyển dữ liệu từ **host byte order** sang **network byte order**.

| Hàm       | Ý nghĩa                 |
| --------- | ----------------------- |
| `htons()` | Host → Network (16 bit) |
| `htonl()` | Host → Network (32 bit) |
| `ntohs()` | Network → Host (16 bit) |
| `ntohl()` | Network → Host (32 bit) |

Trong lập trình socket, gần như lúc nào gán `sin_port` cũng phải dùng `htons()`. Đây là hàm bạn sẽ gặp nhiều nhất trong nhóm chuyển đổi Byte Order.

```c
struct sockaddr_in addr;

addr.sin_family = AF_INET;
addr.sin_port   = htons(8080);
```

Lý do:

```c
sin_port // socket address internet port
```

phải được lưu theo Network Byte Order.

## 6. `inet_pton()` và `inet_ntop()`

Sau khi học `sockaddr_in`, bạn sẽ thấy:

```c
addr.sin_addr
```

không lưu IP dưới dạng chuỗi:

```
"192.168.1.10"
```

mà lưu dưới dạng nhị phân.

Do đó cần các hàm chuyển đổi giữa:

```
Chuỗi IP  ⇄  Dạng nhị phân
```

## 1. Các hàm chuyển đổi

### 1. `inet_pton()`

```c
int inet_pton(int af,
              const char *src,
              void *dst);
```

**Presentation To Network**

Chuyển:

```
Chuỗi IP
↓
Dạng nhị phân
```

Ví dụ:

```c
inet_pton(AF_INET,
"192.168.1.10",
&addr.sin_addr);
```

### 2. `inet_ntop()`

```c
const char *inet_ntop(int af,
                      const void *src,
                      char *dst,
                      socklen_t size);
```

**Network To Presentation**

Chuyển:

```
Dạng nhị phân
    ↓
Chuỗi IP
```

Ví dụ:

```c
char ip[INET_ADDRSTRLEN];

inet_ntop(AF_INET,
          &addr.sin_addr,
          ip,
          sizeof(ip));
```

## 2. Tóm lại

```
"192.168.1.10"
        ↓
    inet_pton()
        ↓
  addr.sin_addr
        ↓
    inet_ntop()
        ↓
"192.168.1.10"
```

| Hàm           | Chức năng           |
| ------------- | ------------------- |
| `inet_pton()` | Chuỗi IP → Nhị phân |
| `inet_ntop()` | Nhị phân → Chuỗi IP |

# 4. Unix Domain Socket

**UNIX Domain Socket (AF_UNIX)** là cơ chế giao tiếp giữa các process trên cùng một máy Linux/Unix.

## 1. UNIX Domain Socket address

Khác với Internet Socket:

```
Internet Socket address
=
IP + Port
```

UNIX Socket sử dụng:

```
UNIX Socket address
=
Pathname
```

Ví dụ:

```
/tmp/server.sock
```

## 2. Cấu trúc địa chỉ

```c
struct sockaddr_un {
    sa_family_t sun_family;
    char        sun_path[108];
};
```

| Thành viên   | Ý nghĩa          |
| ------------ | ---------------- |
| `sun_family` | `AF_UNIX`        |
| `sun_path`   | Đường dẫn socket |

Ví dụ:

```c
struct sockaddr_un addr;

addr.sun_family = AF_UNIX;
strcpy(addr.sun_path, "/tmp/server.sock");
```

## 3. Một số lưu ý về UNIX Domain Socket

### 1. Socket file không tự bị xóa

Khi server đóng socket hoặc chương trình kết thúc:

```
/tmp/server.sock
```

vẫn còn tồn tại trong filesystem.

Nếu chạy lại chương trình và gọi:

```c
bind()
```

có thể gặp lỗi:

```
Address already in use
```

Do đó thường phải xóa file cũ trước khi `bind()`:

```c
unlink("/tmp/server.sock");
```

hoặc:

```c
remove("/tmp/server.sock");
```

### 2. UNIX Socket chịu sự quản lý của quyền file

Để một process kết nối tới UNIX socket:

```
/tmp/server.sock
```

nó phải có quyền phù hợp trên file socket đó.

Thông thường:

```
Client muốn connect()
        ↓
Cần quyền write trên socket file
```

Do đó UNIX Socket có thể dùng cơ chế permission của filesystem để kiểm soát truy cập.

### 3. Quyền của socket file

Khi gọi:

```c
bind()
```

kernel sẽ tạo ra socket file:

```
/tmp/server.sock
```

Quyền thực tế của file phụ thuộc vào:

```
Quyền mặc định
        +
umask()
```

Có thể thay đổi quyền bằng:

```c
chmod()
```

hoặc điều chỉnh:

```c
umask()
```

hay quyền của thư mục chứa socket.

Ví dụ:

```
chmod 660 /tmp/server.sock
```

chỉ cho phép owner và group truy cập.
