#include <stdio.h>
#include "quanly.h"

int main(void)
{
    Playlist *playlist = playlist_create();

    if (playlist == NULL)
    {
        printf("Khong the tao playlist!\n");
        return 1;
    }

    Song song1 = {
        .title = "Shape of You",
        .artist = "Ed Sheeran",
        .duration = 233};

    Song song2 = {
        .title = "Counting Stars",
        .artist = "OneRepublic",
        .duration = 257};

    Song song3 = {
        .title = "Believer",
        .artist = "Imagine Dragons",
        .duration = 204};

    // Thêm 3 bài hát
    playlist_insert(playlist, 1, &song1);
    playlist_insert(playlist, 2, &song2);
    playlist_insert(playlist, 3, &song3);

    playlist_show(playlist);

    playlist_play(playlist, 2);

    playlist_next(playlist);

    playlist_prev(playlist);

    Song song4 = {
        .title = "Thunder",
        .artist = "Imagine Dragons",
        .duration = 187};

    // Chèn bài hát vào vị trí thứ 2
    playlist_insert(playlist, 2, &song4);

    playlist_show(playlist);

    playlist_destroy(playlist);

    return 0;
}