#include "../nettools.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: udpserver.bin <port>\n");
        return 1;
    }

    uint16_t port = atoi(argv[1]);

    int soc_fd;
    if ((soc_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // bind address to the socket
    if (bind(soc_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        close(soc_fd);
        perror("failed to bind socket\n");
        return 1;
    }

    printf("Listening port: %d\n", port);

    // receive loop
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buf[4096];
    size_t received_size;
    for (;;) {
        received_size = recvfrom(soc_fd, (char*)buf, sizeof(buf), 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (received_size == -1) {
            perror("error: failed to receive message from client");
            return 1;
        }
        printf("Received size: %d\n", received_size);
    }
}
