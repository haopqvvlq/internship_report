#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
/* Chứa định nghĩa ioctl và struct dùng chung */
#include "rwbuf_ioctl.h"

int main(void)
{
    int fd;

    /* Nhận dữ liệu thống kê từ driver */
    struct rwbuf_stats stats;

    /* Mở device node */
    fd = open("/dev/rwbufdev", O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    while (1)
    {
        int choice;
        printf("\n===== RWBUF MENU =====\n");
        printf("1. Write\n");
        printf("2. Read\n");
        printf("3. Get Stats\n");
        printf("4. Exit\n");
        printf("Choose: ");

        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            // write()
            char buf1[128];

            printf("Input: ");
            scanf("%127s", buf1);

            write(fd, buf1, strlen(buf1));
            break;

        case 2:
            // read()
            char buf2[128];

            int n = read(fd, buf2, sizeof(buf2) - 1);
            if (n > 0)
            {
                buf2[n] = '\0';
                printf("Data: %s\n", buf2);
            }
            break;

        case 3:
            // ioctl()
            /*
             * Gửi ioctl GET_STATS
             * Driver sẽ copy dữ liệu vào biến stats
             */
            if (ioctl(fd,                    // file descriptor của device
                      RWBUF_IOCTL_GET_STATS, // mã lệnh ioctl cần thực hiện
                      &stats) < 0)           // địa chỉ biến nhận dữ liệu từ driver
            {
                perror("ioctl");
                close(fd);
                return -1;
            }

            /* Hiển thị kết quả nhận từ driver */
            printf("Read count   : %lu\n", stats.read_count);
            printf("Write count  : %lu\n", stats.write_count);
            break;

        case 4:
            printf("Bye!\n");
            close(fd);
            return 0;

        default:
            printf("Invalid choice!\n");
        }
    }

    close(fd);
    return 0;
}