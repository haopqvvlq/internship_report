#ifndef QUANLY_H
#define QUANLY_H

#include <stdbool.h>

typedef struct
{
    char title[100];
    char artist[100];
    int duration;         // Thời lượng bài hát (giây)
} Song;

typedef struct Playlist Playlist;

// Tạo / hủy danh sách phát
Playlist *playlist_create(void);                     // Tạo playlist rỗng
void playlist_destroy(Playlist *pl);                 // Giải phóng toàn bộ bộ nhớ

// Hiển thị
void playlist_show(const Playlist *pl);              // Hiển thị toàn bộ danh sách bài hát
bool playlist_show_song(const Playlist *pl, int index); // Hiển thị bài hát tại vị trí index

// Quản lý bài hát
bool playlist_insert(Playlist *pl,
                     int index,
                     const Song *song);              // Chèn bài hát vào vị trí index

bool playlist_remove(Playlist *pl,
                     int index);                     // Xóa bài hát tại vị trí index

// Phát nhạc
bool playlist_play(Playlist *pl,
                   int index);                       // Chọn bài hát tại vị trí index để phát

bool playlist_next(Playlist *pl);                    // Chuyển sang bài hát kế tiếp

bool playlist_prev(Playlist *pl);                    // Quay về bài hát trước

#endif