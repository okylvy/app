#include "../nettools.h"

int main(int argc, char** argv) {
    if(argc < 4) {
        printf("Usage: udpclient.bin <ip addr> <port> <message>\n");
        return 1;
    }

    int soc_fd;
    if ((soc_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in dst_addr;
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_addr.s_addr = inet_addr(argv[1]);
    dst_addr.sin_port = htons(atoi(argv[2]));

    char* buf = argv[3];
    ssize_t sent_size;

    sent_size = sendto(soc_fd, buf, strlen(buf), 0, (struct sockaddr*)&dst_addr, sizeof(dst_addr));
    printf("Sent size: %d\n", sent_size);

    return 0;
}
