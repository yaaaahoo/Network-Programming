#include "common/common.h"

static int count = 0;

static void RecvFromInt(int singo)
{
    printf("\nrecieved %d datagrams\n", count);
    exit(0);
}

int main(int argc, char** argv)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr*)(&servaddr), (socklen_t)(sizeof(servaddr)));

    signal(SIGINT, RecvFromInt);

    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    char buffer[MAX_LINE];

    for (;;)
    {
        int n = recvfrom(sockfd, buffer, MAX_LINE, 0, (struct sockaddr*)(&cliaddr), &clilen);
        buffer[n] = '\0';
        printf("recv %d bytes: %s\n", n, buffer);

        char send_line[MAX_LINE];
        sprintf(send_line, "Hi, %s\n", buffer);
        sendto(sockfd, send_line, strlen(send_line), 0, (struct sockaddr*)(&cliaddr), clilen);

        count++;
    }

    return 0;
}