#include "common/common.h"

void ReadData(int sockfd)
{
    size_t n;
    char buf[1024];
    int time = 0;
    for (;;)
    {
        std::cout << "block in read" << std::endl;
        n = common::readn(sockfd, buf, 1024);
        if (n == 0)
        {
            return;
        }

        time++;
        std::cout << "1K read for " << time << std::endl;
        usleep(1000);
    }
}

int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr*)(&servaddr), sizeof(servaddr));

    listen(listenfd, BACKLOG);

    int connfd;
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);

    // 循环处理用户请求
    for (;;)
    {
        connfd = accept(listenfd, (struct sockaddr*)(&cliaddr), &clilen);
        ReadData(connfd);
        close(connfd);
    }

    return 0;
}