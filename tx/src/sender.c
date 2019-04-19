#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 255

#define handle_error(msg) \
        do { fprintf(stderr, msg); exit(EXIT_FAILURE); } while (0)

// ./prog group port

int main(int argc, char **argv) {
    if (argc != 3)
       handle_error("args: [group] [port]\n");

    char *group = argv[1];
    uint16_t port = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) 
        handle_error("invalid socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(group);
    addr.sin_port = htons(port);

    while (1) {
        char buffer[BUFFER_SIZE];
        fgets(buffer, BUFFER_SIZE, stdin);
        sendto(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&addr, sizeof(addr));
    }
    return 0;
}