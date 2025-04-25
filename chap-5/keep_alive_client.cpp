#include "common/common.h"

using namespace common;

#define KEEP_ALIVE_TIME 10
#define KEEP_ALIVE_INTEVAL 3
#define KEEP_ALIVE_COUNT 3

static void handler(int signo)
{
    printf("client recv signo: %d\n", signo);
    exit(0);
}

int main(int argc, char** argv)
{
    if(argc<2)
    {
        error(1, "client need ip");
    }

    signal(SIGINT, handler);

    int fd=socket(AF_INET, SOCK_STREAM, 0);
    if(fd<0)
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
        error(errno, "connect error");
    }

    fd_set fd_read, fd_mask;
    FD_ZERO(&fd_read);
    FD_SET(fd, &fd_read);

    struct timeval tv;
    tv.tv_sec=KEEP_ALIVE_TIME;
    tv.tv_usec=0;

    int count=0;

    while(1)
    {
        fd_mask=fd_read;
        int rc=select(fd+1, &fd_mask, nullptr, nullptr, &tv);
        if(rc<0)
        {
            error(errno, "select error");
        }
        else if(rc==0)
        {
            // time out
            if(++count>KEEP_ALIVE_COUNT)
            {
                // dead
                error(1, "connect dead...\n");
            }

            common::Message message;
            message.type=common::Message::Type::PING;
            strcpy(message.data, "are you ok?");
            printf("client send #%d heartbreat: %s\n", count, message.data);

            int wn=write(fd, &message, sizeof(message));
            if(wn<0)
            {
                error(errno, "write error");
            }

            tv.tv_sec=KEEP_ALIVE_INTEVAL;
            continue;
        }

        if(FD_ISSET(fd, &fd_mask))
        {
            common::Message message;
            int rn=read(fd, &message, sizeof(message));
            if(rn<0)
            {
                error(errno, "read error");
            }
            else if(rn==0)
            {
                // EOF
                break;
            }
            else
            {
                if(message.type==common::Message::Type::PONG)
                {
                    printf("client recv heartbeats: %s, will reset count to 0.\n", message.data);
                    count=0;
                    tv.tv_sec=KEEP_ALIVE_TIME;
                }
                else
                {
                    printf("client recv other data.\n");
                }
            }
        }
    }


    return 0;
}