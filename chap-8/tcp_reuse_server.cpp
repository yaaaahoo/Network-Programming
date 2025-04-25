#include "common/common.h"

using namespace common;

static void handler(int signo)
{
    printf("server recv signo: %d\n", signo);
    exit(0);
}

int main(int argc, char** argv)
{
    // 绑定信号处理函数
    signal(SIGINT, handler);

    // 创建socket
    int ret=0;
    int fd=socket(AF_INET, SOCK_STREAM, 0);
    if(fd==-1)
    {
        error(errno, "create socket error");
    }

    int on=1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    // 绑定到端口
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    saddr.sin_port=htons(SERV_PORT);
    
    ret=bind(fd, (struct sockaddr*)(&saddr), sizeof(saddr));
    if(ret!=0)
    {
        error(errno, "bind failed");
    }

    // 开启监听
    ret=listen(fd, BACKLOG);
    if(ret!=0)
    {
        error(errno, "listen failed");
    }

    // 从全连接队列中取出连接
    struct sockaddr_in caddr;
    socklen_t caddr_len=sizeof(caddr);
    int conn_fd=accept(fd, (struct sockaddr*)(&caddr), &caddr_len);
    if(conn_fd==-1)
    {
        error(errno, "accept failed");
    }

    // 读取数据
    char buffer[MAX_LEN];
    while(true)
    {
        bzero(buffer, sizeof(buffer));
        ssize_t n=read(conn_fd, buffer, sizeof(buffer));
        if(n>0)
        {
            buffer[n-1]='\0';
            printf("server recv data: %s", buffer);
        }
        else if(n==0)
        {
            // EOF
            printf("client closed.\n");
            break;
        }
        else
        {
            // error
            error(errno, "read failed");
        }
    }

    return 0;
}