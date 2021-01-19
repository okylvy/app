#include "../nettools.h"

char *addr;
uint16_t port;
char *host;
char *path;

int parse_args(int argc, char **argv) {
    // default values.
    addr = "127.0.0.1";
    port = 80;
    host = "";
    path = "/";

    while (argc > 0) {
        if (strcmp("--addr", argv[0]) == 0 || strcmp("-a", argv[0]) == 0) {
                addr = argv[1];
                argc -= 2;
                argv += 2;
                continue;
        }
        if (strcmp("--port", argv[0]) == 0 || strcmp("-p", argv[0]) == 0) {
                port = atoi(argv[1]);
                argc -= 2;
                argv += 2;
                continue;
        }
        if (strcmp("--host", argv[0]) == 0 || strcmp("-h", argv[0]) == 0) {
                host = argv[1];
                argc -= 2;
                argv += 2;
                continue;
        }
        if (strcmp("--path", argv[0]) == 0 || strcmp("-P", argv[0]) == 0) {
                host = argv[1];
                argc -= 2;
                argv += 2;
                continue;
        }
        return 0;
    }
    return 1;
}

void add_request_line(char *request) {
// TODO: use strncpy and strncat.
    strcpy(request, "GET ");
    strcat(request, path);
    strcat(request, " HTTP/1.1\n");
}

void add_headers(char *request) {
    strcat(request, "Host: ");
    strcat(request, host);
    strcat(request, "\n");
}

void add_crlf(char *request) {
    strcat(request, "\n");
}

void add_body(char *request) {
// TODO: add body.
}

void send_request(char *request) {
// TODO: implement udp socket connection.
    int soc_fd = 0;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    char response[SIZE_RESPONSE];


}

int main(int argc, char **argv) {
    if (!parse_args(argc -1, argv + 1)) {
        printf("Usage: httpclient.bin [ OPTIONS ]\n");
        printf("    -a, --addr    IP address. Default: 127.0.0.1\n");
        printf("    -p, --port    Port number. Default:80\n");
        printf("    -h, --host    Host property of the URL. Default: 0\n");
        printf("    -P, --path    Path property of the URL. Defualt: /\n");
        return 1;
    }

    char *request = (char *) malloc(SIZE_REQUEST);

    add_request_line(request);
    add_headers(request);
    add_crlf(request);
    add_body(request);

    printf("start to HTTP request...\n");
    printf(request);

    send_request(request);

    return 0;
}
