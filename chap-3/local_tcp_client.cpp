#include "common/common.h"

using namespace common;

static void handler(int signo)
{
    printf("client recv signo: %d\n", signo);
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

    struct sockaddr_un caddr;
    caddr.sun_family=AF_LOCAL;
    strcpy(caddr.sun_path, argv[1]);

    int ret=connect(fd, (struct sockaddr*)(&caddr), sizeof(caddr));
    if(ret!=0)
    {
        error(errno, "connect failed");
    }

    char buffer[MAX_LEN];
    while(true)
    {
        bzero(buffer, sizeof(buffer));
        if(fgets(buffer, sizeof(buffer), stdin)!=nullptr)
        {
            int len=strlen(buffer);
            if(buffer[len-1]=='\n')
            {
                buffer[len-1]='\0';
            }
            
            ssize_t n=write(fd, buffer, sizeof(buffer));
            if(n<0)
            {
                error(errno, "client write error");
            }
            printf("client send data: %s\n", buffer);
        }
    }

    return 0;
}