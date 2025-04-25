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
        error(1, "client: need ip");
    }

    // 绑定信号处理函数
    signal(SIGINT, handler);

    int fd=socket(AF_INET, SOCK_STREAM, 0);
    if(fd==-1)
    {
        error(errno, "create socket failed");
    }

    struct sockaddr_in caddr;
    caddr.sin_family=AF_INET;
    caddr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &caddr.sin_addr.s_addr);

    int ret=connect(fd, (struct sockaddr*)(&caddr), sizeof(caddr));
    if(ret!=0)
    {
        error(errno, "connect failed");
    }

    fd_set allread, mask;
    FD_ZERO(&allread);
    FD_SET(0, &allread);
    FD_SET(fd, &allread);

    char send_buffer[MAX_LEN];
    char recv_buffer[MAX_LEN];

    while(1)
    {
        mask=allread;
        int n=select(fd+1, &mask, nullptr, nullptr, nullptr);
        if(n<=0)
        {
            error(errno, "select error");
        }
        if(FD_ISSET(fd, &mask))
        {
            int rn=read(fd, recv_buffer, sizeof(recv_buffer));
            if(rn>0)
            {
                recv_buffer[rn-1]='\0';
                printf("client recv data: %s\n", recv_buffer);
            }
            else if(rn==0)
            {
                // EOF
                printf("server closed\n");
                break;
            }
            else
            {
                // error
                error(errno, "client read error");
            }
        }
        if(FD_ISSET(0, &mask))
        {
            bzero(send_buffer, sizeof(send_buffer));
            if(fgets(send_buffer, sizeof(send_buffer), stdin)!=nullptr)
            {
                if(strncmp(send_buffer, "shutdown", 8)==0)
                {
                    FD_CLR(0, &allread);
                    if(shutdown(fd, SHUT_WR))
                    {
                        error(errno, "shutdown error");
                    }
                }
                else if(strncmp(send_buffer, "close", 5)==0)
                {
                    FD_CLR(0, &allread);
                    if(close(fd))
                    {
                        error(errno, "close error");
                    }

                    sleep(10);
                    break;
                }
                else
                {
                    // 发送数据给服务端
                    int len=strlen(send_buffer);
                    if(send_buffer[len-1]=='\n')
                    {
                        send_buffer[len-1]='\0';
                    }

                    int wn=write(fd, send_buffer, sizeof(send_buffer));
                    if(wn==-1)
                    {
                        error(errno, "client write error");
                    }
                }
            }
        }
    }

    return 0;
}