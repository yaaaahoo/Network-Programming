#include "common/common.h"

void ReadData(int sockfd)
{
    size_t n;
    char buf[1024];
    int time = 0;
    for (;;)
    {
        fprintf(stdout, "block in read\n");
        if ((n = common::readn(sockfd, buf, 1024)) == 0)
        {
            return;
        }

        time++;
        fprintf(stdout, "1K read for %d\n", time);
        usleep(1000);
    }
}

int main()
{
    int listenfd, connfd;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr*)(&servaddr), sizeof(servaddr));

    listen(listenfd, 1024);

    socklen_t clilen = sizeof(cliaddr);

    // 循环处理用户请求
    for (;;)
    {
        connfd = accept(listenfd, (struct sockaddr*)(&cliaddr), &clilen);
        ReadData(connfd);
        close(connfd);
    }
}