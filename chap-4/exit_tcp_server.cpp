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
    signal(SIGPIPE, SIG_IGN);

    int fd=socket(AF_INET, SOCK_STREAM, 0);
    if(fd==-1)
    {
        error(errno, "create socket failed");
    }

    int ret=0;
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(SERV_PORT);
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    ret=bind(fd, (struct sockaddr*)(&saddr), sizeof(saddr));
    if(ret!=0)
    {
        error(errno, "bind failed");
    }

    ret=listen(fd, BACKLOG);
    if(ret!=0)
    {
        error(errno, "listen failed");
    }

    struct sockaddr_in caddr;
    socklen_t clen=sizeof(caddr);
    int conn_fd=accept(fd, (struct sockaddr*)(&caddr), &clen);
    if(conn_fd<0)
    {
        error(errno, "accept failed");
    }

    char recv_buffer[MAX_LEN];
    char send_buffer[MAX_LEN];
    while(1)
    {
        int rn=read(conn_fd, recv_buffer, sizeof(recv_buffer));
        if(rn>0)
        {
            recv_buffer[rn-1]='\0';
            printf("server recv data: %s\n", recv_buffer);
        }
        else if(rn==0)
        {
            // EOF
            printf("client closed\n");
            break;
        }
        else
        {
            error(errno, "server read failed");
        }

        sleep(5);

        sprintf(send_buffer, "Hi, %s", recv_buffer);
        rn=write(conn_fd, send_buffer, sizeof(send_buffer));
        if(rn<0)
        {
            error(errno, "server write failed");
        }
    }

    return 0;
}