#include <stdio.h>
#include <stdbool.h>
#include "quanly.h"
#include <stdlib.h>

typedef struct Node
{
    Song song;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct Playlist
{
    Node *head;
    Node *current; // Bài hát hiện tại đang phát
    int size;      // Số lượng bài hát trong danh sách
};

// Tạo / hủy danh sách phát
Playlist *playlist_create(void) // Tạo playlist rỗng
{
    Playlist *pl = malloc(sizeof(Playlist));
    if (pl == NULL)
    {
        return NULL;
    }
    pl->head = NULL;
    pl->current = NULL;
    pl->size = 0;
    return pl;
}
void playlist_destroy(Playlist *pl) // Giải phóng toàn bộ bộ nhớ
{
    while (pl->head != NULL)
    {
        Node *temp = pl->head;
        pl->head = pl->head->next;
        free(temp);
    }
    free(pl);
}

// Hiển thị
void playlist_show(const Playlist *pl) // Hiển thị toàn bộ danh sách bài hát
{
    if (pl == NULL)
    {
        printf("Playlist khong ton tai.\n");
        return;
    }

    if (pl->head == NULL)
    {
        printf("Playlist rong.\n");
        return;
    }

    Node *temp = pl->head;
    int index = 1;

    printf("\n===== PLAYLIST =====\n");

    while (temp != NULL)
    {
        printf("%2d. %-30s | %-20s | %3d s",
               index,
               temp->song.title,
               temp->song.artist,
               temp->song.duration);

        if (temp == pl->current)
        {
            printf("   <-- Dang phat");
        }

        printf("\n");

        temp = temp->next;
        index++;
    }

    printf("--------------------\n");
    printf("Tong so bai hat: %d\n", pl->size);
}
bool playlist_show_song(const Playlist *pl, int index) // Hiển thị bài hát tại vị trí index
/*index được tính từ 1 (phù hợp với cách hiển thị playlist)*/
{
    if (pl == NULL)
    {
        return false;
    }

    if (index < 1 || index > pl->size)
    {
        return false;
    }

    Node *temp = pl->head;

    for (int i = 1; i < index; i++)
    {
        temp = temp->next;
    }

    printf("Bai hat thu %d\n", index);
    printf("Title    : %s\n", temp->song.title);
    printf("Artist   : %s\n", temp->song.artist);
    printf("Duration : %d s\n", temp->song.duration);

    if (temp == pl->current)
    {
        printf("Status   : Dang phat\n");
    }

    return true;
}
// Quản lý bài hát
bool playlist_insert(Playlist *pl, int index, const Song *song) // Chèn bài hát vào vị trí index
/*Nếu index được đánh số từ 1 thì quy ước hợp lý là:

index = 1: chèn vào đầu danh sách.
index = size + 1: chèn vào cuối danh sách.
Các giá trị khác: chèn vào giữa.*/
{
    if (pl == NULL || song == NULL)
    {
        return false;
    }

    if (index < 1 || index > pl->size + 1)
    {
        return false;
    }

    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        return false;
    }

    newNode->song = *song;
    newNode->next = NULL;
    newNode->prev = NULL;

    // Chèn vào đầu danh sách
    if (index == 1)
    {
        newNode->next = pl->head;

        if (pl->head != NULL)
        {
            pl->head->prev = newNode;
        }

        pl->head = newNode;
    }
    else
    {
        // Tìm node đứng trước vị trí cần chèn
        Node *prev = pl->head;
        for (int i = 1; i < index - 1; i++)
        {
            prev = prev->next;
        }

        newNode->next = prev->next;
        newNode->prev = prev;

        if (prev->next != NULL)
        {
            prev->next->prev = newNode;
        }

        prev->next = newNode;
    }

    pl->size++;

    // Nếu đây là bài hát đầu tiên
    if (pl->current == NULL)
    {
        pl->current = pl->head;
    }

    return true;
}
bool playlist_remove(Playlist *pl, int index) // Xóa bài hát tại vị trí index
/**
 * Ý tưởng:
 * 1. Kiểm tra playlist có tồn tại và index có hợp lệ hay không.
 * 2. Duyệt đến node cần xóa.
 * 3. Nối node trước và node sau của node cần xóa:
 *    - Nếu xóa node đầu: cập nhật head.
 *    - Nếu xóa node giữa/cuối: cập nhật next và prev của hai node lân cận.
 * 4. Nếu node bị xóa đang là current:
 *    - Ưu tiên chuyển current sang bài hát kế tiếp.
 *    - Nếu không có bài kế tiếp thì chuyển sang bài trước.
 *    - Nếu playlist rỗng thì current = NULL.
 * 5. Giải phóng bộ nhớ của node.
 * 6. Giảm size và trả về true.
 */
{
    if (pl == NULL)
    {
        return false;
    }

    if (index < 1 || index > pl->size)
    {
        return false;
    }

    // Tìm node cần xóa
    Node *temp = pl->head;
    for (int i = 1; i < index; i++)
    {
        temp = temp->next;
    }

    // Nếu node bị xóa là current
    if (temp == pl->current)
    {
        if (temp->next != NULL)
        {
            pl->current = temp->next;
        }
        else if (temp->prev != NULL)
        {
            pl->current = temp->prev;
        }
        else
        {
            pl->current = NULL;
        }
    }

    // Cập nhật head nếu xóa node đầu
    if (temp->prev == NULL)
    {
        pl->head = temp->next;
    }
    else
    {
        temp->prev->next = temp->next;
    }

    // Cập nhật prev của node sau
    if (temp->next != NULL)
    {
        temp->next->prev = temp->prev;
    }

    free(temp);
    pl->size--;

    return true;
}
// Phát nhạc
bool playlist_play(Playlist *pl, int index) // Chọn bài hát tại vị trí index để phát
/**
 * Ý tưởng:
 * 1. Kiểm tra playlist có tồn tại hay không.
 * 2. Kiểm tra index có hợp lệ hay không (1 <= index <= size).
 * 3. Duyệt từ head đến node tại vị trí index.
 * 4. Gán current trỏ tới node đó.
 * 5. Trả về true nếu thành công, false nếu thất bại.
 *
 * Ghi chú:
 * - Hàm này chỉ thay đổi bài hát hiện tại đang được chọn/phát.
 * - Không làm thay đổi cấu trúc danh sách.
 * - Độ phức tạp: O(n).
 */
{
    if (pl == NULL)
    {
        return false;
    }

    if (index < 1 || index > pl->size)
    {
        return false;
    }

    Node *temp = pl->head;

    for (int i = 1; i < index; i++)
    {
        temp = temp->next;
    }

    pl->current = temp;

    printf("===== Dang phat =====\n");
    printf("Title    : %s\n", temp->song.title);
    printf("Artist   : %s\n", temp->song.artist);
    printf("Duration : %d s\n", temp->song.duration);

    return true;
}
bool playlist_next(Playlist *pl) // Chuyển sang bài hát kế tiếp
/*Ý tưởng
 * 1. Kiểm tra playlist có tồn tại và current có khác NULL hay không.
 * 2. Nếu current còn bài hát kế tiếp:
 *    - Chuyển current sang current->next.
 * 3. Nếu current đang ở bài hát cuối:
 *    - Không thể chuyển tiếp, trả về false.
 * 4. In thông tin bài hát hiện tại ra terminal.
 * 5. Trả về true nếu chuyển thành công.
 */
{
    if (pl == NULL || pl->current == NULL)
    {
        return false;
    }

    if (pl->current->next == NULL)
    {
        return false; // Không có bài hát kế tiếp
    }

    pl->current = pl->current->next;

    printf("===== Dang phat =====\n");
    printf("Title    : %s\n", pl->current->song.title);
    printf("Artist   : %s\n", pl->current->song.artist);
    printf("Duration : %d s\n", pl->current->song.duration);

    return true;
}
bool playlist_prev(Playlist *pl) // Quay về bài hát trước
/* Ý tưởng:
 * 1. Kiểm tra playlist có tồn tại và current có khác NULL hay không.
 * 2. Nếu current còn bài hát phía trước:
 *    - Chuyển current sang current->prev.
 * 3. Nếu current đang ở bài hát đầu:
 *    - Không thể quay lại, trả về false.
 * 4. In thông tin bài hát hiện tại ra terminal.
 * 5. Trả về true nếu chuyển thành công
 */
{
    if (pl == NULL || pl->current == NULL)
    {
        return false;
    }

    if (pl->current->prev == NULL)
    {
        return false; // Không có bài hát phía trước
    }

    pl->current = pl->current->prev;

    printf("===== Dang phat =====\n");
    printf("Title    : %s\n", pl->current->song.title);
    printf("Artist   : %s\n", pl->current->song.artist);
    printf("Duration : %d s\n", pl->current->song.duration);

    return true;
}