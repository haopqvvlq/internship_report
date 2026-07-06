#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/local_stream.sock"
#define BUFFER_SIZE 256

int main(void)
{
    int server_fd;
    int client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    /* 1. Create socket */
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    /* Remove old socket file if it exists */
    unlink(SOCKET_PATH);

    /* 2. Prepare address */
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    /* 3. Bind */
    if (bind(server_fd,
             (struct sockaddr *)&addr,
             sizeof(addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    /* 4. Listen */
    if (listen(server_fd, 5) == -1)
    {
        perror("listen");
        close(server_fd);
        unlink(SOCKET_PATH);
        return EXIT_FAILURE;
    }

    printf("Server is listening...\n");

    /* 5. Accept */
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1)
    {
        perror("accept");
        close(server_fd);
        unlink(SOCKET_PATH);
        return EXIT_FAILURE;
    }

    printf("Client connected.\n");

    /* 6. Read from client */
    ssize_t n = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (n > 0)
    {
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);

        /* 7. Reply */
        const char *reply = "Hello from server";
        write(client_fd, reply, strlen(reply));
    }

    /* 8. Cleanup */
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}