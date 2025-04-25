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

    int fd=socket(AF_INET, SOCK_DGRAM, 0);
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

    char send_buffer[MAX_LEN];
    char recv_buffer[MAX_LEN];
    struct sockaddr_in caddr;
    socklen_t c_len=sizeof(caddr);
    while(true)
    {
        bzero(recv_buffer, sizeof(recv_buffer));
        bzero(send_buffer, sizeof(send_buffer));

        int rn=recvfrom(fd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr*)(& caddr), &c_len);
        if(rn>0)
        {
            printf("server recv data: %s", recv_buffer);
        }
        else if(rn==0)
        {
            break;
        }
        else
        {
            error(errno, "recvfrom error");
        }

        std::string ss=std::string("Hi, ")+std::string(recv_buffer);
        strcpy(send_buffer, ss.c_str());

        int sn=sendto(fd, send_buffer, sizeof(send_buffer), 0, (struct sockaddr*)(&caddr), c_len);
        if(sn>0)
        {
            printf("server send data: %s", send_buffer);
        }
        else if(sn==0)
        {
            break;
        }
        else
        {
            error(errno, "sendto error");
        }
    }

    return 0;
}