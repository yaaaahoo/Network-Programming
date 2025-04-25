#include "common/common.h"

using namespace common;

static void handler(int signo)
{
    printf("server recv signo: %d\n", signo);
    exit(0);
}

int main(int argc, char** argv)
{
    if(argc<2)
    {
        error(1, "need sock path");
    }

    // 绑定信号处理函数
    signal(SIGINT, handler);

    int fd=socket(AF_LOCAL, SOCK_STREAM, 0);
    if(fd==-1)
    {
        error(errno, "create socket failed");
    }

    struct sockaddr_un saddr;
    saddr.sun_family=AF_LOCAL;
    strcpy(saddr.sun_path, argv[1]);
    unlink(saddr.sun_path);

    int ret=bind(fd, (struct sockaddr*)(&saddr), sizeof(saddr));
    if(ret!=0)
    {
        error(errno, "bind error");
    }

    ret=listen(fd, BACKLOG);
    if(ret!=0)
    {
        error(errno, "listen error");
    }

    char buffer[MAX_LEN];
    struct sockaddr_in caddr;
    socklen_t caddr_len=sizeof(caddr);

    int conn_fd=accept(fd, (struct sockaddr*)(&caddr), &caddr_len);
    if(conn_fd==-1)
    {
        error(errno, "accept failed");
    }

    while(true)
    {
        bzero(buffer, sizeof(buffer));
        int n=read(conn_fd, buffer, sizeof(buffer));
        if(n>0)
        {
            printf("server recv data: %s\n", buffer);
        }
        else if(n==0)
        {
            // EOF
            break;
        }
        else
        {
            error(errno, "read error");
        }
    }
    
    return 0;
}