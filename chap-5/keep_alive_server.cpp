#include "common/common.h"

using namespace common;

static int sleep_time=0;

static void handler(int signo)
{
    printf("server recv signo: %d\n", signo);
    exit(0);
}

int main(int argc, char** argv)
{
    if(argc<2)
    {
        error(1, "need sleeptime");
    }

    signal(SIGINT, handler);

    sleep_time=atoi(argv[1]);

    int fd=socket(AF_INET, SOCK_STREAM, 0);
    if(fd<0)
    {
        error(errno, "create socket failed");
    }

    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(SERV_PORT);
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    
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

    struct sockaddr_in caddr;
    socklen_t clen=sizeof(caddr);
    int conn_fd=accept(fd, (struct sockaddr*)(&caddr), &clen);
    if(conn_fd<0)
    {
        error(errno, "accept error");
    }


    common::Message message;
    while(1)
    {
        int rn=read(conn_fd, &message, sizeof(message));
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
            switch (message.type)
            {
            case common::Message::Type::TYPE1:
                printf("process type1 data.\n");
                sleep(1);
                break;
            case common::Message::Type::TYPE2:
                printf("process type2 data.\n");
                sleep(1);
                break;
            case common::Message::Type::PING:
            {
                printf("server recv ping data: %s\n", message.data);
                
                // pong
                common::Message ack_msg;
                ack_msg.type=common::Message::Type::PONG;
                strcpy(ack_msg.data, "i am ok.");
                sleep(sleep_time);
                int wn=write(conn_fd, &ack_msg, sizeof(ack_msg));
                if(wn<0)
                {
                    error(errno, "write error");
                }
                break;
            }
            default:
                error(1, "unknow type.\n");
                break;
            }
        }
    }

    return 0;
}