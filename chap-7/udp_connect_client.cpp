#include "common/common.h"

using namespace common;

static void handler(int signo)
{
    printf("client recv signo: %d\n", signo);
    exit(0);
}

int main(int argc, char** argv)
{
    if(argc!=2)
    {
        error(0, "client: need ip");
    }

    signal(SIGINT, handler);

    int fd=socket(AF_INET, SOCK_DGRAM, 0);
    if(fd==-1)
    {
        error(errno, "create socket failed");
    }

    char send_buffer[MAX_LEN];
    char recv_buffer[MAX_LEN];

    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &saddr.sin_addr.s_addr);

    // 连接，主要是为了绑定信息
    int ret=connect(fd, (struct sockaddr*)(&saddr), sizeof(saddr));
    if(ret!=0)
    {
        error(1, "connect failed");
    }
    

    struct sockaddr_in caddr;
    socklen_t c_len=sizeof(caddr);
    while(true)
    {
        bzero(send_buffer, sizeof(send_buffer));
        bzero(recv_buffer, sizeof(recv_buffer));

        if(fgets(send_buffer, sizeof(send_buffer), stdin)!=nullptr)
        {
            int sn=sendto(fd, send_buffer, sizeof(send_buffer), 0, (struct sockaddr*)(&saddr), sizeof(saddr));
            if(sn>0)
            {
                printf("client send data: %s", send_buffer);
            }
            else if(sn==0)
            {
                break;
            }
            else
            {
                error(errno, "sendto error");
            }

            int rn=recvfrom(fd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr*)(&caddr), &c_len);
            if(rn>0)
            {
                printf("client recv data: %s", recv_buffer);
            }
            else if(rn==0)
            {
                break;
            }
            else
            {
                error(errno, "recvfrom error");
            }
        }
    }

    return 0;
}