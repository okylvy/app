#include "../nettools.h"

uint16_t port;

int parse_args(int argc, char **argv) {
    // default value.
    port = 80;

    while (argc > 0) {
        if (strcmp("--port", argv[0]) == 0 || strcmp("-p", argv[0]) == 0) {
                port = atoi(argv[1]);
                argc -= 2;
                argv += 2;
                continue;
        }
        return 0;
    }
    return 1;
}

void build_response(char *response, int status, char *message) {
    // status line
    switch (status) {
        case 200:
            strcpy(response, "HTTP/1.1 200 OK\n");
            break;
        case 404:
            strcpy(response, "HTTP/1.1 404 Not Found\n");
            break;
        default:
            strcpy(response, "HTTP/1.1 500 Internal Server Error\n");
    }

    // header
    strcat(response, "Content-Type: text/html; charset=UTF-8\n");

    // crlf
    strcat(response, "\n");

    // body
    strcat(response, message);
}

void create_page(char *response, char *path) {
    if (strcmp(path, "/") == 0 || strcmp(path, "/index.html") == 0) {
        char *body =
            "<html>\n"
            "   <body>\n"
            "       <h1>Hello World</h1>\n"
            "       <div>\n"
            "           <p>This is a sample\n"
            "           <ul>\n"
            "               <li>List</li>\n"
            "           </ul>\n"
            "       </div>\n"
            "   </body>\n"
            "</html>\n";
        build_response(response, 200, body);
    } else {
        char *body =
            "<html>\n"
            "   <body>\n"
            "       <h1>ERROR!!</h1>\n"
            "       <p>Not Found</p>\n"
            "   </body>\n"
            "</html>\n";
        build_response(response, 404, body);
    }
}

int start_server() {
    int soc_fd;
    int accepted_soc;
    struct sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);

    soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (soc_fd < 0) {
        perror("Error: failed to create a socket.");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = port;

    if (bind(soc_fd, (struct sockaddr *)&addr, addr_len) < 0) {
        perror("Error: failed to bind a socket.");
        return 1;
    }

    if (listen(soc_fd, 3) < 0) {
        perror("Error: failed to listen a socket.");
        return 1;
    }

    while(1) {
        printf("waiting for a request...\n");

        char request[SIZE_REQUEST];
        int size = -1;

        if ((accepted_soc = accept(soc_fd, (struct sockaddr *)&addr, (socklen_t *)&addr_len)) < 0) {
            perror("Error: failed to accept socekt.");
            return 1;
        }
        size = read(accepted_soc, request, SIZE_REQUEST);

        if (size < 0) {
            perror("Error: failed to receive a request.");
            return 1;
        }

        printf("-----request-----\n");
        printf("%s", request);

        char *method = strtok(request, " ");
        char *path = strtok(NULL, " ");
        char *response = (char *)malloc(SIZE_RESPONSE);
        printf("Method: '%s'\n", method);
        printf("Path: '%s'\n\n", path);

        if (strcmp(method, "GET") == 0) {
            create_page(response, path);
        } else {
            build_response(response, 500, "Only GET method is supported.");
        }

        size = -1;
        size = sendto(accepted_soc, response, strlen(response), 0, (struct sockaddr *)&addr, addr_len);
        if (size < 0) {
            perror("Error: failed to send a response.");
            return 1;
        }
        close(accepted_soc);
    }
    close(soc_fd);
 
    return 0;
}


int main(int argc, char **argv) {
    if (!parse_args(argc -1, argv + 1)) {
        printf("Usage: httpserver.bin [ OPTIONS ]\n");
        printf("    -p, --port    Port number. Default:80\n");
        return 1;
    }

    printf("listening port %d...\n", port);
    if (start_server() != 0) {
        return 1;
    }

    return 0;
}