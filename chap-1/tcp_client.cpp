#include "common/common.h"

using namespace common;

static void handler(int signo)
{
    printf("server recv signo: %d\n", signo);
    exit(0);
}

int main(int argc, char** argv)
{
    if(argc!=2)
    {
        error(0, "client: need ip");
    }

    // 绑定信号处理函数
    signal(SIGINT, handler);

    // 创建socket
    int fd=socket(AF_INET, SOCK_STREAM, 0);
    if(fd==-1)
    {
        error(errno, "create socket failed");
    }

    // connect
    int ret=0;
    struct sockaddr_in caddr;
    caddr.sin_family=AF_INET;
    caddr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &caddr.sin_addr.s_addr);

    ret=connect(fd, (struct sockaddr*)&caddr, sizeof(caddr));
    if(ret==-1)
    {
        error(errno, "connect failed");
    }

    // 发送数据
    char buffer[MAX_LEN];
    while(true)
    {
        bzero(buffer, sizeof(buffer));
        if(fgets(buffer, sizeof(buffer), stdin)!=nullptr)
        {
            int b_l=strlen(buffer);
            if(buffer[b_l-1]=='\n')
            {
                buffer[b_l]='\0';
            }

            ssize_t n=write(fd, buffer, sizeof(buffer));
            if(n<0)
            {
                error(errno, "client write error");
            }
            else
            {
                printf("client send data: %s", buffer);
            }
        }
    }

    return 0;
}