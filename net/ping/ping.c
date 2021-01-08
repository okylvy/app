#include "../nettools.h"

struct __attribute__((packed)) ICMPMessage {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t sequence;
};

uint16_t CalcCheckSum(void* buf, size_t start, size_t end) {
    // RFC1071
    uint8_t* p = buf;
    uint32_t sum = 0;
    for (size_t i = start; i < end; i += 2) {
        sum += ((uint16_t)p[i + 0]) << 8 | p[i + 1];
    }
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    sum = ~sum;
    return ((sum >> 8) & 0xFF) | ((sum & 0xFF) << 8);
}

int main(int argc, char** argv) {
    int soc;
    struct sockaddr_in addr;
    struct icmphdr hdr;
    char recv_buf[BUFFER_SIZE];
    struct icmphdr *icmphdr_ptr;
    struct iphdr *iphdr_ptr;

    if (argc != 2) {
        printf("Usage: ");
        printf(argv[0]);
        printf(" <ip addr>\n");
        return 1;
    }

    printf("Ping to ");
    printf(argv[1]);
    printf(" ...\n");

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    // Create ICMP RAW socket.
    soc = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (soc < 0) {
        perror("socket() failed");
        return 1;
    }

    // Create ICMP header
    memset(&hdr, 0, sizeof(hdr));
    hdr.type = ICMP_ECHO;
    hdr.code = 0;
    hdr.checksum = CalcCheckSum(&hdr, 0, sizeof(hdr));
    hdr.un.echo.id = 0;
    hdr.un.echo.sequence = 0;

    // Send ICMP
    int sendto_res = sendto(soc, (char *)&hdr, sizeof(hdr), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (sendto_res < 1) {
        perror("sendto() failed");
    }

    memset(recv_buf, 0, sizeof(recv_buf));
    // Receive reply
    int recv_res = recv(soc, recv_buf, sizeof(recv_buf), 0);
    if (recv_res < 1) {
        perror("recv() failed");
    }

    // Get pointer to the part of IP header.
    iphdr_ptr = (struct iphdr *)recv_buf;

    // Get pointer to the part of ICMP header.
    icmphdr_ptr = (struct icmphdr *)(recv_buf + (iphdr_ptr->ihl * 4));

    if (icmphdr_ptr->type == ICMP_ECHOREPLY) {
        printf("received echo reply from %s\n", argv[1]);
    } else {
        printf("received ICMP %d\n", icmphdr_ptr->type);
    }

    close(soc);

    return 0;
}
