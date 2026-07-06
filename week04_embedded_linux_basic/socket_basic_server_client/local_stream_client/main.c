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
    int client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    /* 1. Create socket */
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    /* 2. Prepare server address */
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    /* 3. Connect to server */
    if (connect(client_fd,
                (struct sockaddr *)&addr,
                sizeof(addr)) == -1)
    {
        perror("connect");
        close(client_fd);
        return EXIT_FAILURE;
    }

    printf("Connected to server.\n");

    /* 4. Send message */
    const char *message = "Hello from client";
    if (write(client_fd, message, strlen(message)) == -1)
    {
        perror("write");
        close(client_fd);
        return EXIT_FAILURE;
    }

    /* 5. Receive reply */
    ssize_t n = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (n > 0)
    {
        buffer[n] = '\0';
        printf("Server replied: %s\n", buffer);
    }
    else if (n == -1)
    {
        perror("read");
    }

    /* 6. Cleanup */
    close(client_fd);

    return 0;
}