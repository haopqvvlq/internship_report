#pragma once
#include <linux/ioctl.h>

/* Magic number để nhận diện nhóm ioctl của driver */
#define RWBUF_MAGIC 'R'

/* Struct dữ liệu kernel gửi về cho user */
struct rwbuf_stats
{
    unsigned long read_count;  /* số lần read() */
    unsigned long write_count; /* số lần write() */
};

/*
 * IOCTL:
 * _IOR = User đọc dữ liệu từ driver
 * RWBUF_MAGIC = nhóm lệnh
 * 0 = số thứ tự lệnh
 * struct rwbuf_stats = kiểu dữ liệu trả về
 */
#define RWBUF_IOCTL_GET_STATS \
    _IOR(RWBUF_MAGIC, 0, struct rwbuf_stats)

/*có thể hiểu đơn giản là:
    "Tạo một mã ioctl tên GET_STATS, khi user gọi lệnh này thì driver sẽ trả về một struct rwbuf_stats."
*/