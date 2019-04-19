
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define handle_error(msg) \
        do { fprintf(stderr, msg); exit(EXIT_FAILURE); } while (0)

#define BUFFER_SIZE 255

int main(int argc, char *argv[])
{
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
    addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
    addr.sin_port = htons(port);

    u_int yes = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes));

    if (bind(sock, (struct sockaddr*) &addr, sizeof(addr)) < 0)
        handle_error("bind");

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq));

    while (1) {
        char buffer[BUFFER_SIZE];
        uint32_t addrlen = sizeof(addr);
        recvfrom (
            sock,
            buffer,
            BUFFER_SIZE,
            0,
            (struct sockaddr *)&addr,
            &addrlen
        );
        buffer[BUFFER_SIZE] = '\0';
        fprintf(stdout, "\n%s\n", buffer);
     }

    return 0;
}