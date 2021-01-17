#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 2000
#define SIZE_REQUEST 1000

typedef uint32_t socklen_t;

uint16_t CalcCheckSumi(void* buf, size_t start, size_t end);
